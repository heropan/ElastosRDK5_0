
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

const Int32 ActivityWindowAndroid::REQUEST_CODE_PREFIX;
const Int32 ActivityWindowAndroid::REQUEST_CODE_RANGE_SIZE;
const String ActivityWindowAndroid::TAG("ActivityWindowAndroid");

ActivityWindowAndroid::ActivityWindowAndroid(
    /* in */ IActivity* activity)
    : mNextRequestCode(0)
{
    activity->GetApplicationContext();
    mActivityRef = new WeakReference<Activity>(activity);
}

//@Override
Int32 ActivityWindowAndroid::ShowCancelableIntent(
    /* in */ IPendingIntent* intent,
    /* in */ IIntentCallback* callback,
    /* in */ Int32 errorId)
{
    AutoPtr<IActivity> activity;
    mActivityRef->Get((IActivity**)&activity);
    if (NULL == activity)
        return START_INTENT_FAILURE;

    Int32 requestCode = GenerateNextRequestCode();

    //try {
        AutoPtr<IIntentSender> sender;
        intent->GetIntentSender((IIntentSender**)&sender);
        AutoPtr<IIntent> newIntent;
        CIntent::New((IIntent**)&newIntent);
        activity->StartIntentSenderForResult(sender, requestCode, newIntent, 0, 0, 0);
    //} catch (SendIntentException e) {
    //    return START_INTENT_FAILURE;
    //}

    StoreCallbackData(requestCode, callback, errorId);
    return requestCode;
}

//@Override
Int32 ActivityWindowAndroid::ShowCancelableIntent(
    /* in */ IIntent* intent,
    /* in */ IIntentCallback* callback,
    /* in */ Int32 errorId)
{
    AutoPtr<IActivity> activity;
    mActivityRef->Get((IActivity**)&activity);

    if (NULL == activity)
        return START_INTENT_FAILURE;

    Int32 requestCode = GenerateNextRequestCode();

    //try {
        activity->StartActivityForResult(intent, requestCode);
    //} catch (ActivityNotFoundException e) {
    //    return START_INTENT_FAILURE;
    //}

    StoreCallbackData(requestCode, callback, errorId);
    return requestCode;
}

//@Override
ECode ActivityWindowAndroid::CancelIntent(
    /* in */ Int32 requestCode)
{
    AutoPtr<IActivity> activity;
    mActivityRef->Get((IActivity**)&activity);

    if (NULL == activity)
        return NOERROR;
    activity->FinishActivity(requestCode);
    return NOERROR;
}

//@Override
Boolean ActivityWindowAndroid::OnActivityResult(
    /* in */ Int32 requestCode,
    /* in */ Int32 resultCode,
    /* in */ IIntent* data)
{
    AutoPtr<IIntentCallback> callback;
    mOutstandingIntents->Get(requestCode, (IIntentCallback**)&callback);
    mOutstandingIntents->Delete(requestCode);
    String errorMessage;
    mIntentErrors->Remove(requestCode, &errorMessage);

    if (NULL != callback) {
        AutoPtr<IContextResolver> contextResolver;
        mApplicationContext->GetContentResolver((IContextResolver**)&contextResolver);
        callback->OnIntentCompleted(this, resultCode, contextResolver, data);
        return TRUE;
    }
    else {
        if (NULL != errorMessage) {
            ShowCallbackNonExistentError(errorMessage);
            return TRUE;
        }
    }
    return FALSE;
}

//@Override
AutoPtr< IWeakReference<Activity> > ActivityWindowAndroid::GetActivity()
{
    // Return a new WeakReference to prevent clients from releasing our internal WeakReference.
    AutoPtr<IActivity> activity;
    mActivityRef->Get((IActivity**)&activity);

    AutoPtr< IWeakReference<Activity> > result = new WeakReference<IActivity>(activity);
    return result;
}

Int32 ActivityWindowAndroid::GenerateNextRequestCode()
{
    Int32 requestCode = REQUEST_CODE_PREFIX + mNextRequestCode;
    mNextRequestCode = (mNextRequestCode + 1) % REQUEST_CODE_RANGE_SIZE;
    return requestCode;
}

ECode ActivityWindowAndroid::StoreCallbackData(
    /* in */ Int32 requestCode,
    /* in */ IIntentCallback* callback,
    /* in */ Int32 errorId)
{
    VALIDATE_NOT_NULL(callback);
    String errorStr;
    mApplicationContext->GetString(errorId, &errorStr);
    mOutstandingIntents->Put(requestCode, callback);
    mIntentErrors->Put(requestCode, errorStr);
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

