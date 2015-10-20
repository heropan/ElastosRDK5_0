
#include "elastos/droid/webkit/native/content/browser/accessibility/AccessibilityInjector.h"
#include "elastos/droid/webkit/native/content/browser/accessibility/JellyBeanAccessibilityInjector.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewCore.h"
#include "elastos/droid/webkit/native/content/common/ContentSwitches.h"
#include "elastos/droid/webkit/native/base/CommandLine.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>

// TODO using Elastos::Net::CURI;
using Elastos::Net::IURI;
using Elastos::Core::StringUtils;
using Elastos::Utility::IHashMap;
// TODO using Elastos::Droid::Accessibilityservice::IAccessibilityServiceInfo;
using Elastos::Droid::Webkit::Content::Common::ContentSwitches;
using Elastos::Droid::Webkit::Content::Browser::ContentViewCore;
using Elastos::Droid::Webkit::Base::CommandLine;
using Elastos::Droid::View::EIID_IView;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Accessibility {

//==================================================================
//             AccessibilityInjector::VibratorWrapper
//==================================================================

const Int64 AccessibilityInjector::VibratorWrapper::MAX_VIBRATE_DURATION_MS;

AccessibilityInjector::VibratorWrapper::VibratorWrapper(
    /* [in] */ IContext* context)
{
    context->GetSystemService(IContext::VIBRATOR_SERVICE, (IInterface**)&mVibrator);
}

//@JavascriptInterface
//@SuppressWarnings("unused")
Boolean AccessibilityInjector::VibratorWrapper::HasVibrator()
{
    Boolean result = FALSE;
    mVibrator->HasVibrator(&result);
    return result;
}

//@JavascriptInterface
//@SuppressWarnings("unused")
void AccessibilityInjector::VibratorWrapper::Vibrate(
    /* [in] */ Int64 milliseconds)
{
    milliseconds = Elastos::Core::Math::Min(milliseconds, MAX_VIBRATE_DURATION_MS);
    mVibrator->Vibrate(milliseconds);
}

//@JavascriptInterface
//@SuppressWarnings("unused")
void AccessibilityInjector::VibratorWrapper::Vibrate(
    /* [in] */ ArrayOf<Int64>* pattern,
    /* [in] */ Int32 repeat)
{
    for (Int32 i = 0; i < pattern->GetLength(); ++i) {
        (*pattern)[i] = Elastos::Core::Math::Min((*pattern)[i], MAX_VIBRATE_DURATION_MS);
    }

    repeat = -1;

    mVibrator->Vibrate(pattern, repeat);
}

//@JavascriptInterface
//@SuppressWarnings("unused")
void AccessibilityInjector::VibratorWrapper::Cancel()
{
    mVibrator->Cancel();
}

//==================================================================
//             AccessibilityInjector::TextToSpeechWrapper
//==================================================================

AccessibilityInjector::TextToSpeechWrapper::TextToSpeechWrapper(
    /* [in] */ IView* view,
    /* [in] */ IContext* context)
{
    mView = view;
    assert(0);
    // TODO
    // CTextToSpeech::New(context, NULL, NULL, (ITextToSpeech**)&mTextToSpeech);

    assert(0);
    // TODO
    // mSelfBrailleClient = new SelfBrailleClient(context, CommandLine::GetInstance()::HasSwitch(
    //         ContentSwitches::ACCESSIBILITY_DEBUG_BRAILLE_SERVICE));
}

//@JavascriptInterface
//@SuppressWarnings("unused")
Boolean AccessibilityInjector::TextToSpeechWrapper::IsSpeaking()
{
    Boolean result = FALSE;
    mTextToSpeech->IsSpeaking(&result);
    return result;
}

//@JavascriptInterface
//@SuppressWarnings("unused")
Int32 AccessibilityInjector::TextToSpeechWrapper::Speak(
    /* [in] */ const String& text,
    /* [in] */ Int32 queueMode,
    /* [in] */ const String& jsonParams)
{
    assert(0);
    // TODO
    // // Try to pull the params from the JSON string.
    // AutoPtr<IHashMap> params;
    // // try {
    //     if (jsonParams != NULL) {
    //         params = new HashMap<String, String>();
    //         JSONObject json = new JSONObject(jsonParams);

    //         // Using legacy API here.
    //         //@SuppressWarnings("unchecked")
    //         Iterator<String> keyIt = json.keys();

    //         while (keyIt.hasNext()) {
    //             String key = keyIt.next();
    //             // Only add parameters that are raw data types.
    //             if (json.optJSONObject(key) == null && json.optJSONArray(key) == null) {
    //                 params.put(key, json.getString(key));
    //             }
    //         }
    //     }
    // // } catch (JSONException e) {
    // //     params = null;
    // // }

    // return mTextToSpeech->Speak(text, queueMode, params);

    return -1;
}

//@JavascriptInterface
//@SuppressWarnings("unused")
Int32 AccessibilityInjector::TextToSpeechWrapper::Stop()
{
    Int32 stop;
    mTextToSpeech->Stop(&stop);
    return stop;
}

//@JavascriptInterface
//@SuppressWarnings("unused")
void AccessibilityInjector::TextToSpeechWrapper::Braille(
    /* [in] */ const String& jsonString)
{
    assert(0);
    // TODO
    // // try {
    //     AutoPtr<IJSONObject> jsonObj;
    //     CJSONObject::New(jsonString, (IJSONObject**)&jsonObj);

    //     AutoPtr<WriteData> data = WriteData::ForView(mView);
    //     String text;
    //     jsonObj->GetString(String("text"), &text);
    //     data->SetText(text);
    //     Int32 startIndex;
    //     jsonObj->GetInt32(String("startIndex"), &startIndex);
    //     data->SetSelectionStart(startIndex);
    //     Int32 endIndex;
    //     jsonObj->GetInt32(String("endIndex"), &endIndex);
    //     data->SetSelectionEnd(endIndex);
    //     mSelfBrailleClient->Write(data);
    // // } catch (JSONException ex) {
    // //     Log.w(TAG, "Error parsing JS JSON object", ex);
    // // }
}

//@SuppressWarnings("unused")
void AccessibilityInjector::TextToSpeechWrapper::ShutdownInternal()
{
    mTextToSpeech->Shutdown();
    assert(0);
    // TODO
    // mSelfBrailleClient->Shutdown();
}

//==================================================================
//                     AccessibilityInjector
//==================================================================

const String AccessibilityInjector::TAG("AccessibilityInjector");

// To support building against the JELLY_BEAN and not JELLY_BEAN_MR1 SDK we need to add this
// constant here.
const Int32 AccessibilityInjector::FEEDBACK_BRAILLE;

// constants for determining script injection strategy
const Int32 AccessibilityInjector::ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED;
const Int32 AccessibilityInjector::ACCESSIBILITY_SCRIPT_INJECTION_OPTED_OUT;
const Int32 AccessibilityInjector::ACCESSIBILITY_SCRIPT_INJECTION_PROVIDED;
const String AccessibilityInjector::ALIAS_ACCESSIBILITY_JS_INTERFACE("accessibility");
const String AccessibilityInjector::ALIAS_ACCESSIBILITY_JS_INTERFACE_2("accessibility2");

// Template for JavaScript that injects a screen-reader.
const String AccessibilityInjector::DEFAULT_ACCESSIBILITY_SCREEN_READER_URL("https://ssl.gstatic.com/accessibility/javascript/android/chromeandroidvox.js");

const String AccessibilityInjector::ACCESSIBILITY_SCREEN_READER_JAVASCRIPT_TEMPLATE(
        "(function() {"
        "    var chooser = document.createElement('script');"
        "    chooser.type = 'text/javascript';"
        "    chooser.src = '%1s';"
        "    document.getElementsByTagName('head')[0].appendChild(chooser);"
        "  })();");

// JavaScript call to turn ChromeVox on or off.
const String AccessibilityInjector::TOGGLE_CHROME_VOX_JAVASCRIPT(
        "(function() {"
        "    if (typeof cvox !== 'undefined') {"
        "        cvox.ChromeVox.host.activateOrDeactivateChromeVox(%1s);"
        "    }"
        "  })();");

/**
 * Creates an instance of the IceCreamSandwichAccessibilityInjector.
 * @param view The ContentViewCore that this AccessibilityInjector manages.
 */
AccessibilityInjector::AccessibilityInjector(
    /* [in] */ ContentViewCore* view)
    : WebContentsObserverAndroid(view)
    , mContentViewCore(view)
    , mAccessibilityScreenReaderUrl(CommandLine::GetInstance()->GetSwitchValue(
            ContentSwitches::ACCESSIBILITY_JAVASCRIPT_URL,
            DEFAULT_ACCESSIBILITY_SCREEN_READER_URL))
{
    AutoPtr<IContext> context = mContentViewCore->GetContext();
    Int32 permission;
    assert(0);
    // TODO
    // context->CheckCallingOrSelfPermission(
    //         android::Manifest::permission::VIBRATE, &permission);
    mHasVibratePermission = (permission == IPackageManager::PERMISSION_GRANTED);
}

/**
 * Returns an instance of the {@link AccessibilityInjector} based on the SDK version.
 * @param view The ContentViewCore that this AccessibilityInjector manages.
 * @return An instance of a {@link AccessibilityInjector}.
 */
AutoPtr<AccessibilityInjector> AccessibilityInjector::NewInstance(
    /* [in] */ ContentViewCore* view)
{
    if (Build::VERSION::SDK_INT < Build::VERSION_CODES::JELLY_BEAN) {
        AutoPtr<AccessibilityInjector> ret = new AccessibilityInjector(view);
        return ret;
    }
    else {
        AutoPtr<AccessibilityInjector> ret = new JellyBeanAccessibilityInjector(view);
        return ret;
    }
}

/**
 * Injects a <script> tag into the current web site that pulls in the ChromeVox script for
 * accessibility support.  Only injects if accessibility is turned on by
 * {@link AccessibilityManager#isEnabled()}, accessibility script injection is turned on, and
 * javascript is enabled on this page.
 *
 * @see AccessibilityManager#isEnabled()
 */
void AccessibilityInjector::InjectAccessibilityScriptIntoPage()
{
    if (!AccessibilityIsAvailable()) return;

    Int32 axsParameterValue = GetAxsUrlParameterValue();
    if (axsParameterValue != ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED) {
        return;
    }

    String js = GetScreenReaderInjectingJs();
    if (mContentViewCore->IsDeviceAccessibilityScriptInjectionEnabled() &&
            js != NULL && mContentViewCore->IsAlive()) {
        AddOrRemoveAccessibilityApisIfNecessary();
        mContentViewCore->EvaluateJavaScript(js, NULL);
        mInjectedScriptEnabled = TRUE;
        mScriptInjected = TRUE;
    }
}

/**
 * Handles adding or removing accessibility related Java objects ({@link TextToSpeech} and
 * {@link Vibrator}) interfaces from Javascript.  This method should be called at a time when it
 * is safe to add or remove these interfaces, specifically when the {@link ContentViewCore} is
 * first initialized or right before the {@link ContentViewCore} is about to navigate to a URL
 * or reload.
 * <p>
 * If this method is called at other times, the interfaces might not be correctly removed,
 * meaning that Javascript can still access these Java objects that may have been already
 * shut down.
 */
void AccessibilityInjector::AddOrRemoveAccessibilityApisIfNecessary()
{
    if (AccessibilityIsAvailable()) {
        AddAccessibilityApis();
    }
    else {
        RemoveAccessibilityApis();
    }
}

/**
 * Checks whether or not touch to explore is enabled on the system.
 */
Boolean AccessibilityInjector::AccessibilityIsAvailable()
{
    Boolean isEnabled;
    GetAccessibilityManager()->IsEnabled(&isEnabled);
    if (!isEnabled ||
            mContentViewCore->GetContentSettings() == NULL ||
            !mContentViewCore->GetContentSettings()->GetJavaScriptEnabled()) {
        return FALSE;
    }

    // try {
        // Check that there is actually a service running that requires injecting this script.
        AutoPtr<IList> services;
        assert(0);
        // TODO
        // GetAccessibilityManager()->GetEnabledAccessibilityServiceList(
        //                 FEEDBACK_BRAILLE | IAccessibilityServiceInfo::FEEDBACK_SPOKEN,
        //                 (IList**)&services);
        Int32 size;
        services->GetSize(&size);
        return size > 0;
    // } catch (NullPointerException e) {
    //     // getEnabledAccessibilityServiceList() can throw an NPE due to a bad
    //     // AccessibilityService.
    //     return false;
    // }
}

/**
 * Sets whether or not the script is enabled.  If the script is disabled, we also stop any
 * we output that is occurring. If the script has not yet been injected, injects it.
 * @param enabled Whether or not to enable the script.
 */
void AccessibilityInjector::SetScriptEnabled(
    /* [in] */ Boolean enabled)
{
    if (enabled && !mScriptInjected) InjectAccessibilityScriptIntoPage();
    if (!AccessibilityIsAvailable() || mInjectedScriptEnabled == enabled) return;

    mInjectedScriptEnabled = enabled;
    if (mContentViewCore->IsAlive()) {
        assert(0);
        // TODO
        // String js = String.format(TOGGLE_CHROME_VOX_JAVASCRIPT, Boolean.toString(
        //         mInjectedScriptEnabled));
        // mContentViewCore->EvaluateJavaScript(js, NULL);

        if (!mInjectedScriptEnabled) {
            // Stop any TTS/Vibration right now.
            OnPageLostFocus();
        }
    }
}

/**
 * Notifies this handler that a page load has started, which means we should mark the
 * accessibility script as not being injected.  This way we can properly ignore incoming
 * accessibility gesture events.
 */
//@Override
ECode AccessibilityInjector::DidStartLoading(
    /* [in] */ const String& url)
{
    mScriptInjected = FALSE;
    return NOERROR;
}

//@Override
ECode AccessibilityInjector::DidStopLoading(
    /* [in] */ const String& url)
{
    InjectAccessibilityScriptIntoPage();
    return NOERROR;
}

/**
 * Stop any notifications that are currently going on (e.g. Text-to-Speech).
 */
void AccessibilityInjector::OnPageLostFocus()
{
    if (mContentViewCore->IsAlive()) {
        if (mTextToSpeech != NULL) mTextToSpeech->Stop();
        if (mVibrator != NULL) mVibrator->Cancel();
    }
}

/**
 * Initializes an {@link AccessibilityNodeInfo} with the actions and movement granularity
 * levels supported by this {@link AccessibilityInjector}.
 * <p>
 * If an action identifier is added in this method, this {@link AccessibilityInjector} should
 * also return {@code true} from {@link #supportsAccessibilityAction(int)}.
 * </p>
 *
 * @param info The info to initialize.
 * @see View#onInitializeAccessibilityNodeInfo(AccessibilityNodeInfo)
 */
void AccessibilityInjector::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
}

/**
 * Returns {@code true} if this {@link AccessibilityInjector} should handle the specified
 * action.
 *
 * @param action An accessibility action identifier.
 * @return {@code true} if this {@link AccessibilityInjector} should handle the specified
 *         action.
 */
Boolean AccessibilityInjector::SupportsAccessibilityAction(
    /* [in] */ Int32 action)
{
    return FALSE;
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
    return FALSE;
}

void AccessibilityInjector::AddAccessibilityApis()
{
    AutoPtr<IContext> context = mContentViewCore->GetContext();
    if (context != NULL) {
        // Enabled, we should try to add if we have to.
        if (mTextToSpeech == NULL) {
            AutoPtr<IView> view = (IView*)mContentViewCore->GetContainerView()->Probe(EIID_IView);
            mTextToSpeech = new TextToSpeechWrapper(view,
                    context);
            mContentViewCore->AddJavascriptInterface((IObject*)mTextToSpeech,
                    ALIAS_ACCESSIBILITY_JS_INTERFACE);
        }

        if (mVibrator == NULL && mHasVibratePermission) {
            mVibrator = new VibratorWrapper(context);
            mContentViewCore->AddJavascriptInterface((IObject*)mVibrator,
                    ALIAS_ACCESSIBILITY_JS_INTERFACE_2);
        }
    }
}

void AccessibilityInjector::RemoveAccessibilityApis()
{
    if (mTextToSpeech != NULL) {
        mContentViewCore->RemoveJavascriptInterface(ALIAS_ACCESSIBILITY_JS_INTERFACE);
        mTextToSpeech->Stop();
        mTextToSpeech->ShutdownInternal();
        mTextToSpeech = NULL;
    }

    if (mVibrator != NULL) {
        mContentViewCore->RemoveJavascriptInterface(ALIAS_ACCESSIBILITY_JS_INTERFACE_2);
        mVibrator->Cancel();
        mVibrator = NULL;
    }
}

Int32 AccessibilityInjector::GetAxsUrlParameterValue()
{
    if (mContentViewCore->GetUrl() == NULL) return ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED;

    // try {
        assert(0);
        // TODO
        // AutoPtr<IURI> uri;
        // CURI::New(mContentViewCore->GetUrl(), (IURI**)&uri)
        // AutoPtr<IList> params = URLEncodedUtils::Parse(uri,
        //         NULL);

        // AutoPtr<IIterable> iterable = (IIterable*)params->Probe(EIID_IIterable);
        // AutoPtr<IIterator> iter;
        // iterable->GetIIterator((IIterator**)&iter);
        // AutoPtr<INameValuePair> param;
        // Boolean bNext;
        // for (iter->HasNext(&bNext); bNext; iter->HasNext(&bNext)) {
        //     iter->GetNext((IInterface**)&param);
        //     String name;
        //     param->GetName(&name);
        //     if (String("axs").Equals(name)) {
        //         String value;
        //         param->GetValue(&value);
        //         return StringUtils::ParseInt32(value);
        //     }
        // }
    // } catch (URISyntaxException ex) {
    // } catch (NumberFormatException ex) {
    // } catch (IllegalArgumentException ex) {
    // }

    return ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED;
}

String AccessibilityInjector::GetScreenReaderInjectingJs()
{
    assert(0);
    // TODO
    // return String.format(ACCESSIBILITY_SCREEN_READER_JAVASCRIPT_TEMPLATE,
    //         mAccessibilityScreenReaderUrl);
    return String(NULL);
}

AutoPtr<IAccessibilityManager> AccessibilityInjector::GetAccessibilityManager()
{
    if (mAccessibilityManager == NULL) {
        mContentViewCore->GetContext()->GetSystemService(IContext::ACCESSIBILITY_SERVICE,
            (IInterface**)&mAccessibilityManager);
    }

    return mAccessibilityManager;
}

} // namespace Accessibility
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
