
#ifndef __ELASTOS_DROID_WIDGET_ZOOMBUTTON_H__
#define __ELASTOS_DROID_WIDGET_ZOOMBUTTON_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/ImageButton.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;

namespace Elastos {
namespace Droid {
namespace Widget {

class ZoomButton : public ImageButton
{
private:
    class ZoomButtonRunnable
        : public Runnable
    {
    public:
        ZoomButtonRunnable(
            /* [in] */ ZoomButton* host);

        CARAPI Run();

    private:
        ZoomButton* mHost;
    };

public:
    ZoomButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI SetZoomSpeed(
        /* [in] */ Int64 speed);

    CARAPI_(Boolean) OnLongClick(
        /* [in] */ IView* v);

    CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI_(Boolean) DispatchUnhandledMove(
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* event);

protected:
    ZoomButton();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

private:
    AutoPtr<IHandler> mHandler;
    AutoPtr<IRunnable> mRunnable;

    Int64 mZoomSpeed; //= 1000;
    Boolean mIsInLongpress;
};


} //namespace Widget
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_ZOOMBUTTON_H__
