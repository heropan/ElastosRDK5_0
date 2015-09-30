
#include "elastos/droid/webkit/native/ui/base/ActivityWindowAndroid.h"

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
    : WindowAndroid(NULL) // activity
{
    // ==================before translated======================
    // super(activity.getApplicationContext());
    // mActivityRef = new WeakReference<Activity>(activity);
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
    return 0;
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
    return 0;
}

ECode ActivityWindowAndroid::CancelIntent(
    /* [in] */ Int32 requestCode)
{
    // ==================before translated======================
    // Activity activity = mActivityRef.get();
    // if (activity == null) return;
    // activity.finishActivity(requestCode);
    assert(0);
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
    return FALSE;
}

AutoPtr<IWeakReference> ActivityWindowAndroid::GetActivity()
{
    // ==================before translated======================
    // // Return a new WeakReference to prevent clients from releasing our internal WeakReference.
    // return new WeakReference<Activity>(mActivityRef.get());
    assert(0);
    AutoPtr<IWeakReference> empty;
    return empty;
}

Int32 ActivityWindowAndroid::GenerateNextRequestCode()
{
    // ==================before translated======================
    // int requestCode = REQUEST_CODE_PREFIX + mNextRequestCode;
    // mNextRequestCode = (mNextRequestCode + 1) % REQUEST_CODE_RANGE_SIZE;
    // return requestCode;
    assert(0);
    return 0;
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
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


