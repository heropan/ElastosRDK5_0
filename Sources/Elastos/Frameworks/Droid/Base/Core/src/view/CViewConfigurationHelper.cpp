
#include "view/CViewConfigurationHelper.h"
#include "view/CViewConfiguration.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CViewConfigurationHelper::Get(
    /* [in] */ IContext* context,
    /* [out] */ IViewConfiguration** viewConfig)
{
    VALIDATE_NOT_NULL(viewConfig);
    AutoPtr<CViewConfiguration> temp = CViewConfiguration::Get(context);
    *viewConfig = temp.Get();
    INTERFACE_ADDREF(*viewConfig);

    return NOERROR;
}

ECode CViewConfigurationHelper::GetScrollBarSize(
    /* [out] */ Int32* scrollBarSize)
{
    VALIDATE_NOT_NULL(scrollBarSize);
    *scrollBarSize = CViewConfiguration::GetScrollBarSize();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetScrollBarFadeDuration(
    /* [out] */ Int32* scrollBarFadeDuration)
{
    VALIDATE_NOT_NULL(scrollBarFadeDuration);
    *scrollBarFadeDuration = CViewConfiguration::GetScrollBarFadeDuration();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetScrollDefaultDelay(
    /* [out] */ Int32* scrollDefaultDelay)
{
    VALIDATE_NOT_NULL(scrollDefaultDelay);
    *scrollDefaultDelay = CViewConfiguration::GetScrollDefaultDelay();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetFadingEdgeLength(
    /* [out] */ Int32* fadingEdgeLength)
{
    VALIDATE_NOT_NULL(fadingEdgeLength);
    *fadingEdgeLength = CViewConfiguration::GetFadingEdgeLength();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetPressedStateDuration(
    /* [out] */ Int32* pressedStateDuration)
{
    VALIDATE_NOT_NULL(pressedStateDuration);
    *pressedStateDuration = CViewConfiguration::GetPressedStateDuration();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetLongPressTimeout(
    /* [out] */ Int32* longPressTimeout)
{
    VALIDATE_NOT_NULL(longPressTimeout);
    *longPressTimeout = CViewConfiguration::GetLongPressTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetKeyRepeatTimeout(
    /* [out] */ Int32* keyRepeatTimeout)
{
    VALIDATE_NOT_NULL(keyRepeatTimeout);
    *keyRepeatTimeout = CViewConfiguration::GetKeyRepeatTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetKeyRepeatDelay(
    /* [out] */ Int32* keyRepeatDelay)
{
    VALIDATE_NOT_NULL(keyRepeatDelay);
    *keyRepeatDelay = CViewConfiguration::GetKeyRepeatDelay();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetTapTimeout(
    /* [out] */ Int32* tapTimeout)
{
    VALIDATE_NOT_NULL(tapTimeout);
    *tapTimeout = CViewConfiguration::GetTapTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetJumpTapTimeout(
    /* [out] */ Int32* jumpTapTimeout)
{
    VALIDATE_NOT_NULL(jumpTapTimeout);
    *jumpTapTimeout = CViewConfiguration::GetJumpTapTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetDoubleTapTimeout(
    /* [out] */ Int32* doubleTapTimeout)
{
    VALIDATE_NOT_NULL(doubleTapTimeout);
    *doubleTapTimeout = CViewConfiguration::GetDoubleTapTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetHoverTapTimeout(
    /* [out] */ Int32* hoverTapTimeout)
{
    VALIDATE_NOT_NULL(hoverTapTimeout);
    *hoverTapTimeout = CViewConfiguration::GetHoverTapTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetHoverTapSlop(
    /* [out] */ Int32* hoverTapSlop)
{
    VALIDATE_NOT_NULL(hoverTapSlop);
    *hoverTapSlop = CViewConfiguration::GetHoverTapSlop();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetEdgeSlop(
    /* [out] */ Int32* edgeSlop)
{
    VALIDATE_NOT_NULL(edgeSlop);
    *edgeSlop = CViewConfiguration::GetEdgeSlop();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetTouchSlop(
    /* [out] */ Int32* touchSlop)
{
    VALIDATE_NOT_NULL(touchSlop);
    *touchSlop = CViewConfiguration::GetTouchSlop();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetDoubleTapSlop(
    /* [out] */ Int32* doubleTapSlop)
{
    VALIDATE_NOT_NULL(doubleTapSlop);
    *doubleTapSlop = CViewConfiguration::GetDoubleTapSlop();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetSendRecurringAccessibilityEventsInterval(
    /* [out] */ Int64* sendRecurringAccessibilityEventsInterval)
{
    VALIDATE_NOT_NULL(sendRecurringAccessibilityEventsInterval);
    *sendRecurringAccessibilityEventsInterval =
        CViewConfiguration::GetSendRecurringAccessibilityEventsInterval();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetWindowTouchSlop(
    /* [out] */ Int32* windowTouchSlop)
{
    VALIDATE_NOT_NULL(windowTouchSlop);
    *windowTouchSlop = CViewConfiguration::GetWindowTouchSlop();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetMinimumFlingVelocity(
    /* [out] */ Int32* minimumFlingVelocity)
{
    VALIDATE_NOT_NULL(minimumFlingVelocity);
    *minimumFlingVelocity = CViewConfiguration::GetMinimumFlingVelocity();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetMaximumFlingVelocity(
    /* [out] */ Int32* maximumFlingVelocity)
{
    VALIDATE_NOT_NULL(maximumFlingVelocity);
    *maximumFlingVelocity = CViewConfiguration::GetMaximumFlingVelocity();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetMaximumDrawingCacheSize(
    /* [out] */ Int32* maximumDrawingCacheSize)
{
    VALIDATE_NOT_NULL(maximumDrawingCacheSize);
    *maximumDrawingCacheSize = CViewConfiguration::GetMaximumDrawingCacheSize();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetZoomControlsTimeout(
    /* [out] */ Int64* zoomControlsTimeout)
{
    VALIDATE_NOT_NULL(zoomControlsTimeout);
    *zoomControlsTimeout = CViewConfiguration::GetZoomControlsTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetGlobalActionKeyTimeout(
    /* [out] */ Int64* globalActionKeyTimeout)
{
    VALIDATE_NOT_NULL(globalActionKeyTimeout);
    *globalActionKeyTimeout = CViewConfiguration::GetGlobalActionKeyTimeout();

    return NOERROR;
}

ECode CViewConfigurationHelper::GetScrollFriction(
    /* [out] */ Float* scrollFriction)
{
    VALIDATE_NOT_NULL(scrollFriction);
    *scrollFriction = CViewConfiguration::GetScrollFriction();

    return NOERROR;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
