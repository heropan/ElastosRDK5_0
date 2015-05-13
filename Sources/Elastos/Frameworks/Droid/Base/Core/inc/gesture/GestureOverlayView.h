#ifndef __GESTUREOVERLAYVIEW_H__
#define __GESTUREOVERLAYVIEW_H__

namespace Elastos {
namespace Droid {
namespace Gesture {

class GestureOverlayView {
public:
    CARAPI Init(
        /* [in] */ IContext *context);

    CARAPI Init(
        /* [in] */ IContext *context,
        /* [in] */ IAttributeSet *attrs);

    CARAPI Init(
        /* [in] */ IContext *context,
        /* [in] */ IAttributeSet *attrs,
        /* [in] */ Int32 defStyle);

    virtual CARAPI GetCurrentStroke(
        /* [out] */ IObjectContainer **stroke);

    virtual CARAPI GetOrientation(
        /* [out] */ Int32 *orientation);

    virtual CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    virtual CARAPI SetGestureColor(
        /* [in] */ Int32 color);

    virtual CARAPI SetUncertainGestureColor(
        /* [in] */ Int32 color);

    virtual CARAPI GetUncertainGestureColor(
        /* [out] */ Int32 *color);

    virtual CARAPI GetGestureColor(
        /* [out] */ Int32 *color);

    virtual CARAPI GetGestureStrokeWidth(
        /* [out] */ Float *gestureStrokeWidth);

    virtual CARAPI SetGestureStrokeWidth(
        /* [in] */ Float gestureStrokeWidth);

    virtual CARAPI GetGestureStrokeType(
        /* [out] */ Int32 *gestureStrokeType);

    virtual CARAPI SetGestureStrokeType(
        /* [in] */ Int32 gestureStrokeType);

    virtual CARAPI GetGestureStrokeLengthThreshold(
        /* [out] */ Float *gestureStrokeLengthThreshold);

    virtual CARAPI SetGestureStrokeLengthThreshold(
        /* [in] */ Float gestureStrokeLengthThreshold);

    virtual CARAPI GetGestureStrokeSquarenessTreshold(
        /* [out] */ Float *gestureStrokeSquarenessTreshold);

    virtual CARAPI SetGestureStrokeSquarenessTreshold(
        /* [in] */ Float gestureStrokeSquarenessTreshold);

    virtual CARAPI GetGestureStrokeAngleThreshold(
        /* [out] */ Float *gestureStrokeAngleThreshold);

    virtual CARAPI SetGestureStrokeAngleThreshold(
        /* [in] */ Float gestureStrokeAngleThreshold);

    virtual CARAPI IsEventsInterceptionEnabled(
        /* [out] */ Boolean *isEventsInterceptionEnabled);

    virtual CARAPI SetEventsInterceptionEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI IsFadeEnabled(
        /* [out] */ Boolean *isFadeEnabled);

    virtual CARAPI SetFadeEnabled(
        /* [in] */ Boolean fadeEnabled);

    virtual CARAPI GetGesture(
        /* [out] */ IGesture **gesture);

    virtual CARAPI SetGesture(
        /* [in] */ IGesture *gesture);

    virtual CARAPI GetGesturePath(
        /* [out] */ IPath **gesturePath);

    virtual CARAPI GetGesturePathEx(
        /* [in] */ IPath *path,
        /* [out] */ IPath **gesturePath);

    virtual CARAPI IsGestureVisible(
        /* [out] */ Boolean *isGestureVisible);

    virtual CARAPI SetGestureVisible(
       /* [in] */ Boolean visible);

    virtual CARAPI GetFadeOffset(
        /* [out] */ Int64 *fadeOffset);

    virtual CARAPI SetFadeOffset(
        /* [in] */ Int64 fadeOffset);

    virtual CARAPI AddOnGestureListener(
        /* [in] */ IOnGestureListener *listener);

    virtual CARAPI RemoveOnGestureListener(
        /* [in] */ IOnGestureListener *listener);

    virtual CARAPI RemoveAllOnGestureListeners();

    virtual CARAPI AddOnGesturePerformedListener(
        /* [in] */ IOnGesturePerformedListener *listener);

    virtual CARAPI RemoveOnGesturePerformedListener(
        /* [in] */ IOnGesturePerformedListener *listener);

    virtual CARAPI RemoveAllOnGesturePerformedListeners();

    virtual CARAPI AddOnGesturingListener(
        /* [in] */ IOnGesturingListener *listener);

    virtual CARAPI RemoveOnGesturingListener(
        /* [in] */ IOnGesturingListener *listener);

    virtual CARAPI RemoveAllOnGesturingListeners();

    virtual CARAPI IsGesturing(
        /* [out] */ Boolean *isGesturing);

    /**
     * @hide
     */
    virtual CARAPI GetGesturePaint(
        /* [out] */ IPaint **gesturePaint);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas *canvas);

    virtual CARAPI Clear(
        /* [in] */ Boolean animated);

    virtual CARAPI CancelClearAnimation();

    virtual CARAPI CancelGesture();

    //@Override
    CARAPI DispatchTouchEvent(
        /* [in] */ IMotionEvent *event,
        /* [out] */ Boolean *isDispatched);

protected:
    //@Override
    CARAPI OnDetachedFromWindow();

private:
    GestureOverlayView();

    CARAPI_(void) init();

    CARAPI SetCurrentColor(
        /* [in] */ Int32 color);

    CARAPI SetPaintAlpha(
        /* [in] */ Int32 alpha);

    CARAPI Clear(
        /* [in] */ Boolean animated,
        /* [in] */ Boolean fireActionPerformed,
        /* [in] */ Boolean immediate);

    CARAPI ProcessEvent(
        /* [in] */ IMotionEvent *event,
        /* [out] */ Boolean *isProcessed);

    CARAPI TouchDown(
        /* [in] */ IMotionEvent *event);

    CARAPI TouchMove(
        /* [in] */ IMotionEvent *event,
        /* [out] */ IRect **rect);

    CARAPI TouchUp(
        /* [in] */ IMotionEvent * event,
        /* [in] */ Boolean cancel);

    CARAPI CancelGesture(
        /* [in] */ IMotionEvent * event);

    CARAPI FireOnGesturePerformed();

private:
    static const Int32 FADE_ANIMATION_RATE = 16;
    static const Boolean GESTURE_RENDERING_ANTIALIAS;
    static const Boolean DITHER_FLAG;

    const AutoPtr<IPaint> mGesturePaint;

    Int64 mFadeDuration;
    Int64 mFadeOffset;
    Int64 mFadingStart;
    Boolean mFadingHasStarted;
    Boolean mFadeEnabled;

    Int32 mCurrentColor;
    Int32 mCertainGestureColor;
    Int32 mUncertainGestureColor;
    Float mGestureStrokeWidth;
    Int32 mInvalidateExtraBorder;

    Int32 mGestureStrokeType;
    Float mGestureStrokeLengthThreshold;
    Float mGestureStrokeSquarenessTreshold;
    Float mGestureStrokeAngleThreshold;

    Int32 mOrientation;

    const AutoPtr<IRect> mInvalidRect;
    const AutoPtr<IPath> mPath;
    Boolean mGestureVisible;

    Float mX;
    Float mY;

    Float mCurveEndX;
    Float mCurveEndY;

    Float mTotalLength;
    Boolean mIsGesturing;
    Boolean mPreviousWasGesturing;
    Boolean mInterceptEvents;
    Boolean mIsListeningForGestures;
    Boolean mResetGesture;

    // current gesture
    AutoPtr<IGesture> mCurrentGesture;
    //const AutoPtr<List<IGesturePoint *> > mStrokeBuffer = new ArrayList<GesturePoint>(100);
    const AutoPtr<IObjectContainer> mStrokeBuffer;

    // TODO: Make this a list of WeakReferences
    const AutoPtr< List<IOnGestureListener *> > >mOnGestureListeners; //= new ArrayList<OnGestureListener>();
    // TODO: Make this a list of WeakReferences
    const AutoPtr< List<IOnGesturePerformedListener *> > mOnGesturePerformedListeners; //=
            //new ArrayList<OnGesturePerformedListener>();
    // TODO: Make this a list of WeakReferences
    const AutoPtr< List<IOnGesturingListener *> > mOnGesturingListeners;// =
            //new ArrayList<OnGesturingListener>();

    Boolean mHandleGestureActions;

    // fading out effect
    Boolean mIsFadingOut;
    Float mFadingAlpha;
    const AutoPtr<IAccelerateDecelerateInterpolator> mInterpolator;// =
            //new AccelerateDecelerateInterpolator();

    const AutoPtr<IFadeOutRunnable *> mFadingOut;// = new FadeOutRunnable();
private:
    class FadeOutRunnable {
    public:
        FadeOutRunnable();

        CARAPI Run();

        CARAPI GetFireActionPerformed(
            /* [out] */ Boolean *fireActionPerformed);

        CARAPI GetResetMultipleStrokes(
            /* [out] */ Boolean *resetMultipleStrokes);

        CARAPI SetFireActionPerformed(
            /* [in] */ Boolean fireActionPerformed);

        CARAPI SetResetMultipleStrokes(
            /* [in] */ Boolean resetMultipleStrokes);

    public:
        Boolean mFireActionPerformed;
        Boolean mResetMultipleStrokes;
    };
};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos

#endif //__GESTUREOVERLAYVIEW_H__
