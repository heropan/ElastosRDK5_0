
#include "elastos/droid/widget/FastScroller.h"
#include <elastos/core/Math.h>
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/graphics/CRectF.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/view/CMotionEvent.h"
#include "elastos/droid/view/CViewConfiguration.h"

using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Content::Res::IColorStateList;
using Elastos::Droid::Graphics::Drawable::INinePatchDrawable;
using Elastos::Droid::View::CMotionEvent;
using Elastos::Droid::View::CViewConfiguration;

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//              FastScroller::MyRunnable
//==============================================================================

FastScroller::MyRunnable::MyRunnable(
    /* [in] */ FastScroller* host)
    : mHost(host)
{}

ECode FastScroller::MyRunnable::Run()
{
    return NOERROR;
}

//==============================================================================
//              FastScroller::MyRunnable
//==============================================================================

FastScroller::MyAnimatorListenerAdapter::MyAnimatorListenerAdapter(
    /* [in] */ FastScroller* host)
    : mHost(host)
{}

ECode FastScroller::MyAnimatorListenerAdapter::OnAnimationEnd()
{
    return NOERROR;
}

//==============================================================================
//              FastScroller::Leftroperty
//==============================================================================

FastScroller::Leftroperty::Leftroperty(
    /* [in] */ const String& name)
    : Int32Property(name)
{}

ECode FastScroller::Leftroperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    return NOERROR;
}

ECode FastScroller::Leftroperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 value)
{
    return NOERROR;
}

//==============================================================================
//              FastScroller::RightProperty
//==============================================================================

FastScroller::RightProperty::RightProperty(
    /* [in] */ const String& name)
    : Int32Property(name)
{}

ECode FastScroller::RightProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    return NOERROR;
}

ECode FastScroller::RightProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 value)
{
    return NOERROR;
}
//==============================================================================
//              FastScroller::TopProperty
//==============================================================================

FastScroller::TopProperty::TopProperty(
    /* [in] */ const String& name)
    : Int32Property(name)
{}

ECode FastScroller::TopProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    return NOERROR;
}

ECode FastScroller::TopProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 value)
{
    return NOERROR;
}
//==============================================================================
//              FastScroller::ScrollFade
//==============================================================================

FastScroller::BottomProperty::BottomProperty(
    /* [in] */ const String& name)
    : Int32Property(name)
{}

ECode FastScroller::BottomProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    return NOERROR;
}

ECode FastScroller::BottomProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 value)
{
    return NOERROR;
}



//==============================================================================
//              FastScroller
//==============================================================================

const Int64 FastScroller::TAP_TIMEOUT = 0;
AutoPtr<IProperty> FastScroller::LEFT;
AutoPtr<IProperty> FastScroller::TOP;
AutoPtr<IProperty> FastScroller::RIGHT;
AutoPtr<IProperty> FastScroller::BOTTOM;

FastScroller::FastScroller(
    /* [in] */ IAbsListView* listView,
    /* [in] */ Int32 styleResId)
{}

void FastScroller::SetStyle(
    /* [in] */ Int32 resId)
{}

/**
 * Removes this FastScroller overlay from the host view.
 */
void FastScroller::Remove()
{}

/**
 * @param enabled Whether the fast scroll thumb is enabled.
 */
void FastScroller::SetEnabled(
    /* [in] */ Boolean enabled)
{}

/**
 * @return Whether the fast scroll thumb is enabled.
 */
Boolean FastScroller::IsEnabled()
{
    return FALSE;
}

/**
 * @param alwaysShow Whether the fast scroll thumb should always be shown
 */
void FastScroller::SetAlwaysShow(
    /* [in] */ Boolean alwaysShow)
{}

/**
 * @return Whether the fast scroll thumb will always be shown
 * @see #setAlwaysShow(boolean)
 */
Boolean FastScroller::IsAlwaysShowEnabled()
{
    return FALSE;
}

void FastScroller::SetScrollBarStyle(
    /* [in] */ Int32 style)
{}

/**
 * Immediately transitions the fast scroller decorations to a hidden state.
 */
void FastScroller::Stop()
{}

void FastScroller::SetScrollbarPosition(
    /* [in] */ Int32 position)
{}

Int32 FastScroller::GetWidth()
{
    return 0;
}

void FastScroller::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{}

void FastScroller::OnItemCountChanged(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 itemCount)
{}

/**
 * Measures and layouts the scrollbar and decorations.
 */
void FastScroller::UpdateLayout()
{}

void FastScroller::OnScroll(
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{}

void FastScroller::OnSectionsChanged()
{}

Boolean FastScroller::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    return FALSE;
}

Boolean FastScroller::OnInterceptHoverEvent(
    /* [in] */ IMotionEvent* ev)
{
    return FALSE;
}

Boolean FastScroller::OnTouchEvent(
    /* [in] */ IMotionEvent* me)
{
    return FALSE;
}

void FastScroller::UpdateAppearance()
{}

/**
 * Called when one of the variables affecting enabled state changes.
 *
 * @param peekIfEnabled whether the thumb should peek, if enabled
 */
void FastScroller::OnStateDependencyChanged(
    /* [in] */ Boolean peekIfEnabled)
{}

void FastScroller::UpdateLongList(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 itemCount)
{}

/**
 * Creates a view into which preview text can be placed.
 */
AutoPtr<ITextView> FastScroller::CreatePreviewTextView(
    /* [in] */ IContext* context)
{
    return NULL;
}

/**
 * Layouts a view within the specified bounds and pins the pivot point to
 * the appropriate edge.
 *
 * @param view The view to layout.
 * @param bounds Bounds at which to layout the view.
 */
void FastScroller::ApplyLayout(
    /* [in] */ IView* view,
    /* [in] */ IRect* bounds)
{}

/**
 * Measures the preview text bounds, taking preview image padding into
 * account. This method should only be called after {@link #layoutThumb()}
 * and {@link #layoutTrack()} have both been called at least once.
 *
 * @param v The preview text view to measure.
 * @param out Rectangle into which measured bounds are placed.
 */
void FastScroller::MeasurePreview(
    /* [in] */ IView* v,
    /* [in] */ IRect* out)
{}

/**
 * Measures the bounds for a view that should be laid out against the edge
 * of an adjacent view. If no adjacent view is provided, lays out against
 * the list edge.
 *
 * @param view The view to measure for layout.
 * @param adjacent (Optional) The adjacent view, may be null to align to the
 *            list edge.
 * @param margins Layout margins to apply to the view.
 * @param out Rectangle into which measured bounds are placed.
 */
void FastScroller::MeasureViewToSide(
    /* [in] */ IView* view,
    /* [in] */ IView* adjacent,
    /* [in] */ IRect* margins,
    /* [in] */ IRect* out)
{}

void FastScroller::MeasureFloating(
    /* [in] */ IView* preview,
    /* [in] */ IRect* margins,
    /* [in] */ IRect* out)
{}

/**
 * Updates the container rectangle used for layout.
 */
void FastScroller::UpdateContainerRect()
{}

/**
 * Lays out the thumb according to the current scrollbar position.
 */
void FastScroller::LayoutThumb()
{}

/**
 * Lays out the track centered on the thumb. Must be called after
 * {@link #layoutThumb}.
 */
void FastScroller::LayoutTrack()
{}

void FastScroller::SetState(
    /* [in] */ Int32 state)
{}

void FastScroller::RefreshDrawablePressedState()
{}

/**
 * Shows nothing.
 */
void FastScroller::TransitionToHidden()
{}

/**
 * Shows the thumb and track.
 */
void FastScroller::TransitionToVisible()
{}

/**
 * Shows the thumb, preview, and track.
 */
void FastScroller::TransitionToDragging()
{}

void FastScroller::PostAutoHide()
{}

void FastScroller::GetSectionsFromIndexer()
{}

/**
 * Scrolls to a specific position within the section
 * @param position
 */
void FastScroller::ScrollTo(
    /* [in] */ Float position)
{}


/**
 * Transitions the preview text to a new section. Handles animation,
 * measurement, and layout. If the new preview text is empty, returns false.
 *
 * @param sectionIndex The section index to which the preview should
 *            transition.
 * @return False if the new preview text is empty.
 */
Boolean FastScroller::TransitionPreviewLayout(
    /* [in] */ Int32 sectionIndex)
{
    return FALSE;
}

/**
 * Positions the thumb and preview widgets.
 *
 * @param position The position, between 0 and 1, along the track at which
 *            to place the thumb.
 */
void FastScroller::SetThumbPos(
    /* [in] */ Float position)
{}

Float FastScroller::GetPosFromMotionEvent(
    /* [in] */ Float y)
{
    return 0.f;
}

Float FastScroller::GetPosFromItemCount(
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    return 0.f;
}

/**
 * Cancels an ongoing fling event by injecting a
 * {@link MotionEvent#ACTION_CANCEL} into the host view.
 */
void FastScroller::CancelFling()
{}

/**
 * Cancels a pending drag.
 *
 * @see #startPendingDrag()
 */
void FastScroller::CancelPendingDrag()
{}

/**
 * Delays dragging until after the framework has determined that the user is
 * scrolling, rather than tapping.
 */
void FastScroller::StartPendingDrag()
{}

void FastScroller::BeginDrag()
{}

/**
 * Returns whether a coordinate is inside the scroller's activation area. If
 * there is a track image, touching anywhere within the thumb-width of the
 * track activates scrolling. Otherwise, the user has to touch inside thumb
 * itself.
 *
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return Whether the coordinate is inside the scroller's activation area.
 */
Boolean FastScroller::IsPointInside(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return FALSE;
}

Boolean FastScroller::IsPointInsideX(
    /* [in] */ Float x)
{
    return FALSE;
}

Boolean FastScroller::IsPointInsideY(
    /* [in] */ Float y)
{
    return FALSE;
}

AutoPtr<IAnimator> GroupAnimatorOfFloat(
    /* [in] */ IProperty* property,
    /* [in] */ Float value,
    /* [in] */ ArrayOf<IView*>* views)
{
    return NULL;
}

/**
 * Returns an animator for the view's scaleX value.
 */
AutoPtr<IAnimator> AnimateScaleX(
    /* [in] */ IView* v,
    /* [in] */ Float target)
{
    return NULL;
}

/**
 * Returns an animator for the view's alpha value.
 */
AutoPtr<IAnimator> AnimateAlpha(
    /* [in] */ IView* v,
    /* [in] */ Float alpha)
{
    return NULL;
}

AutoPtr<IAnimator> AnimateBounds(
    /* [in] */ IView* v,
    /* [in] */ IRect* bounds)
{
    return NULL;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
