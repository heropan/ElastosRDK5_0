#ifndef __ELASTOS_DROID_WIDGET_CSCROLLVIEW_H__
#define __ELASTOS_DROID_WIDGET_CSCROLLVIEW_H__

#include "_Elastos_Droid_Widget_CScrollView.h"


#include "widget/ScrollView.h"


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CScrollView), public ScrollView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * @return The maximum amount this scroll view will scroll in response to
     *   an arrow event.
     */
    CARAPI GetMaxScrollAmount(
        /* [out] */ Int32* maxAmount);

    /**
     * Indicates whether this ScrollView's content is stretched to fill the viewport.
     *
     * @return True if the content fills the viewport, false otherwise.
     */
    CARAPI IsFillViewport(
        /* [out] */ Boolean* isFilled);

    /**
     * Indicates this ScrollView whether it should stretch its content height to fill
     * the viewport or not.
     *
     * @param fillViewport True to stretch the content's height to the viewport's
     *        boundaries, false otherwise.
     */
    CARAPI SetFillViewport(
        /* [in] */ Boolean fillViewport);

    /**
     * @return Whether arrow scrolling will animate its transition.
     */
    CARAPI IsSmoothScrollingEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * Set whether arrow scrolling will animate its transition.
     * @param smoothScrollingEnabled whether arrow scrolling will animate its transition
     */
    CARAPI SetSmoothScrollingEnabled(
        /* [in] */ Boolean smoothScrollingEnabled);

    /**
     * You can call this function yourself to have the scroll view perform
     * scrolling from a key event, just as if the event had been dispatched to
     * it by the view hierarchy.
     *
     * @param event The key event to execute.
     * @return Return true if the event was handled, else false.
     */
    CARAPI ExecuteKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* handled);

    /**
     * <p>Handles scrolling in response to a "page up/down" shortcut press. This
     * method will scroll the view by one page up or down and give the focus
     * to the topmost/bottommost component in the new visible area. If no
     * component is a good candidate for focus, this scrollview reclaims the
     * focus.</p>
     *
     * @param direction the scroll direction: {@link android.view.View#FOCUS_UP}
     *                  to go one page up or
     *                  {@link android.view.View#FOCUS_DOWN} to go one page down
     * @return true if the key event is consumed by this method, false otherwise
     */
    CARAPI PageScroll(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* consumed);

    /**
     * <p>Handles scrolling in response to a "home/end" shortcut press. This
     * method will scroll the view to the top or bottom and give the focus
     * to the topmost/bottommost component in the new visible area. If no
     * component is a good candidate for focus, this scrollview reclaims the
     * focus.</p>
     *
     * @param direction the scroll direction: {@link android.view.View#FOCUS_UP}
     *                  to go the top of the view or
     *                  {@link android.view.View#FOCUS_DOWN} to go the bottom
     * @return true if the key event is consumed by this method, false otherwise
     */
    CARAPI FullScroll(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* consumed);

    /**
     * Handle scrolling in response to an up or down arrow click.
     *
     * @param direction The direction corresponding to the arrow key that was
     *                  pressed
     * @return True if we consumed the event, false otherwise
     */
    CARAPI ArrowScroll(
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* consumed);

    /**
     * Like {@link View#scrollBy}, but scroll smoothly instead of immediately.
     *
     * @param dx the number of pixels to scroll by on the X axis
     * @param dy the number of pixels to scroll by on the Y axis
     */
    CARAPI SmoothScrollBy(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    /**
     * Like {@link #scrollTo}, but scroll smoothly instead of immediately.
     *
     * @param x the position where to scroll on the X axis
     * @param y the position where to scroll on the Y axis
     */
    CARAPI SmoothScrollTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * Fling the scroll view
     *
     * @param velocityY The initial velocity in the Y direction. Positive
     *                  numbers mean that the finger/cursor is moving down the screen,
     *                  which means we want to scroll towards the top.
     */
    CARAPI Fling(
        /* [in] */ Int32 velocityY);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CSCROLLVIEW_H__
