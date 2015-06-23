#include "CTextSwitcher.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CTextSwitcher, TextSwitcher)
IFRAMELAYOUT_METHODS_IMPL(CTextSwitcher, TextSwitcher)
IVIEWGROUP_METHODS_IMPL(CTextSwitcher, TextSwitcher)
IVIEWPARENT_METHODS_IMPL(CTextSwitcher, TextSwitcher)
IVIEWMANAGER_METHODS_IMPL(CTextSwitcher, TextSwitcher)
IDRAWABLECALLBACK_METHODS_IMPL(CTextSwitcher, TextSwitcher)
IKEYEVENTCALLBACK_METHODS_IMPL(CTextSwitcher, TextSwitcher)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CTextSwitcher, TextSwitcher)

PInterface CTextSwitcher::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CTextSwitcher::Probe(riid);
}

ECode CTextSwitcher::constructor(
        /* [in] */ IContext* context)
{
    return TextSwitcher::Init(context);
}

ECode CTextSwitcher::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    return TextSwitcher::Init(context, attrs);
}

    /**
     * Sets which child view will be displayed.
     *
     * @param whichChild the index of the child view to display
     */
ECode CTextSwitcher::SetDisplayedChild(
        /* [in] */ Int32 whichChild)
{
    return TextSwitcher::SetDisplayedChild(whichChild);
}

    /**
     * Returns the index of the currently displayed child view.
     */
ECode CTextSwitcher::GetDisplayedChild(
        /* [out] */ Int32* child)
{
    return TextSwitcher::GetDisplayedChild(child);
}

    /**
     * Manually shows the next child.
     */
ECode CTextSwitcher::ShowNext()
{
    return TextSwitcher::ShowNext();
}

    /**
     * Manually shows the previous child.
     */
ECode CTextSwitcher::ShowPrevious()
{
    return TextSwitcher::ShowPrevious();
}

    /**
     * Returns the View corresponding to the currently displayed child.
     *
     * @return The View currently displayed.
     *
     * @see #getDisplayedChild()
     */
ECode CTextSwitcher::GetCurrentView(
        /* [out] */ IView** view)
{
    return TextSwitcher::GetCurrentView(view);
}

    /**
     * Returns the current animation used to animate a View that enters the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setInAnimation(android.view.animation.Animation)
     * @see #setInAnimation(android.content.Context, Int32)
     */
ECode CTextSwitcher::GetInAnimation(
        /* [out] */ IAnimation** animation)
{
    return TextSwitcher::GetInAnimation(animation);
}

    /**
     * Specifies the animation used to animate a View that enters the screen.
     *
     * @param inAnimation The animation started when a View enters the screen.
     *
     * @see #getInAnimation()
     * @see #setInAnimation(android.content.Context, Int32)
     */
ECode CTextSwitcher::SetInAnimation(
        /* [in] */ IAnimation* inAnimation)
{
    return TextSwitcher::SetInAnimation(inAnimation);
}

    /**
     * Returns the current animation used to animate a View that exits the screen.
     *
     * @return An Animation or NULL if none is set.
     *
     * @see #setOutAnimation(android.view.animation.Animation)
     * @see #setOutAnimation(android.content.Context, Int32)
     */
ECode CTextSwitcher::GetOutAnimation(
        /* [out] */ IAnimation** animation)
{
    return TextSwitcher::GetOutAnimation(animation);
}

    /**
     * Specifies the animation used to animate a View that exit the screen.
     *
     * @param outAnimation The animation started when a View exit the screen.
     *
     * @see #getOutAnimation()
     * @see #setOutAnimation(android.content.Context, Int32)
     */
ECode CTextSwitcher::SetOutAnimation(
        /* [in] */ IAnimation* outAnimation)
{
    return TextSwitcher::SetOutAnimation(outAnimation);
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
ECode CTextSwitcher::SetInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID)
{
    return TextSwitcher::SetInAnimation(context, resourceID);
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
ECode CTextSwitcher::SetOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID)
{
    return TextSwitcher::SetOutAnimation(context, resourceID);
}

    /**
     * Returns whether the current View should be animated the first time the TextSwitcher
     * is displayed.
     *
     * @return TRUE if the current View will be animated the first time it is displayed,
     * false otherwise.
     *
     * @see #setAnimateFirstView(Boolean)
     */
ECode CTextSwitcher::GetAnimateFirstView(
        /* [out] */ Boolean* animate)
{
    return TextSwitcher::GetAnimateFirstView(animate);
}

    /**
     * Indicates whether the current View should be animated the first time
     * the TextSwitcher is displayed.
     *
     * @param animate True to animate the current View the first time it is displayed,
     *                false otherwise.
     */
ECode CTextSwitcher::SetAnimateFirstView(
        /* [in] */ Boolean animate)
{
    return TextSwitcher::SetAnimateFirstView(animate);
}

/**
 * Returns the next view to be displayed.
 *
 * @return the view that will be displayed after the next views flip.
 */
ECode CTextSwitcher::GetNextView(
    /* [out] */ IView** view)
{
    return TextSwitcher::GetNextView(view);
}

/**
 * Sets the factory used to create the two views between which the
 * TextSwitcher will flip. Instead of using a factory, you can call
 * {@link #addView(android.view.View, int, android.view.ViewGroup.LayoutParams)}
 * twice.
 *
 * @param factory the view factory used to generate the switcher's content
 */
ECode CTextSwitcher::SetFactory(
    /* [in] */ IViewFactory* factory)
{
    return TextSwitcher::SetFactory(factory);
}

/**
 * Reset the TextSwitcher to hide all of the existing views and to make it
 * think that the first time animation has not yet played.
 */
ECode CTextSwitcher::Reset()
{
    return TextSwitcher::Reset();
}

/**
 * Sets the text of the next view and switches to the next view. This can
 * be used to animate the old text out and animate the next text in.
 *
 * @param text the new text to display
 */
ECode CTextSwitcher::SetText(
    /* [in] */ ICharSequence* text)
{
    return TextSwitcher::SetText(text);
}

/**
 * Sets the text of the text view that is currently showing.  This does
 * not perform the animations.
 *
 * @param text the new text to display
 */
ECode CTextSwitcher::SetCurrentText(
    /* [in] */ ICharSequence* text)
{
    return TextSwitcher::SetCurrentText(text);
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
