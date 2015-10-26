
#include "elastos/droid/webkit/webview/chromium/WebViewChromium.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Concurrent::CConcurrentLinkedQueue;
using Elastos::Droid::Webkit::Base::ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

//=====================================================================
//  WebViewChromium::WebViewChromiumRunQueue::InnerDrainQueueRunnable
//=====================================================================
CAR_INTERFACE_IMPL(WebViewChromium::WebViewChromiumRunQueue::InnerDrainQueueRunnable, Object, IRunnable)

WebViewChromium::WebViewChromiumRunQueue::InnerDrainQueueRunnable::InnerDrainQueueRunnable(
    /* [in] */ WebViewChromiumRunQueue* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::WebViewChromiumRunQueue::InnerDrainQueueRunnable::Run()
{
    // ==================before translated======================
    // drainQueue();

    assert(NULL == mOwner);
    mOwner->DrainQueue();
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::WebViewChromiumRunQueue
//=====================================================================
WebViewChromium::WebViewChromiumRunQueue::WebViewChromiumRunQueue(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mQueue = new ConcurrentLinkedQueue<Runnable>();

    CConcurrentLinkedQueue::New((IQueue**)&mQueue);
}

ECode WebViewChromium::WebViewChromiumRunQueue::AddTask(
    /* [in] */ IRunnable* task)
{
    VALIDATE_NOT_NULL(task);
    // ==================before translated======================
    // mQueue.add(task);
    // if (mFactory.hasStarted()) {
    //     ThreadUtils.runOnUiThread(new Runnable() {
    //         @Override
    //         public void run() {
    //             drainQueue();
    //         }
    //     });
    // }

    assert(0);
    assert(NULL == mOwner);
    mQueue->Add(task);
    //if (mOwner->mFactory->HasStarted()) {
        AutoPtr<IRunnable> runnable = new WebViewChromiumRunQueue::InnerDrainQueueRunnable(this);
        ThreadUtils::RunOnUiThread(runnable);
    //}
    return NOERROR;
}

ECode WebViewChromium::WebViewChromiumRunQueue::DrainQueue()
{
    // ==================before translated======================
    // if (mQueue == null || mQueue.isEmpty()) {
    //     return;
    // }
    //
    // Runnable task = mQueue.poll();
    // while(task != null) {
    //     task.run();
    //     task = mQueue.poll();
    // }

    Boolean empty = FALSE;
    mQueue->IsEmpty(&empty);
    if (mQueue == NULL || empty) {
        return NOERROR;
    }

    AutoPtr<IInterface> taskTmp;
    mQueue->Poll((IInterface**)&taskTmp);
    IRunnable* task = IRunnable::Probe(taskTmp);
    while(task != NULL) {
        task->Run();
        taskTmp = NULL;
        mQueue->Poll((IInterface**)&taskTmp);
        task = IRunnable::Probe(taskTmp);
    }
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerInitForRealRunnable
//=====================================================================
WebViewChromium::InnerInitForRealRunnable::InnerInitForRealRunnable(
    /* [in] */ WebViewChromium* owner,
    /* [in] */ const Boolean privateBrowsing)
    : mOwner(owner)
    , mPrivateBrowsing(privateBrowsing)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerInitForRealRunnable::Run()
{
    // ==================before translated======================
    // initForReal();
    // if (privateBrowsing) {
    //     // Intentionally irreversibly disable the webview instance, so that private
    //     // user data cannot leak through misuse of a non-privateBrowing WebView
    //     // instance. Can't just null out mAwContents as we never null-check it
    //     // before use.
    //     destroy();
    // }

    mOwner->InitForReal();
    if (mPrivateBrowsing) {
        // Intentionally irreversibly disable the webview instance, so that private
        // user data cannot leak through misuse of a non-privateBrowing WebView
        // instance. Can't just null out mAwContents as we never null-check it
        // before use.
        mOwner->Destroy();
    }
    return NOERROR;
}

//=====================================================================
//       WebViewChromium::InnerContextWrapper::InnerClassLoader
//=====================================================================
WebViewChromium::InnerContextWrapper::InnerClassLoader::InnerClassLoader(
    /* [in] */ InnerContextWrapper* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerContextWrapper::InnerClassLoader::FindClass(
    /* [in] */ const String& name,
    /* [out] */ IClassInfo** klass)
{
    VALIDATE_NOT_NULL(klass);
    // ==================before translated======================
    // // First look in the WebViewProvider class loader.
    // try {
    //     return webViewCl.loadClass(name);
    // } catch (ClassNotFoundException e) {
    //     // Look in the app class loader; allowing it to throw ClassNotFoundException.
    //     return appCl.loadClass(name);
    // }

    assert(0);
    // First look in the WebViewProvider class loader.
//    try {
//        return webViewCl.loadClass(name);
//    }
//    catch (ClassNotFoundException e) {
//        // Look in the app class loader; allowing it to throw ClassNotFoundException.
//        return appCl.loadClass(name);
//    }
//    AutoPtr<Class> empty;
    return NOERROR;
}

//=====================================================================
//                WebViewChromium::InnerContextWrapper
//=====================================================================
WebViewChromium::InnerContextWrapper::InnerContextWrapper(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

AutoPtr<IClassLoader> WebViewChromium::InnerContextWrapper::GetClassLoader()
{
    // ==================before translated======================
    // final ClassLoader appCl = getBaseContext().getClassLoader();
    // final ClassLoader webViewCl = this.getClass().getClassLoader();
    // return new ClassLoader() {
    //     @Override
    //     protected Class<?> findClass(String name) throws ClassNotFoundException {
    //         // First look in the WebViewProvider class loader.
    //         try {
    //             return webViewCl.loadClass(name);
    //         } catch (ClassNotFoundException e) {
    //             // Look in the app class loader; allowing it to throw ClassNotFoundException.
    //             return appCl.loadClass(name);
    //         }
    //     }
    // };
    assert(0);
    AutoPtr<IClassLoader> empty;
    return empty;
}

ECode WebViewChromium::InnerContextWrapper::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    // ==================before translated======================
    // if (name.equals(Context.LAYOUT_INFLATER_SERVICE)) {
    //     LayoutInflater i = (LayoutInflater) getBaseContext().getSystemService(name);
    //     return i.cloneInContext(this);
    // } else {
    //     return getBaseContext().getSystemService(name);
    // }
    assert(0);
    // AutoPtr<Object> empty;
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerThreadViolationRunnable
//=====================================================================
WebViewChromium::InnerThreadViolationRunnable::InnerThreadViolationRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerThreadViolationRunnable::Run()
{
    // ==================before translated======================
    // throw threadViolation;
    assert(0);
    return NOERROR;
}

//=====================================================================
//     WebViewChromium::InnerSetHorizontalScrollbarOverlayRunnable
//=====================================================================
WebViewChromium::InnerSetHorizontalScrollbarOverlayRunnable::InnerSetHorizontalScrollbarOverlayRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetHorizontalScrollbarOverlayRunnable::Run()
{
    // ==================before translated======================
    // setHorizontalScrollbarOverlay(overlay);
    assert(0);
    return NOERROR;
}

//=====================================================================
//      WebViewChromium::InnerSetVerticalScrollbarOverlayRunnable
//=====================================================================
WebViewChromium::InnerSetVerticalScrollbarOverlayRunnable::InnerSetVerticalScrollbarOverlayRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetVerticalScrollbarOverlayRunnable::Run()
{
    // ==================before translated======================
    // setVerticalScrollbarOverlay(overlay);
    assert(0);
    return NOERROR;
}

//=====================================================================
//      WebViewChromium::InnerOverlayHorizontalScrollbarCallable
//=====================================================================
WebViewChromium::InnerOverlayHorizontalScrollbarCallable::InnerOverlayHorizontalScrollbarCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOverlayHorizontalScrollbarCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return overlayHorizontalScrollbar();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//        WebViewChromium::InnerOverlayVerticalScrollbarCallable
//=====================================================================
WebViewChromium::InnerOverlayVerticalScrollbarCallable::InnerOverlayVerticalScrollbarCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOverlayVerticalScrollbarCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return overlayVerticalScrollbar();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerGetCertificateCallable
//=====================================================================
WebViewChromium::InnerGetCertificateCallable::InnerGetCertificateCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetCertificateCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getCertificate();
    assert(0);
    // AutoPtr<ISslCertificate> empty;
    return NOERROR;
}

//=====================================================================
//       WebViewChromium::InnerSetHttpAuthUsernamePasswordRunnable
//=====================================================================
WebViewChromium::InnerSetHttpAuthUsernamePasswordRunnable::InnerSetHttpAuthUsernamePasswordRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetHttpAuthUsernamePasswordRunnable::Run()
{
    // ==================before translated======================
    // setHttpAuthUsernamePassword(host, realm, username, password);
    assert(0);
    return NOERROR;
}

//=====================================================================
//      WebViewChromium::InnerGetHttpAuthUsernamePasswordCallable
//=====================================================================
WebViewChromium::InnerGetHttpAuthUsernamePasswordCallable::InnerGetHttpAuthUsernamePasswordCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetHttpAuthUsernamePasswordCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getHttpAuthUsernamePassword(host, realm);
    assert(0);
    // AutoPtr< ArrayOf<String> > empty;
    return NOERROR;
}

//=====================================================================
//                 WebViewChromium::InnerDestroyRunnable
//=====================================================================
WebViewChromium::InnerDestroyRunnable::InnerDestroyRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerDestroyRunnable::Run()
{
    // ==================before translated======================
    // destroy();
    assert(0);
    return NOERROR;
}

//=====================================================================
//        WebViewChromium::InnerSetNetworkAvailableRunnable
//=====================================================================
WebViewChromium::InnerSetNetworkAvailableRunnable::InnerSetNetworkAvailableRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetNetworkAvailableRunnable::Run()
{
    // ==================before translated======================
    // setNetworkAvailable(networkUp);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                WebViewChromium::InnerSaveStateCallable
//=====================================================================
WebViewChromium::InnerSaveStateCallable::InnerSaveStateCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSaveStateCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return saveState(outState);
    assert(0);
    // AutoPtr<IWebBackForwardList> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerRestoreStateCallable
//=====================================================================
WebViewChromium::InnerRestoreStateCallable::InnerRestoreStateCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerRestoreStateCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return restoreState(inState);
    assert(0);
    // AutoPtr<IWebBackForwardList> empty;
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerEvaluateJavaScriptRunnable
//=====================================================================
WebViewChromium::InnerEvaluateJavaScriptRunnable::InnerEvaluateJavaScriptRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerEvaluateJavaScriptRunnable::Run()
{
    // ==================before translated======================
    // mAwContents.evaluateJavaScriptEvenIfNotYetNavigated(
    //         url.substring(JAVASCRIPT_SCHEME.length()));
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerLoadUrlRunnable
//=====================================================================
WebViewChromium::InnerLoadUrlRunnable::InnerLoadUrlRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerLoadUrlRunnable::Run()
{
    // ==================before translated======================
    // mAwContents.loadUrl(loadUrlParams);
    assert(0);
    return NOERROR;
}

//=====================================================================
//            WebViewChromium::InnerSaveWebArchiveRunnable
//=====================================================================
WebViewChromium::InnerSaveWebArchiveRunnable::InnerSaveWebArchiveRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSaveWebArchiveRunnable::Run()
{
    // ==================before translated======================
    // saveWebArchive(basename, autoname, callback);
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerStopLoadingRunnable
//=====================================================================
WebViewChromium::InnerStopLoadingRunnable::InnerStopLoadingRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerStopLoadingRunnable::Run()
{
    // ==================before translated======================
    // stopLoading();
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerReloadRunnable
//=====================================================================
WebViewChromium::InnerReloadRunnable::InnerReloadRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerReloadRunnable::Run()
{
    // ==================before translated======================
    // reload();
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerCanGoBackCallable
//=====================================================================
WebViewChromium::InnerCanGoBackCallable::InnerCanGoBackCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerCanGoBackCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return canGoBack();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//                   WebViewChromium::InnerGoBackRunnable
//=====================================================================
WebViewChromium::InnerGoBackRunnable::InnerGoBackRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGoBackRunnable::Run()
{
    // ==================before translated======================
    // goBack();
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerCanGoForwardCallable
//=====================================================================
WebViewChromium::InnerCanGoForwardCallable::InnerCanGoForwardCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerCanGoForwardCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return canGoForward();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//                 WebViewChromium::InnerGoForwardRunnable
//=====================================================================
WebViewChromium::InnerGoForwardRunnable::InnerGoForwardRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGoForwardRunnable::Run()
{
    // ==================before translated======================
    // goForward();
    assert(0);
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerCanGoBackOrForwardCallable
//=====================================================================
WebViewChromium::InnerCanGoBackOrForwardCallable::InnerCanGoBackOrForwardCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerCanGoBackOrForwardCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return canGoBackOrForward(steps);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerGoBackOrForwardRunnable
//=====================================================================
WebViewChromium::InnerGoBackOrForwardRunnable::InnerGoBackOrForwardRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGoBackOrForwardRunnable::Run()
{
    // ==================before translated======================
    // goBackOrForward(steps);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                    WebViewChromium::InnerPageUpCallable
//=====================================================================
WebViewChromium::InnerPageUpCallable::InnerPageUpCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerPageUpCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return pageUp(top);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//                    WebViewChromium::InnerPageDownCallable
//=====================================================================
WebViewChromium::InnerPageDownCallable::InnerPageDownCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerPageDownCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return pageDown(bottom);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//                   WebViewChromium::InnerClearViewRunnable
//=====================================================================
WebViewChromium::InnerClearViewRunnable::InnerClearViewRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerClearViewRunnable::Run()
{
    // ==================before translated======================
    // clearView();
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerCapturePictureCallable
//=====================================================================
WebViewChromium::InnerCapturePictureCallable::InnerCapturePictureCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerCapturePictureCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return capturePicture();
    assert(0);
    // AutoPtr<Picture> empty;
    return NOERROR;
}

//=====================================================================
//             WebViewChromium::InnerInvokeZoomPickerRunnable
//=====================================================================
WebViewChromium::InnerInvokeZoomPickerRunnable::InnerInvokeZoomPickerRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerInvokeZoomPickerRunnable::Run()
{
    // ==================before translated======================
    // invokeZoomPicker();
    assert(0);
    return NOERROR;
}

//=====================================================================
//            WebViewChromium::InnerGetHitTestResultCallable
//=====================================================================
WebViewChromium::InnerGetHitTestResultCallable::InnerGetHitTestResultCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetHitTestResultCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getHitTestResult();
    assert(0);
    //AutoPtr<WebView.HitTestResult> empty;
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerRequestFocusNodeHrefRunnable
//=====================================================================
WebViewChromium::InnerRequestFocusNodeHrefRunnable::InnerRequestFocusNodeHrefRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerRequestFocusNodeHrefRunnable::Run()
{
    // ==================before translated======================
    // requestFocusNodeHref(hrefMsg);
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerRequestImageRefRunnable
//=====================================================================
WebViewChromium::InnerRequestImageRefRunnable::InnerRequestImageRefRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerRequestImageRefRunnable::Run()
{
    // ==================before translated======================
    // requestImageRef(msg);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                    WebViewChromium::InnerGetUrlCallable
//=====================================================================
WebViewChromium::InnerGetUrlCallable::InnerGetUrlCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetUrlCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getUrl();
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerGetOriginalUrlCallable
//=====================================================================
WebViewChromium::InnerGetOriginalUrlCallable::InnerGetOriginalUrlCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetOriginalUrlCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getOriginalUrl();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                WebViewChromium::InnerGetTitleCallable
//=====================================================================
WebViewChromium::InnerGetTitleCallable::InnerGetTitleCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetTitleCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getTitle();
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerGetFaviconCallable
//=====================================================================
WebViewChromium::InnerGetFaviconCallable::InnerGetFaviconCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetFaviconCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getFavicon();
    assert(0);
    // AutoPtr<IBitmap> empty;
    return NOERROR;
}

//=====================================================================
//             WebViewChromium::InnerPauseTimersRunnable
//=====================================================================
WebViewChromium::InnerPauseTimersRunnable::InnerPauseTimersRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerPauseTimersRunnable::Run()
{
    // ==================before translated======================
    // pauseTimers();
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerResumeTimersRunnable
//=====================================================================
WebViewChromium::InnerResumeTimersRunnable::InnerResumeTimersRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerResumeTimersRunnable::Run()
{
    // ==================before translated======================
    // resumeTimers();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                WebViewChromium::InnerOnPauseRunnable
//=====================================================================
WebViewChromium::InnerOnPauseRunnable::InnerOnPauseRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnPauseRunnable::Run()
{
    // ==================before translated======================
    // onPause();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                WebViewChromium::InnerOnResumeRunnable
//=====================================================================
WebViewChromium::InnerOnResumeRunnable::InnerOnResumeRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnResumeRunnable::Run()
{
    // ==================before translated======================
    // onResume();
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerIsPausedCallable
//=====================================================================
WebViewChromium::InnerIsPausedCallable::InnerIsPausedCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerIsPausedCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return isPaused();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerClearCacheRunnable
//=====================================================================
WebViewChromium::InnerClearCacheRunnable::InnerClearCacheRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerClearCacheRunnable::Run()
{
    // ==================before translated======================
    // clearCache(includeDiskFiles);
    assert(0);
    return NOERROR;
}

//=====================================================================
//            WebViewChromium::InnerClearFormDataRunnable
//=====================================================================
WebViewChromium::InnerClearFormDataRunnable::InnerClearFormDataRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerClearFormDataRunnable::Run()
{
    // ==================before translated======================
    // clearFormData();
    assert(0);
    return NOERROR;
}

//=====================================================================
//             WebViewChromium::InnerClearHistoryRunnable
//=====================================================================
WebViewChromium::InnerClearHistoryRunnable::InnerClearHistoryRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerClearHistoryRunnable::Run()
{
    // ==================before translated======================
    // clearHistory();
    assert(0);
    return NOERROR;
}

//=====================================================================
//          WebViewChromium::InnerClearSslPreferencesRunnable
//=====================================================================
WebViewChromium::InnerClearSslPreferencesRunnable::InnerClearSslPreferencesRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerClearSslPreferencesRunnable::Run()
{
    // ==================before translated======================
    // clearSslPreferences();
    assert(0);
    return NOERROR;
}

//=====================================================================
//          WebViewChromium::InnerCopyBackForwardListCallable
//=====================================================================
WebViewChromium::InnerCopyBackForwardListCallable::InnerCopyBackForwardListCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerCopyBackForwardListCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return copyBackForwardList();
    assert(0);
    // AutoPtr<IWebBackForwardList> empty;
    return NOERROR;
}

//=====================================================================
//                WebViewChromium::InnerFindNextRunnable
//=====================================================================
WebViewChromium::InnerFindNextRunnable::InnerFindNextRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerFindNextRunnable::Run()
{
    // ==================before translated======================
    // findNext(forwards);
    assert(0);
    return NOERROR;
}

//=====================================================================
//             WebViewChromium::InnerFindAllAsyncRunnable
//=====================================================================
WebViewChromium::InnerFindAllAsyncRunnable::InnerFindAllAsyncRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerFindAllAsyncRunnable::Run()
{
    // ==================before translated======================
    // findAllAsync(searchString);
    assert(0);
    return NOERROR;
}

//=====================================================================
//       WebViewChromium::InnerNotifyFindDialogDismissedRunnable
//=====================================================================
WebViewChromium::InnerNotifyFindDialogDismissedRunnable::InnerNotifyFindDialogDismissedRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerNotifyFindDialogDismissedRunnable::Run()
{
    // ==================before translated======================
    // notifyFindDialogDismissed();
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerClearMatchesRunnable
//=====================================================================
WebViewChromium::InnerClearMatchesRunnable::InnerClearMatchesRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerClearMatchesRunnable::Run()
{
    // ==================before translated======================
    // clearMatches();
    assert(0);
    return NOERROR;
}

//=====================================================================
//            WebViewChromium::InnerDocumentHasImagesRunnable
//=====================================================================
WebViewChromium::InnerDocumentHasImagesRunnable::InnerDocumentHasImagesRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerDocumentHasImagesRunnable::Run()
{
    // ==================before translated======================
    // documentHasImages(response);
    assert(0);
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerSetPictureListenerRunnable
//=====================================================================
WebViewChromium::InnerSetPictureListenerRunnable::InnerSetPictureListenerRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetPictureListenerRunnable::Run()
{
    // ==================before translated======================
    // setPictureListener(listener);
    assert(0);
    return NOERROR;
}

//=====================================================================
//          WebViewChromium::InnerAddJavascriptInterfaceRunnable
//=====================================================================
WebViewChromium::InnerAddJavascriptInterfaceRunnable::InnerAddJavascriptInterfaceRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerAddJavascriptInterfaceRunnable::Run()
{
    // ==================before translated======================
    // addJavascriptInterface(obj, interfaceName);
    assert(0);
    return NOERROR;
}

//=====================================================================
//       WebViewChromium::InnerRemoveJavascriptInterfaceRunnable
//=====================================================================
WebViewChromium::InnerRemoveJavascriptInterfaceRunnable::InnerRemoveJavascriptInterfaceRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerRemoveJavascriptInterfaceRunnable::Run()
{
    // ==================before translated======================
    // removeJavascriptInterface(interfaceName);
    assert(0);
    return NOERROR;
}

//=====================================================================
//             WebViewChromium::InnerFlingScrollRunnable
//=====================================================================
WebViewChromium::InnerFlingScrollRunnable::InnerFlingScrollRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerFlingScrollRunnable::Run()
{
    // ==================before translated======================
    // flingScroll(vx, vy);
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerZoomInCallable
//=====================================================================
WebViewChromium::InnerZoomInCallable::InnerZoomInCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerZoomInCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return zoomIn();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerZoomOutCallable
//=====================================================================
WebViewChromium::InnerZoomOutCallable::InnerZoomOutCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerZoomOutCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return zoomOut();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//     WebViewChromium::InnerShouldDelayChildPressedStateCallable
//=====================================================================
WebViewChromium::InnerShouldDelayChildPressedStateCallable::InnerShouldDelayChildPressedStateCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerShouldDelayChildPressedStateCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return shouldDelayChildPressedState();
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//      WebViewChromium::InnerGetAccessibilityNodeProviderCallable
//=====================================================================
WebViewChromium::InnerGetAccessibilityNodeProviderCallable::InnerGetAccessibilityNodeProviderCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerGetAccessibilityNodeProviderCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return getAccessibilityNodeProvider();
    assert(0);
    // AutoPtr<AccessibilityNodeProvider> empty;
    return NOERROR;
}

//=====================================================================
//   WebViewChromium::InnerOnInitializeAccessibilityNodeInfoRunnable
//=====================================================================
WebViewChromium::InnerOnInitializeAccessibilityNodeInfoRunnable::InnerOnInitializeAccessibilityNodeInfoRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnInitializeAccessibilityNodeInfoRunnable::Run()
{
    // ==================before translated======================
    // onInitializeAccessibilityNodeInfo(info);
    assert(0);
    return NOERROR;
}

//=====================================================================
//     WebViewChromium::InnerOnInitializeAccessibilityEventRunnable
//=====================================================================
WebViewChromium::InnerOnInitializeAccessibilityEventRunnable::InnerOnInitializeAccessibilityEventRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnInitializeAccessibilityEventRunnable::Run()
{
    // ==================before translated======================
    // onInitializeAccessibilityEvent(event);
    assert(0);
    return NOERROR;
}

//=====================================================================
//     WebViewChromium::InnerPerformAccessibilityActionCallable
//=====================================================================
WebViewChromium::InnerPerformAccessibilityActionCallable::InnerPerformAccessibilityActionCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerPerformAccessibilityActionCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return performAccessibilityAction(action, arguments);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//         WebViewChromium::InnerSetOverScrollModeRunnable
//=====================================================================
WebViewChromium::InnerSetOverScrollModeRunnable::InnerSetOverScrollModeRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetOverScrollModeRunnable::Run()
{
    // ==================before translated======================
    // setOverScrollMode(mode);
    assert(0);
    return NOERROR;
}

//=====================================================================
//        WebViewChromium::InnerSetScrollBarStyleRunnable
//=====================================================================
WebViewChromium::InnerSetScrollBarStyleRunnable::InnerSetScrollBarStyleRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetScrollBarStyleRunnable::Run()
{
    // ==================before translated======================
    // setScrollBarStyle(style);
    assert(0);
    return NOERROR;
}

//=====================================================================
//          WebViewChromium::InnerOnOverScrolledRunnable
//=====================================================================
WebViewChromium::InnerOnOverScrolledRunnable::InnerOnOverScrolledRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnOverScrolledRunnable::Run()
{
    // ==================before translated======================
    // onOverScrolled(scrollX, scrollY, clampedX, clampedY);
    assert(0);
    return NOERROR;
}

//=====================================================================
//      WebViewChromium::InnerOnWindowVisibilityChangedRunnable
//=====================================================================
WebViewChromium::InnerOnWindowVisibilityChangedRunnable::InnerOnWindowVisibilityChangedRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnWindowVisibilityChangedRunnable::Run()
{
    // ==================before translated======================
    // onWindowVisibilityChanged(visibility);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                   WebViewChromium::InnerOnDrawRunnable
//=====================================================================
WebViewChromium::InnerOnDrawRunnable::InnerOnDrawRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnDrawRunnable::Run()
{
    // ==================before translated======================
    // onDraw(canvas);
    assert(0);
    return NOERROR;
}

//=====================================================================
//        WebViewChromium::InnerOnConfigurationChangedRunnable
//=====================================================================
WebViewChromium::InnerOnConfigurationChangedRunnable::InnerOnConfigurationChangedRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnConfigurationChangedRunnable::Run()
{
    // ==================before translated======================
    // onConfigurationChanged(newConfig);
    assert(0);
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerOnKeyMultipleCallable
//=====================================================================
WebViewChromium::InnerOnKeyMultipleCallable::InnerOnKeyMultipleCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnKeyMultipleCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return onKeyMultiple(keyCode, repeatCount, event);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerOnKeyDownCallable
//=====================================================================
WebViewChromium::InnerOnKeyDownCallable::InnerOnKeyDownCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnKeyDownCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return onKeyDown(keyCode, event);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerOnKeyUpCallable
//=====================================================================
WebViewChromium::InnerOnKeyUpCallable::InnerOnKeyUpCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnKeyUpCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return onKeyUp(keyCode, event);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//         WebViewChromium::InnerOnDetachedFromWindowRunnable
//=====================================================================
WebViewChromium::InnerOnDetachedFromWindowRunnable::InnerOnDetachedFromWindowRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnDetachedFromWindowRunnable::Run()
{
    // ==================before translated======================
    // onDetachedFromWindow();
    assert(0);
    return NOERROR;
}

//=====================================================================
//        WebViewChromium::InnerOnVisibilityChangedRunnable
//=====================================================================
WebViewChromium::InnerOnVisibilityChangedRunnable::InnerOnVisibilityChangedRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnVisibilityChangedRunnable::Run()
{
    // ==================before translated======================
    // onVisibilityChanged(changedView, visibility);
    assert(0);
    return NOERROR;
}

//=====================================================================
//         WebViewChromium::InnerOnWindowFocusChangedRunnable
//=====================================================================
WebViewChromium::InnerOnWindowFocusChangedRunnable::InnerOnWindowFocusChangedRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnWindowFocusChangedRunnable::Run()
{
    // ==================before translated======================
    // onWindowFocusChanged(hasWindowFocus);
    assert(0);
    return NOERROR;
}

//=====================================================================
//            WebViewChromium::InnerOnFocusChangedRunnable
//=====================================================================
WebViewChromium::InnerOnFocusChangedRunnable::InnerOnFocusChangedRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnFocusChangedRunnable::Run()
{
    // ==================before translated======================
    // onFocusChanged(focused, direction, previouslyFocusedRect);
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerOnSizeChangedRunnable
//=====================================================================
WebViewChromium::InnerOnSizeChangedRunnable::InnerOnSizeChangedRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnSizeChangedRunnable::Run()
{
    // ==================before translated======================
    // onSizeChanged(w, h, ow, oh);
    assert(0);
    return NOERROR;
}

//=====================================================================
//           WebViewChromium::InnerDispatchKeyEventCallable
//=====================================================================
WebViewChromium::InnerDispatchKeyEventCallable::InnerDispatchKeyEventCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerDispatchKeyEventCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return dispatchKeyEvent(event);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerOnTouchEventCallable
//=====================================================================
WebViewChromium::InnerOnTouchEventCallable::InnerOnTouchEventCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnTouchEventCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return onTouchEvent(ev);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerOnHoverEventCallable
//=====================================================================
WebViewChromium::InnerOnHoverEventCallable::InnerOnHoverEventCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnHoverEventCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return onHoverEvent(event);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//          WebViewChromium::InnerOnGenericMotionEventCallable
//=====================================================================
WebViewChromium::InnerOnGenericMotionEventCallable::InnerOnGenericMotionEventCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnGenericMotionEventCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return onGenericMotionEvent(event);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//                WebViewChromium::InnerRequestFocusCallable
//=====================================================================
WebViewChromium::InnerRequestFocusCallable::InnerRequestFocusCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerRequestFocusCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return requestFocus(direction, previouslyFocusedRect);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::InnerOnMeasureRunnable
//=====================================================================
WebViewChromium::InnerOnMeasureRunnable::InnerOnMeasureRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerOnMeasureRunnable::Run()
{
    // ==================before translated======================
    // onMeasure(widthMeasureSpec, heightMeasureSpec);
    assert(0);
    return NOERROR;
}

//=====================================================================
//       WebViewChromium::InnerRequestChildRectangleOnScreenCallable
//=====================================================================
WebViewChromium::InnerRequestChildRectangleOnScreenCallable::InnerRequestChildRectangleOnScreenCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerRequestChildRectangleOnScreenCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return requestChildRectangleOnScreen(child, rect, immediate);
    assert(0);
    // AutoPtr<Boolean> empty;
    return NOERROR;
}

//=====================================================================
//         WebViewChromium::InnerSetBackgroundColorRunnable
//=====================================================================
WebViewChromium::InnerSetBackgroundColorRunnable::InnerSetBackgroundColorRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetBackgroundColorRunnable::Run()
{
    // ==================before translated======================
    // setBackgroundColor(color);
    assert(0);
    return NOERROR;
}

//=====================================================================
//              WebViewChromium::InnerSetLayerTypeRunnable
//=====================================================================
WebViewChromium::InnerSetLayerTypeRunnable::InnerSetLayerTypeRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerSetLayerTypeRunnable::Run()
{
    // ==================before translated======================
    // setLayerType(layerType, paint);
    assert(0);
    return NOERROR;
}

//=====================================================================
//     WebViewChromium::InnerComputeHorizontalScrollRangeCallable
//=====================================================================
WebViewChromium::InnerComputeHorizontalScrollRangeCallable::InnerComputeHorizontalScrollRangeCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerComputeHorizontalScrollRangeCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return computeHorizontalScrollRange();
    assert(0);
    // AutoPtr<IInteger32> empty;
    return NOERROR;
}

//=====================================================================
//      WebViewChromium::InnerComputeHorizontalScrollOffsetCallable
//=====================================================================
WebViewChromium::InnerComputeHorizontalScrollOffsetCallable::InnerComputeHorizontalScrollOffsetCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerComputeHorizontalScrollOffsetCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return computeHorizontalScrollOffset();
    assert(0);
    // AutoPtr<IInteger32> empty;
    return NOERROR;
}

//=====================================================================
//        WebViewChromium::InnerComputeVerticalScrollRangeCallable
//=====================================================================
WebViewChromium::InnerComputeVerticalScrollRangeCallable::InnerComputeVerticalScrollRangeCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerComputeVerticalScrollRangeCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return computeVerticalScrollRange();
    assert(0);
    // AutoPtr<IInteger32> empty;
    return NOERROR;
}

//=====================================================================
//        WebViewChromium::InnerComputeVerticalScrollOffsetCallable
//=====================================================================
WebViewChromium::InnerComputeVerticalScrollOffsetCallable::InnerComputeVerticalScrollOffsetCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerComputeVerticalScrollOffsetCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return computeVerticalScrollOffset();
    assert(0);
    // AutoPtr<IInteger32> empty;
    return NOERROR;
}

//=====================================================================
//     WebViewChromium::InnerComputeVerticalScrollExtentCallable
//=====================================================================
WebViewChromium::InnerComputeVerticalScrollExtentCallable::InnerComputeVerticalScrollExtentCallable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerComputeVerticalScrollExtentCallable::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return computeVerticalScrollExtent();
    assert(0);
    // AutoPtr<IInteger32> empty;
    return NOERROR;
}

//=====================================================================
//             WebViewChromium::InnerComputeScrollRunnable
//=====================================================================
WebViewChromium::InnerComputeScrollRunnable::InnerComputeScrollRunnable(
    /* [in] */ WebViewChromium* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WebViewChromium::InnerComputeScrollRunnable::Run()
{
    // ==================before translated======================
    // computeScroll();
    assert(0);
    return NOERROR;
}

//=====================================================================
//               WebViewChromium::WebViewNativeGLDelegate
//=====================================================================
Boolean WebViewChromium::WebViewNativeGLDelegate::RequestDrawGL(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Boolean waitForCompletion,
    /* [in] */ IView* containerView)
{
    // ==================before translated======================
    // if (mGLfunctor == null) {
    //     mGLfunctor = new DrawGLFunctor(mAwContents.getAwDrawGLViewContext());
    // }
    // return mGLfunctor.requestDrawGL(
    //         (HardwareCanvas) canvas, containerView.getViewRootImpl(), waitForCompletion);
    assert(0);
    return FALSE;
}

CARAPI_(void) WebViewChromium::WebViewNativeGLDelegate::DetachGLFunctor()
{
    // ==================before translated======================
    // if (mGLfunctor != null) {
    //     mGLfunctor.detach();
    // }
    assert(0);
}

//=====================================================================
//                WebViewChromium::InternalAccessAdapter
//=====================================================================
Boolean WebViewChromium::InternalAccessAdapter::DrawChild(
    /* [in] */ ICanvas* arg0,
    /* [in] */ IView* arg1,
    /* [in] */ Int64 arg2)
{
    // ==================before translated======================
    // UnimplementedWebViewApi.invoke();
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::InternalAccessAdapter::Super_onKeyUp(
    /* [in] */ Int32 arg0,
    /* [in] */ IKeyEvent* arg1)
{
    // ==================before translated======================
    // // Intentional no-op
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::InternalAccessAdapter::Super_dispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* arg0)
{
    // ==================before translated======================
    // UnimplementedWebViewApi.invoke();
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::InternalAccessAdapter::Super_dispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // ==================before translated======================
    // return mWebViewPrivate.super_dispatchKeyEvent(event);
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::InternalAccessAdapter::Super_onGenericMotionEvent(
    /* [in] */ IMotionEvent* arg0)
{
    // ==================before translated======================
    // return mWebViewPrivate.super_onGenericMotionEvent(arg0);
    assert(0);
    return FALSE;
}

void WebViewChromium::InternalAccessAdapter::Super_onConfigurationChanged(
    /* [in] */ IConfiguration* arg0)
{
    // ==================before translated======================
    // // Intentional no-op
    assert(0);
}

Int32 WebViewChromium::InternalAccessAdapter::Super_getScrollBarStyle()
{
    // ==================before translated======================
    // return mWebViewPrivate.super_getScrollBarStyle();
    assert(0);
    return 0;
}

Boolean WebViewChromium::InternalAccessAdapter::AwakenScrollBars()
{
    // ==================before translated======================
    // mWebViewPrivate.awakenScrollBars(0);
    // // TODO: modify the WebView.PrivateAccess to provide a return value.
    // return true;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::InternalAccessAdapter::Super_awakenScrollBars(
    /* [in] */ Int32 arg0,
    /* [in] */ Boolean arg1)
{
    // ==================before translated======================
    // // TODO: need method on WebView.PrivateAccess?
    // UnimplementedWebViewApi.invoke();
    // return false;
    assert(0);
    return FALSE;
}

void WebViewChromium::InternalAccessAdapter::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    // ==================before translated======================
    // // Intentional no-op.
    // // Chromium calls this directly to trigger accessibility events. That isn't needed
    // // for WebView since super_scrollTo invokes onScrollChanged for us.
    assert(0);
}

void WebViewChromium::InternalAccessAdapter::OverScrollBy(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 scrollRangeX,
    /* [in] */ Int32 scrollRangeY,
    /* [in] */ Int32 maxOverScrollX,
    /* [in] */ Int32 maxOverScrollY,
    /* [in] */ Boolean isTouchEvent)
{
    // ==================before translated======================
    // mWebViewPrivate.overScrollBy(deltaX, deltaY, scrollX, scrollY,
    //         scrollRangeX, scrollRangeY, maxOverScrollX, maxOverScrollY, isTouchEvent);
    assert(0);
}

void WebViewChromium::InternalAccessAdapter::Super_scrollTo(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY)
{
    // ==================before translated======================
    // mWebViewPrivate.super_scrollTo(scrollX, scrollY);
    assert(0);
}

void WebViewChromium::InternalAccessAdapter::SetMeasuredDimension(
    /* [in] */ Int32 measuredWidth,
    /* [in] */ Int32 measuredHeight)
{
    // ==================before translated======================
    // mWebViewPrivate.setMeasuredDimension(measuredWidth, measuredHeight);
    assert(0);
}

Boolean WebViewChromium::InternalAccessAdapter::Super_onHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    // ==================before translated======================
    // return mWebViewPrivate.super_onHoverEvent(event);
    assert(0);
    return FALSE;
}

//=====================================================================
//                           WebViewChromium
//=====================================================================
//const String WebViewChromium::TAG("WebViewChromium");//WebViewChromium.class.getSimpleName());
//Boolean WebViewChromium::sRecordWholeDocumentEnabledByApi = FALSE;

WebViewChromium::WebViewChromium(
    /* [in] */ IInterface/*WebViewChromiumFactoryProvider*/* factory,
    /* [in] */ IInterface/*WebView*/* webView,
    /* [in] */ IInterface/*WebView::PrivateAccess*/* webViewPrivate)
{
    // ==================before translated======================
    // mWebView = webView;
    // mWebViewPrivate = webViewPrivate;
    // mHitTestResult = new WebView.HitTestResult();
    // mAppTargetSdkVersion = mWebView.getContext().getApplicationInfo().targetSdkVersion;
    // mFactory = factory;
    // mRunQueue = new WebViewChromiumRunQueue();
    // String webViewAssetPath = WebViewFactory.getLoadedPackageInfo().applicationInfo.sourceDir;
    // mWebView.getContext().getAssets().addAssetPath(webViewAssetPath);
}

ECode WebViewChromium::EnableSlowWholeDocumentDraw()
{
    // ==================before translated======================
    // sRecordWholeDocumentEnabledByApi = true;
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::CompleteWindowCreation(
    /* [in] */ IInterface/*WebView*/* parent,
    /* [in] */ IInterface/*WebView*/* child)
{
    VALIDATE_NOT_NULL(parent);
    VALIDATE_NOT_NULL(child);
    // ==================before translated======================
    // AwContents parentContents = ((WebViewChromium) parent.getWebViewProvider()).mAwContents;
    // AwContents childContents =
    //         child == null ? null : ((WebViewChromium) child.getWebViewProvider()).mAwContents;
    // parentContents.supplyContentsForPopup(childContents);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::Init(
    /* [in] */ const IMap* javaScriptInterfaces,
    /* [in] */ const Boolean& privateBrowsing)
{
    VALIDATE_NOT_NULL(javaScriptInterfaces);
    // ==================before translated======================
    // if (privateBrowsing) {
    //     mFactory.startYourEngines(true);
    //     final String msg = "Private browsing is not supported in WebView.";
    //     if (mAppTargetSdkVersion >= Build.VERSION_CODES.KITKAT) {
    //         throw new IllegalArgumentException(msg);
    //     } else {
    //         Log.w(TAG, msg);
    //         TextView warningLabel = new TextView(mWebView.getContext());
    //         warningLabel.setText(mWebView.getContext().getString(
    //                 R.string.webviewchromium_private_browsing_warning));
    //         mWebView.addView(warningLabel);
    //     }
    // }
    //
    // // We will defer real initialization until we know which thread to do it on, unless:
    // // - we are on the main thread already (common case),
    // // - the app is targeting >= JB MR2, in which case checkThread enforces that all usage
    // //   comes from a single thread. (Note in JB MR2 this exception was in WebView.java).
    // if (mAppTargetSdkVersion >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
    //     mFactory.startYourEngines(false);
    //     checkThread();
    // } else if (!mFactory.hasStarted()) {
    //     if (Looper.myLooper() == Looper.getMainLooper()) {
    //         mFactory.startYourEngines(true);
    //     }
    // }
    //
    // final boolean isAccessFromFileURLsGrantedByDefault =
    //         mAppTargetSdkVersion < Build.VERSION_CODES.JELLY_BEAN;
    // final boolean areLegacyQuirksEnabled =
    //         mAppTargetSdkVersion < Build.VERSION_CODES.KITKAT;
    //
    // mContentsClientAdapter = new WebViewContentsClientAdapter(mWebView);
    // mWebSettings = new ContentSettingsAdapter(new AwSettings(
    //         mWebView.getContext(), isAccessFromFileURLsGrantedByDefault,
    //         areLegacyQuirksEnabled));
    //
    // if (mAppTargetSdkVersion <= Build.VERSION_CODES.KITKAT) {
    //     mWebSettings.setMixedContentMode(WebSettings.MIXED_CONTENT_ALWAYS_ALLOW);
    //     // On KK and older versions we always allowed third party cookies.
    //     mWebSettings.setAcceptThirdPartyCookies(true);
    //     mWebSettings.getAwSettings().setZeroLayoutHeightDisablesViewportQuirk(true);
    // }
    //
    // mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             initForReal();
    //             if (privateBrowsing) {
    //                 // Intentionally irreversibly disable the webview instance, so that private
    //                 // user data cannot leak through misuse of a non-privateBrowing WebView
    //                 // instance. Can't just null out mAwContents as we never null-check it
    //                 // before use.
    //                 destroy();
    //             }
    //         }
    // });
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::StartYourEngine()
{
    // ==================before translated======================
    // mRunQueue.drainQueue();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetHorizontalScrollbarOverlay(
    /* [in] */ const Boolean& overlay)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             setHorizontalScrollbarOverlay(overlay);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setHorizontalScrollbarOverlay(overlay);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetVerticalScrollbarOverlay(
    /* [in] */ const Boolean& overlay)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             setVerticalScrollbarOverlay(overlay);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setVerticalScrollbarOverlay(overlay);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::OverlayHorizontalScrollbar()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return overlayHorizontalScrollbar();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.overlayHorizontalScrollbar();
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::OverlayVerticalScrollbar()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return overlayVerticalScrollbar();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.overlayVerticalScrollbar();
    assert(0);
    return FALSE;
}

Int32 WebViewChromium::GetVisibleTitleHeight()
{
    // ==================before translated======================
    // // This is deprecated in WebView and should always return 0.
    // return 0;
    assert(0);
    return 0;
}

AutoPtr<ISslCertificate> WebViewChromium::GetCertificate()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     SslCertificate ret = runOnUiThreadBlocking(new Callable<SslCertificate>() {
    //         @Override
    //         public SslCertificate call() {
    //             return getCertificate();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.getCertificate();
    assert(0);
    AutoPtr<ISslCertificate> empty;
    return empty;
}

ECode WebViewChromium::SetCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    VALIDATE_NOT_NULL(certificate);
    // ==================before translated======================
    // // intentional no-op
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SavePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    // ==================before translated======================
    // // This is a deprecated API: intentional no-op.
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             setHttpAuthUsernamePassword(host, realm, username, password);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setHttpAuthUsernamePassword(host, realm, username, password);
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<String> > WebViewChromium::GetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm)
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     String[] ret = runOnUiThreadBlocking(new Callable<String[]>() {
    //         @Override
    //         public String[] call() {
    //             return getHttpAuthUsernamePassword(host, realm);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.getHttpAuthUsernamePassword(host, realm);
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

ECode WebViewChromium::Destroy()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             destroy();
    //         }
    //     });
    //     return;
    // }
    //
    // mAwContents.destroy();
    // if (mGLfunctor != null) {
    //     mGLfunctor.destroy();
    //     mGLfunctor = null;
    // }
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetNetworkAvailable(
    /* [in] */ const Boolean& networkUp)
{
    // ==================before translated======================
    // // Note that this purely toggles the JS navigator.online property.
    // // It does not in affect chromium or network stack state in any way.
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             setNetworkAvailable(networkUp);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setNetworkAvailable(networkUp);
    assert(0);
    return NOERROR;
}

AutoPtr<IInterface/*IWebBackForwardList*/> WebViewChromium::SaveState(
    /* [in] */ const IBundle* outState)
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     WebBackForwardList ret = runOnUiThreadBlocking(new Callable<WebBackForwardList>() {
    //         @Override
    //         public WebBackForwardList call() {
    //             return saveState(outState);
    //         }
    //     });
    //     return ret;
    // }
    // if (outState == null) return null;
    // if (!mAwContents.saveState(outState)) return null;
    // return copyBackForwardList();
    assert(0);
    AutoPtr<IInterface/*IWebBackForwardList*/> empty;
    return empty;
}

Boolean WebViewChromium::SavePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* dest)
{
    // ==================before translated======================
    // // Intentional no-op: hidden method on WebView.
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::RestorePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* src)
{
    // ==================before translated======================
    // // Intentional no-op: hidden method on WebView.
    // return false;
    assert(0);
    return FALSE;
}

AutoPtr<IInterface/*IWebBackForwardList*/> WebViewChromium::RestoreState(
    /* [in] */ const IBundle* inState)
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     WebBackForwardList ret = runOnUiThreadBlocking(new Callable<WebBackForwardList>() {
    //         @Override
    //         public WebBackForwardList call() {
    //             return restoreState(inState);
    //         }
    //     });
    //     return ret;
    // }
    // if (inState == null) return null;
    // if (!mAwContents.restoreState(inState)) return null;
    // return copyBackForwardList();
    assert(0);
    AutoPtr<IInterface/*IWebBackForwardList*/> empty;
    return empty;
}

ECode WebViewChromium::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IMap* additionalHttpHeaders)
{
    VALIDATE_NOT_NULL(additionalHttpHeaders);
    // ==================before translated======================
    // // TODO: We may actually want to do some sanity checks here (like filter about://chrome).
    //
    // // For backwards compatibility, apps targeting less than K will have JS URLs evaluated
    // // directly and any result of the evaluation will not replace the current page content.
    // // Matching Chrome behavior more closely; apps targetting >= K that load a JS URL will
    // // have the result of that URL replace the content of the current page.
    // final String JAVASCRIPT_SCHEME = "javascript:";
    // if (mAppTargetSdkVersion < Build.VERSION_CODES.KITKAT &&
    //         url != null && url.startsWith(JAVASCRIPT_SCHEME)) {
    //     mFactory.startYourEngines(true);
    //     if (checkNeedsPost()) {
    //         mRunQueue.addTask(new Runnable() {
    //             @Override
    //             public void run() {
    //                 mAwContents.evaluateJavaScriptEvenIfNotYetNavigated(
    //                         url.substring(JAVASCRIPT_SCHEME.length()));
    //             }
    //         });
    //     } else {
    //         mAwContents.evaluateJavaScriptEvenIfNotYetNavigated(
    //                 url.substring(JAVASCRIPT_SCHEME.length()));
    //     }
    //     return;
    // }
    //
    // LoadUrlParams params = new LoadUrlParams(url);
    // if (additionalHttpHeaders != null) params.setExtraHeaders(additionalHttpHeaders);
    // loadUrlOnUiThread(params);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::LoadUrl(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // // Early out to match old WebView implementation
    // if (url == null) {
    //     return;
    // }
    // loadUrl(url, null);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ ArrayOf<Byte>* postData)
{
    VALIDATE_NOT_NULL(postData);
    // ==================before translated======================
    // LoadUrlParams params = LoadUrlParams.createLoadHttpPostParams(url, postData);
    // Map<String,String> headers = new HashMap<String,String>();
    // headers.put("Content-Type", "application/x-www-form-urlencoded");
    // params.setExtraHeaders(headers);
    // loadUrlOnUiThread(params);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::LoadData(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding)
{
    // ==================before translated======================
    // loadUrlOnUiThread(LoadUrlParams.createLoadDataParams(
    //         fixupData(data), fixupMimeType(mimeType), isBase64Encoded(encoding)));
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::LoadDataWithBaseURL(
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& historyUrl)
{
    // ==================before translated======================
    // data = fixupData(data);
    // mimeType = fixupMimeType(mimeType);
    // LoadUrlParams loadUrlParams;
    // baseUrl = fixupBase(baseUrl);
    // historyUrl = fixupHistory(historyUrl);
    //
    // if (baseUrl.startsWith("data:")) {
    //     // For backwards compatibility with WebViewClassic, we use the value of |encoding|
    //     // as the charset, as long as it's not "base64".
    //     boolean isBase64 = isBase64Encoded(encoding);
    //     loadUrlParams = LoadUrlParams.createLoadDataParamsWithBaseUrl(
    //             data, mimeType, isBase64, baseUrl, historyUrl, isBase64 ? null : encoding);
    // } else {
    //     // When loading data with a non-data: base URL, the classic WebView would effectively
    //     // "dump" that string of data into the WebView without going through regular URL
    //     // loading steps such as decoding URL-encoded entities. We achieve this same behavior by
    //     // base64 encoding the data that is passed here and then loading that as a data: URL.
    //     try {
    //         loadUrlParams = LoadUrlParams.createLoadDataParamsWithBaseUrl(
    //                 Base64.encodeToString(data.getBytes("utf-8"), Base64.DEFAULT), mimeType,
    //                 true, baseUrl, historyUrl, "utf-8");
    //     } catch (java.io.UnsupportedEncodingException e) {
    //         Log.wtf(TAG, "Unable to load data string " + data, e);
    //         return;
    //     }
    // }
    // loadUrlOnUiThread(loadUrlParams);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::EvaluateJavaScript(
    /* [in] */ const String& script,
    /* [in] */ IInterface/*IValueCallback*/* resultCallback)
{
    VALIDATE_NOT_NULL(resultCallback);
    // ==================before translated======================
    // checkThread();
    // mAwContents.evaluateJavaScript(script, resultCallback);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SaveWebArchive(
    /* [in] */ const String& filename)
{
    // ==================before translated======================
    // saveWebArchive(filename, false, null);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SaveWebArchive(
    /* [in] */ const String& basename,
    /* [in] */ const Boolean& autoname,
    /* [in] */ const IInterface/*IValueCallback*/* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             saveWebArchive(basename, autoname, callback);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.saveWebArchive(basename, autoname, callback);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::StopLoading()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             stopLoading();
    //         }
    //     });
    //     return;
    // }
    //
    // mAwContents.stopLoading();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::Reload()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             reload();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.reload();
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::CanGoBack()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return canGoBack();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.canGoBack();
    assert(0);
    return FALSE;
}

ECode WebViewChromium::GoBack()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             goBack();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.goBack();
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::CanGoForward()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return canGoForward();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.canGoForward();
    assert(0);
    return FALSE;
}

ECode WebViewChromium::GoForward()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             goForward();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.goForward();
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::CanGoBackOrForward(
    /* [in] */ const Int32& steps)
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return canGoBackOrForward(steps);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.canGoBackOrForward(steps);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::GoBackOrForward(
    /* [in] */ const Int32& steps)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             goBackOrForward(steps);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.goBackOrForward(steps);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::IsPrivateBrowsingEnabled()
{
    // ==================before translated======================
    // // Not supported in this WebView implementation.
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::PageUp(
    /* [in] */ const Boolean& top)
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return pageUp(top);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.pageUp(top);
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::PageDown(
    /* [in] */ const Boolean& bottom)
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return pageDown(bottom);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.pageDown(bottom);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::ClearView()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             clearView();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.clearView();
    assert(0);
    return NOERROR;
}

AutoPtr<IPicture> WebViewChromium::CapturePicture()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Picture ret = runOnUiThreadBlocking(new Callable<Picture>() {
    //         @Override
    //         public Picture call() {
    //             return capturePicture();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.capturePicture();
    assert(0);
    AutoPtr<IPicture> empty;
    return empty;
}

Float WebViewChromium::GetScale()
{
    // ==================before translated======================
    // // No checkThread() as it is mostly thread safe (workaround for b/10652991).
    // mFactory.startYourEngines(true);
    // return mAwContents.getScale();
    assert(0);
    return 0.0f;
}

ECode WebViewChromium::SetInitialScale(
    /* [in] */ const Int32& scaleInPercent)
{
    // ==================before translated======================
    // // No checkThread() as it is thread safe
    // mWebSettings.getAwSettings().setInitialPageScale(scaleInPercent);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::InvokeZoomPicker()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             invokeZoomPicker();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.invokeZoomPicker();
    assert(0);
    return NOERROR;
}

AutoPtr<IInterface/*WebView::HitTestResult*/> WebViewChromium::GetHitTestResult()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     WebView.HitTestResult ret = runOnUiThreadBlocking(
    //             new Callable<WebView.HitTestResult>() {
    //         @Override
    //         public WebView.HitTestResult call() {
    //             return getHitTestResult();
    //         }
    //     });
    //     return ret;
    // }
    // AwContents.HitTestData data = mAwContents.getLastHitTestResult();
    // mHitTestResult.setType(data.hitTestResultType);
    // mHitTestResult.setExtra(data.hitTestResultExtraData);
    // return mHitTestResult;
    assert(0);
    AutoPtr<IInterface/*WebView::HitTestResult*/> empty;
    return empty;
}

ECode WebViewChromium::RequestFocusNodeHref(
    /* [in] */ const IMessage* hrefMsg)
{
    VALIDATE_NOT_NULL(hrefMsg);
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             requestFocusNodeHref(hrefMsg);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.requestFocusNodeHref(hrefMsg);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::RequestImageRef(
    /* [in] */ const IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             requestImageRef(msg);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.requestImageRef(msg);
    assert(0);
    return NOERROR;
}

String WebViewChromium::GetUrl()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     String ret = runOnUiThreadBlocking(new Callable<String>() {
    //         @Override
    //         public String call() {
    //             return getUrl();
    //         }
    //     });
    //     return ret;
    // }
    // String url =  mAwContents.getUrl();
    // if (url == null || url.trim().isEmpty()) return null;
    // return url;
    assert(0);
    return String("");
}

String WebViewChromium::GetOriginalUrl()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     String ret = runOnUiThreadBlocking(new Callable<String>() {
    //         @Override
    //         public String call() {
    //             return getOriginalUrl();
    //         }
    //     });
    //     return ret;
    // }
    // String url =  mAwContents.getOriginalUrl();
    // if (url == null || url.trim().isEmpty()) return null;
    // return url;
    assert(0);
    return String("");
}

String WebViewChromium::GetTitle()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     String ret = runOnUiThreadBlocking(new Callable<String>() {
    //         @Override
    //         public String call() {
    //             return getTitle();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.getTitle();
    assert(0);
    return String("");
}

AutoPtr<IBitmap> WebViewChromium::GetFavicon()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Bitmap ret = runOnUiThreadBlocking(new Callable<Bitmap>() {
    //         @Override
    //         public Bitmap call() {
    //             return getFavicon();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.getFavicon();
    assert(0);
    AutoPtr<IBitmap> empty;
    return empty;
}

String WebViewChromium::GetTouchIconUrl()
{
    // ==================before translated======================
    // // Intentional no-op: hidden method on WebView.
    // return null;
    assert(0);
    return String("");
}

Int32 WebViewChromium::GetProgress()
{
    // ==================before translated======================
    // if (mAwContents == null) return 100;
    // // No checkThread() because the value is cached java side (workaround for b/10533304).
    // return mAwContents.getMostRecentProgress();
    assert(0);
    return 0;
}

Int32 WebViewChromium::GetContentHeight()
{
    // ==================before translated======================
    // if (mAwContents == null) return 0;
    // // No checkThread() as it is mostly thread safe (workaround for b/10594869).
    // return mAwContents.getContentHeightCss();
    assert(0);
    return 0;
}

Int32 WebViewChromium::GetContentWidth()
{
    // ==================before translated======================
    // if (mAwContents == null) return 0;
    // // No checkThread() as it is mostly thread safe (workaround for b/10594869).
    // return mAwContents.getContentWidthCss();
    assert(0);
    return 0;
}

ECode WebViewChromium::PauseTimers()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             pauseTimers();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.pauseTimers();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::ResumeTimers()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             resumeTimers();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.resumeTimers();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnPause()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onPause();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onPause();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnResume()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onResume();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onResume();
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::IsPaused()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     Boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return isPaused();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.isPaused();
    assert(0);
    return FALSE;
}

ECode WebViewChromium::FreeMemory()
{
    // ==================before translated======================
    // // Intentional no-op. Memory is managed automatically by Chromium.
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::ClearCache(
    /* [in] */ const Boolean& includeDiskFiles)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             clearCache(includeDiskFiles);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.clearCache(includeDiskFiles);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::ClearFormData()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             clearFormData();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.hideAutofillPopup();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::ClearHistory()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             clearHistory();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.clearHistory();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::ClearSslPreferences()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             clearSslPreferences();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.clearSslPreferences();
    assert(0);
    return NOERROR;
}

AutoPtr<IInterface/*IWebBackForwardList*/> WebViewChromium::CopyBackForwardList()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     WebBackForwardList ret = runOnUiThreadBlocking(new Callable<WebBackForwardList>() {
    //         @Override
    //         public WebBackForwardList call() {
    //             return copyBackForwardList();
    //         }
    //     });
    //     return ret;
    // }
    // return new WebBackForwardListChromium(
    //         mAwContents.getNavigationHistory());
    assert(0);
    AutoPtr<IInterface/*IWebBackForwardList*/> empty;
    return empty;
}

ECode WebViewChromium::SetFindListener(
    /* [in] */ IInterface/*WebView::FindListener*/* listener)
{
    // ==================before translated======================
    // mContentsClientAdapter.setFindListener(listener);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::FindNext(
    /* [in] */ const Boolean& forwards)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             findNext(forwards);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.findNext(forwards);
    assert(0);
    return NOERROR;
}

Int32 WebViewChromium::FindAll(
    /* [in] */ const String& searchString)
{
    // ==================before translated======================
    // findAllAsync(searchString);
    // return 0;
    assert(0);
    return 0;
}

ECode WebViewChromium::FindAllAsync(
    /* [in] */ const String& searchString)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             findAllAsync(searchString);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.findAllAsync(searchString);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::ShowFindDialog(
    /* [in] */ const String& text,
    /* [in] */ const Boolean& showIme)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     return false;
    // }
    // if (mWebView.getParent() == null) {
    //     return false;
    // }
    //
    // FindActionModeCallback findAction = new FindActionModeCallback(mWebView.getContext());
    // if (findAction == null) {
    //     return false;
    // }
    //
    // mWebView.startActionMode(findAction);
    // findAction.setWebView(mWebView);
    // if (showIme) {
    //     findAction.showSoftInput();
    // }
    //
    // if (text != null) {
    //     findAction.setText(text);
    //     findAction.findAll();
    // }
    //
    // return true;
    assert(0);
    return FALSE;
}

ECode WebViewChromium::NotifyFindDialogDismissed()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             notifyFindDialogDismissed();
    //         }
    //     });
    //     return;
    // }
    // clearMatches();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::ClearMatches()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             clearMatches();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.clearMatches();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::DocumentHasImages(
    /* [in] */ const IMessage* response)
{
    VALIDATE_NOT_NULL(response);
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             documentHasImages(response);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.documentHasImages(response);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetWebViewClient(
    /* [in] */ IInterface/*IWebViewClient*/* client)
{
    VALIDATE_NOT_NULL(client);
    // ==================before translated======================
    // mContentsClientAdapter.setWebViewClient(client);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetDownloadListener(
    /* [in] */ IInterface/*IDownloadListener*/* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // mContentsClientAdapter.setDownloadListener(listener);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetWebChromeClient(
    /* [in] */ IInterface/*WebChromeClient*/* client)
{
    VALIDATE_NOT_NULL(client);
    // ==================before translated======================
    // mWebSettings.getAwSettings().setFullscreenSupported(doesSupportFullscreen(client));
    // mContentsClientAdapter.setWebChromeClient(client);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetPictureListener(
    /* [in] */ const IInterface/*WebView::PictureListener*/* listener)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             setPictureListener(listener);
    //         }
    //     });
    //     return;
    // }
    // mContentsClientAdapter.setPictureListener(listener);
    // mAwContents.enableOnNewPicture(listener != null,
    //         mAppTargetSdkVersion >= Build.VERSION_CODES.JELLY_BEAN_MR2);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::AddJavascriptInterface(
    /* [in] */ const Object* obj,
    /* [in] */ const String& interfaceName)
{
    VALIDATE_NOT_NULL(obj);
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             addJavascriptInterface(obj, interfaceName);
    //         }
    //     });
    //     return;
    // }
    // Class<? extends Annotation> requiredAnnotation = null;
    // if (mAppTargetSdkVersion >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
    //    requiredAnnotation = JavascriptInterface.class;
    // }
    // mAwContents.addPossiblyUnsafeJavascriptInterface(obj, interfaceName, requiredAnnotation);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::RemoveJavascriptInterface(
    /* [in] */ const String& interfaceName)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             removeJavascriptInterface(interfaceName);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.removeJavascriptInterface(interfaceName);
    assert(0);
    return NOERROR;
}

AutoPtr<IInterface/*WebSettings*/> WebViewChromium::GetSettings()
{
    // ==================before translated======================
    // return mWebSettings;
    assert(0);
    AutoPtr<IInterface/*WebSettings*/> empty;
    return empty;
}

ECode WebViewChromium::SetMapTrackballToArrowKeys(
    /* [in] */ Boolean setMap)
{
    // ==================before translated======================
    // // This is a deprecated API: intentional no-op.
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::FlingScroll(
    /* [in] */ const Int32& vx,
    /* [in] */ const Int32& vy)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             flingScroll(vx, vy);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.flingScroll(vx, vy);
    assert(0);
    return NOERROR;
}

AutoPtr<IView> WebViewChromium::GetZoomControls()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     return null;
    // }
    //
    // // This was deprecated in 2009 and hidden in JB MR1, so just provide the minimum needed
    // // to stop very out-dated applications from crashing.
    // Log.w(TAG, "WebView doesn't support getZoomControls");
    // return mAwContents.getSettings().supportZoom() ? new View(mWebView.getContext()) : null;
    assert(0);
    AutoPtr<IView> empty;
    return empty;
}

Boolean WebViewChromium::CanZoomIn()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     return false;
    // }
    // return mAwContents.canZoomIn();
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::CanZoomOut()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     return false;
    // }
    // return mAwContents.canZoomOut();
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::ZoomIn()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return zoomIn();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.zoomIn();
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::ZoomOut()
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return zoomOut();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.zoomOut();
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::ZoomBy(
    /* [in] */ Float factor)
{
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // // This is an L API and therefore we can enforce stricter threading constraints.
    // checkThread();
    // return mAwContents.zoomBy(factor);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::DumpViewHierarchyWithProperties(
    /* [in] */ IBufferedWriter* out,
    /* [in] */ Int32 level)
{
    VALIDATE_NOT_NULL(out);
    // ==================before translated======================
    // // Intentional no-op
    assert(0);
    return NOERROR;
}

AutoPtr<IView> WebViewChromium::FindHierarchyView(
    /* [in] */ const String& className,
    /* [in] */ Int32 hashCode)
{
    // ==================before translated======================
    // // Intentional no-op
    // return null;
    assert(0);
    AutoPtr<IView> empty;
    return empty;
}

AutoPtr<IInterface/*IWebViewProviderViewDelegate*/> WebViewChromium::GetViewDelegate()
{
    // ==================before translated======================
    // return this;
    assert(0);
    AutoPtr<IInterface/*IWebViewProviderViewDelegate*/> empty;
    return empty;
}

AutoPtr<IInterface/*IWebViewProviderScrollDelegate*/> WebViewChromium::GetScrollDelegate()
{
    // ==================before translated======================
    // return this;
    assert(0);
    AutoPtr<IInterface/*IWebViewProviderScrollDelegate*/> empty;
    return empty;
}

Boolean WebViewChromium::ShouldDelayChildPressedState()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return shouldDelayChildPressedState();
    //         }
    //     });
    //     return ret;
    // }
    // return true;
    assert(0);
    return FALSE;
}

AutoPtr<IAccessibilityNodeProvider> WebViewChromium::GetAccessibilityNodeProvider()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     AccessibilityNodeProvider ret = runOnUiThreadBlocking(
    //             new Callable<AccessibilityNodeProvider>() {
    //         @Override
    //         public AccessibilityNodeProvider call() {
    //             return getAccessibilityNodeProvider();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.getAccessibilityNodeProvider();
    assert(0);
    AutoPtr<IAccessibilityNodeProvider> empty;
    return empty;
}

ECode WebViewChromium::OnInitializeAccessibilityNodeInfo(
    /* [in] */ const IAccessibilityNodeInfo* info)
{
    VALIDATE_NOT_NULL(info);
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     runVoidTaskOnUiThreadBlocking(new Runnable() {
    //         @Override
    //         public void run() {
    //             onInitializeAccessibilityNodeInfo(info);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onInitializeAccessibilityNodeInfo(info);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnInitializeAccessibilityEvent(
    /* [in] */ const IAccessibilityEvent* event)
{
    VALIDATE_NOT_NULL(event);
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     runVoidTaskOnUiThreadBlocking(new Runnable() {
    //         @Override
    //         public void run() {
    //             onInitializeAccessibilityEvent(event);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onInitializeAccessibilityEvent(event);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::PerformAccessibilityAction(
    /* [in] */ const Int32& action,
    /* [in] */ const IBundle* arguments)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return performAccessibilityAction(action, arguments);
    //         }
    //     });
    //     return ret;
    // }
    // if (mAwContents.supportsAccessibilityAction(action)) {
    //     return mAwContents.performAccessibilityAction(action, arguments);
    // }
    // return mWebViewPrivate.super_performAccessibilityAction(action, arguments);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::SetOverScrollMode(
    /* [in] */ const Int32& mode)
{
    // ==================before translated======================
    // // This gets called from the android.view.View c'tor that WebView inherits from. This
    // // causes the method to be called when mAwContents == null.
    // // It's safe to ignore these calls however since AwContents will read the current value of
    // // this setting when it's created.
    // if (mAwContents == null) return;
    //
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             setOverScrollMode(mode);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setOverScrollMode(mode);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetScrollBarStyle(
    /* [in] */ const Int32& style)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             setScrollBarStyle(style);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setScrollBarStyle(style);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnDrawVerticalScrollBar(
    /* [in] */ const ICanvas* canvas,
    /* [in] */ const IDrawable* scrollBar,
    /* [in] */ const Int32& l,
    /* [in] */ const Int32& t,
    /* [in] */ const Int32& r,
    /* [in] */ const Int32& b)
{
    VALIDATE_NOT_NULL(canvas);
    VALIDATE_NOT_NULL(scrollBar);
    // ==================before translated======================
    // // WebViewClassic was overriding this method to handle rubberband over-scroll. Since
    // // WebViewChromium doesn't support that the vanilla implementation of this method can be
    // // used.
    // mWebViewPrivate.super_onDrawVerticalScrollBar(canvas, scrollBar, l, t, r, b);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnOverScrolled(
    /* [in] */ const Int32& scrollX,
    /* [in] */ const Int32& scrollY,
    /* [in] */ const Boolean& clampedX,
    /* [in] */ const Boolean& clampedY)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onOverScrolled(scrollX, scrollY, clampedX, clampedY);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onContainerViewOverScrolled(scrollX, scrollY, clampedX, clampedY);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnWindowVisibilityChanged(
    /* [in] */ const Int32& visibility)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onWindowVisibilityChanged(visibility);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onWindowVisibilityChanged(visibility);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnDraw(
    /* [in] */ const ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     runVoidTaskOnUiThreadBlocking(new Runnable() {
    //         @Override
    //         public void run() {
    //             onDraw(canvas);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onDraw(canvas);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetLayoutParams(
    /* [in] */ const IViewGroupLayoutParams* layoutParams)
{
    // ==================before translated======================
    // // This API is our strongest signal from the View system that this
    // // WebView is going to be bound to a View hierarchy and so at this
    // // point we must bind Chromium's UI thread to the current thread.
    // mFactory.startYourEngines(false);
    // checkThread();
    // mWebViewPrivate.super_setLayoutParams(layoutParams);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::PerformLongClick()
{
    // ==================before translated======================
    // // Return false unless the WebView is attached to a View with a parent
    // return mWebView.getParent() != null ? mWebViewPrivate.super_performLongClick() : false;
    assert(0);
    return FALSE;
}

ECode WebViewChromium::OnConfigurationChanged(
    /* [in] */ const IConfiguration* newConfig)
{
    VALIDATE_NOT_NULL(newConfig);
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onConfigurationChanged(newConfig);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onConfigurationChanged(newConfig);
    assert(0);
    return NOERROR;
}

AutoPtr<IInputConnection> WebViewChromium::OnCreateInputConnection(
    /* [in] */ const IEditorInfo* outAttrs)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //    return null;
    // }
    // return mAwContents.onCreateInputConnection(outAttrs);
    assert(0);
    AutoPtr<IInputConnection> empty;
    return empty;
}

Boolean WebViewChromium::OnKeyMultiple(
    /* [in] */ const Int32& keyCode,
    /* [in] */ const Int32& repeatCount,
    /* [in] */ const IKeyEvent* event)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return onKeyMultiple(keyCode, repeatCount, event);
    //         }
    //     });
    //     return ret;
    // }
    // UnimplementedWebViewApi.invoke();
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::OnKeyDown(
    /* [in] */ const Int32& keyCode,
    /* [in] */ const IKeyEvent* event)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return onKeyDown(keyCode, event);
    //         }
    //     });
    //     return ret;
    // }
    // UnimplementedWebViewApi.invoke();
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::OnKeyUp(
    /* [in] */ const Int32& keyCode,
    /* [in] */ const IKeyEvent* event)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return onKeyUp(keyCode, event);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.onKeyUp(keyCode, event);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::OnAttachedToWindow()
{
    // ==================before translated======================
    // // This API is our strongest signal from the View system that this
    // // WebView is going to be bound to a View hierarchy and so at this
    // // point we must bind Chromium's UI thread to the current thread.
    // mFactory.startYourEngines(false);
    // checkThread();
    // mAwContents.onAttachedToWindow();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnDetachedFromWindow()
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onDetachedFromWindow();
    //         }
    //     });
    //     return;
    // }
    //
    // mAwContents.onDetachedFromWindow();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnVisibilityChanged(
    /* [in] */ const IView* changedView,
    /* [in] */ const Int32& visibility)
{
    VALIDATE_NOT_NULL(changedView);
    // ==================before translated======================
    // // The AwContents will find out the container view visibility before the first draw so we
    // // can safely ignore onVisibilityChanged callbacks that happen before init().
    // if (mAwContents == null) return;
    //
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onVisibilityChanged(changedView, visibility);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onVisibilityChanged(changedView, visibility);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnWindowFocusChanged(
    /* [in] */ const Boolean& hasWindowFocus)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onWindowFocusChanged(hasWindowFocus);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onWindowFocusChanged(hasWindowFocus);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnFocusChanged(
    /* [in] */ const Boolean& focused,
    /* [in] */ const Int32& direction,
    /* [in] */ const IRect* previouslyFocusedRect)
{
    VALIDATE_NOT_NULL(previouslyFocusedRect);
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onFocusChanged(focused, direction, previouslyFocusedRect);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onFocusChanged(focused, direction, previouslyFocusedRect);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::SetFrame(
    /* [in] */ const Int32& left,
    /* [in] */ const Int32& top,
    /* [in] */ const Int32& right,
    /* [in] */ const Int32& bottom)
{
    // ==================before translated======================
    // return mWebViewPrivate.super_setFrame(left, top, right, bottom);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::OnSizeChanged(
    /* [in] */ const Int32& w,
    /* [in] */ const Int32& h,
    /* [in] */ const Int32& ow,
    /* [in] */ const Int32& oh)
{
    // ==================before translated======================
    // if (checkNeedsPost()) {
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             onSizeChanged(w, h, ow, oh);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onSizeChanged(w, h, ow, oh);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::DispatchKeyEvent(
    /* [in] */ const IKeyEvent* event)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return dispatchKeyEvent(event);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.dispatchKeyEvent(event);
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::OnTouchEvent(
    /* [in] */ const IMotionEvent* ev)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return onTouchEvent(ev);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.onTouchEvent(ev);
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::OnHoverEvent(
    /* [in] */ const IMotionEvent* event)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return onHoverEvent(event);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.onHoverEvent(event);
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::OnGenericMotionEvent(
    /* [in] */ const IMotionEvent* event)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return onGenericMotionEvent(event);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.onGenericMotionEvent(event);
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::OnTrackballEvent(
    /* [in] */ IMotionEvent* ev)
{
    // ==================before translated======================
    // // Trackball event not handled, which eventually gets converted to DPAD keyevents
    // return false;
    assert(0);
    return FALSE;
}

Boolean WebViewChromium::RequestFocus(
    /* [in] */ const Int32& direction,
    /* [in] */ const IRect* previouslyFocusedRect)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return requestFocus(direction, previouslyFocusedRect);
    //         }
    //     });
    //     return ret;
    // }
    // mAwContents.requestFocus();
    // return mWebViewPrivate.super_requestFocus(direction, previouslyFocusedRect);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::OnMeasure(
    /* [in] */ const Int32& widthMeasureSpec,
    /* [in] */ const Int32& heightMeasureSpec)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     runVoidTaskOnUiThreadBlocking(new Runnable() {
    //         @Override
    //         public void run() {
    //             onMeasure(widthMeasureSpec, heightMeasureSpec);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.onMeasure(widthMeasureSpec, heightMeasureSpec);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::RequestChildRectangleOnScreen(
    /* [in] */ const IView* child,
    /* [in] */ const IRect* rect,
    /* [in] */ const Boolean& immediate)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     boolean ret = runOnUiThreadBlocking(new Callable<Boolean>() {
    //         @Override
    //         public Boolean call() {
    //             return requestChildRectangleOnScreen(child, rect, immediate);
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.requestChildRectangleOnScreen(child, rect, immediate);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::SetBackgroundColor(
    /* [in] */ const Int32& color)
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     ThreadUtils.postOnUiThread(new Runnable() {
    //         @Override
    //         public void run() {
    //             setBackgroundColor(color);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setBackgroundColor(color);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetLayerType(
    /* [in] */ const Int32& layerType,
    /* [in] */ const IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    // ==================before translated======================
    // // This can be called from WebView constructor in which case mAwContents
    // // is still null. We set the layer type in initForReal in that case.
    // if (mAwContents == null) return;
    // if (checkNeedsPost()) {
    //     ThreadUtils.postOnUiThread(new Runnable() {
    //         @Override
    //         public void run() {
    //             setLayerType(layerType, paint);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.setLayerType(layerType, paint);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::PreDispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // // TODO(leandrogracia): remove this method from WebViewProvider if we think
    // // we won't need it again.
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnStartTemporaryDetach()
{
    // ==================before translated======================
    // mAwContents.onStartTemporaryDetach();
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::OnFinishTemporaryDetach()
{
    // ==================before translated======================
    // mAwContents.onFinishTemporaryDetach();
    assert(0);
    return NOERROR;
}

Int32 WebViewChromium::ComputeHorizontalScrollRange()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     int ret = runOnUiThreadBlocking(new Callable<Integer>() {
    //         @Override
    //         public Integer call() {
    //             return computeHorizontalScrollRange();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.computeHorizontalScrollRange();
    assert(0);
    return 0;
}

Int32 WebViewChromium::ComputeHorizontalScrollOffset()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     int ret = runOnUiThreadBlocking(new Callable<Integer>() {
    //         @Override
    //         public Integer call() {
    //             return computeHorizontalScrollOffset();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.computeHorizontalScrollOffset();
    assert(0);
    return 0;
}

Int32 WebViewChromium::ComputeVerticalScrollRange()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     int ret = runOnUiThreadBlocking(new Callable<Integer>() {
    //         @Override
    //         public Integer call() {
    //             return computeVerticalScrollRange();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.computeVerticalScrollRange();
    assert(0);
    return 0;
}

Int32 WebViewChromium::ComputeVerticalScrollOffset()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     int ret = runOnUiThreadBlocking(new Callable<Integer>() {
    //         @Override
    //         public Integer call() {
    //             return computeVerticalScrollOffset();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.computeVerticalScrollOffset();
    assert(0);
    return 0;
}

Int32 WebViewChromium::ComputeVerticalScrollExtent()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     int ret = runOnUiThreadBlocking(new Callable<Integer>() {
    //         @Override
    //         public Integer call() {
    //             return computeVerticalScrollExtent();
    //         }
    //     });
    //     return ret;
    // }
    // return mAwContents.computeVerticalScrollExtent();
    assert(0);
    return 0;
}

ECode WebViewChromium::ComputeScroll()
{
    // ==================before translated======================
    // mFactory.startYourEngines(false);
    // if (checkNeedsPost()) {
    //     runVoidTaskOnUiThreadBlocking(new Runnable() {
    //         @Override
    //         public void run() {
    //             computeScroll();
    //         }
    //     });
    //     return;
    // }
    // mAwContents.computeScroll();
    assert(0);
    return NOERROR;
}

AutoPtr<IInterface/*IPrintDocumentAdapter*/> WebViewChromium::CreatePrintDocumentAdapter()
{
    // ==================before translated======================
    // return createPrintDocumentAdapter("default");
    assert(0);
    AutoPtr<IInterface/*IPrintDocumentAdapter*/> empty;
    return empty;
}

AutoPtr<IInterface/*IPrintDocumentAdapter*/> WebViewChromium::CreatePrintDocumentAdapter(
    /* [in] */ const String& documentName)
{
    // ==================before translated======================
    // checkThread();
    // return new AwPrintDocumentAdapter(mAwContents.getPdfExporter(), documentName);
    assert(0);
    AutoPtr<IInterface/*IPrintDocumentAdapter*/> empty;
    return empty;
}

ECode WebViewChromium::ExtractSmartClipData(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    // ==================before translated======================
    // checkThread();
    // mAwContents.extractSmartClipData(x, y, width, height);
    assert(0);
    return NOERROR;
}

ECode WebViewChromium::SetSmartClipResultHandler(
    /* [in] */ const IHandler* resultHandler)
{
    VALIDATE_NOT_NULL(resultHandler);
    // ==================before translated======================
    // checkThread();
    // mAwContents.setSmartClipResultHandler(resultHandler);
    assert(0);
    return NOERROR;
}

AutoPtr<IInterface> WebViewChromium::RunBlockingFuture(
    /* [in] */ FutureTask* task)
{
    // ==================before translated======================
    // if (!mFactory.hasStarted()) throw new RuntimeException("Must be started before we block!");
    // if (ThreadUtils.runningOnUiThread()) {
    //     throw new IllegalStateException("This method should only be called off the UI thread");
    // }
    // mRunQueue.addTask(task);
    // try {
    //     return task.get(4, TimeUnit.SECONDS);
    // } catch (java.util.concurrent.TimeoutException e) {
    //     throw new RuntimeException("Probable deadlock detected due to WebView API being called "
    //             + "on incorrect thread while the UI thread is blocked.", e);
    // } catch (Exception e) {
    //     throw new RuntimeException(e);
    // }
    assert(0);
    AutoPtr<IInterface> empty;
    return empty;
}

ECode WebViewChromium::RunVoidTaskOnUiThreadBlocking(
    /* [in] */ IRunnable* r)
{
    VALIDATE_NOT_NULL(r);
    // ==================before translated======================
    // FutureTask<Void> task = new FutureTask<Void>(r, null);
    // runBlockingFuture(task);
    assert(0);
    return NOERROR;
}

AutoPtr<IInterface> WebViewChromium::RunOnUiThreadBlocking(
    /* [in] */ ICallable* c)
{
    // ==================before translated======================
    // return runBlockingFuture(new FutureTask<T>(c));
    assert(0);
    AutoPtr<IInterface> empty;
    return empty;
}

AutoPtr<IContext> WebViewChromium::ResourcesContextWrapper(
    /* [in] */ const IContext* ctx)
{
    // ==================before translated======================
    // return new ContextWrapper(ctx) {
    //     @Override
    //     public ClassLoader getClassLoader() {
    //         final ClassLoader appCl = getBaseContext().getClassLoader();
    //         final ClassLoader webViewCl = this.getClass().getClassLoader();
    //         return new ClassLoader() {
    //             @Override
    //             protected Class<?> findClass(String name) throws ClassNotFoundException {
    //                 // First look in the WebViewProvider class loader.
    //                 try {
    //                     return webViewCl.loadClass(name);
    //                 } catch (ClassNotFoundException e) {
    //                     // Look in the app class loader; allowing it to throw ClassNotFoundException.
    //                     return appCl.loadClass(name);
    //                 }
    //             }
    //         };
    //     }
    //
    //     @Override
    //     public Object getSystemService(String name) {
    //         if (name.equals(Context.LAYOUT_INFLATER_SERVICE)) {
    //             LayoutInflater i = (LayoutInflater) getBaseContext().getSystemService(name);
    //             return i.cloneInContext(this);
    //         } else {
    //             return getBaseContext().getSystemService(name);
    //         }
    //     }
    //
    // };
    assert(0);
    AutoPtr<IContext> empty;
    return empty;
}

ECode WebViewChromium::InitForReal()
{
    // ==================before translated======================
    // Context ctx = resourcesContextWrapper(mWebView.getContext());
    // mAwContents = new AwContents(mFactory.getBrowserContext(), mWebView, ctx,
    //         new InternalAccessAdapter(), new WebViewNativeGLDelegate(),
    //         mContentsClientAdapter, mWebSettings.getAwSettings());
    //
    // if (mAppTargetSdkVersion >= Build.VERSION_CODES.KITKAT) {
    //     // On KK and above, favicons are automatically downloaded as the method
    //     // old apps use to enable that behavior is deprecated.
    //     AwContents.setShouldDownloadFavicons();
    // }
    //
    // AwContentsStatics.setRecordFullDocument(sRecordWholeDocumentEnabledByApi ||
    //         mAppTargetSdkVersion < Build.VERSION_CODES.L);
    //
    // if (mAppTargetSdkVersion <= Build.VERSION_CODES.KITKAT) {
    //     // On KK and older versions, JavaScript objects injected via addJavascriptInterface
    //     // were not inspectable.
    //     mAwContents.disableJavascriptInterfacesInspection();
    // }
    //
    // // TODO: This assumes AwContents ignores second Paint param.
    // mAwContents.setLayerType(mWebView.getLayerType(), null);
    assert(0);
    return NOERROR;
}

//AutoPtr<RuntimeException> WebViewChromium::CreateThreadException()
//{
//    // ==================before translated======================
//    // return new IllegalStateException(
//    //         "Calling View methods on another thread than the UI thread.");
//    assert(0);
//    AutoPtr<RuntimeException> empty;
//    return empty;
//}

Boolean WebViewChromium::CheckNeedsPost()
{
    // ==================before translated======================
    // boolean needsPost = !mFactory.hasStarted() || !ThreadUtils.runningOnUiThread();
    // if (!needsPost && mAwContents == null) {
    //     throw new IllegalStateException(
    //             "AwContents must be created if we are not posting!");
    // }
    // return needsPost;
    assert(0);
    return FALSE;
}

ECode WebViewChromium::CheckThread()
{
    // ==================before translated======================
    // if (!ThreadUtils.runningOnUiThread()) {
    //     final RuntimeException threadViolation = createThreadException();
    //     ThreadUtils.postOnUiThread(new Runnable() {
    //         @Override
    //         public void run() {
    //             throw threadViolation;
    //         }
    //     });
    //     throw createThreadException();
    // }
    assert(0);
    return NOERROR;
}

String WebViewChromium::FixupMimeType(
    /* [in] */ const String& mimeType)
{
    // ==================before translated======================
    // return TextUtils.isEmpty(mimeType) ? "text/html" : mimeType;
    assert(0);
    return String("");
}

String WebViewChromium::FixupData(
    /* [in] */ const String& data)
{
    // ==================before translated======================
    // return TextUtils.isEmpty(data) ? "" : data;
    assert(0);
    return String("");
}

String WebViewChromium::FixupBase(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // return TextUtils.isEmpty(url) ? "about:blank" : url;
    assert(0);
    return String("");
}

String WebViewChromium::FixupHistory(
    /* [in] */ const String& url)
{
    // ==================before translated======================
    // return TextUtils.isEmpty(url) ? "about:blank" : url;
    assert(0);
    return String("");
}

Boolean WebViewChromium::IsBase64Encoded(
    /* [in] */ const String& encoding)
{
    // ==================before translated======================
    // return "base64".equals(encoding);
    assert(0);
    return FALSE;
}

ECode WebViewChromium::LoadUrlOnUiThread(
    /* [in] */ const IInterface/*LoadUrlParams*/* loadUrlParams)
{
    VALIDATE_NOT_NULL(loadUrlParams);
    // ==================before translated======================
    // // This is the last point that we can delay starting the Chromium backend up
    // // and if the app has not caused us to bind the Chromium UI thread to a background thread
    // // we now bind Chromium's notion of the UI thread to the app main thread.
    // mFactory.startYourEngines(true);
    // if (checkNeedsPost()) {
    //     // Disallowed in WebView API for apps targetting a new SDK
    //     assert mAppTargetSdkVersion < Build.VERSION_CODES.JELLY_BEAN_MR2;
    //     mRunQueue.addTask(new Runnable() {
    //         @Override
    //         public void run() {
    //             mAwContents.loadUrl(loadUrlParams);
    //         }
    //     });
    //     return;
    // }
    // mAwContents.loadUrl(loadUrlParams);
    assert(0);
    return NOERROR;
}

Boolean WebViewChromium::DoesSupportFullscreen(
    /* [in] */ IInterface/*LoadUrlParams*/* client)
{
    // ==================before translated======================
    // if (client == null) {
    //     return false;
    // }
    // // If client is not a subclass of WebChromeClient then the methods have not been
    // // implemented because WebChromeClient has empty implementations.
    // if (client.getClass().isAssignableFrom(WebChromeClient.class)) {
    //     return false;
    // }
    // try {
    //     client.getClass().getDeclaredMethod("onShowCustomView", View.class,
    //             CustomViewCallback.class);
    //     client.getClass().getDeclaredMethod("onHideCustomView");
    //     return true;
    // } catch (NoSuchMethodException e) {
    //     return false;
    // }
    assert(0);
    return FALSE;
}


} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


