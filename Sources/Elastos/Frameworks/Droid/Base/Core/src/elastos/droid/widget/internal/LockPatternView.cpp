#include "elastos/droid/R.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include "elastos/droid/widget/internal/LockPatternView.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CMatrix.h"
#include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/view/accessibility/CAccessibilityManager.h"

using Elastos::Core::Math;

using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::View::IHapticFeedbackConstants;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

const String LockPatternView::TAG("LockPatternView");
const Int32 LockPatternView::ASPECT_SQUARE;
const Int32 LockPatternView::ASPECT_LOCK_WIDTH;
const Int32 LockPatternView::ASPECT_LOCK_HEIGHT;
const Int32 LockPatternView::STATUS_BAR_HEIGHT;
const Int32 LockPatternView::MILLIS_PER_CIRCLE_ANIMATING;
const Boolean LockPatternView::PROFILE_DRAWING;

LockPatternView::LockPatternView()
    : mDrawingProfilingStarted(FALSE)
    , mInProgressX(-1)
    , mInProgressY(-1)
    , mAnimatingPeriodStart(0)
    , mPatternDisplayMode(DisplayMode_Correct)
    , mInputEnabled(TRUE)
    , mInStealthMode(FALSE)
    , mEnableHapticFeedback(TRUE)
    , mPatternInProgress(FALSE)
    , mDiameterFactor(0.10f)
    , mStrokeAlpha(128)
    , mHitFactor(0.6f)
    , mSquareWidth(0)
    , mSquareHeight(0)
    , mBitmapWidth(8)
    , mBitmapHeight(0)
    , mAspect(0)
{
    CPaint::New((IPaint**)&mPaint);
    CPaint::New((IPaint**)&mPathPaint);
    mPattern = ArrayOf<Cell*>::Alloc(9);
    memset(mPatternDrawLookup, 0, sizeof(mPatternDrawLookup));
    CPaint::New((IPaint**)&mCurrentPath);
    CRect::NewByFriend((CRect**)&mInvalidate);
    CMatrix::New((IMatrix**)&mArrowMatrix);
    CMatrix::New((IMatrix**)&mCircleMatrix);
}

LockPatternView::LockPatternView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mDrawingProfilingStarted(FALSE)
    , mInProgressX(-1)
    , mInProgressY(-1)
    , mAnimatingPeriodStart(0)
    , mPatternDisplayMode(DisplayMode_Correct)
    , mInputEnabled(TRUE)
    , mInStealthMode(FALSE)
    , mEnableHapticFeedback(TRUE)
    , mPatternInProgress(FALSE)
    , mDiameterFactor(0.10f)
    , mStrokeAlpha(128)
    , mHitFactor(0.6f)
    , mSquareWidth(0)
    , mSquareHeight(0)
    , mBitmapWidth(8)
    , mBitmapHeight(0)
    , mAspect(0)
{
    CPaint::New((IPaint**)&mPaint);
    CPaint::New((IPaint**)&mPathPaint);
    mPattern = ArrayOf<Cell*>::Alloc(9);
    memset(mPatternDrawLookup, 0, sizeof(mPatternDrawLookup));
    CPaint::New((IPaint**)&mCurrentPath);
    CRect::NewByFriend((CRect**)&mInvalidate);
    CMatrix::New((IMatrix**)&mArrowMatrix);
    CMatrix::New((IMatrix**)&mCircleMatrix);

    Init(context, attrs);
}

ECode LockPatternView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    View::Init(context, attrs);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::LockPatternView),
            ArraySize(R::styleable::LockPatternView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);
    String aspect;
    a->GetString(R::styleable::LockPatternView_aspect, &aspect);

    if (aspect.Equals("square")) {
        mAspect = ASPECT_SQUARE;
    } else if (aspect.Equals("lock_width")) {
        mAspect = ASPECT_LOCK_WIDTH;
    } else if (aspect.Equals("lock_height")) {
        mAspect = ASPECT_LOCK_HEIGHT;
    } else {
        mAspect = ASPECT_SQUARE;
    }

    SetClickable(TRUE);

    mPathPaint->SetAntiAlias(TRUE);
    mPathPaint->SetDither(TRUE);
    mPathPaint->SetColor(IColor::WHITE);
    mPathPaint->SetAlpha(mStrokeAlpha);
    mPathPaint->SetStyle(PaintStyle_STROKE);
    mPathPaint->SetStrokeJoin(PaintJoin_ROUND);
    mPathPaint->SetStrokeCap(PaintCap_ROUND);

    mBitmapBtnDefault = GetBitmapFor(R::drawable::btn_code_lock_default_holo);
    mBitmapBtnTouched = GetBitmapFor(R::drawable::btn_code_lock_touched_holo);
    mBitmapCircleDefault = GetBitmapFor(R::drawable::indicator_code_lock_point_area_default_holo);
    mBitmapCircleGreen = GetBitmapFor(R::drawable::indicator_code_lock_point_area_green_holo);
    mBitmapCircleRed = GetBitmapFor(R::drawable::indicator_code_lock_point_area_red_holo);
    mBitmapArrowGreenUp = GetBitmapFor(R::drawable::indicator_code_lock_drag_direction_green_up);
    mBitmapArrowRedUp = GetBitmapFor(R::drawable::indicator_code_lock_drag_direction_red_up);

    AutoPtr< ArrayOf<IBitmap*> > bitmaps = ArrayOf<IBitmap*>::Alloc(5);
    bitmaps->Set(0, mBitmapBtnDefault);
    bitmaps->Set(1, mBitmapBtnTouched);
    bitmaps->Set(2, mBitmapCircleDefault);
    bitmaps->Set(3, mBitmapCircleGreen);
    bitmaps->Set(4, mBitmapCircleRed);

    for (Int32 i = 0; i < bitmaps->GetLength(); i++) {
        AutoPtr<IBitmap> bm = (*bitmaps)[i];
        Int32 width = 0, height = 0;
        bm->GetWidth(&width);
        bm->GetHeight(&height);
        mBitmapWidth = Elastos::Core::Math::Max(mBitmapWidth, width);
        mBitmapHeight = Elastos::Core::Math::Max(mBitmapHeight, height);
    }
    return NOERROR;
}

Boolean LockPatternView::IsInStealthMode()
{
    return mInStealthMode;
}

Boolean LockPatternView::IsTactileFeedbackEnabled()
{
    return mEnableHapticFeedback;
}

ECode LockPatternView::SetInStealthMode(
    /* [in] */ Boolean inStealthMode)
{
    mInStealthMode = inStealthMode;
    return NOERROR;
}

ECode LockPatternView::SetTactileFeedbackEnabled(
    /* [in] */ Boolean tactileFeedbackEnabled)
{
    mEnableHapticFeedback = tactileFeedbackEnabled;
    return NOERROR;
}

ECode LockPatternView::SetOnPatternListener(
    /* [in] */ IOnPatternListener* onPatternListener)
{
    mOnPatternListener = onPatternListener;
    return NOERROR;
}

ECode LockPatternView::SetPattern(
    /* [in] */ DisplayMode displayMode,
    /* [in] */ IObjectContainer* pattern)
{
    mPattern->Release();
    AutoPtr<IObjectEnumerator> em;
    pattern->GetObjectEnumerator((IObjectEnumerator**)&em);
    Boolean res = FALSE;
    em->MoveNext(&res);
    Int32 i = 0;
    while(res) {
        AutoPtr<IInterface> cell;
        em->Current((IInterface**)&cell);
        mPattern->Set(i, (Cell*)cell.Get());
        i ++;
    }
    ClearPatternDrawLookup();
    for (Int32 i = 0; i < mPattern->GetLength(); i++) {
        mPatternDrawLookup[(*mPattern)[i]->GetRow()][(*mPattern)[i]->GetColumn()] = TRUE;
    }

    SetDisplayMode(displayMode);
    return NOERROR;
}

ECode LockPatternView::SetDisplayMode(
    /* [in] */ DisplayMode displayMode)
{
    mPatternDisplayMode = displayMode;
    if (displayMode == DisplayMode_Animate) {
        if (mPattern->GetLength() == 0) {
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        mAnimatingPeriodStart = SystemClock::GetElapsedRealtime();
        Cell* first = (*mPattern)[0];
        mInProgressX = GetCenterXForColumn(first->GetColumn());
        mInProgressY = GetCenterYForRow(first->GetRow());
        ClearPatternDrawLookup();
    }
    Invalidate();
    return NOERROR;
}

ECode LockPatternView::ClearPattern()
{
    ResetPattern();
    return NOERROR;
}

ECode LockPatternView::DisableInput()
{
    mInputEnabled = FALSE;
    return NOERROR;
}

ECode LockPatternView::EnableInput()
{
    mInputEnabled = TRUE;
    return NOERROR;
}

Boolean LockPatternView::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IAccessibilityManager> am;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&am);
    Boolean res = FALSE;
    if ((am->IsTouchExplorationEnabled(&res), res)) {
        Int32 action = 0;
        event->GetAction(&action);
        switch (action) {
            case IMotionEvent::ACTION_HOVER_ENTER:
                event->SetAction(IMotionEvent::ACTION_DOWN);
                break;
            case IMotionEvent::ACTION_HOVER_MOVE:
                event->SetAction(IMotionEvent::ACTION_MOVE);
                break;
            case IMotionEvent::ACTION_HOVER_EXIT:
                event->SetAction(IMotionEvent::ACTION_UP);
                break;
        }
        OnTouchEvent(event);
        event->SetAction(action);
    }
    return View::OnHoverEvent(event);
}


Boolean LockPatternView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (!mInputEnabled || !IsEnabled()) {
        return FALSE;
    }
    Int32 action = 0;
    event->GetAction(&action);
    switch (action) {
        case IMotionEvent::ACTION_DOWN:
            HandleActionDown(event);
            return TRUE;
        case IMotionEvent::ACTION_UP:
            HandleActionUp(event);
            return TRUE;
        case IMotionEvent::ACTION_MOVE:
            HandleActionMove(event);
            return TRUE;
        case IMotionEvent::ACTION_CANCEL:
            if (mPatternInProgress) {
                mPatternInProgress = FALSE;
                ResetPattern();
                NotifyPatternCleared();
            }
            if (PROFILE_DRAWING) {
                if (mDrawingProfilingStarted) {
                    //Debug.stopMethodTracing();
                    mDrawingProfilingStarted = FALSE;
                }
            }
            return TRUE;
    }
    return FALSE;
}

AutoPtr<IBitmap> LockPatternView::GetBitmapFor(
    /* [in] */ Int32 resId)
{
    AutoPtr<IBitmapFactory> factory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
    AutoPtr<IResources> res;
    GetContext()->GetResources((IResources**)&res);
    AutoPtr<IBitmap> bp;
    factory->DecodeResource(res, resId, (IBitmap**)&bp);
    return bp;
}

void LockPatternView::NotifyCellAdded()
{
    SendAccessEvent(R::string::lockscreen_access_pattern_cell_added);
    if (mOnPatternListener != NULL) {
        AutoPtr<IObjectContainer> contain;
        CObjectContainer::New((IObjectContainer**)&contain);
        for (Int32 i = 0; i < mPattern->GetLength(); ++i)
        {
            contain->Add((*mPattern)[i]);
        }
        mOnPatternListener->OnPatternCellAdded(contain);
    }
}

void LockPatternView::NotifyPatternStarted()
{
    SendAccessEvent(R::string::lockscreen_access_pattern_start);
    if (mOnPatternListener != NULL) {
        mOnPatternListener->OnPatternStart();
    }
}

void LockPatternView::NotifyPatternDetected()
{
    SendAccessEvent(R::string::lockscreen_access_pattern_detected);
    if (mOnPatternListener != NULL) {
        AutoPtr<IObjectContainer> contain;
        CObjectContainer::New((IObjectContainer**)&contain);
        for (Int32 i = 0; i < mPattern->GetLength(); ++i)
        {
            contain->Add((*mPattern)[i]);
        }
        mOnPatternListener->OnPatternCellAdded(contain);
    }
}

void LockPatternView::NotifyPatternCleared()
{
    SendAccessEvent(R::string::lockscreen_access_pattern_cleared);
    if (mOnPatternListener != NULL) {
        mOnPatternListener->OnPatternCleared();
    }
}

void LockPatternView::ResetPattern()
{
    mPattern->Release();
    ClearPatternDrawLookup();
    mPatternDisplayMode = DisplayMode_Correct;
    Invalidate();
}

void LockPatternView::ClearPatternDrawLookup()
{
    for (Int32 i = 0; i < 3; i++) {
        for (Int32 j = 0; j < 3; j++) {
            mPatternDrawLookup[i][j] = FALSE;
        }
    }
}

void LockPatternView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    Int32 width = w - mPaddingLeft - mPaddingRight;
    mSquareWidth = width / 3.0f;

    Int32 height = h - mPaddingTop - mPaddingBottom;
    mSquareHeight = height / 3.0f;
}

Int32 LockPatternView::ResolveMeasured(
    /* [in] */ Int32 measureSpec,
    /* [in] */ Int32 desired)
{
    Int32 result = 0;
    Int32 specSize = MeasureSpec::GetSize(measureSpec);
    switch (MeasureSpec::GetMode(measureSpec)) {
        case MeasureSpec::UNSPECIFIED:
            result = desired;
            break;
        case MeasureSpec::AT_MOST:
            result = Elastos::Core::Math::Max(specSize, desired);
            break;
        case MeasureSpec::EXACTLY:
        default:
            result = specSize;
    }
    return result;
}

AutoPtr<LockPatternView::Cell> LockPatternView::DetectAndAddHit(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    AutoPtr<Cell> cell = CheckForNewHit(x, y);
    if (cell != NULL) {

        AutoPtr<Cell> fillInGapCell;
        AutoPtr< ArrayOf<Cell*> > pattern = mPattern;
        if (pattern->GetLength() != 0) {
            AutoPtr<Cell> lastCell = (*pattern)[pattern->GetLength() - 1];
            Int32 dRow = cell->mRow - lastCell->mRow;
            Int32 dColumn = cell->mColumn - lastCell->mColumn;

            Int32 fillInRow = lastCell->mRow;
            Int32 fillInColumn = lastCell->mColumn;

            if (Elastos::Core::Math::Abs(dRow) == 2 && Elastos::Core::Math::Abs(dColumn) != 1) {
                fillInRow = lastCell->mRow + ((dRow > 0) ? 1 : -1);
            }

            if (Elastos::Core::Math::Abs(dColumn) == 2 && Elastos::Core::Math::Abs(dRow) != 1) {
                fillInColumn = lastCell->mColumn + ((dColumn > 0) ? 1 : -1);
            }

            fillInGapCell = Cell::Of(fillInRow, fillInColumn);
        }

        if (fillInGapCell && !mPatternDrawLookup[fillInGapCell->mRow][fillInGapCell->mColumn]) {
            AddCellToPattern(fillInGapCell);
        }
        AddCellToPattern(cell);
        if (mEnableHapticFeedback) {
            PerformHapticFeedback(IHapticFeedbackConstants::VIRTUAL_KEY,
                    IHapticFeedbackConstants::FLAG_IGNORE_VIEW_SETTING
                    | IHapticFeedbackConstants::FLAG_IGNORE_GLOBAL_SETTING);
        }
        return cell;
    }
    return NULL;
}

void LockPatternView::AddCellToPattern(
    /* [in] */ Cell* newCell)
{
    mPatternDrawLookup[newCell->GetRow()][newCell->GetColumn()] = TRUE;
    mPattern->Set(mPattern->GetLength(), newCell);
    NotifyCellAdded();
}

AutoPtr<LockPatternView::Cell> LockPatternView::CheckForNewHit(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Int32 rowHit = GetRowHit(y);
    if (rowHit < 0) {
        return NULL;
    }
    Int32 columnHit = GetColumnHit(x);
    if (columnHit < 0) {
        return NULL;
    }

    if (mPatternDrawLookup[rowHit][columnHit]) {
        return NULL;
    }
    return Cell::Of(rowHit, columnHit);
}

Int32 LockPatternView::GetRowHit(
    /* [in] */ Float y)
{
    Float squareHeight = mSquareHeight;
    Float hitSize = squareHeight * mHitFactor;

    Float offset = mPaddingTop + (squareHeight - hitSize) / 2.0f;
    for (Int32 i = 0; i < 3; i++) {

        Float hitTop = offset + squareHeight * i;
        if (y >= hitTop && y <= hitTop + hitSize) {
            return i;
        }
    }
    return -1;
}

Int32 LockPatternView::GetColumnHit(
    /* [in] */ Float x)
{
    Float squareWidth = mSquareWidth;
    Float hitSize = squareWidth * mHitFactor;

    Float offset = mPaddingLeft + (squareWidth - hitSize) / 2.0f;
    for (Int32 i = 0; i < 3; i++) {

        Float hitLeft = offset + squareWidth * i;
        if (x >= hitLeft && x <= hitLeft + hitSize) {
            return i;
        }
    }
    return -1;
}

void LockPatternView::HandleActionMove(
    /* [in] */ IMotionEvent* event)
{
    Int32 historySize = 0;
    event->GetHistorySize(&historySize);
    for (Int32 i = 0; i < historySize + 1; i++) {
        Float x, y;
        if (i < historySize) {
            event->GetHistoricalX(i, &x);
            event->GetHistoricalY(i, &y);
        } else {
            event->GetX(&x);
            event->GetY(&y);
        }

        Int32 patternSizePreHitDetect = mPattern->GetLength();
        AutoPtr<Cell> hitCell = DetectAndAddHit(x, y);
        Int32 patternSize = mPattern->GetLength();
        if (hitCell && patternSize == 1) {
            mPatternInProgress = TRUE;
            NotifyPatternStarted();
        }
        Float dx = Elastos::Core::Math::Abs(x - mInProgressX);
        Float dy = Elastos::Core::Math::Abs(y - mInProgressY);
        if (dx + dy > mSquareWidth * 0.01f) {
            Float oldX = mInProgressX;
            Float oldY = mInProgressY;

            mInProgressX = x;
            mInProgressY = y;

            if (mPatternInProgress && patternSize > 0) {
                AutoPtr<ArrayOf<Cell*> > pattern = mPattern;
                Float radius = mSquareWidth * mDiameterFactor * 0.5f;

                AutoPtr<Cell> lastCell = (*pattern)[patternSize - 1];

                Float startX = GetCenterXForColumn(lastCell->mColumn);
                Float startY = GetCenterYForRow(lastCell->mRow);

                Float left;
                Float top;
                Float right;
                Float bottom;

                AutoPtr<CRect> invalidateRect = mInvalidate;

                if (startX < x) {
                    left = startX;
                    right = x;
                } else {
                    left = x;
                    right = startX;
                }

                if (startY < y) {
                    top = startY;
                    bottom = y;
                } else {
                    top = y;
                    bottom = startY;
                }

                invalidateRect->Set((Int32) (left - radius), (Int32) (top - radius),
                        (Int32) (right + radius), (Int32) (bottom + radius));

                if (startX < oldX) {
                    left = startX;
                    right = oldX;
                } else {
                    left = oldX;
                    right = startX;
                }

                if (startY < oldY) {
                    top = startY;
                    bottom = oldY;
                } else {
                    top = oldY;
                    bottom = startY;
                }

                invalidateRect->Union((Int32) (left - radius), (Int32) (top - radius),
                        (Int32) (right + radius), (Int32) (bottom + radius));

                if (hitCell) {
                    startX = GetCenterXForColumn(hitCell->mColumn);
                    startY = GetCenterYForRow(hitCell->mRow);

                    if (patternSize >= 2) {
                        hitCell = (*pattern)[patternSize - 1 - (patternSize - patternSizePreHitDetect)];
                        oldX = GetCenterXForColumn(hitCell->mColumn);
                        oldY = GetCenterYForRow(hitCell->mRow);

                        if (startX < oldX) {
                            left = startX;
                            right = oldX;
                        } else {
                            left = oldX;
                            right = startX;
                        }

                        if (startY < oldY) {
                            top = startY;
                            bottom = oldY;
                        } else {
                            top = oldY;
                            bottom = startY;
                        }
                    } else {
                        left = right = startX;
                        top = bottom = startY;
                    }

                    Float widthOffset = mSquareWidth / 2.0f;
                    Float heightOffset = mSquareHeight / 2.0f;

                    invalidateRect->Set((Int32) (left - widthOffset),
                            (Int32) (top - heightOffset), (Int32) (right + widthOffset),
                            (Int32) (bottom + heightOffset));
                }

                Invalidate(invalidateRect);
            } else {
                Invalidate();
            }
        }
    }
}

void LockPatternView::SendAccessEvent(
    /* [in] */ Int32 resId)
{
    String str;
    mContext->GetString(resId, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);
    AnnounceForAccessibility(csq);
}

void LockPatternView::HandleActionUp(
    /* [in] */ IMotionEvent* event)
{
    if (mPattern->GetLength() != 0) {
        mPatternInProgress = FALSE;
        NotifyPatternDetected();
        Invalidate();
    }
    if (PROFILE_DRAWING) {
        if (mDrawingProfilingStarted) {
            //Debug.stopMethodTracing();
            mDrawingProfilingStarted = FALSE;
        }
    }
}

void LockPatternView::HandleActionDown(
    /* [in] */ IMotionEvent* event)
{
    ResetPattern();
    Float x, y;
    event->GetX(&x);
    event->GetY(&y);
    AutoPtr<Cell> hitCell = DetectAndAddHit(x, y);
    if (hitCell) {
        mPatternInProgress = TRUE;
        mPatternDisplayMode = DisplayMode_Correct;
        NotifyPatternStarted();
    } else if (mPatternInProgress) {
        mPatternInProgress = FALSE;
        NotifyPatternCleared();
    }
    if (hitCell) {
        Float startX = GetCenterXForColumn(hitCell->mColumn);
        Float startY = GetCenterYForRow(hitCell->mRow);

        Float widthOffset = mSquareWidth / 2.0f;
        Float heightOffset = mSquareHeight / 2.0f;

        Invalidate((Int32) (startX - widthOffset), (Int32) (startY - heightOffset),
                (Int32) (startX + widthOffset), (Int32) (startY + heightOffset));
    }
    mInProgressX = x;
    mInProgressY = y;
    if (PROFILE_DRAWING) {
        if (!mDrawingProfilingStarted) {
            //Debug.startMethodTracing("LockPatternDrawing");
            mDrawingProfilingStarted = TRUE;
        }
    }
}

Float LockPatternView::GetCenterXForColumn(
    /* [in] */ Int32 column)
{
    return mPaddingLeft + column * mSquareWidth + mSquareWidth / 2.0f;
}

Float LockPatternView::GetCenterYForRow(
    /* [in] */ Int32 row)
{
    return mPaddingTop + row * mSquareHeight + mSquareHeight / 2.0f;
}

void LockPatternView::DrawArrow(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Float leftX,
    /* [in] */ Float topY,
    /* [in] */ Cell* start,
    /* [in] */ Cell* end)
{
    Boolean green = mPatternDisplayMode != DisplayMode_Wrong;

    Int32 endRow = end->mRow;
    Int32 startRow = start->mRow;
    Int32 endColumn = end->mColumn;
    Int32 startColumn = start->mColumn;

    Int32 offsetX = ((Int32) mSquareWidth - mBitmapWidth) / 2;
    Int32 offsetY = ((Int32) mSquareHeight - mBitmapHeight) / 2;

    AutoPtr<IBitmap> arrow = green ? mBitmapArrowGreenUp : mBitmapArrowRedUp;
    Int32 cellWidth = mBitmapWidth;
    Int32 cellHeight = mBitmapHeight;

    Float theta = (Float) Elastos::Core::Math::Atan2((Double) (endRow - startRow), (Double) (endColumn - startColumn));
    Float angle = (Float) Elastos::Core::Math::ToDegrees(theta) + 90.0f;

    Float sx = Elastos::Core::Math::Min(mSquareWidth / mBitmapWidth, 1.0f);
    Float sy = Elastos::Core::Math::Min(mSquareHeight / mBitmapHeight, 1.0f);
    mArrowMatrix->SetTranslate(leftX + offsetX, topY + offsetY);
    mArrowMatrix->PreTranslate(mBitmapWidth/2, mBitmapHeight/2, &green);
    mArrowMatrix->PreScale(sx, sy, &green);
    mArrowMatrix->PreTranslate(-mBitmapWidth/2, -mBitmapHeight/2, &green);
    mArrowMatrix->PreRotate(angle, cellWidth / 2.0f, cellHeight / 2.0f, &green);
    Int32 width = 0;
    arrow->GetWidth(&width);
    mArrowMatrix->PreTranslate((cellWidth - width) / 2.0f, 0.0f, &green);
    canvas->DrawBitmap(arrow, mArrowMatrix, mPaint);
}

ECode LockPatternView::DrawCircle(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 leftX,
    /* [in] */ Int32 topY,
    /* [in] */ Boolean partOfPattern)
{
    AutoPtr<IBitmap> outerCircle;
    AutoPtr<IBitmap> innerCircle;

    if (!partOfPattern || (mInStealthMode && mPatternDisplayMode != DisplayMode_Wrong)) {
        outerCircle = mBitmapCircleDefault;
        innerCircle = mBitmapBtnDefault;
    } else if (mPatternInProgress) {
        outerCircle = mBitmapCircleGreen;
        innerCircle = mBitmapBtnTouched;
    } else if (mPatternDisplayMode == DisplayMode_Wrong) {
        outerCircle = mBitmapCircleRed;
        innerCircle = mBitmapBtnDefault;
    } else if (mPatternDisplayMode == DisplayMode_Wrong || mPatternDisplayMode == DisplayMode_Animate) {
        outerCircle = mBitmapCircleGreen;
        innerCircle = mBitmapBtnDefault;
    } else {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int32 width = mBitmapWidth;
    Int32 height = mBitmapHeight;

    Float squareWidth = mSquareWidth;
    Float squareHeight = mSquareHeight;

    Int32 offsetX = (Int32) ((squareWidth - width) / 2.0f);
    Int32 offsetY = (Int32) ((squareHeight - height) / 2.0f);

    Float sx = Elastos::Core::Math::Min(mSquareWidth / mBitmapWidth, 1.0f);
    Float sy = Elastos::Core::Math::Min(mSquareHeight / mBitmapHeight, 1.0f);
    Boolean res = FALSE;
    mCircleMatrix->SetTranslate(leftX + offsetX, topY + offsetY);
    mCircleMatrix->PreTranslate(mBitmapWidth/2, mBitmapHeight/2, &res);
    mCircleMatrix->PreScale(sx, sy, &res);
    mCircleMatrix->PreTranslate(-mBitmapWidth/2, -mBitmapHeight/2, &res);

    canvas->DrawBitmap(outerCircle, mCircleMatrix, mPaint);
    canvas->DrawBitmap(innerCircle, mCircleMatrix, mPaint);

    return NOERROR;
}

Int32 LockPatternView::GetSuggestedMinimumWidth()
{
    return 3 * mBitmapWidth;
}

Int32 LockPatternView::GetSuggestedMinimumHeight()
{
    return 3 * mBitmapWidth;
}

void LockPatternView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 minimumWidth = GetSuggestedMinimumWidth();
    Int32 minimumHeight = GetSuggestedMinimumHeight();
    Int32 viewWidth = ResolveMeasured(widthMeasureSpec, minimumWidth);
    Int32 viewHeight = ResolveMeasured(heightMeasureSpec, minimumHeight);

    switch (mAspect) {
        case ASPECT_SQUARE:
            viewWidth = viewHeight = Elastos::Core::Math::Min(viewWidth, viewHeight);
            break;
        case ASPECT_LOCK_WIDTH:
            viewHeight = Elastos::Core::Math::Min(viewWidth, viewHeight);
            break;
        case ASPECT_LOCK_HEIGHT:
            viewWidth = Elastos::Core::Math::Min(viewWidth, viewHeight);
            break;
    }
}

void LockPatternView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr< ArrayOf<Cell*> > pattern = mPattern;
    Int32 count = pattern->GetLength();

    if (mPatternDisplayMode == DisplayMode_Animate) {

        Int32 oneCycle = (count + 1) * MILLIS_PER_CIRCLE_ANIMATING;
        Int32 spotInCycle = (Int32) (SystemClock::GetElapsedRealtime() - mAnimatingPeriodStart) % oneCycle;
        Int32 numCircles = spotInCycle / MILLIS_PER_CIRCLE_ANIMATING;

        ClearPatternDrawLookup();
        for (Int32 i = 0; i < numCircles; i++) {
            AutoPtr<Cell> cell = (*pattern)[i];
            mPatternDrawLookup[cell->GetRow()][cell->GetColumn()] = TRUE;
        }

        Boolean needToUpdateInProgressPoint = numCircles > 0 && numCircles < count;

        if (needToUpdateInProgressPoint) {
            Float percentageOfNextCircle = ((Float) (spotInCycle % MILLIS_PER_CIRCLE_ANIMATING)) /
                            MILLIS_PER_CIRCLE_ANIMATING;

            AutoPtr<Cell> currentCell = (*pattern)[numCircles - 1];
            Float centerX = GetCenterXForColumn(currentCell->mColumn);
            Float centerY = GetCenterYForRow(currentCell->mRow);

            AutoPtr<Cell> nextCell = (*pattern)[numCircles];
            Float dx = percentageOfNextCircle * (GetCenterXForColumn(nextCell->mColumn) - centerX);
            Float dy = percentageOfNextCircle * (GetCenterYForRow(nextCell->mRow) - centerY);
            mInProgressX = centerX + dx;
            mInProgressY = centerY + dy;
        }
        Invalidate();
    }

    Float squareWidth = mSquareWidth;
    Float squareHeight = mSquareHeight;

    Float radius = (squareWidth * mDiameterFactor * 0.5f);
    mPathPaint->SetStrokeWidth(radius);

    AutoPtr<IPath> currentPath = mCurrentPath;
    currentPath->Rewind();

    Int32 paddingTop = mPaddingTop;
    Int32 paddingLeft = mPaddingLeft;

    for (Int32 i = 0; i < 3; i++) {
        Float topY = paddingTop + i * squareHeight;
        for (Int32 j = 0; j < 3; j++) {
            Float leftX = paddingLeft + j * squareWidth;
            DrawCircle(canvas, (Int32) leftX, (Int32) topY, mPatternDrawLookup[i][j]);
        }
    }

    Boolean drawPath = (!mInStealthMode || mPatternDisplayMode == DisplayMode_Wrong);
    Int32 flag = 0;
    Boolean oldFlag = ((mPaint->GetFlags(&flag), flag) & IPaint::FILTER_BITMAP_FLAG) != 0;
    mPaint->SetFilterBitmap(TRUE);
    if (drawPath) {
        for (Int32 i = 0; i < count - 1; i++) {
            AutoPtr<Cell> cell = (*pattern)[i];
            AutoPtr<Cell> next = (*pattern)[i + 1];

            if (!mPatternDrawLookup[next->mRow][next->mColumn]) {
                break;
            }

            Float leftX = paddingLeft + cell->mColumn * squareWidth;
            Float topY = paddingTop + cell->mRow * squareHeight;

            DrawArrow(canvas, leftX, topY, cell, next);
        }
    }

    if (drawPath) {
        Boolean anyCircles = FALSE;
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<Cell> cell = (*pattern)[i];

            if (!mPatternDrawLookup[cell->mRow][cell->mColumn]) {
                break;
            }
            anyCircles = TRUE;

            Float centerX = GetCenterXForColumn(cell->mColumn);
            Float centerY = GetCenterYForRow(cell->mRow);
            if (i == 0) {
                currentPath->MoveTo(centerX, centerY);
            } else {
                currentPath->LineTo(centerX, centerY);
            }
        }

        if ((mPatternInProgress || mPatternDisplayMode == DisplayMode_Animate)
                && anyCircles) {
            currentPath->LineTo(mInProgressX, mInProgressY);
        }
        canvas->DrawPath(currentPath, mPathPaint);
    }

    mPaint->SetFilterBitmap(oldFlag);
}

AutoPtr<IParcelable> LockPatternView::OnSaveInstanceState()
{
    /*Parcelable superState = super.onSaveInstanceState();
    return new SavedState(superState,
            LockPatternUtils.patternToString(mPattern),
            mPatternDisplayMode.ordinal(),
            mInputEnabled, mInStealthMode, mEnableHapticFeedback);*/
    return NOERROR;
}

void LockPatternView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    /*final SavedState ss = (SavedState) state;
    super.onRestoreInstanceState(ss.getSuperState());
    setPattern(
            DisplayMode.Correct,
            LockPatternUtils.stringToPattern(ss.getSerializedPattern()));
    mPatternDisplayMode = DisplayMode.values()[ss.getDisplayMode()];
    mInputEnabled = ss.isInputEnabled();
    mInStealthMode = ss.isInStealthMode();
    mEnableHapticFeedback = ss.isTactileFeedbackEnabled();*/
}

/////////////////////////////////////////////////////////////
//                  LockPatternView::Cell
/////////////////////////////////////////////////////////////
// AutoPtr< ArrayOf< AutoPtr <ArrayOf< LockPatternView::Cell*> > > > InitStatic ()
// {
//     AutoPtr< ArrayOf< AutoPtr< ArrayOf< LockPatternView::Cell*> > > > temp = ArrayOf< AutoPtr< ArrayOf<LockPatternView::Cell*> > >::Alloc(3);

//     for (Int32 i = 0; i < 3 ; i++) {
//             AutoPtr< ArrayOf<LockPatternView::Cell*> > inner = ArrayOf<LockPatternView::Cell*>::Alloc(3);
//             temp->Set(i, inner);
//         for (Int32 j = 0; j < 3; j++) {
//             AutoPtr<LockPatternView::Cell> cell = new LockPatternView::Cell(i, j);
//             inner->Set(j, cell);
//         }
//     }

//     return temp;
// }

// AutoPtr< ArrayOf< AutoPtr< ArrayOf< LockPatternView::Cell* > > > > LockPatternView::Cell::sCells = InitStatic();

Boolean static InitStatic()
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            LockPatternView::Cell::sCells[i][j] = new LockPatternView::Cell(i, j);
        }
    }
    return TRUE;
}

Boolean LockPatternView::Cell::sInit = InitStatic();
AutoPtr<LockPatternView::Cell> LockPatternView::Cell::sCells[3][3];

CAR_INTERFACE_IMPL(LockPatternView::Cell, IInterface)

LockPatternView::Cell::Cell(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column)
{
    CheckRange(row, column);
    mRow = row;
    mColumn = column;
}

ECode LockPatternView::Cell::CheckRange(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column)
{
    if (row < 0 || row > 2) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (column < 0 || column > 2) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

Int32 LockPatternView::Cell::GetRow()
{
    return mRow;
}

Int32 LockPatternView::Cell::GetColumn()
{
    return mColumn;
}

AutoPtr<LockPatternView::Cell> LockPatternView::Cell::Of(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column)
{
    CheckRange(row, column);
    AutoPtr<Cell> temp = sCells[row][column];
    return temp;
}

String LockPatternView::Cell::ToString()
{
    String row = StringUtils::ToString(mRow);
    String column = StringUtils::ToString(mColumn);
    return String("(row=") + row + String(",clmn=") + column + String(")");
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

