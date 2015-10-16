#ifndef __ELASTOS_DROID_WIDGET_CTOASTTRANSIENTNOTIFICATION_H__
#define __ELASTOS_DROID_WIDGET_CTOASTTRANSIENTNOTIFICATION_H__

#include "_Elastos_Droid_Widget_CToastTransientNotification.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IWindowManagerLayoutParams;

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CToastTransientNotification)
{
private:
    class ShowAction : public Runnable
    {
    public:
        ShowAction(
            /* [in] */ CToastTransientNotification* host);

        CARAPI Run();

    private:
        CToastTransientNotification* mHost;
    };

    class HideAction : public Runnable
    {
    public:
        HideAction(
            /* [in] */ CToastTransientNotification* host);

        CARAPI Run();

    private:
        CToastTransientNotification* mHost;
    };

public:
    CToastTransientNotification();

    CARAPI constructor(
        /* [in] */ IToast* toast);

    CARAPI Show();

    CARAPI Hide();

    CARAPI SetNextView(
        /* [in] */ IView* nextView);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetX(
        /* [in] */ Int32 x);

    CARAPI GetX(
        /* [out] */ Int32* x);

    CARAPI SetY(
        /* [in] */ Int32 y);

    CARAPI GetY(
        /* [out] */ Int32* y);

    CARAPI SetHorizontalMargin(
        /* [in] */ Float horizontalMargin);

    CARAPI GetHorizontalMargin(
        /* [out] */ Float* horizontalMargin);

    CARAPI SetVerticalMargin(
        /* [in] */ Float verticalMargin);

    CARAPI GetVerticalMargin(
        /* [out] */ Float* verticalMargin);

    CARAPI ToString(
        /* [out] */ String* str);
private:
    CARAPI HandleShow();

    CARAPI HandleHide();

private:
    CARAPI TrySendAccessibilityEvent();

public:
    AutoPtr<IView> mView;
    AutoPtr<IView> mNextView;

private:
    /* add a callback method when the toast hides and another method to check whether the toast is showing */
    AutoPtr<IWeakReference> mToast; // IToast

    AutoPtr<IRunnable> mShow;
    AutoPtr<IRunnable> mHide;
    AutoPtr<IWindowManagerLayoutParams> mParams; //= new WindowManager.LayoutParams();
    AutoPtr<IWindowManager> mWM;

    AutoPtr<IHandler> mHandler;// = new Handler();

    Int32 mGravity;// = Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM;
    Int32 mX;
    Int32 mY;
    Float mHorizontalMargin;
    Float mVerticalMargin;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CTOASTTRANSIENTNOTIFICATION_H__
