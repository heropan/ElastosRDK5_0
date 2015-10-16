#include "GestureOverlayView.h"
#include <elastos/core/Math.h>
#include "Widget/FrameLayout.h"
#include "View/Animation/AnimationUtils.h"
#include "elastos/droid/os/SystemClock.h"
#include "GestureStroke.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

const Int32 GestureOverlayView::FADE_ANIMATION_RATE;
const Boolean GestureOverlayView::GESTURE_RENDERING_ANTIALIAS = TRUE;
const Boolean GestureOverlayView::DITHER_FLAG = TRUE;

static AutoPtr<IPaint> CreatePaint()
{
    AutoPtr<CPaint> p;
    CPaint::NewByFriend((CPaint**)&p);
    return (IPaint *)p.Get();
}

static AutoPtr<IRect> CreateRect()
{
    AutoPtr<CRect> r;
    CRect::NewByFriend((CRect**)&r);
    return (IRect *)r.Get();
}

static AutoPtr<IAccelerateDecelerateInterpolator> CreateInterpolator()
{
    AutoPtr<CAccelerateDecelerateInterpolator> a;
    CAccelerateDecelerateInterpolator::NewByFriend((CAccelerateDecelerateInterpolator**)&a);
    return (IAccelerateDecelerateInterpolator *)a.Get();
}

AutoPtr<IFadeOutRunnable> CreateFadingOutRunnable()
{
    AutoPtr<CFadeOutRunnable> f;
    CFadeOutRunnable::New((CFadeOutRunnable **)&f);
    return (IFadeOutRunnable *)f.Get();
}

GestureOverlayView::GestureOverlayView() : mGesturePaint(InitGesturePaint()),
    mFadeDuration(150), mFadeOffset(420), mFadingStart(420), mFadingHasStarted(FALSE),
    mFadeEnabled(TRUE), mCurrentColor(0), mCertainGestureColor(0xFFFFFF00),
    mUncertainGestureColor(0x48FFFF00), mGestureStrokeWidth(12.0f),
    mInvalidateExtraBorder(10), mGestureStrokeType(GESTURE_STROKE_TYPE_SINGLE),
    mGestureStrokeLengthThreshold(50.0f), mGestureStrokeSquarenessTreshold(0.275f),
    mGestureStrokeAngleThreshold(40.0f), mOrientation(ORIENTATION_VERTICAL),
    mInvalidRect(InitInvalidRect()), mPath(InitPath()), mGestureVisible(TRUE),
    mX(0.0f), mY(0.0f), mCurveEndX(0.0f), mCurveEndY(0.0f), mTotalLength(0.0f),
    mIsGesturing(FALSE), mPreviousWasGesturing(FALSE), mInterceptEvents(TRUE),
    mIsListeningForGestures(FALSE), mResetGesture(FALSE),
    mHandleGestureActions(FALSE), mIsFadingOut(FALSE), mFadingAlpha(1.0f),
    mInterpolator(InitInterpolator()), mFadingOut(InitFadingOut())
{}

void GestureOverlayView::init()
{
    SetWillNotDraw(FALSE);

    const AutoPtr<IPaint> gesturePaint = mGesturePaint;
    gesturePaint->SetAntiAlias(GESTURE_RENDERING_ANTIALIAS);
    gesturePaint->SetColor(mCertainGestureColor);
    gesturePaint->SetStyle(PaintStyle::PaintStyle_STROKE);
    gesturePaint->SetStrokeJoin(PaintJoin::PaintJoin_ROUND);
    gesturePaint->SetStrokeCap(PaintCap::PaintCap_ROUND);
    gesturePaint->SetStrokeWidth(mGestureStrokeWidth);
    gesturePaint->SetDither(DITHER_FLAG);

    mCurrentColor = mCertainGestureColor;
    SetPaintAlpha(255);
}

ECode GestureOverlayView::Init(
    /* [in] */ IContext *context)
{
    //super(context);
    init();
    return NOERROR;
}

ECode GestureOverlayView::Init(
    /* [in] */ IContext *context,
    /* [in] */ IAttributeSet *attrs)
{
    Init(context, attrs, com.android.internal.R.attr.gestureOverlayViewStyle);
    return NOERROR;
}

ECode GestureOverlayView::Init(
    /* [in] */ IContext *context,
    /* [in] */ IAttributeSet *attrs,
    /* [in] */ Int32 defStyle)
{
/*
    super(context, attrs, defStyle);

    TypedArray a = context.obtainStyledAttributes(attrs,
            R.styleable.GestureOverlayView, defStyle, 0);

    mGestureStrokeWidth = a.getFloat(R.styleable.GestureOverlayView_gestureStrokeWidth,
            mGestureStrokeWidth);
    mInvalidateExtraBorder = Math.max(1, ((int) mGestureStrokeWidth) - 1);
    mCertainGestureColor = a.getColor(R.styleable.GestureOverlayView_gestureColor,
            mCertainGestureColor);
    mUncertainGestureColor = a.getColor(R.styleable.GestureOverlayView_uncertainGestureColor,
            mUncertainGestureColor);
    mFadeDuration = a.getInt(R.styleable.GestureOverlayView_fadeDuration, (int) mFadeDuration);
    mFadeOffset = a.getInt(R.styleable.GestureOverlayView_fadeOffset, (int) mFadeOffset);
    mGestureStrokeType = a.getInt(R.styleable.GestureOverlayView_gestureStrokeType,
            mGestureStrokeType);
    mGestureStrokeLengthThreshold = a.getFloat(
            R.styleable.GestureOverlayView_gestureStrokeLengthThreshold,
            mGestureStrokeLengthThreshold);
    mGestureStrokeAngleThreshold = a.getFloat(
            R.styleable.GestureOverlayView_gestureStrokeAngleThreshold,
            mGestureStrokeAngleThreshold);
    mGestureStrokeSquarenessTreshold = a.getFloat(
            R.styleable.GestureOverlayView_gestureStrokeSquarenessThreshold,
            mGestureStrokeSquarenessTreshold);
    mInterceptEvents = a.getBoolean(R.styleable.GestureOverlayView_eventsInterceptionEnabled,
            mInterceptEvents);
    mFadeEnabled = a.getBoolean(R.styleable.GestureOverlayView_fadeEnabled,
            mFadeEnabled);
    mOrientation = a.getInt(R.styleable.GestureOverlayView_orientation, mOrientation);

    a.recycle();
*/
    init();
    return NOERROR;
}

ECode GestureOverlayView::GetCurrentStroke(
    /* [out] */ IObjectContainer **stroke)
{
    VALIDATE_NOT_NULL(stroke);
    *stroke = mStrokeBuffer;
    REFCOUNT_ADD(*stroke);
    return NOERROR;
}

ECode GestureOverlayView::GetOrientation(
    /* [out] */ Int32 *orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mOrientation;
    return NOERROR;
}

ECode GestureOverlayView::SetOrientation(
    /* [in] */ Int32 orientation)
{
    mOrientation = orientation;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureColor(
    /* [in] */ Int32 color)
{
    mCertainGestureColor = color;
    return NOERROR;
}

ECode GestureOverlayView::SetUncertainGestureColor(
    /* [in] */ Int32 color)
{
    mUncertainGestureColor = color;
    return NOERROR;
}

ECode GestureOverlayView::GetUncertainGestureColor(
    /* [out] */ Int32 *color)
{
    VALIDATE_NOT_NULL(color);
    *color = mCertainGestureColor;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureColor(
    /* [out] */ Int32 *color)
{
    VALIDATE_NOT_NULL(color);
    *color = mCertainGestureColor;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeWidth(
    /* [out] */ Float *gestureStrokeWidth)
{
    VALIDATE_NOT_NULL(gestureStrokeWidth);
    *gestureStrokeWidth = mGestureStrokeWidth;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeWidth(
    /* [in] */ Float gestureStrokeWidth)
{
    mGestureStrokeWidth = gestureStrokeWidth;
    mInvalidateExtraBorder = Elastos::Core::Math::Max(1, ((Int32) gestureStrokeWidth) - 1);
    mGesturePaint->SetStrokeWidth(gestureStrokeWidth);
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeType(
    /* [out] */ Int32 *gestureStrokeType)
{
    VALIDATE_NOT_NULL(gestureStrokeType);
    *gestureStrokeType = mGestureStrokeType;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeType(
    /* [in] */ Int32 gestureStrokeType)
{
    mGestureStrokeType = gestureStrokeType;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeLengthThreshold(
    /* [out] */ Float *gestureStrokeLengthThreshold)
{
    VALIDATE_NOT_NULL(gestureStrokeLengthThreshold);
    *gestureStrokeLengthThreshold = mGestureStrokeLengthThreshold;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeLengthThreshold(
    /* [in] */ Float gestureStrokeLengthThreshold)
{
    mGestureStrokeLengthThreshold = gestureStrokeLengthThreshold;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeSquarenessTreshold(
    /* [out] */ Float *gestureStrokeSquarenessTreshold)
{
    VALIDATE_NOT_NULL(gestureStrokeSquarenessTreshold);
    *gestureStrokeSquarenessTreshold = mGestureStrokeSquarenessTreshold;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeSquarenessTreshold(
    /* [in] */ Float gestureStrokeSquarenessTreshold)
{
    mGestureStrokeSquarenessTreshold = gestureStrokeSquarenessTreshold;
    return NOERROR;
}

ECode GestureOverlayView::GetGestureStrokeAngleThreshold(
    /* [out] */ Float *gestureStrokeAngleThreshold)
{
    VALIDATE_NOT_NULL(gestureStrokeAngleThreshold);
    *gestureStrokeAngleThreshold = mGestureStrokeAngleThreshold;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureStrokeAngleThreshold(
    /* [in] */ Float gestureStrokeAngleThreshold)
{
    mGestureStrokeAngleThreshold = gestureStrokeAngleThreshold;
    return NOERROR;
}

ECode GestureOverlayView::IsEventsInterceptionEnabled(
    /* [out] */ Boolean *isEventsInterceptionEnabled)
{
    VALIDATE_NOT_NULL(isEventsInterceptionEnabled);
    *isEventsInterceptionEnabled = mInterceptEvents;
    return NOERROR;
}

ECode GestureOverlayView::SetEventsInterceptionEnabled(
    /* [in] */ Boolean enabled)
{
    mInterceptEvents = enabled;
    return NOERROR;
}

ECode GestureOverlayView::IsFadeEnabled(
    /* [out] */ Boolean *isFadeEnabled)
{
    VALIDATE_NOT_NULL(isFadeEnabled);
    *isFadeEnabled = mFadeEnabled;
    return NOERROR;
}

ECode GestureOverlayView::SetFadeEnabled(
    /* [in] */ Boolean fadeEnabled)
{
    mFadeEnabled = fadeEnabled;
    return NOERROR;
}

ECode GestureOverlayView::GetGesture(
    /* [out] */ IGesture **gesture)
{
    VALIDATE_NOT_NULL(gesture);
    *gesture = mCurrentGesture;
    REFCOUNT_ADD(*gesture);
    return NOERROR;
}

ECode GestureOverlayView::SetGesture(
    /* [in] */ IGesture *gesture)
{
    if (mCurrentGesture != NULL) {
        Clear(FALSE);
    }

    SetCurrentColor(mCertainGestureColor);
    mCurrentGesture = gesture;

    const AutoPtr<IPath> path;
    mCurrentGesture->ToPath((IPath**)&path);
    const AutoPtr<IRectF> bounds;
    CRectF::New((IRectF**)&bounds);
    path->ComputeBounds(bounds, TRUE);

    // TODO: The path should also be scaled to fit inside this view
    mPath->Rewind();
    Float left, width, top, height;
    bounds->GetLeft(&left);
    bounds->GetWidth(&width);
    bounds->GetTop(&top);
    bounds->GetHeight(&height);
    Int32 w,h;
    this->GetWidth(&w);
    this->GetHeight(&h);
    mPath->AddPath(path, -left + (w - width) / 2.0f,
            -top + (h - height) / 2.0f);

    mResetGesture = TRUE;

    Invalidate();

    return NOERROR;
}

ECode GestureOverlayView::GetGesturePath(
    /* [out] */ IPath **gesturePath)
{
    VALIDATE_NOT_NULL(gesturePath);
    *gesturePath = mPath;
    REFCOUNT_ADD(*gesturePath);
    return NOERROR;
}

ECode GestureOverlayView::GetGesturePath(
    /* [in] */ IPath *path,
    /* [out] */ IPath **gesturePath)
{
    VALIDATE_NOT_NULL(gesturePath);
    path->Set(mPath);
    *gesturePath = path;
    REFCOUNT_ADD(*gesturePath);
    return NOERROR;
}

ECode GestureOverlayView::IsGestureVisible(
    /* [out] */ Boolean *isGestureVisible)
{
    VALIDATE_NOT_NULL(isGestureVisible);
    *isGestureVisible = mGestureVisible;
    return NOERROR;
}

ECode GestureOverlayView::SetGestureVisible(
   /* [in] */ Boolean visible)
{
    mGestureVisible = visible;
    return NOERROR;
}

ECode GestureOverlayView::GetFadeOffset(
    /* [out] */ Int64 *fadeOffset)
{
    VALIDATE_NOT_NULL(fadeOffset);
    *fadeOffset = mFadeOffset;
    return NOERROR;
}

ECode GestureOverlayView::SetFadeOffset(
    /* [in] */ Int64 fadeOffset)
{
    mFadeOffset = fadeOffset;
    return NOERROR;
}

ECode GestureOverlayView::AddOnGestureListener(
    /* [in] */ IOnGestureListener *listener)
{
    return mOnGestureListeners->Add(listener);
}

ECode GestureOverlayView::RemoveOnGestureListener(
    /* [in] */ IOnGestureListener *listener)
{
    return mOnGestureListeners->Remove(listener);
}

ECode GestureOverlayView::RemoveAllOnGestureListeners()
{
    return mOnGestureListeners->Clear();
}

ECode GestureOverlayView::AddOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener *listener)
{
    mOnGesturePerformedListeners->Add(listener);
    if (mOnGesturePerformedListeners->IsEmpty() == FALSE) {
        mHandleGestureActions = TRUE;
    }
    return NOERROR;
}

ECode GestureOverlayView::RemoveOnGesturePerformedListener(
    /* [in] */ IOnGesturePerformedListener *listener)
{
    mOnGesturePerformedListeners->Remove(listener);
    if (mOnGesturePerformedListeners->IsEmpty()) {
        mHandleGestureActions = FALSE;
    }
    return NOERROR;
}

ECode GestureOverlayView::RemoveAllOnGesturePerformedListeners()
{
    mOnGesturePerformedListeners->Clear();
    mHandleGestureActions = FALSE;
    return NOERROR;
}

ECode GestureOverlayView::AddOnGesturingListener(
    /* [in] */ IOnGesturingListener *listener)
{
    return mOnGesturingListeners->Add(listener);
}

ECode GestureOverlayView::RemoveOnGesturingListener(
    /* [in] */ IOnGesturingListener *listener)
{
    return mOnGesturingListeners->Remove(listener);
}

ECode GestureOverlayView::RemoveAllOnGesturingListeners()
{
    return mOnGesturingListeners->Clear();
}

ECode GestureOverlayView::IsGesturing(
    /* [out] */ Boolean *isGesturing)
{
    VALIDATE_NOT_NULL(isGesturing);
    *isGesturing = mIsGesturing;
    return NOERROR;
}

ECode GestureOverlayView::SetCurrentColor(
    /* [in] */ Int32 color)
{
    mCurrentColor = color;
    if (mFadingHasStarted) {
        SetPaintAlpha((Int32) (255 * mFadingAlpha));
    } else {
        SetPaintAlpha(255);
    }
    Invalidate();

    return NOERROR;
}

ECode GestureOverlayView::GetGesturePaint(
    /* [out] */ IPaint **gesturePaint)
{
    VALIDATE_NOT_NULL(gesturePaint);
    *gesturePaint = mGesturePaint;
    REFCOUNT_ADD(*gesturePaint);
    return NOERROR;
}

ECode GestureOverlayView::Draw(
    /* [in] */ ICanvas *canvas)
{
    FrameLayout::Draw(canvas);

    if (mCurrentGesture != NULL && mGestureVisible) {
        canvas->DrawPath(mPath, mGesturePaint);
    }

    return NOERROR;
}

ECode GestureOverlayView::SetPaintAlpha(
    /* [in] */ Int32 alpha)
{
    alpha += alpha >> 7;
    const Int32 baseAlpha = (mCurrentColor/2e24);//mCurrentColor >>> 24;
    const Int32 useAlpha = baseAlpha * alpha >> 8;
    mGesturePaint->SetColor((mCurrentColor << 8)/2e8) | (useAlpha << 24));

    return NOERROR;
}

ECode GestureOverlayView::Clear(
    /* [in] */ Boolean animated)
{
    return Clear(animated, FALSE, TRUE);
}

ECode GestureOverlayView::Clear(
    /* [in] */ Boolean animated,
    /* [in] */ Boolean fireActionPerformed,
    /* [in] */ Boolean immediate)
{
    SetPaintAlpha(255);
    RemoveCallbacks(mFadingOut);
    mResetGesture = FALSE;
    mFadingOut->SetFireActionPerformed(fireActionPerformed);
    mFadingOut->SetResetMultipleStrokes(FALSE);

    if (animated && mCurrentGesture != NULL) {
        mFadingAlpha = 1.0f;
        mIsFadingOut = TRUE;
        mFadingHasStarted = FALSE;
        Int64 millis;
        AnimationUtils::CurrentAnimationTimeMillis(&millis);
        mFadingStart = millis + mFadeOffset;
        PostDelayed(mFadingOut, mFadeOffset);
    } else {
        mFadingAlpha = 1.0f;
        mIsFadingOut = FALSE;
        mFadingHasStarted = FALSE;

        if (immediate) {
            mCurrentGesture = NULL;
            mPath->Rewind();
            Invalidate();
        } else if (fireActionPerformed) {
            PostDelayed(mFadingOut, mFadeOffset);
        } else if (mGestureStrokeType == GESTURE_STROKE_TYPE_MULTIPLE) {
            mFadingOut->SetResetMultipleStrokes(TRUE);
            PostDelayed(mFadingOut, mFadeOffset);
        } else {
            mCurrentGesture = NULL;
            mPath->Rewind();
            Invalidate();
        }
    }

    return NOERROR;
}

ECode GestureOverlayView::CancelClearAnimation()
{
    SetPaintAlpha(255);
    mIsFadingOut = FALSE;
    mFadingHasStarted = FALSE;
    RemoveCallbacks(mFadingOut);
    mPath->Rewind();
    mCurrentGesture = NULL;

    return NOERROR;
}

ECode GestureOverlayView::CancelGesture()
{
    mIsListeningForGestures = FALSE;

    // add the stroke to the current gesture
    AutoPtr<IGestureStroke> stroke;
    CGestureStroke::New(mStrokeBuffer, (IGestureStroke**)&stroke);
    mCurrentGesture->AddStroke(stroke);

    // pass the event to handlers
    const Int64 now;
    SystemClock::UptimeMillis(&now);
    const AutoPtr<IMotionEvent> event;
    CMotionEvent::Obtain(now, now,
            ACTION_CANCEL, 0.0f, 0.0f, 0, (IMotionEvent**)&event);

    AutoPtr< List<IOnGestureListener *> > listeners = mOnGestureListeners;
    Int32 count = listeners->GetSize();
    List<IOnGestureListener *>::Iterator iter = listeners->Begin();
    for (Int32 i = 0; i < count; i++) {
        if (iter != listeners->End()) {
            AutoPtr<IOnGestureListener> listener = *iter;
            listener->OnGestureCancelled(THIS_PROBE(IGestureOverlayView), event);
        }
        ++iter;
    }

    event->Recycle();

    Clear(FALSE);
    mIsGesturing = FALSE;
    mPreviousWasGesturing = FALSE;
    mStrokeBuffer->Clear();

    AutoPtr< List<IOnGesturingListener *> > otherListeners = mOnGesturingListeners;
    count = otherListeners->GetSize();
    List<IOnGesturingListener *>::Iterator it = otherListeners->Begin();
    for (Int32 i = 0; i < count; i++) {
        if (it != otherListeners->End()) {
            AutoPtr<IOnGesturingListener> otherListener = *it;
            otherListener->onGesturingEnded(THIS_PROBE(IGestureOverlayView));
        }
        ++it;
    }

    return NOERROR;
}

ECode GestureOverlayView::OnDetachedFromWindow()
{
    return CancelClearAnimation();
}

ECode GestureOverlayView::DispatchTouchEvent(
    /* [in] */ IMotionEvent *event,
    /* [out] */ Boolean *isDispatched)
{
    VALIDATE_NOT_NULL(isDispatched);
    if (isEnabled()) {
        Int32 strokesCount;
        mCurrentGesture->GetStrokesCount(&strokesCount);
        const Boolean cancelDispatch = (mIsGesturing || (mCurrentGesture != NULL &&
                strokesCount > 0 && mPreviousWasGesturing)) &&
                mInterceptEvents;

        ProcessEvent(event);

        if (cancelDispatch) {
            event->SetAction(ACTION_CANCEL);
        }

        AutoPtr<IView> v = THIS_PROBE(IView);
        v->DispatchTouchEvent(event);

        *isDispatched = TRUE;
        return NOERROR;
    }

    AutoPtr<IView> v = THIS_PROBE(IView);
    *isDispatched = v->DispatchTouchEvent(event);

    return NOERROR;
}

ECode GestureOverlayView::ProcessEvent(
    /* [in] */ IMotionEvent *event,
    /* [out] */ Boolean *isProcessed)
{
    VALIDATE_NOT_NULL(isProcessed);
    Int32 action;
    event->GetAction(&action);
    switch (action) {
        case MotionEvent::ACTION_DOWN:
            TouchDown(event);
            Invalidate();
            *isProcessed = TRUE;
            return NOERROR;
        case MotionEvent::ACTION_MOVE:
            if (mIsListeningForGestures) {
                AutoPtr<IRect> rect;
                TouchMove(event, (IRect**)&rect);
                if (rect != NULL) {
                    Invalidate(rect);
                }
                *isProcessed = TRUE;
                return NOERROR;
            }
            break;
        case MotionEvent::ACTION_UP:
            if (mIsListeningForGestures) {
                TouchUp(event, FALSE);
                Invalidate();
                *isProcessed = TRUE;
                return NOERROR;
            }
            break;
        case MotionEvent::ACTION_CANCEL:
            if (mIsListeningForGestures) {
                TouchUp(event, TRUE);
                Invalidate();
                *isProcessed = TRUE;
                return NOERROR;
            }
    }
    *isProcessed = FALSE;
    return NOERROR;
}

ECode GestureOverlayView::TouchDown(
    /* [in] */ IMotionEvent *event)
{
    mIsListeningForGestures = TRUE;

    Float x;
    event->GetX(&x);
    Float y;
    event->GetY(&y);

    mX = x;
    mY = y;

    mTotalLength = 0;
    mIsGesturing = FALSE;

    Int32 strokesCount;
    mCurrentGesture->GetStrokesCount(&strokesCount);
    if (mGestureStrokeType == GESTURE_STROKE_TYPE_SINGLE || mResetGesture) {
        if (mHandleGestureActions) {
            SetCurrentColor(mUncertainGestureColor);
        }
        mResetGesture = FALSE;
        mCurrentGesture = NULL;
        mPath->Rewind();
    } else if (mCurrentGesture == NULL || strokesCount == 0) {
        if (mHandleGestureActions) {
            SetCurrentColor(mUncertainGestureColor);
        }
    }

    // if there is fading out going on, stop it.
    if (mFadingHasStarted) {
        CancelClearAnimation();
    } else if (mIsFadingOut) {
        SetPaintAlpha(255);
        mIsFadingOut = FALSE;
        mFadingHasStarted = FALSE;
        RemoveCallbacks(mFadingOut);
    }

    if (mCurrentGesture == NULL) {
        CGesture::New((IGesture**)&mCurrentGesture);
    }

    Int64 eventTime;
    event->GetEventTime(&eventTime);
    AutoPtr<IGesturePoint> gp;
    CGesturePoint::New(x, y, eventTime, (IGesturePoint**)&gp);
    mStrokeBuffer->Add(gp);
    mPath->MoveTo(x, y);

    const Int32 border = mInvalidateExtraBorder;
    mInvalidRect->Set((Int32) x - border, (Int32) y - border, (Int32) x + border, (Int32) y + border);

    mCurveEndX = x;
    mCurveEndY = y;

    // pass the event to handlers
    AutoPtr< List<IOnGestureListener *> > listeners = mOnGestureListeners;
    const Int32 count = listeners->GetSize();
    List<IOnGestureListener *>::Iterator iter = listeners->Begin();
    for (Int32 i = 0; i < count; i++) {
        if (iter != listeners->End()) {
            AutoPtr<IOnGestureListener> listener = *iter;
            listener->onGestureStarted(THIS_PROBE(IGestureOverlayView), event);
        }
        ++iter;
    }

    return NOERROR;
}

ECode GestureOverlayView::TouchMove(
    /* [in] */ IMotionEvent *event,
    /* [out] */ IRect **rect)
{
    VALIDATE_NOT_NULL(rect);
    AutoPtr<IRect> areaToRefresh;

    const Float x;
    event->GetX(&x);
    const Float y;
    event->GetY(&y);

    const Float previousX = mX;
    const Float previousY = mY;

    const Float dx = Elastos::Core::Math::Abs(x - previousX);
    const Float dy = Elastos::Core::Math::Abs(y - previousY);

    if (dx >= GestureStroke::TOUCH_TOLERANCE || dy >= GestureStroke::TOUCH_TOLERANCE) {
        areaToRefresh = mInvalidRect;

        // start with the curve end
        const Int32 border = mInvalidateExtraBorder;
        areaToRefresh->Set((Int32) mCurveEndX - border, (Int32) mCurveEndY - border,
                (Int32) mCurveEndX + border, (Int32) mCurveEndY + border);

        Float cX = mCurveEndX = (x + previousX) / 2;
        Float cY = mCurveEndY = (y + previousY) / 2;

        mPath->QuadTo(previousX, previousY, cX, cY);

        // union with the control point of the new curve
        areaToRefresh->Union((Int32) previousX - border, (Int32) previousY - border,
                (Int32) previousX + border, (Int32) previousY + border);

        // union with the end point of the new curve
        areaToRefresh->Union((Int32) cX - border, (Int32) cY - border,
                (Int32) cX + border, (Int32) cY + border);

        mX = x;
        mY = y;

        Int64 eventTime;
        event->GetEventtime(&eventTime);
        AutoPtr<IGesturePoint> point;
        CGesturePoint::New(x, y, eventTime, (IGesturePoint**)&point);
        mStrokeBuffer->Add(point);

        if (mHandleGestureActions && !mIsGesturing) {
            mTotalLength += (Float) Elastos::Core::Math::Sqrt(dx * dx + dy * dy);

            if (mTotalLength > mGestureStrokeLengthThreshold) {
                const AutoPtr<IOrientedBoundingBox> box =
                        GestureUtils::ComputeOrientedBoundingBox(mStrokeBuffer);

                Float orientation;
                box->GetOrientation(&orientation);
                Float angle = Elastos::Core::Math::Abs(orientation);
                if (angle > 90) {
                    angle = 180 - angle;
                }

                Float squareness;
                box->GetSquareness(&squareness);
                if (squareness > mGestureStrokeSquarenessTreshold ||
                        (mOrientation == ORIENTATION_VERTICAL ?
                                angle < mGestureStrokeAngleThreshold :
                                angle > mGestureStrokeAngleThreshold)) {

                    mIsGesturing = TRUE;
                    SetCurrentColor(mCertainGestureColor);

                    AutoPtr< List<IOnGesturingListener *> > listeners = mOnGesturingListeners;
                    Int32 count = listeners->GetSize();
                    List<IOnGesturingListener *>::Iterator iter = listeners->Begin();
                    for (Int32 i = 0; i < count; i++) {
                        if (iter != listeners->End()) {
                            AutoPtr<IOnGesturingListener> listener = *iter;
                            listener->OnGesturingStarted(THIS_PROBE(IGestureOverlayView));
                        }
                        ++iter;
                    }
                }
            }
        }

        // pass the event to handlers
        AutoPtr< List<IOnGestureListener *> > listeners = mOnGestureListeners;
        const Int32 count = listeners->GetSize();
        List<IOnGestureListener *>::Iterator iter = listeners->Begin();
        for (Int32 i = 0; i < count; i++) {
            if (iter != listeners->End()) {
                AutoPtr<IOnGestureListener> listener = *iter;
                listener->OnGesture(THIS_PROBE(IGestureOverlayView), event);
            }
            ++iter;
        }
    }

    *rect = areaToRefresh;
    REFCOUNT_ADD(rect);
    return NOERROR;
}

ECode GestureOverlayView::TouchUp(
    /* [in] */ IMotionEvent * event,
    /* [in] */ Boolean cancel)
{
    mIsListeningForGestures = FALSE;

    // A gesture wasn't started or was cancelled
    if (mCurrentGesture != NULL) {
        // add the stroke to the current gesture
        AutoPtr<IGestureStroke> stroke;
        CGestureStroke::New(mStrokeBuffer, (IGestureStroke**)&stroke);
        mCurrentGesture->AddStroke(stroke);

        if (!cancel) {
            // pass the event to handlers
            AutoPtr< List<IOnGestureListener *> > listeners = mOnGestureListeners;
            Int32 count = listeners->GetSize();
            List<IOnGestureListener *>::Iterator iter = listeners->Begin();
            for (Int32 i = 0; i < count; i++) {
                if (iter != listeners->End()) {
                    AutoPtr<IOnGestureListener> listener = *iter;
                    listener->OnGestureEnded(THIS_PROBE(IGestureOverlayView), event);
                }
                ++iter;
            }

            Clear(mHandleGestureActions && mFadeEnabled, mHandleGestureActions && mIsGesturing,
                    FALSE);
        } else {
            CancelGesture(event);

        }
    } else {
        CancelGesture(event);
    }

    mStrokeBuffer->Dispose();
    mPreviousWasGesturing = mIsGesturing;
    mIsGesturing = FALSE;

    AutoPtr< List<IOnGesturingListener *> > listeners = mOnGesturingListeners;
    Int32 count = listeners->GetSize();
    List<IOnGesturingListener *>::Iterator iter = listeners->Begin();
    for (Int32 i = 0; i < count; i++) {
        if (iter != listeners->End()) {
            AutoPtr<IOnGesturingListener> listener = *iter;
            listener->OnGesturingEnded(THIS_PROBE(IGestureOverlayView));
        }
        ++iter;
    }

    return NOERROR;
}

ECode GestureOverlayView::CancelGesture(
    /* [in] */ IMotionEvent * event)
{
    // pass the event to handlers
    AutoPtr< List<IOnGestureListener *> > listeners = mOnGestureListeners;
    const Int32 count = listeners->GetSize();
    List<IOnGestureListener *>::Iterator iter = listeners->Begin();
    for (Int32 i = 0; i < count; i++) {
        if (iter != listeners->End()) {
            AutoPtr<IOnGestureListener> listener = *iter;
            listener->OnGestureCancelled(THIS_PROBE(IGestureOverlayView), event);
        }
        ++iter;
    }

    Clear(FALSE);

    return NOERROR;
}

ECode GestureOverlayView::FireOnGesturePerformed()
{
    AutoPtr< List<IOnGesturePerformedListener *> > actionListeners = mOnGesturePerformedListeners;
    const Int32 count = actionListeners->GetSize();
    List<IOnGesturePerformedListener *>::Iterator iter = actionListeners->Begin();
    for (Int32 i = 0; i < count; i++) {
        if (iter != actionListeners->End()) {
            AutoPtr<IOnGesturePerformedListener> actionListener = *iter;
            actionListener->OnGesturePerformed(THIS_PROBE(IGestureOverlayView), mCurrentGesture);
        }
        ++iter;
    }

    return NOERROR;
}

GestureOverlayView::FadeOutRunnable::FadeOutRunnable()
    : fireActionPerformed(FALSE), resetMultipleStrokes(FALSE)
{}

ECode GestureOverlayView::FadeOutRunnable::Run()
{
    if (mIsFadingOut) {
        const Int64 now = AnimationUtils::CurrentAnimationTimeMillis();
        const Int64 duration = now - mFadingStart;

        if (duration > mFadeDuration) {
            if (mFireActionPerformed) {
                FireOnGesturePerformed();
            }

            mPreviousWasGesturing = FALSE;
            mIsFadingOut = FALSE;
            mFadingHasStarted = FALSE;
            mPath->Rewind();
            mCurrentGesture = NULL;
            SetPaintAlpha(255);
        } else {
            mFadingHasStarted = TRUE;
            Float interpolatedTime = Elastos::Core::Math::Max(0.0f,
                    Elastos::Core::Math::Min(1.0f, duration / (Float) mFadeDuration));

            Float output;
            mInterpolator->GetInterpolation(interpolatedTime, &output);
            mFadingAlpha = 1.0f - output;
            SetPaintAlpha((Int32) (255 * mFadingAlpha));
            PostDelayed(this, FADE_ANIMATION_RATE);
        }
    }
    else if (mResetMultipleStrokes) {
        mResetGesture = TRUE;
    }
    else {
        FireOnGesturePerformed();

        mFadingHasStarted = FALSE;
        mPath->Rewind();
        mCurrentGesture = NULL;
        mPreviousWasGesturing = FALSE;
        SetPaintAlpha(255);
    }

    Invalidate();

    return NOERROR;
}

ECode GestureOverlayView::FadeOutRunnable::GetFireActionPerformed(
    /* [out] */ Boolean *fireActionPerformed)
{
    VALIDATE_NOT_NULL(fireActionPerformed);
    *fireActionPerformed = mFireActionPerformed;
    return NOERROR;
}

ECode GestureOverlayView::FadeOutRunnable::GetResetMultipleStrokes(
    /* [out] */ Boolean *resetMultipleStrokes)
{
    VALIDATE_NOT_NULL(resetMultipleStrokes);
    *resetMultipleStrokes = mResetMultipleStrokes;
    return NOERROR;
}

ECode GestureOverlayView::FadeOutRunnable::SetFireActionPerformed(
    /* [in] */ Boolean fireActionPerformed)
{
    mFireActionPerformed = fireActionPerformed;
    return NOERROR;
}

ECode GestureOverlayView::FadeOutRunnable::SetResetMultipleStrokes(
    /* [in] */ Boolean resetMultipleStrokes)
{
    mResetMultipleStrokes = resetMultipleStrokes;
    return NOERROR;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
