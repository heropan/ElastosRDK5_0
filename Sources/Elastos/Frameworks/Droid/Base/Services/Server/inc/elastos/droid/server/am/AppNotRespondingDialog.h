
#ifndef __ELASTOS_DROID_SERVER_AM_APPNOTRESPONDINGDIALOG_H__
#define __ELASTOS_DROID_SERVER_AM_APPNOTRESPONDINGDIALOG_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/server/am/BaseErrorDialog.h"
#include "elastos/droid/server/am/ProcessRecord.h"

using Elastos::Droid::Os::Handler;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class AppNotRespondingDialog : public BaseErrorDialog
{
private:
    class MyHandler
        : public Handler
    {
    public:
        MyHandler(
            /* [in] */ AppNotRespondingDialog* host);

        ~MyHandler();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AppNotRespondingDialog* mHost;
    };

public:
    AppNotRespondingDialog(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ IContext* context,
        /* [in] */ ProcessRecord* app,
        /* [in] */ ActivityRecord* activity,
        /* [in] */ Boolean aboveSystem);

    CARAPI OnStop();

private:
    static const String TAG;

    AutoPtr<IHandler> mHandler;

public:
    // Event 'what' codes
    static const Int32 FORCE_CLOSE;
    static const Int32 WAIT;
    static const Int32 WAIT_AND_REPORT;

private:
    AutoPtr<CActivityManagerService> mService;
    AutoPtr<ProcessRecord> mProc;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_APPNOTRESPONDINGDIALOG_H__
