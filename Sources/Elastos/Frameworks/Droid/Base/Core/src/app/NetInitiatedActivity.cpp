
#include "app/NetInitiatedActivity.h"
#include "content/CIntentFilter.h"
#include "content/BroadcastReceiver.h"
#include "location/LocationManager.h"
#include <elastos/Slogger.h>

using Elastos::Core::CStringWrapper;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::EIID_IBroadcastReceiver;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Location::LocationManager;
using Elastos::Droid::Location::ILocationManager;
using Elastos::Droid::Location::IGpsNetInitiatedHandler;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

ECode NetInitiatedReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (NetInitiatedActivity::DEBUG) Slogger::D(NetInitiatedActivity::TAG,
        "NetInitiatedReceiver onReceive: %s", action.string());
    if (action == IGpsNetInitiatedHandler::ACTION_NI_VERIFY) {
        mHost->HandleNIVerify(intent);
    }
    return NOERROR;
}


const String NetInitiatedActivity::TAG = String("NetInitiatedActivity");
const Boolean NetInitiatedActivity::DEBUG;
const Boolean NetInitiatedActivity::VERBOSE;
const Int32 NetInitiatedActivity::POSITIVE_BUTTON;
const Int32 NetInitiatedActivity::NEGATIVE_BUTTON;
const Int32 NetInitiatedActivity::GPS_NO_RESPONSE_TIME_OUT;


ECode NetInitiatedActivity::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case NetInitiatedActivity::GPS_NO_RESPONSE_TIME_OUT:
            mHost->HandleGPSNoResponseTimeout();
            break;
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(NetInitiatedActivity, IDialogInterfaceOnClickListener);

NetInitiatedActivity::NetInitiatedActivity()
    : mNotificationId(-1)
    , mTimeout(-1)
    , mDefault_response(-1)
    , mDefault_response_timeout(6)
    , mNetInitiatedReceiver(new NetInitiatedReceiver(this))
{
    mHandler = new MyHandler(this);
}

ECode NetInitiatedActivity::HandleGPSNoResponseTimeout()
{
    if (mNotificationId != -1) {
        SendUserResponse(mDefault_response);
    }
    Finish();
    return NOERROR;
}

ECode NetInitiatedActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    AlertActivity::OnCreate(savedInstanceState);

    // Set up the "dialog"
    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);
    AutoPtr<CAlertControllerAlertParams> p = mAlertParams;
    AutoPtr<IContext> context;
    GetApplicationContext((IContext**)&context);
    p->SetIconId(0/*com.android.internal.R.drawable.ic_dialog_usb*/);
    String title;
    intent->GetStringExtra(IGpsNetInitiatedHandler::NI_INTENT_KEY_TITLE, &title);
    AutoPtr<ICharSequence> titleSequence;
    CStringWrapper::New(title, (ICharSequence**)&titleSequence);
    p->SetTitle(titleSequence);
    String message;
    intent->GetStringExtra(IGpsNetInitiatedHandler::NI_INTENT_KEY_MESSAGE, &message);
    AutoPtr<ICharSequence> mSequence;
    CStringWrapper::New(message, (ICharSequence**)&mSequence);
    p->SetMessage(mSequence);
//    p->mPositiveButtonText = String.format(context.getString(R.string.gpsVerifYes));
    p->SetPositiveButtonListener(this);
//    p->mNegativeButtonText = String.format(context.getString(R.string.gpsVerifNo));
    p->SetNegativeButtonListener(this);

    intent->GetInt32Extra(IGpsNetInitiatedHandler::NI_INTENT_KEY_NOTIF_ID, -1, &mNotificationId);
    intent->GetInt32Extra(IGpsNetInitiatedHandler::NI_INTENT_KEY_TIMEOUT,
            mDefault_response_timeout,
            &mTimeout);
    intent->GetInt32Extra(IGpsNetInitiatedHandler::NI_INTENT_KEY_DEFAULT_RESPONSE,
            IGpsNetInitiatedHandler::GPS_NI_RESPONSE_ACCEPT,
            &mDefault_response);
    if (DEBUG) Slogger::D(TAG, "onCreate() : notificationId: %d timeout: %d default_response:%d", mNotificationId, mTimeout, mDefault_response);

    Boolean result;
    mHandler->SendEmptyMessageDelayed(GPS_NO_RESPONSE_TIME_OUT, (mTimeout * 1000), &result);
    SetupAlert();
    return NOERROR;
}

ECode NetInitiatedActivity::OnResume()
{
    AlertActivity::OnResume();
    if (DEBUG) Slogger::D(TAG, "onResume");
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New(IGpsNetInitiatedHandler::ACTION_NI_VERIFY, (IIntentFilter**)&intentFilter);
    AutoPtr<IIntent> intent;
    RegisterReceiver(mNetInitiatedReceiver, intentFilter, (IIntent**)&intent);
    return NOERROR;
}

ECode NetInitiatedActivity::OnPause()
{
    AlertActivity::OnPause();
    if (DEBUG) Slogger::D(TAG, "onPause");
    UnregisterReceiver(mNetInitiatedReceiver);
    return NOERROR;
}

ECode NetInitiatedActivity::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    if (which == POSITIVE_BUTTON) {
        SendUserResponse(IGpsNetInitiatedHandler::GPS_NI_RESPONSE_ACCEPT);
    }
    if (which == NEGATIVE_BUTTON) {
        SendUserResponse(IGpsNetInitiatedHandler::GPS_NI_RESPONSE_DENY);
    }

    // No matter what, finish the activity
    Finish();
    mNotificationId = -1;
    return NOERROR;
}

void NetInitiatedActivity::SendUserResponse(
    /* [in] */ Int32 response)
{
    if (DEBUG) Slogger::D(TAG, "sendUserResponse, response: %d", response);
    AutoPtr<ILocationManager> locationManager;
    this->GetSystemService(IContext::LOCATION_SERVICE,
            (IInterface**)(ILocationManager**)&locationManager);
    Boolean result;
    locationManager->SendNiResponse(mNotificationId, response, &result);
}

void NetInitiatedActivity::HandleNIVerify(
    /* [in] */ IIntent* intent)
{
    Int32 notifId;
    intent->GetInt32Extra(IGpsNetInitiatedHandler::NI_INTENT_KEY_NOTIF_ID, -1, &notifId);
    mNotificationId = notifId;

    String action;
    intent->GetAction(&action);
    if (DEBUG) Slogger::D(TAG, "handleNIVerify action: %s", action.string());
}

void NetInitiatedActivity::ShowNIError()
{
//    Toast.makeText(this, "NI error" /* com.android.internal.R.string.usb_storage_error_message */,
//            Toast.LENGTH_LONG).show();
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos
