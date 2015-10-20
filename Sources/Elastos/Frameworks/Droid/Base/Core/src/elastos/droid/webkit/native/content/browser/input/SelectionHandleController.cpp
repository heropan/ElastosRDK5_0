
#include "elastos/droid/webkit/native/content/browser/input/SelectionHandleController.h"
#include "elastos/droid/webkit/native/content/browser/PositionObserver.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

// The following constants match the ones in
// third_party/WebKit/public/web/WebTextDirection.h
const Int32 SelectionHandleController::TEXT_DIRECTION_DEFAULT;
const Int32 SelectionHandleController::TEXT_DIRECTION_LTR;
const Int32 SelectionHandleController::TEXT_DIRECTION_RTL;

SelectionHandleController::SelectionHandleController(
    /* [in] */ IView* parent,
    /* [in] */ PositionObserver* positionObserver)
    : mAllowAutomaticShowing(TRUE)
    , mIsShowing(FALSE)
    , mFixedHandleX(0)
    , mFixedHandleY(0)
    , mParent(parent)
    , mPositionObserver(positionObserver)
{
}

/** Automatically show selection anchors when text is selected. */
void SelectionHandleController::AllowAutomaticShowing()
{
    mAllowAutomaticShowing = TRUE;
}

/** Hide selection anchors, and don't automatically show them. */
void SelectionHandleController::HideAndDisallowAutomaticShowing()
{
    Hide();
    mAllowAutomaticShowing = FALSE;
}

//@Override
ECode SelectionHandleController::IsShowing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIsShowing;
    return NOERROR;
}

//@Override
ECode SelectionHandleController::Hide()
{
    if (mIsShowing) {
        if (mStartHandle != NULL) {
            mStartHandle->Hide();
        }

        if (mEndHandle != NULL) {
            mEndHandle->Hide();
        }

        mIsShowing = FALSE;
    }

    return NOERROR;
}

void SelectionHandleController::CancelFadeOutAnimation()
{
    Hide();
}

/**
 * Updates the selection for a movement of the given handle (which
 * should be the start handle or end handle) to coordinates x,y.
 * Note that this will not actually result in the handle moving to (x,y):
 * selectBetweenCoordinates(x1,y1,x2,y2) will trigger the selection and set the
 * actual coordinates later via set[Start|End]HandlePosition.
 */
//@Override
ECode SelectionHandleController::UpdatePosition(
    /* [in] */ HandleView* handle,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    SelectBetweenCoordinates(mFixedHandleX, mFixedHandleY, x, y);
    return NOERROR;
}

//@Override
ECode SelectionHandleController::BeforeStartUpdatingPosition(
    /* [in] */ HandleView* handle)
{
    AutoPtr<HandleView> fixedHandle = (handle == mStartHandle) ? mEndHandle : mStartHandle;
    mFixedHandleX = fixedHandle->GetAdjustedPositionX();
    mFixedHandleY = fixedHandle->GetLineAdjustedPositionY();
    return NOERROR;
}

/**
 * @return true iff this controller is being used to move the selection start.
 */
Boolean SelectionHandleController::IsSelectionStartDragged()
{
    return mStartHandle != NULL && mStartHandle->IsDragging();
}

/**
 * @return true iff this controller is being used to drag either the selection start or end.
 */
Boolean SelectionHandleController::IsDragging()
{
    return (mStartHandle != NULL && mStartHandle->IsDragging()) ||
           (mEndHandle != NULL && mEndHandle->IsDragging());
}

//@Override
ECode SelectionHandleController::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (!isInTouchMode) {
        Hide();
    }
    return NOERROR;
}

//@Override
ECode SelectionHandleController::OnDetached()
{
    return NOERROR;
}

/**
 * Moves the start handle so that it points at the given coordinates.
 * @param x The start handle position X in physical pixels.
 * @param y The start handle position Y in physical pixels.
 */
void SelectionHandleController::SetStartHandlePosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mStartHandle->PositionAt((Int32) x, (Int32) y);
}

/**
 * Moves the end handle so that it points at the given coordinates.
 * @param x The end handle position X in physical pixels.
 * @param y The end handle position Y in physical pixels.
 */
void SelectionHandleController::SetEndHandlePosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mEndHandle->PositionAt((Int32) x, (Int32) y);
}

/**
 * If the handles are not visible, sets their visibility to View.VISIBLE and begins fading them
 * in.
 */
void SelectionHandleController::BeginHandleFadeIn()
{
    mStartHandle->BeginFadeIn();
    mEndHandle->BeginFadeIn();
}

/**
 * Sets the start and end handles to the given visibility.
 */
void SelectionHandleController::SetHandleVisibility(
    /* [in] */ Int32 visibility)
{
    assert(0);
    // TODO
    // mStartHandle->SetVisibility(visibility);
    // mEndHandle->SetVisibility(visibility);
}

/**
 * Shows the handles if allowed.
 *
 * @param startDir Direction (left/right) of start handle.
 * @param endDir Direction (left/right) of end handle.
 */
void SelectionHandleController::OnSelectionChanged(
    /* [in] */ Int32 startDir,
    /* [in] */ Int32 endDir)
{
    if (mAllowAutomaticShowing) {
        ShowHandles(startDir, endDir);
    }
}

/**
 * Sets both start and end position and show the handles.
 * Note: this method does not trigger a selection, see
 * selectBetweenCoordinates()
 *
 * @param startDir Direction (left/right) of start handle.
 * @param endDir Direction (left/right) of end handle.
 */
void SelectionHandleController::ShowHandles(
    /* [in] */ Int32 startDir,
    /* [in] */ Int32 endDir)
{
    CreateHandlesIfNeeded(startDir, endDir);
    ShowHandlesIfNeeded();
}

//@VisibleForTesting
AutoPtr<HandleView> SelectionHandleController::GetStartHandleViewForTest()
{
    return mStartHandle;
}

//@VisibleForTesting
AutoPtr<HandleView> SelectionHandleController::GetEndHandleViewForTest()
{
    return mEndHandle;
}

void SelectionHandleController::CreateHandlesIfNeeded(
    /* [in] */ Int32 startDir,
    /* [in] */ Int32 endDir)
{
    if (mStartHandle == NULL) {
        mStartHandle = new HandleView(this,
                startDir == TEXT_DIRECTION_RTL ? HandleView::RIGHT : HandleView::LEFT, mParent,
                mPositionObserver);
    }

    if (mEndHandle == NULL) {
        mEndHandle = new HandleView(this,
                endDir == TEXT_DIRECTION_RTL ? HandleView::LEFT : HandleView::RIGHT, mParent,
                mPositionObserver);
    }
}

void SelectionHandleController::ShowHandlesIfNeeded()
{
    if (!mIsShowing) {
        mIsShowing = TRUE;
        mStartHandle->Show();
        mEndHandle->Show();
        SetHandleVisibility(IView::VISIBLE);
    }
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
