
#include "elastos/droid/view/CViewConfiguration.h"
#include "elastos/droid/view/CWindowManagerGlobal.h"
#include "elastos/droid/utility/CDisplayMetrics.h"
#include "elastos/droid/graphics/CPoint.h"
#include "elastos/droid/R.h"

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Graphics::CPoint;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 CViewConfiguration::SCROLL_BAR_SIZE;
const Int32 CViewConfiguration::SCROLL_BAR_FADE_DURATION;
const Int32 CViewConfiguration::SCROLL_BAR_DEFAULT_DELAY;
const Int32 CViewConfiguration::FADING_EDGE_LENGTH;
const Int32 CViewConfiguration::PRESSED_STATE_DURATION;
const Int32 CViewConfiguration::DEFAULT_LONG_PRESS_TIMEOUT;
const Int32 CViewConfiguration::KEY_REPEAT_DELAY;
const Int32 CViewConfiguration::GLOBAL_ACTIONS_KEY_TIMEOUT;
const Int32 CViewConfiguration::TAP_TIMEOUT;
const Int32 CViewConfiguration::JUMP_TAP_TIMEOUT;
const Int32 CViewConfiguration::DOUBLE_TAP_TIMEOUT;
const Int32 CViewConfiguration::HOVER_TAP_TIMEOUT;
const Int32 CViewConfiguration::HOVER_TAP_SLOP;
const Int32 CViewConfiguration::ZOOM_CONTROLS_TIMEOUT;
const Int32 CViewConfiguration::EDGE_SLOP;
const Int32 CViewConfiguration::TOUCH_SLOP;
const Int32 CViewConfiguration::DOUBLE_TAP_TOUCH_SLOP;
const Int32 CViewConfiguration::PAGING_TOUCH_SLOP;
const Int32 CViewConfiguration::DOUBLE_TAP_SLOP;
const Int32 CViewConfiguration::WINDOW_TOUCH_SLOP;
const Int32 CViewConfiguration::MINIMUM_FLING_VELOCITY;
const Int32 CViewConfiguration::MAXIMUM_FLING_VELOCITY;
const Int64 CViewConfiguration::SEND_RECURRING_ACCESSIBILITY_EVENTS_INTERVAL_MILLIS;
const Int32 CViewConfiguration::MAXIMUM_DRAWING_CACHE_SIZE; // ARGB8888
const Float CViewConfiguration::SCROLL_FRICTION;
const Int32 CViewConfiguration::OVERSCROLL_DISTANCE;
const Int32 CViewConfiguration::OVERFLING_DISTANCE;

HashMap<Int32, AutoPtr<CViewConfiguration> > CViewConfiguration::sConfigurations;

CViewConfiguration::CViewConfiguration() :
    mEdgeSlop(EDGE_SLOP),
    mFadingEdgeLength(FADING_EDGE_LENGTH),
    mMinimumFlingVelocity(MINIMUM_FLING_VELOCITY),
    mMaximumFlingVelocity(MAXIMUM_FLING_VELOCITY),
    mScrollbarSize(SCROLL_BAR_SIZE),
    mTouchSlop(TOUCH_SLOP),
    mDoubleTapTouchSlop(DOUBLE_TAP_TOUCH_SLOP),
    mPagingTouchSlop(PAGING_TOUCH_SLOP),
    mDoubleTapSlop(DOUBLE_TAP_SLOP),
    mWindowTouchSlop(WINDOW_TOUCH_SLOP),
    //noinspection deprecation
    mMaximumDrawingCacheSize(MAXIMUM_DRAWING_CACHE_SIZE),
    mOverscrollDistance(OVERSCROLL_DISTANCE),
    mOverflingDistance(OVERFLING_DISTANCE),
    mFadingMarqueeEnabled(TRUE),
    sHasPermanentMenuKey(FALSE),
    sHasPermanentMenuKeySet(FALSE)
{
}

ECode CViewConfiguration::constructor()
{
    return NOERROR;
}

ECode CViewConfiguration::constructor(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    Float density;
    metrics->GetDensity(&density);
    Float sizeAndDensity = 0.f;
    Boolean result;
    config->IsLayoutSizeAtLeast(IConfiguration::SCREENLAYOUT_SIZE_XLARGE, &result);
    if (result) {
        sizeAndDensity = density * 1.5f;
    }
    else {
        sizeAndDensity = density;
    }

    mEdgeSlop = (Int32)(density * EDGE_SLOP + 0.5f);
    mFadingEdgeLength = (Int32)(density * FADING_EDGE_LENGTH + 0.5f);
    mMinimumFlingVelocity = (Int32)(density * MINIMUM_FLING_VELOCITY + 0.5f);
    mMaximumFlingVelocity = (Int32)(density * MAXIMUM_FLING_VELOCITY + 0.5f);
    mScrollbarSize = (Int32)(density * SCROLL_BAR_SIZE + 0.5f);
    mDoubleTapSlop = (Int32)(density * DOUBLE_TAP_SLOP + 0.5f);
    mWindowTouchSlop = (Int32)(density * WINDOW_TOUCH_SLOP + 0.5f);

    // Size of the screen in bytes, in ARGB_8888 format
    AutoPtr<IInterface> service;
    context->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&service);
    AutoPtr<IWindowManager> win = IWindowManager::Probe(service);
    assert(win != NULL);
    AutoPtr<IDisplay> display;
    win->GetDefaultDisplay((IDisplay**)&display);
    AutoPtr<CPoint> size;
    CPoint::NewByFriend((CPoint**)&size);
    display->GetRealSize(size.Get());
    mMaximumDrawingCacheSize = 4 * size->mX * size->mY;

    mOverscrollDistance = (Int32)(sizeAndDensity * OVERSCROLL_DISTANCE + 0.5f);
    mOverflingDistance = (Int32)(sizeAndDensity * OVERFLING_DISTANCE + 0.5f);

    if (!sHasPermanentMenuKeySet) {
        AutoPtr<IIWindowManager> wm = CWindowManagerGlobal::GetWindowManagerService();
        Boolean hasSystemNavBar, hasNavigationBar;
        if (FAILED(wm->HasSystemNavBar(&hasSystemNavBar))
            || FAILED(wm->HasNavigationBar(&hasNavigationBar))) {
            sHasPermanentMenuKey = FALSE;
        }
        else {
            sHasPermanentMenuKey = !hasSystemNavBar && !hasNavigationBar;
            sHasPermanentMenuKeySet = TRUE;
        }
    }

    res->GetBoolean(
        R::bool_::config_ui_enableFadingMarquee, &mFadingMarqueeEnabled);
    res->GetDimensionPixelSize(
        R::dimen::config_viewConfigurationTouchSlop, &mTouchSlop);
    mPagingTouchSlop = mTouchSlop * 2;

    mDoubleTapTouchSlop = mTouchSlop;

    return NOERROR;
}

AutoPtr<CViewConfiguration> CViewConfiguration::Get(
    /* [in] */ IContext* context)
{
    AutoPtr<IDisplayMetrics> metrics;
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float fdensity;
    metrics->GetDensity(&fdensity);
    const Int32 density = (Int32)(100.0f * fdensity);

    AutoPtr<CViewConfiguration> configuration;
    HashMap<Int32, AutoPtr<CViewConfiguration> >::Iterator find = sConfigurations.Find(density);
    if (find == sConfigurations.End()) {
        CViewConfiguration::NewByFriend(context, (CViewConfiguration**)&configuration);
        sConfigurations[density] = configuration;
    }
    else {
        configuration = find->mSecond;
    }

    return configuration;
}

Int32 CViewConfiguration::GetScrollBarSize()
{
    return SCROLL_BAR_SIZE;
}

ECode CViewConfiguration::GetScaledScrollBarSize(
    /* [out] */ Int32* scrollbarSize)
{
    VALIDATE_NOT_NULL(scrollbarSize);
    *scrollbarSize = mScrollbarSize;

    return NOERROR;
}

Int32 CViewConfiguration::GetScrollBarFadeDuration()
{
    return SCROLL_BAR_FADE_DURATION;
}

Int32 CViewConfiguration::GetScrollDefaultDelay()
{
    return SCROLL_BAR_DEFAULT_DELAY;
}

Int32 CViewConfiguration::GetFadingEdgeLength()
{
    return FADING_EDGE_LENGTH;
}

ECode CViewConfiguration::GetScaledFadingEdgeLength(
    /* [out] */ Int32* fadingEdgeLength)
{
    VALIDATE_NOT_NULL(fadingEdgeLength);
    *fadingEdgeLength = mFadingEdgeLength;

    return NOERROR;
}

Int32 CViewConfiguration::GetPressedStateDuration()
{
    return PRESSED_STATE_DURATION;
}

Int32 CViewConfiguration::GetLongPressTimeout()
{
    return DEFAULT_LONG_PRESS_TIMEOUT;
}

Int32 CViewConfiguration::GetKeyRepeatTimeout()
{
    return GetLongPressTimeout();
}

Int32 CViewConfiguration::GetKeyRepeatDelay()
{
    return KEY_REPEAT_DELAY;
}

Int32 CViewConfiguration::GetTapTimeout()
{
    return TAP_TIMEOUT;
}

Int32 CViewConfiguration::GetJumpTapTimeout()
{
    return JUMP_TAP_TIMEOUT;
}

Int32 CViewConfiguration::GetDoubleTapTimeout()
{
    return DOUBLE_TAP_TIMEOUT;
}

Int32 CViewConfiguration::GetHoverTapTimeout()
{
    return HOVER_TAP_TIMEOUT;
}

Int32 CViewConfiguration::GetHoverTapSlop()
{
    return HOVER_TAP_SLOP;
}

Int32 CViewConfiguration::GetEdgeSlop()
{
    return EDGE_SLOP;
}

ECode CViewConfiguration::GetScaledEdgeSlop(
    /* [out] */ Int32* edgeSlop)
{
    VALIDATE_NOT_NULL(edgeSlop);
    *edgeSlop = mEdgeSlop;
    return NOERROR;
}

Int32 CViewConfiguration::GetTouchSlop()
{
    return TOUCH_SLOP;
}

ECode CViewConfiguration::GetScaledTouchSlop(
    /* [out] */ Int32* touchSlop)
{
    VALIDATE_NOT_NULL(touchSlop);
    *touchSlop = mTouchSlop;
    return NOERROR;
}

ECode CViewConfiguration::GetScaledDoubleTapTouchSlop(
    /* [out] */ Int32* doubleTapTouchSlop)
{
    VALIDATE_NOT_NULL(doubleTapTouchSlop);
    *doubleTapTouchSlop = mDoubleTapTouchSlop;
    return NOERROR;
}

ECode CViewConfiguration::GetScaledPagingTouchSlop(
    /* [out] */ Int32* pagingTouchSlop)
{
    VALIDATE_NOT_NULL(pagingTouchSlop);
    *pagingTouchSlop = mPagingTouchSlop;
    return NOERROR;
}

Int32 CViewConfiguration::GetDoubleTapSlop()
{
    return DOUBLE_TAP_SLOP;
}

ECode CViewConfiguration::GetScaledDoubleTapSlop(
    /* [out] */ Int32* doubleTapSlop)
{
    VALIDATE_NOT_NULL(doubleTapSlop);
    *doubleTapSlop = mDoubleTapSlop;
    return NOERROR;
}

Int64 CViewConfiguration::GetSendRecurringAccessibilityEventsInterval()
{
    return SEND_RECURRING_ACCESSIBILITY_EVENTS_INTERVAL_MILLIS;
}

Int32 CViewConfiguration::GetWindowTouchSlop()
{
    return WINDOW_TOUCH_SLOP;
}

ECode CViewConfiguration::GetScaledWindowTouchSlop(
    /* [out] */ Int32* windowTouchSlop)
{
    VALIDATE_NOT_NULL(windowTouchSlop);
    *windowTouchSlop = mWindowTouchSlop;
    return NOERROR;
}

Int32 CViewConfiguration::GetMinimumFlingVelocity()
{
    return MINIMUM_FLING_VELOCITY;
}

ECode CViewConfiguration::GetScaledMinimumFlingVelocity(
    /* [out] */ Int32* minimumFlingVelocity)
{
    VALIDATE_NOT_NULL(minimumFlingVelocity);
    *minimumFlingVelocity = mMinimumFlingVelocity;
    return NOERROR;
}

Int32 CViewConfiguration::GetMaximumFlingVelocity()
{
    return MAXIMUM_FLING_VELOCITY;
}

ECode CViewConfiguration::GetScaledMaximumFlingVelocity(
    /* [out] */ Int32* maximumFlingVelocity)
{
    VALIDATE_NOT_NULL(maximumFlingVelocity);
    *maximumFlingVelocity = mMaximumFlingVelocity;
    return NOERROR;
}

Int32 CViewConfiguration::GetMaximumDrawingCacheSize()
{
    //noinspection deprecation
    return MAXIMUM_DRAWING_CACHE_SIZE;
}

ECode CViewConfiguration::GetScaledMaximumDrawingCacheSize(
    /* [out] */ Int32* maximumDrawingCacheSize)
{
    VALIDATE_NOT_NULL(maximumDrawingCacheSize);
    *maximumDrawingCacheSize = mMaximumDrawingCacheSize;
    return NOERROR;
}

ECode CViewConfiguration::GetScaledOverscrollDistance(
    /* [out] */ Int32* overscrollDistance)
{
    VALIDATE_NOT_NULL(overscrollDistance);
    *overscrollDistance = mOverscrollDistance;
    return NOERROR;
}

ECode CViewConfiguration::GetScaledOverflingDistance(
    /* [out] */ Int32* overflingDistance)
{
    VALIDATE_NOT_NULL(overflingDistance);
    *overflingDistance = mOverflingDistance;
    return NOERROR;
}

Int64 CViewConfiguration::GetZoomControlsTimeout()
{
    return ZOOM_CONTROLS_TIMEOUT;
}

Int64 CViewConfiguration::GetGlobalActionKeyTimeout()
{
    return GLOBAL_ACTIONS_KEY_TIMEOUT;
}

Float CViewConfiguration::GetScrollFriction()
{
    return SCROLL_FRICTION;
}

ECode CViewConfiguration::HasPermanentMenuKey(
    /* [out] */ Boolean* hasPermanentMenuKey)
{
    VALIDATE_NOT_NULL(hasPermanentMenuKey);
    *hasPermanentMenuKey = sHasPermanentMenuKey;
    return NOERROR;
}

/**
 * @hide
 * @return Whether or not marquee should use fading edges.
 */
ECode CViewConfiguration::IsFadingMarqueeEnabled(
    /* [out] */ Boolean* fadingMarqueeEnabled)
{
    VALIDATE_NOT_NULL(fadingMarqueeEnabled);
    *fadingMarqueeEnabled = mFadingMarqueeEnabled;
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
