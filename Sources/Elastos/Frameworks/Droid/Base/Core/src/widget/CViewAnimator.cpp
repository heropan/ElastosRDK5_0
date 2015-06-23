#include "CViewAnimator.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CViewAnimator, ViewAnimator)
IFRAMELAYOUT_METHODS_IMPL(CViewAnimator, ViewAnimator)
IVIEWGROUP_METHODS_IMPL(CViewAnimator, ViewAnimator)
IVIEWPARENT_METHODS_IMPL(CViewAnimator, ViewAnimator)
IVIEWMANAGER_METHODS_IMPL(CViewAnimator, ViewAnimator)
IDRAWABLECALLBACK_METHODS_IMPL(CViewAnimator, ViewAnimator)
IKEYEVENTCALLBACK_METHODS_IMPL(CViewAnimator, ViewAnimator)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CViewAnimator, ViewAnimator)

PInterface CViewAnimator::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CViewAnimator::Probe(riid);
}

ECode CViewAnimator::constructor(
        /* [in] */ IContext* context)
{
    return ViewAnimator::Init(context);
}

ECode CViewAnimator::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    return ViewAnimator::Init(context, attrs);
}

    /**
     * Sets which child view will be displayed.
     *
     * @param whichChild the index of the child view to display
     */
ECode CViewAnimator::SetDisplayedChild(
        /* [in] */ Int32 whichChild)
{
    return ViewAnimator::SetDisplayedChild(whichChild);
}

    /**
     * Returns the index of the currently displayed child view.
     */
ECode CViewAnimator::GetDisplayedChild(
        /* [out] */ Int32* child)
{
    return ViewAnimator::GetDisplayedChild(child);
}

    /**
     * Manually shows the next child.
     */
ECode CViewAnimator::ShowNext()
{
    return ViewAnimator::ShowNext();
}

    /**
     * Manually shows the previous child.
     */
ECode CViewAnimator::ShowPrevious()
{
    return ViewAnimator::ShowPrevious();
}

    /**
     * Returns the View corresponding to the currently displayed child.
     *
     * @return The View currently displayed.
     *
     * @see #getDisplayedChild()
     */
ECode CViewAnimator::GetCurrentView(
        /* [out] */ IView** view)
{
    return ViewAnimator::GetCurrentView(view);
}

    /**
     * Returns the current animation used to animate a View that enters the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setInAnimation(android.view.animation.Animation)
     * @see #setInAnimation(android.content.Context, Int32)
     */
ECode CViewAnimator::GetInAnimation(
        /* [out] */ IAnimation** animation)
{
    return ViewAnimator::GetInAnimation(animation);
}

    /**
     * Specifies the animation used to animate a View that enters the screen.
     *
     * @param inAnimation The animation started when a View enters the screen.
     *
     * @see #getInAnimation()
     * @see #setInAnimation(android.content.Context, Int32)
     */
ECode CViewAnimator::SetInAnimation(
        /* [in] */ IAnimation* inAnimation)
{
    return ViewAnimator::SetInAnimation(inAnimation);
}

    /**
     * Returns the current animation used to animate a View that exits the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setOutAnimation(android.view.animation.Animation)
     * @see #setOutAnimation(android.content.Context, Int32)
     */
ECode CViewAnimator::GetOutAnimation(
        /* [out] */ IAnimation** animation)
{
    return ViewAnimator::GetOutAnimation(animation);
}

    /**
     * Specifies the animation used to animate a View that exit the screen.
     *
     * @param outAnimation The animation started when a View exit the screen.
     *
     * @see #getOutAnimation()
     * @see #setOutAnimation(android.content.Context, Int32)
     */
ECode CViewAnimator::SetOutAnimation(
        /* [in] */ IAnimation* outAnimation)
{
    return ViewAnimator::SetOutAnimation(outAnimation);
}

    /**
     * Specifies the animation used to animate a View that enters the screen.
     *
     * @param context The application's environment.
     * @param resourceID The resource id of the animation.
     *
     * @see #getInAnimation()
     * @see #setInAnimation(android.view.animation.Animation)
     */
ECode CViewAnimator::SetInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID)
{
    return ViewAnimator::SetInAnimation(context, resourceID);
}

    /**
     * Specifies the animation used to animate a View that exit the screen.
     *
     * @param context The application's environment.
     * @param resourceID The resource id of the animation.
     *
     * @see #getOutAnimation()
     * @see #setOutAnimation(android.view.animation.Animation)
     */
ECode CViewAnimator::SetOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID)
{
    return ViewAnimator::SetOutAnimation(context, resourceID);
}

    /**
     * Returns whether the current View should be animated the first time the ViewAnimator
     * is displayed.
     *
     * @return TRUE if the current View will be animated the first time it is displayed,
     * false otherwise.
     *
     * @see #setAnimateFirstView(Boolean)
     */
ECode CViewAnimator::GetAnimateFirstView(
        /* [out] */ Boolean* animate)
{
    return ViewAnimator::GetAnimateFirstView(animate);
}

    /**
     * Indicates whether the current View should be animated the first time
     * the ViewAnimator is displayed.
     *
     * @param animate True to animate the current View the first time it is displayed,
     *                false otherwise.
     */
ECode CViewAnimator::SetAnimateFirstView(
        /* [in] */ Boolean animate)
{
    return ViewAnimator::SetAnimateFirstView(animate);
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
