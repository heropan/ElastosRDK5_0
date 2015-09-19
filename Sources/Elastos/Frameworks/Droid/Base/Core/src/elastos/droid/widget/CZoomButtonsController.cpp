#include "widget/CZoomButtonsController.h"

namespace Elastos{
namespace Droid{
namespace Widget{

PInterface CZoomButtonsController::Probe(
    /* [in] */ REIID riid)
{
    return _CZoomButtonsController::Probe(riid);
}

ECode CZoomButtonsController::constructor(
    /* [in] */ IView* v)
{
    return ZoomButtonsController::Init(v);
}

    /**
     * Whether to enable the zoom in control.
     *
     * @param enabled Whether to enable the zoom in control.
     */
ECode CZoomButtonsController::SetZoomInEnabled(
    /* [in] */ Boolean enabled)
{
    return ZoomButtonsController::SetZoomInEnabled(enabled);
}

    /**
     * Whether to enable the zoom out control.
     *
     * @param enabled Whether to enable the zoom out control.
     */
ECode CZoomButtonsController::SetZoomOutEnabled(
    /* [in] */ Boolean enabled)
{
    return ZoomButtonsController::SetZoomOutEnabled(enabled);
}

    /**
     * Sets the delay between zoom callbacks as the user holds a zoom button.    }
     *
     * @param speed The delay in milliseconds between zoom callbacks.
     */
ECode CZoomButtonsController::SetZoomSpeed(
    /* [in] */ Int64 speed)
{
    return ZoomButtonsController::SetZoomSpeed(speed);
}

    /**
     * Sets the {@link OnZoomListener} listener that receives callbacks to zoom.
     *
     * @param listener The listener that will be told to zoom.
     */
ECode CZoomButtonsController::SetOnZoomListener(
    /* [in] */ IOnZoomListener* listener)
{
    return ZoomButtonsController::SetOnZoomListener(listener);
}

    /**
     * Sets whether the zoom controls should be focusable. If the controls are
     * focusable, then trackball and arrow key interactions are possible.
     * Otherwise, only touch interactions are possible.
     *
     * @param focusable Whether the zoom controls should be focusable.
     */
ECode CZoomButtonsController::SetFocusable(
    /* [in] */ Boolean focusable)
{
    return ZoomButtonsController::SetFocusable(focusable);
}

    /**
     * Whether the zoom controls will be automatically dismissed after showing.
     *
     * @return Whether the zoom controls will be auto dismissed after showing.
     */
ECode CZoomButtonsController::IsAutoDismissed(
    /* [out] */ Boolean* dimissed)
{
    VALIDATE_NOT_NULL(dimissed);
    *dimissed = ZoomButtonsController::IsAutoDismissed();
    return NOERROR;
}

    /**
     * Sets whether the zoom controls will be automatically dismissed after
     * showing.
     */
ECode CZoomButtonsController::SetAutoDismissed(
    /* [in] */ Boolean autoDismiss)
{
    return ZoomButtonsController::SetAutoDismissed(autoDismiss);
}

    /**
     * Whether the zoom controls are visible to the user.
     *
     * @return Whether the zoom controls are visible to the user.
     */
ECode CZoomButtonsController::IsVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = ZoomButtonsController::IsVisible();
    return NOERROR;
}

    /**
     * Sets whether the zoom controls should be visible to the user.
     *
     * @param visible Whether the zoom controls should be visible to the user.
     */
ECode CZoomButtonsController::SetVisible(
    /* [in] */ Boolean visible)
{
    return ZoomButtonsController::SetVisible(visible);
}

    /**
     * Gets the container that is the parent of the zoom controls.
     * <p>
     * The client can add other views to this container to link them with the
     * zoom controls.
     *
     * @return The container of the zoom controls. It will be a layout that
     *         respects the gravity of a child's layout parameters.
     */
ECode CZoomButtonsController::GetContainer(
    /* [out] */ IViewGroup** container)
{
    VALIDATE_NOT_NULL(container);
    AutoPtr<IViewGroup> vTemp = ZoomButtonsController::GetContainer();
    *container = vTemp;
    REFCOUNT_ADD(*container)
    return NOERROR;
}

    /**
     * Gets the view for the zoom controls.
     *
     * @return The zoom controls view.
     */
ECode CZoomButtonsController::GetZoomControls(
    /* [out] */ IView** controls)
{
    VALIDATE_NOT_NULL(controls);
    AutoPtr<IView> vTemp = ZoomButtonsController::GetZoomControls();
    *controls = vTemp;
    REFCOUNT_ADD(*controls)
    return NOERROR;
}

    /**
     * @hide The ZoomButtonsController implements the OnTouchListener, but this
     *       does not need to be shown in its public API.
     */
ECode CZoomButtonsController::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    ZoomButtonsController::OnTouch(v, event, res);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
