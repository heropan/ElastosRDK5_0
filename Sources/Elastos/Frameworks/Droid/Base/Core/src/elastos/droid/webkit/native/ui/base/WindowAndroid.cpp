
#include "elastos/droid/webkit/native/ui/base/WindowAndroid.h"
//#include "elastos/droid/widget/CToastHelper.h"

using Elastos::Utility::CHashMap;
using Elastos::IO::ISerializable;
using Elastos::Core::ICharSequence;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::CString;
using Elastos::Droid::Widget::IToast;
using Elastos::Droid::Widget::IToastHelper;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
//using Elastos::Droid::Widget::CToastHelper;

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

    if (mOwner->mNativeWindowAndroid != 0) {
        mOwner->NativeOnVSync(mOwner->mNativeWindowAndroid, vsyncTimeMicros);
    }
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

    assert(0);
    AutoPtr<IContext> applicationContext;
    context->GetApplicationContext((IContext**)&applicationContext);
    assert (context == applicationContext);
    mApplicationContext = context;

    //mOutstandingIntents = SparseArray<IntentCallback>::Alloc(1);
    CHashMap::New((IHashMap**)&mIntentErrors);
    mVSyncMonitor = new VSyncMonitor(context, mVSyncListener);
}

Boolean WindowAndroid::ShowIntent(
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // return showCancelableIntent(intent, callback, errorId) >= 0;

    return (ShowCancelableIntent(intent, callback, errorId) >= 0);
}

Boolean WindowAndroid::ShowIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // return showCancelableIntent(intent, callback, errorId) >= 0;

    return (ShowCancelableIntent(intent, callback, errorId) >= 0);
}

Int32 WindowAndroid::ShowCancelableIntent(
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    // return START_INTENT_FAILURE;

    // Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    return START_INTENT_FAILURE;
}

Int32 WindowAndroid::ShowCancelableIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    // return START_INTENT_FAILURE;

    // Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    return START_INTENT_FAILURE;
}

ECode WindowAndroid::CancelIntent(
    /* [in] */ Int32 requestCode)
{
    // ==================before translated======================
    // Log.d(TAG, "Can't cancel intent as context is not an Activity: " + requestCode);

    // Log.d(TAG, "Can't cancel intent as context is not an Activity: " + requestCode);
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
    IInterface* interfaceTmp = callback->Probe(EIID_IInterface);
    Int32 requestCode = 0;
    mOutstandingIntents->IndexOfValue(interfaceTmp, &requestCode);
    if (requestCode < 0) return FALSE;
    mOutstandingIntents->Remove(requestCode);

    AutoPtr<IInteger32> requestCodeTmp;
    CInteger32::New(requestCode, (IInteger32**)&requestCodeTmp);
    mIntentErrors->Remove(requestCodeTmp);
    return TRUE;
}

ECode WindowAndroid::ShowError(
    /* [in] */ const String& error)
{
    // ==================before translated======================
    // if (error != null) {
    //     Toast.makeText(mApplicationContext, error, Toast.LENGTH_SHORT).show();
    // }

    if (!error.IsEmpty()) {
        AutoPtr<IToastHelper> helper;
        //CToastHelper::AcquierSingleton((IToastHelper**)&helper);

        AutoPtr<ICharSequence> charSequence;
        CString::New(error, (ICharSequence**)&charSequence);

        AutoPtr<IToast> toast;
        helper->MakeText(mApplicationContext, charSequence, IToast::LENGTH_SHORT, (IToast**)&toast);
        toast->Show();
    }
    return NOERROR;
}

ECode WindowAndroid::ShowError(
    /* [in] */ Int32 resId)
{
    // ==================before translated======================
    // showError(mApplicationContext.getString(resId));

    String str;
    mApplicationContext->GetString(resId, &str);
    ShowError(str);
    return NOERROR;
}

ECode WindowAndroid::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // mApplicationContext.sendBroadcast(intent);

    mApplicationContext->SendBroadcast(intent);
    return NOERROR;
}

AutoPtr<IWeakReference> WindowAndroid::GetActivity()
{
    // ==================before translated======================
    // return new WeakReference<Activity>(null);

    IWeakReferenceSource* source = IWeakReferenceSource::Probe(NULL);
    AutoPtr<IWeakReference> wr;
    source->GetWeakReference((IWeakReference**)&wr);
    return wr;
}

AutoPtr<IContext> WindowAndroid::GetApplicationContext()
{
    // ==================before translated======================
    // return mApplicationContext;

    return mApplicationContext;
}

ECode WindowAndroid::SaveInstanceState(
    /* [in] */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    // ==================before translated======================
    // bundle.putSerializable(WINDOW_CALLBACK_ERRORS, mIntentErrors);

    AutoPtr<ISerializable> serialize = ISerializable::Probe(mIntentErrors);
    bundle->PutSerializable(WINDOW_CALLBACK_ERRORS, (ISerializable*)serialize);
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

    if (bundle == NULL) return NOERROR;
    AutoPtr<ISerializable> errors;
    bundle->GetSerializable(WINDOW_CALLBACK_ERRORS, (ISerializable**)&errors);

    IInterface* interfaceTmp = (IInterface*)errors;
    AutoPtr<IHashMap> hashMap = IHashMap::Probe(interfaceTmp);
    if (hashMap != NULL) {
        mIntentErrors = hashMap;
    }
    return NOERROR;
}

Boolean WindowAndroid::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    // ==================before translated======================
    // return false;

    return FALSE;
}

Boolean WindowAndroid::CanResolveActivity(
    /* [in] */ IIntent* intent)
{
    // ==================before translated======================
    // return mApplicationContext.getPackageManager().resolveActivity(intent, 0) != null;

    assert(0);
    AutoPtr<IPackageManager> packageManager;
    mApplicationContext->GetPackageManager((IPackageManager**)&packageManager);

    AutoPtr<IResolveInfo> resolveInfo;
    //-- car hasno define this func: packageManager->ResolveActivity(intent, 0, (IResolveInfo**)&resolveInfo);
    return resolveInfo != NULL;
}

ECode WindowAndroid::Destroy()
{
    // ==================before translated======================
    // if (mNativeWindowAndroid != 0) {
    //     nativeDestroy(mNativeWindowAndroid);
    //     mNativeWindowAndroid = 0;
    // }

    if (mNativeWindowAndroid != 0) {
        NativeDestroy(mNativeWindowAndroid);
        mNativeWindowAndroid = 0;
    }
    return NOERROR;
}

Int64 WindowAndroid::GetNativePointer()
{
    // ==================before translated======================
    // if (mNativeWindowAndroid == 0) {
    //     mNativeWindowAndroid = nativeInit(mVSyncMonitor.getVSyncPeriodInMicroseconds());
    // }
    // return mNativeWindowAndroid;

    if (mNativeWindowAndroid == 0) {
        mNativeWindowAndroid = NativeInit(mVSyncMonitor->GetVSyncPeriodInMicroseconds());
    }
    return mNativeWindowAndroid;
}

ECode WindowAndroid::ShowCallbackNonExistentError(
    /* [in] */ const String& error)
{
    // ==================before translated======================
    // showError(error);

    ShowError(error);
    return NOERROR;
}

ECode WindowAndroid::RequestVSyncUpdate()
{
    // ==================before translated======================
    // mVSyncMonitor.requestUpdate();

    mVSyncMonitor->RequestUpdate();
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


