
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_POINTERLOCATIONVIEW_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_POINTERLOCATIONVIEW_H__

#include "elastos/droid/view/View.h"
#include "elastos/droid/view/VelocityTracker.h"
#include "elastos/droid/view/CMotionEvent.h"

using Elastos::Droid::View::View;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::CMotionEvent;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::VelocityTracker;
using Elastos::Droid::View::IPointerCoords;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Hardware::Input::IInputManager;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class PointerLocationView : public Elastos::Droid::View::View
{
public:
    typedef VelocityTracker::Estimator Estimator;
    class PointerState : public ElRefBase
    {
    public:
        PointerState();

        CARAPI_(void) ClearTrace();

        CARAPI_(void) AddTrace(
            /* [in] */ Float x,
            /* [in] */ Float y);

    private:
        // Trace of previous points.
        AutoPtr<ArrayOf<Float> > mTraceX;
        AutoPtr<ArrayOf<Float> > mTraceY;
        Int32 mTraceCount;

        // True if the pointer is down.
        Boolean mCurDown;

        // Most recent coordinates.
        AutoPtr<IPointerCoords> mCoords;
        Int32 mToolType;

        // Most recent velocity.
        Float mXVelocity;
        Float mYVelocity;
        Float mAltXVelocity;
        Float mAltYVelocity;

        // Position estimator.
        AutoPtr<Estimator> mEstimator;
        AutoPtr<Estimator> mAltEstimator;
        friend class PointerLocationView;
    };

private:
    class FasterStringBuilder : public ElRefBase
    {
    public:
        FasterStringBuilder();

        CARAPI_(AutoPtr<FasterStringBuilder>) Clear();

        CARAPI_(AutoPtr<FasterStringBuilder>) Append(
            /* [in] */ const String& value);

        CARAPI_(AutoPtr<FasterStringBuilder>) Append(
            /* [in] */ Int32 value);

        CARAPI_(AutoPtr<FasterStringBuilder>) Append(
            /* [in] */ Int32 value,
            /* [in] */ Int32 zeroPadWidth);

        CARAPI_(AutoPtr<FasterStringBuilder>) Append(
            /* [in] */ Float value,
            /* [in] */ Int32 precision);

        CARAPI_(String) ToString();

    private:
        CARAPI_(Int32) Reserve(
            /* [in] */ Int32 length);

        AutoPtr<ArrayOf<Char32> > mChars;
        Int32 mLength;
    };
public:
    PointerLocationView(
        /* [in] */ IContext* c);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI SetPrintCoords(
        /* [in] */ Boolean state);

    virtual CARAPI AddPointerEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    virtual CARAPI OnInputDeviceAdded(
        /* [in] */ Int32 deviceId);

    virtual CARAPI OnInputDeviceChanged(
        /* [in] */ Int32 deviceId);

    virtual CARAPI OnInputDeviceRemoved(
        /* [in] */ Int32 deviceId);

protected:
    PointerLocationView();

    virtual CARAPI OnAttachedToWindow();

    virtual CARAPI OnDetachedFromWindow();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    CARAPI Init(
        /* [in] */ IContext* c);
private:
    static CARAPI_(Boolean) ShouldLogKey(
        /* [in] */ Int32 keyCode);

    CARAPI_(void) DrawOval(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float major,
        /* [in] */ Float minor,
        /* [in] */ Float angle,
        /* [in] */ IPaint* paint);

    CARAPI_(void) LogMotionEvent(
        /* [in] */ const String& type,
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) LogCoords(
        /* [in] */ const String& type,
        /* [in] */ Int32 action,
        /* [in] */ Int32 index,
        /* [in] */ IPointerCoords* coords,
        /* [in] */ Int32 id,
        /* [in] */ Int32 toolType,
        /* [in] */ Int32 buttonState);


    CARAPI_(void) LogInputDevices();

    CARAPI_(void) LogInputDeviceState(
        /* [in] */ Int32 deviceId,
        /* [in] */ const String& state);

    CARAPI InitInternal(
        /* [in] */ IContext* c);

    // Draw an oval.  When angle is 0 radians, orients the major axis vertically,
    // angles less than or greater than 0 radians rotate the major axis left or right.
    AutoPtr<IRectF> mReusableOvalRect;

    static const String TAG;

    // The system property key used to specify an alternate velocity tracker strategy
    // to plot alongside the default one.  Useful for testing and comparison purposes.
    static const String ALT_STRATEGY_PROPERY_KEY;

    Int32 ESTIMATE_PAST_POINTS;
    Int32 ESTIMATE_FUTURE_POINTS;
    Float ESTIMATE_INTERVAL;

    AutoPtr<IInputManager> mIm;

    AutoPtr<IViewConfiguration> mVC;
    AutoPtr<IPaint> mTextPaint;
    AutoPtr<IPaint> mTextBackgroundPaint;
    AutoPtr<IPaint> mTextLevelPaint;
    AutoPtr<IPaint> mPaint;
    AutoPtr<IPaint> mTargetPaint;
    AutoPtr<IPaint> mPathPaint;
    AutoPtr<IPaintFontMetricsInt> mTextMetrics;
    Int32 mHeaderBottom;
    Boolean mCurDown;
    Int32 mCurNumPointers;
    Int32 mMaxNumPointers;
    Int32 mActivePointerId;
    List<AutoPtr<PointerState> > mPointers;
    AutoPtr<IPointerCoords> mTempCoords;

    AutoPtr<VelocityTracker> mVelocity;
    AutoPtr<VelocityTracker> mAltVelocity;

    AutoPtr<FasterStringBuilder> mText;

    Boolean mPrintCoords;

    // HACK
    // A quick and dirty string builder implementation optimized for GC.
    // Using String.format causes the application grind to a halt when
    // more than a couple of pointers are down due to the number of
    // temporary objects allocated while formatting strings for drawing or logging.

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
