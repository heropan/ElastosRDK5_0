
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNVIEW_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNVIEW_H__

#include "elastos/droid/view/View.h"
#include "elastos/droid/graphics/CRect.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Widget::Internal::IOnPatternListener;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class LockPatternView : public Elastos::Droid::View::View
{
public:
    class Cell
        : public IInterface
        , public ElRefBase
    {
    private:

        static CARAPI CheckRange(
            /* [in] */ Int32 row,
            /* [in] */ Int32 column);

    public:
        CAR_INTERFACE_DECL()

        Cell(
            /* [in] */ Int32 row,
            /* [in] */ Int32 column);

        virtual CARAPI_(Int32) GetRow();

        virtual CARAPI_(Int32) GetColumn();

        /**
         * @param row The row of the cell.
         * @param column The column of the cell.
         */
        static CARAPI_(AutoPtr<Cell>) Of(
            /* [in] */ Int32 row,
            /* [in] */ Int32 column);

        virtual CARAPI_(String) ToString();

    public:
        Int32 mRow;
        Int32 mColumn;
        static AutoPtr<Cell> sCells[3][3];
        static Boolean sInit;
        // static AutoPtr< ArrayOf< AutoPtr< ArrayOf<Cell*> > > > sCells;
    };

public:
    LockPatternView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    virtual CARAPI_(Boolean) IsInStealthMode();

    virtual CARAPI_(Boolean) IsTactileFeedbackEnabled();

    virtual CARAPI SetInStealthMode(
        /* [in] */ Boolean inStealthMode);

    virtual CARAPI SetTactileFeedbackEnabled(
        /* [in] */ Boolean tactileFeedbackEnabled);

    virtual CARAPI SetOnPatternListener(
        /* [in] */ IOnPatternListener* onPatternListener);

    virtual CARAPI SetPattern(
        /* [in] */ DisplayMode displayMode,
        /* [in] */ IObjectContainer* pattern);

    virtual CARAPI SetDisplayMode(
        /* [in] */ DisplayMode displayMode);

    virtual CARAPI ClearPattern();

    virtual CARAPI DisableInput();

    virtual CARAPI EnableInput();

    virtual CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ IMotionEvent* event);


    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

private:
    CARAPI_(AutoPtr<IBitmap>) GetBitmapFor(
        /* [in] */ Int32 resId);

    CARAPI_(void) NotifyCellAdded();

    CARAPI_(void) NotifyPatternStarted();

    CARAPI_(void) NotifyPatternDetected();

    CARAPI_(void) NotifyPatternCleared();

    CARAPI_(void) ResetPattern();

    CARAPI_(void) ClearPatternDrawLookup();

    CARAPI_(Int32) ResolveMeasured(
        /* [in] */ Int32 measureSpec,
        /* [in] */ Int32 desired);

    CARAPI_(AutoPtr<Cell>) DetectAndAddHit(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(void) AddCellToPattern(
        /* [in] */ Cell* newCell);

    CARAPI_(AutoPtr<Cell>) CheckForNewHit(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(Int32) GetRowHit(
        /* [in] */ Float y);

    CARAPI_(Int32) GetColumnHit(
        /* [in] */ Float x);

    CARAPI_(void) HandleActionMove(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) SendAccessEvent(
        /* [in] */ Int32 resId);

    CARAPI_(void) HandleActionUp(
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) HandleActionDown(
        /* [in] */ IMotionEvent* event);

    CARAPI_(Float) GetCenterXForColumn(
        /* [in] */ Int32 column);

    CARAPI_(Float) GetCenterYForRow(
        /* [in] */ Int32 row);

    CARAPI_(void) DrawArrow(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Float leftX,
        /* [in] */ Float topY,
        /* [in] */ Cell* start,
        /* [in] */ Cell* end);

    CARAPI DrawCircle(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Int32 leftX,
        /* [in] */ Int32 topY,
        /* [in] */ Boolean partOfPattern);

protected:
    LockPatternView();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    virtual CARAPI_(Int32) GetSuggestedMinimumWidth();

    virtual CARAPI_(Int32) GetSuggestedMinimumHeight();

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    static const String TAG;
    static const Int32 ASPECT_SQUARE = 0;
    static const Int32 ASPECT_LOCK_WIDTH = 1;
    static const Int32 ASPECT_LOCK_HEIGHT = 2;
    static const Int32 STATUS_BAR_HEIGHT = 25;
    static const Int32 MILLIS_PER_CIRCLE_ANIMATING = 700;
    static const Boolean PROFILE_DRAWING = FALSE;

    Boolean mDrawingProfilingStarted;// = False;

    AutoPtr<IPaint> mPaint;// = new Paint();
    AutoPtr<IPaint> mPathPaint;// = new Paint();
    AutoPtr<IOnPatternListener> mOnPatternListener;
    AutoPtr< ArrayOf<Cell*> > mPattern;// = new ArrayList<Cell>(9);

    Boolean mPatternDrawLookup[3][3];

    Float mInProgressX;// = -1;
    Float mInProgressY;// = -1;

    Int64 mAnimatingPeriodStart;

    DisplayMode mPatternDisplayMode;// = DisplayMode::Correct;
    Boolean mInputEnabled;// = TRUE;
    Boolean mInStealthMode;// = False;
    Boolean mEnableHapticFeedback;// = TRUE;
    Boolean mPatternInProgress;// = False;

    Float mDiameterFactor;// = 0.10f; // TODO: move to attrs
    Int32 mStrokeAlpha;// = 128;
    Float mHitFactor;// = 0.6f;

    Float mSquareWidth;
    Float mSquareHeight;

    AutoPtr<IBitmap> mBitmapBtnDefault;
    AutoPtr<IBitmap> mBitmapBtnTouched;
    AutoPtr<IBitmap> mBitmapCircleDefault;
    AutoPtr<IBitmap> mBitmapCircleGreen;
    AutoPtr<IBitmap> mBitmapCircleRed;

    AutoPtr<IBitmap> mBitmapArrowGreenUp;
    AutoPtr<IBitmap> mBitmapArrowRedUp;

    AutoPtr<IPath> mCurrentPath;// = new Path();
    AutoPtr<CRect> mInvalidate;// = new Rect();

    Int32 mBitmapWidth;
    Int32 mBitmapHeight;

    Int32 mAspect;
    AutoPtr<IMatrix> mArrowMatrix;// = new Matrix();
    AutoPtr<IMatrix> mCircleMatrix;// = new Matrix();

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_LOCKPATTERNVIEW_H__
