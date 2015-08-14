
#ifndef __ELASTOS_DROID_WIDGET_CVIEWFLIPPER_H__
#define __ELASTOS_DROID_WIDGET_CVIEWFLIPPER_H__

#include "_CViewFlipper.h"


#include "widget/ViewFlipper.h"

using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * Simple {@link ViewAnimator} that will animate between two or more views
 * that have been added to it.  Only one child is shown at a time.  If
 * requested, can automatically flip between each child at a regular interval.
 *
 * @attr ref android.R.styleable#ViewFlipper_flipInterval
 * @attr ref android.R.styleable#ViewFlipper_autoStart
 */
CarClass(CViewFlipper), public ViewFlipper
{
public:
    CViewFlipper();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    /**
     * How long to wait before flipping to the next view
     *
     * @param milliseconds
     *            time in milliseconds
     */
    CARAPI SetFlipInterval(
        /* [in] */ Int32 milliseconds);

    /**
     * Start a timer to cycle through child views
     */
    CARAPI StartFlipping();

    /**
     * No more flips
     */
    CARAPI StopFlipping();

    /**
     * Returns TRUE if the child views are flipping.
     */
    CARAPI IsFlipping(
        /* [out] */ Boolean* flipping);

    /**
     * Set if this view automatically calls {@link #startFlipping()} when it
     * becomes attached to a window.
     */
    CARAPI SetAutoStart(
        /* [in] */ Boolean autoStart);

    /**
     * Returns TRUE if this view automatically calls {@link #startFlipping()}
     * when it becomes attached to a window.
     */
    CARAPI IsAutoStart(
        /* [out] */ Boolean* start);

    CARAPI GetForegroundGravity(
        /* [out] */ Int32* foregroundGravity);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI GetMeasureAllChildren(
        /* [out] */ Boolean* measureAll);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);

    CARAPI SetDisplayedChild(
        /* [in] */ Int32 whichChild);

    CARAPI GetDisplayedChild(
        /* [out] */ Int32* child);

    CARAPI ShowNext();

    CARAPI ShowPrevious();

    CARAPI GetCurrentView(
        /* [out] */ IView** view);

    CARAPI GetInAnimation(
        /* [out] */ IAnimation** animation);

    CARAPI SetInAnimation(
        /* [in] */ IAnimation* inAnimation);

    CARAPI GetOutAnimation(
        /* [out] */ IAnimation** animation);

    CARAPI SetOutAnimation(
        /* [in] */ IAnimation* outAnimation);

    CARAPI SetInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI SetOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI GetAnimateFirstView(
        /* [out] */ Boolean* animate);

    CARAPI SetAnimateFirstView(
        /* [in] */ Boolean animate);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WIDGET_CVIEWFLIPPER_H__
