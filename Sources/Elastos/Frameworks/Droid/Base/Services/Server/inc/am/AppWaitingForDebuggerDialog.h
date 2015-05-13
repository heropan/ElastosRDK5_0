
#ifndef __APPWAITINGFORDEBUGGERDIALOG_H__
#define __APPWAITINGFORDEBUGGERDIALOG_H__

#include "ext/frameworkext.h"
#include "am/BaseErrorDialog.h"
#include "am/ProcessRecord.h"
#include "am/CActivityManagerService.h"
#include "os/HandlerBase.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class AppWaitingForDebuggerDialog : public BaseErrorDialog
{
private:
	class MyHandler
	    : public HandlerBase
	{
	public:
		MyHandler(
			/* [in] */ AppWaitingForDebuggerDialog* host)
			: mHost(host)
		{}

        CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

    private:
    	AppWaitingForDebuggerDialog* mHost;
	};

public:
    AppWaitingForDebuggerDialog(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ IContext* context,
        /* [in] */ ProcessRecord* app);

    void OnStop();

public:
    CActivityManagerService* mService;  // weak-ref
    AutoPtr<ProcessRecord> mProc;

private:
    AutoPtr<ICharSequence> mAppName;
    AutoPtr<IHandler> mHandler;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__APPWAITINGFORDEBUGGERDIALOG_H__
