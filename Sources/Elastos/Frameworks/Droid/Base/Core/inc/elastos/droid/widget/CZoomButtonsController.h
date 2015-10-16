#ifndef __ELASTOS_DROID_WIDGET_CZOOMBUTTONSCONTROLLER_H__
#define __ELASTOS_DROID_WIDGET_CZOOMBUTTONSCONTROLLER_H__

#include "_Elastos_Droid_Widget_CZoomButtonsController.h"
#include "elastos/droid/widget/ZoomButtonsController.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CZoomButtonsController) , public ZoomButtonsController
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IView* v);

    /**
     * Whether to enable the zoom in control.
     *
     * @param enabled Whether to enable the zoom in control.
     */
    CARAPI SetZoomInEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Whether to enable the zoom out control.
     *
     * @param enabled Whether to enable the zoom out control.
     */
    CARAPI SetZoomOutEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Sets the delay between zoom callbacks as the user holds a zoom button.    }
     *
     * @param speed The delay in milliseconds between zoom callbacks.
     */
    CARAPI SetZoomSpeed(
        /* [in] */ Int64 speed);

    /**
     * Sets the {@link OnZoomListener} listener that receives callbacks to zoom.
     *
     * @param listener The listener that will be told to zoom.
     */
    CARAPI SetOnZoomListener(
        /* [in] */ IOnZoomListener* listener);

    /**
     * Sets whether the zoom controls should be focusable. If the controls are
     * focusable, then trackball and arrow key interactions are possible.
     * Otherwise, only touch interactions are possible.
     *
     * @param focusable Whether the zoom controls should be focusable.
     */
    CARAPI SetFocusable(
        /* [in] */ Boolean focusable);

    /**
     * Whether the zoom controls will be automatically dismissed after showing.
     *
     * @return Whether the zoom controls will be auto dismissed after showing.
     */
    CARAPI IsAutoDismissed(
        /* [out] */ Boolean* dimissed);

    /**
     * Sets whether the zoom controls will be automatically dismissed after
     * showing.
     */
    CARAPI SetAutoDismissed(
        /* [in] */ Boolean autoDismiss);

    /**
     * Whether the zoom controls are visible to the user.
     *
     * @return Whether the zoom controls are visible to the user.
     */
    CARAPI IsVisible(
        /* [out] */ Boolean* visible);

    /**
     * Sets whether the zoom controls should be visible to the user.
     *
     * @param visible Whether the zoom controls should be visible to the user.
     */
    CARAPI SetVisible(
        /* [in] */ Boolean visible);

    /**
     * Gets the container that is the parent of the zoom controls.
     * <p>
     * The client can add other views to this container to link them with the
     * zoom controls.
     *
     * @return The container of the zoom controls. It will be a layout that
     *         respects the gravity of a child's layout parameters.
     */
    CARAPI GetContainer(
        /* [out] */ IViewGroup** container);

    /**
     * Gets the view for the zoom controls.
     *
     * @return The zoom controls view.
     */
    CARAPI GetZoomControls(
        /* [out] */ IView** controls);

    /**
     * @hide The ZoomButtonsController implements the OnTouchListener, but this
     *       does not need to be shown in its public API.
     */
    CARAPI OnTouch(
        /* [in] */ IView* v,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
