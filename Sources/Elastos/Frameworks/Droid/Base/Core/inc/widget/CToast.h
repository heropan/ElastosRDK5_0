#ifndef __ELASTOS_DROID_WIDGET_CTOAST_H__
#define __ELASTOS_DROID_WIDGET_CTOAST_H__

#include "_CToast.h"
#include "widget/Toast.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CToast), public Toast
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
    * Show the view for the specified duration.
    */
    CARAPI Show();

    /**
    * Close the view if it's showing, or don't show it if it isn't showing yet.
    * You do not normally have to call this.  Normally view will disappear on its own
    * after the appropriate duration.
    */
    CARAPI Cancel();

    /**
    * Set the view to show.
    * @see #getView
    */
    CARAPI SetView(
        /* [in] */ IView* view);

    /**
    * Return the view.
    * @see #setView
    */
    CARAPI GetView(
        /* [out] */ IView** view);

    /**
    * Set how long to show the view for.
    * @see #LENGTH_SHORT
    * @see #LENGTH_LONG
    */
    CARAPI SetDuration(
        /* [in] */ Int32 duration);

    /**
    * Return the duration.
    * @see #setDuration
    */
    CARAPI GetDuration(
        /* [out] */ Int32* duration);

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
    CARAPI SetMargin(
        /* [in] */ Float horizontalMargin,
        /* [in] */ Float verticalMargin);

    /**
    * Return the horizontal margin.
    */
    CARAPI GetHorizontalMargin(
        /* [out] */ Float* horizontalMargin);

    /**
    * Return the vertical margin.
    */
    CARAPI GetVerticalMargin(
        /* [out] */ Float* verticalMargin);

    /**
    * Set the location at which the notification should appear on the screen.
    * @see android.view.Gravity
    * @see #getGravity
    */
    CARAPI SetGravity(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 xOffset,
        /* [in] */ Int32 yOffset);

    /**
    * Get the location at which the notification should appear on the screen.
    * @see android.view.Gravity
    * @see #getGravity
    */
    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    /**
    * Return the X offset in pixels to apply to the gravity's location.
    */
    CARAPI GetXOffset(
        /* [out] */ Int32* xOffset);

    /**
    * Return the Y offset in pixels to apply to the gravity's location.
    */
    CARAPI GetYOffset(
        /* [out] */ Int32* yOffset);

    /**
     * Update the text in a Toast that was previously created using one of the makeText() methods.
     * @param resId The new text for the Toast.
     */
    CARAPI SetText(
        /* [in] */ Int32 resId);

    /**
     * Update the text in a Toast that was previously created using one of the makeText() methods.
     * @param s The new text for the Toast.
     */
    CARAPI SetText(
        /* [in] */ ICharSequence* s);

    CARAPI OnHide();

    CARAPI IsShowing(
        /* [out] */ Boolean* showing);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CTOAST_H__
