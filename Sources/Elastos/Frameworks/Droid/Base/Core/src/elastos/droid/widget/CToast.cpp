
#include "elastos/droid/widget/CToast.h"

namespace Elastos {
namespace Droid {
namespace Widget {


ECode CToast::constructor(
    /* [in] */ IContext* context)
{
    return Toast::Init(context);
}

PInterface CToast::Probe(
    /* [in] */ REIID riid)
{
    if (riid == Elastos::Droid::Widget::EIID_Toast) {
        return reinterpret_cast<PInterface>((Toast*)this);
    }
    return _CToast::Probe(riid);
}

/**
* Show the view for the specified duration.
*/
ECode CToast::Show()
{
    return Toast::Show();
}

/**
* Close the view if it's showing, or don't show it if it isn't showing yet.
* You do not normally have to call this.  Normally view will disappear on its own
* after the appropriate duration.
*/
ECode CToast::Cancel()
{
    return Toast::Cancel();
}

/**
* Set the view to show.
* @see #getView
*/
ECode CToast::SetView(
    /* [in] */ IView* view)
{
    return Toast::SetView(view);
}

/**
* Return the view.
* @see #setView
*/
ECode CToast::GetView(
    /* [out] */ IView** view)
{
    return Toast::GetView(view);
}

/**
* Set how long to show the view for.
* @see #LENGTH_SHORT
* @see #LENGTH_LONG
*/
ECode CToast::SetDuration(
    /* [in] */ Int32 duration)
{
    return Toast::SetDuration(duration);
}

/**
* Return the duration.
* @see #setDuration
*/
ECode CToast::GetDuration(
    /* [out] */ Int32* duration)
{
    return Toast::GetDuration(duration);
}

/**
* Set the margins of the view.
*
* @param horizontalMargin The horizontal margin, in percentage of the
*        container width, between the container's edges and the
*        notification
* @param verticalMargin The vertical margin, in percentage of the
*        container height, between the container's edges and the
*        notification
*/
ECode CToast::SetMargin(
    /* [in] */ Float horizontalMargin,
    /* [in] */ Float verticalMargin)
{
    return Toast::SetMargin(horizontalMargin, verticalMargin);
}

/**
* Return the horizontal margin.
*/
ECode CToast::GetHorizontalMargin(
    /* [out] */ Float* horizontalMargin)
{
    return Toast::GetHorizontalMargin(horizontalMargin);
}

/**
* Return the vertical margin.
*/
ECode CToast::GetVerticalMargin(
    /* [out] */ Float* verticalMargin)
{
    return Toast::GetVerticalMargin(verticalMargin);
}

/**
* Set the location at which the notification should appear on the screen.
* @see android.view.Gravity
* @see #getGravity
*/
ECode CToast::SetGravity(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 xOffset,
    /* [in] */ Int32 yOffset)
{
    return Toast::SetGravity(gravity, xOffset, yOffset);
}

/**
* Get the location at which the notification should appear on the screen.
* @see android.view.Gravity
* @see #getGravity
*/
ECode CToast::GetGravity(
    /* [out] */ Int32* gravity)
{
    return Toast::GetGravity(gravity);
}

/**
* Return the X offset in pixels to apply to the gravity's location.
*/
ECode CToast::GetXOffset(
    /* [out] */ Int32* xOffset)
{
    return Toast::GetXOffset(xOffset);
}

/**
* Return the Y offset in pixels to apply to the gravity's location.
*/
ECode CToast::GetYOffset(
    /* [out] */ Int32* yOffset)
{
    return Toast::GetYOffset(yOffset);
}

/**
 * Update the text in a Toast that was previously created using one of the makeText() methods.
 * @param resId The new text for the Toast.
 */
ECode CToast::SetText(
    /* [in] */ Int32 resId)
{
    return Toast::SetText(resId);
}

/**
 * Update the text in a Toast that was previously created using one of the makeText() methods.
 * @param s The new text for the Toast.
 */
ECode CToast::SetText(
    /* [in] */ ICharSequence* s)
{
    return Toast::SetText(s);
}

ECode CToast::OnHide()
{
    return Toast::OnHide();
}

ECode CToast::IsShowing(
        /* [out] */ Boolean* showing)
{
    return Toast::IsShowing(showing);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
