#ifndef __CGALLERY_H_
#define __CGALLERY_H_

#include "_CGallery.h"


#include "widget/Gallery.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CGallery), public Gallery
{
public:

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IADAPTERVIEW_METHODS_DECL()

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

public:
    CARAPI SetCallbackDuringFling(
        /* [in] */ Boolean shouldCallback);

    CARAPI SetCallbackOnUnselectedItemClick(
        /* [in] */ Boolean shouldCallback);

    CARAPI SetAnimationDuration(
        /* [in] */ Int32 animationDurationMillis);

    CARAPI SetSpacing(
        /* [in] */ Int32 spacing);

    CARAPI SetUnselectedAlpha(
        /* [in] */ Float unselectedAlpha);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetSelection(
        /* [in] */ Int32 position,
        /* [in] */ Boolean animate);

    CARAPI PointToPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Int32* pos);

    // Interface IOnGestureListener

    /**
     * Notified when a tap occurs with the down {@link MotionEvent}
     * that triggered it. This will be triggered immediately for
     * every down event. All other events should be preceded by this.
     *
     * @param e The down motion event.
     */
    CARAPI OnDown(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* res);

    /**
     * The user has performed a down {@link MotionEvent} and not performed
     * a move or up yet. This event is commonly used to provide visual
     * feedback to the user to let them know that their action has been
     * recognized i.e. highlight an element.
     *
     * @param e The down motion event
     */
    CARAPI OnShowPress(
        /* [in] */ IMotionEvent* e);

    /**
     * Notified when a tap occurs with the up {@link MotionEvent}
     * that triggered it.
     *
     * @param e The up motion event that completed the first tap
     * @return true if the event is consumed, else false
     */
    CARAPI OnSingleTapUp(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* res);

    /**
     * Notified when a scroll occurs with the initial on down {@link MotionEvent} and the
     * current move {@link MotionEvent}. The distance in x and y is also supplied for
     * convenience.
     *
     * @param e1 The first down motion event that started the scrolling.
     * @param e2 The move motion event that triggered the current onScroll.
     * @param distanceX The distance along the X axis that has been scrolled since the last
     *              call to onScroll. This is NOT the distance between {@code e1}
     *              and {@code e2}.
     * @param distanceY The distance along the Y axis that has been scrolled since the last
     *              call to onScroll. This is NOT the distance between {@code e1}
     *              and {@code e2}.
     * @return true if the event is consumed, else false
     */
    CARAPI OnScroll(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float distanceX,
        /* [in] */ Float distanceY,
        /* [out] */ Boolean* res);

    /**
     * Notified when a long press occurs with the initial on down {@link MotionEvent}
     * that trigged it.
     *
     * @param e The initial on down motion event that started the longpress.
     */
    CARAPI OnLongPress(
        /* [in] */ IMotionEvent* e);

    /**
     * Notified of a fling event when it occurs with the initial on down {@link MotionEvent}
     * and the matching up {@link MotionEvent}. The calculated velocity is supplied along
     * the x and y axis in pixels per second.
     *
     * @param e1 The first down motion event that started the fling.
     * @param e2 The move motion event that triggered the current onFling.
     * @param velocityX The velocity of this fling measured in pixels per second
     *              along the x axis.
     * @param velocityY The velocity of this fling measured in pixels per second
     *              along the y axis.
     * @return true if the event is consumed, else false
     */
    CARAPI OnFling(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float velocityX,
        /* [in] */ Float velocityY,
        /* [out] */ Boolean* res);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CGALLERY_H_
