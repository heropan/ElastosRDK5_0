
#ifndef __ELASTOS_DROID_WEBKIT_BROWSERFRAME_H__
#define __ELASTOS_DROID_WEBKIT_BROWSERFRAME_H__

#include "ext/frameworkext.h"
#include "os/HandlerBase.h"
#include "os/Runnable.h"
#include "webkit/HttpAuthHandler.h"
#include "webkit/SslErrorHandler.h"
#include "webkit/WebCoreElastosBridge.h"
#include "webkit/KeyStoreHandler.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::IO::IInputStream;
using Elastos::Utility::IMap;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Net::Http::ISslError;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::View::IWindowManager;

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaWebFrameCallback
{
    AutoPtr<IWeakReference> obj;// week reference to the BrowserFrame
    AutoPtr<IWeakReference> historyList;//week reference to the HistoryList
    void (*startLoadingResource)(IInterface* obj);//not used
    void (*maybeSavePassword)(IInterface* obj, ArrayOf<Byte>*, const Elastos::String&, const Elastos::String&);
    AutoPtr<IInterface> (*shouldInterceptRequest)(IInterface*, const Elastos::String&, Int32**);
    void (*loadStarted)(IInterface*, const Elastos::String&, IInterface*, Int32, Boolean);
    void (*transitionToCommitted)(IInterface*, Int32, Boolean);
    void (*loadFinished)(IInterface*, const Elastos::String&, Int32, Boolean);
    void (*reportError)(IInterface*, Int32, const Elastos::String&, const Elastos::String&);
    void (*setTitle)(IInterface*, const Elastos::String&);
    void (*windowObjectCleared)(IInterface*, Int32);
    void (*setProgress)(IInterface*, Int32);
    void (*didReceiveIcon)(IInterface*, IInterface*);
    void (*didReceiveTouchIconUrl)(IInterface*, const Elastos::String&, Boolean);
    void (*updateVisitedHistory)(IInterface*, const Elastos::String&, Boolean);
    Boolean (*handleUrl)(IInterface*, const Elastos::String&);
    Int32 (*createWindow)(IInterface*, Boolean, Boolean);
    void (*closeWindow)(IInterface*, IInterface*);
    void (*decidePolicyForFormResubmission)(IInterface*, Int32);
    void (*requestFocus)(IInterface*);
    Elastos::String (*getRawResFilename)(IInterface*, Int32);
    Float (*density)(IInterface*);
    Int32 (*getFileSize)(IInterface* obj, const Elastos::String& uri);
    Int32 (*getFile)(IInterface*, const Elastos::String&, ArrayOf<Byte>*, Int32, Int32);
    void (*didReceiveAuthenticationChallenge)(IInterface*, Int32, const Elastos::String&, const Elastos::String&, Boolean, Boolean);
    void (*reportSslCertError)(IInterface*, Int32, Int32, ArrayOf<Byte>*, const Elastos::String&);
    void (*requestClientCert)(IInterface*, Int32, const Elastos::String&);
    void (*downloadStart)(IInterface*, const Elastos::String&, const Elastos::String&, const Elastos::String&, const Elastos::String&, const Elastos::String&,Int64);
    void (*didReceiveData)(IInterface*, ArrayOf<Byte>*, Int32);
    void (*didFinishLoading)(IInterface*);
    void (*setCertificate)(IInterface*, ArrayOf<Byte>*);
    Boolean (*shouldSaveFormData)(IInterface*);
    void (*saveFormData)(IInterface*, IMap*);
    void (*autoLogin)(IInterface*,const Elastos::String&,const Elastos::String&,const Elastos::String&);
    void (*setFrameField)(IInterface*, Int32);
    Int32 (*getFrameField)(Int32);
    struct ElaWebViewCoreCallback* webViewCoreCallback;
};

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_WebFrame_CallPolicyFunction(Int32 obj, Int32 func, Int32 decision);
    extern void Elastos_WebFrame_CreateFrame(Int32 callback,
                                                AutoPtr<IWeakReference> obj,
                                                AutoPtr<IWeakReference> javaview,
                                                Int32 jAssetManager,
                                                AutoPtr<IWeakReference> historyList);
   extern void Elastos_WebFrame_DestroyFrame(Int32 obj);
   extern void Elastos_WebFrame_StopLoading(Int32 obj);
   extern void Elastos_WebFrame_LoadUrl(Int32 obj,
                                           const Elastos::String& url,
                                           IMap* extraHeaders);
   extern void Elastos_WebFrame_PostUrl(Int32 obj,
                                           const Elastos::String& url,
                                           ArrayOf<Byte>* postData);
   extern void Elastos_WebFrame_LoadData(Int32 obj,
                                            const Elastos::String& baseUrl,
                                            const Elastos::String& data,
                                            const Elastos::String& mimeType,
                                            const Elastos::String& encoding,
                                            const Elastos::String& failUrl);
   extern Elastos::String Elastos_WebFrame_SaveWebArchive(Int32 obj,
                                                             const Elastos::String& basename,
                                                             Boolean autoname);
   extern Elastos::String Elastos_WebFrame_ExternalRepresentation(Int32 obj);
   extern Elastos::String Elastos_WebFrame_DocumentAsText(Int32 obj);
   extern Elastos::String Elastos_WebFrame_ChildFramesAsText(Int32 obj);
   extern void Elastos_WebFrame_Reload(Int32 obj, Boolean allowStale);
   extern void Elastos_WebFrame_GoBackOrForward(Int32 obj, Int32 pos);
   extern void Elastos_WebFrame_AddJavascriptInterface(Int32 obj,
                                                          Int32 nativeFramePointer,
                                                          IInterface* javascriptObj,
                                                          const Elastos::String& interfaceName,
                                                          Boolean requireAnnotation);
   extern Elastos::String Elastos_WebFrame_StringByEvaluatingJavaScriptFromString(Int32 obj,
                                                                    Elastos::String script);
   extern void Elastos_WebFrame_ClearCache(Int32 obj);
   extern Boolean Elastos_WebFrame_DocumentHasImages(Int32 obj);
   extern Boolean Elastos_WebFrame_HasPasswordField(Int32 obj);
   extern AutoPtr< ArrayOf<String> > Elastos_WebFrame_GetUsernamePassword(Int32 obj);
   extern void Elastos_WebFrame_SetUsernamePassword(Int32 obj, Elastos::String username, Elastos::String password);
   extern void Elastos_WebFrame_OrientationChanged(Int32 obj, Int32 orientation);
   extern void Elastos_WebFrame_AuthenticationProceed(Int32 obj, Int32 handle,
                                                         Elastos::String jUsername, Elastos::String jPassword);
   extern void Elastos_WebFrame_AuthenticationCancel(Int32 obj, Int32 handle);
   extern void Elastos_WebFrame_SslCertErrorProceed(Int32 obj, Int32 handle);
   extern void Elastos_WebFrame_SslCertErrorCancel(Int32 obj, Int32 handle, Int32 cert_error);
   extern void Elastos_WebFrame_SslClientCertCtx(Int32 obj, Int32 handle, Int32 ctx,
                                                    ArrayOf< AutoPtr< ArrayOf<Byte> > >* chain);
   extern void Elastos_WebFrame_SslClientCertPKCS8(Int32 obj, Int32 handle, ArrayOf<Byte>* pkey,
                                     ArrayOf< AutoPtr< ArrayOf<Byte> > >* chain);
   extern Boolean Elastos_WebFrame_GetShouldStartScrolledRight(Int32 obj, Int32 browserFrame);
#ifdef __cplusplus
}
#endif

class CallbackProxy;
class CWebViewCore;
class WebViewDatabaseClassic;
class WebSettingsClassic;

class BrowserFrame
    : public HandlerBase
{
private:
    // Javascript interface object
    class JSObject
        : public ElLightRefBase
        , public IInterface
    {
    public:
        JSObject(
            /* [in] */ IInterface* object,
            /* [in] */ Boolean requireAnnotation)
            : mObject(object)
            , mRequireAnnotation(requireAnnotation)
        {}

        CAR_INTERFACE_DECL();

    public:
        AutoPtr<IInterface> mObject;
        Boolean mRequireAnnotation;
    };

    class ConfigCallback
        : public ElLightRefBase
        , public IComponentCallbacks
    {
    public:
        ConfigCallback(
            /* [in] */ IWindowManager* wm);

        CAR_INTERFACE_DECL();

        CARAPI_(void) AddHandler(
            /* [in] */ IHandler* h);

        CARAPI OnConfigurationChanged(
            /* [in] */ IConfiguration* newConfig);

        CARAPI OnLowMemory();

    private:
        List< AutoPtr<IWeakReference> > mHandlers;
        AutoPtr<IWindowManager> mWindowManager;
        Object mLock;
    };

    class MyHttpAuthHandler
        : public ElLightRefBase
        , public IHttpAuthHandler
        , public HttpAuthHandler
    {
    public:
        MyHttpAuthHandler(
            /* [in] */ const Int32 handle,
            /* [in] */ const Boolean useCachedCredentials,
            /* [in] */ const Boolean suppressDialog,
            /* [in] */ BrowserFrame* owner)
            : mHandle(handle)
            , mUseCachedCredentials(useCachedCredentials)
            , mSuppressDialog(suppressDialog)
            , mOwner(owner)
        {
            Handler::Init();
        }

        CAR_INTERFACE_DECL();

        IHANDLER_METHODS_DECL();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI UseHttpAuthUsernamePassword(
            /* [out] */ Boolean* result);

        CARAPI Proceed(
            /* [in] */ const String& username,
            /* [in] */ const String& password);

        CARAPI Cancel();

        CARAPI SuppressDialog(
            /* [out] */ Boolean* result);

    private:
        Int32 mHandle;
        Boolean mUseCachedCredentials;
        Boolean mSuppressDialog;
        BrowserFrame* mOwner;
    };

    class MySslErrorHandler
        : public ElRefBase
        , public ISslErrorHandler
        , public SslErrorHandler
    {
    private:
        class ProceedRunnable : public Runnable
        {
        public:
            ProceedRunnable(
                /* [in] */ MySslErrorHandler* owner);

            CARAPI Run();

        private:
            MySslErrorHandler* mOwner;
        };

        class CancelRunnable : public Runnable
        {
        public:
            CancelRunnable(
                /* [in] */ MySslErrorHandler* owner);

            CARAPI Run();

        private:
            MySslErrorHandler* mOwner;
        };

    public:
        MySslErrorHandler(
            /* [in] */ Int32 handle,
            /* [in] */ Int32 certError,
            /* [in] */ ISslError* sslError,
            /* [in] */ BrowserFrame* owner)
            : mSslError(sslError)
            , mHandle(handle)
            , mCertError(certError)
            , mOwner(owner)
        {}

        CAR_INTERFACE_DECL();

        IHANDLER_METHODS_DECL();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        CARAPI Proceed();

        CARAPI Cancel();

    private:
        AutoPtr<ISslError> mSslError;
        Int32 mHandle;
        Int32 mCertError;
        BrowserFrame* mOwner;
    };

public:
    /**
     * Create a new BrowserFrame to be used in an application.
     * @param context An application context to use when retrieving assets.
     * @param w A WebViewCore used as the view for this frame.
     * @param proxy A CallbackProxy for posting messages to the UI thread and
     *              querying a client for information.
     * @param settings A WebSettings object that holds all settings.
     * XXX: Called by WebCore thread.
     */
    BrowserFrame(
        /* [in] */ IContext* context,
        /* [in] */ CWebViewCore* w,
        /* [in] */ CallbackProxy* proxy,
        /* [in] */ WebSettingsClassic* settings,
        /* [in] */ IMap* javascriptInterfaces);

    ~BrowserFrame();

    CAR_INTERFACE_DECL()

    /**
     * Load a url from the network or the filesystem into the main frame.
     * Following the same behaviour as Safari, javascript: URLs are not passed
     * to the main frame, instead they are evaluated immediately.
     * @param url The url to load.
     * @param extraHeaders The extra headers sent with this url. This should not
     *            include the common headers like "user-agent". If it does, it
     *            will be replaced by the intrinsic value of the WebView.
     */
    CARAPI_(void) LoadUrl(
        /* [in] */ const String& url,
        /* [in] */ IMap* extraHeaders);

    /**
     * Load a url with "POST" method from the network into the main frame.
     * @param url The url to load.
     * @param data The data for POST request.
     */
    CARAPI_(void) PostUrl(
        /* [in] */ const String& url,
        /* [in] */ ArrayOf<Byte>* data);

    /**
     * Load the content as if it was loaded by the provided base URL. The
     * historyUrl is used as the history entry for the load data.
     *
     * @param baseUrl Base URL used to resolve relative paths in the content
     * @param data Content to render in the browser
     * @param mimeType Mimetype of the data being passed in
     * @param encoding Character set encoding of the provided data.
     * @param historyUrl URL to use as the history entry.
     */
    CARAPI_(void) LoadData(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    /**
     * Saves the contents of the frame as a web archive.
     *
     * @param basename The filename where the archive should be placed.
     * @param autoname If false, takes filename to be a file. If true, filename
     *                 is assumed to be a directory in which a filename will be
     *                 chosen according to the url of the current page.
     */
    /* package */
    CARAPI_(String) SaveWebArchive(
        /* [in] */ const String& basename,
        /* [in] */ Boolean autoname);

    /**
     * Go back or forward the number of steps given.
     * @param steps A negative or positive number indicating the direction
     *              and number of steps to move.
     */
    CARAPI_(void) GoBackOrForward(
        /* [in] */ Int32 steps);

    /* package */
    CARAPI_(Boolean) Committed();

    /* package */
    CARAPI_(Boolean) FirstLayoutDone();

    /* package */
    CARAPI_(Int32) LoadType();

    /* package */
    CARAPI_(void) DidFirstLayout();

    /**
     * Destroy all native components of the BrowserFrame.
     */
    CARAPI_(void) Destroy();

    /**
     * Handle messages posted to us.
     * @param msg The message to handle.
     */
    CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    /**
     * Retrieves the render tree of this frame and puts it as the object for
     * the message and sends the message.
     * @param callback the message to use to send the render tree
     */
    CARAPI_(void) ExternalRepresentation(
        /* [in] */ IMessage* callback);

    /**
     * Retrieves the visual text of the frames, puts it as the object for
     * the message and sends the message.
     * @param callback the message to use to send the visual text
     */
    CARAPI_(void) DocumentAsText(
        /* [in] */ IMessage* callback);

    /**
     * This method is called by WebCore to check whether application
     * wants to hijack url loading
     */
    CARAPI_(Boolean) HandleUrl(
        /* [in] */ const String& url);

    CARAPI_(void) AddJavascriptInterface(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& interfaceName,
        /* [in] */ Boolean requireAnnotation);

    CARAPI_(void) RemoveJavascriptInterface(
        /* [in] */ const String& interfaceName);

    /**
     * Get the CallbackProxy for sending messages to the UI thread.
     */
    /* package */
    CARAPI_(AutoPtr<CallbackProxy>) GetCallbackProxy();

    /**
     * Returns the User Agent used by this frame
     */
    CARAPI_(String) GetUserAgentString();


    /* package */
    static CARAPI_(String) GetRawResFilename(
        /* [in] */ Int32 id,
        /* [in] */ IContext* context);

    //==========================================================================
    // native functions
    //==========================================================================

    /**
     * Destroy the native frame.
     */
    CARAPI_(void) NativeDestroyFrame();

    /**
     * Reload the current main frame.
     */
    CARAPI_(void) Reload(
        /* [in] */ Boolean allowStale);

    /**
     * stringByEvaluatingJavaScriptFromString will execute the
     * JS passed in in the context of this browser frame.
     * @param script A javascript string to execute
     *
     * @return string result of execution or null
     */
    CARAPI_(String) StringByEvaluatingJavaScriptFromString(
        /* [in] */ const String& script);

    CARAPI_(void) ClearCache();

    /**
     * Stop loading the current page.
     */
    CARAPI_(void) StopLoading();

    /**
     * Return true if the document has images.
     */
    CARAPI_(Boolean) DocumentHasImages();

    CARAPI_(void) NativeSslClientCert(
        /* [in] */ Int32 handle,
        /* [in] */ Int32 ctx,
        /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* asn1DerEncodedCertificateChain);

    CARAPI_(void) NativeSslClientCert(
        /* [in] */ Int32 handle,
        /* [in] */ ArrayOf<Byte>* pkey,
        /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* asn1DerEncodedCertificateChain);

    /**
     * Returns true when the contents of the frame is an RTL or vertical-rl
     * page. This is used for determining whether a frame should be initially
     * scrolled right-most as opposed to left-most.
     * @return true when the frame should be initially scrolled right-most
     * based on the text direction and writing mode.
     */
    /* package */
    CARAPI_(Boolean) GetShouldStartScrolledRight();

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

private:
    /**
     * native callback
     * Report an error to an activity.
     * @param errorCode The HTTP error code.
     * @param description Optional human-readable description. If no description
     *     is given, we'll use a standard localized error message.
     * @param failingUrl The URL that was being loaded when the error occurred.
     * TODO: Report all errors including resource errors but include some kind
     * of domain identifier. Change errorCode to an enum for a cleaner
     * interface.
     */
    CARAPI_(void) ReportError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    CARAPI_(void) ResetLoadingStates();

    /**
     * native callback
     * Indicates the beginning of a new load.
     * This method will be called once for the main frame.
     */
    CARAPI_(void) LoadStarted(
        /* [in] */ const String& url,
        /* [in] */ IBitmap* favicon,
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    CARAPI_(void) SaveFormData(
        /* [in] */ IMap* data);

    CARAPI_(Boolean) ShouldSaveFormData();

    /**
     * native callback
     * Indicates the WebKit has committed to the new load
     */
    CARAPI_(void) TransitionToCommitted(
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    /**
     * native callback
     * <p>
     * Indicates the end of a new load.
     * This method will be called once for the main frame.
     */
    CARAPI_(void) LoadFinished(
        /* [in] */ const String& url,
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    /**
     * Punch-through for WebCore to set the document
     * title. Inform the Activity of the new title.
     * @param title The new title of the document.
     */
    CARAPI_(void) SetTitle(
        /* [in] */ const String& title);

    /**
     * Return the render tree as a string
     */
    CARAPI_(String) ExternalRepresentation();

    /**
     * Return the text drawn on the screen as a string
     */
    CARAPI_(String) DocumentAsText();

    /**
     * Return the text drawn on the child frames as a string
     */
    CARAPI_(String) ChildFramesAsText();

    /*
     * This method is called by WebCore to inform the frame that
     * the Javascript window object has been cleared.
     * We should re-attach any attached js interfaces.
     */
    CARAPI_(void) WindowObjectCleared(
        /* [in] */ Int32 nativeFramePointer);

    /*
     * Add javascript objects to the internal list of objects. The default behavior
     * is to allow access to inherited methods (no annotation needed). This is only
     * used when js objects are passed through a constructor (via a hidden constructor).
     *
     * @TODO change the default behavior to be compatible with the public addjavascriptinterface
     */
    CARAPI_(void) AddJavaScriptObjects(
        /* [in] */ IMap* javascriptInterfaces);

    /**
     * Called by JNI.  Given a URI, find the associated file and return its size
     * @param uri A String representing the URI of the desired file.
     * @return Int32 The size of the given file.
     */
    CARAPI_(Int32) GetFileSize(
        /* [in] */ const String& uri);

    /**
     * Called by JNI.  Given a URI, a buffer, and an offset into the buffer,
     * copy the resource into buffer.
     * @param uri A String representing the URI of the desired file.
     * @param buffer The byte array to copy the data into.
     * @param offset The offet into buffer to place the data.
     * @param expectedSize The size that the buffer has allocated for this file.
     * @return Int32 The size of the given file, or zero if it fails.
     */
    CARAPI_(Int32) GetFile(
        /* [in] */ const String& uri,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 expectedSize);

    /**
     * Get the InputStream for an Android resource
     * There are three different kinds of android resources:
     * - file:///android_res
     * - file:///android_asset
     * - content://
     * @param url The url to load.
     * @return An InputStream to the android resource
     */
    CARAPI_(AutoPtr<IInputStream>) InputStreamForAndroidResource(
        /* [in] */ const String& url);

    /**
     * If this looks like a POST request (form submission) containing a username
     * and password, give the user the option of saving them. Will either do
     * nothing, or block until the UI interaction is complete.
     *
     * Called directly by WebKit.
     *
     * @param postData The data about to be sent as the body of a POST request.
     * @param username The username entered by the user (sniffed from the DOM).
     * @param password The password entered by the user (sniffed from the DOM).
     */
    CARAPI_(void) MaybeSavePassword(
        /* [in] */ ArrayOf<Byte>* postData,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    // Called by jni from the chrome network stack.
    CARAPI_(AutoPtr<IWebResourceResponse>) ShouldInterceptRequest(
        /* [in] */ const String& url);

    /**
     * Set the progress for the browser activity.  Called by native code.
     * Uses a delay so it does not happen too often.
     * @param newProgress An Int32 between zero and one hundred representing
     *                    the current progress percentage of loading the page.
     */
    CARAPI_(void) SetProgress(
        /* [in] */ Int32 newProgress);

    /**
     * Send the icon to the activity for display.
     * @param icon A Bitmap representing a page's favicon.
     */
    CARAPI_(void) DidReceiveIcon(
        /* [in] */ IBitmap* icon);

    // Called by JNI when an apple-touch-icon attribute was found.
    CARAPI_(void) DidReceiveTouchIconUrl(
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    /**
     * Request a new window from the client.
     * @return The BrowserFrame object stored in the new WebView.
     */
    CARAPI_(AutoPtr<BrowserFrame>) CreateWindow(
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture);

    /**
     * Try to focus this WebView.
     */
    CARAPI_(void) RequestFocus();

    /**
     * Close this frame and window.
     */
    CARAPI_(void) CloseWindow(
        /* [in] */ CWebViewCore* w);

    CARAPI_(void) DecidePolicyForFormResubmission(
        /* [in] */ Int32 policyFunction);

    /**
     * Tell the activity to update its global history.
     */
    CARAPI_(void) UpdateVisitedHistory(
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    CARAPI_(String) GetRawResFilename(
        /* [in] */ Int32 id);

    CARAPI_(Float) Density();

    /**
     * Called by JNI when the native HTTP stack gets an authentication request.
     *
     * We delegate the request to CallbackProxy, and route its response to
     * {@link #nativeAuthenticationProceed(int, String, String)} or
     * {@link #nativeAuthenticationCancel(int)}.
     *
     * We don't care what thread the callback is invoked on. All threading is
     * handled on the C++ side, because the WebKit thread may be blocked on a
     * synchronous call and unable to pump our MessageQueue.
     */
    CARAPI_(void) DidReceiveAuthenticationChallenge(
        /* [in] */ const Int32 handle,
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const Boolean useCachedCredentials,
        /* [in] */ const Boolean suppressDialog);

    /**
     * Called by JNI when the Chromium HTTP stack gets an invalid certificate chain.
     *
     * We delegate the request to CallbackProxy, and route its response to
     * {@link #nativeSslCertErrorProceed(int)} or
     * {@link #nativeSslCertErrorCancel(int, int)}.
     */
    CARAPI_(void) ReportSslCertError(
        /* [in] */ const Int32 handle,
        /* [in] */ const Int32 certError,
        /* [in] */ ArrayOf<Byte>* certDER,
        /* [in] */ const String& url);

    /**
     * Called by JNI when the native HTTPS stack gets a client
     * certificate request.
     *
     * We delegate the request to CallbackProxy, and route its response to
     * {@link #nativeSslClientCert(int, X509Certificate)}.
     */
    CARAPI_(void) RequestClientCert(
        /* [in] */ Int32 handle,
        /* [in] */ const String& hostAndPort);

    /**
     * Called by JNI when the native HTTP stack needs to download a file.
     *
     * We delegate the request to CallbackProxy, which owns the current app's
     * DownloadListener.
     */
    CARAPI_(void) DownloadStart(
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& referer,
        /* [in] */ Int64 contentLength);

    /**
     * Called by JNI for Chrome HTTP stack when the Java side needs to access the data.
     */
    CARAPI_(void) DidReceiveData(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 size);

    CARAPI_(void) DidFinishLoading();

    /**
     * Called by JNI when we recieve a certificate for the page's main resource.
     * Used by the Chromium HTTP stack only.
     */
    CARAPI_(void) SetCertificate(
        /* [in] */ ArrayOf<Byte>* cert_der);

    /**
     * Called by JNI when processing the X-Auto-Login header.
     */
    CARAPI_(void) AutoLogin(
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args);

    /**
     * Create a new native frame for a given WebView
     * @param w     A WebView that the frame draws into.
     * @param am    AssetManager to use to get assets.
     * @param list  The native side will add and remove items from this list as
     *              the native list changes.
     */
    CARAPI_(void) NativeCreateFrame(
        /* [in] */ CWebViewCore* w,
        /* [in] */ IAssetManager* am,
        /* [in] */ IWebBackForwardList* list);

    CARAPI_(void) NativeCallPolicyFunction(
        /* [in] */ Int32 policyFunction,
        /* [in] */ Int32 decision);

    /**
     * Go back or forward the number of steps given.
     * @param steps A negative or positive number indicating the direction
     *              and number of steps to move.
     */
    CARAPI_(void) NativeGoBackOrForward(
        /* [in] */ Int32 steps);

    /**
     * Add a javascript interface to the main frame.
     */
    CARAPI_(void) NativeAddJavascriptInterface(
        /* [in] */ Int32 nativeFramePointer,
        /* [in] */ IInterface* obj,
        /* [in] */ const String& interfaceName,
        /* [in] */ Boolean requireAnnotation);

    /**
     * Returns false if the url is bad.
     */
    CARAPI_(void) NativeLoadUrl(
        /* [in] */ const String& url,
        /* [in] */ IMap* headers);

    CARAPI_(void) NativePostUrl(
        /* [in] */ const String& url,
        /* [in] */ ArrayOf<Byte>* postData);

    CARAPI_(void) NativeLoadData(
        /* [in] */ const String& baseUrl,
        /* [in] */ const String& data,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& encoding,
        /* [in] */ const String& historyUrl);

    CARAPI_(void) NativeStopLoading();

    /**
     * @return TRUE if there is a password field in the current frame
     */
    CARAPI_(Boolean) HasPasswordField();

    /**
     * Get username and password in the current frame. If found, String[0] is
     * username and String[1] is password. Otherwise return NULL.
     * @return String[]
     */
    CARAPI_(AutoPtr< ArrayOf<String> >) GetUsernamePassword();

    /**
     * Set username and password to the proper fields in the current frame
     * @param username
     * @param password
     */
    CARAPI_(void) SetUsernamePassword(
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI_(String) NativeSaveWebArchive(
        /* [in] */ const String& basename,
        /* [in] */ Boolean autoname);

    CARAPI_(void) NativeOrientationChanged(
        /* [in] */ Int32 orientation);

    CARAPI_(void) NativeAuthenticationProceed(
        /* [in] */ Int32 handle,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    CARAPI_(void) NativeAuthenticationCancel(
        /* [in] */ Int32 handle);

    CARAPI_(void) NativeSslCertErrorProceed(
        /* [in] */ Int32 handle);

    CARAPI_(void) NativeSslCertErrorCancel(
        /* [in] */ Int32 handle,
        /* [in] */ Int32 certError);

    CARAPI_(Boolean) NativeGetShouldStartScrolledRight(
        /* [in] */ Int32 nativeBrowserFrame);

    static CARAPI_(void) MaybeSavePassword(
        /* [in] */ IInterface* obj,
        /* [in] */ ArrayOf<Byte>* postData,
        /* [in] */ const String& username,
        /* [in] */ const String& password);

    /*
     *@param callback pointer to a pointer of struct ElaWebResourceResponseCallback
     */
    static CARAPI_(AutoPtr<IInterface>) ShouldInterceptRequest(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [out] */ Int32** callback);

    static CARAPI_(void) LoadStarted(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ IInterface* favicon,
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    static CARAPI_(void) TransitionToCommitted(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    static CARAPI_(void) LoadFinished(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ Int32 loadType,
        /* [in] */ Boolean isMainFrame);

    static CARAPI_(void) ReportError(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& description,
        /* [in] */ const String& failingUrl);

    static CARAPI_(void) SetTitle(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& title);

    static CARAPI_(void) WindowObjectCleared(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 nativeFramePointer);

    static CARAPI_(void) SetProgress(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 newProgress);

    static CARAPI_(void) DidReceiveIcon(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* icon);

    static CARAPI_(void) DidReceiveTouchIconUrl(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ Boolean precomposed);

    static CARAPI_(void) UpdateVisitedHistory(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ Boolean isReload);

    static CARAPI_(Boolean) HandleUrl(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url);

    static CARAPI_(Int32) CreateWindow(
        /* [in] */ IInterface* obj,
        /* [in] */ Boolean dialog,
        /* [in] */ Boolean userGesture);

    static CARAPI_(void) CloseWindow(
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* w);

    static CARAPI_(void) DecidePolicyForFormResubmission(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 policyFunction);

    static CARAPI_(void) RequestFocus(
        /* [in] */ IInterface* obj);

    static CARAPI_(String) GetRawResFilename(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 id);

    static CARAPI_(Float) Density(
        /* [in] */ IInterface* obj);

    static CARAPI_(Int32) GetFileSize(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& uri);

    static CARAPI_(Int32) GetFile(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& uri,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 expectedSize);

    static CARAPI_(void) DidReceiveAuthenticationChallenge(
        /* [in] */ IInterface* obj,
        /* [in] */ const Int32 handle,
        /* [in] */ const String& host,
        /* [in] */ const String& realm,
        /* [in] */ const Boolean useCachedCredentials,
        /* [in] */ const Boolean suppressDialog);

    static CARAPI_(void) ReportSslCertError(
        /* [in] */ IInterface* obj,
        /* [in] */ const Int32 handle,
        /* [in] */ const Int32 certError,
        /* [in] */ ArrayOf<Byte>* certDER,
        /* [in] */ const String& url);

    static CARAPI_(void) RequestClientCert(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 handle,
        /* [in] */ const String& hostAndPort);

    static CARAPI_(void) DownloadStart(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& url,
        /* [in] */ const String& userAgent,
        /* [in] */ const String& contentDisposition,
        /* [in] */ const String& mimeType,
        /* [in] */ const String& referer,
        /* [in] */ Int64 contentLength);

    static CARAPI_(void) DidReceiveData(
        /* [in] */ IInterface* obj,
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 size);

    static CARAPI_(void) DidFinishLoading(
        /* [in] */ IInterface* obj);

    static CARAPI_(void) SetCertificate(
        /* [in] */ IInterface* obj,
        /* [in] */ ArrayOf<Byte>* cert_der);

    static CARAPI_(Boolean) ShouldSaveFormData(
        /* [in] */ IInterface* obj);

    static CARAPI_(void) SaveFormData(
        /* [in] */ IInterface* obj,
        /* [in] */ IMap* data);

    static CARAPI_(void) AutoLogin(
        /* [in] */ IInterface* obj,
        /* [in] */ const String& realm,
        /* [in] */ const String& account,
        /* [in] */ const String& args);

    static CARAPI_(void) SetFrameField(
        /* [in] */ IInterface* obj,
        /* [in] */ Int32 frame);

    static CARAPI_(Int32) GetFrameField(
        /* [in] */ Int32 obj);
public:
    /* package */ Boolean mLoadInitFromElastos;

    // message ids
    // a message posted when a frame loading is completed
    static const Int32 FRAME_COMPLETED = 1001;
    // orientation change message
    static const Int32 ORIENTATION_CHANGED = 1002;
    // a message posted when the user decides the policy
    static const Int32 POLICY_FUNCTION = 1003;

    // Note: need to keep these in sync with FrameLoaderTypes.h in native
    static const Int32 FRAME_LOADTYPE_STANDARD = 0;
    static const Int32 FRAME_LOADTYPE_BACK = 1;
    static const Int32 FRAME_LOADTYPE_FORWARD = 2;
    static const Int32 FRAME_LOADTYPE_INDEXEDBACKFORWARD = 3;
    static const Int32 FRAME_LOADTYPE_RELOAD = 4;
    static const Int32 FRAME_LOADTYPE_RELOADALLOWINGSTALEDATA = 5;
    static const Int32 FRAME_LOADTYPE_SAME = 6;
    static const Int32 FRAME_LOADTYPE_REDIRECT = 7;
    static const Int32 FRAME_LOADTYPE_REPLACE = 8;

    // This is a field accessed by native code as well as package classes.
    /*package*/ Int32 mNativeFrame;

    // Static instance of a WebCoreElastosBridge to handle timer and cookie
    // requests from WebCore.
    static AutoPtr<WebCoreElastosBridge> sElastosBridge;

    static AutoPtr<ConfigCallback> sConfigCallback;

        // XXX: Must match PolicyAction in FrameLoaderTypes.h in webcore
    static const Int32 POLICY_USE = 0;
    static const Int32 POLICY_IGNORE = 2;
    /* package */ static const Int32 DRAWABLEDIR = 3;

    static struct ElaWebFrameCallback sWebFrameCallback;

private:
    static const String LOGTAG;

    /**
     * Cap the number of LoadListeners that will be instantiated, so
     * we don't blow the GREF count.  Attempting to queue more than
     * this many requests will prompt an error() callback on the
     * request's LoadListener
     */
    const static Int32 MAX_OUTSTANDING_REQUESTS = 300;

    AutoPtr<CallbackProxy> mCallbackProxy;
    AutoPtr<WebSettingsClassic> mSettings;
    AutoPtr<IContext> mContext;
    AutoPtr<WebViewDatabaseClassic> mDatabase;
    CWebViewCore* mWebViewCore;
    Int32 mLoadType;
    Boolean mFirstLayoutDone;
    Boolean mCommitted;
    // Flag for blocking messages. This is used during destroy() so
    // that if the UI thread posts any messages after the message
    // queue has been cleared,they are ignored.
    Boolean mBlockMessages;
    Int32 mOrientation;

    // Is this frame the main frame?
    Boolean mIsMainFrame;

    // Attached Javascript interfaces
    HashMap<String, AutoPtr<JSObject> > mJavaScriptObjects;
    HashSet< AutoPtr<IInterface> > mRemovedJavaScriptObjects;

    // Key store handler when Chromium HTTP stack is used.
    AutoPtr<KeyStoreHandler> mKeyStoreHandler;

    // A progress threshold to switch from history Picture to live Picture
    static const Int32 TRANSITION_SWITCH_THRESHOLD = 75;

    // These ids need to be in sync with enum rawResId in PlatformBridge.h
    static const Int32 NODOMAIN = 1;
    static const Int32 LOADERROR = 2;
    static const Int32 FILE_UPLOAD_LABEL = 4;
    static const Int32 RESET_LABEL = 5;
    static const Int32 SUBMIT_LABEL = 6;
    static const Int32 FILE_UPLOAD_NO_FILE_CHOSEN = 7;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_BROWSERFRAME_H__
