#ifndef __CGESTUREOVERLAYVIEW_H__
#define __CGESTUREOVERLAYVIEW_H__

#include "_CGestureOverlayView.h"
#include "GestureOverlayView.h"

namespace Elastos {
namespace Droid {
namespace Gesture {
CarClass(CGestureOverlayView), public GestureOverlayView
{
public:
    CARAPI constructor(
        /* [in] */ IContext *context);

    CARAPI constructor(
        /* [in] */ IContext *context,
        /* [in] */ IAttributeSet *attrs);

    CARAPI constructor(
        /* [in] */ IContext *context,
        /* [in] */ IAttributeSet *attrs,
        /* [in] */ Int32 defStyle);

    CARAPI GetCurrentStroke(
        /* [out] */ IObjectContainer **stroke);

    CARAPI GetOrientation(
        /* [out] */ Int32 *orientation);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI SetGestureColor(
        /* [in] */ Int32 color);

    CARAPI SetUncertainGestureColor(
        /* [in] */ Int32 color);

    CARAPI GetUncertainGestureColor(
        /* [out] */ Int32 *color);

    CARAPI GetGestureColor(
        /* [out] */ Int32 *color);

    CARAPI GetGestureStrokeWidth(
        /* [out] */ Float *gestureStrokeWidth);

    CARAPI SetGestureStrokeWidth(
        /* [in] */ Float gestureStrokeWidth);

    CARAPI GetGestureStrokeType(
        /* [out] */ Int32 *gestureStrokeType);

    CARAPI SetGestureStrokeType(
        /* [in] */ Int32 gestureStrokeType);

    CARAPI GetGestureStrokeLengthThreshold(
        /* [out] */ Float *gestureStrokeLengthThreshold);

    CARAPI SetGestureStrokeLengthThreshold(
        /* [in] */ Float gestureStrokeLengthThreshold);

    CARAPI GetGestureStrokeSquarenessTreshold(
        /* [out] */ Float *gestureStrokeSquarenessTreshold);

    CARAPI SetGestureStrokeSquarenessTreshold(
        /* [in] */ Float gestureStrokeSquarenessTreshold);

    CARAPI GetGestureStrokeAngleThreshold(
        /* [out] */ Float *gestureStrokeAngleThreshold);

    CARAPI SetGestureStrokeAngleThreshold(
        /* [in] */ Float gestureStrokeAngleThreshold);

    CARAPI IsEventsInterceptionEnabled(
        /* [out] */ Boolean *isEventsInterceptionEnabled);

    CARAPI SetEventsInterceptionEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsFadeEnabled(
        /* [out] */ Boolean *isFadeEnabled);

    CARAPI SetFadeEnabled(
        /* [in] */ Boolean fadeEnabled);

    CARAPI GetGesture(
        /* [out] */ IGesture **gesture);

    CARAPI SetGesture(
        /* [in] */ IGesture *gesture);

    CARAPI GetGesturePath(
        /* [out] */ IPath **gesturePath);

    CARAPI GetGesturePathEx(
        /* [in] */ IPath *path,
        /* [out] */ IPath **gesturePath);

    CARAPI IsGestureVisible(
        /* [out] */ Boolean *isGestureVisible);

    CARAPI SetGestureVisible(
        /* [in] */ Boolean visible);

    CARAPI GetFadeOffset(
        /* [out] */ Int64 *fadeOffset);

    CARAPI SetFadeOffset(
        /* [in] */ Int64 fadeOffset);

    CARAPI AddOnGestureListener(
        /* [in] */ IOnGestureListener *listener);

    CARAPI RemoveOnGestureListener(
        /* [in] */ IOnGestureListener *listener);

    CARAPI RemoveAllOnGestureListeners();

    CARAPI AddOnGesturePerformedListener(
        /* [in] */ IOnGesturePerformedListener *listener);

    CARAPI RemoveOnGesturePerformedListener(
        /* [in] */ IOnGesturePerformedListener *listener);

    CARAPI RemoveAllOnGesturePerformedListeners();

    CARAPI AddOnGesturingListener(
        /* [in] */ IOnGesturingListener *listener);

    CARAPI RemoveOnGesturingListener(
        /* [in] */ IOnGesturingListener *listener);

    CARAPI RemoveAllOnGesturingListeners();

    CARAPI IsGesturing(
        /* [out] */ Boolean *isGesturing);

    CARAPI GetGesturePaint(
        /* [out] */ IPaint **gesturePaint);

    CARAPI Clear(
        /* [in] */ Boolean animated);

    CARAPI CancelClearAnimation();

    CARAPI CancelGesture();
};
}
}
}

#endif