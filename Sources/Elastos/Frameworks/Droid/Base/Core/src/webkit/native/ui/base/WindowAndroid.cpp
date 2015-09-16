// wuweizuo automatic build .cpp file from .java file.

#include "WindowAndroid.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//               WindowAndroid::InnerVSyncMonitorListener
//=====================================================================
WindowAndroid::InnerVSyncMonitorListener::InnerVSyncMonitorListener(
    /* [in] */ WindowAndroid* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode WindowAndroid::InnerVSyncMonitorListener::OnVSync(
    /* [in] */ VSyncMonitor* monitor,
    /* [in] */ Int64 vsyncTimeMicros)
{
    VALIDATE_NOT_NULL(monitor);
    // ==================before translated======================
    // if (mNativeWindowAndroid != 0) {
    //     nativeOnVSync(mNativeWindowAndroid, vsyncTimeMicros);
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                            WindowAndroid
//=====================================================================
const String WindowAndroid::WINDOW_CALLBACK_ERRORS("window_callback_errors");
const Int32 WindowAndroid::START_INTENT_FAILURE;
const String WindowAndroid::TAG("WindowAndroid");

WindowAndroid::WindowAndroid(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // assert context == context.getApplicationContext();
    // mApplicationContext = context;
    // mOutstandingIntents = new SparseArray<IntentCallback>();
    // mIntentErrors = new HashMap<Integer, String>();
    // mVSyncMonitor = new VSyncMonitor(context, mVSyncListener);
}

Boolean WindowAndroid::ShowIntent(
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // return showCancelableIntent(intent, callback, errorId) >= 0;
    assert(0);
    return FALSE;
}

Boolean WindowAndroid::ShowIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // return showCancelableIntent(intent, callback, errorId) >= 0;
    assert(0);
    return FALSE;
}

Int32 WindowAndroid::ShowCancelableIntent(
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    // return START_INTENT_FAILURE;
    assert(0);
    return 0;
}

Int32 WindowAndroid::ShowCancelableIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    // return START_INTENT_FAILURE;
    assert(0);
    return 0;
}

ECode WindowAndroid::CancelIntent(
    /* [in] */ Int32 requestCode)
{
    // ==================before translated======================
    // Log.d(TAG, "Can't cancel intent as context is not an Activity: " + requestCode);
    assert(0);
    return NOERROR;
}

Boolean WindowAndroid::RemoveIntentCallback(
    /* [in] */ IntentCallback* callback)
{
    // ==================before translated======================
    // int requestCode = mOutstandingIntents.indexOfValue(callback);
    // if (requestCode < 0) return false;
    // mOutstandingIntents.remove(requestCode);
    // mIntentErrors.remove(requestCode);
    // return true;
    assert(0);
    return FALSE;
}

ECode WindowAndroid::ShowError(
    /* [in] */ String error)
{
    // ==================before translated======================
    // if (error != null) {
    //     Toast.makeText(mApplicationContext, error, Toast.LENGTH_SHORT).show();
    // }
    assert(0);
    return NOERROR;
}

ECode WindowAndroid::ShowError(
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // showError(mApplicationContext.getString(resId));
    assert(0);
    return NOERROR;
}

ECode WindowAndroid::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // mApplicationContext.sendBroadcast(intent);
    assert(0);
    return NOERROR;
}

AutoPtr< IWeakReference< AutoPtr<IActivity> > > WindowAndroid::GetActivity()
{
    // ==================before translated======================
    // return new WeakReference<Activity>(null);
    assert(0);
    AutoPtr< IWeakReference< AutoPtr<IActivity> > > empty;
    return empty;
}

AutoPtr<IContext> WindowAndroid::GetApplicationContext()
{
    // ==================before translated======================
    // return mApplicationContext;
    assert(0);
    AutoPtr<IContext> empty;
    return empty;
}

ECode WindowAndroid::SaveInstanceState(
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    // ==================before translated======================
    // bundle.putSerializable(WINDOW_CALLBACK_ERRORS, mIntentErrors);
    assert(0);
    return NOERROR;
}

ECode WindowAndroid::RestoreInstanceState(
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    // ==================before translated======================
    // if (bundle == null) return;
    //
    // Object errors = bundle.getSerializable(WINDOW_CALLBACK_ERRORS);
    // if (errors instanceof HashMap) {
    //     @SuppressWarnings("unchecked")
    //     HashMap<Integer, String> intentErrors = (HashMap<Integer, String>) errors;
    //     mIntentErrors = intentErrors;
    // }
    assert(0);
    return NOERROR;
}

Boolean WindowAndroid::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    // ==================before translated======================
    // return false;
    assert(0);
    return FALSE;
}

Boolean WindowAndroid::CanResolveActivity(
    /* [in] */ IIntent* intent)
{
    // ==================before translated======================
    // return mApplicationContext.getPackageManager().resolveActivity(intent, 0) != null;
    assert(0);
    return FALSE;
}

ECode WindowAndroid::Destroy()
{
    // ==================before translated======================
    // if (mNativeWindowAndroid != 0) {
    //     nativeDestroy(mNativeWindowAndroid);
    //     mNativeWindowAndroid = 0;
    // }
    assert(0);
    return NOERROR;
}

Int64 WindowAndroid::GetNativePointer()
{
    // ==================before translated======================
    // if (mNativeWindowAndroid == 0) {
    //     mNativeWindowAndroid = nativeInit(mVSyncMonitor.getVSyncPeriodInMicroseconds());
    // }
    // return mNativeWindowAndroid;
    assert(0);
    return 0;
}

ECode WindowAndroid::ShowCallbackNonExistentError(
    /* [in] */ String error)
{
    // ==================before translated======================
    // showError(error);
    assert(0);
    return NOERROR;
}

ECode WindowAndroid::RequestVSyncUpdate()
{
    // ==================before translated======================
    // mVSyncMonitor.requestUpdate();
    assert(0);
    return NOERROR;
}

Int64 WindowAndroid::NativeInit(
    /* [in] */ Int64 vsyncPeriod)
{
    assert(0);
    return 0;
}

ECode WindowAndroid::NativeOnVSync(
    /* [in] */ Int64 nativeWindowAndroid,
    /* [in] */ Int64 vsyncTimeMicros)
{
    assert(0);
    return NOERROR;
}

ECode WindowAndroid::NativeDestroy(
    /* [in] */ Int64 nativeWindowAndroid)
{
    assert(0);
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


