
#include "elastos/droid/webkit/AccessibilityInjector.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
//#include "elastos/droid/provider/CSettingsSecure.h"
#include "elastos/droid/speech/tts/CTextToSpeech.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/SystemClock.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/Thread.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::Thread;
using Elastos::Droid::Content::IContentResolver;
//using Elastos::Droid::Provider::CSettingsSecure;
//using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Speech::Tts::CTextToSpeech;
using Elastos::Droid::Speech::Tts::ITextToSpeechEngine;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::SystemClock;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//             AccessibilityInjector::TextToSpeechWrapper
//===============================================================

const String AccessibilityInjector::TextToSpeechWrapper::WRAP_TAG("TextToSpeechWrapper");

AccessibilityInjector::TextToSpeechWrapper::TextToSpeechWrapper(
    /* [in] */ IContext* context)
{
//    if (DEBUG) {
//        Log.d(WRAP_TAG, "[" + hashCode() + "] Initializing text-to-speech on thread "
//                + Thread.currentThread().getId() + "...");
//    }

    String pkgName;
    context->GetPackageName(&pkgName);

    mReady = FALSE;
    mShutdown = FALSE;

    mTtsParams[ITextToSpeechEngine::KEY_PARAM_UTTERANCE_ID] = WRAP_TAG;

    CTextToSpeech::New(
            context, mInitListener, String(NULL), pkgName + String(".**webview**"), TRUE, (ITextToSpeech**)&mTextToSpeech);

    Int32 result;
    mTextToSpeech->SetOnUtteranceProgressListener(mErrorListener, &result);
}

CAR_INTERFACE_IMPL(AccessibilityInjector::TextToSpeechWrapper, IInterface);

Boolean AccessibilityInjector::TextToSpeechWrapper::IsSpeaking()
{
    AutoLock lock(mTextToSpeechMutex);

    if (!mReady) {
        return FALSE;
    }

    Boolean bRet = FALSE;
    mTextToSpeech->IsSpeaking(&bRet);
    return bRet;
}

Int32 AccessibilityInjector::TextToSpeechWrapper::Speak(
    /* [in] */ const String& text,
    /* [in] */ Int32 queueMode,
    /* [in] */ HashMap<String, String>* params)
{
    AutoLock lock(mTextToSpeechMutex);

    if (!mReady) {
//        if (DEBUG) {
//            Log.w(WRAP_TAG, "[" + hashCode() + "] Attempted to speak before TTS init");
//        }
        return ITextToSpeech::TTS_ERROR;
    }
    else {
//        if (DEBUG) {
//            Log.i(WRAP_TAG, "[" + hashCode() + "] Speak called from JS binder");
//        }
    }

    Int32 ret = -1;
//    mTextToSpeech->Speak(text, queueMode, params, &ret);
    return ret;
}

Int32 AccessibilityInjector::TextToSpeechWrapper::Stop()
{
    AutoLock lock(mTextToSpeechMutex);

    if (!mReady) {
//        if (DEBUG) {
//            Log.w(WRAP_TAG, "[" + hashCode() + "] Attempted to stop before initialize");
//        }
        return ITextToSpeech::TTS_ERROR;
    }
    else {
//        if (DEBUG) {
//            Log.i(WRAP_TAG, "[" + hashCode() + "] Stop called from JS binder");
//        }
    }

    Int32 ret = -1;
    mTextToSpeech->Stop(&ret);
    return ret;
}

void AccessibilityInjector::TextToSpeechWrapper::Shutdown()
{
    AutoLock lock(mTextToSpeechMutex);
/*
    if (!mReady) {
        if (DEBUG) {
            Log.w(WRAP_TAG, "[" + hashCode() + "] Called shutdown before initialize");
        }
    } else {
        if (DEBUG) {
            Log.i(WRAP_TAG, "[" + hashCode() + "] Shutting down text-to-speech from "
                    + "thread " + Thread.currentThread().getId() + "...");
        }
    }*/

    mShutdown = TRUE;
    mReady = FALSE;
    mTextToSpeech->Shutdown();
}

//===============================================================
//             AccessibilityInjector::CallbackHandler
//===============================================================

const String AccessibilityInjector::CallbackHandler::JAVASCRIPT_ACTION_TEMPLATE("javascript:(function() { %s.onResult(%d, %s); })();");
const Int64 AccessibilityInjector::CallbackHandler::RESULT_TIMEOUT;

AccessibilityInjector::CallbackHandler::CallbackHandler(
    /* [in] */ const String& interfaceName)
    : mInterfaceName(interfaceName)
    , mResult(FALSE)
    , mResultId(-1)
{
    CAtomicInteger32::New((IAtomicInteger32**)&mResultIdCounter);
    CHandler::New((IHandler**)&mMainHandler);
}

CAR_INTERFACE_IMPL_LIGHT(AccessibilityInjector::CallbackHandler, IInterface)

/**
 * Performs an action and attempts to wait for a result.
 *
 * @param webView The WebView to perform the action on.
 * @param code JavaScript code that evaluates to a result.
 * @return The result of the action, or false if it timed out.
 */
Boolean AccessibilityInjector::CallbackHandler::PerformAction(
    /* [in] */ IWebView* webView,
    /* [in] */ const String& code)
{
    Int32 resultId;
    mResultIdCounter->GetAndIncrement(&resultId);

    String url;
    url.AppendFormat(JAVASCRIPT_ACTION_TEMPLATE, (const char*)mInterfaceName, resultId, (const char*)code);
    webView->LoadUrl(url);
    return GetResultAndClear(resultId);
}

/**
 * Gets the result of a request to perform an accessibility action.
 *
 * @param resultId The result id to match the result with the request.
 * @return The result of the request.
 */
Boolean AccessibilityInjector::CallbackHandler::GetResultAndClear(
    /* [in] */ Int32 resultId)
{
    AutoLock lock(mResultLock);

    Boolean success = WaitForResultTimedLocked(resultId);
    Boolean result = success ? mResult : FALSE;
    ClearResultLocked();
    return result;
}

/**
 * Clears the result state.
 */
void AccessibilityInjector::CallbackHandler::ClearResultLocked()
{
    mResultId = -1;
    mResult = FALSE;
}

/**
 * Waits up to a given bound for a result of a request and returns it.
 *
 * @param resultId The result id to match the result with the request.
 * @return Whether the result was received.
 */
Boolean AccessibilityInjector::CallbackHandler::WaitForResultTimedLocked(
    /* [in] */ Int32 resultId)
{
    Int64 startTimeMillis = SystemClock::GetUptimeMillis();

//    if (DEBUG) {
//        Log.d(TAG, "Waiting for CVOX result with ID " + resultId + "...");
//    }

    while (TRUE) {
        // Fail if we received a callback from the future.
        if (mResultId > resultId) {
//            if (DEBUG) {
//                Log.w(TAG, "Aborted CVOX result");
//            }
            return FALSE;
        }

        Int64 elapsedTimeMillis = SystemClock::GetUptimeMillis() - startTimeMillis;

        // Succeed if we received the callback we were expecting.
//        if (DEBUG) {
//            Log.w(TAG, "Check " + mResultId + " versus expected " + resultId);
//        }
        if (mResultId == resultId) {
//            if (DEBUG) {
//                Log.w(TAG, "Received CVOX result after " + elapsedTimeMillis + " ms");
//            }
            return TRUE;
        }

        Int64 waitTimeMillis = (RESULT_TIMEOUT - elapsedTimeMillis);

        // Fail if we've already exceeded the timeout.
        if (waitTimeMillis <= 0) {
//            if (DEBUG) {
//                Log.w(TAG, "Timed out while waiting for CVOX result");
//            }
            return FALSE;
        }

//        try {
//            if (DEBUG) {
//                Log.w(TAG, "Start waiting...");
//            }
        AutoPtr<IThread> thread;
        Thread::Attach((IThread**)&thread);
        {
            AutoLock lock(mResultLock);
            mResultLock.Wait(waitTimeMillis);
        }

//        } catch (InterruptedException ie) {
//            if (DEBUG) {
//                Log.w(TAG, "Interrupted while waiting for CVOX result");
//            }
//        }
    }
}

/**
 * Callback exposed to JavaScript. Handles returning the result of a
 * request to a waiting (or potentially timed out) thread.
 *
 * @param id The result id of the request as a {@link String}.
 * @param result The result of the request as a {@link String}.
 */
void AccessibilityInjector::CallbackHandler::OnResult(
    /* [in] */ const String& id,
    /* [in] */ const String& result)
{
//    if (DEBUG) {
//        Log.w(TAG, "Saw CVOX result of '" + result + "' for ID " + id);
//    }
    Int32 resultId;

//    try {
//        resultId = Integer.parseInt(id);
//    } catch (NumberFormatException e) {
//        return;
//    }

    AutoPtr<IThread> thread;
    Thread::Attach((IThread**)&thread);
    {
        AutoLock lock(mResultLock);
        if (resultId > mResultId) {
            mResult = result.EqualsIgnoreCase("TRUE");
            mResultId = resultId;
        }
        else {
//            if (DEBUG) {
//                Log.w(TAG, "Result with ID " + resultId + " was stale vesus " + mResultId);
//            }
        }
        mResultLock.NotifyAll();
    }
}

/**
 * Requests a callback to ensure that the JavaScript interface for this
 * object has been added successfully.
 *
 * @param webView The web view to request a callback from.
 * @param callbackRunnable Runnable to execute if a callback is received.
 */
void AccessibilityInjector::CallbackHandler::RequestCallback(
    /* [in] */ IWebView* webView,
    /* [in] */ IRunnable* callbackRunnable)
{
    mCallbackRunnable = callbackRunnable;

    StringBuilder sb("javascript:(function() { ");
    sb += mInterfaceName;
    sb += ".callback(); })();";
    webView->LoadUrl(sb.ToString());
}

void AccessibilityInjector::CallbackHandler::Callback()
{
    if (mCallbackRunnable != NULL) {
        Boolean result;
        mMainHandler->Post(mCallbackRunnable, &result);
        mCallbackRunnable = NULL;
    }
}

//===============================================================
//          AccessibilityInjector::InjectScriptRunnable
//===============================================================
ECode AccessibilityInjector::InjectScriptRunnable::Run()
{
//    if (DEBUG) {
//        Log.d(TAG, "[" + mWebView.hashCode() + "] Received callback");
//    }

    mOwner->InjectJavaScript();
    return NOERROR;
}

//===============================================================
//                    AccessibilityInjector
//===============================================================
const String AccessibilityInjector::TAG("AccessibilityInjector");

Boolean AccessibilityInjector::DEBUG;

// Constants for determining script injection strategy.
const Int32 AccessibilityInjector::ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED;
const Int32 AccessibilityInjector::ACCESSIBILITY_SCRIPT_INJECTION_OPTED_OUT;
//@SuppressWarnings("unused")
const Int32 AccessibilityInjector::ACCESSIBILITY_SCRIPT_INJECTION_PROVIDED;

// Alias for TTS API exposed to JavaScript.
const String AccessibilityInjector::ALIAS_TTS_JS_INTERFACE("accessibility");

// Alias for traversal callback exposed to JavaScript.
const String AccessibilityInjector::ALIAS_TRAVERSAL_JS_INTERFACE("accessibilityTraversal");

// Template for JavaScript that injects a screen-reader.
const String AccessibilityInjector::ACCESSIBILITY_SCREEN_READER_JAVASCRIPT_TEMPLATE(
            "javascript:(function() "
            "{    var chooser = document.createElement('script');   "
            "chooser.type = 'text/javascript';   "
            "chooser.src = '%1s';    "
            "document.getElementsByTagName('head')[0].appendChild(chooser);  })();");

// Template for JavaScript that performs AndroidVox actions.
const String AccessibilityInjector::ACCESSIBILITY_ANDROIDVOX_TEMPLATE(
            "(function() "
            "{  if ((typeof(cvox) != 'undefined')      "
            "&& (cvox != null)     "
            "&& (typeof(cvox.ChromeVox) != 'undefined')      "
            "&& (cvox.ChromeVox != null)      "
            "&& (typeof(cvox.AndroidVox) != 'undefined')      "
            "&& (cvox.AndroidVox != null)      "
            "&& cvox.ChromeVox.isActive) "
            "{    return cvox.AndroidVox.performAction('%1s');  } "
            "else {    return false;  }})()");

// JS code used to shut down an active AndroidVox instance.
const String AccessibilityInjector::TOGGLE_CVOX_TEMPLATE(
            "javascript:(function() "
            "{  if ((typeof(cvox) != 'undefined')      "
            "&& (cvox != null)      "
            "&& (typeof(cvox.ChromeVox) != 'undefined')      "
            "&& (cvox.ChromeVox != null)      "
            "&& (typeof(cvox.ChromeVox.host) != 'undefined')      "
            "&& (cvox.ChromeVox.host != null)) "
            "{    cvox.ChromeVox.host.activateOrDeactivateChromeVox(%b);  }})();");

/**
 * Creates an instance of the AccessibilityInjector based on
 * {@code webViewClassic}.
 *
 * @param webViewClassic The WebViewClassic that this AccessibilityInjector
 *            manages.
 */
AccessibilityInjector::AccessibilityInjector(
    /* [in] */ CWebViewClassic* webViewClassic)
    : mAccessibilityScriptInjected(FALSE)
{
    mWebViewClassic = webViewClassic;
    webViewClassic->GetWebView((IWebView**)&mWebView);
//    webViewClassic->GetContext((IContext**)&mContext);
//    mAccessibilityManager = AccessibilityManager.getInstance(mContext);
}

/**
 * If JavaScript is enabled, pauses or resumes AndroidVox.
 *
 * @param enabled Whether feedback should be enabled.
 */
void AccessibilityInjector::ToggleAccessibilityFeedback(
    /* [in] */ Boolean enabled)
{
    if (!IsAccessibilityEnabled() || !IsJavaScriptEnabled()) {
        return;
    }

    ToggleAndroidVox(enabled);

    if (!enabled && (mTextToSpeech != NULL)) {
        mTextToSpeech->Stop();
    }
}

/**
 * Attempts to load scripting interfaces for accessibility.
 * <p>
 * This should only be called before a page loads.
 */
void AccessibilityInjector::AddAccessibilityApisIfNecessary()
{
    if (!IsAccessibilityEnabled() || !IsJavaScriptEnabled()) {
        return;
    }

    AddTtsApis();
    AddCallbackApis();
}

/**
 * Destroys this accessibility injector.
 */
void AccessibilityInjector::Destroy()
{
    if (mTextToSpeech != NULL) {
        mTextToSpeech->Shutdown();
        mTextToSpeech = NULL;
    }

    if (mCallback != NULL) {
        mCallback = NULL;
    }
}

void AccessibilityInjector::ToggleAndroidVox(
    /* [in] */ Boolean state)
{
    if (!mAccessibilityScriptInjected) {
        return;
    }

    String code;
    code.AppendFormat(TOGGLE_CVOX_TEMPLATE, state);
    mWebView->LoadUrl(code);
}

/**
 * Initializes an {@link AccessibilityNodeInfo} with the actions and
 * movement granularity levels supported by this
 * {@link AccessibilityInjector}.
 * <p>
 * If an action identifier is added in this method, this
 * {@link AccessibilityInjector} should also return {@code true} from
 * {@link #supportsAccessibilityAction(int)}.
 * </p>
 *
 * @param info The info to initialize.
 * @see View#onInitializeAccessibilityNodeInfo(AccessibilityNodeInfo)
 */
void AccessibilityInjector::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    info->SetMovementGranularities(IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_CHARACTER
            | IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_WORD
            | IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_LINE
            | IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_PARAGRAPH
            | IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_PAGE);

    info->AddAction(IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY);
    info->AddAction(IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY);
    info->AddAction(IAccessibilityNodeInfo::ACTION_NEXT_HTML_ELEMENT);
    info->AddAction(IAccessibilityNodeInfo::ACTION_PREVIOUS_HTML_ELEMENT);
    info->AddAction(IAccessibilityNodeInfo::ACTION_CLICK);
    info->SetClickable(TRUE);
}

/**
 * Returns {@code true} if this {@link AccessibilityInjector} should handle
 * the specified action.
 *
 * @param action An accessibility action identifier.
 * @return {@code true} if this {@link AccessibilityInjector} should handle
 *         the specified action.
 */
Boolean AccessibilityInjector::SupportsAccessibilityAction(
    /* [in] */ Int32 action)
{
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY:
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY:
        case IAccessibilityNodeInfo::ACTION_NEXT_HTML_ELEMENT:
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_HTML_ELEMENT:
        case IAccessibilityNodeInfo::ACTION_CLICK:
            return TRUE;
        default:
            return FALSE;
    }
}

/**
 * Performs the specified accessibility action.
 *
 * @param action The identifier of the action to perform.
 * @param arguments The action arguments, or {@code null} if no arguments.
 * @return {@code true} if the action was successful.
 * @see View#performAccessibilityAction(int, Bundle)
 */
Boolean AccessibilityInjector::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if (!IsAccessibilityEnabled()) {
        mAccessibilityScriptInjected = FALSE;
        ToggleFallbackAccessibilityInjector(FALSE);
        return FALSE;
    }

    if (mAccessibilityScriptInjected) {
        return SendActionToAndroidVox(action, arguments);
    }

    if (mAccessibilityInjectorFallback != NULL) {
        return mAccessibilityInjectorFallback->PerformAccessibilityAction(action, arguments);
    }

    return FALSE;
}

/**
 * Attempts to handle key events when accessibility is turned on.
 *
 * @param event The key event to handle.
 * @return {@code true} if the event was handled.
 */
Boolean AccessibilityInjector::HandleKeyEventIfNecessary(
    /* [in] */ IKeyEvent* event)
{
    if (!IsAccessibilityEnabled()) {
        mAccessibilityScriptInjected = FALSE;
        ToggleFallbackAccessibilityInjector(FALSE);
        return FALSE;
    }

    if (mAccessibilityScriptInjected) {
        // if an accessibility script is injected we delegate to it the key
        // handling. this script is a screen reader which is a fully fledged
        // solution for blind users to navigate in and interact with web
        // pages.
        Int32 action;
        event->GetAction(&action);
        if (action == IKeyEvent::ACTION_UP) {
            assert(0);
//            mWebViewClassic->SendBatchableInputMessage(WebViewCore::EventHub::KEY_UP, 0, 0, event);
        }
        else if (action == IKeyEvent::ACTION_DOWN) {
            assert(0);
//            mWebViewClassic->SendBatchableInputMessage(WebViewCore::EventHub::KEY_DOWN, 0, 0, event);
        }
        else {
            return FALSE;
        }

        return TRUE;
    }

    if (mAccessibilityInjectorFallback != NULL) {
        // if an accessibility injector is present (no JavaScript enabled or
        // the site opts out injecting our JavaScript screen reader) we let
        // it decide whether to act on and consume the event.
        return mAccessibilityInjectorFallback->OnKeyEvent(event);
    }

    return FALSE;
}

/**
 * Attempts to handle selection change events when accessibility is using a
 * non-JavaScript method.
 *
 * @param selectionString The selection string.
 */
void AccessibilityInjector::HandleSelectionChangedIfNecessary(
    /* [in] */ const String& selectionString)
{
    if (mAccessibilityInjectorFallback != NULL) {
        mAccessibilityInjectorFallback->OnSelectionStringChange(selectionString);
    }
}

/**
 * Prepares for injecting accessibility scripts into a new page.
 *
 * @param url The URL that will be loaded.
 */
void AccessibilityInjector::OnPageStarted(
    /* [in] */ const String& url)
{
    mAccessibilityScriptInjected = FALSE;
//    if (DEBUG) {
//        Log.w(TAG, "[" + mWebView.hashCode() + "] Started loading new page");
//    }
    AddAccessibilityApisIfNecessary();
}

/**
 * Attempts to inject the accessibility script using a {@code <script>} tag.
 * <p>
 * This should be called after a page has finished loading.
 * </p>
 *
 * @param url The URL that just finished loading.
 */
void AccessibilityInjector::OnPageFinished(
    /* [in] */ const String& url)
{
    if (!IsAccessibilityEnabled()) {
        ToggleFallbackAccessibilityInjector(FALSE);
        return;
    }

    ToggleFallbackAccessibilityInjector(TRUE);

    if (ShouldInjectJavaScript(url)) {
        // If we're supposed to use the JS screen reader, request a
        // callback to confirm that CallbackHandler is working.
//        if (DEBUG) {
//            Log.d(TAG, "[" + mWebView.hashCode() + "] Request callback ");
//        }
        mCallback->RequestCallback(mWebView, mInjectScriptRunnable);
    }
}

/**
 * Attempts to unload scripting interfaces for accessibility.
 * <p>
 * This should only be called before a page loads.
 */
void AccessibilityInjector::RemoveAccessibilityApisIfNecessary()
{
    RemoveTtsApis();
    RemoveCallbackApis();
}

/**
 * Called by {@link #mInjectScriptRunnable} to inject the JavaScript-based
 * screen reader after confirming that the {@link CallbackHandler} API is
 * functional.
 */
void AccessibilityInjector::InjectJavaScript()
{
    ToggleFallbackAccessibilityInjector(FALSE);

    if (!mAccessibilityScriptInjected) {
        mAccessibilityScriptInjected = TRUE;
        String injectionUrl = GetScreenReaderInjectionUrl();
        mWebView->LoadUrl(injectionUrl);
//        if (DEBUG) {
//            Log.d(TAG, "[" + mWebView.hashCode() + "] Loading screen reader into WebView");
//        }
    }
    else {
//        if (DEBUG) {
//            Log.w(TAG, "[" + mWebView.hashCode() + "] Attempted to inject screen reader twice");
//        }
    }
}

/**
 * Adjusts the accessibility injection state to reflect changes in the
 * JavaScript enabled state.
 *
 * @param enabled Whether JavaScript is enabled.
 */
void AccessibilityInjector::UpdateJavaScriptEnabled(
    /* [in] */ Boolean enabled)
{
    if (enabled) {
        AddAccessibilityApisIfNecessary();
    }
    else {
        RemoveAccessibilityApisIfNecessary();
    }

    // We have to reload the page after adding or removing APIs.
    mWebView->Reload();
}

/**
 * Toggles the non-JavaScript method for handling accessibility.
 *
 * @param enabled {@code true} to enable the non-JavaScript method, or
 *            {@code false} to disable it.
 */
void AccessibilityInjector::ToggleFallbackAccessibilityInjector(
    /* [in] */ Boolean enabled)
{
    if (enabled && (mAccessibilityInjectorFallback == NULL)) {
        mAccessibilityInjectorFallback = new AccessibilityInjectorFallback(mWebViewClassic);
    }
    else {
        mAccessibilityInjectorFallback = NULL;
    }
}

/**
 * Determines whether it's okay to inject JavaScript into a given URL.
 *
 * @param url The URL to check.
 * @return {@code true} if JavaScript should be injected, {@code false} if a
 *         non-JavaScript method should be used.
 */
Boolean AccessibilityInjector::ShouldInjectJavaScript(
    /* [in] */ const String& url)
{
    // Respect the WebView's JavaScript setting.
    if (!IsJavaScriptEnabled()) {
        return FALSE;
    }

    // Allow the page to opt out of Accessibility script injection.
    if (GetAxsUrlParameterValue(url) == ACCESSIBILITY_SCRIPT_INJECTION_OPTED_OUT) {
        return FALSE;
    }

    // The user must explicitly enable Accessibility script injection.
    if (!IsScriptInjectionEnabled()) {
        return FALSE;
    }

    return TRUE;
}

/**
 * @return {@code true} if the user has explicitly enabled Accessibility
 *         script injection.
 */
Boolean AccessibilityInjector::IsScriptInjectionEnabled()
{
    //TODO
    assert(0);
//    AutoPtr<ISettingsSecure> secure;
//    CSettingsSecure::New((ISettingsSecure**)&secure);
//    AutoPtr<IContentResolver> cr;
//    mContext->GetContentResolver((IContentResolver**)&cr);
//    Int32 injectionSetting;
//    ISettingsSecure->GetInt32(
//            cr, ISettingsSecure::ACCESSIBILITY_SCRIPT_INJECTION, 0, &injectionSetting);

//    return (injectionSetting == 1);
    return TRUE;
}

/**
 * Attempts to initialize and add interfaces for TTS, if that hasn't already
 * been done.
 */
void AccessibilityInjector::AddTtsApis()
{
    if (mTextToSpeech == NULL) {
        mTextToSpeech = new TextToSpeechWrapper(mContext);
    }

    mWebView->AddJavascriptInterface((IInterface*)mTextToSpeech.Get(), ALIAS_TTS_JS_INTERFACE);
}

/**
 * Attempts to shutdown and remove interfaces for TTS, if that hasn't
 * already been done.
 */
void AccessibilityInjector::RemoveTtsApis()
{
    if (mTextToSpeech != NULL) {
        mTextToSpeech->Stop();
        mTextToSpeech->Shutdown();
        mTextToSpeech = NULL;
    }

    mWebView->RemoveJavascriptInterface(ALIAS_TTS_JS_INTERFACE);
}

void AccessibilityInjector::AddCallbackApis()
{
    if (mCallback == NULL) {
        mCallback = new CallbackHandler(ALIAS_TRAVERSAL_JS_INTERFACE);
    }

    mWebView->AddJavascriptInterface((IInterface*)mCallback.Get(), ALIAS_TRAVERSAL_JS_INTERFACE);
}

void AccessibilityInjector::RemoveCallbackApis()
{
    if (mCallback != NULL) {
        mCallback = NULL;
    }

    mWebView->RemoveJavascriptInterface(ALIAS_TRAVERSAL_JS_INTERFACE);
}

/**
 * Returns the script injection preference requested by the URL, or
 * {@link #ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED} if the page has no
 * preference.
 *
 * @param url The URL to check.
 * @return A script injection preference.
 */
Int32 AccessibilityInjector::GetAxsUrlParameterValue(
    /* [in] */ const String& url)
{
    if (url.IsNull() == 0) {
        return ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED;
    }

    assert(0);
    // TODO
//    try {
//        final List<NameValuePair> params = URLEncodedUtils.parse(new URI(url), null);
//
//        for (NameValuePair param : params) {
//            if ("axs".equals(param.getName())) {
//                return verifyInjectionValue(param.getValue());
//            }
//        }
//    } catch (URISyntaxException e) {
//        // Do nothing.
//    } catch (IllegalArgumentException e) {
//        // Catch badly-formed URLs.
//    }

    return ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED;
}

Int32 AccessibilityInjector::VerifyInjectionValue(
    /* [in] */ const String& value)
{
    Int32 parsed;

    if (SUCCEEDED(StringUtils::ParseInt32(value, 10, &parsed))) {
        switch (parsed) {
            case ACCESSIBILITY_SCRIPT_INJECTION_OPTED_OUT:
                return ACCESSIBILITY_SCRIPT_INJECTION_OPTED_OUT;
            case ACCESSIBILITY_SCRIPT_INJECTION_PROVIDED:
                return ACCESSIBILITY_SCRIPT_INJECTION_PROVIDED;
        }
    }

    return ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED;
}

/**
 * @return The URL for injecting the screen reader.
 */
String AccessibilityInjector::GetScreenReaderInjectionUrl()
{
    String screenReaderUrl;
    assert(0);
    //TODO
//    AutoPtr<IContentResolver> cr;
//    mContext->GetContentResolver((IContentResolver**)&cr);
//    AutoPtr<ISettingsSecure> secure;
//    CSettingsSecure::New((ISettingsSecure**)&secure);
//    secure->GetString(cr, ISettingsSecure::ACCESSIBILITY_SCREEN_READER_URL);

//    String str;
//    str.AppendFormat(ACCESSIBILITY_SCREEN_READER_JAVASCRIPT_TEMPLATE, (const char*)screenReaderUrl);
    return screenReaderUrl;
}

/**
 * @return {@code true} if JavaScript is enabled in the {@link WebView}
 *         settings.
 */
Boolean AccessibilityInjector::IsJavaScriptEnabled()
{
    AutoPtr<IWebSettings> settings;
    mWebView->GetSettings((IWebSettings**)&settings);
    if (settings == NULL) {
        return FALSE;
    }

    Boolean bRet = FALSE;
    settings->GetJavaScriptEnabled(&bRet);
    return bRet;
}

/**
 * @return {@code true} if accessibility is enabled.
 */
Boolean AccessibilityInjector::IsAccessibilityEnabled()
{
    Boolean bRet = FALSE;
    mAccessibilityManager->IsEnabled(&bRet);
    return bRet;
}

/**
 * Packs an accessibility action into a JSON object and sends it to AndroidVox.
 *
 * @param action The action identifier.
 * @param arguments The action arguments, if applicable.
 * @return The result of the action.
 */
Boolean AccessibilityInjector::SendActionToAndroidVox(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    assert(0);
    // TODO
//    if (mAccessibilityJSONObject == NULL) {
//        mAccessibilityJSONObject = new JSONObject();
//    } else {
//        // Remove all keys from the object.
//        final Iterator<?> keys = mAccessibilityJSONObject.keys();
//        while (keys.hasNext()) {
//            keys.next();
//            keys.remove();
//        }
//    }
//
//    try {
//        mAccessibilityJSONObject.accumulate("action", action);
//
//        switch (action) {
//            case AccessibilityNodeInfo.ACTION_NEXT_AT_MOVEMENT_GRANULARITY:
//            case AccessibilityNodeInfo.ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY:
//                if (arguments != null) {
//                    final int granularity = arguments.getInt(
//                            AccessibilityNodeInfo.ACTION_ARGUMENT_MOVEMENT_GRANULARITY_INT);
//                    mAccessibilityJSONObject.accumulate("granularity", granularity);
//                }
//                break;
//            case AccessibilityNodeInfo.ACTION_NEXT_HTML_ELEMENT:
//            case AccessibilityNodeInfo.ACTION_PREVIOUS_HTML_ELEMENT:
//                if (arguments != null) {
//                    final String element = arguments.getString(
//                            AccessibilityNodeInfo.ACTION_ARGUMENT_HTML_ELEMENT_STRING);
//                    mAccessibilityJSONObject.accumulate("element", element);
//                }
//                break;
//        }
//    } catch (JSONException e) {
//        return false;
//    }
//
//    final String jsonString = mAccessibilityJSONObject.toString();
//    final String jsCode = String.format(ACCESSIBILITY_ANDROIDVOX_TEMPLATE, jsonString);
//    return mCallback.performAction(mWebView, jsCode);
    return FALSE;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
