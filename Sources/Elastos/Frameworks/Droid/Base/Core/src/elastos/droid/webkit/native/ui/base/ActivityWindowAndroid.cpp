
#include "elastos/droid/webkit/native/ui/base/ActivityWindowAndroid.h"
#include "elastos/droid/content/CIntent.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IContentResolver;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                        ActivityWindowAndroid
//=====================================================================
const Int32 ActivityWindowAndroid::REQUEST_CODE_PREFIX;
const Int32 ActivityWindowAndroid::REQUEST_CODE_RANGE_SIZE;
const String ActivityWindowAndroid::TAG("ActivityWindowAndroid");

ActivityWindowAndroid::ActivityWindowAndroid(
    /* [in] */ IActivity* activity)
    : WindowAndroid(NULL) // WindowAndroid(context)
{
    // ==================before translated======================
    // super(activity.getApplicationContext());
    // mActivityRef = new WeakReference<Activity>(activity);

    AutoPtr<IContext> tmp = IContext::Probe(activity);
    AutoPtr<IContext> context;
    tmp->GetApplicationContext((IContext**)&context);

    AutoPtr<IWeakReferenceSource> source = IWeakReferenceSource::Probe((IWeakReferenceSource*)activity);
    source->GetWeakReference((IWeakReference**)&mActivityRef);
}

Int32 ActivityWindowAndroid::ShowCancelableIntent(
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // Activity activity = mActivityRef.get();
    // if (activity == null) return START_INTENT_FAILURE;
    //
    // int requestCode = generateNextRequestCode();
    //
    // try {
    //     activity.startIntentSenderForResult(
    //             intent.getIntentSender(), requestCode, new Intent(), 0, 0, 0);
    // } catch (SendIntentException e) {
    //     return START_INTENT_FAILURE;
    // }
    //
    // storeCallbackData(requestCode, callback, errorId);
    // return requestCode;

    assert(0);
    AutoPtr<IActivity> activityTmp;
    mActivityRef->Resolve(EIID_IInterface, (IInterface**)&activityTmp);
    AutoPtr<IActivity> activity = IActivity::Probe(activityTmp);
    if (NULL == activity)
        return START_INTENT_FAILURE;

    Int32 requestCode = GenerateNextRequestCode();

    //try {
        AutoPtr<IIntentSender> sender;
        intent->GetIntentSender((IIntentSender**)&sender);
        AutoPtr<IIntent> newIntent;
        CIntent::New((IIntent**)&newIntent);
        //-- hasno matching func activity->StartIntentSenderForResult(sender, requestCode, newIntent, 0, 0, 0);
    //} catch (SendIntentException e) {
    //    return START_INTENT_FAILURE;
    //}

    StoreCallbackData(requestCode, callback, errorId);
    return requestCode;
}

Int32 ActivityWindowAndroid::ShowCancelableIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    // ==================before translated======================
    // Activity activity = mActivityRef.get();
    // if (activity == null) return START_INTENT_FAILURE;
    //
    // int requestCode = generateNextRequestCode();
    //
    // try {
    //     activity.startActivityForResult(intent, requestCode);
    // } catch (ActivityNotFoundException e) {
    //     return START_INTENT_FAILURE;
    // }
    //
    // storeCallbackData(requestCode, callback, errorId);
    // return requestCode;

    assert(0);
    AutoPtr<IActivity> activityTmp;
    mActivityRef->Resolve(EIID_IInterface, (IInterface**)&activityTmp);
    AutoPtr<IActivity> activity = IActivity::Probe(activityTmp);
    if (NULL == activity)
        return START_INTENT_FAILURE;

    Int32 requestCode = GenerateNextRequestCode();

    //try {
        AutoPtr<IIntent> newIntent;
        CIntent::New((IIntent**)&newIntent);
        //-- hasno matching func activity->StartIntentSenderForResult(sender, requestCode, newIntent, 0, 0, 0);
        // java call: activity->StartActivityForResult(intent, requestCode);
    //} catch (SendIntentException e) {
    //    return START_INTENT_FAILURE;
    //}

    StoreCallbackData(requestCode, callback, errorId);
    return requestCode;
}

ECode ActivityWindowAndroid::CancelIntent(
    /* [in] */ Int32 requestCode)
{
    // ==================before translated======================
    // Activity activity = mActivityRef.get();
    // if (activity == null) return;
    // activity.finishActivity(requestCode);

    AutoPtr<IActivity> activityTmp;
    mActivityRef->Resolve(EIID_IInterface, (IInterface**)&activityTmp);
    AutoPtr<IActivity> activity = IActivity::Probe(activityTmp);
    if (NULL == activity)
        return NOERROR;
    activity->FinishActivity(requestCode);
    return NOERROR;
}

Boolean ActivityWindowAndroid::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    // ==================before translated======================
    // IntentCallback callback = mOutstandingIntents.get(requestCode);
    // mOutstandingIntents.delete(requestCode);
    // String errorMessage = mIntentErrors.remove(requestCode);
    //
    // if (callback != null) {
    //     callback.onIntentCompleted(this, resultCode,
    //             mApplicationContext.getContentResolver(), data);
    //     return true;
    // } else {
    //     if (errorMessage != null) {
    //         showCallbackNonExistentError(errorMessage);
    //         return true;
    //     }
    // }
    // return false;

    assert(0);
    AutoPtr<IInterface> interfaceTmp;
    mOutstandingIntents->Get(requestCode, (IInterface**)&interfaceTmp);
    IObject* objectTmp = IObject::Probe(interfaceTmp);
    Object* objectTmp1 = (Object*)objectTmp;
    AutoPtr<IntentCallback> callback = (IntentCallback*)objectTmp1;
    mOutstandingIntents->Delete(requestCode);

    AutoPtr<IInteger32> requestCodeTmp;
    CInteger32::New(requestCode, (IInteger32**)&requestCodeTmp);
    AutoPtr<ICharSequence> errorMessageTmp;
    mIntentErrors->Remove(requestCodeTmp, (IInterface**)&errorMessageTmp);
    String errorMessage;
    errorMessageTmp->ToString(&errorMessage);

    if (NULL != callback) {
        AutoPtr<IContentResolver> contentResolver;
        mApplicationContext->GetContentResolver((IContentResolver**)&contentResolver);
        callback->OnIntentCompleted(this, resultCode, contentResolver, data);
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

AutoPtr<IWeakReference> ActivityWindowAndroid::GetActivity()
{
    // ==================before translated======================
    // // Return a new WeakReference to prevent clients from releasing our internal WeakReference.
    // return new WeakReference<Activity>(mActivityRef.get());

    AutoPtr<IActivity> activityTmp;
    mActivityRef->Resolve(EIID_IInterface, (IInterface**)&activityTmp);
    IActivity* activity = IActivity::Probe(activityTmp);
    if (NULL == activity)
        return NULL;

    AutoPtr<IWeakReferenceSource> source = IWeakReferenceSource::Probe((IWeakReferenceSource*)activity);
    AutoPtr<IWeakReference> result;
    source->GetWeakReference((IWeakReference**)&result);
    return result;
}

Int32 ActivityWindowAndroid::GenerateNextRequestCode()
{
    // ==================before translated======================
    // int requestCode = REQUEST_CODE_PREFIX + mNextRequestCode;
    // mNextRequestCode = (mNextRequestCode + 1) % REQUEST_CODE_RANGE_SIZE;
    // return requestCode;

    Int32 requestCode = REQUEST_CODE_PREFIX + mNextRequestCode;
    mNextRequestCode = (mNextRequestCode + 1) % REQUEST_CODE_RANGE_SIZE;
    return requestCode;
}

ECode ActivityWindowAndroid::StoreCallbackData(
    /* [in] */ Int32 requestCode,
    /* [in] */ IntentCallback* callback,
    /* [in] */ Int32 errorId)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // mOutstandingIntents.put(requestCode, callback);
    // mIntentErrors.put(requestCode, mApplicationContext.getString(errorId));

    assert(0);
    Object* callbackTmp = (Object*)callback;
    IObject* callbackTmp1 = (IObject*)callbackTmp;
    //IInterface* callbackTmp2 = IInterface::Probe((IObject*)callbackTmp1);
    IInterface* callbackTmp2 = callbackTmp1->Probe(EIID_IInterface);
    mOutstandingIntents->Put(requestCode, callbackTmp2);

    AutoPtr<IInteger32> requestCodeTmp;
    CInteger32::New(requestCode, (IInteger32**)&requestCodeTmp);

    String errorStr;
    mApplicationContext->GetString(errorId, &errorStr);
    AutoPtr<ICharSequence> charSequenceError;
    //CString errorStrTmp(errorStr);
    //errorStrTmp.SubSequence(0, errorStr.GetLength()-1, (ICharSequence**)&charSequenceError);
    mIntentErrors->Put(requestCodeTmp, charSequenceError);
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


