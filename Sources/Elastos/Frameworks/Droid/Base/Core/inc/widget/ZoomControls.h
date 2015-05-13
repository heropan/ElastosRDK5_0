
#ifndef __ZOOMCONTROLS_H__
#define __ZOOMCONTROLS_H__

#include "widget/LinearLayout.h"

/**
 * The {@code ZoomControls} class displays a simple set of controls used for zooming and
 * provides callbacks to register for events. */

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;

namespace Elastos {
namespace Droid {
namespace Widget {

class ZoomControls : public LinearLayout
{
public:
    ZoomControls(
        /* [in] */ IContext* context);

    ZoomControls(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetOnZoomInClickListener(
        /* [in] */ IViewOnClickListener* listener);

    CARAPI SetOnZoomOutClickListener(
        /* [in] */ IViewOnClickListener* listener);

    /*
     * Sets how fast you get zoom events when the user holds down the
     * zoom in/out buttons.
     */
    CARAPI SetZoomSpeed(
        /* [in] */ Int64 speed);

    // @Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI Show();

    CARAPI Hide();


    CARAPI SetIsZoomInEnabled(
        /* [in] */ Boolean isEnabled);

    CARAPI SetIsZoomOutEnabled(
        /* [in] */ Boolean isEnabled);

    // @Override
    CARAPI_(Boolean) HasFocus();

    // @Override
    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    // @Override
    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    ZoomControls();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    CARAPI InitInternal(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

private:
    CARAPI Fade(
        /* [in] */ Int32 visibility,
        /* [in] */ Float startAlpha,
        /* [in] */ Float endAlpha);

    AutoPtr<IZoomButton> mZoomIn;
    AutoPtr<IZoomButton> mZoomOut;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
