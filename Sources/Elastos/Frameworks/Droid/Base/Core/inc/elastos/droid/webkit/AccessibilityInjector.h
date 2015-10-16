
#ifndef __ELASTOS_DROID_WEBKIT_ACCESSIBILITYINJECTOR_H__
#define __ELASTOS_DROID_WEBKIT_ACCESSIBILITYINJECTOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Runnable.h"
#include "elastos/droid/webkit/AccessibilityInjectorFallback.h"
//#include "elastos/droid/webkit/CWebViewClassic.h"
#include <elastos/Core/Object.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::IRunnable;
using Elastos::Core::Object;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Speech::Tts::ITextToSpeech;
using Elastos::Droid::Speech::Tts::ITextToSpeechOnInitListener;
using Elastos::Droid::Speech::Tts::IUtteranceProgressListener;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

/**
 * Handles injecting accessibility JavaScript and related JavaScript -> Java
 * APIs.
 */
class AccessibilityInjector : public ElRefBase
{
private:
    /**
     * Used to protect the TextToSpeech class, only exposing the methods we want to expose.
     */
    class TextToSpeechWrapper : public ElRefBase
    {
        friend class AccessibilityInjector;

    public:
        TextToSpeechWrapper(
            /* [in] */ IContext* context);

        CAR_INTERFACE_DECL();

        virtual CARAPI_(Boolean) IsSpeaking();

        virtual CARAPI_(Int32) Speak(
            /* [in] */ const String& text,
            /* [in] */ Int32 queueMode,
            /* [in] */ HashMap<String, String>* params);

        virtual CARAPI_(Int32) Stop();

    protected:
        virtual CARAPI_(void) Shutdown();

    private:
        static const String WRAP_TAG;

        HashMap<String, String> mTtsParams;
        AutoPtr<ITextToSpeech> mTextToSpeech;
        Object mTextToSpeechMutex;

        /**
         * Whether this wrapper is ready to speak. If this is {@code true} then
         * {@link #mShutdown} is guaranteed to be {@code false}.
         */
        volatile Boolean mReady;

        /**
         * Whether this wrapper was shut down. If this is {@code true} then
         * {@link #mReady} is guaranteed to be {@code false}.
         */
        volatile Boolean mShutdown;

        AutoPtr<ITextToSpeechOnInitListener> mInitListener;

        AutoPtr<IUtteranceProgressListener> mErrorListener;
    };

    /**
     * Exposes result interface to JavaScript.
     */
    class CallbackHandler
        : public ElLightRefBase
        , public IInterface
    {
        friend class AccessibilityInjector;
    public:
        CAR_INTERFACE_DECL();

        /**
         * Callback exposed to JavaScript. Handles returning the result of a
         * request to a waiting (or potentially timed out) thread.
         *
         * @param id The result id of the request as a {@link String}.
         * @param result The result of the request as a {@link String}.
         */
        virtual CARAPI_(void) OnResult(
            /* [in] */ const String& id,
            /* [in] */ const String& result);

        /**
         * Requests a callback to ensure that the JavaScript interface for this
         * object has been added successfully.
         *
         * @param webView The web view to request a callback from.
         * @param callbackRunnable Runnable to execute if a callback is received.
         */
        virtual CARAPI_(void) RequestCallback(
            /* [in] */ IWebView* webView,
            /* [in] */ IRunnable* callbackRunnable);

        virtual CARAPI_(void) Callback();

    private:
        CallbackHandler(
            /* [in] */ const String& interfaceName);

        /**
         * Performs an action and attempts to wait for a result.
         *
         * @param webView The WebView to perform the action on.
         * @param code JavaScript code that evaluates to a result.
         * @return The result of the action, or false if it timed out.
         */
        CARAPI_(Boolean) PerformAction(
            /* [in] */ IWebView* webView,
            /* [in] */ const String& code);

        /**
         * Gets the result of a request to perform an accessibility action.
         *
         * @param resultId The result id to match the result with the request.
         * @return The result of the request.
         */
        CARAPI_(Boolean) GetResultAndClear(
            /* [in] */ Int32 resultId);

        /**
         * Clears the result state.
         */
        CARAPI_(void) ClearResultLocked();

        /**
         * Waits up to a given bound for a result of a request and returns it.
         *
         * @param resultId The result id to match the result with the request.
         * @return Whether the result was received.
         */
        CARAPI_(Boolean) WaitForResultTimedLocked(
            /* [in] */ Int32 resultId);

    private:
        static const String JAVASCRIPT_ACTION_TEMPLATE;

        // Time in milliseconds to wait for a result before failing.
        static const Int64 RESULT_TIMEOUT = 5000;

        AutoPtr<IAtomicInteger32> mResultIdCounter;

        Object mResultLock;
        String mInterfaceName;
        AutoPtr<IHandler> mMainHandler;

        AutoPtr<IRunnable> mCallbackRunnable;

        Boolean mResult;
        Int32 mResultId;
    };

    class InjectScriptRunnable : public Runnable
    {
    public:
        InjectScriptRunnable(
            /* [in] */ AccessibilityInjector* owner)
            : mOwner(owner)
        {}

        CARAPI Run();

    private:
        AccessibilityInjector* mOwner;
    };

public:
    /**
     * Creates an instance of the AccessibilityInjector based on
     * {@code webViewClassic}.
     *
     * @param webViewClassic The WebViewClassic that this AccessibilityInjector
     *            manages.
     */
    AccessibilityInjector(
        /* [in] */ CWebViewClassic* webViewClassic);

    /**
     * If JavaScript is enabled, pauses or resumes AndroidVox.
     *
     * @param enabled Whether feedback should be enabled.
     */
    virtual CARAPI_(void) ToggleAccessibilityFeedback(
        /* [in] */ Boolean enabled);

    /**
     * Attempts to load scripting interfaces for accessibility.
     * <p>
     * This should only be called before a page loads.
     */
    virtual CARAPI_(void) AddAccessibilityApisIfNecessary();

    /**
     * Destroys this accessibility injector.
     */
    virtual CARAPI_(void) Destroy();

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
    virtual CARAPI_(void) OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    /**
     * Returns {@code true} if this {@link AccessibilityInjector} should handle
     * the specified action.
     *
     * @param action An accessibility action identifier.
     * @return {@code true} if this {@link AccessibilityInjector} should handle
     *         the specified action.
     */
    virtual CARAPI_(Boolean) SupportsAccessibilityAction(
        /* [in] */ Int32 action);

    /**
     * Performs the specified accessibility action.
     *
     * @param action The identifier of the action to perform.
     * @param arguments The action arguments, or {@code null} if no arguments.
     * @return {@code true} if the action was successful.
     * @see View#performAccessibilityAction(int, Bundle)
     */
    virtual CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

    /**
     * Attempts to handle key events when accessibility is turned on.
     *
     * @param event The key event to handle.
     * @return {@code true} if the event was handled.
     */
    virtual CARAPI_(Boolean) HandleKeyEventIfNecessary(
        /* [in] */ IKeyEvent* event);

    /**
     * Attempts to handle selection change events when accessibility is using a
     * non-JavaScript method.
     *
     * @param selectionString The selection string.
     */
    virtual CARAPI_(void) HandleSelectionChangedIfNecessary(
        /* [in] */ const String& selectionString);

    /**
     * Prepares for injecting accessibility scripts into a new page.
     *
     * @param url The URL that will be loaded.
     */
    virtual CARAPI_(void) OnPageStarted(
        /* [in] */ const String& url);

    /**
     * Attempts to inject the accessibility script using a {@code <script>} tag.
     * <p>
     * This should be called after a page has finished loading.
     * </p>
     *
     * @param url The URL that just finished loading.
     */
    virtual CARAPI_(void) OnPageFinished(
        /* [in] */ const String& url);

    /**
     * Adjusts the accessibility injection state to reflect changes in the
     * JavaScript enabled state.
     *
     * @param enabled Whether JavaScript is enabled.
     */
    virtual CARAPI_(void) UpdateJavaScriptEnabled(
        /* [in] */ Boolean enabled);

private:
    /**
     * Attempts to unload scripting interfaces for accessibility.
     * <p>
     * This should only be called before a page loads.
     */
    CARAPI_(void) RemoveAccessibilityApisIfNecessary();

    CARAPI_(void) ToggleAndroidVox(
        /* [in] */ Boolean state);

    /**
     * Called by {@link #mInjectScriptRunnable} to inject the JavaScript-based
     * screen reader after confirming that the {@link CallbackHandler} API is
     * functional.
     */
    CARAPI_(void) InjectJavaScript();

    /**
     * Toggles the non-JavaScript method for handling accessibility.
     *
     * @param enabled {@code true} to enable the non-JavaScript method, or
     *            {@code false} to disable it.
     */
    CARAPI_(void) ToggleFallbackAccessibilityInjector(
        /* [in] */ Boolean enabled);

    /**
     * Determines whether it's okay to inject JavaScript into a given URL.
     *
     * @param url The URL to check.
     * @return {@code true} if JavaScript should be injected, {@code false} if a
     *         non-JavaScript method should be used.
     */
    CARAPI_(Boolean) ShouldInjectJavaScript(
        /* [in] */ const String& url);

    /**
     * @return {@code true} if the user has explicitly enabled Accessibility
     *         script injection.
     */
    CARAPI_(Boolean) IsScriptInjectionEnabled();

    /**
     * Attempts to initialize and add interfaces for TTS, if that hasn't already
     * been done.
     */
    CARAPI_(void) AddTtsApis();

    /**
     * Attempts to shutdown and remove interfaces for TTS, if that hasn't
     * already been done.
     */
    CARAPI_(void) RemoveTtsApis();

    CARAPI_(void) AddCallbackApis();

    CARAPI_(void) RemoveCallbackApis();

    /**
     * Returns the script injection preference requested by the URL, or
     * {@link #ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED} if the page has no
     * preference.
     *
     * @param url The URL to check.
     * @return A script injection preference.
     */
    CARAPI_(Int32) GetAxsUrlParameterValue(
        /* [in] */ const String& url);

    CARAPI_(Int32) VerifyInjectionValue(
        /* [in] */ const String& value);

    /**
     * @return The URL for injecting the screen reader.
     */
    CARAPI_(String) GetScreenReaderInjectionUrl();

    /**
     * @return {@code true} if JavaScript is enabled in the {@link WebView}
     *         settings.
     */
    CARAPI_(Boolean) IsJavaScriptEnabled();

    /**
     * @return {@code true} if accessibility is enabled.
     */
    CARAPI_(Boolean) IsAccessibilityEnabled();

    /**
     * Packs an accessibility action into a JSON object and sends it to AndroidVox.
     *
     * @param action The action identifier.
     * @param arguments The action arguments, if applicable.
     * @return The result of the action.
     */
    CARAPI_(Boolean) SendActionToAndroidVox(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

private:
    static const String TAG;

    static Boolean DEBUG;

    // The WebViewClassic this injector is responsible for managing.
    AutoPtr<CWebViewClassic> mWebViewClassic;

    // Cached reference to mWebViewClassic.getContext(), for convenience.
    AutoPtr<IContext> mContext;

    // Cached reference to mWebViewClassic.getWebView(), for convenience.
    AutoPtr<IWebView> mWebView;

    // The Java objects that are exposed to JavaScript.
    AutoPtr<TextToSpeechWrapper> mTextToSpeech;
    AutoPtr<CallbackHandler> mCallback;

    // Lazily loaded helper objects.
    AutoPtr<IAccessibilityManager> mAccessibilityManager;
    AutoPtr<AccessibilityInjectorFallback> mAccessibilityInjectorFallback;
//    AutoPtr<IJSONObject> mAccessibilityJSONObject;

    // Whether the accessibility script has been injected into the current page.
    Boolean mAccessibilityScriptInjected;

    // Constants for determining script injection strategy.
    static const Int32 ACCESSIBILITY_SCRIPT_INJECTION_UNDEFINED      = -1;
    static const Int32 ACCESSIBILITY_SCRIPT_INJECTION_OPTED_OUT      = 0;

    static const Int32 ACCESSIBILITY_SCRIPT_INJECTION_PROVIDED       = 1;

    // Alias for TTS API exposed to JavaScript.
    static const String ALIAS_TTS_JS_INTERFACE;

    // Alias for traversal callback exposed to JavaScript.
    static const String ALIAS_TRAVERSAL_JS_INTERFACE;

    // Template for JavaScript that injects a screen-reader.
    static const String ACCESSIBILITY_SCREEN_READER_JAVASCRIPT_TEMPLATE;

    // Template for JavaScript that performs AndroidVox actions.
    static const String ACCESSIBILITY_ANDROIDVOX_TEMPLATE;

    // JS code used to shut down an active AndroidVox instance.
    static const String TOGGLE_CVOX_TEMPLATE;

    /**
     * Runnable used to inject the JavaScript-based screen reader if the
     * {@link CallbackHandler} API was successfully exposed to JavaScript.
     */
    AutoPtr<IRunnable> mInjectScriptRunnable;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ACCESSIBILITYINJECTOR_H__
