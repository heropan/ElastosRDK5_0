#include "app/CActionBarImpl.h"
#include "view/CMenuInflater.h"
#include "view/LayoutInflater.h"
#include "view/CContextThemeWrapper.h"
#include "view/menu/CMenuBuilder.h"
#include "view/menu/CMenuPopupHelper.h"
#include "view/animation/AnimationUtils.h"
#include "animation/CAnimatorSet.h"
#include "animation/CObjectAnimator.h"
#include "widget/internal/CScrollingTabContainerView.h"
#include "util/CTypedValue.h"
#include "os/CHandler.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::App::IFragmentManager;
using Elastos::Droid::App::EIID_IActionBarTab;
using Elastos::Droid::App::IFragmentTransaction;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Animation::IAnimatorSet;
using Elastos::Droid::Animation::CAnimatorSet;
using Elastos::Droid::Animation::CObjectAnimator;
using Elastos::Droid::Animation::IAnimatorSetBuilder;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::CContextThemeWrapper;
using Elastos::Droid::View::CMenuInflater;
using Elastos::Droid::View::Menu::CMenuBuilder;
using Elastos::Droid::View::Menu::IMenuPopupHelper;
using Elastos::Droid::View::Menu::CMenuPopupHelper;
using Elastos::Droid::View::Menu::EIID_IMenuBuilderCallback;
using Elastos::Droid::View::Animation::AnimationUtils;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::Widget::Internal::IScrollingTabContainerView;
using Elastos::Droid::Widget::Internal::CScrollingTabContainerView;


namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

const String CActionBarImpl::TAG("CActionBarImpl");
const Int32 CActionBarImpl::CONTEXT_DISPLAY_NORMAL;
const Int32 CActionBarImpl::CONTEXT_DISPLAY_SPLIT;
const Int32 CActionBarImpl::INVALID_POSITION;

ECode CActionBarImpl::HideAnimatorListenerAdapter::OnAnimationEnd(
            /* [in] */ IAnimator* animation)
{
    if (mHost->mContentView != NULL) {
        mHost->mContentView->SetTranslationY(0);
        mHost->mTopVisibilityView->SetTranslationY(0);
    }
    if (mHost->mSplitView != NULL && mHost->mContextDisplayMode == CActionBarImpl::CONTEXT_DISPLAY_SPLIT) {
        mHost->mSplitView->SetVisibility(IView::GONE);
    }
    mHost->mTopVisibilityView->SetVisibility(IView::GONE);
    mHost->mContainerView->SetTransitioning(FALSE);
    mHost->mCurrentShowAnim = NULL;
    mHost->CompleteDeferredDestroyActionMode();
    if (mHost->mOverlayLayout != NULL) {
        mHost->mOverlayLayout->RequestFitSystemWindows();
    }
    return NOERROR;
}

ECode CActionBarImpl::ShowAnimatorListenerAdapter::OnAnimationEnd(
            /* [in] */ IAnimator* animation)
{
    mHost->mCurrentShowAnim = NULL;
    mHost->mTopVisibilityView->RequestLayout();
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(CActionBarImpl::ActionModeImpl, IActionMode, IMenuBuilderCallback)

CActionBarImpl::ActionModeImpl::ActionModeImpl(
    /* [in] */ IActionModeCallback* callback,
    /* [in] */ CActionBarImpl* actionBarImpl)
    : mActionBarImpl(actionBarImpl)
{
    mCallback = callback;
    AutoPtr<IContext> cxt;
    mActionBarImpl->GetThemedContext((IContext**)&cxt);
    AutoPtr<IMenuBuilder> menuBuilder;
    CMenuBuilder::New(cxt, (IMenuBuilder**)&menuBuilder);
    menuBuilder->SetDefaultShowAsAction(IMenuItem::SHOW_AS_ACTION_IF_ROOM, (IMenuBuilder**)&mMenu);
    mMenu->SetCallback(this);
}

CActionBarImpl::ActionModeImpl::~ActionModeImpl()
{
}

ECode CActionBarImpl::ActionModeImpl::GetMenuInflater(
    /* [out] */ IMenuInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);
    AutoPtr<IContext> cxt;
    mActionBarImpl->GetThemedContext((IContext**)&cxt);
    return CMenuInflater::New(cxt, inflater);
}

ECode CActionBarImpl::ActionModeImpl::GetMenu(
    /* [out] */ IMenu** menu)
{
    VALIDATE_NOT_NULL(menu);
    *menu = mMenu;
    REFCOUNT_ADD(*menu)
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::Finish()
{
    if (mActionBarImpl->mActionMode.Get() != this) {
        // Not the active action mode - no-op
        return NOERROR;
    }

    // If this change in state is going to cause the action bar
    // to be hidden, defer the onDestroy callback until the animation
    // is finished and associated relayout is about to happen. This lets
    // apps better anticipate visibility and layout behavior.
    if (!CActionBarImpl::CheckShowingFlags(mActionBarImpl->mHiddenByApp, mActionBarImpl->mHiddenBySystem, FALSE))
    {
        // With the current state but the action bar hidden, our
        // overall showing state is going to be false.
        mActionBarImpl->mDeferredDestroyActionMode = this;
        mActionBarImpl->mDeferredModeDestroyCallback = mCallback;
    } else {
        mCallback->OnDestroyActionMode(this);
    }
    mCallback = NULL;
    mActionBarImpl->AnimateToMode(FALSE);

    // Clear out the context mode views after the animation finishes
    mActionBarImpl->mContextView->CloseMode();
    IAccessibilityEventSource* source = IAccessibilityEventSource::Probe(mActionBarImpl->mActionView);
    assert(source != NULL);
    if (source) {
        source->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED);
    }

    mActionBarImpl->mActionMode = NULL;
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::Invalidate()
{
    mMenu->StopDispatchingItemsChanged();
//     try {
    Boolean result;
    ECode ec = mCallback->OnPrepareActionMode(this, mMenu, &result);
//     } finally {
    mMenu->StartDispatchingItemsChanged();
//     }
    return ec;
}

ECode CActionBarImpl::ActionModeImpl::DispatchOnCreate(
    /* [out] */ Boolean* result)
{
    mMenu->StopDispatchingItemsChanged();
//     try {
    ECode ec = mCallback->OnCreateActionMode(this, mMenu, result);
//     } finally {
    mMenu->StartDispatchingItemsChanged();
//     }
    return ec;
}

ECode CActionBarImpl::ActionModeImpl::SetCustomView(
    /* [in] */ IView* view)
{
    mActionBarImpl->mContextView->SetCustomView(view);
    mCustomView = view;
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    mActionBarImpl->mContextView->SetSubtitle(subtitle);
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mActionBarImpl->mContextView->SetTitle(title);
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::SetTitle(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> res;
    mActionBarImpl->mContext->GetResources((IResources**)&res);
    AutoPtr<ICharSequence> strId;
    res->GetText(resId, (ICharSequence**)&strId);
    SetTitle(strId);
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::SetSubtitle(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> res;
    mActionBarImpl->mContext->GetResources((IResources**)&res);
    AutoPtr<ICharSequence> strId;
    res->GetText(resId, (ICharSequence**)&strId);
    SetSubtitle(strId);
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    return mActionBarImpl->mContextView->GetTitle(title);
}

ECode CActionBarImpl::ActionModeImpl::GetSubtitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    return mActionBarImpl->mContextView->GetSubtitle(title);
}

ECode CActionBarImpl::ActionModeImpl::SetTitleOptionalHint(
    /* [in] */ Boolean titleOptional)
{
    ActionMode::SetTitleOptionalHint(titleOptional);
    mActionBarImpl->mContextView->SetTitleOptional(titleOptional);
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::IsTitleOptional(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mActionBarImpl->mContextView->IsTitleOptional(result);
}

ECode CActionBarImpl::ActionModeImpl::GetCustomView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mCustomView != NULL ? mCustomView : NULL;
    REFCOUNT_ADD(*view)
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    if (mCallback != NULL) {
        mCallback->OnActionItemClicked(this, item, result);
        return NOERROR;
    } else {
        *result = FALSE;
        return NOERROR;
    }
}

ECode CActionBarImpl::ActionModeImpl::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    if (mCallback == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    Boolean hasItems;
    subMenu->HasVisibleItems(&hasItems);
    if (!hasItems) {
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<IContext> cxt;
    mActionBarImpl->GetThemedContext((IContext**)&cxt);
    AutoPtr<IMenuPopupHelper> helper;
    CMenuPopupHelper::New(cxt, subMenu, (IMenuPopupHelper**)&helper);
    helper->Show();
    *result = TRUE;
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::OnCloseSubMenu(
    /* [in] */ ISubMenuBuilder* menu)
{
    return NOERROR;
}

ECode CActionBarImpl::ActionModeImpl::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    if (mCallback == NULL) {
        return NOERROR;
    }
    Invalidate();
    Boolean showMenu;
    mActionBarImpl->mContextView->ShowOverflowMenu(&showMenu);
    return NOERROR;
}

CActionBarImpl::TabImpl::TabImpl(
    /* [in] */ CActionBarImpl* actionBarImpl)
    : mPosition(-1)
    , mActionBarImpl(actionBarImpl)
{}

CAR_INTERFACE_IMPL_2(CActionBarImpl::TabImpl, IActionBarTab, IObject)

ECode CActionBarImpl::TabImpl::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result)
    *result = (IActionBarTab::Probe(other) == THIS_PROBE(IActionBarTab));
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash)
    *hash = (Int32)THIS_PROBE(IActionBarTab);
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = String("CActionBarImpl::TabImpl");
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetTag(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = mTag;
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetTag(
    /* [in] */ IInterface* tag)
{
    mTag = tag;
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetCallback(
    /* [out] */ IActionBarTabListener** cb)
{
    VALIDATE_NOT_NULL(cb);
    *cb = mCallback;
    REFCOUNT_ADD(*cb)
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetTabListener(
    /* [in] */ IActionBarTabListener* listener)
{
    mCallback = listener;
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetCustomView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mCustomView;
    REFCOUNT_ADD(*view)
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetCustomView(
    /* [in] */ IView* view)
{
    mCustomView = view;
    if (mPosition >= 0) {
        mActionBarImpl->mTabScrollView->UpdateTab(mPosition);
    }
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetCustomView(
    /* [in] */ Int32 layoutResId)
{
    AutoPtr<IContext> cxt;
    mActionBarImpl->GetThemedContext((IContext**)&cxt);
    AutoPtr<ILayoutInflater> inflater;
    LayoutInflater::From(cxt, (ILayoutInflater**)&inflater);
    AutoPtr<IView> view;
    inflater->Inflate(layoutResId, NULL, (IView**)&view);
    return SetCustomView(view);
}

ECode CActionBarImpl::TabImpl::GetIcon(
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    REFCOUNT_ADD(*icon)
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mPosition;
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetPosition(
    /* [in] */ Int32 position)
{
    mPosition = position;
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    REFCOUNT_ADD(*text)
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIcon = icon;
    if (mPosition >= 0) {
        mActionBarImpl->mTabScrollView->UpdateTab(mPosition);
    }
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetIcon(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mActionBarImpl->mContext->GetResources((IResources**)&resources);
    AutoPtr<IDrawable> drawable;
    resources->GetDrawable(resId, (IDrawable**)&drawable);
    return SetIcon(drawable);
}

ECode CActionBarImpl::TabImpl::SetText(
    /* [in] */ ICharSequence* text)
{
    mText = text;
    if (mPosition >= 0) {
        mActionBarImpl->mTabScrollView->UpdateTab(mPosition);
    }
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetText(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mActionBarImpl->mContext->GetResources((IResources**)&resources);
    AutoPtr<ICharSequence> textId;
    resources->GetText(resId, (ICharSequence**)&textId);
    return SetText(textId);
}

ECode CActionBarImpl::TabImpl::Select()
{
    mActionBarImpl->SelectTab(this);
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::SetContentDescription(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mActionBarImpl->mContext->GetResources((IResources**)&resources);
    AutoPtr<ICharSequence> textId;
    resources->GetText(resId, (ICharSequence**)&textId);
    return SetContentDescription(textId);
}

ECode CActionBarImpl::TabImpl::SetContentDescription(
    /* [in] */ ICharSequence* contentDesc)
{
    mContentDesc = contentDesc;
    if (mPosition >= 0) {
        mActionBarImpl->mTabScrollView->UpdateTab(mPosition);
    }
    return NOERROR;
}

ECode CActionBarImpl::TabImpl::GetContentDescription(
    /* [out] */ ICharSequence** description)
{
    VALIDATE_NOT_NULL(description);
    *description = mContentDesc;
    REFCOUNT_ADD(*description)
    return NOERROR;
}

CActionBarImpl::CActionBarImpl()
    : mSavedTabPosition(INVALID_POSITION)
    , mDisplayHomeAsUpSet(FALSE)
    , mLastMenuVisibility(FALSE)
    , mContextDisplayMode(0)
    , mHasEmbeddedTabs(FALSE)
    , mCurWindowVisibility(IView::VISIBLE)
    , mHiddenByApp(FALSE)
    , mHiddenBySystem(FALSE)
    , mShowingForMode(FALSE)
    , mNowShowing(TRUE)
    , mShowHideAnimationEnabled(FALSE)
{
    CHandler::New((IHandler**)&mHandler);
    mHideListener = new HideAnimatorListenerAdapter(this);
    mShowListener = new ShowAnimatorListenerAdapter(this);
}

ECode CActionBarImpl::constructor(
    /* [in] */ IActivity* activity)
{
    VALIDATE_NOT_NULL(activity);

    mActivity = activity;
    AutoPtr<IWindow> window;
    activity->GetWindow((IWindow**)&window);
    AutoPtr<IView> decor;
    window->GetDecorView((IView**)&decor);
    Init(decor);
    AutoPtr<IWindow> w;
    mActivity->GetWindow((IWindow**)&w);
    Boolean hasFeature;
    w->HasFeature(IWindow::FEATURE_ACTION_BAR_OVERLAY, &hasFeature);
    if (!hasFeature) {
        decor->FindViewById(R::id::content, (IView**)&mContentView);
    }
    return NOERROR;
}

ECode CActionBarImpl::constructor(
    /* [in] */ IDialog* dialog)
{
    mDialog = dialog;
    AutoPtr<IWindow> w;
    dialog->GetWindow((IWindow**)&w);
    AutoPtr<IView> v;
    w->GetDecorView((IView**)&v);
    Init(v);
    return NOERROR;
}

ECode CActionBarImpl::Init(
    /* [in] */ IView* decor)
{
    decor->GetContext((IContext**)&mContext);
    decor->FindViewById(
            R::id::action_bar_overlay_layout, (IView**)&mOverlayLayout);
    if (mOverlayLayout != NULL) {
        mOverlayLayout->SetActionBar(this);
    }
    decor->FindViewById(R::id::action_bar, (IView**)&mActionView);
    assert(mActionView != NULL);
    decor->FindViewById(
            R::id::action_context_bar, (IView**)&mContextView);
    decor->FindViewById(
            R::id::action_bar_container, (IView**)&mContainerView);
    decor->FindViewById(
            R::id::top_action_bar, (IView**)&mTopVisibilityView);
    if (mTopVisibilityView == NULL) {
        mTopVisibilityView = mContainerView;
    }
    decor->FindViewById(
            R::id::split_action_bar, (IView**)&mSplitView);

    if (mActionView == NULL || mContextView == NULL || mContainerView == NULL) {
//         throw new IllegalStateException(getClass().getSimpleName() + " can only be used " +
//                 "with a compatible window decor layout");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mActionView->SetContextView(mContextView);
    Boolean isSplit;
    mActionView->IsSplitActionBar(&isSplit);
    mContextDisplayMode = isSplit ?
            CONTEXT_DISPLAY_SPLIT : CONTEXT_DISPLAY_NORMAL;

    // This was initially read from the action bar style
    Int32 current;
    mActionView->GetDisplayOptions(&current);
    const Boolean homeAsUp = (current & DISPLAY_HOME_AS_UP) != 0;
    if (homeAsUp) {
        mDisplayHomeAsUpSet = TRUE;
    }

    AutoPtr<IActionBarPolicy> abp;
    CActionBarPolicy::New(mContext, (IActionBarPolicy**)&abp);
    Boolean eb, hasTabs;
    abp->EnableHomeButtonByDefault(&eb);
    SetHomeButtonEnabled(eb || homeAsUp);
    abp->HasEmbeddedTabs(&hasTabs);
    SetHasEmbeddedTabs(hasTabs);
    return NOERROR;
}

ECode CActionBarImpl::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr<IActionBarPolicy> abp;
    CActionBarPolicy::New(mContext, (IActionBarPolicy**)&abp);
    Boolean hasTabs;
    abp->HasEmbeddedTabs(&hasTabs);
    SetHasEmbeddedTabs(hasTabs);
    return NOERROR;
}

void CActionBarImpl::SetHasEmbeddedTabs(
    /* [in] */ Boolean hasEmbeddedTabs)
{
    mHasEmbeddedTabs = hasEmbeddedTabs;
    // Switch tab layout configuration if needed
    if (!mHasEmbeddedTabs) {
        mActionView->SetEmbeddedTabView(NULL);
        mContainerView->SetTabContainer(mTabScrollView);
    } else {
        mContainerView->SetTabContainer(NULL);
        mActionView->SetEmbeddedTabView(mTabScrollView);
    }
    Int32 mode;
    GetNavigationMode(&mode);
    const Boolean isInTabMode = mode == NAVIGATION_MODE_TABS;
    if (mTabScrollView != NULL) {
        if (isInTabMode) {
            mTabScrollView->SetVisibility(IView::VISIBLE);
            if (mOverlayLayout != NULL) {
                mOverlayLayout->RequestFitSystemWindows();
            }
        } else {
            mTabScrollView->SetVisibility(IView::GONE);
        }
    }
    mActionView->SetCollapsable(!mHasEmbeddedTabs && isInTabMode);
}

ECode CActionBarImpl::HasNonEmbeddedTabs(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 mode;
    GetNavigationMode(&mode);
    *result = !mHasEmbeddedTabs && mode == NAVIGATION_MODE_TABS;
    return NOERROR;
}

void CActionBarImpl::EnsureTabsExist()
{
    if (mTabScrollView != NULL) {
        return ;
    }

    AutoPtr<IScrollingTabContainerView> tabScroller;
    CScrollingTabContainerView::New(mContext, (IScrollingTabContainerView**)&tabScroller);

    if (mHasEmbeddedTabs) {
        tabScroller->SetVisibility(IView::VISIBLE);
        mActionView->SetEmbeddedTabView(tabScroller);
    } else {
        Int32 mode;
        GetNavigationMode(&mode);
        if (mode == NAVIGATION_MODE_TABS) {
            tabScroller->SetVisibility(IView::VISIBLE);
            if (mOverlayLayout != NULL) {
                mOverlayLayout->RequestFitSystemWindows();
            }
        } else {
            tabScroller->SetVisibility(IView::GONE);
        }
        mContainerView->SetTabContainer(tabScroller);
    }
    mTabScrollView = tabScroller;
}

ECode CActionBarImpl::CompleteDeferredDestroyActionMode()
{
    if (mDeferredModeDestroyCallback != NULL) {
        mDeferredModeDestroyCallback->OnDestroyActionMode(mDeferredDestroyActionMode);
        mDeferredDestroyActionMode = NULL;
        mDeferredModeDestroyCallback = NULL;
    }
    return NOERROR;
}

ECode CActionBarImpl::SetWindowVisibility(
    /* [in] */ Int32 visibility)
{
    mCurWindowVisibility = visibility;
    return NOERROR;
}

/**
 * Enables or disables animation between show/hide states.
 * If animation is disabled using this method, animations in progress
 * will be finished.
 *
 * @param enabled true to animate, false to not animate.
 */
ECode CActionBarImpl::SetShowHideAnimationEnabled(
        /* [in] */ Boolean enabled)
{
    mShowHideAnimationEnabled = enabled;
    if (!enabled && mCurrentShowAnim != NULL) {
        mCurrentShowAnim->End();
    }
    return NOERROR;
}

ECode CActionBarImpl::AddOnMenuVisibilityListener(
    /* [in] */ IActionBarOnMenuVisibilityListener* listener)
{
    mMenuVisibilityListeners.PushBack(listener);
    return NOERROR;
}

ECode CActionBarImpl::RemoveOnMenuVisibilityListener(
    /* [in] */ IActionBarOnMenuVisibilityListener* listener)
{
   mMenuVisibilityListeners.Remove(listener);
    return NOERROR;
}

ECode CActionBarImpl::DispatchMenuVisibilityChanged(
    /* [in] */ Boolean isVisible)
{
    if (isVisible == mLastMenuVisibility) {
        return NOERROR;
    }
    mLastMenuVisibility = isVisible;

    List< AutoPtr<IActionBarOnMenuVisibilityListener> >::Iterator it;
    for (it = mMenuVisibilityListeners.Begin(); it != mMenuVisibilityListeners.End(); ++it)
    {
        (*it)->OnMenuVisibilityChanged(isVisible);
    }
    return NOERROR;
}

ECode CActionBarImpl::SetCustomView(
    /* [in] */ Int32 resId)
{
    AutoPtr<IContext> cxt;
    GetThemedContext((IContext**)&cxt);
    AutoPtr<ILayoutInflater> inflater;
    LayoutInflater::From(cxt, (ILayoutInflater**)&inflater);
    AutoPtr<IView> view;
    inflater->Inflate(resId, mActionView, FALSE, (IView**)&view);
    SetCustomView(view);
    return NOERROR;
}

ECode CActionBarImpl::SetDisplayUseLogoEnabled(
    /* [in] */ Boolean useLogo)
{
    return SetDisplayOptions(useLogo ? DISPLAY_USE_LOGO : 0, DISPLAY_USE_LOGO);
}

ECode CActionBarImpl::SetDisplayShowHomeEnabled(
    /* [in] */ Boolean showHome)
{
    return SetDisplayOptions(showHome ? DISPLAY_SHOW_HOME : 0, DISPLAY_SHOW_HOME);
}

ECode CActionBarImpl::SetDisplayHomeAsUpEnabled(
        /* [in] */ Boolean showHomeAsUp)
{
    return SetDisplayOptions(showHomeAsUp ? DISPLAY_HOME_AS_UP : 0, DISPLAY_HOME_AS_UP);
}

ECode CActionBarImpl::SetDisplayShowTitleEnabled(
    /* [in] */ Boolean showTitle)
{
    return SetDisplayOptions(showTitle ? DISPLAY_SHOW_TITLE : 0, DISPLAY_SHOW_TITLE);
}

ECode CActionBarImpl::SetDisplayShowCustomEnabled(
    /* [in] */ Boolean showCustom)
{
    return SetDisplayOptions(showCustom ? DISPLAY_SHOW_CUSTOM : 0, DISPLAY_SHOW_CUSTOM);
}

ECode CActionBarImpl::SetHomeButtonEnabled(
    /* [in] */ Boolean enable)
{
    mActionView->SetHomeButtonEnabled(enable);
    return NOERROR;
}

ECode CActionBarImpl::SetTitle(
    /* [in] */ Int32 resId)
{
    AutoPtr<ICharSequence> textId;
    mContext->GetText(resId, (ICharSequence**)&textId);
    SetTitle(textId);
    return NOERROR;
}

ECode CActionBarImpl::SetSubtitle(
    /* [in] */ Int32 resId)
{
    AutoPtr<ICharSequence> textId;
    mContext->GetText(resId, (ICharSequence**)&textId);
    SetSubtitle(textId);
    return NOERROR;
}

ECode CActionBarImpl::SetSelectedNavigationItem(
    /* [in] */ Int32 position)
{
    Int32 mode;
    mActionView->GetNavigationMode(&mode);
    switch (mode) {
    case NAVIGATION_MODE_TABS:
        SelectTab(mTabs[position]);
        break;
    case NAVIGATION_MODE_LIST:
        mActionView->SetDropdownSelectedPosition(position);
        break;
    default:
//         throw new IllegalStateException(
//                 "setSelectedNavigationIndex not valid for current navigation mode");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CActionBarImpl::RemoveAllTabs()
{
    return CleanupTabs();
}

ECode CActionBarImpl::CleanupTabs()
{
    if (mSelectedTab != NULL) {
        SelectTab(NULL);
    }
    mTabs.Clear();
    if (mTabScrollView != NULL) {
        mTabScrollView->RemoveAllTabs();
    }
    mSavedTabPosition = INVALID_POSITION;
    return NOERROR;
}

ECode CActionBarImpl::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mActionView->SetTitle(title);
    return NOERROR;
}

ECode CActionBarImpl::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    mActionView->SetSubtitle(subtitle);
    return NOERROR;
}

ECode CActionBarImpl::SetDisplayOptions(
    /* [in] */ Int32 options)
{
    if ((options & DISPLAY_HOME_AS_UP) != 0) {
        mDisplayHomeAsUpSet = TRUE;
    }
    mActionView->SetDisplayOptions(options);
    return NOERROR;
}

ECode CActionBarImpl::SetDisplayOptions(
    /* [in] */ Int32 options,
    /* [in] */ Int32 mask)
{
    Int32 current;
    assert(mActionView != NULL);
    mActionView->GetDisplayOptions(&current);
    if ((mask & DISPLAY_HOME_AS_UP) != 0) {
        mDisplayHomeAsUpSet = TRUE;
    }
    mActionView->SetDisplayOptions((options & mask) | (current & ~mask));
    return NOERROR;
}

ECode CActionBarImpl::SetBackgroundDrawable(
        /* [in] */ IDrawable* d)
{
    return mContainerView->SetPrimaryBackground(d);
}

ECode CActionBarImpl::SetStackedBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return mContainerView->SetStackedBackground(d);
}

ECode CActionBarImpl::SetSplitBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    if (mSplitView != NULL) {
        mSplitView->SetSplitBackground(d);
    }
    return NOERROR;
}

ECode CActionBarImpl::GetCustomView(
    /* [out] */ IView** view)
{
    return mActionView->GetCustomNavigationView(view);
}

ECode CActionBarImpl::GetTitle(
    /* [out] */ ICharSequence** title)
{
    return mActionView->GetTitle(title);
}

ECode CActionBarImpl::GetSubtitle(
    /* [out] */ ICharSequence** title)
{
    return mActionView->GetSubtitle(title);
}

ECode CActionBarImpl::GetNavigationMode(
    /* [out] */ Int32* mode)
{
    return mActionView->GetNavigationMode(mode);
}

ECode CActionBarImpl::GetDisplayOptions(
    /* [out] */ Int32* opt)
{
    return mActionView->GetDisplayOptions(opt);
}

ECode CActionBarImpl::StartActionMode(
    /* [in] */ IActionModeCallback* callback,
    /* [out] */ IActionMode** actionmode)
{
    if (mActionMode != NULL) {
        mActionMode->Finish();
    }

    mContextView->KillMode();
    AutoPtr<ActionModeImpl> mode = new ActionModeImpl(callback, this);
    Boolean dispatch;
    mode->DispatchOnCreate(&dispatch);
    if (dispatch) {
        mode->Invalidate();
        mContextView->InitForMode(mode);
        AnimateToMode(TRUE);
        if (mSplitView != NULL && mContextDisplayMode == CONTEXT_DISPLAY_SPLIT) {
            // TODO animate this
            Int32 visibility;
            mSplitView->GetVisibility(&visibility);
            if (visibility != IView::VISIBLE) {
                mSplitView->SetVisibility(IView::VISIBLE);
                if (mOverlayLayout != NULL) {
                    mOverlayLayout->RequestFitSystemWindows();
                }
            }
        }

        IAccessibilityEventSource* source = IAccessibilityEventSource::Probe(mContextView);
        assert(source);
        if (source) {
            source->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED);
        }
        mActionMode = mode;
        *actionmode = mode;
        REFCOUNT_ADD(*actionmode);
        return NOERROR;
    }
    *actionmode = NULL;
    return NOERROR;
}

void CActionBarImpl::ConfigureTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Int32 position)
{
    TabImpl* tabi = (TabImpl*) tab;
    AutoPtr<IActionBarTabListener> callback;
    tabi->GetCallback((IActionBarTabListener**)&callback);

    if (callback == NULL) {
//         throw new IllegalStateException("Action Bar Tab must have a Callback");
    }

    tabi->SetPosition(position);
    mTabs.Insert(position, tabi);

    Int32 count = mTabs.GetSize();
    for (Int32 i = position + 1; i < count; i++) {
        mTabs[i]->SetPosition(i);
    }
}

ECode CActionBarImpl::AddTab(
    /* [in] */ IActionBarTab* tab)
{
    return AddTab(tab, mTabs.IsEmpty());
}

ECode CActionBarImpl::AddTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Int32 position)
{
    return AddTab(tab, position, mTabs.IsEmpty());
}

ECode CActionBarImpl::AddTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Boolean setSelected)
{
    EnsureTabsExist();
    mTabScrollView->AddTab(tab, setSelected);
    ConfigureTab(tab, mTabs.GetSize());
    if (setSelected) {
        SelectTab(tab);
    }
    return NOERROR;
}

ECode CActionBarImpl::AddTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Int32 position,
    /* [in] */ Boolean setSelected)
{
    EnsureTabsExist();
    mTabScrollView->AddTab(tab, position, setSelected);
    ConfigureTab(tab, position);
    if (setSelected) {
        SelectTab(tab);
    }
    return NOERROR;
}

ECode CActionBarImpl::NewTab(
    /* [out] */ IActionBarTab** tab)
{
   *tab = new TabImpl(this);
   REFCOUNT_ADD(*tab)
    return NOERROR;
}

ECode CActionBarImpl::RemoveTab(
    /* [in] */ IActionBarTab* tab)
{
    Int32 position;
    tab->GetPosition(&position);
    RemoveTabAt(position);
    return NOERROR;
}

ECode CActionBarImpl::RemoveTabAt(
    /* [in] */ Int32 position)
{
    if (mTabScrollView == NULL) {
        // No tabs around to remove
        return NOERROR;
    }

    Int32 pos;
    mSelectedTab->GetPosition(&pos);
    Int32 selectedTabPosition = mSelectedTab != NULL
            ? pos : mSavedTabPosition;
    mTabScrollView->RemoveTabAt(position);
    TabImpl* removedTab = mTabs[position];
    mTabs.Remove(position);
    if (removedTab != NULL) {
        removedTab->SetPosition(-1);
    }

    Int32 newTabCount = mTabs.GetSize();
    for (Int32 i = position; i < newTabCount; i++) {
        mTabs[i]->SetPosition(i);
    }

    Int32 max = 0 > position - 1 ? 0 : position - 1;
    if (selectedTabPosition == position) {
        SelectTab(mTabs.IsEmpty() ? NULL : mTabs[max]);
    }
    return NOERROR;
}

ECode CActionBarImpl::SelectTab(
    /* [in] */ IActionBarTab* tab)
{
    Int32 mode;
    GetNavigationMode(&mode);
    Int32 pos;
    if (mode != NAVIGATION_MODE_TABS) {
        mSavedTabPosition = tab != NULL ? (tab->GetPosition(&pos), pos) : INVALID_POSITION;
        return NOERROR;
    }

    AutoPtr<IFragmentManager> fmg;
    mActivity->GetFragmentManager((IFragmentManager**)&fmg);
    AutoPtr<IFragmentTransaction> trans;
    fmg->BeginTransaction((IFragmentTransaction**)&trans);
    trans->DisallowAddToBackStack();

    if ((mSelectedTab == NULL && tab == NULL) || (mSelectedTab != NULL && (IActionBarTab*)mSelectedTab->Probe(EIID_IActionBarTab) == tab)) {
        if (mSelectedTab != NULL) {
            AutoPtr<IActionBarTabListener> cb;
            mSelectedTab->GetCallback((IActionBarTabListener**)&cb);
            cb->OnTabReselected(mSelectedTab, trans);
            Int32 pos;
            tab->GetPosition(&pos);
            mTabScrollView->AnimateToTab(pos);
        }
    } else {
        Int32 pos;
        mTabScrollView->SetTabSelected(tab != NULL ? (tab->GetPosition(&pos), pos) : IActionBarTab::INVALID_POSITION);
        AutoPtr<IActionBarTabListener> cb;
        if (mSelectedTab != NULL) {
            mSelectedTab->GetCallback((IActionBarTabListener**)&cb);
            cb->OnTabUnselected(mSelectedTab, trans);
        }

        mSelectedTab = (TabImpl*) tab;
        if (mSelectedTab != NULL) {
            cb = NULL;
            mSelectedTab->GetCallback((IActionBarTabListener**)&cb);
            cb->OnTabSelected(mSelectedTab, trans);
        }
    }

    Boolean isEmpty;
    trans->IsEmpty(&isEmpty);
    if (!isEmpty) {
        Int32 identifier;
        trans->Commit(&identifier);
    }
    return NOERROR;
}

ECode CActionBarImpl::GetSelectedTab(
    /* [out] */ IActionBarTab** tab)
{
    VALIDATE_NOT_NULL(tab);
    *tab = mSelectedTab;
    REFCOUNT_ADD(*tab)
    return NOERROR;
}

ECode CActionBarImpl::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    return mContainerView->GetHeight(height);
    return NOERROR;
}

ECode CActionBarImpl::Show()
{
    if (mHiddenByApp) {
        mHiddenByApp = FALSE;
        UpdateVisibility(FALSE);
    }
    return NOERROR;
}

void CActionBarImpl::ShowForActionMode()
{
    if (!mShowingForMode) {
        mShowingForMode = TRUE;
        if (mOverlayLayout != NULL) {
            mOverlayLayout->SetShowingForActionMode(TRUE);
        }
        UpdateVisibility(FALSE);
    }
}

ECode CActionBarImpl::ShowForSystem()
{
    if (mHiddenBySystem) {
        mHiddenBySystem = FALSE;
        UpdateVisibility(TRUE);
    }
    return NOERROR;
}

ECode CActionBarImpl::Hide()
{
    if (!mHiddenByApp) {
        mHiddenByApp = TRUE;
        UpdateVisibility(FALSE);
    }
    return NOERROR;
}

void CActionBarImpl::HideForActionMode()
{
    if (mShowingForMode) {
        mShowingForMode = FALSE;
        if (mOverlayLayout != NULL) {
            mOverlayLayout->SetShowingForActionMode(FALSE);
        }
        UpdateVisibility(FALSE);
    }
}

ECode CActionBarImpl::HideForSystem()
{
    if (!mHiddenBySystem) {
        mHiddenBySystem = TRUE;
        UpdateVisibility(TRUE);
    }
    return NOERROR;
}

Boolean CActionBarImpl::CheckShowingFlags(
    /* [in] */ Boolean hiddenByApp,
    /* [in] */ Boolean hiddenBySystem,
    /* [in] */ Boolean showingForMode)
{
    if (showingForMode) {
        return TRUE;
    } else if (hiddenByApp || hiddenBySystem) {
        return FALSE;
    } else {
        return TRUE;
    }
}

void CActionBarImpl::UpdateVisibility(
    /* [in] */ Boolean fromSystem)
{
    // Based on the current state, should we be hidden or shown?
    const Boolean shown = CheckShowingFlags(mHiddenByApp, mHiddenBySystem,
            mShowingForMode);

    if (shown) {
        if (!mNowShowing) {
            mNowShowing = TRUE;
            DoShow(fromSystem);
        }
    } else {
        if (mNowShowing) {
            mNowShowing = FALSE;
            DoHide(fromSystem);
        }
    }
}

ECode CActionBarImpl::DoShow(
    /* [in] */ Boolean fromSystem)
{
    if (mCurrentShowAnim != NULL) {
        mCurrentShowAnim->End();
    }
    mTopVisibilityView->SetVisibility(IView::VISIBLE);

    if (mCurWindowVisibility == IView::VISIBLE && (mShowHideAnimationEnabled
            || fromSystem)) {
        mTopVisibilityView->SetTranslationY(0); // because we're about to ask its window loc
        Float startingY;
        Float height;
        mTopVisibilityView->GetHeight((Int32*)&height);
        startingY = -height;
        if (fromSystem) {
            Int32 topLeftX = 0;
            Int32 topLeftY = 0;
            mTopVisibilityView->GetLocationInWindow(&topLeftX, &topLeftY);
            startingY -= topLeftY;
        }
        mTopVisibilityView->SetTranslationY(startingY);
        AutoPtr<IAnimatorSet> anim;
        CAnimatorSet::New((IAnimatorSet**)&anim);
        AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(1);
        (*array)[0] = 0.0f;
        AutoPtr<IAnimator> objAnim = CObjectAnimator::OfFloat(mTopVisibilityView, String("translationY"), array);
        AutoPtr<IAnimatorSetBuilder> b;
        anim->Play(objAnim, (IAnimatorSetBuilder**)&b);
        if (mContentView != NULL) {
            AutoPtr<ArrayOf<Float> > cViewArray = ArrayOf<Float>::Alloc(1);
            (*cViewArray)[0] = 0.0f;
            AutoPtr<IAnimator> cViewAnim = CObjectAnimator::OfFloat(mContentView, String("translationY"), cViewArray);
            b->With(cViewAnim);
        }
        if (mSplitView != NULL && mContextDisplayMode == CONTEXT_DISPLAY_SPLIT) {
            Int32 height;
            mSplitView->GetHeight(&height);
            mSplitView->SetTranslationY(height);
            mSplitView->SetVisibility(IView::VISIBLE);
            AutoPtr<ArrayOf<Float> > splitArray = ArrayOf<Float>::Alloc(1);
            (*splitArray)[0] = 0.0f;
            AutoPtr<IAnimator> splitAnim = CObjectAnimator::OfFloat(mSplitView, String("translationY"), splitArray);
            b->With(splitAnim);
        }
        AutoPtr<Elastos::Droid::View::Animation::IInterpolator> interpolator;
        AnimationUtils::LoadInterpolator(mContext, R::interpolator::decelerate_cubic, (Elastos::Droid::View::Animation::IInterpolator**)&interpolator);
        anim->SetInterpolator(interpolator);
        anim->SetDuration(250);
        // If this is being shown from the system, add a small delay.
        // This is because we will also be animating in the status bar,
        // and these two elements can't be done in lock-step.  So we give
        // a little time for the status bar to start its animation before
        // the action bar animates.  (This corresponds to the corresponding
        // case when hiding, where the status bar has a small delay before
        // starting.)
        anim->AddListener(mShowListener);
        mCurrentShowAnim = anim;
        anim->Start();
    } else {
        mTopVisibilityView->SetAlpha(1);
        mTopVisibilityView->SetTranslationY(0);
        if (mContentView != NULL) {
            mContentView->SetTranslationY(0);
        }
        if (mSplitView != NULL && mContextDisplayMode == CONTEXT_DISPLAY_SPLIT) {
            mSplitView->SetAlpha(1);
            mSplitView->SetTranslationY(0);
            mSplitView->SetVisibility(IView::VISIBLE);
        }
        mShowListener->OnAnimationEnd(NULL);
    }
    if (mOverlayLayout != NULL) {
        mOverlayLayout->RequestFitSystemWindows();
    }
    return NOERROR;
}

ECode CActionBarImpl::DoHide(
    /* [in] */ Boolean fromSystem)
{
    if (mCurrentShowAnim != NULL) {
        mCurrentShowAnim->End();
    }

    if (mCurWindowVisibility == IView::VISIBLE && (mShowHideAnimationEnabled
            || fromSystem)) {
        mTopVisibilityView->SetAlpha(1);
        mContainerView->SetTransitioning(TRUE);
        AutoPtr<IAnimatorSet> anim;
        CAnimatorSet::New((IAnimatorSet**)&anim);
        Float endingY;
        Int32 height;
        mTopVisibilityView->GetHeight(&height);
        endingY = -height;
        if (fromSystem) {
            Int32 topLeftX = 0;
            Int32 topLeftY = 0;
            mTopVisibilityView->GetLocationInWindow(&topLeftX, &topLeftY);
            endingY -= topLeftY;
        }
        AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(1);
        (*array)[0] = endingY;
        AutoPtr<IAnimator> objAnim = CObjectAnimator::OfFloat(mTopVisibilityView, String("translationY"), array);
        AutoPtr<IAnimatorSetBuilder> b;
        anim->Play(objAnim, (IAnimatorSetBuilder**)&b);
        if (mContentView != NULL) {
            AutoPtr<ArrayOf<Float> > cViewArray = ArrayOf<Float>::Alloc(1);
            (*cViewArray)[0] = endingY;
            AutoPtr<IAnimator> cViewAnim = CObjectAnimator::OfFloat(mContentView, String("translationY"), cViewArray);
            b->With(cViewAnim);
        }

        Int32 visibility;
        mSplitView->GetVisibility(&visibility);
        if (mSplitView != NULL && visibility == IView::VISIBLE) {
            mSplitView->SetAlpha(1);
            Int32 height;
            mSplitView->GetHeight(&height);
            AutoPtr<ArrayOf<Float> > splitArray = ArrayOf<Float>::Alloc(1);
            (*splitArray)[0] = height;
            AutoPtr<IAnimator> splitAnim = CObjectAnimator::OfFloat(mSplitView, String("translationY"), splitArray);
            b->With(splitAnim);
        }
        AutoPtr<Elastos::Droid::View::Animation::IInterpolator> interpolator;
        AnimationUtils::LoadInterpolator(mContext, R::interpolator::decelerate_cubic, (Elastos::Droid::View::Animation::IInterpolator**)&interpolator);
        anim->SetInterpolator(interpolator);
        anim->SetDuration(250);
        anim->AddListener(mHideListener);
        mCurrentShowAnim = anim;
        anim->Start();
    } else {
        mHideListener->OnAnimationEnd(NULL);
    }
    return NOERROR;
}

ECode CActionBarImpl::IsShowing(
    /* [out] */ Boolean* isShow)
{
    VALIDATE_NOT_NULL(isShow);
    *isShow = mNowShowing;
    return NOERROR;
}

ECode CActionBarImpl::IsSystemShowing(
    /* [out] */ Boolean* isShow)
{
    VALIDATE_NOT_NULL(isShow);
    *isShow = !mHiddenBySystem;
    return NOERROR;
}

ECode CActionBarImpl::AnimateToMode(
    /* [in] */ Boolean toActionMode)
{
    if (toActionMode) {
        ShowForActionMode();
    } else {
        HideForActionMode();
    }

    mActionView->AnimateToVisibility(toActionMode ? IView::GONE : IView::VISIBLE);
    mContextView->AnimateToVisibility(toActionMode ? IView::VISIBLE : IView::GONE);
    Boolean hasTabs;
    mActionView->HasEmbeddedTabs(&hasTabs);
    Boolean collapsed;
    mActionView->IsCollapsed(&collapsed);
    if (mTabScrollView != NULL && !hasTabs && collapsed) {
        mTabScrollView->AnimateToVisibility(toActionMode ? IView::GONE : IView::VISIBLE);
    }
    return NOERROR;
}

ECode CActionBarImpl::GetThemedContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    if (mThemedContext == NULL) {
        AutoPtr<ITypedValue> outValue;
        CTypedValue::New((ITypedValue**)&outValue);
        AutoPtr<IResourcesTheme> currentTheme;
        mContext->GetTheme((IResourcesTheme**)&currentTheme);
        Boolean isFounded;
        currentTheme->ResolveAttribute(R::attr::actionBarWidgetTheme,
                outValue, TRUE, &isFounded);
        Int32 targetThemeRes;
        outValue->GetResourceId(&targetThemeRes);

        Int32 resId;
        mContext->GetThemeResId(&resId);
        if (targetThemeRes != 0 && resId != targetThemeRes) {
            CContextThemeWrapper::New(mContext, targetThemeRes, (IContextThemeWrapper**)&mThemedContext);
        } else {
            mThemedContext = mContext;
        }
    }
    *context = mThemedContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode CActionBarImpl::IsTitleTruncated(
    /* [out] */ Boolean* isTruncated)
{
    VALIDATE_NOT_NULL(isTruncated);
    Boolean result;
    mActionView->IsTitleTruncated(&result);
    *isTruncated = mActionView != NULL && result;
    return NOERROR;
}

ECode CActionBarImpl::SetCustomView(
    /* [in] */ IView* view)
{
    return mActionView->SetCustomNavigationView(view);
}

ECode CActionBarImpl::SetCustomView(
    /* [in] */ IView* view,
    /* [in] */ IActionBarLayoutParams* layoutParams)
{
    VALIDATE_NOT_NULL(view);
    VALIDATE_NOT_NULL(layoutParams);

    view->SetLayoutParams(layoutParams);
    mActionView->SetCustomNavigationView(view);
    return NOERROR;
}

ECode CActionBarImpl::SetListNavigationCallbacks(
    /* [in] */ ISpinnerAdapter* adapter,
    /* [in] */ IActionBarOnNavigationListener* callback)
{
    VALIDATE_NOT_NULL(adapter);
    VALIDATE_NOT_NULL(callback);

    mActionView->SetDropdownAdapter(adapter);
    mActionView->SetCallback(callback);
    return NOERROR;
}

ECode CActionBarImpl::GetSelectedNavigationIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    Int32 mode;
    mActionView->GetNavigationMode(&mode);
    switch (mode) {
        case NAVIGATION_MODE_TABS:
            {
                Int32 pos;
                mSelectedTab->GetPosition(&pos);
                *index = mSelectedTab != NULL ? pos : -1;
                return NOERROR;
            }
        case NAVIGATION_MODE_LIST:
            return mActionView->GetDropdownSelectedPosition(index);
        default:
            *index = -1;
    }
    return NOERROR;
}

ECode CActionBarImpl::GetNavigationItemCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    Int32 mode;
    mActionView->GetNavigationMode(&mode);
    switch (mode) {
        case NAVIGATION_MODE_TABS:
            *count = mTabs.GetSize();
            return NOERROR;
        case NAVIGATION_MODE_LIST:
            {
                AutoPtr<ISpinnerAdapter> adapter;
                mActionView->GetDropdownAdapter((ISpinnerAdapter**)&adapter);
                Int32 ct;
                adapter->GetCount(&ct);
                *count = adapter != NULL ? ct : 0;
                return NOERROR;
            }
        default:
            *count = 0;
    }
    return NOERROR;
}

ECode CActionBarImpl::GetTabCount(
    /* [out] */ Int32* count)
{
    *count = mTabs.GetSize();
    return NOERROR;
}

ECode CActionBarImpl::SetNavigationMode(
    /* [in] */ Int32 mode)
{
    Int32 oldMode;
    mActionView->GetNavigationMode(&oldMode);
    switch (oldMode) {
        case NAVIGATION_MODE_TABS:
            GetSelectedNavigationIndex(&mSavedTabPosition);
            SelectTab(NULL);
            mTabScrollView->SetVisibility(IView::GONE);
            break;
    }
    if (oldMode != mode && !mHasEmbeddedTabs) {
        if (mOverlayLayout != NULL) {
            mOverlayLayout->RequestFitSystemWindows();
        }
    }
    mActionView->SetNavigationMode(mode);
    switch (mode) {
        case NAVIGATION_MODE_TABS:
            EnsureTabsExist();
            mTabScrollView->SetVisibility(IView::VISIBLE);
            if (mSavedTabPosition != INVALID_POSITION) {
                SetSelectedNavigationItem(mSavedTabPosition);
                mSavedTabPosition = INVALID_POSITION;
            }
            break;
    }
    mActionView->SetCollapsable(mode == NAVIGATION_MODE_TABS && !mHasEmbeddedTabs);
    return NOERROR;
}

ECode CActionBarImpl::GetTabAt(
    /* [in] */ Int32 index,
    /* [out] */ IActionBarTab** tab)
{
    VALIDATE_NOT_NULL(tab);
    *tab = mTabs[index];
    REFCOUNT_ADD(*tab)
    return NOERROR;
}


ECode CActionBarImpl::SetIcon(
    /* [in] */ Int32 resId)
{
    return mActionView->SetIcon(resId);
}

ECode CActionBarImpl::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return mActionView->SetIcon(icon);
}

ECode CActionBarImpl::SetLogo(
    /* [in] */ Int32 resId)
{
    return mActionView->SetLogo(resId);
}

ECode CActionBarImpl::SetLogo(
    /* [in] */ IDrawable* logo)
{
    return mActionView->SetLogo(logo);
}

ECode CActionBarImpl::SetDefaultDisplayHomeAsUpEnabled(
    /* [in] */ Boolean enable)
{
    if (!mDisplayHomeAsUpSet) {
        SetDisplayHomeAsUpEnabled(enable);
    }
    return NOERROR;
}

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos
