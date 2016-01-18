
#ifndef __ELASTOS_DROID_SERVER_AM_BASEERRORDIALOG_H__
#define __ELASTOS_DROID_SERVER_AM_BASEERRORDIALOG_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/app/AlertDialog.h"
#include "elastos/droid/os/Handler.h"

using Elastos::Droid::App::AlertDialog;
using Elastos::Droid::Os::Handler;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class BaseErrorDialog
    : public AlertDialog
{
private:
    class MyHandler : public Handler
    {
    public:
        MyHandler(
            /* [in] */ BaseErrorDialog* service)
            : mHost(service)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        BaseErrorDialog* mHost;
    };

public:
    BaseErrorDialog();

    BaseErrorDialog(
        /* [in] */ IContext* context);

    CARAPI OnStart();

    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

protected:
    CARAPI Init(
        /* [in] */ IContext* context);

private:
    CARAPI_(void) HandleOnStartMessage();

    CARAPI_(void) SetEnabled(
        /* [in] */ Boolean enabled);

private:
    static const Int32 ENABLE_BUTTONS;
    static const Int32 DISABLE_BUTTONS;
    Boolean mConsuming;

public:
    AutoPtr<IHandler> mHandler;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_BASEERRORDIALOG_H__
