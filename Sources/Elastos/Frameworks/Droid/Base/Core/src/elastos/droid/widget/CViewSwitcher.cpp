#include "CViewSwitcher.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CViewSwitcher, ViewSwitcher)
IFRAMELAYOUT_METHODS_IMPL(CViewSwitcher, ViewSwitcher)
IVIEWGROUP_METHODS_IMPL(CViewSwitcher, ViewSwitcher)
IVIEWPARENT_METHODS_IMPL(CViewSwitcher, ViewSwitcher)
IVIEWMANAGER_METHODS_IMPL(CViewSwitcher, ViewSwitcher)
IDRAWABLECALLBACK_METHODS_IMPL(CViewSwitcher, ViewSwitcher)
IKEYEVENTCALLBACK_METHODS_IMPL(CViewSwitcher, ViewSwitcher)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CViewSwitcher, ViewSwitcher)

PInterface CViewSwitcher::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CViewSwitcher::Probe(riid);
}

ECode CViewSwitcher::constructor(
        /* [in] */ IContext* context)
{
    return ViewSwitcher::Init(context);
}

ECode CViewSwitcher::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    return ViewSwitcher::Init(context, attrs);
}

    /**
     * Sets which child view will be displayed.
     *
     * @param whichChild the index of the child view to display
     */
ECode CViewSwitcher::SetDisplayedChild(
        /* [in] */ Int32 whichChild)
{
    return ViewSwitcher::SetDisplayedChild(whichChild);
}

    /**
     * Returns the index of the currently displayed child view.
     */
ECode CViewSwitcher::GetDisplayedChild(
        /* [out] */ Int32* child)
{
    return ViewSwitcher::GetDisplayedChild(child);
}

    /**
     * Manually shows the next child.
     */
ECode CViewSwitcher::ShowNext()
{
    return ViewSwitcher::ShowNext();
}

    /**
     * Manually shows the previous child.
     */
ECode CViewSwitcher::ShowPrevious()
{
    return ViewSwitcher::ShowPrevious();
}

    /**
     * Returns the View corresponding to the currently displayed child.
     *
     * @return The View currently displayed.
     *
     * @see #getDisplayedChild()
     */
ECode CViewSwitcher::GetCurrentView(
        /* [out] */ IView** view)
{
    return ViewSwitcher::GetCurrentView(view);
}

    /**
     * Returns the current animation used to animate a View that enters the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setInAnimation(android.view.animation.Animation)
     * @see #setInAnimation(android.content.Context, Int32)
     */
ECode CViewSwitcher::GetInAnimation(
        /* [out] */ IAnimation** animation)
{
    return ViewSwitcher::GetInAnimation(animation);
}

    /**
     * Specifies the animation used to animate a View that enters the screen.
     *
     * @param inAnimation The animation started when a View enters the screen.
     *
     * @see #getInAnimation()
     * @see #setInAnimation(android.content.Context, Int32)
     */
ECode CViewSwitcher::SetInAnimation(
        /* [in] */ IAnimation* inAnimation)
{
    return ViewSwitcher::SetInAnimation(inAnimation);
}

    /**
     * Returns the current animation used to animate a View that exits the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setOutAnimation(android.view.animation.Animation)
     * @see #setOutAnimation(android.content.Context, Int32)
     */
ECode CViewSwitcher::GetOutAnimation(
        /* [out] */ IAnimation** animation)
{
    return ViewSwitcher::GetOutAnimation(animation);
}

    /**
     * Specifies the animation used to animate a View that exit the screen.
     *
     * @param outAnimation The animation started when a View exit the screen.
     *
     * @see #getOutAnimation()
     * @see #setOutAnimation(android.content.Context, Int32)
     */
ECode CViewSwitcher::SetOutAnimation(
        /* [in] */ IAnimation* outAnimation)
{
    return ViewSwitcher::SetOutAnimation(outAnimation);
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
ECode CViewSwitcher::SetInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID)
{
    return ViewSwitcher::SetInAnimation(context, resourceID);
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
ECode CViewSwitcher::SetOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID)
{
    return ViewSwitcher::SetOutAnimation(context, resourceID);
}

    /**
     * Returns whether the current View should be animated the first time the ViewSwitcher
     * is displayed.
     *
     * @return TRUE if the current View will be animated the first time it is displayed,
     * false otherwise.
     *
     * @see #setAnimateFirstView(Boolean)
     */
ECode CViewSwitcher::GetAnimateFirstView(
        /* [out] */ Boolean* animate)
{
    return ViewSwitcher::GetAnimateFirstView(animate);
}

    /**
     * Indicates whether the current View should be animated the first time
     * the ViewSwitcher is displayed.
     *
     * @param animate True to animate the current View the first time it is displayed,
     *                false otherwise.
     */
ECode CViewSwitcher::SetAnimateFirstView(
        /* [in] */ Boolean animate)
{
    return ViewSwitcher::SetAnimateFirstView(animate);
}

/**
 * Returns the next view to be displayed.
 *
 * @return the view that will be displayed after the next views flip.
 */
ECode CViewSwitcher::GetNextView(
    /* [out] */ IView** view)
{
    return ViewSwitcher::GetNextView(view);
}

/**
 * Sets the factory used to create the two views between which the
 * ViewSwitcher will flip. Instead of using a factory, you can call
 * {@link #addView(android.view.View, int, android.view.ViewGroup.LayoutParams)}
 * twice.
 *
 * @param factory the view factory used to generate the switcher's content
 */
ECode CViewSwitcher::SetFactory(
    /* [in] */ IViewFactory* factory)
{
    return ViewSwitcher::SetFactory(factory);
}

/**
 * Reset the ViewSwitcher to hide all of the existing views and to make it
 * think that the first time animation has not yet played.
 */
ECode CViewSwitcher::Reset()
{
    return ViewSwitcher::Reset();
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
