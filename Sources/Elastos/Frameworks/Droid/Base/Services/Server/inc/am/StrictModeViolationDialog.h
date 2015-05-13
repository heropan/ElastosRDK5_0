
#ifndef __STRICTMODEVIOLATIONDIALOG_H__
#define __STRICTMODEVIOLATIONDIALOG_H__

#include "ext/frameworkext.h"
#include "am/BaseErrorDialog.h"
#include "am/AppErrorResult.h"
#include "am/ProcessRecord.h"
#include "os/HandlerBase.h"
#include <elastos/Mutex.h>

using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class StrictModeViolationDialog
    : public BaseErrorDialog
{
private:
    class MyHandler
        : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ StrictModeViolationDialog* owner);

        ~MyHandler();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        StrictModeViolationDialog* mOwner;
    };

public:
    StrictModeViolationDialog(
        /* [in] */ IContext* context,
        /* [in] */ CActivityManagerService* service,
        /* [in] */ AppErrorResult* result,
        /* [in] */ ProcessRecord* app);

private:
    friend class MyHandler;

    static const String TAG;

public:
    // Event 'what' codes
    static const Int32 ACTION_OK;
    static const Int32 ACTION_OK_AND_REPORT;

    // 5-minute timeout, then we automatically dismiss the crash dialog
    static const Int64 DISMISS_TIMEOUT;

private:
    CActivityManagerService* mService;
    AutoPtr<AppErrorResult> mResult;
    AutoPtr<ProcessRecord> mProc;

    AutoPtr<IHandler> mHandler;

    Mutex mProcLock;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__STRICTMODEVIOLATIONDIALOG_H__
