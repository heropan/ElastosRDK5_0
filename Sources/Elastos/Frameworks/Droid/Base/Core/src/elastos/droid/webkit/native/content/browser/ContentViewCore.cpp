
#include "webkit/native/content/browser/ContentViewCore.h"
#include "webkit/native/content/common/ContentSwitches.h"
#include "webkit/native/base/CommandLine.h"
//TODO #include "content/CIntent.h"
//TODO #include "content/CIntentHelper.h"
#include "os/CHandler.h"
#include "text/TextUtils.h"
//TODO #include "text/CEditableFactory.h"
//TODO #include "text/CSelection.h"
//TODO #include "widget/CView.h"

#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::IList;
using Elastos::Droid::App::EIID_IActivity;
using Elastos::Droid::App::ISearchManager;
//TODO using Elastos::Droid::Content::CIntent;
//TODO using Elastos::Droid::Content::CIntentHelper;
using Elastos::Droid::Content::IIntentHelper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Text::TextUtils;
//TODO using Elastos::Droid::Text::CSelection;
using Elastos::Droid::Text::ISelection;
//TODO using Elastos::Droid::Text::CEditableFactory;
using Elastos::Droid::Text::IEditableFactory;
//TODO using Elastos::Droid::Widget::CView;
using Elastos::Droid::Utility::IDisplayMetrics;

using Elastos::Droid::Webkit::Content::Common::ContentSwitches;
using Elastos::Droid::Webkit::Base::CommandLine;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//===============================================================
//         ContentViewCore::InnerViewAndroidDelegate
//===============================================================

ContentViewCore::InnerViewAndroidDelegate::InnerViewAndroidDelegate(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
    , mContainerViewAtCreation(owner->mContainerView)
{
}

//@Override
AutoPtr<IView> ContentViewCore::InnerViewAndroidDelegate::AcquireAnchorView()
{
    assert(0);
#if 0
    AutoPtr<IView> anchorView;
    CView::New(mOwner->mContext, (IView**)&anchorView);
    mContainerViewAtCreation->AddView(anchorView);
    return anchorView;
#endif

    return NULL;
}

//@Override
//@SuppressWarnings("deprecation")  // AbsoluteLayout
ECode ContentViewCore::InnerViewAndroidDelegate::SetAnchorViewPosition(
    /* [in] */ IView* view,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float width,
    /* [in] */ Float height)
{
    assert(0);
#if 0
    AutoPtr<IViewParent> viewParent;
    view->GetParent((IViewParent**)&viewParent);
    if (viewParent == NULL) {
        // Ignore. setAnchorViewPosition has been called after the anchor view has
        // already been released.
        return NOERROR;
    }
    assert(viewParent == mContainerViewAtCreation);

    Float scale = (Float) DeviceDisplayInfo::Create(mContext)->GetDIPScale();

    // The anchor view should not go outside the bounds of the ContainerView.
    Int32 leftMargin = Math::Round(x * scale);
    Int32 topMargin = Math::Round(mRenderCoordinates->GetContentOffsetYPix() + y * scale);
    Int32 scaledWidth = Math::Round(width * scale);
    // ContentViewCore currently only supports these two container view types.
    if (mContainerViewAtCreation->Probe(EIID_IFrameLayout) != NULL) {
        Int32 startMargin;
        if (ApiCompatibilityUtils::IsLayoutRtl(mContainerViewAtCreation)) {
            startMargin = mContainerViewAtCreation->GetMeasuredWidth()
                    - Math::Round((width + x) * scale);
        }
        else {
            startMargin = leftMargin;
        }

        if (scaledWidth + startMargin > mContainerViewAtCreation->GetWidth()) {
            scaledWidth = mContainerViewAtCreation->GetWidth() - startMargin;
        }

        AutoPtr<IFrameLayoutLayoutParams> lp;
        CFrameLayoutLayoutParams::New(
            scaledWidth, Math::Round(height * scale),
            (IFrameLayoutLayoutParams**)&lp);
        ApiCompatibilityUtils::SetMarginStart(lp, startMargin);
        lp->SetTopMargin(topMargin);
        view->SetLayoutParams(lp);
    }
    else if (mContainerViewAtCreation->Probe(EIID_IAbsoluteLayout) != NULL) {
        // This fixes the offset due to a difference in
        // scrolling model of WebView vs. Chrome.
        // TODO(sgurun) fix this to use mContainerViewAtCreation.getScroll[X/Y]()
        // as it naturally accounts for scroll differences between
        // these models.
        leftMargin += mRenderCoordinates->GetScrollXPixInt();
        topMargin += mRenderCoordinates->GetScrollYPixInt();

        AutoPtr<IAbsoluteLayoutLayoutParams> lp;
        CAbsoluteLayoutLayoutParams::New(
                    scaledWidth, (Int32) (height * scale), leftMargin, topMargin,
                    (IAbsoluteLayoutLayoutParams**)&lp);
        view->SetLayoutParams(lp);
    }
    else {
//        Log.e(TAG, "Unknown layout " + mContainerViewAtCreation.getClass().getName());
    }
#endif
    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerViewAndroidDelegate::ReleaseAnchorView(
    /* [in] */ IView* anchorView)
{
    assert(0);
//    mContainerViewAtCreation->RemoveView(anchorView);
    return NOERROR;
}

//===============================================================
//           ContentViewCore::InnerImeAdapterDelegate
//===============================================================

ContentViewCore::InnerImeAdapterDelegate::InnerImeAdapterDelegate(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
{
}

//@Override
void ContentViewCore::InnerImeAdapterDelegate::OnImeEvent(
    /* [in] */ Boolean isFinish)
{
    mOwner->GetContentViewClient()->OnImeEvent();
    if (!isFinish) {
        mOwner->HideHandles();
    }
}

//@Override
void ContentViewCore::InnerImeAdapterDelegate::OnDismissInput()
{
    mOwner->GetContentViewClient()->OnImeStateChangeRequested(FALSE);
}

//@Override
AutoPtr<IView> ContentViewCore::InnerImeAdapterDelegate::GetAttachedView()
{
    return mOwner->mContainerView;
}

//@Override
AutoPtr<IResultReceiver> ContentViewCore::InnerImeAdapterDelegate::GetNewShowKeyboardReceiver()
{
    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    assert(0);
    AutoPtr<IResultReceiver> receiver;// = new InnerResultReceiver(this, handler);
    return receiver;
}

//==========================================================================
//       ContentViewCore::InnerImeAdapterDelegate::InnerResultReceiver
//==========================================================================

ContentViewCore::InnerImeAdapterDelegate::InnerResultReceiver::InnerResultReceiver(
    /* [in] */ InnerImeAdapterDelegate* owner)
    : mOwner(owner)
{
}

ECode ContentViewCore::InnerImeAdapterDelegate::InnerResultReceiver::OnReceiveResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    mOwner->mOwner->GetContentViewClient()->OnImeStateChangeRequested(
            resultCode == IInputMethodManager::RESULT_SHOWN ||
            resultCode == IInputMethodManager::RESULT_UNCHANGED_SHOWN);
    if (resultCode == IInputMethodManager::RESULT_SHOWN) {
        // If OSK is newly shown, delay the form focus until
        // the onSizeChanged (in order to adjust relative to the
        // new size).
        // TODO(jdduke): We should not assume that onSizeChanged will
        // always be called, crbug.com/294908.
        assert(0);
//        mOwner->mOwner->GetContainerView()->GetWindowVisibleDisplayFrame(
//                mFocusPreOSKViewportRect);
    }
    else if (mOwner->mOwner->HasFocus() && resultCode ==
            IInputMethodManager::RESULT_UNCHANGED_SHOWN) {
        // If the OSK was already there, focus the form immediately.
        mOwner->mOwner->ScrollFocusedEditableNodeIntoView();
    }

    return NOERROR;
}

//===============================================================
//               ContentViewCore::InnerListener
//===============================================================

ContentViewCore::InnerListener::InnerListener(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
{
}

ECode ContentViewCore::InnerListener::OnPositionChanged(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mOwner->IsSelectionHandleShowing() || mOwner->IsInsertionHandleShowing()) {
        mOwner->TemporarilyHideTextHandles();
    }

    return NOERROR;
}

//===============================================================
//          ContentViewCore::InnerZoomControlsDelegate
//===============================================================

ContentViewCore::InnerZoomControlsDelegate::InnerZoomControlsDelegate(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
{
}

//@Override
void ContentViewCore::InnerZoomControlsDelegate::InvokeZoomPicker()
{
}

//@Override
void ContentViewCore::InnerZoomControlsDelegate::DismissZoomPicker()
{
}

//@Override
void ContentViewCore::InnerZoomControlsDelegate::UpdateZoomControls()
{
}

//===============================================================
//       ContentViewCore::InnerWebContentsObserverAndroid
//===============================================================

ContentViewCore::InnerWebContentsObserverAndroid::InnerWebContentsObserverAndroid(
    /* [in] */ ContentViewCore* owner)
    : WebContentsObserverAndroid(owner)
    , mOwner(owner)
{
}

//@Override
ECode ContentViewCore::InnerWebContentsObserverAndroid::DidNavigateMainFrame(
    /* [in] */ const String& url,
    /* [in] */ const String& baseUrl,
    /* [in] */ Boolean isNavigationToDifferentPage,
    /* [in] */ Boolean isFragmentNavigation)
{
    if (!isNavigationToDifferentPage) {
        return NOERROR;
    }

    mOwner->HidePopups();
    mOwner->ResetScrollInProgress();
    mOwner->ResetGestureDetection();

    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerWebContentsObserverAndroid::RenderProcessGone(
    /* [in] */ Boolean wasOomProtected)
{
    mOwner->HidePopups();
    mOwner->ResetScrollInProgress();
    // No need to reset gesture detection as the detector will have
    // been destroyed in the RenderWidgetHostView.

    return NOERROR;
}

//===============================================================
//       ContentViewCore::InnerOnVisibilityChangedListener
//===============================================================

ContentViewCore::InnerOnVisibilityChangedListener::InnerOnVisibilityChangedListener(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
{
}

//@Override
void ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerShown(
    /* [in] */ const PopupZoomer* zoomer)
{
    assert(0);
//    AutoPtr<IRunnable> runnable = new OnPopupZoomerShownRunnable(this, zoomer);
//    mContainerViewAtCreation->Post(runnable);
}

//@Override
void ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerHidden(
    /* [in] */ const PopupZoomer* zoomer)
{
    assert(0);
//    AutoPtr<IRunnable> runnable = new OnPopupZoomerHiddenRunnable(this, zoomer);
//    mContainerViewAtCreation->Post(runnable);
}

//===========================================================================================
//       ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerShownRunnable
//===========================================================================================

ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerShownRunnable::OnPopupZoomerShownRunnable(
    /* [in] */ InnerOnVisibilityChangedListener* owner,
    /* [in] */ const PopupZoomer* zoomer)
    : mOwner(owner)
    , mZoomer(zoomer)
{
}

ECode ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerShownRunnable::Run()
{
    assert(0);
#if 0
    if (mContainerViewAtCreation->IndexOfChild(mZoomer) == -1) {
        mContainerViewAtCreation->AddView(mZoomer);
    }
    else {
        assert(0);
//        assert false : "PopupZoomer should never be shown without being hidden";
    }
#endif

    return NOERROR;
}

//===========================================================================================
//       ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerHiddenRunnable
//===========================================================================================

ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerHiddenRunnable::OnPopupZoomerHiddenRunnable(
    /* [in] */ InnerOnVisibilityChangedListener* owner,
    /* [in] */ const PopupZoomer* zoomer)
    : mOwner(owner)
    , mZoomer(zoomer)
{
}

ECode ContentViewCore::InnerOnVisibilityChangedListener::OnPopupZoomerHiddenRunnable::Run()
{
    assert(0);
#if 0
    if (mContainerViewAtCreation->IndexOfChild(zoomer) != -1) {
        mContainerViewAtCreation->RemoveView(zoomer);
        mContainerViewAtCreation->Invalidate();
    }
    else {
        assert(0);
//        assert false : "PopupZoomer should never be hidden without being shown";
    }
#endif

    return NOERROR;
}

//===============================================================
//              ContentViewCore::InnerOnTapListener
//===============================================================

ContentViewCore::InnerOnTapListener::InnerOnTapListener(
    /* [in] */ ContentViewCore* owner,
    /* [in] */ IViewGroup* vg)
    : mOwner(owner)
    , mContainerViewAtCreation(vg)
{
}

//@Override
Boolean ContentViewCore::InnerOnTapListener::OnSingleTap(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* e)
{
    assert(0);
#if 0
    mContainerViewAtCreation->RequestFocus();
    if (mOwner->mNativeContentViewCore != 0) {
        Int64 time;
        Float x, y;
        e->GetEventTime(&time);
        e->GetX(&x);
        e->GetY(&y);
        if (mOwner->NativeSingleTap(mNativeContentViewCore, time, x, y);
    }
#endif

    return TRUE;
}

//@Override
Boolean ContentViewCore::InnerOnTapListener::OnLongPress(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* e)
{
    assert(0);
#if 0
    if (mOwner->mNativeContentViewCore != 0) {
        Int64 time;
        Int32 x, y;
        e->GetEventTime(&time);
        e->GetX(&x);
        e->GetY(&y);
        mOwner->NativeLongPress(mOwner->mNativeContentViewCore, time, x, y);
    }
#endif

    return TRUE;
}

//===============================================================
//            ContentViewCore::FakeMouseMoveRunnable
//===============================================================

ContentViewCore::FakeMouseMoveRunnable::FakeMouseMoveRunnable(
    /* [in] */ ContentViewCore* owner,
    /* [in] */ IMotionEvent* eventFakeMouseMove)
    : mOwner(owner)
    , mEventFakeMouseMove(eventFakeMouseMove)
{
}

ECode ContentViewCore::FakeMouseMoveRunnable::Run()
{
    mOwner->OnHoverEvent(mEventFakeMouseMove);
    assert(0);
//    mEventFakeMouseMove->Recycle();
    return NOERROR;
}

//===============================================================
//        ContentViewCore::InnerSelectionHandleController
//===============================================================

ContentViewCore::InnerSelectionHandleController::InnerSelectionHandleController(
    /* [in] */ ContentViewCore* owner,
    /* [in] */ IView* parent,
    /* [in] */ PositionObserver* positionObserver)
    : SelectionHandleController(parent, positionObserver)
    , mOwner(owner)
{
}

//@Override
void ContentViewCore::InnerSelectionHandleController::SelectBetweenCoordinates(
    /* [in] */ Int32 x1,
    /* [in] */ Int32 y1,
    /* [in] */ Int32 x2,
    /* [in] */ Int32 y2)
{
    if (mOwner->mNativeContentViewCore != 0 && !(x1 == x2 && y1 == y2)) {
        mOwner->NativeSelectBetweenCoordinates(mOwner->mNativeContentViewCore,
                x1, y1 - mOwner->mRenderCoordinates->GetContentOffsetYPix(),
                x2, y2 - mOwner->mRenderCoordinates->GetContentOffsetYPix());
    }
}

//@Override
void ContentViewCore::InnerSelectionHandleController::ShowHandles(
    /* [in] */ Int32 startDir,
    /* [in] */ Int32 endDir)
{
    Boolean wasShowing;
    IsShowing(&wasShowing);
    SelectionHandleController::ShowHandles(startDir, endDir);
    if (!wasShowing || mOwner->mActionMode == NULL) {
        mOwner->ShowSelectActionBar();
    }
}

//===============================================================
//       ContentViewCore::InnerInsertionHandleController
//===============================================================

const Int32 ContentViewCore::InnerInsertionHandleController::AVERAGE_LINE_HEIGHT;

ContentViewCore::InnerInsertionHandleController::InnerInsertionHandleController(
    /* [in] */ ContentViewCore* owner,
    /* [in] */ IView* parent,
    /* [in] */ PositionObserver* positionObserver)
    : InsertionHandleController(parent, positionObserver)
    , mOwner(owner)
{
}

//@Override
void ContentViewCore::InnerInsertionHandleController::SetCursorPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mOwner->mNativeContentViewCore != 0) {
        mOwner->NativeMoveCaret(mOwner->mNativeContentViewCore,
                x, y - mOwner->mRenderCoordinates->GetContentOffsetYPix());
    }
}

//@Override
void ContentViewCore::InnerInsertionHandleController::Paste()
{
    mOwner->mImeAdapter->Paste();
    mOwner->HideHandles();
}

//@Override
Int32 ContentViewCore::InnerInsertionHandleController::GetLineHeight()
{
    assert(0);
//    return (Int32) Math::Ceil(
//        mRenderCoordinates->FromLocalCssToPix(AVERAGE_LINE_HEIGHT));
    return 0;
}

//@Override
void ContentViewCore::InnerInsertionHandleController::ShowHandle()
{
    InsertionHandleController::ShowHandle();
}

//===============================================================
//              ContentViewCore::InnerActionHandler
//===============================================================

ContentViewCore::InnerActionHandler::InnerActionHandler(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
{
}

//@Override
ECode ContentViewCore::InnerActionHandler::SelectAll()
{
    mOwner->mImeAdapter->SelectAll();
    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::Cut()
{
    mOwner->mImeAdapter->Cut();
    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::Copy()
{
    mOwner->mImeAdapter->Copy();
    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::Paste()
{
    mOwner->mImeAdapter->Paste();
    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::Share()
{
    const String query = mOwner->GetSelectedText();
    if (TextUtils::IsEmpty(query)) {
        return NOERROR;
    }

    AutoPtr<IIntent> send;
    // TODO
//    CIntent::New(IIntent::ACTION_SEND, (IIntent**)&send);
    send->SetType(String("text/plain"));
    send->PutExtra(IIntent::EXTRA_TEXT, query);
    // try {
        AutoPtr<IIntentHelper> helper;
        // TODO
        //CIntentHelper::AcquireSingleton((IIntentHelper**)&helper);
        AutoPtr<IIntent> i;
        String str;
        //TODO
        assert(0);
//        mOwner->GetContext()->GetString(R::string::actionbar_share, &str);
//        helper->CreateChooser(send, str, (IIntent**)&i);
        i->SetFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
        mOwner->GetContext()->StartActivity(i);
    // } catch (android.content.ActivityNotFoundException ex) {
    //     // If no app handles it, do nothing.
    // }

    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::Search()
{
    const String query = mOwner->GetSelectedText();
    if (TextUtils::IsEmpty(query)) {
        return NOERROR;
    }

    // See if ContentViewClient wants to override
    if (mOwner->GetContentViewClient()->DoesPerformWebSearch()) {
        mOwner->GetContentViewClient()->PerformWebSearch(query);
        return NOERROR;
    }

    AutoPtr<IIntent> i;
    // TODO
//    CIntent::New(IIntent::ACTION_WEB_SEARCH, (IIntent**)&i);
    i->PutExtra(ISearchManager::EXTRA_NEW_SEARCH, TRUE);
    i->PutExtra(ISearchManager::QUERY, query);
    String name;
    mOwner->GetContext()->GetPackageName(&name);
    i->PutExtra(IBrowser::EXTRA_APPLICATION_ID, name);
    if (mOwner->GetContext()->Probe(EIID_IActivity) == NULL) {
        i->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    }

    // try {
        mOwner->GetContext()->StartActivity(i);
    // } catch (android.content.ActivityNotFoundException ex) {
    //     // If no app handles it, do nothing.
    // }

    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::IsSelectionPassword(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mOwner->mImeAdapter->IsSelectionPassword();
    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::IsSelectionEditable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mOwner->mSelectionEditable;
    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::OnDestroyActionMode()
{
    mOwner->mActionMode = NULL;
    if (mOwner->mUnselectAllOnActionModeDismiss) {
        mOwner->mImeAdapter->Unselect();
    }

    mOwner->GetContentViewClient()->OnContextualActionBarHidden();

    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::IsShareAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IIntent> intent;
    // TODO
    //CIntent::New(IIntent::ACTION_SEND, (IIntent**)&intent);
    intent->SetType(String("text/plain"));
    AutoPtr<IPackageManager> pm;
    mOwner->GetContext()->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IList> resolves;
    pm->QueryIntentActivities(intent,
            IPackageManager::MATCH_DEFAULT_ONLY,
            (IList**)&resolves);
    Int32 size;
    resolves->GetSize(&size);

    *result = size > 0;

    return NOERROR;
}

//@Override
ECode ContentViewCore::InnerActionHandler::IsWebSearchAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mOwner->GetContentViewClient()->DoesPerformWebSearch()) {
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<IIntent> intent;
    // TODO
    //CIntent::New(IIntent::ACTION_WEB_SEARCH, (IIntent**)&intent);
    intent->PutExtra(ISearchManager::EXTRA_NEW_SEARCH, TRUE);
    AutoPtr<IPackageManager> pm;
    mOwner->GetContext()->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IList> resolves;
    pm->QueryIntentActivities(intent,
            IPackageManager::MATCH_DEFAULT_ONLY,
            (IList**)&resolves);
    Int32 size;
    resolves->GetSize(&size);

    *result = size > 0;

    return NOERROR;
}

//===============================================================
//         ContentViewCore::DeferredHandleFadeInRunnable
//===============================================================

ContentViewCore::DeferredHandleFadeInRunnable::DeferredHandleFadeInRunnable(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
{
}

ECode ContentViewCore::DeferredHandleFadeInRunnable::Run()
{
    if (!mOwner->AllowTextHandleFadeIn()) {
        // Delay fade in until it is allowed.
        mOwner->ScheduleTextHandleFadeIn();
    }
    else {
        if (mOwner->IsSelectionHandleShowing()) {
            mOwner->mSelectionHandleController->BeginHandleFadeIn();
        }

        if (mOwner->IsInsertionHandleShowing()) {
            mOwner->mInsertionHandleController->BeginHandleFadeIn();
        }
    }

    return NOERROR;
}

//===============================================================
//             ContentViewCore::InnerContentObserver
//===============================================================

ContentViewCore::InnerContentObserver::InnerContentObserver(
    /* [in] */ ContentViewCore* owner)
    : mOwner(owner)
{
}

ECode ContentViewCore::InnerContentObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    Boolean isEnabled = FALSE;
    mOwner->mAccessibilityManager->IsEnabled(&isEnabled);
    mOwner->SetAccessibilityState(isEnabled);
    return NOERROR;
}

//===============================================================
//                    ContentViewCore
//===============================================================

const String ContentViewCore::TAG("ContentViewCore");

// Used to avoid enabling zooming in / out if resulting zooming will
// produce little visible difference.
const Float ContentViewCore::ZOOM_CONTROLS_EPSILON = 0.007f;

// Used to represent gestures for long press and long tap.
const Int32 ContentViewCore::IS_LONG_PRESS;
const Int32 ContentViewCore::IS_LONG_TAP;

// Length of the delay (in ms) before fading in handles after the last page movement.
const Int32 ContentViewCore::TEXT_HANDLE_FADE_IN_DELAY;

// These values are obtained from Samsung.
// TODO(changwan): refactor SPen related code into a separate class. See
// http://crbug.com/398169.
const Int32 ContentViewCore::SPEN_ACTION_DOWN;;
const Int32 ContentViewCore::SPEN_ACTION_UP;
const Int32 ContentViewCore::SPEN_ACTION_MOVE;
const Int32 ContentViewCore::SPEN_ACTION_CANCEL;
Boolean ContentViewCore::sIsSPenSupported = FALSE;

/**
 * PID used to indicate an invalid render process.
 */
// Keep in sync with the value returned from ContentViewCoreImpl::GetCurrentRendererProcessId()
// if there is no render process.
const Int32 ContentViewCore::INVALID_RENDER_PROCESS_PID;

/**
 * Constructs a new ContentViewCore. Embedders must call initialize() after constructing
 * a ContentViewCore and before using it.
 *
 * @param context The context used to create this.
 */
ContentViewCore::ContentViewCore(
    /* [in] */ IContext* context)
    : mNativeContentViewCore(0)
    , mViewportWidthPix(0)
    , mViewportHeightPix(0)
    , mPhysicalBackingWidthPix(0)
    , mPhysicalBackingHeightPix(0)
    , mOverdrawBottomHeightPix(0)
    , mViewportSizeOffsetWidthPix(0)
    , mViewportSizeOffsetHeightPix(0)
    , mHasSelection(FALSE)
    , mSelectionEditable(FALSE)
    , mUnselectAllOnActionModeDismiss(FALSE)
    , mNativeAccessibilityAllowed(FALSE)
    , mNativeAccessibilityEnabled(FALSE)
    , mTouchExplorationEnabled(FALSE)
    , mShouldSetAccessibilityFocusOnPageLoad(FALSE)
    , mLastTapX(0)
    , mLastTapY(0)
    , mTouchScrollInProgress(FALSE)
    , mPotentiallyActiveFlingCount(0)
    , mCurrentTouchOffsetX(0.0f)
    , mCurrentTouchOffsetY(0.0f)
    , mSmartClipOffsetX(0)
    , mSmartClipOffsetY(0)
    , mContext(context)
{
    mAdapterInputConnectionFactory = new ImeAdapter::AdapterInputConnectionFactory();
    mInputMethodManagerWrapper = new InputMethodManagerWrapper(mContext);

    mRenderCoordinates = new RenderCoordinates();
    AutoPtr<IResources> res;
    GetContext()->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> dm;
    res->GetDisplayMetrics((IDisplayMetrics**)&dm);
    Float deviceScaleFactor;
    dm->GetDensity(&deviceScaleFactor);
    String forceScaleFactor = CommandLine::GetInstance()->GetSwitchValue(
            ContentSwitches::FORCE_DEVICE_SCALE_FACTOR);
    if (forceScaleFactor != NULL) {
        deviceScaleFactor = StringUtils::ParseFloat(forceScaleFactor);
    }
    mRenderCoordinates->SetDeviceScaleFactor(deviceScaleFactor);
    mStartHandlePoint = mRenderCoordinates->CreateNormalizedPoint();
    mEndHandlePoint = mRenderCoordinates->CreateNormalizedPoint();
    mInsertionHandlePoint = mRenderCoordinates->CreateNormalizedPoint();
    GetContext()->GetSystemService(IContext::ACCESSIBILITY_SERVICE, (IInterface**)&mAccessibilityManager);
    // TODO
    //mGestureStateListeners = new ObserverList<GestureStateListener>();
    //mGestureStateListenersIterator = mGestureStateListeners->RewindableIterator();

    AutoPtr<IEditableFactory> factory;
    // TODO
    //CEditableFactory::AcquireSingleton((IEditableFactory**)&factory);
    factory->GetInstance(String(""), (IEditable**)&mEditable);
    AutoPtr<ISelection> select;
    // TODO
    //CSelection::AcquireSingleton((ISelection**)&select);
    select->SetSelection(mEditable, 0);
}

/**
 * @return The context used for creating this ContentViewCore.
 */
//@CalledByNative
AutoPtr<IContext> ContentViewCore::GetContext()
{
    return mContext;
}

/**
 * @return The ViewGroup that all view actions of this ContentViewCore should interact with.
 */
AutoPtr<IViewGroup> ContentViewCore::GetContainerView()
{
    return mContainerView;
}

/**
 * @return The WebContents currently being rendered.
 */
AutoPtr<WebContents> ContentViewCore::GetWebContents()
{
    return mWebContents;
}

/**
 * Specifies how much smaller the WebKit layout size should be relative to the size of this
 * view.
 * @param offsetXPix The X amount in pixels to shrink the viewport by.
 * @param offsetYPix The Y amount in pixels to shrink the viewport by.
 */
void ContentViewCore::SetViewportSizeOffset(
    /* [in] */ Int32 offsetXPix,
    /* [in] */ Int32 offsetYPix)
{
    if (offsetXPix != mViewportSizeOffsetWidthPix ||
            offsetYPix != mViewportSizeOffsetHeightPix) {
        mViewportSizeOffsetWidthPix = offsetXPix;
        mViewportSizeOffsetHeightPix = offsetYPix;
        if (mNativeContentViewCore != 0) NativeWasResized(mNativeContentViewCore);
    }
}

/**
 * Returns a delegate that can be used to add and remove views from the ContainerView.
 *
 * NOTE: Use with care, as not all ContentViewCore users setup their ContainerView in the same
 * way. In particular, the Android WebView has limitations on what implementation details can
 * be provided via a child view, as they are visible in the API and could introduce
 * compatibility breaks with existing applications. If in doubt, contact the
 * android_webview/OWNERS
 *
 * @return A ViewAndroidDelegate that can be used to add and remove views.
 */
//@VisibleForTesting
AutoPtr<ViewAndroidDelegate> ContentViewCore::GetViewAndroidDelegate()
{
    return new InnerViewAndroidDelegate(this);
}

//@VisibleForTesting
void ContentViewCore::SetImeAdapterForTest(
    /* [in] */ ImeAdapter* imeAdapter)
{
    mImeAdapter = imeAdapter;
}

//@VisibleForTesting
AutoPtr<ImeAdapter> ContentViewCore::GetImeAdapterForTest()
{
    return mImeAdapter;
}

//@VisibleForTesting
void ContentViewCore::SetAdapterInputConnectionFactory(
    /* [in] */ AdapterInputConnectionFactory* factory)
{
    mAdapterInputConnectionFactory = factory;
}

//@VisibleForTesting
void ContentViewCore::SetInputMethodManagerWrapperForTest(
    /* [in] */ InputMethodManagerWrapper* immw)
{
    mInputMethodManagerWrapper = immw;
}

//@VisibleForTesting
AutoPtr<AdapterInputConnection> ContentViewCore::GetInputConnectionForTest()
{
    return mInputConnection;
}

AutoPtr<ImeAdapter> ContentViewCore::CreateImeAdapter(
    /* [in] */ IContext* context)
{
    AutoPtr<ImeAdapterDelegate> imeAdapterDelegate = new InnerImeAdapterDelegate(this);
    AutoPtr<ImeAdapter> imeAdapter = new ImeAdapter(mInputMethodManagerWrapper, imeAdapterDelegate);
    return imeAdapter;
}

/**
 *
 * @param containerView The view that will act as a container for all views created by this.
 * @param internalDispatcher Handles dispatching all hidden or super methods to the
 *                           containerView.
 * @param nativeWebContents A pointer to the native web contents.
 * @param windowAndroid An instance of the WindowAndroid.
 */
// Perform important post-construction set up of the ContentViewCore.
// We do not require the containing view in the constructor to allow embedders to create a
// ContentViewCore without having fully created its containing view. The containing view
// is a vital component of the ContentViewCore, so embedders must exercise caution in what
// they do with the ContentViewCore before calling initialize().
// We supply the nativeWebContents pointer here rather than in the constructor to allow us
// to set the private browsing mode at a later point for the WebView implementation.
// Note that the caller remains the owner of the nativeWebContents and is responsible for
// deleting it after destroying the ContentViewCore.
void ContentViewCore::Initialize(
    /* [in] */ IViewGroup* containerView,
    /* [in] */ InternalAccessDelegate* internalDispatcher,
    /* [in] */ Int64 nativeWebContents,
    /* [in] */ WindowAndroid* windowAndroid)
{
    setContainerView(containerView);

    mPositionListener = new InnerListener(this);

    Int64 windowNativePointer = windowAndroid != NULL ? windowAndroid->GetNativePointer() : 0;

    Int64 viewAndroidNativePointer = 0;
    if (windowNativePointer != 0) {
        mViewAndroid = new ViewAndroid(windowAndroid, GetViewAndroidDelegate());
        viewAndroidNativePointer = mViewAndroid->GetNativePointer();
    }

    mZoomControlsDelegate = new InnerZoomControlsDelegate(this);

    mNativeContentViewCore = NativeInit(
            nativeWebContents, viewAndroidNativePointer, windowNativePointer,
            mRetainedJavaScriptObjects);
    mWebContents = NativeGetWebContentsAndroid(mNativeContentViewCore);
    mContentSettings = new ContentSettings(this, mNativeContentViewCore);

    SetContainerViewInternals(internalDispatcher);
    mRenderCoordinates->Reset();
    InitPopupZoomer(mContext);
    mImeAdapter = CreateImeAdapter(mContext);

    mAccessibilityInjector = AccessibilityInjector::NewInstance(this);

    mWebContentsObserver = new InnerWebContentsObserverAndroid(this);
}

/**
 * Sets a new container view for this {@link ContentViewCore}.
 *
 * <p>WARNING: This is not a general purpose method and has been designed with WebView
 * fullscreen in mind. Please be aware that it might not be appropriate for other use cases
 * and that it has a number of limitations. For example the PopupZoomer only works with the
 * container view with which this ContentViewCore has been initialized.
 *
 * <p>This method only performs a small part of replacing the container view and
 * embedders are responsible for:
 * <ul>
 *     <li>Disconnecting the old container view from this ContentViewCore</li>
 *     <li>Updating the InternalAccessDelegate</li>
 *     <li>Reconciling the state of this ContentViewCore with the new container view</li>
 *     <li>Tearing down and recreating the native GL rendering where appropriate</li>
 *     <li>etc.</li>
 * </ul>
 */
void ContentViewCore::SetContainerView(
    /* [in] */ IViewGroup* containerView)
{
    TraceEvent::Begin();
    if (mContainerView != NULL) {
        mPositionObserver->RemoveListener(mPositionListener);
        mSelectionHandleController = NULL;
        mInsertionHandleController = NULL;
        mInputConnection = NULL;
    }

    mContainerView = containerView;
    mPositionObserver = new ViewPositionObserver(mContainerView);
    String contentDescription("Web View");
    if (R::string::accessibility_content_view == 0) {
//        Log.w(TAG, "Setting contentDescription to 'Web View' as no value was specified.");
    }
    else {
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        res->GetString(
                R::string::accessibility_content_view,
                &contentDescription);
    }

    mContainerView->SetContentDescription(contentDescription);
    mContainerView->SetWillNotDraw(FALSE);
    mContainerView->SetClickable(TRUE);
    TraceEvent::End();
}

//@CalledByNative
void ContentViewCore::OnNativeContentViewCoreDestroyed(
    /* [in] */ Int64 nativeContentViewCore)
{
    assert(nativeContentViewCore == mNativeContentViewCore);
    mNativeContentViewCore = 0;
}

/**
 * Set the Container view Internals.
 * @param internalDispatcher Handles dispatching all hidden or super methods to the
 *                           containerView.
 */
void ContentViewCore::SetContainerViewInternals(
    /* [in] */ InternalAccessDelegate* internalDispatcher)
{
    mContainerViewInternals = internalDispatcher;
}

void ContentViewCore::InitPopupZoomer(
    /* [in] */ IContext* context)
{
    mPopupZoomer = new PopupZoomer(context);
    AutoPtr<PopupZoomer::OnVisibilityChangedListener> changedListener = new InnerOnVisibilityChangedListener(this);
    mPopupZoomer->SetOnVisibilityChangedListener(changedListener);

    // TODO(yongsheng): LONG_TAP is not enabled in PopupZoomer. So need to dispatch a LONG_TAP
    // gesture if a user completes a tap on PopupZoomer UI after a LONG_PRESS gesture.
    AutoPtr<PopupZoomer::OnTapListener> listener = InnerOnTapListener(this, mContainerView);
    mPopupZoomer.setOnTapListener(listener);
}

/**
 * Destroy the internal state of the ContentView. This method may only be
 * called after the ContentView has been removed from the view system. No
 * other methods may be called on this ContentView after this method has
 * been called.
 */
void ContentViewCore::Destroy()
{
    if (mNativeContentViewCore != 0) {
        NativeOnJavaContentViewCoreDestroyed(mNativeContentViewCore);
    }

    mWebContents = NULL;
    if (mViewAndroid != NULL) {
        mViewAndroid->Destroy();
    }
    mNativeContentViewCore = 0;
    mContentSettings = NULL;
    mJavaScriptInterfaces->Clear();
    mRetainedJavaScriptObjects->Clear();
    UnregisterAccessibilityContentObserver();
    mGestureStateListeners->Clear();
    ScreenOrientationListener::GetInstance()->RemoveObserver(this);
}

void ContentViewCore::UnregisterAccessibilityContentObserver()
{
    if (mAccessibilityScriptInjectionObserver == NULL) {
        return;
    }

    AutoPtr<IContentResolver> resolver;
    GetContext()->GetContentResolver((IContentResolver**)&resolver);
    resolver->UnregisterContentObserver(
            mAccessibilityScriptInjectionObserver);
    mAccessibilityScriptInjectionObserver = NULL;
}

/**
 * Returns true initially, false after destroy() has been called.
 * It is illegal to call any other public method after destroy().
 */
Boolean ContentViewCore::IsAlive()
{
    return mNativeContentViewCore != 0;
}

/**
 * This is only useful for passing over JNI to native code that requires ContentViewCore*.
 * @return native ContentViewCore pointer.
 */
//@CalledByNative
Int64 ContentViewCore::GetNativeContentViewCore()
{
    return mNativeContentViewCore;
}

void ContentViewCore::SetContentViewClient(
    /* [in] */ IContentViewClient* client)
{
    if (client == NULL) {
        //throw new IllegalArgumentException("The client can't be null.");
        assert(0);
    }

    mContentViewClient = client;
}

//@VisibleForTesting
AutoPtr<ContentViewClient> ContentViewCore::GetContentViewClient()
{
    if (mContentViewClient == NULL) {
        // We use the Null Object pattern to avoid having to perform a null check in this class.
        // We create it lazily because most of the time a client will be set almost immediately
        // after ContentView is created.
        mContentViewClient = new ContentViewClient();
        // We don't set the native ContentViewClient pointer here on purpose. The native
        // implementation doesn't mind a null delegate and using one is better than passing a
        // Null Object, since we cut down on the number of JNI calls.
    }

    return mContentViewClient;
}

Int32 ContentViewCore::GetBackgroundColor()
{
    if (mNativeContentViewCore != 0) {
        return NativeGetBackgroundColor(mNativeContentViewCore);
    }

    return IColor::WHITE;
}

//@CalledByNative
void ContentViewCore::OnBackgroundColorChanged(
    /* [in] */ Int32 color)
{
    GetContentViewClient()->OnBackgroundColorChanged(color);
}

/**
 * Load url without fixing up the url string. Consumers of ContentView are responsible for
 * ensuring the URL passed in is properly formatted (i.e. the scheme has been added if left
 * off during user input).
 *
 * @param params Parameters for this load.
 */
void ContentViewCore::LoadUrl(
    /* [in] */ LoadUrlParams* params)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeLoadUrl(mNativeContentViewCore,
            params->mUrl,
            params->mLoadUrlType,
            params->mTransitionType,
            params->GetReferrer() != NULL ? params->GetReferrer()->GetUrl() : NULL,
            params->GetReferrer() != NULL ? params->GetReferrer()->GetPolicy() : 0,
            params->mUaOverrideOption,
            params->GetExtraHeadersString(),
            params->mPostData,
            params->mBaseUrlForDataUrl,
            params->mVirtualUrlForDataUrl,
            params->mCanLoadLocalResources,
            params->mIsRendererInitiated);
}

/**
 * Stops loading the current web contents.
 */
void ContentViewCore::StopLoading()
{
    if (mWebContents != NULL) {
        mWebContents->Stop();
    }
}

/**
 * Get the URL of the current page.
 *
 * @return The URL of the current page.
 */
String ContentViewCore::GetUrl()
{
    if (mNativeContentViewCore != 0) {
        return NativeGetURL(mNativeContentViewCore);
    }

    return NULL;
}

/**
 * Get the title of the current page.
 *
 * @return The title of the current page.
 */
String ContentViewCore::GetTitle()
{
    return mWebContents == NULL ? NULL : mWebContents->GetTitle();
}

/**
 * Shows an interstitial page driven by the passed in delegate.
 *
 * @param url The URL being blocked by the interstitial.
 * @param delegate The delegate handling the interstitial.
 */
//@VisibleForTesting
void ContentViewCore::ShowInterstitialPage(
    /* [in] */ const String& url,
    /* [in] */ InterstitialPageDelegateAndroid* delegate)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeShowInterstitialPage(mNativeContentViewCore, url, delegate->GetNative());
}

/**
 * @return Whether the page is currently showing an interstitial, such as a bad HTTPS page.
 */
Boolean ContentViewCore::IsShowingInterstitialPage()
{
    return mNativeContentViewCore == 0 ?
            FALSE : NativeIsShowingInterstitialPage(mNativeContentViewCore);
}

/**
 * @return Viewport width in physical pixels as set from onSizeChanged.
 */
//@CalledByNative
Int32 ContentViewCore::GetViewportWidthPix()
{
    return mViewportWidthPix;
}

/**
 * @return Viewport height in physical pixels as set from onSizeChanged.
 */
//@CalledByNative
Int32 ContentViewCore::GetViewportHeightPix()
{
    return mViewportHeightPix;
}

/**
 * @return Width of underlying physical surface.
 */
//@CalledByNative
Int32 ContentViewCore::GetPhysicalBackingWidthPix()
{
    return mPhysicalBackingWidthPix;
}

/**
 * @return Height of underlying physical surface.
 */
//@CalledByNative
Int32 ContentViewCore::GetPhysicalBackingHeightPix()
{
    return mPhysicalBackingHeightPix;
}

/**
 * @return Amount the output surface extends past the bottom of the window viewport.
 */
//@CalledByNative
Int32 ContentViewCore::GetOverdrawBottomHeightPix()
{
    return mOverdrawBottomHeightPix;
}

/**
 * @return The amount to shrink the viewport relative to {@link #getViewportWidthPix()}.
 */
//@CalledByNative
Int32 ContentViewCore::GetViewportSizeOffsetWidthPix()
{
    return mViewportSizeOffsetWidthPix;
}

/**
 * @return The amount to shrink the viewport relative to {@link #getViewportHeightPix()}.
 */
//@CalledByNative
Int32 ContentViewCore::GetViewportSizeOffsetHeightPix()
{
    return mViewportSizeOffsetHeightPix;
}

/**
 * @see android.webkit.WebView#getContentHeight()
 */
Float ContentViewCore::GetContentHeightCss()
{
    return mRenderCoordinates->GetContentHeightCss();
}

/**
 * @see android.webkit.WebView#getContentWidth()
 */
Float ContentViewCore::GetContentWidthCss()
{
    return mRenderCoordinates->GetContentWidthCss();
}

// TODO(teddchoc): Remove all these navigation controller methods from here and have the
//                 embedders manage it.
/**
 * @return Whether the current WebContents has a previous navigation entry.
 */
Boolean ContentViewCore::CanGoBack()
{
    return mWebContents != NULL && mWebContents->GetNavigationController()->CanGoBack();
}

/**
 * @return Whether the current WebContents has a navigation entry after the current one.
 */
Boolean ContentViewCore::CanGoForward()
{
    return mWebContents != NULL && mWebContents->GetNavigationController()->CanGoForward();
}

/**
 * @param offset The offset into the navigation history.
 * @return Whether we can move in history by given offset
 */
Boolean ContentViewCore::CanGoToOffset(
    /* [in] */ Int32 offset)
{
    return mWebContents != NULL &&
            mWebContents->GetNavigationController()->CanGoToOffset(offset);
}

/**
 * Navigates to the specified offset from the "current entry". Does nothing if the offset is out
 * of bounds.
 * @param offset The offset into the navigation history.
 */
void ContentViewCore::GoToOffset(
    /* [in] */ Int32 offset)
{
    if (mWebContents != NULL) {
        mWebContents->GetNavigationController()->GoToOffset(offset);
    }
}

//@Override
void ContentViewCore::GoToNavigationIndex(
    /* [in] */ Int32 index)
{
    if (mWebContents != NULL) {
        mWebContents->GetNavigationController()->GoToNavigationIndex(index);
    }
}

/**
 * Goes to the navigation entry before the current one.
 */
void ContentViewCore::GoBack()
{
    if (mWebContents != NULL) {
        mWebContents->GetNavigationController()->GoBack();
    }
}

/**
 * Goes to the navigation entry following the current one.
 */
void ContentViewCore::GoForward()
{
    if (mWebContents != NULL) {
        mWebContents->GetNavigationController()->GoForward();
    }
}

/**
 * Loads the current navigation if there is a pending lazy load (after tab restore).
 */
void ContentViewCore::LoadIfNecessary()
{
    if (mNativeContentViewCore != 0) {
        NativeLoadIfNecessary(mNativeContentViewCore);
    }
}

/**
 * Requests the current navigation to be loaded upon the next call to loadIfNecessary().
 */
void ContentViewCore::RequestRestoreLoad()
{
    if (mNativeContentViewCore != 0) {
        NativeRequestRestoreLoad(mNativeContentViewCore);
    }
}

/**
 * Reload the current page.
 */
void ContentViewCore::Reload(
    /* [in] */ Boolean checkForRepost)
{
    mAccessibilityInjector->AddOrRemoveAccessibilityApisIfNecessary();
    if (mNativeContentViewCore != 0) {
        NativeReload(mNativeContentViewCore, checkForRepost);
    }
}

/**
 * Reload the current page, ignoring the contents of the cache.
 */
void ContentViewCore::ReloadIgnoringCache(
    /* [in] */ Boolean checkForRepost)
{
    mAccessibilityInjector->AddOrRemoveAccessibilityApisIfNecessary();
    if (mNativeContentViewCore != 0) {
        NativeReloadIgnoringCache(mNativeContentViewCore, checkForRepost);
    }
}

/**
 * Cancel the pending reload.
 */
void ContentViewCore::CancelPendingReload()
{
    if (mNativeContentViewCore != 0) {
        NativeCancelPendingReload(mNativeContentViewCore);
    }
}

/**
 * Continue the pending reload.
 */
void ContentViewCore::ContinuePendingReload()
{
    if (mNativeContentViewCore != 0) {
        NativeContinuePendingReload(mNativeContentViewCore);
    }
}

/**
 * Clears the ContentViewCore's page history in both the backwards and
 * forwards directions.
 */
void ContentViewCore::ClearHistory()
{
    if (mNativeContentViewCore != 0) {
        NativeClearHistory(mNativeContentViewCore);
    }
}

/**
 * @return The selected text (empty if no text selected).
 */
String ContentViewCore::GetSelectedText()
{
    return mHasSelection ? mLastSelectedText : String("");
}

/**
 * @return Whether the current selection is editable (false if no text selected).
 */
Boolean ContentViewCore::IsSelectionEditable()
{
    return mHasSelection ? mSelectionEditable : FALSE;
}

// End FrameLayout overrides.

/**
 * TODO(changwan): refactor SPen related code into a separate class. See
 * http://crbug.com/398169.
 * @return Whether SPen is supported on the device.
 */
Boolean ContentViewCore::IsSPenSupported(
    /* [in] */ IContext* context)
{
    if (sIsSPenSupported == NULL) {
        sIsSPenSupported = DetectSPenSupport(context);
    }

    return sIsSPenSupported.booleanValue();
}

Boolean ContentViewCore::DetectSPenSupport(
    /* [in] */ IContext* context)
{
    if (!String("SAMSUNG").EqualsIgnoreCase(Build.MANUFACTURER)) {
        return FALSE;
    }

    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    AutoPtr< ArrayOf<IFeatureInfo*> > infos;
    infos->GetSystemAvailableFeatures((ArrayOf<IFeatureInfo*>**)&infos);
    Int32 length = infos->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        String name;
        (*infos)[i]->GetName(&name);
        if (String("com.sec.feature.spen_usp").EqualsIgnoreCase(name)) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Convert SPen event action into normal event action.
 * TODO(changwan): refactor SPen related code into a separate class. See
 * http://crbug.com/398169.
 *
 * @param eventActionMasked Input event action. It is assumed that it is masked as the values
                            cannot be ORed.
 * @return Event action after the conversion
 */
Int32 ContentViewCore::ConvertSPenEventAction(
    /* [in] */ Int32 eventActionMasked)
{
    // S-Pen support: convert to normal stylus event handling
    switch (eventActionMasked) {
        case SPEN_ACTION_DOWN:
            return IMotionEvent::ACTION_DOWN;
        case SPEN_ACTION_UP:
            return IMotionEvent::ACTION_UP;
        case SPEN_ACTION_MOVE:
            return IMotionEvent::ACTION_MOVE;
        case SPEN_ACTION_CANCEL:
            return IMotionEvent::ACTION_CANCEL;
        default:
            return eventActionMasked;
    }
}

/**
 * @see View#onTouchEvent(MotionEvent)
 */
Boolean ContentViewCore::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    TraceEvent::Begin("onTouchEvent");
    // try {
        CancelRequestToScrollFocusedEditableNodeIntoView();

        Int32 eventAction;
        event->GetActionMasked(&eventAction);

        if (IsSPenSupported(mContext)) {
            eventAction = ConvertSPenEventAction(eventAction);
        }

        // Only these actions have any effect on gesture detection.  Other
        // actions have no corresponding WebTouchEvent type and may confuse the
        // touch pipline, so we ignore them entirely.
        if (eventAction != IMotionEvent::ACTION_DOWN
                && eventAction != IMotionEvent::ACTION_UP
                && eventAction != IMotionEvent::ACTION_CANCEL
                && eventAction != IMotionEvent::ACTION_MOVE
                && eventAction != IMotionEvent::ACTION_POINTER_DOWN
                && eventAction != IMotionEvent::ACTION_POINTER_UP) {
            return FALSE;
        }

        if (mNativeContentViewCore == 0) {
            return FALSE;
        }

        // A zero offset is quite common, in which case the unnecessary copy should be avoided.
        AutoPtr<IMotionEvent> offset;
        if (mCurrentTouchOffsetX != 0 || mCurrentTouchOffsetY != 0) {
            offset = CreateOffsetMotionEvent(event);
            event = offset;
        }

        Int32 pointerCount;
        event->GetPointerCount(&pointerCount);
        Int32 eventTime;
        event->GetEventTime(&eventTime);
        Int32 historySize;
        event->GetHistorySize(&historySize);
        Int32 actionIndex;
        event->GetActionIndex(&actionIndex);
        Int32 x, y;
        event->GetX(&x);
        event->GetY(&y);
        Int32 x1, y1;
        event->GetX(1, &x1);
        event->GetY(1, &y1);
        Int32 id0, id1;
        event->GetPointerId(0, &id0);
        event->GetPointerId(0, &id1);
        Int32 major0, major1;
        event->GetTouchMajor(&major0);
        event->GetTouchMajor(1, &major1);
        Int32 rawX, rawY;
        event->GetRawX(&rawX);
        event->GetRawY(&rawY);
        Int32 type0, type1;
        event->GetToolType(0, type0);
        event->GetToolType(1, type1);
        Int32 state;
        event->GetButtonState(&state);
        const Boolean consumed = NativeOnTouchEvent(mNativeContentViewCore, event,
                eventTime, eventAction,
                pointerCount, historySize, actionIndex,
                x, y,
                pointerCount > 1 ? x1 : 0,
                pointerCount > 1 ? y1 : 0,
                id0, pointerCount > 1 ? id1 : -1,
                major0, pointerCount > 1 ? major1 : 0,
                rawX,rawY,
                type0,
                pointerCount > 1 ? type1 : MotionEvent.TOOL_TYPE_UNKNOWN,
                state);

        if (offset != NULL) {
            offset->Recycle();
        }

        return consumed;
    // } finally {
    //     TraceEvent.end("onTouchEvent");
    // }
}

void ContentViewCore::SetIgnoreRemainingTouchEvents()
{
    ResetGestureDetection();
}

Boolean ContentViewCore::IsScrollInProgress()
{
    return mTouchScrollInProgress || mPotentiallyActiveFlingCount > 0;
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnFlingStartEventConsumed(
    /* [in] */ Int32 vx,
    /* [in] */ Int32 vy)
{
    mTouchScrollInProgress = FALSE;
    mPotentiallyActiveFlingCount++;
    TemporarilyHideTextHandles();
    for (mGestureStateListenersIterator->Rewind();
                mGestureStateListenersIterator->HasNext();) {
        mGestureStateListenersIterator->Next()->OnFlingStartGesture(
                vx, vy, ComputeVerticalScrollOffset(), ComputeVerticalScrollExtent());
    }
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnFlingStartEventHadNoConsumer(
    /* [in] */ Int32 vx,
    /* [in] */ Int32 vy)
{
    mTouchScrollInProgress = FALSE;
    for (mGestureStateListenersIterator->Rewind();
                mGestureStateListenersIterator->HasNext();) {
        mGestureStateListenersIterator->Next()->OnUnhandledFlingStartEvent(vx, vy);
    }
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnFlingCancelEventAck()
{
    UpdateGestureStateListener(GestureEventType::FLING_CANCEL);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnScrollBeginEventAck()
{
    mTouchScrollInProgress = TRUE;
    TemporarilyHideTextHandles();
    mZoomControlsDelegate->InvokeZoomPicker();
    UpdateGestureStateListener(GestureEventType::SCROLL_START);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnScrollUpdateGestureConsumed()
{
    mZoomControlsDelegate->InvokeZoomPicker();
    for (mGestureStateListenersIterator->Rewind();
            mGestureStateListenersIterator->HasNext();) {
        mGestureStateListenersIterator->Next()->OnScrollUpdateGestureConsumed();
    }
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnScrollEndEventAck()
{
    if (!mTouchScrollInProgress) {
        return;
    }
    mTouchScrollInProgress = FALSE;
    UpdateGestureStateListener(GestureEventType::SCROLL_END);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnPinchBeginEventAck()
{
    TemporarilyHideTextHandles();
    UpdateGestureStateListener(GestureEventType::PINCH_BEGIN);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnPinchEndEventAck()
{
    UpdateGestureStateListener(GestureEventType::PINCH_END);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnSingleTapEventAck(
    /* [in] */ Boolean consumed,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    for (mGestureStateListenersIterator->Rewind();
            mGestureStateListenersIterator->HasNext();) {
        mGestureStateListenersIterator->Next()->OnSingleTap(consumed, x, y);
    }
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnDoubleTapEventAck()
{
    TemporarilyHideTextHandles();
}

/**
 * Called just prior to a tap or press gesture being forwarded to the renderer.
 */
//@SuppressWarnings("unused")
//@CalledByNative
Boolean ContentViewCore::FilterTapOrPressEvent(
    /* [in] */ Int32 type,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (type == GestureEventType::LONG_PRESS && OfferLongPressToEmbedder()) {
        return TRUE;
    }

    UpdateForTapOrPress(type, x, y);

    return FALSE;
}

//@VisibleForTesting
void ContentViewCore::SendDoubleTapForTest(
    /* [in] */ Int64 timeMs,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeDoubleTap(mNativeContentViewCore, timeMs, x, y);
}

//@VisibleForTesting
void ContentViewCore::FlingForTest(
    /* [in] */ Int64 timeMs,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeFlingCancel(mNativeContentViewCore, timeMs);
    NativeScrollBegin(mNativeContentViewCore, timeMs, x, y, velocityX, velocityY);
    NativeFlingStart(mNativeContentViewCore, timeMs, x, y, velocityX, velocityY);
}

/**
 * Cancel any fling gestures active.
 * @param timeMs Current time (in milliseconds).
 */
void ContentViewCore::CancelFling(
    /* [in] */ Int64 timeMs)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeFlingCancel(mNativeContentViewCore, timeMs);
}

/**
 * Add a listener that gets alerted on gesture state changes.
 * @param listener Listener to add.
 */
void ContentViewCore::AddGestureStateListener(
    /* [in] */ GestureStateListener* listener)
{
    mGestureStateListeners->AddObserver(listener);
}

/**
 * Removes a listener that was added to watch for gesture state changes.
 * @param listener Listener to remove.
 */
void ContentViewCore::RemoveGestureStateListener(
    /* [in] */ GestureStateListener* listener)
{
    mGestureStateListeners->RemoveObserver(listener);
}

void ContentViewCore::UpdateGestureStateListener(
    /* [in] */ Int32 gestureType)
{
    for (mGestureStateListenersIterator->Rewind();
            mGestureStateListenersIterator->HasNext();) {
        GestureStateListener listener = mGestureStateListenersIterator->Next();
        switch (gestureType) {
            case GestureEventType::PINCH_BEGIN:
                listener->OnPinchStarted();
                break;
            case GestureEventType::PINCH_END:
                listener->OnPinchEnded();
                break;
            case GestureEventType::FLING_END:
                listener->OnFlingEndGesture(
                        ComputeVerticalScrollOffset(),
                        ComputeVerticalScrollExtent());
                break;
            case GestureEventType::FLING_CANCEL:
                listener->OnFlingCancelGesture();
                break;
            case GestureEventType::SCROLL_START:
                listener->OnScrollStarted(
                        ComputeVerticalScrollOffset(),
                        ComputeVerticalScrollExtent());
                break;
            case GestureEventType->SCROLL_END:
                listener->OnScrollEnded(
                        ComputeVerticalScrollOffset(),
                        ComputeVerticalScrollExtent());
                break;
            default:
                break;
        }
    }
}

/**
 * Requests the renderer insert a link to the specified stylesheet in the
 * main frame's document.
 */
void ContentViewCore::AddStyleSheetByURL(
    /* [in] */ const String& url)
{
    NativeAddStyleSheetByURL(mNativeContentViewCore, url);
}

/**
 * Injects the passed Javascript code in the current page and evaluates it.
 * If a result is required, pass in a callback.
 * Used in automation tests.
 *
 * @param script The Javascript to execute.
 * @param callback The callback to be fired off when a result is ready. The script's
 *                 result will be json encoded and passed as the parameter, and the call
 *                 will be made on the main thread.
 *                 If no result is required, pass null.
 */
void ContentViewCore::EvaluateJavaScript(
    /* [in] */ const String& script,
    /* [in] */ JavaScriptCallback* callback)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeEvaluateJavaScript(mNativeContentViewCore, script, callback, FALSE);
}

/**
 * Injects the passed Javascript code in the current page and evaluates it.
 * If there is no page existing, a new one will be created.
 *
 * @param script The Javascript to execute.
 */
void ContentViewCore::EvaluateJavaScriptEvenIfNotYetNavigated(
    /* [in] */ const String& script)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeEvaluateJavaScript(mNativeContentViewCore, script, NULL, TRUE);
}

/**
 * To be called when the ContentView is shown.
 */
void ContentViewCore::OnShow()
{
    assert(mNativeContentViewCore != 0);
    NativeOnShow(mNativeContentViewCore);
    Boolean bFlag = FALSE;
    mAccessibilityManager->IsEnabled(&bFlag);
    SetAccessibilityState(bFlag);
}

/**
 * @return The ID of the renderer process that backs this tab or
 *         {@link #INVALID_RENDER_PROCESS_PID} if there is none.
 */
Int32 ContentViewCore::GetCurrentRenderProcessId()
{
    return NativeGetCurrentRenderProcessId(mNativeContentViewCore);
}

/**
 * To be called when the ContentView is hidden.
 */
void ContentViewCore::OnHide()
{
    assert(mNativeContentViewCore != 0);
    HidePopups();
    SetInjectedAccessibility(FALSE);
    NativeOnHide(mNativeContentViewCore);
}

/**
 * Return the ContentSettings object used to retrieve the settings for this
 * ContentViewCore. For modifications, ChromeNativePreferences is to be used.
 * @return A ContentSettings object that can be used to retrieve this
 *         ContentViewCore's settings.
 */
AutoPtr<ContentSettings> ContentViewCore::GetContentSettings()
{
    return mContentSettings;
}

void ContentViewCore::HidePopups()
{
    HideSelectPopup();
    HideHandles();
    HideSelectActionBar();
}

void ContentViewCore::HideSelectActionBar()
{
    if (mActionMode != NULL) {
        mActionMode->Finish();
        mActionMode = NULL;
    }
}

Boolean ContentViewCore::IsSelectActionBarShowing()
{
    return mActionMode != NULL;
}

void ContentViewCore::ResetGestureDetection()
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeResetGestureDetection(mNativeContentViewCore);
}

/**
 * @see View#onAttachedToWindow()
 */
//@SuppressWarnings("javadoc")
void ContentViewCore::OnAttachedToWindow()
{
    SetAccessibilityState(mAccessibilityManager->IsEnabled());

    ScreenOrientationListener::GetInstance()->AddObserver(this, mContext);
    GamepadList::OnAttachedToWindow(mContext);
}

/**
 * @see View#onDetachedFromWindow()
 */
//@SuppressWarnings("javadoc")
//@SuppressLint("MissingSuperCall")
void ContentViewCore::OnDetachedFromWindow()
{
    SetInjectedAccessibility(FALSE);
    HidePopups();
    mZoomControlsDelegate->DismissZoomPicker();
    UnregisterAccessibilityContentObserver();

    ScreenOrientationListener::GetInstance()->RemoveObserver(this);
    GamepadList::OnDetachedFromWindow();
}

/**
 * @see View#onVisibilityChanged(android.view.View, int)
 */
void ContentViewCore::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    if (visibility != IView::VISIBLE) {
        mZoomControlsDelegate->DismissZoomPicker();
    }
}

/**
 * @see View#onCreateInputConnection(EditorInfo)
 */
AutoPtr<InputConnection> ContentViewCore::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    if (!mImeAdapter->HasTextInputType()) {
        // Although onCheckIsTextEditor will return false in this case, the EditorInfo
        // is still used by the InputMethodService. Need to make sure the IME doesn't
        // enter fullscreen mode.
        outAttrs->SetImeOptions(IEditorInfo::IME_FLAG_NO_FULLSCREEN);
    }

    mInputConnection = mAdapterInputConnectionFactory->Get(mContainerView, mImeAdapter,
            mEditable, outAttrs);

    return mInputConnection;
}

//@VisibleForTesting
AutoPtr<AdapterInputConnection> ContentViewCore::GetAdapterInputConnectionForTest()
{
    return mInputConnection;
}

//@VisibleForTesting
AutoPtr<IEditable> ContentViewCore::GetEditableForTest()
{
    return mEditable;
}

/**
 * @see View#onCheckIsTextEditor()
 */
Boolean ContentViewCore::OnCheckIsTextEditor()
{
    return mImeAdapter->HasTextInputType();
}

/**
 * @see View#onConfigurationChanged(Configuration)
 */
//@SuppressWarnings("javadoc")
void ContentViewCore::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    TraceEvent::Begin();

    Int32 keyboard;
    newConfig->GetKeyboard(&keyboard);
    if (keyboard != IConfiguration::KEYBOARD_NOKEYS) {
        if (mNativeContentViewCore != 0) {
            mImeAdapter->Attach(nativeGetNativeImeAdapter(mNativeContentViewCore),
                    ImeAdapter::GetTextInputTypeNone());
        }
        mInputMethodManagerWrapper->RestartInput(mContainerView);
    }
    mContainerViewInternals->Super_onConfigurationChanged(newConfig);

    // To request layout has side effect, but it seems OK as it only happen in
    // onConfigurationChange and layout has to be changed in most case.
    mContainerView->RequestLayout();
    TraceEvent::End();
}

/**
 * @see View#onSizeChanged(int, int, int, int)
 */
//@SuppressWarnings("javadoc")
void ContentViewCore::OnSizeChanged(
    /* [in] */ Int32 wPix,
    /* [in] */ Int32 hPix,
    /* [in] */ Int32 owPix,
    /* [in] */ Int32 ohPix)
{
    if (GetViewportWidthPix() == wPix && GetViewportHeightPix() == hPix) {
        return;
    }

    mViewportWidthPix = wPix;
    mViewportHeightPix = hPix;
    if (mNativeContentViewCore != 0) {
        NativeWasResized(mNativeContentViewCore);
    }

    UpdateAfterSizeChanged();
}

/**
 * Called when the underlying surface the compositor draws to changes size.
 * This may be larger than the viewport size.
 */
void ContentViewCore::OnPhysicalBackingSizeChanged(
    /* [in] */ Int32 wPix,
    /* [in] */ Int32 hPix)
{
    if (mPhysicalBackingWidthPix == wPix && mPhysicalBackingHeightPix == hPix) {
        return;
    }

    mPhysicalBackingWidthPix = wPix;
    mPhysicalBackingHeightPix = hPix;

    if (mNativeContentViewCore != 0) {
        NativeWasResized(mNativeContentViewCore);
    }
}

/**
 * Called when the amount the surface is overdrawing off the bottom has changed.
 * @param overdrawHeightPix The overdraw height.
 */
void ContentViewCore::OnOverdrawBottomHeightChanged(
    /* [in] */ Int32 overdrawHeightPix)
{
    if (mOverdrawBottomHeightPix == overdrawHeightPix) {
        return;
    }

    mOverdrawBottomHeightPix = overdrawHeightPix;

    if (mNativeContentViewCore != 0) {
        NativeWasResized(mNativeContentViewCore);
    }
}

void ContentViewCore::UpdateAfterSizeChanged()
{
    mPopupZoomer->Hide(FALSE);

    // Execute a delayed form focus operation because the OSK was brought
    // up earlier.
    if (!mFocusPreOSKViewportRect->IsEmpty()) {
        AutoPtr<IRect> rect;
        CRect::New((IRect**)&rect);
        GetContainerView()->GetWindowVisibleDisplayFrame(rect);
        Boolean bFlag = FALSE;
        rect->Equals(mFocusPreOSKViewportRect, &bFlag);
        if (!bFlag) {
            // Only assume the OSK triggered the onSizeChanged if width was preserved.
            Int32 width;
            rect->GetWidth(&width);
            if (width == mFocusPreOSKViewportRect->Width()) {
                ScrollFocusedEditableNodeIntoView();
            }
            CancelRequestToScrollFocusedEditableNodeIntoView();
        }
    }
}

void ContentViewCore::CancelRequestToScrollFocusedEditableNodeIntoView()
{
    // Zero-ing the rect will prevent |updateAfterSizeChanged()| from
    // issuing the delayed form focus event.
    mFocusPreOSKViewportRect->SetEmpty();
}

void ContentViewCore::ScrollFocusedEditableNodeIntoView()
{
    if (mNativeContentViewCore == 0) return;
    // The native side keeps track of whether the zoom and scroll actually occurred. It is
    // more efficient to do it this way and sometimes fire an unnecessary message rather
    // than synchronize with the renderer and always have an additional message.
    NativeScrollFocusedEditableNodeIntoView(mNativeContentViewCore);
}

/**
 * Selects the word around the caret, if any.
 * The caller can check if selection actually occurred by listening to OnSelectionChanged.
 */
void ContentViewCore::SelectWordAroundCaret()
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeSelectWordAroundCaret(mNativeContentViewCore);
}

/**
 * @see View#onWindowFocusChanged(boolean)
 */
void ContentViewCore::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    if (!hasWindowFocus) {
        ResetGestureDetection();
    }
}

void ContentViewCore::OnFocusChanged(
    /* [in] */ Boolean gainFocus)
{
    if (!gainFocus) {
        HideImeIfNeeded();
        CancelRequestToScrollFocusedEditableNodeIntoView();
    }

    if (mNativeContentViewCore != 0) {
        NativeSetFocus(mNativeContentViewCore, gainFocus);
    }
}

/**
 * @see View#onKeyUp(int, KeyEvent)
 */
Boolean ContentViewCore::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mPopupZoomer->IsShowing() && keyCode == IKeyEvent::KEYCODE_BACK) {
        mPopupZoomer->Hide(TRUE);
        return TRUE;
    }

    return mContainerViewInternals->Super_onKeyUp(keyCode, event);
}

/**
 * @see View#dispatchKeyEventPreIme(KeyEvent)
 */
Boolean ContentViewCore::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* event)
{
    // try {
        TraceEvent::Begin();
        return mContainerViewInternals->Super_dispatchKeyEventPreIme(event);
    // } finally {
        TraceEvent::End();
    // }
}

/**
 * @see View#dispatchKeyEvent(KeyEvent)
 */
Boolean ContentViewCore::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if (GamepadList::DispatchKeyEvent(event)) {
        return TRUE;
    }

    if (GetContentViewClient()->ShouldOverrideKeyEvent(event)) {
        return mContainerViewInternals->Super_dispatchKeyEvent(event);
    }

    if (mImeAdapter->DispatchKeyEvent(event)) {
        return TRUE;
    }

    return mContainerViewInternals->Super_dispatchKeyEvent(event);
}

/**
 * @see View#onHoverEvent(MotionEvent)
 * Mouse move events are sent on hover enter, hover move and hover exit.
 * They are sent on hover exit because sometimes it acts as both a hover
 * move and hover exit.
 */
Boolean ContentViewCore::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    TraceEvent::Begin("onHoverEvent");
    AutoPtr<IMotionEvent> offset = CreateOffsetMotionEvent(event);
    //try {
        if (mBrowserAccessibilityManager != NULL) {
            return mBrowserAccessibilityManager->OnHoverEvent(offset);
        }

        // Work around Android bug where the x, y coordinates of a hover exit
        // event are incorrect when touch exploration is on.
        Int32 action;
        offset->GetAction(&action);
        if (mTouchExplorationEnabled && action == IMotionEvent::ACTION_HOVER_EXIT) {
            return TRUE;
        }

        mContainerView->RemoveCallbacks(mFakeMouseMoveRunnable);
        if (mNativeContentViewCore != 0) {
            Int32 time;
            offset->GetEventTime(&time);
            Int32 x, y;
            offset->GetX(&x);
            offset->GetY(&y);
            NativeSendMouseMoveEvent(mNativeContentViewCore, time,
                    x, y);
        }

        return TRUE;
    // } finally {
        offset->Recycle();
        TraceEvent::End("onHoverEvent");
    // }
}

/**
 * @see View#onGenericMotionEvent(MotionEvent)
 */
Boolean ContentViewCore::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    if (GamepadList::OnGenericMotionEvent(event)) {
        return TRUE;
    }

    Int32 source;
    event->GetSource(&source);
    if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
        Int32 action;
        event->GetAction(&action);
        switch (action) {
            case IMotionEvent::ACTION_SCROLL:
                if (mNativeContentViewCore == 0) {
                    return FALSE;
                }

                Int32 time;
                event->GetEventTime(&time);
                Int32 x, y;
                event->GetX(&x);
                event->GetY(&y);
                Int32 value;
                event->GetAxisValue(IMotionEvent::AXIS_VSCROLL, &value);
                NativeSendMouseWheelEvent(mNativeContentViewCore, time,
                        x, y,
                        value);

                mContainerView->RemoveCallbacks(mFakeMouseMoveRunnable);
                // Send a delayed onMouseMove event so that we end
                // up hovering over the right position after the scroll.
                AutoPtr<IMotionEventHelper> helper;
                CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
                AutoPtr<IMotionEvent> eventFakeMouseMove;
                helper->Obtain(event, (IMotionEvent**)&eventFakeMouseMove);
                mFakeMouseMoveRunnable = new FakeMouseMoveRunnable(this, eventFakeMouseMove);
                mContainerView->PostDelayed(mFakeMouseMoveRunnable, 250);
                return TRUE;
        }
    }

    return mContainerViewInternals->Super_onGenericMotionEvent(event);
}

/**
 * Sets the current amount to offset incoming touch events by.  This is used to handle content
 * moving and not lining up properly with the android input system.
 * @param dx The X offset in pixels to shift touch events.
 * @param dy The Y offset in pixels to shift touch events.
 */
void ContentViewCore::SetCurrentMotionEventOffsets(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mCurrentTouchOffsetX = dx;
    mCurrentTouchOffsetY = dy;
}

AutoPtr<IMotionEvent> ContentViewCore::CreateOffsetMotionEvent(
    /* [in] */ IMotionEvent* src)
{
    AutoPtr<IMotionEvent> dst;
    AutoPtr<IMotionEventHelper> helper;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
    helper->Obtain(src, (IMotionEvent**)&dst);
    dst->OffsetLocation(mCurrentTouchOffsetX, mCurrentTouchOffsetY);
    return dst;
}

/**
 * @see View#scrollBy(int, int)
 * Currently the ContentView scrolling happens in the native side. In
 * the Java view system, it is always pinned at (0, 0). scrollBy() and scrollTo()
 * are overridden, so that View's mScrollX and mScrollY will be unchanged at
 * (0, 0). This is critical for drawing ContentView correctly.
 */
void ContentViewCore::ScrollBy(
    /* [in] */ Int32 xPix,
    /* [in] */ Int32 yPix)
{
    if (mNativeContentViewCore != 0) {
        NativeScrollBy(mNativeContentViewCore,
                SystemClock::UptimeMillis(), 0, 0, xPix, yPix);
    }
}

/**
 * @see View#scrollTo(int, int)
 */
void ContentViewCore::ScrollTo(
    /* [in] */ Int32 xPix,
    /* [in] */ Int32 yPix)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    const Float xCurrentPix = mRenderCoordinates->GetScrollXPix();
    const Float yCurrentPix = mRenderCoordinates->GetScrollYPix();
    const Float dxPix = xPix - xCurrentPix;
    const Float dyPix = yPix - yCurrentPix;
    if (dxPix != 0 || dyPix != 0) {
        Int64 time = SystemClock::UptimeMillis();
        NativeScrollBegin(mNativeContentViewCore, time,
                xCurrentPix, yCurrentPix, -dxPix, -dyPix);
        NativeScrollBy(mNativeContentViewCore,
                time, xCurrentPix, yCurrentPix, dxPix, dyPix);
        NativeScrollEnd(mNativeContentViewCore, time);
    }
}

// NOTE: this can go away once ContentView.getScrollX() reports correct values.
//       see: b/6029133
Int32 ContentViewCore::GetNativeScrollXForTest()
{
    return mRenderCoordinates->GetScrollXPixInt();
}

// NOTE: this can go away once ContentView.getScrollY() reports correct values.
//       see: b/6029133
Int32 ContentViewCore::GetNativeScrollYForTest()
{
    return mRenderCoordinates->GetScrollYPixInt();
}

/**
 * @see View#computeHorizontalScrollExtent()
 */
//@SuppressWarnings("javadoc")
Int32 ContentViewCore::ComputeHorizontalScrollExtent()
{
    return mRenderCoordinates->GetLastFrameViewportWidthPixInt();
}

/**
 * @see View#computeHorizontalScrollOffset()
 */
//@SuppressWarnings("javadoc")
Int32 ContentViewCore::ComputeHorizontalScrollOffset()
{
    return mRenderCoordinates->GetScrollXPixInt();
}

/**
 * @see View#computeHorizontalScrollRange()
 */
//@SuppressWarnings("javadoc")
Int32 ContentViewCore::ComputeHorizontalScrollRange()
{
    return mRenderCoordinates->GetContentWidthPixInt();
}

/**
 * @see View#computeVerticalScrollExtent()
 */
//@SuppressWarnings("javadoc")
Int32 ContentViewCore::ComputeVerticalScrollExtent()
{
    return mRenderCoordinates->GetLastFrameViewportHeightPixInt();
}

/**
 * @see View#computeVerticalScrollOffset()
 */
//@SuppressWarnings("javadoc")
Int32 ContentViewCore::ComputeVerticalScrollOffset()
{
    return mRenderCoordinates->GetScrollYPixInt();
}

/**
 * @see View#computeVerticalScrollRange()
 */
//@SuppressWarnings("javadoc")
Int32 ContentViewCore::ComputeVerticalScrollRange()
{
    return mRenderCoordinates->GetContentHeightPixInt();
}

// End FrameLayout overrides.

/**
 * @see View#awakenScrollBars(int, boolean)
 */
//@SuppressWarnings("javadoc")
Boolean ContentViewCore::AwakenScrollBars(
    /* [in] */ Int32 startDelay,
    /* [in] */ Boolean invalidate)
{
    // For the default implementation of ContentView which draws the scrollBars on the native
    // side, calling this function may get us into a bad state where we keep drawing the
    // scrollBars, so disable it by always returning false.
    Int32 style;
    mContainerView->GetScrollBarStyle(&style);
    if (style == IView::SCROLLBARS_INSIDE_OVERLAY) {
        return FALSE;
    }
    else {
        return mContainerViewInternals->Super_awakenScrollBars(startDelay, invalidate);
    }
}

void ContentViewCore::UpdateForTapOrPress(
    /* [in] */ Int32 type,
    /* [in] */ Float xPix,
    /* [in] */ Float yPix)
{
    if (type != GestureEventType.SINGLE_TAP_CONFIRMED
            && type != GestureEventType.SINGLE_TAP_UP
            && type != GestureEventType.LONG_PRESS
            && type != GestureEventType.LONG_TAP) {
        return;
    }

    Boolean bFocusable, bTouchMode, bFocused;
    mContainerView->IsFocusable(&bFocusable);
    mContainerView->IsFocusableInTouchMode(&bTouchMode);
    mContainerView->IsFocused(&bFocused);
    if (bFocusable && bTouchMode
            && !bFocused)  {
        mContainerView->RequestFocus();
    }

    if (!mPopupZoomer->IsShowing()) {
        mPopupZoomer->SetLastTouch(xPix, yPix);
    }

    mLastTapX = (Int32) xPix;
    mLastTapY = (Int32) yPix;

    if (type == GestureEventType::LONG_PRESS
            || type == GestureEventType::LONG_TAP) {
        GetInsertionHandleController()->AllowAutomaticShowing();
        GetSelectionHandleController()->AllowAutomaticShowing();
    }
    else {
        if (mSelectionEditable) {
            GetInsertionHandleController()->AllowAutomaticShowing();
        }
    }
}

/**
 * @return The x coordinate for the last point that a tap or press gesture was initiated from.
 */
Int32 ContentViewCore::GetLastTapX()
{
    return mLastTapX;
}

/**
 * @return The y coordinate for the last point that a tap or press gesture was initiated from.
 */
Int32 ContentViewCore::GetLastTapY()
{
    return mLastTapY;
}

void ContentViewCore::SetZoomControlsDelegate(
    /* [in] */ ZoomControlsDelegate* zoomControlsDelegate)
{
    mZoomControlsDelegate = zoomControlsDelegate;
}

void ContentViewCore::UpdateMultiTouchZoomSupport(
    /* [in] */ Boolean supportsMultiTouchZoom)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeSetMultiTouchZoomSupportEnabled(mNativeContentViewCore, supportsMultiTouchZoom);
}

void ContentViewCore::UpdateDoubleTapSupport(
    /* [in] */ Boolean supportsDoubleTap)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeSetDoubleTapSupportEnabled(mNativeContentViewCore, supportsDoubleTap);
}

void ContentViewCore::SelectPopupMenuItems(
    /* [in] */ ArrayOf<Int32>* indices)
{
    if (mNativeContentViewCore != 0) {
        NativeSelectPopupMenuItems(mNativeContentViewCore, indices);
    }

    mSelectPopup = NULL;
}

/**
 * Send the screen orientation value to the renderer.
 */
//@VisibleForTesting
void ContentViewCore::SendOrientationChangeEvent(
    /* [in] */ Int32 orientation)
{
    if (mNativeContentViewCore == 0) {
        return;
    }

    NativeSendOrientationChangeEvent(mNativeContentViewCore, orientation);
}

/**
 * Register the delegate to be used when content can not be handled by
 * the rendering engine, and should be downloaded instead. This will replace
 * the current delegate, if any.
 * @param delegate An implementation of ContentViewDownloadDelegate.
 */
void ContentViewCore::SetDownloadDelegate(
    /* [in] */ ContentViewDownloadDelegate* delegate)
{
    mDownloadDelegate = delegate;
}

// Called by DownloadController.
AutoPtr<ContentViewDownloadDelegate> ContentViewCore::GetDownloadDelegate()
{
    return mDownloadDelegate;
}

AutoPtr<SelectionHandleController> ContentViewCore::GetSelectionHandleController()
{
    if (mSelectionHandleController == NULL) {
        mSelectionHandleController = new InnerSelectionHandleController(this,
                GetContainerView(), mPositionObserver);

    return mSelectionHandleController;
}

AutoPtr<InsertionHandleController> ContentViewCore::GetInsertionHandleController()
{
    if (mInsertionHandleController == NULL) {
        mInsertionHandleController = new InnerInsertionHandleController(this,
                GetContainerView(), mPositionObserver);

        mInsertionHandleController->HideAndDisallowAutomaticShowing();
    }

    return mInsertionHandleController;
}

//@VisibleForTesting
AutoPtr<InsertionHandleController> ContentViewCore::GetInsertionHandleControllerForTest()
{
    return mInsertionHandleController;
}

//@VisibleForTesting
AutoPtr<SelectionHandleController> ContentViewCore::GetSelectionHandleControllerForTest()
{
    return mSelectionHandleController;
}

void ContentViewCore::UpdateHandleScreenPositions()
{
    if (IsSelectionHandleShowing()) {
        mSelectionHandleController->SetStartHandlePosition(
                mStartHandlePoint->GetXPix(), mStartHandlePoint->GetYPix());
        mSelectionHandleController->SetEndHandlePosition(
                mEndHandlePoint->GetXPix(), mEndHandlePoint->GetYPix());
    }

    if (IsInsertionHandleShowing()) {
        mInsertionHandleController->SetHandlePosition(
                mInsertionHandlePoint->GetXPix(), mInsertionHandlePoint->GetYPix());
    }
}

void ContentViewCore::HideHandles()
{
    if (mSelectionHandleController != NULL) {
        mSelectionHandleController->HideAndDisallowAutomaticShowing();
    }

    if (mInsertionHandleController != NULL) {
        mInsertionHandleController->HideAndDisallowAutomaticShowing();
    }

    mPositionObserver->RemoveListener(mPositionListener);
}

void ContentViewCore::ShowSelectActionBar()
{
    if (mActionMode != NULL) {
        mActionMode->Invalidate();
        return;
    }

    // Start a new action mode with a SelectActionModeCallback.
    AutoPtr<SelectActionModeCallback::ActionHandler> actionHandler =
            new InnerActionHandler(this);

    mActionMode = NULL;
    // On ICS, startActionMode throws an NPE when getParent() is null.
    if (mContainerView->GetParent() != NULL) {
        mActionMode = mContainerView->StartActionMode(
                GetContentViewClient()->GetSelectActionModeCallback(GetContext(), actionHandler,
                        NativeIsIncognito(mNativeContentViewCore)));
    }

    mUnselectAllOnActionModeDismiss = TRUE;
    if (mActionMode == NULL) {
        // There is no ActionMode, so remove the selection.
        mImeAdapter->Unselect();
    }
    else {
        GetContentViewClient()->OnContextualActionBarShown();
    }
}

Boolean ContentViewCore::GetUseDesktopUserAgent()
{
    if (mNativeContentViewCore != 0) {
        return NativeGetUseDesktopUserAgent(mNativeContentViewCore);
    }

    return FALSE;
}

/**
 * Set whether or not we're using a desktop user agent for the currently loaded page.
 * @param override If true, use a desktop user agent.  Use a mobile one otherwise.
 * @param reloadOnChange Reload the page if the UA has changed.
 */
void ContentViewCore::SetUseDesktopUserAgent(
    /* [in] */ Boolean override,
    /* [in] */ Boolean reloadOnChange)
{
    if (mNativeContentViewCore != 0) {
        NativeSetUseDesktopUserAgent(mNativeContentViewCore, override, reloadOnChange);
    }
}

void ContentViewCore::ClearSslPreferences()
{
    if (mNativeContentViewCore != 0) {
        NativeClearSslPreferences(mNativeContentViewCore);
    }
}

Boolean ContentViewCore::IsSelectionHandleShowing()
{
    return mSelectionHandleController != NULL && mSelectionHandleController->IsShowing();
}

Boolean ContentViewCore::IsInsertionHandleShowing()
{
    return mInsertionHandleController != NULL && mInsertionHandleController->IsShowing();
}

// Makes the insertion/selection handles invisible. They will fade back in shortly after the
// last call to scheduleTextHandleFadeIn (or temporarilyHideTextHandles).
void ContentViewCore::TemporarilyHideTextHandles()
{
    if (IsSelectionHandleShowing() && !mSelectionHandleController->IsDragging()) {
        mSelectionHandleController->SetHandleVisibility(HandleView::INVISIBLE);
    }

    if (IsInsertionHandleShowing() && !mInsertionHandleController->IsDragging()) {
        mInsertionHandleController->SetHandleVisibility(HandleView::INVISIBLE);
    }

    ScheduleTextHandleFadeIn();
}

Boolean ContentViewCore::AllowTextHandleFadeIn()
{
    if (mTouchScrollInProgress) {
        return FALSE;
    }

    if (mPopupZoomer->IsShowing()) {
        return FALSE;
    }

    return TRUE;
}

// Cancels any pending fade in and schedules a new one.
void ContentViewCore::ScheduleTextHandleFadeIn()
{
    if (!IsInsertionHandleShowing() && !IsSelectionHandleShowing()) {
        return;
    }

    if (mDeferredHandleFadeInRunnable == NULL) {
        mDeferredHandleFadeInRunnable = new DeferredHandleFadeInRunnable(this);
    }

    mContainerView->RemoveCallbacks(mDeferredHandleFadeInRunnable);
    mContainerView->PostDelayed(mDeferredHandleFadeInRunnable, TEXT_HANDLE_FADE_IN_DELAY);
}

/**
 * Shows the IME if the focused widget could accept text input.
 */
void ContentViewCore::ShowImeIfNeeded()
{
    if (mNativeContentViewCore != 0) {
        NativeShowImeIfNeeded(mNativeContentViewCore);
    }
}

/**
 * Hides the IME if the containerView is the active view for IME.
 */
void ContentViewCore::HideImeIfNeeded()
{
    // Hide input method window from the current view synchronously
    // because ImeAdapter does so asynchronouly with a delay, and
    // by the time when ImeAdapter dismisses the input, the
    // containerView may have lost focus.
    // We cannot trust ContentViewClient#onImeStateChangeRequested to
    // hide the input window because it has an empty default implementation.
    // So we need to explicitly hide the input method window here.
    if (mInputMethodManagerWrapper->IsActive(mContainerView)) {
        mInputMethodManagerWrapper->HideSoftInputFromWindow(
                mContainerView->GetWindowToken(), 0, null);
    }

    GetContentViewClient()->OnImeStateChangeRequested(FALSE);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::UpdateFrameInfo(
    /* [in] */ Float scrollOffsetX,
    /* [in] */ Float scrollOffsetY,
    /* [in] */ Float pageScaleFactor,
    /* [in] */ Float minPageScaleFactor,
    /* [in] */ Float maxPageScaleFactor,
    /* [in] */ Float contentWidth,
    /* [in] */ Float contentHeight,
    /* [in] */ Float viewportWidth,
    /* [in] */ Float viewportHeight,
    /* [in] */ Float controlsOffsetYCss,
    /* [in] */ Float contentOffsetYCss,
    /* [in] */ Float overdrawBottomHeightCss)
{
    TraceEvent::Instant("ContentViewCore:updateFrameInfo");

    // Adjust contentWidth/Height to be always at least as big as
    // the actual viewport (as set by onSizeChanged).
    const Float deviceScale = mRenderCoordinates->GetDeviceScaleFactor();
    contentWidth = Math::Max(contentWidth,
            mViewportWidthPix / (deviceScale * pageScaleFactor));
    contentHeight = Math::Max(contentHeight,
            mViewportHeightPix / (deviceScale * pageScaleFactor));
    const Float contentOffsetYPix = mRenderCoordinates->FromDipToPix(contentOffsetYCss);

    const Boolean contentSizeChanged =
            contentWidth != mRenderCoordinates->GetContentWidthCss()
            || contentHeight != mRenderCoordinates->GetContentHeightCss();
    const Boolean scaleLimitsChanged =
            minPageScaleFactor != mRenderCoordinates->GetMinPageScaleFactor()
            || maxPageScaleFactor != mRenderCoordinates->GetMaxPageScaleFactor();
    const Boolean pageScaleChanged =
            pageScaleFactor != mRenderCoordinates->GetPageScaleFactor();
    const Boolean scrollChanged =
            pageScaleChanged
            || scrollOffsetX != mRenderCoordinates->GetScrollX()
            || scrollOffsetY != mRenderCoordinates->GetScrollY();
    const Boolean contentOffsetChanged =
            contentOffsetYPix != mRenderCoordinates->GetContentOffsetYPix();

    const Boolean needHidePopupZoomer = contentSizeChanged || scrollChanged;
    const Boolean needUpdateZoomControls = scaleLimitsChanged || scrollChanged;
    const Boolean needTemporarilyHideHandles = scrollChanged;

    if (needHidePopupZoomer) {
        mPopupZoomer->Hide(TRUE);
    }

    if (scrollChanged) {
        mContainerViewInternals->OnScrollChanged(
                (Int32) mRenderCoordinates->FromLocalCssToPix(scrollOffsetX),
                (Int32) mRenderCoordinates->FromLocalCssToPix(scrollOffsetY),
                (Int32) mRenderCoordinates->GetScrollXPix(),
                (Int32) mRenderCoordinates->GetScrollYPix());
    }

    mRenderCoordinates->UpdateFrameInfo(
            scrollOffsetX, scrollOffsetY,
            contentWidth, contentHeight,
            viewportWidth, viewportHeight,
            pageScaleFactor, minPageScaleFactor, maxPageScaleFactor,
            contentOffsetYPix);

    if (scrollChanged || contentOffsetChanged) {
        for (mGestureStateListenersIterator->Rewind();
                mGestureStateListenersIterator->HasNext();) {
            mGestureStateListenersIterator->Next()->OnScrollOffsetOrExtentChanged(
                    ComputeVerticalScrollOffset(),
                    ComputeVerticalScrollExtent());
        }
    }

    if (needTemporarilyHideHandles) {
        TemporarilyHideTextHandles();
    }

    if (needUpdateZoomControls) {
        mZoomControlsDelegate->UpdateZoomControls();
    }

    if (contentOffsetChanged) {
        UpdateHandleScreenPositions();
    }

    // Update offsets for fullscreen.
    const Float controlsOffsetPix = controlsOffsetYCss * deviceScale;
    const Float overdrawBottomHeightPix = overdrawBottomHeightCss * deviceScale;
    GetContentViewClient()->OnOffsetsForFullscreenChanged(
            controlsOffsetPix, contentOffsetYPix, overdrawBottomHeightPix);

    if (mBrowserAccessibilityManager != NULL) {
        mBrowserAccessibilityManager->NotifyFrameInfoInitialized();
    }
}

//@CalledByNative
void ContentViewCore::UpdateImeAdapter(
    /* [in] */ Int64 nativeImeAdapterAndroid,
    /* [in] */ Int32 textInputType,
    /* [in] */ const String& text,
    /* [in] */ Int32 selectionStart,
    /* [in] */ Int32 selectionEnd,
    /* [in] */ Int32 compositionStart,
    /* [in] */ Int32 compositionEnd,
    /* [in] */ Boolean showImeIfNeeded,
    /* [in] */ Boolean isNonImeChange)
{
    TraceEvent::Begin();
    mSelectionEditable = (textInputType != ImeAdapter::GetTextInputTypeNone());

    mImeAdapter::UpdateKeyboardVisibility(
            nativeImeAdapterAndroid, textInputType, showImeIfNeeded);

    if (mInputConnection != NULL) {
        mInputConnection->UpdateState(text, selectionStart, selectionEnd, compositionStart,
                compositionEnd, isNonImeChange);
    }

    if (mActionMode != NULL) {
        mActionMode::Invalidate();
    }

    TraceEvent::End();
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::SetTitle(
    /* [in] */ const String& title)
{
    GetContentViewClient()->OnUpdateTitle(title);
}

/**
 * Called (from native) when the <select> popup needs to be shown.
 * @param items           Items to show.
 * @param enabled         POPUP_ITEM_TYPEs for items.
 * @param multiple        Whether the popup menu should support multi-select.
 * @param selectedIndices Indices of selected items.
 */
//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::ShowSelectPopup(
    /* [in] */ IRect* bounds,
    /* [in] */ ArrayOf<String>* items,
    /* [in] */ ArrayOf<Int32>* enabled,
    /* [in] */ Boolean multiple,
    /* [in] */ ArrayOf<Int32>* selectedIndices)
{
    if (mContainerView->GetParent() == NULL || mContainerView->GetVisibility() != IView::VISIBLE) {
        SelectPopupMenuItems(NULL);
        return;
    }

    assert(items->GetLength() == enabled->GetLength());
    List<SelectPopupItem> popupItems = new ArrayList<SelectPopupItem>();
    for (Int32 i = 0; i < items.length; i++) {
        popupItems.add(new SelectPopupItem(items[i], enabled[i]));
    }

    HidePopups();
    if (DeviceUtils::IsTablet(mContext) && !multiple) {
        mSelectPopup = new SelectPopupDropdown(this, popupItems, bounds, selectedIndices);
    }
    else {
        mSelectPopup = new SelectPopupDialog(this, popupItems, multiple, selectedIndices);
    }

    mSelectPopup->Show();
}

/**
 * Called when the <select> popup needs to be hidden.
 */
//@CalledByNative
void ContentViewCore::HideSelectPopup()
{
    if (mSelectPopup != NULL) {
        mSelectPopup->Hide();
    }
}

/**
 * @return The visible select popup being shown.
 */
AutoPtr<SelectPopup> ContentViewCore::GetSelectPopupForTest()
{
    return mSelectPopup;
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::ShowDisambiguationPopup(
    /* [in] */ IRect* targetRect,
    /* [in] */ IBitmap* zoomedBitmap)
{
    mPopupZoomer->SetBitmap(zoomedBitmap);
    mPopupZoomer->Show(targetRect);
    TemporarilyHideTextHandles();
}

//@SuppressWarnings("unused")
//@CalledByNative
AutoPtr<TouchEventSynthesizer> ContentViewCore::CreateTouchEventSynthesizer()
{
    return new TouchEventSynthesizer(this);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnSelectionChanged(
    /* [in] */ const String& text)
{
    mLastSelectedText = text;
    GetContentViewClient()->OnSelectionChanged(text);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::ShowSelectionHandlesAutomatically()
{
    GetSelectionHandleController()->AllowAutomaticShowing();
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnSelectionBoundsChanged(
    /* [in] */ IRect* anchorRectDip,
    /* [in] */ Int32 anchorDir,
    /* [in] */ IRect* focusRectDip,
    /* [in] */ Int32 focusDir,
    /* [in] */ Boolean isAnchorFirst)
{
    // All coordinates are in DIP.
    Int32 x1 = anchorRectDip.left;
    Int32 y1 = anchorRectDip.bottom;
    Int32 x2 = focusRectDip.left;
    Int32 y2 = focusRectDip.bottom;

    if (x1 != x2 || y1 != y2 ||
            (mSelectionHandleController != NULL && mSelectionHandleController->IsDragging())) {
        if (mInsertionHandleController != NULL) {
            mInsertionHandleController->Hide();
        }

        if (isAnchorFirst) {
            mStartHandlePoint->SetLocalDip(x1, y1);
            mEndHandlePoint->SetLocalDip(x2, y2);
        }
        else {
            mStartHandlePoint->SetLocalDip(x2, y2);
            mEndHandlePoint->SetLocalDip(x1, y1);
        }

        Boolean wereSelectionHandlesShowing = GetSelectionHandleController()->IsShowing();

        GetSelectionHandleController()->OnSelectionChanged(anchorDir, focusDir);
        UpdateHandleScreenPositions();
        mHasSelection = TRUE;

        if (!wereSelectionHandlesShowing && GetSelectionHandleController()->IsShowing()) {
            // TODO(cjhopman): Remove this when there is a better signal that long press caused
            // a selection. See http://crbug.com/150151.
            mContainerView->PerformHapticFeedback(HapticFeedbackConstants::LONG_PRESS);
        }

    }
    else {
        mUnselectAllOnActionModeDismiss = FALSE;
        HideSelectActionBar();
        if (x1 != 0 && y1 != 0 && mSelectionEditable) {
            // Selection is a caret, and a text field is focused.
            if (mSelectionHandleController != NULL) {
                mSelectionHandleController->Hide();
            }

            mInsertionHandlePoint->SetLocalDip(x1, y1);

            GetInsertionHandleController()->OnCursorPositionChanged();
            UpdateHandleScreenPositions();
            if (mInputMethodManagerWrapper->IsWatchingCursor(mContainerView)) {
                const Int32 xPix = (Int32) mInsertionHandlePoint->GetXPix();
                const Int32 yPix = (Int32) mInsertionHandlePoint->GetYPix();
                mInputMethodManagerWrapper->UpdateCursor(
                        mContainerView, xPix, yPix, xPix, yPix);
            }
        }
        else {
            // Deselection
            if (mSelectionHandleController != NULL) {
                mSelectionHandleController->HideAndDisallowAutomaticShowing();
            }

            if (mInsertionHandleController != NULL) {
                mInsertionHandleController->HideAndDisallowAutomaticShowing();
            }
        }
        mHasSelection = FALSE;
    }

    if (IsSelectionHandleShowing() || IsInsertionHandleShowing()) {
        mPositionObserver->AddListener(mPositionListener);
    }
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnEvaluateJavaScriptResult(
    /* [in] */ const String& jsonResult,
    /* [in] */ JavaScriptCallback* callback)
{
    callback->HandleJavaScriptResult(jsonResult);
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::ShowPastePopup(
    /* [in] */ Int32 xDip,
    /* [in] */ Int32 yDip)
{
    mInsertionHandlePoint->SetLocalDip(xDip, yDip);
    GetInsertionHandleController()->ShowHandle();
    UpdateHandleScreenPositions();
    GetInsertionHandleController()->ShowHandleWithPastePopup();
}

//@SuppressWarnings("unused")
//@CalledByNative
void ContentViewCore::OnRenderProcessChange()
{
    AttachImeAdapter();
}

/**
 * Attaches the native ImeAdapter object to the java ImeAdapter to allow communication via JNI.
 */
void ContentViewCore::AttachImeAdapter()
{
    if (mImeAdapter != NULL && mNativeContentViewCore != 0) {
        mImeAdapter->Attach(NativeGetNativeImeAdapter(mNativeContentViewCore));
    }
}

/**
 * @see View#hasFocus()
 */
//@CalledByNative
Boolean ContentViewCore::HasFocus()
{
    return mContainerView->HasFocus();
}

/**
 * Checks whether the ContentViewCore can be zoomed in.
 *
 * @return True if the ContentViewCore can be zoomed in.
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean ContentViewCore::CanZoomIn()
{
    const Float zoomInExtent = mRenderCoordinates->GetMaxPageScaleFactor()
            - mRenderCoordinates->GetPageScaleFactor();
    return zoomInExtent > ZOOM_CONTROLS_EPSILON;
}

/**
 * Checks whether the ContentViewCore can be zoomed out.
 *
 * @return True if the ContentViewCore can be zoomed out.
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean ContentViewCore::CanZoomOut()
{
    const Float zoomOutExtent = mRenderCoordinates->GetPageScaleFactor()
            - mRenderCoordinates->GetMinPageScaleFactor();
    return zoomOutExtent > ZOOM_CONTROLS_EPSILON;
}

/**
 * Zooms in the ContentViewCore by 25% (or less if that would result in
 * zooming in more than possible).
 *
 * @return True if there was a zoom change, false otherwise.
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean ContentViewCore::ZoomIn()
{
    if (!CanZoomIn()) {
        return FALSE;
    }

    return PinchByDelta(1.25f);
}

/**
 * Zooms out the ContentViewCore by 20% (or less if that would result in
 * zooming out more than possible).
 *
 * @return True if there was a zoom change, false otherwise.
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean ContentViewCore::ZoomOut()
{
    if (!CanZoomOut()) {
        return FALSE;
    }

    return PinchByDelta(0.8f);
}

/**
 * Resets the zoom factor of the ContentViewCore.
 *
 * @return True if there was a zoom change, false otherwise.
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean ContentViewCore::ZoomReset()
{
    // The page scale factor is initialized to mNativeMinimumScale when
    // the page finishes loading. Thus sets it back to mNativeMinimumScale.
    if (!CanZoomOut()) {
        return FALSE;
    }

    return PinchByDelta(
            mRenderCoordinates->GetMinPageScaleFactor()
                    / mRenderCoordinates->GetPageScaleFactor());
}

/**
 * Simulate a pinch zoom gesture.
 *
 * @param delta the factor by which the current page scale should be multiplied by.
 * @return whether the gesture was sent.
 */
Boolean ContentViewCore::PinchByDelta(
    /* [in] */ Float delta)
{
    if (mNativeContentViewCore == 0) {
        return FALSE;
    }

    Int64 timeMs = SystemClock::UptimeMillis();
    Int32 xPix = GetViewportWidthPix() / 2;
    Int32 yPix = GetViewportHeightPix() / 2;

    NativePinchBegin(mNativeContentViewCore, timeMs, xPix, yPix);
    NativePinchBy(mNativeContentViewCore, timeMs, xPix, yPix, delta);
    NativePinchEnd(mNativeContentViewCore, timeMs);

    return TRUE;
}

/**
 * Invokes the graphical zoom picker widget for this ContentView.
 */
void ContentViewCore::InvokeZoomPicker()
{
    mZoomControlsDelegate->InvokeZoomPicker();
}

/**
 * Enables or disables inspection of JavaScript objects added via
 * {@link #addJavascriptInterface(Object, String)} by means of Object.keys() method and
 * &quot;for .. in&quot; loop. Being able to inspect JavaScript objects is useful
 * when debugging hybrid Android apps, but can't be enabled for legacy applications due
 * to compatibility risks.
 *
 * @param allow Whether to allow JavaScript objects inspection.
 */
void ContentViewCore::SetAllowJavascriptInterfacesInspection(
    /* [in] */ Boolean allow)
{
    NsativeSetAllowJavascriptInterfacesInspection(mNativeContentViewCore, allow);
}

/**
 * Returns JavaScript interface objects previously injected via
 * {@link #addJavascriptInterface(Object, String)}.
 *
 * @return the mapping of names to interface objects and corresponding annotation classes
 */
Map<String, Pair<Object, Class>> ContentViewCore::GetJavascriptInterfaces()
{
    return mJavaScriptInterfaces;
}

/**
 * This will mimic {@link #addPossiblyUnsafeJavascriptInterface(Object, String, Class)}
 * and automatically pass in {@link JavascriptInterface} as the required annotation.
 *
 * @param object The Java object to inject into the ContentViewCore's JavaScript context.  Null
 *               values are ignored.
 * @param name   The name used to expose the instance in JavaScript.
 */
void ContentViewCore::AddJavascriptInterface(
    /* [in] */ IInterface* object,
    /* [in] */ const String& name)
{
    AddPossiblyUnsafeJavascriptInterface(object, name, JavascriptInterface.class);
}

/**
 * This method injects the supplied Java object into the ContentViewCore.
 * The object is injected into the JavaScript context of the main frame,
 * using the supplied name. This allows the Java object to be accessed from
 * JavaScript. Note that that injected objects will not appear in
 * JavaScript until the page is next (re)loaded. For example:
 * <pre> view.addJavascriptInterface(new Object(), "injectedObject");
 * view.loadData("<!DOCTYPE html><title></title>", "text/html", null);
 * view.loadUrl("javascript:alert(injectedObject.toString())");</pre>
 * <p><strong>IMPORTANT:</strong>
 * <ul>
 * <li> addJavascriptInterface() can be used to allow JavaScript to control
 * the host application. This is a powerful feature, but also presents a
 * security risk. Use of this method in a ContentViewCore containing
 * untrusted content could allow an attacker to manipulate the host
 * application in unintended ways, executing Java code with the permissions
 * of the host application. Use extreme care when using this method in a
 * ContentViewCore which could contain untrusted content. Particular care
 * should be taken to avoid unintentional access to inherited methods, such
 * as {@link Object#getClass()}. To prevent access to inherited methods,
 * pass an annotation for {@code requiredAnnotation}.  This will ensure
 * that only methods with {@code requiredAnnotation} are exposed to the
 * Javascript layer.  {@code requiredAnnotation} will be passed to all
 * subsequently injected Java objects if any methods return an object.  This
 * means the same restrictions (or lack thereof) will apply.  Alternatively,
 * {@link #addJavascriptInterface(Object, String)} can be called, which
 * automatically uses the {@link JavascriptInterface} annotation.
 * <li> JavaScript interacts with Java objects on a private, background
 * thread of the ContentViewCore. Care is therefore required to maintain
 * thread safety.</li>
 * </ul></p>
 *
 * @param object             The Java object to inject into the
 *                           ContentViewCore's JavaScript context. Null
 *                           values are ignored.
 * @param name               The name used to expose the instance in
 *                           JavaScript.
 * @param requiredAnnotation Restrict exposed methods to ones with this
 *                           annotation.  If {@code null} all methods are
 *                           exposed.
 *
 */
void ContentViewCore::AddPossiblyUnsafeJavascriptInterface(
    /* [in] */ IInterface* object,
    /* [in] */ const String& name,
    /* [in] */ Annotation* requiredAnnotation)
{
    if (mNativeContentViewCore != 0 && object != null) {
        mJavaScriptInterfaces.put(name, new Pair<Object, Class>(object, requiredAnnotation));
        NativeAddJavascriptInterface(mNativeContentViewCore, object, name, requiredAnnotation);
    }
}

/**
 * Removes a previously added JavaScript interface with the given name.
 *
 * @param name The name of the interface to remove.
 */
void ContentViewCore::RemoveJavascriptInterface(
    /* [in] */ const String& name)
{
    mJavaScriptInterfaces->Remove(name);
    if (mNativeContentViewCore != 0) {
        NativeRemoveJavascriptInterface(mNativeContentViewCore, name);
    }
}

/**
 * Return the current scale of the ContentView.
 * @return The current page scale factor.
 */
Float ContentViewCore::GetScale()
{
    return mRenderCoordinates->GetPageScaleFactor();
}

/**
 * If the view is ready to draw contents to the screen. In hardware mode,
 * the initialization of the surface texture may not occur until after the
 * view has been added to the layout. This method will return {@code true}
 * once the texture is actually ready.
 */
Boolean ContentViewCore::IsReady()
{
    if (mNativeContentViewCore == 0) {
        return FALSE;
    }

    return NativeIsRenderWidgetHostViewReady(mNativeContentViewCore);
}

//@CalledByNative
void ContentViewCore::StartContentIntent(
    /* [in] */ const String& contentUrl)
{
    GetContentViewClient()->OnStartContentIntent(GetContext(), contentUrl);
}

//@Override
void ContentViewCore::OnAccessibilityStateChanged(
    /* [in] */ Boolean enabled)
{
    SetAccessibilityState(enabled);
}

/**
 * Determines whether or not this ContentViewCore can handle this accessibility action.
 * @param action The action to perform.
 * @return Whether or not this action is supported.
 */
Boolean ContentViewCore::SupportsAccessibilityAction(
    /* [in] */ Int32 action)
{
    return mAccessibilityInjector->SupportsAccessibilityAction(action);
}

/**
 * Attempts to perform an accessibility action on the web content.  If the accessibility action
 * cannot be processed, it returns {@code null}, allowing the caller to know to call the
 * super {@link View#performAccessibilityAction(int, Bundle)} method and use that return value.
 * Otherwise the return value from this method should be used.
 * @param action The action to perform.
 * @param arguments Optional action arguments.
 * @return Whether the action was performed or {@code null} if the call should be delegated to
 *         the super {@link View} class.
 */
Boolean ContentViewCore::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if (mAccessibilityInjector->SupportsAccessibilityAction(action)) {
        return mAccessibilityInjector->PerformAccessibilityAction(action, arguments);
    }

    return FALSE;
}

/**
 * Set the BrowserAccessibilityManager, used for native accessibility
 * (not script injection). This is only set when system accessibility
 * has been enabled.
 * @param manager The new BrowserAccessibilityManager.
 */
void ContentViewCore::SetBrowserAccessibilityManager(
    /* [in] */ BrowserAccessibilityManager* manager)
{
    mBrowserAccessibilityManager = manager;
}

/**
 * Get the BrowserAccessibilityManager, used for native accessibility
 * (not script injection). This will return null when system accessibility
 * is not enabled.
 * @return This view's BrowserAccessibilityManager.
 */
AutoPtr<BrowserAccessibilityManager> ContentViewCore::GetBrowserAccessibilityManager()
{
    return mBrowserAccessibilityManager;
}

/**
 * If native accessibility (not script injection) is enabled, and if this is
 * running on JellyBean or later, returns an AccessibilityNodeProvider that
 * implements native accessibility for this view. Returns null otherwise.
 * Lazily initializes native accessibility here if it's allowed.
 * @return The AccessibilityNodeProvider, if available, or null otherwise.
 */
AutoPtr<AccessibilityNodeProvider> ContentViewCore::GetAccessibilityNodeProvider()
{
    if (mBrowserAccessibilityManager != NULL) {
        return mBrowserAccessibilityManager.getAccessibilityNodeProvider();
    }

    if (mNativeAccessibilityAllowed &&
            !mNativeAccessibilityEnabled &&
            mNativeContentViewCore != 0 &&
            Build.VERSION.SDK_INT >= Build::VERSION_CODES.JELLY_BEAN) {
        mNativeAccessibilityEnabled = TRUE;
        NativeSetAccessibilityEnabled(mNativeContentViewCore, TRUE);
    }

    return NULL;
}

/**
 * @see View#onInitializeAccessibilityNodeInfo(AccessibilityNodeInfo)
 */
void ContentViewCore::OnInitializeAccessibilityNodeInfo(
    /* [in] */  IAccessibilityNodeInfo* info)
{
    // Note: this is only used by the script-injecting accessibility code.
    mAccessibilityInjector->OnInitializeAccessibilityNodeInfo(info);
}

/**
 * @see View#onInitializeAccessibilityEvent(AccessibilityEvent)
 */
void ContentViewCore::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    // Note: this is only used by the script-injecting accessibility code.
    event->SetClassName(this.getClass().getName());

    // Identify where the top-left of the screen currently points to.
    event->SetScrollX(mRenderCoordinates->GetScrollXPixInt());
    event->SetScrollY(mRenderCoordinates->GetScrollYPixInt());

    // The maximum scroll values are determined by taking the content dimensions and
    // subtracting off the actual dimensions of the ChromeView.
    Int32 maxScrollXPix = Math::Max(0, mRenderCoordinates->GetMaxHorizontalScrollPixInt());
    Int32 maxScrollYPix = Math::Max(0, mRenderCoordinates->GetMaxVerticalScrollPixInt());
    event->SetScrollable(maxScrollXPix > 0 || maxScrollYPix > 0);

    // Setting the maximum scroll values requires API level 15 or higher.
    const Int32 SDK_VERSION_REQUIRED_TO_SET_SCROLL = 15;
    if (Build.VERSION.SDK_INT >= SDK_VERSION_REQUIRED_TO_SET_SCROLL) {
        event->SetMaxScrollX(maxScrollXPix);
        event->SetMaxScrollY(maxScrollYPix);
    }
}

/**
 * Returns whether accessibility script injection is enabled on the device
 */
Boolean ContentViewCore::IsDeviceAccessibilityScriptInjectionEnabled()
{
    // try {
        // On JellyBean and higher, native accessibility is the default so script
        // injection is only allowed if enabled via a flag.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN &&
                !CommandLine::GetInstance()->HasSwitch(
                        ContentSwitches::ENABLE_ACCESSIBILITY_SCRIPT_INJECTION)) {
            return FALSE;
        }

        if (!mContentSettings->GetJavaScriptEnabled()) {
            return FALSE;
        }

        Int32 result;
        GetContext()->CheckCallingOrSelfPermission(
                android::Manifest::permission::INTERNET, &result);
        if (result != IPackageManager::PERMISSION_GRANTED) {
            return FALSE;
        }

        Field field = Settings.Secure.class.getField("ACCESSIBILITY_SCRIPT_INJECTION");
        field.setAccessible(true);
        String accessibilityScriptInjection = (String) field.get(null);
        AutoPtr<IContentResolver> contentResolver;
        GetContext()->GetContentResolver((IContentResolver**)&contentResolver);

        if (mAccessibilityScriptInjectionObserver == NULL) {
            AutoPtr<IHandler> handle;
            CHandler::New((IHandler**)&handle);
            AutoPtr<IContentObserver> contentObserver = new InnerContentObserver(this, handle);
            contentResolver->RegisterContentObserver(
                Settings.Secure.getUriFor(accessibilityScriptInjection),
                FALSE,
                contentObserver);
            mAccessibilityScriptInjectionObserver = contentObserver;
        }

        return Settings::Secure::GetInt32(contentResolver, accessibilityScriptInjection, 0) == 1;
    // } catch (NoSuchFieldException e) {
    //     // Do nothing, default to false.
    // } catch (IllegalAccessException e) {
    //     // Do nothing, default to false.
    // }

    return FALSE;
}

/**
 * Returns whether or not accessibility injection is being used.
 */
Boolean ContentViewCore::isInjectingAccessibilityScript()
{
    return mAccessibilityInjector->AccessibilityIsAvailable();
}

/**
 * Returns true if accessibility is on and touch exploration is enabled.
 */
Boolean ContentViewCore::isTouchExplorationEnabled()
{
    return mTouchExplorationEnabled;
}

/**
 * Turns browser accessibility on or off.
 * If |state| is |false|, this turns off both native and injected accessibility.
 * Otherwise, if accessibility script injection is enabled, this will enable the injected
 * accessibility scripts. Native accessibility is enabled on demand.
 */
void ContentViewCore::setAccessibilityState(
    /* [in] */ Boolean state)
{
    if (!state) {
        SetInjectedAccessibility(FALSE);
        mNativeAccessibilityAllowed = FALSE;
        mTouchExplorationEnabled = FALSE;
    }
    else {
        Boolean useScriptInjection = IsDeviceAccessibilityScriptInjectionEnabled();
        SetInjectedAccessibility(useScriptInjection);
        mNativeAccessibilityAllowed = !useScriptInjection;
        mTouchExplorationEnabled = mAccessibilityManager->IsTouchExplorationEnabled();
    }
}

/**
 * Enable or disable injected accessibility features
 */
void ContentViewCore::SetInjectedAccessibility(
    /* [in] */ Boolean enabled)
{
    mAccessibilityInjector->AddOrRemoveAccessibilityApisIfNecessary();
    mAccessibilityInjector->SetScriptEnabled(enabled);
}

/**
 * Stop any TTS notifications that are currently going on.
 */
void ContentViewCore::StopCurrentAccessibilityNotifications()
{
    mAccessibilityInjector->OnPageLostFocus();
}

/**
 * Return whether or not we should set accessibility focus on page load.
 */
Boolean ContentViewCore::ShouldSetAccessibilityFocusOnPageLoad()
{
    return mShouldSetAccessibilityFocusOnPageLoad;
}

/**
 * Sets whether or not we should set accessibility focus on page load.
 * This only applies if an accessibility service like TalkBack is running.
 * This is desirable behavior for a browser window, but not for an embedded
 * WebView.
 */
void ContentViewCore::SetShouldSetAccessibilityFocusOnPageLoad(
    /* [in] */ Boolean on)
{
    mShouldSetAccessibilityFocusOnPageLoad = on;
}

/**
 * Inform WebKit that Fullscreen mode has been exited by the user.
 */
void ContentViewCore::ExitFullscreen()
{
    if (mNativeContentViewCore != 0) {
        NativeExitFullscreen(mNativeContentViewCore);
    }
}

/**
 * Changes whether hiding the top controls is enabled.
 *
 * @param enableHiding Whether hiding the top controls should be enabled or not.
 * @param enableShowing Whether showing the top controls should be enabled or not.
 * @param animate Whether the transition should be animated or not.
 */
void ContentViewCore::UpdateTopControlsState(
    /* [in] */ Boolean enableHiding,
    /* [in] */ Boolean enableShowing,
    /* [in] */ Boolean animate)
{
    if (mNativeContentViewCore != 0) {
        NativeUpdateTopControlsState(
                mNativeContentViewCore, enableHiding, enableShowing, animate);
    }
}

/**
 * Callback factory method for nativeGetNavigationHistory().
 */
//@CalledByNative
void ContentViewCore::AddToNavigationHistory(
    /* [in] */ IInterface* history,
    /* [in] */ Int32 index,
    /* [in] */ const String& url,
    /* [in] */ const String& virtualUrl,
    /* [in] */ const String& originalUrl,
    /* [in] */ const String& title,
    /* [in] */ IBitmap* favicon)
{
    AutoPtr<NavigationEntry> entry = new NavigationEntry(
            index, url, virtualUrl, originalUrl, title, favicon);
    ((NavigationHistory*) history)->AddEntry(entry);
}

/**
 * Get a copy of the navigation history of the view.
 */
AutoPtr<NavigationHistory> ContentViewCore::GetNavigationHistory()
{
    AutoPtr<NavigationHistory> history = new NavigationHistory();
    if (mNativeContentViewCore != 0) {
        Int32 currentIndex = NativeGetNavigationHistory(mNativeContentViewCore, history);
        history->SetCurrentEntryIndex(currentIndex);
    }

    return history;
}

//@Override
AutoPtr<NavigationHistory> ContentViewCore::GetDirectedNavigationHistory(
    /* [in] */ Boolean isForward,
    /* [in] */ Int32 itemLimit)
{
    AutoPtr<NavigationHistory> history = new NavigationHistory();
    if (mNativeContentViewCore != 0) {
        NativeGetDirectedNavigationHistory(
            mNativeContentViewCore, history, isForward, itemLimit);
    }
    return history;
}

/**
 * @return The original request URL for the current navigation entry, or null if there is no
 *         current entry.
 */
String ContentViewCore::GetOriginalUrlForActiveNavigationEntry()
{
    if (mNativeContentViewCore != 0) {
        return NativeGetOriginalUrlForActiveNavigationEntry(mNativeContentViewCore);
    }

    return String("");
}

/**
 * @return The cached copy of render positions and scales.
 */
AutoPtr<RenderCoordinates> ContentViewCore::GetRenderCoordinates()
{
    return mRenderCoordinates;
}

//@CalledByNative
AutoPtr<IRect> ContentViewCore::CreateRect(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IRect> rect;
    CRect::New(x, y, right, bottomj, (IRect**)&rect);
}

void ContentViewCore::ExtractSmartClipData(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mNativeContentViewCore != 0) {
        x += mSmartClipOffsetX;
        y += mSmartClipOffsetY;
        NativeExtractSmartClipData(mNativeContentViewCore, x, y, width, height);
    }
}

/**
 * Set offsets for smart clip.
 *
 * <p>This should be called if there is a viewport change introduced by,
 * e.g., show and hide of a location bar.
 *
 * @param offsetX Offset for X position.
 * @param offsetY Offset for Y position.
 */
void ContentViewCore::SetSmartClipOffsets(
    /* [in] */ Int32 offsetX,
    /* [in] */ Int32 offsetY)
{
    mSmartClipOffsetX = offsetX;
    mSmartClipOffsetY = offsetY;
}

//@CalledByNative
void ContentViewCore::OnSmartClipDataExtracted(
    /* [in] */ const String& text,
    /* [in] */ const String& html,
    /* [in] */ IRect* clipRect)
{
    if (mSmartClipDataListener != NULL) {
        mSmartClipDataListener->OnSmartClipDataExtracted(text, html, clipRect);
    }
}

void ContentViewCore::SetSmartClipDataListener(
    /* [in] */ SmartClipDataListener* listener)
{
    mSmartClipDataListener = listener;
}

void ContentViewCore::SetBackgroundOpaque(
    /* [in] */ Boolean opaque)
{
    if (mNativeContentViewCore != 0) {
        NativeSetBackgroundOpaque(mNativeContentViewCore, opaque);
    }
}

/**
 * Offer a long press gesture to the embedding View, primarily for WebView compatibility.
 *
 * @return true if the embedder handled the event.
 */
Boolean ContentViewCore::OfferLongPressToEmbedder()
{
    return mContainerView->PerformLongClick();
}

/**
 * Reset scroll and fling accounting, notifying listeners as appropriate.
 * This is useful as a failsafe when the input stream may have been interruped.
 */
void ContentViewCore::ResetScrollInProgress()
{
    if (!IsScrollInProgress()) {
        return;
    }

    const Boolean touchScrollInProgress = mTouchScrollInProgress;
    const Int32 potentiallyActiveFlingCount = mPotentiallyActiveFlingCount;

    mTouchScrollInProgress = FALSE;
    mPotentiallyActiveFlingCount = 0;

    if (touchScrollInProgress) {
        UpdateGestureStateListener(GestureEventType::SCROLL_END);
    }

    if (potentiallyActiveFlingCount > 0) {
        UpdateGestureStateListener(GestureEventType.FLING_END);
    }
}

Int64 ContentViewCore::NativeInit(
    /* [in] */ Int64 webContentsPtr,
    /* [in] */ Int64 viewAndroidPtr,
    /* [in] */ Int64 windowAndroidPtr,
    /* [in] */ HashSet<Object> retainedObjectSet)
{
}

//@CalledByNative
AutoPtr<ContentVideoViewClient> ContentViewCore::GetContentVideoViewClient()
{
    return GetContentViewClient()->GetContentVideoViewClient();
}

//@CalledByNative
Boolean ContentViewCore::ShouldBlockMediaRequest(
    /* [in] */ const String& url)
{
    return GetContentViewClient()->ShouldBlockMediaRequest(url);
}

//@CalledByNative
void ContentViewCore::OnNativeFlingStopped()
{
    // Note that mTouchScrollInProgress should normally be false at this
    // point, but we reset it anyway as another failsafe.
    mTouchScrollInProgress = FALSE;
    if (mPotentiallyActiveFlingCount <= 0) {
        return;
    }
    mPotentiallyActiveFlingCount--;
    UpdateGestureStateListener(GestureEventType.FLING_END);
}

//@Override
void ContentViewCore::OnScreenOrientationChanged(
    /* [in] */ Int32 orientation)
{
    SendOrientationChangeEvent(orientation);
}

AutoPtr<WebContents> ContentViewCore::NativeGetWebContentsAndroid(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeOnJavaContentViewCoreDestroyed(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeLoadUrl(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ const String& url,
    /* [in] */ Int32 loadUrlType,
    /* [in] */ Int32 transitionType,
    /* [in] */ const String& referrerUrl,
    /* [in] */ Int32 referrerPolicy,
    /* [in] */ Int32 uaOverrideOption,
    /* [in] */ const String& extraHeaders,
    /* [in] */ ArrayOf<Byte>* postData,
    /* [in] */ const String& baseUrlForDataUrl,
    /* [in] */ const String& virtualUrlForDataUrl,
    /* [in] */ Boolean canLoadLocalResources,
    /* [in] */ Boolean isRendererInitiated)
{
}

String ContentViewCore::NativeGetURL(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeShowInterstitialPage(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ const String& url,
    /* [in] */ Int64 nativeInterstitialPageDelegateAndroid)
{
}

Boolean ContentViewCore::NativeIsShowingInterstitialPage(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

Boolean ContentViewCore::NativeIsIncognito(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeSetFocus(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean focused)
{
}

void ContentViewCore::NativeSendOrientationChangeEvent(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int32 orientation)
{
}

// All touch events (including flings, scrolls etc) accept coordinates in physical pixels.
Boolean ContentViewCore::NativeOnTouchEvent(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int64 timeMs,
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ Int32 historySize,
    /* [in] */ Int32 actionIndex,
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Int32 pointerId0,
    /* [in] */ Int32 pointerId1,
    /* [in] */ Float touchMajor0,
    /* [in] */ Float touchMajor1,
    /* [in] */ Float rawX,
    /* [in] */ Float rawY,
    /* [in] */ Int32 androidToolType0,
    /* [in] */ Int32 androidToolType1,
    /* [in] */ Int32 androidButtonState)
{
}

Int32 ContentViewCore::NativeSendMouseMoveEvent(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
}

Int32 ContentViewCore::NativeSendMouseWheelEvent(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float verticalAxis)
{
}

void ContentViewCore::NativeScrollBegin(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float hintX,
    /* [in] */ Float hintY)
{
}

void ContentViewCore::NativeScrollEnd(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs)
{
}

void ContentViewCore::NativeScrollBy(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float deltaX,
    /* [in] */ Float deltaY)
{
}

void ContentViewCore::NativeFlingStart(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float vx,
    /* [in] */ Float vy)
{
}

void ContentViewCore::NativeFlingCancel(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs)
{
}

void ContentViewCore::NativeSingleTap(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
}

void ContentViewCore::NativeDoubleTap(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
}

void ContentViewCore::NativeLongPress(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
}

void ContentViewCore::NativePinchBegin(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
}

void ContentViewCore::NativePinchEnd(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs)
{
}

void ContentViewCore::NativePinchBy(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int64 timeMs,
    /* [in] */ Float anchorX,
    /* [in] */ Float anchorY,
    /* [in] */ Float deltaScale)
{
}

void ContentViewCore::NativeSelectBetweenCoordinates(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2)
{
}

void ContentViewCore::NativeMoveCaret(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
}

void ContentViewCore::NativeResetGestureDetection(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeSetDoubleTapSupportEnabled(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean enabled)
{
}

void ContentViewCore::NativeSetMultiTouchZoomSupportEnabled(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean enabled)
{
}

void ContentViewCore::NativeLoadIfNecessary(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeRequestRestoreLoad(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeReload(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean checkForRepost)
{
}

void ContentViewCore::NativeReloadIgnoringCache(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean checkForRepost)
{
}

void ContentViewCore::NativeCancelPendingReload(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeContinuePendingReload(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeSelectPopupMenuItems(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ ArrayOf<Int32>* indices)
{
}

void ContentViewCore::NativeScrollFocusedEditableNodeIntoView(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeSelectWordAroundCaret(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeClearHistory(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeAddStyleSheetByURL(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ const String& stylesheetUrl)
{
}

void ContentViewCore::NativeEvaluateJavaScript(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ const String& script,
    /* [in] */ JavaScriptCallback* callback,
    /* [in] */ Boolean startRenderer)
{
}

Int64 ContentViewCore::NativeGetNativeImeAdapter(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

Int32 ContentViewCore::NativeGetCurrentRenderProcessId(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

Int32 ContentViewCore::NativeGetBackgroundColor(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeOnShow(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeOnHide(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeSetUseDesktopUserAgent(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean reloadOnChange)
{
}

Boolean ContentViewCore::NativeGetUseDesktopUserAgent(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeClearSslPreferences(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeSetAllowJavascriptInterfacesInspection(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean allow)
{
}

void ContentViewCore::NativeAddJavascriptInterface(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ IInterface* object,
    /* [in] */ const String& name,
    /* [in] */ Class requiredAnnotation)
{
}

void ContentViewCore::NativeRemoveJavascriptInterface(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ const String& name)
{
}

Int32 ContentViewCore::NativeGetNavigationHistory(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ IInterface* context)
{
}

void ContentViewCore::NativeGetDirectedNavigationHistory(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ IInterface* context,
    /* [in] */ Boolean isForward,
    /* [in] */ Int32 maxEntries)
{
}

String ContentViewCore::NativeGetOriginalUrlForActiveNavigationEntry(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeWasResized(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

Boolean ContentViewCore::NativeIsRenderWidgetHostViewReady(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeExitFullscreen(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeUpdateTopControlsState(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean enableHiding,
    /* [in] */ Boolean enableShowing,
    /* [in] */ Boolean animate)
{
}

void ContentViewCore::NativeShowImeIfNeeded(
    /* [in] */ Int64 nativeContentViewCoreImpl)
{
}

void ContentViewCore::NativeSetAccessibilityEnabled(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean enabled)
{
}

void ContentViewCore::NativeExtractSmartClipData(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
}

void ContentViewCore::NativeSetBackgroundOpaque(
    /* [in] */ Int64 nativeContentViewCoreImpl,
    /* [in] */ Boolean opaque)
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
