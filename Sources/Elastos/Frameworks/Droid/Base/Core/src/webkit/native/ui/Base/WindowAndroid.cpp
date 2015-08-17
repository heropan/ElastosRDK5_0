// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

const String WindowAndroid::TAG("WindowAndroid");
const String WindowAndroid::WINDOW_CALLBACK_ERRORS("window_callback_errors");
const Int32 WindowAndroid::START_INTENT_FAILURE;

//===============================================================
//           WindowAndroid::InnerVSyncMonitorListener
//===============================================================
WindowAndroid::InnerVSyncMonitorListener::InnerVSyncMonitorListener(
    /* in */ WindowAndroid* owner)
    : mOwner(owner)
{
}

ECode WindowAndroid::InnerVSyncMonitorListener::OnVSync(
    /* in */ VSyncMonitor* monitor,
    /* in */ Int64 vsyncTimeMicros)
{
    VALIDATE_NOT_NULL(monitor);
    if (mOwner->mNativeWindowAndroid != 0) {
        mOwner->NativeOnVSync(mOwner->mNativeWindowAndroid, vsyncTimeMicros);
    }
    return NOERROR;
}

//===============================================================
//                       WindowAndroid
//===============================================================
WindowAndroid::WindowAndroid(
    /* in */ IContext* context)
{
    AutoPtr<IContext> applicationContext;
    context->GetApplicationContext((IContext**)&applicationContext);
    assert (context == applicationContext);
    mApplicationContext = context;
    mOutstandingIntents = SparseArray<IntentCallback>::Alloc(1);
    mIntentErrors = new HashMap<IInteger32, String>();
    mVSyncMonitor = new VSyncMonitor(context, mVSyncListener);
}

Boolean WindowAndroid::ShowIntent(
    /* in */ IPendingIntent* intent,
    /* in */ IntentCallback* callback,
    /* in */ Int32 errorId)
{
    return ShowCancelableIntent(intent, callback, errorId) >= 0;
}

Boolean WindowAndroid::ShowIntent(
    /* in */ IIntent* intent,
    /* in */ IntentCallback* callback,
    /* in */ Int32 errorId)
{
    return ShowCancelableIntent(intent, callback, errorId) >= 0;
}

Int32 WindowAndroid::ShowCancelableIntent(
    /* in */ IPendingIntent* intent,
    /* in */ IntentCallback* callback,
    /* in */ Int32 errorId)
{
    //Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    return START_INTENT_FAILURE;
}

Int32 WindowAndroid::ShowCancelableIntent(
    /* in */ IIntent* intent,
    /* in */ IntentCallback* callback,
    /* in */ Int32 errorId)
{
    //Log.d(TAG, "Can't show intent as context is not an Activity: " + intent);
    return START_INTENT_FAILURE;
}

ECode WindowAndroid::CancelIntent(
    /* in */ Int32 requestCode)
{
    //Log.d(TAG, "Can't cancel intent as context is not an Activity: " + requestCode);
    return NOERROR;
}

Boolean WindowAndroid::RemoveIntentCallback(
    /* in */ IntentCallback* callback)
{
    Int32 requestCode = mOutstandingIntents->IndexOfValue(callback);
    if (requestCode < 0) return FALSE;
    mOutstandingIntents->Remove(requestCode);
    mIntentErrors->Remove(requestCode);
    return TRUE;
}

ECode WindowAndroid::ShowError(
    /* in */ String error)
{
    if (!error.IsEmpty()) {
        Toast::MakeText(mApplicationContext, error, Toast::LENGTH_SHORT)->Show();
    }
    return NOERROR;
}

ECode WindowAndroid::ShowError(
    /* in */ Int32 resId)
{
    String str;
    mApplicationContext->GetString(resId, &str);
    ShowError(str);
    return NOERROR;
}

ECode WindowAndroid::SendBroadcast(
    /* in */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent);
    mApplicationContext->SendBroadcast(intent);
    return NOERROR;
}

AutoPtr<IContext> WindowAndroid::GetApplicationContext()
{
    return mApplicationContext;
}

ECode WindowAndroid::SaveInstanceState(
    /* in */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    bundle->PutSerializable(WINDOW_CALLBACK_ERRORS, mIntentErrors);
    return NOERROR;
}

ECode WindowAndroid::RestoreInstanceState(
    /* in */ IBundle* bundle)
{
    VALIDATE_NOT_NULL(bundle);
    if (bundle == NULL) return NOERROR;
    AutoPtr<Object> errors;
    bundle->GetSerializable(WINDOW_CALLBACK_ERRORS, (Object**)&errors);
    AutoPtr<IHashMap> hashMap = errors->Probe(EIID_IHashMap);
    if (hashMap = != NULL) {
        mIntentErrors = hashMap;
    }
    return NOERROR;
}

Boolean WindowAndroid::OnActivityResult(
    /* in */ Int32 requestCode,
    /* in */ Int32 resultCode,
    /* in */ IIntent* data)
{
    return FALSE;
}

Boolean WindowAndroid::CanResolveActivity(
    /* in */ IIntent* intent)
{
    AutoPtr<IPackageManager> packageManager;
    mApplicationContext->GetPackageManager((IPackageManager**)&packageManager);

    AutoPtr<IResolveInfo> resolveInfo;
    packageManager->ResolveActivity(intent, 0, (IResolveInfo**)&resolveInfo);
    return resolveInfo != NULL;
}

ECode WindowAndroid::Destroy()
{
    if (mNativeWindowAndroid != 0) {
        NativeDestroy(mNativeWindowAndroid);
        mNativeWindowAndroid = 0;
    }
    return NOERROR;
}

Int64 WindowAndroid::GetNativePointer()
{
    if (mNativeWindowAndroid == 0) {
        mNativeWindowAndroid = NativeInit(mVSyncMonitor.getVSyncPeriodInMicroseconds());
    }
    return mNativeWindowAndroid;
}

ECode WindowAndroid::ShowCallbackNonExistentError(
    /* in */ String error)
{
    ShowError(error);
    return NOERROR;
}

ECode WindowAndroid::RequestVSyncUpdate()
{
       mVSyncMonitor->RequestUpdate();
    return NOERROR;
}

Int64 WindowAndroid::NativeInit(
    /* in */ Int64 vsyncPeriod)
{
}

ECode WindowAndroid::NativeOnVSync(
    /* in */ Int64 nativeWindowAndroid,
    /* in */ Int64 vsyncTimeMicros)
{
    return NOERROR;
}

ECode WindowAndroid::NativeDestroy(
    /* in */ Int64 nativeWindowAndroid)
{
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

