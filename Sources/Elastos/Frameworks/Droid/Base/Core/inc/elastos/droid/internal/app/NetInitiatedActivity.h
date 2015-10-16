
#ifndef __ELASTOS_DROID_INTERNAL_APP_NETINITIATEDACTIVITY_H__
#define __ELASTOS_DROID_INTERNAL_APP_NETINITIATEDACTIVITY_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/app/AlertActivity.h"
#include "elastos/droid/os/HandlerBase.h"

#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IPendingResult;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Internal::App::AlertActivity;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class NetInitiatedActivity;

class NetInitiatedReceiver
    : public BroadcastReceiver
{
public:
    NetInitiatedReceiver(
        /* [in] */ NetInitiatedActivity* host)
        : mHost(host)
    {}

    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    CARAPI ToString(
        /* [out] */ String* info)
    {
        VALIDATE_NOT_NULL(info);
        *info = String("NetInitiatedReceiver: ");
        (*info).AppendFormat("%p", this);
        return NOERROR;
    }

private:
    NetInitiatedActivity* mHost;
};

class NetInitiatedActivity
    : public AlertActivity
    , public IDialogInterfaceOnClickListener
{
friend class NetInitiatedReceiver;

private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ NetInitiatedActivity* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        NetInitiatedActivity* mHost;
    };

public:
    NetInitiatedActivity();

    CAR_INTERFACE_DECL();

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

private:
    CARAPI_(void) SendUserResponse(
        /* [in] */ Int32 response);

    CARAPI_(void) HandleNIVerify(
        /* [in] */ IIntent* intent);

    CARAPI_(void) ShowNIError();

    CARAPI HandleGPSNoResponseTimeout();

private:
    static const String TAG;

    static const Boolean DEBUG = TRUE;
    static const Boolean VERBOSE = FALSE;

    static const Int32 POSITIVE_BUTTON = IAlertDialog::BUTTON_POSITIVE;
    static const Int32 NEGATIVE_BUTTON = IAlertDialog::BUTTON_NEGATIVE;

    static const Int32 GPS_NO_RESPONSE_TIME_OUT = 1;
    // Received ID from intent, -1 when no notification is in progress
    Int32 mNotificationId;
    Int32 mTimeout;
    Int32 mDefault_response;
    Int32 mDefault_response_timeout;

    AutoPtr<NetInitiatedReceiver> mNetInitiatedReceiver;
    AutoPtr<IHandler> mHandler;
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_NETINITIATEDACTIVITY_H__
