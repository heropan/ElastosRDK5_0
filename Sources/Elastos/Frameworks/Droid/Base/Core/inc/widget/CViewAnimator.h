#ifndef __ELASTOS_DROID_WIDGET_CVIEWANIMATOR_H__
#define __ELASTOS_DROID_WIDGET_CVIEWANIMATOR_H__

#include "_CViewAnimator.h"
#include "ViewAnimator.h"



namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CViewAnimator) , public ViewAnimator
{
public:
    IVIEW_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Sets which child view will be displayed.
     *
     * @param whichChild the index of the child view to display
     */
    CARAPI SetDisplayedChild(
        /* [in] */ Int32 whichChild);

    /**
     * Returns the index of the currently displayed child view.
     */
    CARAPI GetDisplayedChild(
        /* [out] */ Int32* child);

    /**
     * Manually shows the next child.
     */
    CARAPI ShowNext();

    /**
     * Manually shows the previous child.
     */
    CARAPI ShowPrevious();

    /**
     * Returns the View corresponding to the currently displayed child.
     *
     * @return The View currently displayed.
     *
     * @see #getDisplayedChild()
     */
    CARAPI GetCurrentView(
        /* [out] */ IView** view);

    /**
     * Returns the current animation used to animate a View that enters the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setInAnimation(android.view.animation.Animation)
     * @see #setInAnimation(android.content.Context, Int32)
     */
    CARAPI GetInAnimation(
        /* [out] */ IAnimation** animation);

    /**
     * Specifies the animation used to animate a View that enters the screen.
     *
     * @param inAnimation The animation started when a View enters the screen.
     *
     * @see #getInAnimation()
     * @see #setInAnimation(android.content.Context, Int32)
     */
    CARAPI SetInAnimation(
        /* [in] */ IAnimation* inAnimation);

    /**
     * Returns the current animation used to animate a View that exits the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setOutAnimation(android.view.animation.Animation)
     * @see #setOutAnimation(android.content.Context, Int32)
     */
    CARAPI GetOutAnimation(
        /* [out] */ IAnimation** animation);

    /**
     * Specifies the animation used to animate a View that exit the screen.
     *
     * @param outAnimation The animation started when a View exit the screen.
     *
     * @see #getOutAnimation()
     * @see #setOutAnimation(android.content.Context, Int32)
     */
    CARAPI SetOutAnimation(
        /* [in] */ IAnimation* outAnimation);

    /**
     * Specifies the animation used to animate a View that enters the screen.
     *
     * @param context The application's environment.
     * @param resourceID The resource id of the animation.
     *
     * @see #getInAnimation()
     * @see #setInAnimation(android.view.animation.Animation)
     */
    CARAPI SetInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    /**
     * Specifies the animation used to animate a View that exit the screen.
     *
     * @param context The application's environment.
     * @param resourceID The resource id of the animation.
     *
     * @see #getOutAnimation()
     * @see #setOutAnimation(android.view.animation.Animation)
     */
    CARAPI SetOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    /**
     * Returns whether the current View should be animated the first time the ViewAnimator
     * is displayed.
     *
     * @return TRUE if the current View will be animated the first time it is displayed,
     * false otherwise.
     *
     * @see #setAnimateFirstView(Boolean)
     */
    CARAPI GetAnimateFirstView(
        /* [out] */ Boolean* animate);

    /**
     * Indicates whether the current View should be animated the first time
     * the ViewAnimator is displayed.
     *
     * @param animate True to animate the current View the first time it is displayed,
     *                false otherwise.
     */
    CARAPI SetAnimateFirstView(
        /* [in] */ Boolean animate);
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget
#endif
