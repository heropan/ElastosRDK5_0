
#include "elastos/droid/packages/launcher2/DragController.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

DragController::ScrollRunnable::ScrollRunnable(
    /* [in] */ DragController* host)
    : mDirection(0)
    , mHost(host)
{
}

ECode DragController::ScrollRunnable::Run()
{
    if (mHost->mDragScroller != NULL) {
        if (mDirection == SCROLL_LEFT) {
            mHost->mDragScroller->ScrollLeft();
        }
        else {
            mHost->mDragScroller->ScrollRight();
        }
        mHost->mScrollState = SCROLL_OUTSIDE_ZONE;
        mHost->mDistanceSinceScroll = 0;
        mHost->mDragScroller->OnExitScrollArea();
        AutoPtr<IDragLayer> dragLayer;
        mHost->mLauncher->GetDragLayer((IDragLayer**)&dragLayer);
        dragLayer->OnExitScrollArea();

        Boolean res;
        mHost->IsDragging(&res);
        if (res) {
            // Check the scroll again so that we can requeue the scroller if necessary
            mHost->CheckScrollState((*mLastTouch)[0], (*mLastTouch)[1]);
        }
    }
    return NOERROR;
}

ECode DragController::ScrollRunnable::SetDirection(
    /* [in] */ Int32 direction)
{
    mDirection = direction;
    return NOERROR;
}

const String DragController::TAG("Launcher.DragController");

const Int32 DragController::SCROLL_DELAY = 500;
const Int32 DragController::RESCROLL_DELAY = 750;
const Int32 DragController::VIBRATE_DURATION = 15;

const Boolean DragController::PROFILE_DRAWING_DURING_DRAG = false;

const Int32 DragController::SCROLL_OUTSIDE_ZONE = 0;
const Int32 DragController::SCROLL_WAITING_IN_ZONE = 1;

const Float DragController::MAX_FLING_DEGREES = 35f;

CAR_INTERFACE_IMPL(DragController, Object, IDragController);

DragController::DragController()
{
    CRect::New((IRect**)&mRectTemp);
    mCoordinatesTemp = ArrayOf<Int32>::Alloc(2);
    mDragging = FALSE;
    mMotionDownX = 0;
    mMotionDownY = 0;
    mScrollZone = 0;
    CArrayList::New((IArrayList**)&mDropTargets);
    CArrayList::New((IArrayList**)&mListeners);
    mScrollState = SCROLL_OUTSIDE_ZONE;
    mScrollRunnable = new ScrollRunnable();
    mLastTouch = ArrayOf<Int32>::Alloc(2);
    mLastTouchUpTime = -1;
    mDistanceSinceScroll = 0;
    mTmpPoint = ArrayOf<Int32>::Alloc(2);
    CRect::New((IRect**)&mDragLayerRect);
    mFlingToDeleteThresholdVelocity = 0;
}

ECode DragController::constructor(
    /* [in] */ ILauncher* launcher)
{
    AutoPtr<IResources> r;
    launcher->GetResources((IResources**)&r);
    mLauncher = launcher;
    CHandler::New((Handler**)&mHandler);
    r->GetDimensionPixelSize(
            Elastos::Droid::Packages::Launcher2::R::dimen::scroll_zone,
            &mScrollZone);
    VelocityTracker::Obtain((IVelocityTracker**)&mVelocityTracker);
    AutoPtr<IInterface> obj;
    launcher->GetSystemService(IContext::VIBRATOR_SERVICE, (IInterface**)&obj);
    mVibrator = IVibrator::Probe(obj);

    AutoPtr<IDisplayMetrics> metrics;
    r->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float density;
    metrics->GetDensity(&density);

    Int32 value;
    r->GetInteger(
            Elastos::Droid::Packages::Launcher2::R::integer::config_flingToDeleteMinVelocity,
            &value);
    mFlingToDeleteThresholdVelocity = (Int32)(value * density);
    return NOERROR;
}

ECode DragController::Dragging(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDragging;
    return NOERROR;
}

ECode DragController::StartDrag(
    /* [in] */ IView* v,
    /* [in] */ IBitmap* bmp,
    /* [in] */ IDragSource* source,
    /* [in] */ IInterface* dragInfo,
    /* [in] */ Int32 dragAction,
    /* [in] */ IPoint* extraPadding,
    /* [in] */ Float initialDragViewScale)
{
    AutoPtr<ArrayOf<Int32> > loc = mCoordinatesTemp;
    AutoPtr<IDragLayer> dragLayer;
    mLauncher->GetDragLayer((IDragLayer**)&dragLayer);
    dragLayer->GetLocationInDragLayer(v, loc);
    Int32 viewExtraPaddingLeft;
    Int32 viewExtraPaddingTop;
    if (extraPadding != NULL) {
        extraPadding->GetX(&viewExtraPaddingLeft);
        extraPadding->GetY(&viewExtraPaddingTop);
    }
    else {
        viewExtraPaddingLeft = 0;
        viewExtraPaddingTop = 0;
    }
    Int32 left;
    v->GetPaddingLeft(&left);
    Int32 width;
    bmp->GetWidth(&width);
    Int32 dragLayerX = (*loc)[0] + left + viewExtraPaddingLeft +
            (Int32)((initialDragViewScale * width - width) / 2);

    Int32 top;
    v->GetPaddingTop(&top);
    Int32 height;
    bmp->GetHeight(&height);
    Int32 dragLayerY = (*loc)[1] + top + viewExtraPaddingTop +
            (Int32)((initialDragViewScale * height - height) / 2);

    StartDrag(bmp, dragLayerX, dragLayerY, source, dragInfo, dragAction, NULL,
            NULL, initialDragViewScale);

    if (dragAction == DRAG_ACTION_MOVE) {
        v->SetVisibility(IView::GONE);
    }
    return NOERROR;
}

ECode DragController::StartDrag(
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 dragLayerX,
    /* [in] */ Int32 dragLayerY,
    /* [in] */ IDragSource* source,
    /* [in] */ IInterface* dragInfo,
    /* [in] */ Int32 dragAction,
    /* [in] */ IPoint* dragOffset,
    /* [in] */ IRect* dragRegion,
    /* [in] */ Float initialDragViewScale)
{
    if (PROFILE_DRAWING_DURING_DRAG) {
        Debug::StartMethodTracing(String("Launcher"));
    }

    // Hide soft keyboard, if visible
    if (mInputMethodManager == NULL) {
        AutoPtr<IInterface> obj;
        mLauncher->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInterface**)&obj);
        mInputMethodManager = IInputMethodManager::Probe(obj);
    }
    mInputMethodManager->HideSoftInputFromWindow(mWindowToken, 0);

    Int32 size;
    mListeners->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mListeners->Get(i, (IInterface**)&obj);
        AutoPtr<IDragListener> listener = IDragListener::Probe(obj);

        listener->OnDragStart(source, dragInfo, dragAction);
    }

    Int32 registrationX = mMotionDownX - dragLayerX;
    Int32 registrationY = mMotionDownY - dragLayerY;

    Int23 left;
    Int32 dragRegionLeft = dragRegion == NULL ? 0 : (dragRegion->GetLeft(&left), left);
    Int32 top;
    Int32 dragRegionTop = dragRegion == NULL ? 0 : (dragRegion->GetTop(&top), top);

    mDragging = TRUE;

    mDragObject = new DropTarget::DragObject();

    mDragObject->mDragComplete = FALSE;
    mDragObject->mXOffset = mMotionDownX - (dragLayerX + dragRegionLeft);
    mDragObject->mYOffset = mMotionDownY - (dragLayerY + dragRegionTop);
    mDragObject->mDragSource = source;
    mDragObject->mDragInfo = dragInfo;

    mVibrator->Vibrate(VIBRATE_DURATION);

    Int32 width;
    b->GetWidth(&width);
    Int32 height;
    b->GetHeight(&height);
    AutoPtr<IDragView> dragView;
    CDragView::New(mLauncher, b, registrationX,
            registrationY, 0, 0, width, height, initialDragViewScale, (IDragView**)&dragView);
    mDragObject->mDragView = dragView;

    if (dragOffset != NULL) {
        AutoPtr<IPoint> p;
        CPoint::New(dragOffset, (IPoint**)&p);
        dragView->SetDragVisualizeOffset(p);
    }
    if (dragRegion != NULL) {
        AutoPtr<IRect> r;
        CRect::New(dragRegion, (IRect**)&r)
        dragView->SetDragRegion(r);
    }

    dragView->Show(mMotionDownX, mMotionDownY);
    return HandleMoveEvent(mMotionDownX, mMotionDownY);
}

ECode DragController::GetViewBitmap(
    /* [in] */ IView* v,
    /* [out] */ IBitmap** map)
{
    VALIDATE_NOT_NULL(map);

    v->ClearFocus();
    v->SetPressed(FALSE);

    Boolean willNotCache;
    v->WillNotCacheDrawing(&willNotCache);
    v->SetWillNotCacheDrawing(FALSE);

    // Reset the drawing cache background color to fully transparent
    // for the duration of this operation
    Int32 color;
    v->GetDrawingCacheBackgroundColor();
    v->SetDrawingCacheBackgroundColor(0);
    Float alpha;
    v->GetAlpha(&alpha);
    v->SetAlpha(1.0f);

    if (color != 0) {
        v->DestroyDrawingCache();
    }
    v->BuildDrawingCache();
    AutoPtr<IBitmap> cacheBitmap;
    v->GetDrawingCache((IBitmap**)&cacheBitmap);
    if (cacheBitmap == NULL) {
        //Slogger::E(TAG, "failed getViewBitmap(" + v + ")", new RuntimeException());
        Slogger::E(TAG, "failed getViewBitmap");
        *map = NULL;
        return NOERROR;
    }

    AutoPtr<IBitmap> bitmap;
    Bitmap::CreateBitmap(cacheBitmap, (IBitmap**)&bitmap);

    // Restore the view
    v->DestroyDrawingCache();
    v->SetAlpha(alpha);
    v->SetWillNotCacheDrawing(willNotCache);
    v->SetDrawingCacheBackgroundColor(color);

    *map = bitmap;
    REFCOUNT_ADD(*map);
    return NOERROR;
}

ECode DragController::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDragging;
    return NOERROR;
}

ECode DragController::IsDragging(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDragging;
    return NOERROR;
}

ECode DragController::CancelDrag()
{
    if (mDragging) {
        if (mLastDropTarget != NULL) {
            mLastDropTarget->OnDragExit(mDragObject);
        }
        mDragObject->mDeferDragViewCleanupPostAnimation = FALSE;
        mDragObject->mCancelled = TRUE;
        mDragObject->mDragComplete = TRUE;
        mDragObject->mDragSource->OnDropCompleted(NULL, mDragObject, FALSE, FALSE);
    }
    return EndDrag();
}

ECode DragController::OnAppsRemoved(
    /* [in] */ IArrayList* appInfos,
    /* [in] */ IContext* context)
{
    // Cancel the current drag if we are removing an app that we are dragging
    if (mDragObject != NULL) {
        AutoPtr<IInterface> rawDragInfo = mDragObject->mDragInfo;
        if (IShortcutInfo::Probe(rawDragInfo) != NULL) {
            AutoPtr<IShortcutInfo> dragInfo = IShortcutInfo::Probe(rawDragInfo);
            Int32 size;
            appInfos->GetSize(&size);
            for (Int32 i = 0; i < size; i++) {
                AutoPtr<IInterface> obj;
                appInfos->Get(i, (IInterface**)&obj);
                AutoPtr<IApplicationInfo> info = IApplicationInfo::Probe(obj);

                // Added null checks to prevent NPE we've seen in the wild
                if (dragInfo != NULL) {
                    AutoPtr<IIntent> intent;
                    dragInfo->GetIntent((IIntent**)&intent);
                    if (intent != NULL) {
                        AutoPtr<IComponentName> componentName;
                        intent->GetComponent((IComponentName**)&componentName);
                        AutoPtr<IComponentName> componentName2;
                        info->GetComponentName((IComponentName**)&componentName2);
                        Boolean isSameComponent;
                        componentName->Equals(componentName2, &isSameComponent);
                        if (isSameComponent) {
                            return CancelDrag();
                        }
                    }
                }
            }
        }
    }
    return NOERROR;
}

void DragController::EndDrag()
{
    if (mDragging) {
        mDragging = FALSE;
        ClearScrollRunnable();
        Boolean isDeferred = FALSE;
        if (mDragObject->mDragView != NULL) {
            isDeferred = mDragObject->mDeferDragViewCleanupPostAnimation;
            if (!isDeferred) {
                mDragObject->mDragView->Remove();
            }
            mDragObject->mDragView = NULL;
        }

        // Only end the drag if we are not deferred
        if (!isDeferred) {
            Int32 size;
            mListeners->GetSize(&size);
            for (Int32 i = 0; i < size; i++) {
                AutoPtr<IInterface> obj;
                mListeners->Get(i, (IInterface**)&obj);
                AutoPtr<IDragListener> listener = IDragListener::Probe(obj);
                listener->OnDragEnd();
            }
        }
    }

    ReleaseVelocityTracker();
}

ECode DragController::OnDeferredEndDrag(
    /* [in] */ IDragView* dragView)
{
    dragView->Remove();

    // If we skipped calling onDragEnd() before, do it now
    Int32 size;
    mListeners->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mListeners->Get(i, (IInterface**)&obj);
        AutoPtr<IDragListener> listener = IDragListener::Probe(obj);
        listener->OnDragEnd();
    }
}

ECode DragController::OnDeferredEndFling(
    /* [in] */ DragObject* d)
{
    return d->mDragSource->OnFlingToDeleteCompleted();
}

AutoPtr<ArrayOf<Int32> > DragController::GetClampedDragLayerPos(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    AutoPtr<IDragLayer> dragLayer;
    mLauncher->GetDragLayer((IDragLayer**)&dragLayer);
    dragLayer->GetLocalVisibleRect(mDragLayerRect);
    Int32 left;
    mDragLayerRect->GetLeft(&left);
    Int32 right;
    mDragLayerRect->GetRight(&right);
    (*mTmpPoint)[0] = (Int32)Math::Max(leftleft, Math::Min(x, right - 1));
    Int32 top;
    mDragLayerRect->GetTop(&top);
    Int32 bottom;
    mDragLayerRect->GetBottom(&bottom);
    (*mTmpPoint)[1] = (Int32)Math::Max(top, Math::Min(y, bottom - 1));
    return mTmpPoint;
}

ECode DragController::GetLastGestureUpTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = 0;

    if (mDragging) {
        *time = System::GetCurrentTimeMillis();
        return NOERROR;
    }
    else {
        *time = mLastTouchUpTime;
        return NOERROR;
    }
    return NOERROR;
}

ECode DragController::ResetLastGestureUpTime()
{
    mLastTouchUpTime = -1;
    return NOERROR;
}

ECode DragController::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    //@SuppressWarnings("all") // suppress dead code warning
    const Boolean debug = FALSE;
    if (debug) {
        // Slogger::D(Launcher::TAG, "DragController.onInterceptTouchEvent " + ev + " mDragging="
        //         + mDragging);
    }

    // Update the velocity tracker
    AcquireVelocityTrackerAndAddMovement(ev);

    Int32 action;
    ev->GetAction(&action);
    Int32 x;
    ev->GetX(&x);
    Int32 y;
    ev->GetY(&y);
    AutoPtr<ArrayOf<Int32> > dragLayerPos = GetClampedDragLayerPos(x, y);
    Int32 dragLayerX = (*dragLayerPos)[0];
    Int32 dragLayerY = (*dragLayerPos)[1];

    switch (action) {
        case IMotionEvent::ACTION_MOVE:
            break;
        case IMotionEvent::ACTION_DOWN:
            // Remember location of down touch
            mMotionDownX = dragLayerX;
            mMotionDownY = dragLayerY;
            mLastDropTarget = null;
            break;
        case IMotionEvent::ACTION_UP:
            mLastTouchUpTime = System::GetCurrentTimeMillis();
            if (mDragging) {
                AutoPtr<IPointF> vec = IsFlingingToDelete(mDragObject->mDragSource);
                if (vec != NULL) {
                    DropOnFlingToDeleteTarget(dragLayerX, dragLayerY, vec);
                }
                else {
                    Drop(dragLayerX, dragLayerY);
                }
            }
            EndDrag();
            break;
        case IMotionEvent::ACTION_CANCEL:
            CancelDrag();
            break;
    }

    *result = mDragging;
    return NOERROR;
}

ECode DragController::SetMoveTarget(
    /* [in] */ IView* view)
{
    mMoveTarget = view;
    return NOERROR;
}

ECode DragController::DispatchUnhandledMove(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean res;
    *result = mMoveTarget != NULL &&
            (mMoveTarget->DispatchUnhandledMove(focused, direction, &res), res);
    return NOERROR;
}

void DragController::ClearScrollRunnable()
{
    mHandler->RemoveCallbacks(mScrollRunnable);
    if (mScrollState == SCROLL_WAITING_IN_ZONE) {
        mScrollState = SCROLL_OUTSIDE_ZONE;
        mScrollRunnable->SetDirection(SCROLL_RIGHT);
        mDragScroller->OnExitScrollArea();
        AutoPtr<IDragLayer> dragLayer;
        mLauncher->GetDragLayer((IDragLayer**)&dragLayer);
        dragLayer->OnExitScrollArea();
    }
}

void DragController::HandleMoveEvent(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mDragObject->mDragView->Move(x, y);

    // Drop on someone?
    AutoPtr<ArrayOf<Int32> > coordinates = mCoordinatesTemp;
    AutoPtr<IDropTarget> dropTarget = FindDropTarget(x, y, coordinates);
    mDragObject->mX = (*coordinates)[0];
    mDragObject->mY = (*coordinates)[1];
    CheckTouchMove(dropTarget);

    // Check if we are hovering over the scroll areas
    mDistanceSinceScroll +=
        Math::Sqrt(Math::Pow((*mLastTouch)[0] - x, 2) + Math::Pow((*mLastTouch)[1] - y, 2));
    (*mLastTouch)[0] = x;
    (*mLastTouch)[1] = y;
    CheckScrollState(x, y);
}

ECode DragController::ForceTouchMove()
{
    AutoPtr<ArrayOf<Int32> > dummyCoordinates = mCoordinatesTemp;
    AutoPtr<IDropTarget> dropTarget = FindDropTarget((*mLastTouch)[0],
            (*mLastTouch)[1], dummyCoordinates);
    CheckTouchMove(dropTarget);
    return NOERROR;
}

void DragController::CheckTouchMove(
    /* [in] */ IDropTarget* dropTarget)
{
    if (dropTarget != NULL) {
        AutoPtr<IDropTarget> delegate;
        dropTarget->GetDropTargetDelegate(mDragObject, (IDropTarget**)&delegate);
        if (delegate != NULL) {
            dropTarget = delegate;
        }

        if (mLastDropTarget != dropTarget) {
            if (mLastDropTarget != NULL) {
                mLastDropTarget->OnDragExit(mDragObject);
            }
            dropTarget->OnDragEnter(mDragObject);
        }
        dropTarget->OnDragOver(mDragObject);
    }
    else {
        if (mLastDropTarget != NULL) {
            mLastDropTarget->OnDragExit(mDragObject);
        }
    }
    mLastDropTarget = dropTarget;
}

void DragController::CheckScrollState(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y)
{
    AutoPtr<IViewConfiguration> viewConfig;
    ViewConfiguration::Get(IContext::Prone(mLauncher), (IViewConfiguration**)&viewConfig);
    Int32 slop;
    viewConfig->GetScaledWindowTouchSlop(&slop);
    Int32 delay = mDistanceSinceScroll < slop ? RESCROLL_DELAY : SCROLL_DELAY;
    AutoPtr<IDragLayer> dragLayer;
    mLauncher->GetDragLayer((IDragLayer**)&dragLayer);

    Int32 direction;
    dragLayer->GetLayoutDirection(&direction);
    Boolean isRtl = (direction == IView::LAYOUT_DIRECTION_RTL);
    Int32 forwardDirection = isRtl ? SCROLL_RIGHT : SCROLL_LEFT;
    Int32 backwardsDirection = isRtl ? SCROLL_LEFT : SCROLL_RIGHT;

    Int32 width;
    if (x < mScrollZone) {
        if (mScrollState == SCROLL_OUTSIDE_ZONE) {
            mScrollState = SCROLL_WAITING_IN_ZONE;
            Boolean res;
            mDragScroller->OnEnterScrollArea(x, y, forwardDirection, &res);
            if (res) {
                dragLayer->OnEnterScrollArea(forwardDirection);
                mScrollRunnable->SetDirection(forwardDirection);
                mHandler->PostDelayed(mScrollRunnable, delay);
            }
        }
    }
    else if (x > (mScrollView->GetWidth(&width), width) - mScrollZone) {
        if (mScrollState == SCROLL_OUTSIDE_ZONE) {
            mScrollState = SCROLL_WAITING_IN_ZONE;
            Boolean res;
            mDragScroller->OnEnterScrollArea(x, y, backwardsDirection, &res);
            if (res) {
                dragLayer->OnEnterScrollArea(backwardsDirection);
                mScrollRunnable->SetDirection(backwardsDirection);
                mHandler->PostDelayed(mScrollRunnable, delay);
            }
        }
    }
    else {
        ClearScrollRunnable();
    }
}

ECode DragController::OnTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (!mDragging) {
        *result = FALSE;
        return NOERROR;
    }

    // Update the velocity tracker
    AcquireVelocityTrackerAndAddMovement(ev);

    Int32 action;
    ev->GetAction(&action);
    Int32 x;
    ev->GetX(&x);
    Int32 y;
    ev->GetY(&y);
    AutoPtr<ArrayOf<Int32> > dragLayerPos = GetClampedDragLayerPos(x, y);
    Int32 dragLayerX = (*dragLayerPos)[0];
    Int32 dragLayerY = (*dragLayerPos)[1];

    switch (action) {
        case IMotionEvent::ACTION_DOWN:
            // Remember where the motion event started
            mMotionDownX = dragLayerX;
            mMotionDownY = dragLayerY;

            Int32 width;

            if ((dragLayerX < mScrollZone) ||
                    (dragLayerX > (mScrollView->GetWidth(&width), width) - mScrollZone)) {
                mScrollState = SCROLL_WAITING_IN_ZONE;
                mHandler->PostDelayed(mScrollRunnable, SCROLL_DELAY);
            }
            else {
                mScrollState = SCROLL_OUTSIDE_ZONE;
            }
            break;
        case IMotionEvent::ACTION_MOVE:
            HandleMoveEvent(dragLayerX, dragLayerY);
            break;
        case IMotionEvent::ACTION_UP:
            // Ensure that we've processed a move event at the current pointer location.
            HandleMoveEvent(dragLayerX, dragLayerY);
            mHandler->RemoveCallbacks(mScrollRunnable);

            if (mDragging) {
                AutoPtr<IPointF> vec = IsFlingingToDelete(mDragObject->mDragSource);
                if (vec != NULL) {
                    DropOnFlingToDeleteTarget(dragLayerX, dragLayerY, vec);
                }
                else {
                    Drop(dragLayerX, dragLayerY);
                }
            }
            EndDrag();
            break;
        case IMotionEvent::ACTION_CANCEL:
            mHandler->RemoveCallbacks(mScrollRunnable);
            CancelDrag();
            break;
    }

    *result = TRUE;
    return NOERROR;
}

AutoPtr<IPointF> DragController::IsFlingingToDelete(
        /* [in] */ IDragSource* source)
{
    if (mFlingToDeleteDropTarget == NULL) return NULL;
    Boolean res;
    source->SupportsFlingToDelete(&res);
    if (!res) return NULL;

    AutoPtr<IViewConfiguration> config;
    ViewConfiguration::Get(mLauncher, (IViewConfiguration**)&config);
    Int32 maximumFlingVelocity;
    config->GetScaledMaximumFlingVelocity(&maximumFlingVelocity);
    mVelocityTracker->ComputeCurrentVelocity(1000, maximumFlingVelocity);

    Int32 yvelocity;
    mVelocityTracker->GetYVelocity(&yvelocity);
    if (velocity < mFlingToDeleteThresholdVelocity) {
        // Do a quick dot product test to ensure that we are flinging upwards
        Int32 xvelocity;
        mVelocityTracker->GetXVelocity(&xvelocity);
        AutoPtr<IPointF> vel;
        CPointF::New(xvelocity, yvelocity, (IPointF**)&vel);
        AutoPtr<IPointF> upVec;
        CPointF::New(0f, -1f, (IPointF**)&upVec);

        Int32 vx;
        vel->GetX(&vx);
        Int32 vy;
        vel->GetY(&vy);
        Int32 ux;
        upVec->GetX(&ux);
        Int32 uy;
        upVec->GetY(&uy);
        Int32 vlength;
        vel->GetLength(&vlength);
        Int32 ulength;
        upVec->GetLength(&ulength);
        Float theta = (Float)Math::Acos(((vx * ux) + (vy * uy)) /
                (vlength * ulength));
        if (theta <= Math::ToRadians(MAX_FLING_DEGREES)) {
            return vel;
        }
    }
    return NULL;
}

void DragController::DropOnFlingToDeleteTarget(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPointF* vel)
{
    AutoPtr<ArrayOf<Int32> > coordinates = mCoordinatesTemp;

    mDragObject->mX = (*coordinates)[0];
    mDragObject->mY = (*coordinates)[1];

    // Clean up dragging on the target if it's not the current fling delete target otherwise,
    // start dragging to it.
    if (mLastDropTarget != NULL && mFlingToDeleteDropTarget != mLastDropTarget) {
        mLastDropTarget->OnDragExit(mDragObject);
    }

    // Drop onto the fling-to-delete target
    Boolean accepted = FALSE;
    mFlingToDeleteDropTarget->OnDragEnter(mDragObject);
    // We must set dragComplete to true _only_ after we "enter" the fling-to-delete target for
    // "drop"
    mDragObject->mDragComplete = TRUE;
    mFlingToDeleteDropTarget->OnDragExit(mDragObject);
    Boolean res;
    mFlingToDeleteDropTarget->AcceptDrop(mDragObject, &res);
    if (res) {
        mFlingToDeleteDropTarget->OnFlingToDelete(mDragObject,
                mDragObject->mX, mDragObject->mY, vel);
        accepted = TRUE;
    }
    mDragObject->mDragSource->OnDropCompleted(IView::Probe(mFlingToDeleteDropTarget),
            mDragObject, TRUE, accepted);
}

void DragController::Drop(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    AutoPtr<ArrayOf<Int32> > coordinates = mCoordinatesTemp;
    AutoPtr<IDropTarget> dropTarget = FindDropTarget((Int32)x, (Int32)y, coordinates);

    mDragObject->mX = (*coordinates)[0];
    mDragObject->mY = (*coordinates)[1];
    Boolean accepted = FALSE;
    if (dropTarget != NULL) {
        mDragObject->mDragComplete = TRUE;
        dropTarget->OnDragExit(mDragObject);
        Boolean res;
        dropTarget->AcceptDrop(mDragObject, &res);
        if (res) {
            dropTarget->OnDrop(mDragObject);
            accepted = TRUE;
        }
    }
}

AutoPtr<IDropTarget> DragController::FindDropTarget(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ ArrayOf<Int32>* dropCoordinates)
{
    AutoPtr<IRect> r = mRectTemp;

    AutoPtr<IArrayList> dropTargets = mDropTargets;
    Int32 count;
    dropTargets->GetSize(&count);
    for (Int32 i=count-1; i>=0; i--) {
        AutoPtr<IInterface> obj;
        dropTargets->Get(i, (IInterface**)&obj);
        AutoPtr<IDropTarget> target = IDropTarget::Probe(obj);
        Boolean res;
        target->IsDropEnabled(&res);
        if (!res)
            continue;

        target->GetHitRect(r);

        // Convert the hit rect to DragLayer coordinates
        target->GetLocationInDragLayer(dropCoordinates);
        Int32 left;
        target->GetLeft(&left);
        Int32 top;
        target->GetTop(&top);
        r->Offset(dropCoordinates[0] - left, dropCoordinates[1] - top);

        mDragObject->mX = x;
        mDragObject->mY = y;
        Boolean tmp;
        r->Contains(x, y, &tmp);
        if (tmp) {
            AutoPtr<IDropTarget> delegate;
            target->GetDropTargetDelegate(mDragObject, (IDropTarget**)&delegate);
            if (delegate != NULL) {
                target = delegate;
                target->GetLocationInDragLayer(dropCoordinates);
            }

            // Make dropCoordinates relative to the DropTarget
            (*dropCoordinates)[0] = x - (*dropCoordinates)[0];
            (*dropCoordinates)[1] = y - (*dropCoordinates)[1];

            return target;
        }
    }
    return NULL;
}

ECode DragController::SetDragScoller(
    /* [in] */ IDragScroller* scroller)
{
    mDragScroller = scroller;
    return NOERROR;
}

ECode DragController::SetWindowToken(
    /* [in] */ IBinder* token)
{
    mWindowToken = token;
    return NOERROR;
}

ECode DragController::AddDragListener(
    /* [in] */ IDragListener* l)
{
    return mListeners->Add(l);
}

ECode DragController::RemoveDragListener(
    /* [in] */ IDragListener* l)
{
    return mListeners->Remove(l);
}

ECode DragController::AddDropTarget(
    /* [in] */ IDropTarget* target)
{
    return mDropTargets->Add(target);
}

ECode DragController::RemoveDropTarget(
    /* [in] */ IDropTarget* target)
{
    return mDropTargets->Remove(target);
}

ECode DragController::SetFlingToDeleteDropTarget(
    /* [in] */ IDropTarget* target)
{
     mFlingToDeleteDropTarget = target;
     return NOERROR;
}

void DragController::AcquireVelocityTrackerAndAddMovement(
    /* [in] */ IMotionEvent* ev)
{
    if (mVelocityTracker == NULL) {
        VelocityTracker::Obtain((IVelocityTracker**)&mVelocityTracker);
    }
    mVelocityTracker->AddMovement(ev);
}

void DragController::ReleaseVelocityTracker()
{
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }
}

ECode DragController::SetScrollView(
    /* [in] */ IView* v)
{
    mScrollView = v;
    return NOERROR;
}

ECode DragController::GetDragView(
    /* [out] */ IDragView** view)
{
    VALIDATE_NOT_NULL(view);

    *view = mDragObject->mDragView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos