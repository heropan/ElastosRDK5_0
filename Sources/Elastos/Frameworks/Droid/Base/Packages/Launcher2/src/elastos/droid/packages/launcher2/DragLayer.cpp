
#include "elastos/droid/packages/launcher2/DragLayer.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

CAR_INTERFACE_IMPL(DragLayer::MyAnimatorUpdateListener, Object, IAnimatorUpdateListener);

MyAnimatorUpdateListener(
    /* [in] */ IDragView* view,
    /* [in] */ IInterpolator* alphaInterpolator,
    /* [in] */ IInterpolator* motionInterpolator,
    /* [in] */ Float initScaleX,
    /* [in] */ Float initScaleY,
    /* [in] */ Float dropViewScale,
    /* [in] */ Float finalScaleX,
    /* [in] */ Float finalScaleY,
    /* [in] */ Float finalAlpha,
    /* [in] */ Float initAlpha,
    /* [in] */ IRect* from,
    /* [in] */ IRect* to,
    /* [in] */ LayoutParams* host)
    : mView(view)
    , mAlphaInterpolator(alphaInterpolator)
    , mMotionInterpolator(motionInterpolator)
    , mInitScaleX(initScaleX)
    , mInitScaleY(initScaleY)
    , mDropViewScale(dropViewScale)
    , mFinalScaleX(finalScaleX)
    , mFinalScaleY(finalScaleY)
    , mFinalAlpha(finalAlpha)
    , mInitAlpha(initAlpha)
    , mFrom(from)
    , mTo(to)
    , mHost(host)
{
}

ECode DragLayer::MyAnimatorUpdateListener::OnAnimationUpdate(
    /* [in] */ IValueAnimator* animation)
{
    AutoPtr<IInterface> obj;
    animation->GetAnimatedValue((IInterface**)&obj);
    AutoPtr<IFloat> fValue = IFloat::Probe(obj);
    Float percent;
    fValue->GetValue(&percent);
    Int32 width;
    mView->GetMeasuredWidth(&width);
    Int32 height;
    mView->GetMeasuredHeight(&height);

    Float alphaPercent;
    if (mAlphaInterpolator == NULL) {
        alphaPercent = percent;
    }
    else {
        ITimeInterpolator::Probe(mAlphaInterpolator)->GetInterpolation(percent, &alphaPercent);
    }

    Float motionPercent;
    if (mMotionInterpolator == NULL) {
        motionPercent = percent;
    }
    else {
        ITimeInterpolator::Probe(mMotionInterpolator)->GetInterpolation(percent, &motionPercent);
    }

    Float initialScaleX = mInitScaleX * mDropViewScale;
    Float initialScaleY = mInitScaleY * mDropViewScale;
    Float scaleX = mFinalScaleX * percent + initialScaleX * (1 - percent);
    Float scaleY = mFinalScaleY * percent + initialScaleY * (1 - percent);
    Float alpha = mFinalAlpha * alphaPercent + mInitAlpha * (1 - alphaPercent);

    Int32 fromleft;
    mFrom->GetLeft(fromleft);
    Float fromLeft = fromleft + (initialScaleX - 1f) * width / 2;
    Int32 fromtop;
    mFrom->GetTop(&fromtop);
    Float fromTop = fromtop + (initialScaleY - 1f) * height / 2;

    Int32 toleft;
    mTo->GetLeft(&toleft);
    Int32 x = (Int32)(fromLeft + Math::Round(((toleft - fromLeft) * motionPercent)));
    Int32 totop;
    mTo->GetTop(&totop);
    Int32 y = (Int32)(fromTop + Math::Round(((totop - fromTop) * motionPercent)));

    Int32 sx;
    mHost->mDropView->GetScrollX(&sx);
    Int32 sx2;
    mHost->mAnchorView->GetScrollX(&sx2);
    Int32 xPos = x - sx + (mHost->mAnchorView != NULL
            ? (mHost->mAnchorViewInitialScrollX - sx2) : 0);
    Int32 sy;
    mHost->mDropView->GetScrollY(&sy);
    Int32 yPos = y - sy;

    mHost->mDropView->SetTranslationX(xPos);
    mHost->mDropView->SetTranslationY(yPos);
    mHost->mDropView->SetScaleX(scaleX);
    mHost->mDropView->SetScaleY(scaleY);
    return mHost->mDropView->SetAlpha(alpha);
}

ECode DragLayer::MyAnimatorListenerAdapter::MyAnimatorListenerAdapter(
    /* [in] */ IRunnable* onCompleteRunnable,
    /* [in] */ Int32 animationEndStyle,
    /* [in] */ LayoutParams* host)
    : mOnCompleteRunnable(onCompleteRunnable)
    , mAnimationEndStyle(animationEndStyle)
    , mHost(host)
{
}

ECode DragLayer::MyAnimatorListenerAdapter::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mOnCompleteRunnable != NULL) {
        mOnCompleteRunnable->Run();
    }
    switch (mAnimationEndStyle) {
        case ANIMATION_END_DISAPPEAR:
            mHost->ClearAnimatedView();
            break;
        case ANIMATION_END_FADE_OUT:
            mHost->FadeOutDragView();
            break;
        case ANIMATION_END_REMAIN_VISIBLE:
            break;
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(DragLayer::MyAnimatorUpdateListener2, Object, IAnimatorUpdateListener);

DragLayer::MyAnimatorUpdateListener2::MyAnimatorUpdateListener2(
    /* [in] */ LayoutParams* host)
    : mHost(host)
{
}

ECode DragLayer::MyAnimatorUpdateListener2::OnAnimationUpdate(
    /* [in] */ IValueAnimator* animation)
{
    AutoPtr<IInterface> obj;
    animation->GetAnimatedValue((IInterface**)&obj);
    AutoPtr<IFloat> fValue = IFloat::Probe(obj);
    Float percent;
    fValue->GetValue(&percent);

    Float alpha = 1 - percent;
    return mHost->mDropView->SetAlpha(alpha);
}

DragLayer::MyAnimatorListenerAdapter2::MyAnimatorListenerAdapter2(
    /* [in] */ LayoutParams* host)
    : mHost(host)
{
}

DragLayer::MyAnimatorListenerAdapter2::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mHost->mDropView != NULL) {
        mHost->mDragController->OnDeferredEndDrag(mDropView);
    }
    mHost->mDropView = NULL;
    return mHost->Invalidate();
}

DragLayer::LayoutParams::LayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : FrameLayout::LayoutParams(width, height);
    , mX(0)
    , mY(0)
    , mCustomPosition(FALSE)
{
}

ECode DragLayer::LayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode DragLayer::LayoutParams::GetWidth(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mWidth;
    return NOERROR;
}

ECode DragLayer::LayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

ECode DragLayer::LayoutParams::GetHeight(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mHeight;
    return NOERROR;
}

ECode DragLayer::LayoutParams::SetX(
    /* [in] */ Int32 x)
{
    mX = x;
    return NOERROR;
}

ECode DragLayer::LayoutParams::GetX(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mX;
    return NOERROR;
}

ECode DragLayer::LayoutParams::SetY(
    /* [in] */ Int32 y)
{
    mY = y;
    return NOERROR;
}

ECode DragLayer::LayoutParams::GetY(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mY;
    return NOERROR;
}

DragLayer::MyRunnable::MyRunnable(
    /* [in] */ IView* child,
    /* [in] */ IRunnable* onFinishAnimationRunnable)
    : mChild(child)
    , mOnFinishAnimationRunnable(onFinishAnimationRunnable)
{
}

ECode DragLayer::MyRunnable::Run()
{
    mChild->SetVisibility(VISIBLE);
    if (mOnFinishAnimationRunnable != NULL) {
        mOnFinishAnimationRunnable->Run();
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(DragLayer, FrameLayout, IDragLayer, IViewGroupOnHierarchyChangeListener);

DragLayer::DragLayer()
{
    mTmpXY = ArrayOf<Int32>::Alloc(2);
    mXDown = 0;
    mYDown = 0;
    CArrayList::New((IArrayList**)&mResizeFrames);
    CDecelerateInterpolator::New(1.5f, (ITimeInterpolator**)&mCubicEaseOutInterpolator);
    mAnchorViewInitialScrollX = 0;
    mHoverPointClosesFolder = FALSE;
    CRect::New((Rect**)&mHitRect);
    mWorkspaceIndex = -1;
    mQsbIndex = -1;
    mInScrollArea = FALSE;
}

ECode DragLayer::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FrameLayout::constructor(context, attrs);

    // Disable multitouch across the workspace/all apps/customize tray
    SetMotionEventSplittingEnabled(FALSE);
    SetChildrenDrawingOrderEnabled(TRUE);
    SetOnHierarchyChangeListener(this);

    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetDrawable(
            Elastos::Droid::Packages::Launcher2::R::drawable::page_hover_left_holo,
            (IDrawable**)&mLeftHoverDrawable);

    return resources->GetDrawable(
            Elastos::Droid::Packages::Launcher2::R::drawable::page_hover_right_holo,
            (IDrawable**)&mRightHoverDrawable);
}

ECode DragLayer::Setup(
    /* [in] */ ILauncher* launcher,
    /* [in] */ IDragController* controller)
{
    mLauncher = launcher;
    mDragController = controller;
    return NOERROR;
}

ECode DragLayer::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean res;
    mDragController->DispatchKeyEvent(event, &res);
    Boolean res2;
    FrameLayout::DispatchKeyEvent(event, &res2);
    *result = res || res2;
    return NOERROR;
}

Boolean DragLayer::IsEventOverFolderTextRegion(
    /* [in] */ IFolder* folder,
    /* [in] */ IMotionEvent* ev)
{
    AutoPtr<IView> view;
    folder->GetEditTextRegion((IView**)&view);
    GetDescendantRectRelativeToSelf(view, mHitRect);
    Float x;
    ev->GetX(&x);
    Float y;
    ev->GetY(&y);
    Boolean res;
    mHitRect->Contains((Int32)x, (Int32)y, &res);
    if (res) {
        return TRUE;
    }
    return FALSE;
}

Boolean DragLayer::IsEventOverFolder(
    /* [in] */ IFolder* folder,
    /* [in] */ IMotionEvent* ev)
{
    GetDescendantRectRelativeToSelf(folder, mHitRect);
    Float x;
    ev->GetX(&x);
    Float y;
    ev->GetY(&y);
    Boolean res;
    Boolean res;
    mHitRect->Contains((Int32)x, (Int32)y, &res);
    if (res) {
        return TRUE;
    }
    return FALSE;
}

Boolean DragLayer::HandleTouchDown(
    /* [in] */ IMotionEvent* ev,
    /* [in] */ Boolean intercept)
{
    AutoPtr<IRect> hitRect;
    CRect::New((IRect**)&hitRect);
    Float x;
    ev->GetX(&x);
    Int32 x = (Int32)x);
    Float y;
    ev->GetY(&y);
    Int32 y = (Int32)y;

    Int32 size;
    mResizeFrames->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mResizeFrames->Get(i, (IInterface**)&obj);
        AutoPtr<IAppWidgetResizeFrame> child = IAppWidgetResizeFrame::Probe(obj);

        child->GetHitRect(hitRect);
        Boolean res;
        hitRect->Contains(x, y, &res);
        if (res) {
            Int32 left;
            child->GetLeft(&left);
            Int32 top;
            child->GetTop(&top);
            Boolean tmp;
            child->BeginResizeIfPointInRegion(x - left, y - top, &tmp);
            if (tmp) {
                mCurrentResizeFrame = child;
                mXDown = x;
                mYDown = y;
                RequestDisallowInterceptTouchEvent(TRUE);
                return TRUE;
            }
        }
    }

    AutoPtr<IWorkspace> workspace;
    mLauncher->GetWorkspace((IWorkspace**)&workspace);
    AutoPtr<IFolder> currentFolder;
    workspace->GetOpenFolder((IFolder**)&currentFolder);
    if (currentFolder != NULL) {
        Boolean res;
        mLauncher->IsFolderClingVisible(&res);
        if (!res  && intercept) {
            Int32 res2;
            currentFolder->IsEditingName(&res2);
            if (res2) {
                if (!IsEventOverFoIderTextRegion(currentFolder, ev)) {
                    currentFolder->DismissEditingName();
                    return TRUE;
                }
            }

            GetDescendantRectRelativeToSelf(currentFolder, hitRect);
            if (!IsEventOverFolder(currentFolder, ev)) {
                mLauncher->CloseFolder();
                return TRUE;
            }
        }
    }
    return FALSE;
}

ECode DragLayer::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 action;
    ev->GetAction(&action);
    if (action == IMotionEvent::ACTION_DOWN) {
        if (HandleTouchDown(ev, TRUE)) {
            *result = TRUE;
            return NOERROR;
        }
    }
    ClearAllResizeFrames();
    return mDragController->OnInterceptTouchEvent(ev, result);
}

ECode DragLayer::OnInterceptHoverEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mLauncher == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<IWorkspace> workspace;
    mLauncher->GetWorkspace((IWorkspace**)&workspace);
    if (workspace == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<IFolder> currentFolder;
    workspace->GetOpenFolder((IFolder**)&currentFolder);
    if (currentFolder == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    else {
        AutoPtr<IContext> context;
        GetContext((IContext**)&context);
        AutoPtr<IInterface> obj;
        context->GetSystemService(IContext::ACCESSIBILITY_SERVICE, (IInterface**)&obj);
        AutoPtr<IAccessibilityManager> accessibilityManager = IAccessibilityManager::Probe(obj);

        Boolean res;
        accessibilityManager->IsTouchExplorationEnabled(&res);
        if (res) {
            Int32 action;
            ev->GetAction(&action);
            Boolean isOverFolder;
            switch (action) {
                case IMotionEvent::ACTION_HOVER_ENTER:
                {
                    isOverFolder = IsEventOverFolder(currentFolder, ev);
                    if (!isOverFolder) {
                        Boolean tmp;
                        currentFolder->IsEditingName(&tmp);
                        SendTapOutsideFolderAccessibilityEvent(tmp);
                        mHoverPointClosesFolder = TRUE;
                        *result = TRUE;
                        return NOERROR;
                    }
                    else if (isOverFolder) {
                        mHoverPointClosesFolder = FALSE;
                    }
                    else {
                        *result = TRUE;
                        return NOERROR;
                    }
                }
                case IMotionEvent::ACTION_HOVER_MOVE:
                {
                    isOverFolder = IsEventOverFolder(currentFolder, ev);
                    if (!isOverFolder && !mHoverPointClosesFolder) {
                        Boolean tmp;
                        currentFolder->IsEditingName(&tmp);
                        SendTapOutsideFolderAccessibilityEvent(tmp);
                        mHoverPointClosesFolder = TRUE;
                        *result = TRUE;
                        return NOERROR;
                    }
                    else if (isOverFolder) {
                        mHoverPointClosesFolder = FALSE;
                    }
                    else {
                        *result = TRUE;
                        return NOERROR;
                    }
                }
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

void DragLayer::SendTapOutsideFolderAccessibilityEvent(
    /* [in] */ Boolean isEditingName)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::ACCESSIBILITY_SERVICE, (IInterface**)&obj);
    AutoPtr<IAccessibilityManager> accessibilityManager = IAccessibilityManager::Probe(obj);

    Boolean res;
    accessibilityManager->IsEnabled(&res);
    if (res) {
        Int32 stringId = isEditingName ?
                Elastos::Droid::Packages::Launcher2::R::string::folder_tap_to_rename :
                Elastos::Droid::Packages::Launcher2::R::string::folder_tap_to_close;
        AutoPtr<IAccessibilityEvent> event;
        AccessibilityEvent::Obtain(IAccessibilityEvent::TYPE_VIEW_FOCUSED, (IAccessibilityEvent)&event);
        OnInitializeAccessibilityEvent(event);

        AutoPtr<IContext> _context;
        GetContext((IContext**)&_context);
        String str;
        _context->GetString(stringId, &str);
        AutoPtr<ICharSequence> cchar = CoreUtils::Convert(str);
        AutoPtr<IList> list;
        event->GetText((IList**)&list);
        list->Add(TO_IINTERFACE(cchar));
        accessibilityManager->SendAccessibilityEvent(event);
    }
}

ECode DragLayer::OnRequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IWorkspace> workspace;
    mLauncher->GetWorkspace((IWorkspace**)&workspace);
    AutoPtr<IFolder> currentFolder;
    workspace->GetOpenFolder((IFolder**)&currentFolder);
    if (currentFolder != NULL) {
        if (TO_IINTERFACE(child) == TO_IINTERFACE(currentFolder)) {
            return FrameLayout::OnRequestSendAccessibilityEvent(child, event, result);
        }
        // Skip propagating onRequestSendAccessibilityEvent all for other children
        // when a folder is open
        *result = FALSE;
        return NOERROR;
    }
    return FrameLayout::OnRequestSendAccessibilityEvent(child, event, result);
}

ECode DragLayer::AddChildrenForAccessibility(
    /* [in] */ IArrayList* childrenForAccessibility)
{
    AutoPtr<IWorkspace> workspace;
    mLauncher->GetWorkspace((IWorkspace**)&workspace);
    AutoPtr<IFolder> currentFolder;
    workspace->GetOpenFolder((IFolder**)&currentFolder);
    if (currentFolder != NULL) {
        // Only add the folder as a child for accessibility when it is open
        return childrenForAccessibility->Add(TO_IINTERFACE(currentFolder));
    }
    else {
        return FrameLayout::AddChildrenForAccessibility(childrenForAccessibility);
    }
    return NOERROR;
}

ECode DragLayer::OnHoverEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // If we've received this, we've already done the necessary handling
    // in onInterceptHoverEvent. Return true to consume the event.
    *result = FALSE;
    return NOERROR;
}

ECode DragLayer::OnTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean handled = FALSE;
    Int32 action;
    ev->GetAction(&action);

    Float x;
    ev->GetX(&x);
    Int32 x = (Int32)x);
    Float y;
    ev->GetY(&y);
    Int32 y = (Int32)y;

    if (action == IMotionEvent::ACTION_DOWN) {
        if (action == IMotionEvent::ACTION_DOWN) {
            if (HandleTouchDown(ev, FALSE)) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }

    if (mCurrentResizeFrame != NULL) {
        handled = TRUE;
        switch (action) {
            case IMotionEvent::ACTION_MOVE:
                mCurrentResizeFrame->VisualizeResizeForDelta(x - mXDown, y - mYDown);
                break;
            case IMotionEvent::ACTION_CANCEL:
            case IMotionEvent::ACTION_UP:
                mCurrentResizeFrame->VisualizeResizeForDelta(x - mXDown, y - mYDown);
                mCurrentResizeFrame->OnTouchUp();
                mCurrentResizeFrame = NULL;
        }
    }
    if (handled) {
        *result = TRUE;
        return NOERROR;
    }
    return mDragController->OnTouchEvent(ev, result);
}

ECode DragLayer::GetDescendantRectRelativeToSelf(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* r,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);

    (*mTmpXY)[0] = 0;
    (*mTmpXY)[1] = 0;
    Float scale;
    GetDescendantCoordRelativeToSelf(descendant, mTmpXY, &scale);

    Int32 width;
    descendant->GetWidth(&width);
    Int32 height;
    descendant->GetHeight(&height);
    r->Set((*mTmpXY)[0], (*mTmpXY)[1],
            (*mTmpXY)[0] + width, (*mTmpXY)[1] + height);
    *result = scale;
    return NOERROR;
}

ECode DragLayer::GetLocationInDragLayer(
    /* [in] */ IView* child,
    /* [in] */ ArrayOf<Int32>* loc,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);

    (*loc)[0] = 0;
    (*loc)[1] = 0;
    return GetDescendantCoordRelativeToSelf(child, loc, result);
}

ECode DragLayer::GetDescendantCoordRelativeToSelf(
    /* [in] */ IView* descendant,
    /* [in] */ ArrayOf<Int32>* coord,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);

    Float scale = 1.0f;
    AutoPtr<ArrayOf<Float> > pt = ArrayOf<Float>::Alloc(2);
    (*pt)[0] = (*coord)[0];
    (*pt)[1] = (*coord)[1];
    AutoPtr<IMatrix> matrix;
    descendant->GetMatrix((IMatrix**)&matrix);
    matrix->MapPoints(pt);

    Float x;
    descendant->GetScaleX(&x);
    scale *= x;
    Int32 left;
    descendant->GetLeft(&left);
    (*pt)[0] += left;
    Int32 top;
    descendant->GetTop(&top);
    (*pt)[1] += top;
    AutoPtr<IViewParent> viewParent;
    descendant->GetParent((IViewParent**)&viewParent);
    while (IView::Probe(viewParent) != NULL && TO_IINTERFACE(viewParent) != THIS_PROBE(IInterface)) {
        AutoPtr<IView> view = IView::Probe(viewParent);
        AutoPtr<IMatrix> _matrix;
        view->GetMatrix((IMatrix**)&_matrix);
        _matrix->MapPoints(pt);

        Float _x;
        view->GetScaleX(&_x);
        scale *= _x;
        Int32 _left;
        view->GetLeft(&_left);
        Int32 sx;
        view->GetScrollX(&sx);
        (*pt)[0] += _left - sx;
        Int32 top;
        view->GetTop(&top);
        Int32 sy;
        view->GetScrollY(&sy);
        (*pt)[1] += top - sy;
        view->GetParent((IViewParent**)&viewParent);
    }
    (*coord)[0] = (Int32)Math::Round((*pt)[0]);
    (*coord)[1] = (Int32)Math::Round((*pt)[1]);
    *result = scale;
    return NOERROR;
}

ECode DragLayer::GetViewRectRelativeToSelf(
    /* [in] */ IView* v,
    /* [in] */ IRect* r)
{
    AutoPtr<ArrayOf<Int32> > loc = ArrayOf<Int32>::Alloc(2);
    GetLocationInWindow(loc);
    Int32 x = (*loc)[0];
    Int32 y = (*loc)[1];

    v->GetLocationInWindow(loc);
    Int32 vX = (*loc)[0];
    Int32 vY = (*loc)[1];

    Int32 left = vX - x;
    Int32 top = vY - y;
    Int32 width;
    v->GetMeasuredWidth(&width);
    Int32 height;
    v->GetMeasuredHeight(&height);
    return r->Set(left, top, left + width, top + height);
}

ECode DragLayer::DispatchUnhandledMove(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return mDragController->DispatchUnhandledMove(focused, direction, result);
}

ECode DragLayer::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    FrameLayout::OnLayout(changed, l, t, r, b);
    Int32 count;
    GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        AutoPtr<IViewGroupLayoutParams> param;
        child->GetLayoutParams((IViewGroupLayoutParams**)&param);
        AutoPtr<IFrameLayoutLayoutParams> flp = IFrameLayoutLayoutParams::Probe(param);
        if (ILayoutParams::Probe(flp) != NULL) {
            AutoPtr<ILayoutParams> lp = ILayoutParams::Probe(flp);
            Boolean res;
            lp->GetCustomPosition(&res);
            if (res) {
                Int32 x;
                lp->GetX(&x);
                Int32 y;
                lp->GetY(&y);
                Int32 width;
                lp->GetWidth(&width);
                Int32 height;
                lp->GetHeight(&height);
                child->Layout(x, y, x + width, y + height);
            }
        }
    }
    return NOERROR;
}

ECode DragLayer::ClearAllResizeFrames()
{
    Int32 size;
    mResizeFrames->GetSize(&size);
    if (size > 0) {
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> obj;
            mResizeFrames->Get(i, (IInterface**)&obj);
            AutoPtr<IAppWidgetResizeFrame> frame = IAppWidgetResizeFrame:: Probe(obj);

            frame->CommitResize();
            RemoveView(frame);
        }
        mResizeFrames->Clear();
    }
    return NOERROR;
}

ECode DragLayer::HasResizeFrames(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 size;
    mResizeFrames->GetSize(&size);
    *result = size > 0;
    return NOERROR;
}

ECode DragLayer::IsWidgetBeingResized(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mCurrentResizeFrame != NULL;
    return NOERROR;
}

ECode DragLayer::AddResizeFrame(
    /* [in] */ IItemInfo* itemInfo,
    /* [in] */ ILauncherAppWidgetHostView* widget,
    /* [in] */ ICellLayout* cellLayout)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<AppWidgetResizeFrame> resizeFrame = new AppWidgetResizeFrame(context,
            widget, cellLayout, this);

    AutoPtr<LayoutParams> lp = new LayoutParams(-1, -1);
    lp->mCustomPosition = TRUE;

    AddView(resizeFrame, lp);
    mResizeFrames->Add(TO_IINTERFACE(resizeFrame));

    return resizeFrame->SnapToWidget(FALSE);
}

ECode DragLayer::AnimateViewIntoPosition(
    /* [in] */ IDragView* dragView,
    /* [in] */ IView* child)
{
    return AnimateViewIntoPosition(dragView, child, NULL);
}

ECode DragLayer::AnimateViewIntoPosition(
    /* [in] */ IDragView* dragView,
    /* [in] */ ArrayOf<Int32>* pos,
    /* [in] */ Float alpha,
    /* [in] */ Float scaleX,
    /* [in] */ Float scaleY,
    /* [in] */ Int32 animationEndStyle,
    /* [in] */ IRunnable* onFinishRunnable,
    /* [in] */ Int32 duration)
{
    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    GetViewRectRelativeToSelf(dragView, r);
    Int32 fromX;
    r->GetLeft(&fromX);
    Int32 fromY;
    r->GetTop(&fromY);

    return AnimateViewIntoPosition(dragView, fromX, fromY, (*pos)[0],
            (*pos)[1], alpha, 1, 1, scaleX, scaleY,
            onFinishRunnable, animationEndStyle, duration, NULL);
}

ECode DragLayer::AnimateViewIntoPosition(
    /* [in] */ IDragView* dragView,
    /* [in] */ IView* child,
    /* [in] */ IRunnable* onFinishAnimationRunnable)
{
    return AnimateViewIntoPosition(dragView, child, -1,
            onFinishAnimationRunnable, NULL);
}

ECode DragLayer::AnimateViewIntoPosition(
    /* [in] */ IDragView* dragView,
    /* [in] */ IView* child,
    /* [in] */ Int32 duration,
    /* [in] */ IRunnable* onFinishAnimationRunnable,
    /* [in] */ IView* anchorView)
{
    AutoPtr<IViewParent> p;
    child->GetParent((IViewParent**)&p);
    AutoPtr<IShortcutAndWidgetContainer> parentChildren = IShortcutAndWidgetContainer::Probe(p);

    AutoPtr<IViewGroupLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)params);
    AutoPtr<ICellLayoutLayoutParams> lp =  ICellLayoutLayoutParams::Probe(params);
    parentChildren->MeasureChild(child);

    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    GetViewRectRelativeToSelf(dragView, r);

    AutoPtr<ArrayOf<Int32> > coord = ArrayOf<Int32>::Alloc(2);
    Float childScale;
    child->GetScaleX(&childScale);
    Int32 x;
    lp->GetX(&x);
    Int32 width;
    child->GetMeasuredWidth(&width);
    (*coord)[0] = x + (Int32)(width * (1 - childScale) / 2);
    Int32 y;
    lp->GetY(&y);
    Int32 height;
    child->GetMeasuredHeight(&height);
    (*coord)[1] = y + (Int32)(height * (1 - childScale) / 2);

    // Since the child hasn't necessarily been laid out, we force the lp to be updated with
    // the correct coordinates (above) and use these to determine the final location
    AutoPtr<IViewParent> p2;
    child->GetParent((IViewParent**)&p2);
    Float scale;
    GetDescendantCoordRelativeToSelf(IView::Probe(p2), coord, &scale);
    // We need to account for the scale of the child itself, as the above only accounts for
    // for the scale in parents.
    scale *= childScale;
    Int32 toX = (*coord)[0];
    Int32 toY = (*coord)[1];
    if (ITextView::Probe(child) != NULL) {
        AutoPtr<ITextView> tv = ITextView::Probe(child);

        // The child may be scaled (always about the center of the view) so to account for it,
        // we have to offset the position by the scaled size.  Once we do that, we can center
        // the drag view about the scaled child view.
        Int32 ptop;
        tv->GetPaddingTop(&ptop);
        toY += Math::Round(scale * ptop);
        Int32 mheight;
        dragView->GetMeasuredHeight(&mheight);
        toY -= mheight * (1 - scale) / 2;
        Int32 mwidth;
        dragView->GetMeasuredWidth(&mwidth);
        Int32 mwidth2;
        child->GetMeasuredWidth(&mwidth2);
        toX -= (mwidth - Math::Round(scale * mwidth2)) / 2;
    }
    else if (IFolderIcon::Probe(child) != NULL) {
        // Account for holographic blur padding on the drag view
        toY -= scale * IWorkspace::DRAG_BITMAP_PADDING / 2;
        Int32 mheight;
        dragView->GetMeasuredHeight(&mheight);
        toY -= (1 - scale) * mheight / 2;
        // Center in the x coordinate about the target's drawable
        Int32 mwidth;
        dragView->GetMeasuredWidth(&mwidth);
        Int32 mwidth2;
        child->GetMeasuredWidth(&mwidth2);
        toX -= (mwidth - Math::Round(scale * mwidth2)) / 2;
    } else {
        Int32 height;
        dragView->GetHeight(&height);
        Int32 mheigth;
        child->GetMeasuredHeight(&mheigth);
        toY -= (Math::Round(scale * (height - mheigth))) / 2;
        Int32 mwidth;
        dragView->GetMeasuredWidth(&mwidth);
        Int32 mwidth2;
        child->GetMeasuredWidth(&mwidth2);
        toX -= (Math::Round(scale * (mwidth - mwidth2))) / 2;
    }

    Int32 fromX;
    r->GetLeft(&fromX);
    Int32 fromY;
    r->GetTop(&fromY);
    child->SetVisibility(INVISIBLE);
    AutoPtr<IRunnable> onCompleteRunnable = new MyRunnable(child, onFinishAnimationRunnable);
    return AnimateViewIntoPosition(dragView, fromX, fromY, toX, toY, 1, 1, 1, scale, scale,
            onCompleteRunnable, ANIMATION_END_DISAPPEAR, duration, anchorView);
}

ECode DragLayer::AnimateViewIntoPosition(
    /* [in] */ IDragView* view,
    /* [in] */ Int32 fromX,
    /* [in] */ Int32 fromY,
    /* [in] */ Int32 toX,
    /* [in] */ Int32 toY,
    /* [in] */ Float finalAlpha,
    /* [in] */ Float initScaleX,
    /* [in] */ Float initScaleY,
    /* [in] */ Float finalScaleX,
    /* [in] */ Float finalScaleY,
    /* [in] */ IRunnable* onCompleteRunnable,
    /* [in] */ Int32 animationEndStyle,
    /* [in] */ Int32 duration,
    /* [in] */ IView* anchorView)
{
    Int32 width;
    view->GetMeasuredWidth(&width);
    Int32 height;
    view->GetMeasuredHeight(&height);
    AutoPtr<IRect> from;
    CRect::New(fromX, fromY, fromX + width, fromY + height, (IRect**)&from);

    AutoPtr<IRect> to;
    CRect::New(toX, toY, toX + width, toY + height, (IRect**)&to);
    return AnimateView(view, from, to, finalAlpha, initScaleX, initScaleY,
            finalScaleX, finalScaleY, duration, NULL, NULL, onCompleteRunnable,
            animationEndStyle, anchorView);
}

ECode DragLayer::AnimateView(
    /* [in] */ IDragView* view,
    /* [in] */ IRect* from,
    /* [in] */ IRect* to,
    /* [in] */ Float finalAlpha,
    /* [in] */ Float initScaleX,
    /* [in] */ Float initScaleY,
    /* [in] */ Float finalScaleX,
    /* [in] */ Float finalScaleY,
    /* [in] */ Int32 duration,
    /* [in] */ IInterpolator* motionInterpolator,
    /* [in] */ IInterpolator* alphaInterpolator,
    /* [in] */ IRunnable* onCompleteRunnable,
    /* [in] */ Int32 animationEndStyle,
    /* [in] */ IView* anchorView)
{
    // Calculate the duration of the animation based on the object's distance
    Int32 toleft;
    to->GetLeft(&toleft);
    Int32 fromleft;
    from->GetLeft(&fromleft);
    Int32 totop;
    to->GetTop(&totop);
    Int32 fromtop;
    from->GetTop(*fromtop);
    Float dist = (Float)Math::Sqrt(Math::Pow(toleft - fromleft, 2) +
            Math.pow(totop - fromtop, 2));
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    Int32 value;
    res->GetInteger(Elastos::Droid::Packages::Launcher2::R::integer::config_dropAnimMaxDist,
            &value);
    Float maxDist = (Float)value;

    // If duration < 0, this is a cue to compute the duration based on the distance
    if (duration < 0) {
        res->GetInteger(
                Elastos::Droid::Packages::Launcher2::R::integer::config_dropAnimMaxDuration,
                &duration);
        if (dist < maxDist) {
            Int32 polation;
            mCubicEaseOutInterpolator->GetInterpolation(dist / maxDist);
            duration *= polation;
        }
        Int32 _duration;
        res->GetInteger(Elastos::Droid::Packages::Launcher2::R::integer::config_dropAnimMinDuration,
                &_duration)
        duration = Math::Max(duration, _duration);
    }

    // Fall back to cubic ease out interpolator for the animation if none is specified
    AutoPtr<ITimeInterpolator> interpolator;
    if (alphaInterpolator == NULL || motionInterpolator == NULL) {
        interpolator = mCubicEaseOutInterpolator;
    }

    // Animate the view
    Float initAlpha;
    view->GetAlpha(&initAlpha);
    Float dropViewScale;
    view->GetScaleX(&dropViewScale);

    AutoPtr<IAnimatorUpdateListener> updateCb new MyAnimatorUpdateListener(view,
            alphaInterpolator, motionInterpolator, initScaleX, initScaleY, dropViewScale,
            finalScaleX, finalScaleY, finalAlpha, initAlpha,
            from, to, this);
    return AnimateView(view, updateCb, duration, interpolator, onCompleteRunnable,
            animationEndStyle, anchorView);
}

ECode DragLayer::AnimateView(
    /* [in] */ IDragView* view,
    /* [in] */ IAnimatorUpdateListener* updateCb,
    /* [in] */ Int32 duration,
    /* [in] */ ITimeInterpolator* interpolator,
    /* [in] */ IRunnable* onCompleteRunnable,
    /* [in] */ Int32 animationEndStyle,
    /* [in] */ IView* anchorView)
{
    // Clean up the previous animations
    if (mDropAnim != NULL) mDropAnim->Cancel();
    if (mFadeOutAnim != NULL) mFadeOutAnim->Cancel();

    // Show the drop view if it was previously hidden
    mDropView = view;
    mDropView->CancelAnimation();
    mDropView->ResetLayoutParams();

    // Set the anchor view if the page is scrolling
    if (anchorView != NULL) {
        anchorView->GetScrollX(&mAnchorViewInitialScrollX);
    }
    mAnchorView = anchorView;

    // Create and start the animation
    CValueAnimator::New((IValueAnimator**)&mDropAnim);
    mDropAnim->SetInterpolator(interpolator);
    mDropAnim->SetDuration(duration);
    mDropAnim->SetFloatValues(0f, 1f);
    mDropAnim->AddUpdateListener(updateCb);
    AutoPtr<IAnimatorListener> lisener = MyAnimatorListenerAdapter(
            onCompleteRunnable, animationEndStyle, this);
    mDropAnim->AddListener(lisener);
    return mDropAnim->Start();
}

ECode DragLayer::ClearAnimatedView()
{
    if (mDropAnim != NULL) {
        mDropAnim->Cancel();
    }
    if (mDropView != NULL) {
        mDragController->OnDeferredEndDrag(mDropView);
    }
    mDropView = NULL;
    return Invalidate();
}

ECode DragLayer::GetAnimatedView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    *view = mDropView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

void DragLayer::FadeOutDragView()
{
    CValueAnimator::New((IValueAnimator**)&mFadeOutAnim);
    mFadeOutAnim->SetDuration(150);
    mFadeOutAnim->SetFloatValues(0f, 1f);
    mFadeOutAnim->RemoveAllUpdateListeners();
    AutoPtr<IAnimatorUpdateListener> lisener = new MyAnimatorUpdateListener2(this);
    mFadeOutAnim->AddUpdateListener(lisener);
    AutoPtr<IAnimatorListener> adapter = new MyAnimatorListenerAdapter2(this);
    mFadeOutAnim->AddListener(adapter);
    mFadeOutAnim->Start();
}

ECode DragLayer::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    return UpdateChildIndices();
}

ECode DragLayer::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    return UpdateChildIndices();
}

void DragLayer::UpdateChildIndices()
{
    if (mLauncher != NULL) {
        AutoPtr<IWorkspace> workspace;
        mLauncher->GetWorkspace((IWorkspace**)&workspace);
        IndexOfChild(workspace, &mWorkspaceIndex);
        AutoPtr<ISearchDropTargetBar> bar;
        IndexOfChild(bar, &mQsbIndex);
    }
}

ECode DragLayer::GetChildDrawingOrder(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 i,
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);

    // TODO: We have turned off this custom drawing order because it now effects touch
    // dispatch order. We need to sort that issue out and then decide how to go about this.
    // if (TRUE || LauncherApplication.isScreenLandscape(getContext()) ||
    //         mWorkspaceIndex == -1 || mQsbIndex == -1 ||
    //         mLauncher.getWorkspace().isDrawingBackgroundGradient()) {
    //     return i;
    // }
    *order = i;
    return NOERROR;

    // This ensures that the workspace is drawn above the hotseat and qsb,
    // except when the workspace is drawing a background gradient, in which
    // case we want the workspace to stay behind these elements.
    if (i == mQsbIndex) {
        *order = mWorkspaceIndex;
        return NOERROR;
    }
    else if (i == mWorkspaceIndex) {
        *order = mQsbIndex;
        return NOERROR;
    }
    else {
        *order = i;
        return NOERROR;
    }
}

ECode DragLayer::OnEnterScrollArea(
    /* [in] */ Int32 direction)
{
    mInScrollArea = TRUE;
    return Invalidate();
}

ECode DragLayer::OnExitScrollArea()
{
    mInScrollArea = FALSE;
    return Invalidate();
}

Boolean DragLayer::IsLayoutDirectionRtl()
{
    Int32 direction;
    GetLayoutDirection(&direction);
    return (direction == LAYOUT_DIRECTION_RTL);
}

ECode DragLayer::DispatchDraw(
        /* [in] */ ICanvas* canvas)
{
    FrameLayout::DispatchDraw(canvas);

    Boolean res;
    LauncherApplication::IsScreenLarge(&res);
    if (mInScrollArea && !res) {
        AutoPtr<IWorkspace> workspace;
        mLauncher->GetWorkspace((IWorkspace**)&workspace);
        Int32 width;
        workspace->GetWidth(&width);
        AutoPtr<IRect> childRect;
        CRect::New((IRect**)&childRect);
        AutoPtr<IView> view;
        workspace->GetChildAt(0, (IView**)&view);
        GetDescendantRectRelativeToSelf(view, childRect);

        Int32 page;
        workspace->GetNextPage(&page);
        Boolean isRtl = IsLayoutDirectionRtl();
        AutoPtr<IView> tmp;
        workspace->GetChildAt(isRtl ? page + 1 : page - 1, (IView**)&tmp);
        AutoPtr<ICellLayout> leftPage = ICellLayout::Probe(tmp);
        AutoPtr<IView> tmp2;
        workspace->GetChildAt(isRtl ? page - 1 : page + 1, (IView**)&tmp2);
        AutoPtr<ICellLayout> rightPage = ICellLayout::Probe(tmp2);

        if (leftPage != NULL) {
            Boolean res;
            leftPage->GetIsDragOverlapping(&res);
            if (res) {
                Int32 top;
                childRect->GetTop(&top);
                Int32 width;
                mLeftHoverDrawable->GetIntrinsicWidth(&width);
                Int32 bottom;
                childRect->GetBottom(&bottom);
                mLeftHoverDrawable->SetBounds(0, top, width, bottom);
                mLeftHoverDrawable->Draw(canvas);
            }
        }
        else if (rightPage != NULL) {
            Boolean res;
            rightPage->GetIsDragOverlapping(&res);
            if (res) {
                Int32 _width;
                mRightHoverDrawable->GetIntrinsicWidth(&_width);
                Int32 top;
                childRect->GetTop(&top);
                Int32 bottom;
                childRect->GetBottom(&bottom);
                mRightHoverDrawable->SetBounds(width - _width, top, width, bottom);
                mRightHoverDrawable->Draw(canvas);
            }
        }
    }
    return NOERROR;
}

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos