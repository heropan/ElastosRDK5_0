
#ifndef __FACTORYERRORDIALOG_H__
#define __FACTORYERRORDIALOG_H__

#include "ext/frameworkext.h"
#include "am/BaseErrorDialog.h"
#include "os/HandlerBase.h"

using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class FactoryErrorDialog
    : public BaseErrorDialog
{
private:
    class MyHandler
        : public HandlerBase
    {
    public:
        MyHandler();

        ~MyHandler();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    };

public:
    FactoryErrorDialog(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* msg);

    CARAPI_(void) OnStop();

private:
    AutoPtr<MyHandler> mHandler;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__FACTORYERRORDIALOG_H__
