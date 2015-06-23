#include "CGestureOverlayView.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

ECode CGestureOverlayView::constructor(
    /* [in] */ IContext *context)
{
    return GestureOverlayView::Init(context);
}

ECode CGestureOverlayView::constructor(
    /* [in] */ IContext *context,
    /* [in] */ IAttributeSet *attrs)
{
    return GestureOverlayView::Init(context, attrs);
}

ECode CGestureOverlayView::constructor(
    /* [in] */ IContext *context,
    /* [in] */ IAttributeSet *attrs,
    /* [in] */ Int32 defStyle)
{
    return GestureOverlayView::Init(context, attrs, defStyle);
}

ECode CGestureOverlayView::GetCurrentStroke(
    /* [out] */ IObjectContainer **stroke)
{
    return GestureOverlayView::GetCurrentStroke(stroke);
}

ECode CGestureOverlayView::GetOrientation(
    /* [out] */ Int32 *orientation)
{
    return GestureOverlayView::GetOrientation(orientation);
}

ECode CGestureOverlayView::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return GestureOverlayView::SetOrientation(orientation);
}

ECode CGestureOverlayView::SetGestureColor(
    /* [in] */ Int32 color)
{
    return GestureOverlayView::SetGestureColor(color);
}

ECode CGestureOverlayView::SetUncertainGestureColor(
    /* [in] */ Int32 color)
{
    return GestureOverlayView::SetUncertainGestureColor(color);
}

ECode CGestureOverlayView::GetUncertainGestureColor(
    /* [out] */ Int32 *color)
{
    return GestureOverlayView::GetUncertainGestureColor(color);
}

ECode CGestureOverlayView::GetGestureColor(
    /* [out] */ Int32 *color)
{
    return GestureOverlayView::GetGestureColor(color);
}

ECode CGestureOverlayView::GetGestureStrokeWidth(
    /* [out] */ Float *gestureStrokeWidth)
{
    return GestureOverlayView::GetGestureStrokeWidth(gestureStrokeWidth);
}

ECode CGestureOverlayView::SetGestureStrokeWidth(
    /* [in] */ Float gestureStrokeWidth)
{
    return GestureOverlayView::SetGestureStrokeWidth(gestureStrokeWidth);
}

ECode CGestureOverlayView::GetGestureStrokeType(
    /* [out] */ Int32 *gestureStrokeType)
{
    return GestureOverlayView::GetGestureStrokeType(gestureStrokeType);
}

ECode CGestureOverlayView::SetGestureStrokeType(
    /* [in] */ Int32 gestureStrokeType)
{
    return GestureOverlayView::SetGestureStrokeType(gestureStrokeType);
}

ECode CGestureOverlayView::GetGestureStrokeLengthThreshold(
    /* [out] */ Float *gestureStrokeLengthThreshold)
{
    return GestureOverlayView::GetGestureStrokeLengthThreshold(
        gestureStrokeLengthThreshold);
}

ECode CGestureOverlayView::SetGestureStrokeLengthThreshold(
    /* [in] */ Float gestureStrokeLengthThreshold)
{
    return GestureOverlayView::SetGestureStrokeLengthThreshold(
        gestureStrokeLengthThreshold);
}

ECode CGestureOverlayView::GetGestureStrokeSquarenessTreshold(
    /* [out] */ Float *gestureStrokeSquarenessTreshold)
{
    return GestureOverlayView::GetGestureStrokeSquarenessTreshold(
        gestureStrokeLengthThreshold);
}

ECode CGestureOverlayView::SetGestureStrokeSquarenessTreshold(
    /* [in] */ Float gestureStrokeSquarenessTreshold)
{
    return GestureOverlayView::SetGestureStrokeSquarenessTreshold(
        gestureStrokeLengthThreshold);
}

ECode CGestureOverlayView::GetGestureStrokeAngleThreshold(
    /* [out] */ Float *gestureStrokeAngleThreshold)
{
    return GestureOverlayView::gestureStrokeAngleThreshold(
        gestureStrokeAngleThreshold);
}

ECode CGestureOverlayView::SetGestureStrokeAngleThreshold(
    /* [in] */ Float gestureStrokeAngleThreshold)
{
    return GestureOverlayView::SetGestureStrokeAngleThreshold(
        gestureStrokeAngleThreshold);
}

ECode CGestureOverlayView::IsEventsInterceptionEnabled(
    /* [out] */ Boolean *isEventsInterceptionEnabled)
{
    return GestureOverlayView::IsEventsInterceptionEnabled(
        isEventsInterceptionEnabled);
}

ECode CGestureOverlayView::SetEventsInterceptionEnabled(
    /* [in] */ Boolean enabled)
{
    return GestureOverlayView::SetEventsInterceptionEnabled(
        enabled);
}

ECode CGestureOverlayView::IsFadeEnabled(
    /* [out] */ Boolean *isFadeEnabled)
{
    return GestureOverlayView::IsFadeEnabled(isFadeEnabled);
}

ECode CGestureOverlayView::SetFadeEnabled(
    /* [in] */ Boolean fadeEnabled)
{
    return GestureOverlayView::SetFadeEnabled(fadeEnabled);
}

ECode CGestureOverlayView::GetGesture(
    /* [out] */ IGesture **gesture)
{
    return GestureOverlayView::GetGesture(gesture);
}

ECode CGestureOverlayView::SetGesture(
    /* [in] */ IGesture *gesture)
{
    return GestureOverlayView::SetGesture(gesture);
}

ECode CGestureOverlayView::GetGesturePath(
    /* [out] */ IPath **gesturePath)
{
    return GestureOverlayView::GetGesturePath(gesturePath);
}

ECode CGestureOverlayView::GetGesturePath(
    /* [in] */ IPath *path,
    /* [out] */ IPath **gesturePath)
{
    return GestureOverlayView::GetGesturePath(path, gesturePath);
}

ECode CGestureOverlayView::IsGestureVisible(
    /* [out] */ Boolean *isGestureVisible)
{
    return GestureOverlayView::IsGestureVisible(isGestureVisible);
}

ECode CGestureOverlayView::SetGestureVisible(
    /* [in] */ Boolean visible)
{
    return GestureOverlayView::SetGestureVisible(visible);
}

ECode CGestureOverlayView::GetFadeOffset(
    /* [out] */ Int64 *fadeOffset)
{
    return GestureOverlayView::GetFadeOffset(fadeOffset);
}

ECode CGestureOverlayView::SetFadeOffset(
    /* [in] */ Int64 fadeOffset)
{
    return GestureOverlayView::SetFadeOffset(fadeOffset);
}

ECode CGestureOverlayView::AddOnGestureListener(
    /* [in] */ IOnGestureListener *listener)
{
    return GestureOverlayView::AddOnGestureListener(listener);
}

ECode CGestureOverlayView::RemoveOnGestureListener(
    /* [in] */ IOnGestureListener *listener)
{
    return GestureOverlayView::RemoveOnGestureListener(listener);
}

ECode CGestureOverlayView::RemoveAllOnGestureListeners()
{
    return GestureOverlayView::RemoveAllOnGestureListeners();
}

ECode CGestureOverlayView::AddOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener *listener)
{
    return GestureOverlayView::AddOnGesturePerformedListener(listener);
}

ECode CGestureOverlayView::RemoveOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener *listener)
{
    return GestureOverlayView::RemoveOnGesturePerformedListener(
        listener);
}

ECode CGestureOverlayView::RemoveAllOnGesturePerformedListeners()
{
    return GestureOverlayView::RemoveOnGesturePerformedListeners();
}

ECode CGestureOverlayView::AddOnGesturingListener(
    /* [in] */ IOnGesturingListener *listener)
{
    return GestureOverlayView::AddOnGesturingListener(listener);
}

ECode CGestureOverlayView::RemoveOnGesturingListener(
    /* [in] */ IOnGesturingListener *listener)
{
    return GestureOverlayView::RemoveOnGesturingListener(listener);
}

ECode CGestureOverlayView::RemoveAllOnGesturingListeners()
{
    return GestureOverlayView::RemoveOnGesturingListeners();
}

ECode CGestureOverlayView::IsGesturing(
    /* [out] */ Boolean *isGesturing)
{
    return GestureOverlayView::IsGesturing(isGesturing);
}

ECode CGestureOverlayView::GetGesturePaint(
    /* [out] */ IPaint **gesturePaint)
{
    return GestureOverlayView::GetGesturePaint(gesturePaint);
}

ECode CGestureOverlayView::Clear(
    /* [in] */ Boolean animated)
{
    return GestureOverlayView::Clear(animated);
}

ECode CGestureOverlayView::CancelClearAnimation()
{
    return GestureOverlayView::CancelClearAnimation();
}

ECode CGestureOverlayView::CancelGesture()
{
    return GestureOverlayView::CancelGesture();
}

}
}
}