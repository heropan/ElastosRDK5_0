
#include "elastos/droid/internal/ActionBarView.h"
#include "elastos/droid/view/LayoutInflater.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"
#include "elastos/droid/view/Gravity.h"
#include "elastos/droid/widget/CProgressBar.h"
#include "elastos/droid/widget/CSpinner.h"
#include "elastos/droid/widget/CLinearLayout.h"
#include "elastos/droid/widget/CFrameLayoutLayoutParams.h"
#include <elastos/core/Math.h>
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/app/CActionBarLayoutParams.h"
#include "elastos/droid/view/menu/CActionMenuPresenter.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::Math;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IActionBarLayoutParams;
using Elastos::Droid::App::CActionBarLayoutParams;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Graphics::Drawable::IDrawableConstantState;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::ILayout;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::Menu::EIID_IMenuPresenter;
using Elastos::Droid::View::Menu::IActionMenuPresenter;
using Elastos::Droid::View::Menu::CActionMenuPresenter;
using Elastos::Droid::View::Gravity;
using Elastos::Droid::Widget::CProgressBar;
using Elastos::Droid::Widget::CSpinner;
using Elastos::Droid::Widget::CLinearLayout;
using Elastos::Droid::Widget::CFrameLayoutLayoutParams;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::ICollapsibleActionView;

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

ActionBarView::ActionBarView()
{}

ActionBarView::ActionBarView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mDisplayOptions(-1)
{
    Init(context, attrs);
}

ECode ActionBarView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroup::Init(context, attrs);

    SetBackgroundResource(0);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ActionBar),
            ARRAY_SIZE(R::styleable::ActionBar));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds,
        R::attr::actionBarStyle, 0, (ITypedArray**)&a);

    AutoPtr<IApplicationInfo> appInfo;
    context->GetApplicationInfo((IApplicationInfo**)&appInfo);

    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);

    a->GetInt32(R::styleable::ActionBar_navigationMode,
            IActionBar::NAVIGATION_MODE_STANDARD, &mNavigationMode);

    a->GetText(R::styleable::ActionBar_title, (ICharSequence**)&mTitle);
    a->GetText(R::styleable::ActionBar_subtitle, (ICharSequence**)&mSubtitle);

    a->GetDrawable(R::styleable::ActionBar_logo, (IDrawable**)&mLogo);
    if (mLogo == NULL) {
        if (IActivity::Probe(context)) {
            //try {
                //// is right?
            AutoPtr<IComponentName> name;
            (IActivity::Probe(context))->GetComponentName((IComponentName**)&name);
            pm->GetActivityLogo(name, (IDrawable**)&mIcon);
            //} catch (NameNotFoundException e) {
            //    Log.e(TAG, "Activity component name not found!", e);
            //}
        }
        if (mLogo == NULL) {
            appInfo->LoadLogo(pm, (IDrawable**)&mLogo);
        }
    }
    mIcon = NULL;
    a->GetDrawable(R::styleable::ActionBar_icon, (IDrawable**)&mIcon);
    if (mIcon == NULL) {
        if (IActivity::Probe(context)) {
            //try {
            AutoPtr<IComponentName> name;
            (IActivity::Probe(context))->GetComponentName((IComponentName**)&name);
            pm->GetActivityIcon(name, (IDrawable**)&mIcon);
           // } catch (NameNotFoundException e) {
           //     Log.e(TAG, "Activity component name not found!", e);
           // }
        }
        if (mIcon == NULL) {
            appInfo->LoadIcon(pm, (IDrawable**)&mIcon);
        }
    }

    AutoPtr<ILayoutInflater> inflater;
    LayoutInflater::From(context, (ILayoutInflater**)&inflater);

    Int32 homeResId = 0;
    a->GetResourceId(R::styleable::ActionBar_homeLayout, R::layout::action_bar_home, &homeResId);

    AutoPtr<IView> upView;
    inflater->Inflate(R::layout::action_bar_up_container, THIS_PROBE(IViewGroup), FALSE, (IView**)&upView);
    mUpGoerFive = IViewGroup::Probe(upView);
    AutoPtr<IView> homev;
    inflater->Inflate(homeResId, mUpGoerFive, FALSE, (IView**)&homev);
    mHomeLayout = IActionBarViewHomeView::Probe(homev);
    AutoPtr<IView> expv;
    inflater->Inflate(homeResId, mUpGoerFive, FALSE, (IView**)&expv);
    mExpandedHomeLayout = IActionBarViewHomeView::Probe(expv);
    mExpandedHomeLayout->SetUp(TRUE);
    mExpandedHomeLayout->SetOnClickListener(mExpandedActionViewUpListener);
    AutoPtr<ICharSequence> csq;
    GetResources()->GetText(R::string::action_bar_up_description, (ICharSequence**)&csq);
    mExpandedHomeLayout->SetContentDescription(csq);
    AutoPtr<IDrawable> upBackground;
    mUpGoerFive->GetBackground((IDrawable**)&upBackground);
    if (upBackground) {
        AutoPtr<IDrawableConstantState> upstate;
        upBackground->GetConstantState((IDrawableConstantState**)&upstate);
        AutoPtr<IDrawable> upd;
        upstate->NewDrawable((IDrawable**)&upd);
        mExpandedHomeLayout->SetBackground(upd);
    }
    mExpandedHomeLayout->SetEnabled(TRUE);
    mExpandedHomeLayout->SetFocusable(TRUE);

    a->GetResourceId(R::styleable::ActionBar_titleTextStyle, 0, &mTitleStyleRes);
    a->GetResourceId(R::styleable::ActionBar_subtitleTextStyle, 0, &mSubtitleStyleRes);
    a->GetResourceId(R::styleable::ActionBar_progressBarStyle, 0, &mProgressStyle);
    a->GetResourceId(R::styleable::ActionBar_indeterminateProgressStyle, 0, &mIndeterminateProgressStyle);

    a->GetDimensionPixelOffset(R::styleable::ActionBar_progressBarPadding, 0, &mProgressBarPadding);
    a->GetDimensionPixelOffset(R::styleable::ActionBar_itemPadding, 0, &mItemPadding);

    Int32 options = 0;
    a->GetInt32(R::styleable::ActionBar_displayOptions, DISPLAY_DEFAULT, &options);
    SetDisplayOptions(options);

    Int32 customNavId = 0;
    a->GetResourceId(R::styleable::ActionBar_customNavigationLayout, 0, &customNavId);
    if (customNavId != 0) {
        inflater->Inflate(customNavId, THIS_PROBE(IViewGroup), FALSE, (IView**)&mCustomNavView);
        mNavigationMode = IActionBar::NAVIGATION_MODE_STANDARD;
        SetDisplayOptions(mDisplayOptions | IActionBar::DISPLAY_SHOW_CUSTOM);
    }

    a->GetLayoutDimension(R::styleable::ActionBar_height, 0, &mContentHeight);

    a->Recycle();

    //mLogoNavItem = new ActionMenuItem(context, 0, R::id::home, 0, 0, mTitle);
    mHomeLayout->SetOnClickListener(mUpClickListener);
    mHomeLayout->SetClickable(TRUE);
    mHomeLayout->SetFocusable(TRUE);

    if (GetImportantForAccessibility() == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
        SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }

    return NOERROR;
}

void ActionBarView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AbsActionBarView::OnConfigurationChanged(newConfig);

    mTitleView = NULL;
    mSubtitleView = NULL;
    mTitleUpView = NULL;

    if (mTitleLayout) {
        AutoPtr<IViewParent> p;
        mTitleLayout->GetParent((IViewParent**)&p);
        if(p.Get() == IViewParent::Probe(mUpGoerFive)) {
            AutoPtr<IViewManager> vm = IViewManager::Probe(mUpGoerFive);
            vm->RemoveView(mTitleLayout);
        }
    }
    mTitleLayout = NULL;
    if ((mDisplayOptions & IActionBar::DISPLAY_SHOW_TITLE) != 0) {
        InitTitle();
    }

    if (mTabScrollView && mIncludeTabs) {
        AutoPtr<IViewGroupLayoutParams> lp;
        mTabScrollView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        if (lp != NULL) {
            lp->SetWidth(IViewGroupLayoutParams::WRAP_CONTENT);
            lp->SetHeight(IViewGroupLayoutParams::MATCH_PARENT);
        }
        mTabScrollView->SetAllowCollapse(TRUE);
    }
}

ECode ActionBarView::SetWindowCallback(
    /* [in] */ IWindowCallback* cb)
{
    mWindowCallback = cb;
    return NOERROR;
}

ECode ActionBarView::OnDetachedFromWindow()
{
    AbsActionBarView::OnDetachedFromWindow();
    RemoveCallbacks(mTabSelector);
    if (mActionMenuPresenter) {
        Boolean res = FALSE;
        mActionMenuPresenter->HideOverflowMenu(&res);
        mActionMenuPresenter->HideSubMenus(&res);
    }
    return NOERROR;
}

Boolean ActionBarView::ShouldDelayChildPressedState()
{
    return FALSE;
}

ECode ActionBarView::InitProgress()
{
    CProgressBar::New(mContext, NULL, 0, mProgressStyle, (IProgressBar**)&mProgressView);
    mProgressView->SetId(R::id::progress_horizontal);
    mProgressView->SetMax(10000);
    mProgressView->SetVisibility(IView::GONE);
    AddView(mProgressView);

    return NOERROR;
}

ECode ActionBarView::InitIndeterminateProgress()
{
    CProgressBar::New(mContext, NULL, 0, mIndeterminateProgressStyle, (IProgressBar**)&mIndeterminateProgressView);
    mIndeterminateProgressView->SetId(R::id::progress_circular);
    mIndeterminateProgressView->SetVisibility(IView::GONE);
    AddView(mIndeterminateProgressView);

    return NOERROR;
}

ECode ActionBarView::SetSplitActionBar(
    /* [in] */ Boolean splitActionBar)
{
    if (mSplitActionBar != splitActionBar) {
        if (mMenuView) {
            AutoPtr<IViewParent> p;
            mMenuView->GetParent((IViewParent**)&p);
            AutoPtr<IViewGroup> oldParent = IViewGroup::Probe(p);

            if (oldParent != NULL) {
                AutoPtr<IViewManager> mg = IViewManager::Probe(oldParent);
                mg->RemoveView(mMenuView);
            }
            AutoPtr<IViewGroupLayoutParams> lp;
            mMenuView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            if (splitActionBar) {
                if (mSplitView) {
                    mSplitView->AddView(mMenuView);
                }
                lp->SetWidth(IViewGroupLayoutParams::MATCH_PARENT);
            } else {
                AddView(mMenuView);
                lp->SetWidth(IViewGroupLayoutParams::WRAP_CONTENT);
            }
            mMenuView->RequestLayout();
        }
        if (mSplitView) {
            mSplitView->SetVisibility(splitActionBar ? IView::VISIBLE : IView::GONE);
        }

        if (mActionMenuPresenter) {
            if (!splitActionBar) {
                AutoPtr<IResources> res = GetResources();
                Boolean b = FALSE;
                res->GetBoolean(R::bool_::action_bar_expanded_action_views_exclusive, &b);
                mActionMenuPresenter->SetExpandedActionViewsExclusive(b);
            } else {
                mActionMenuPresenter->SetExpandedActionViewsExclusive(FALSE);
                AutoPtr<IResources> res;
                GetContext()->GetResources((IResources**)&res);
                AutoPtr<IDisplayMetrics> metrics;
                res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
                Int32 pixels = 0;
                metrics->GetWidthPixels(&pixels);
                mActionMenuPresenter->SetWidthLimit(pixels, TRUE);
                mActionMenuPresenter->SetItemLimit(Elastos::Core::Math::INT32_MAX_VALUE);
            }
        }
        AbsActionBarView::SetSplitActionBar(splitActionBar);
    }
    return NOERROR;
}

Boolean ActionBarView::IsSplitActionBar()
{
    return mSplitActionBar;
}

Boolean ActionBarView::HasEmbeddedTabs()
{
    return mIncludeTabs;
}

ECode ActionBarView::SetEmbeddedTabView(
    /* [in] */ IScrollingTabContainerView* tabs)
{
    if (mTabScrollView) {
        RemoveView(mTabScrollView);
    }
    mTabScrollView = tabs;
    mIncludeTabs = tabs != NULL;
    if (mIncludeTabs && mNavigationMode == IActionBar::NAVIGATION_MODE_TABS) {
        AddView(mTabScrollView);
        AutoPtr<IViewGroupLayoutParams> lp;
        mTabScrollView->GetLayoutParams((IViewGroupLayoutParams**)&lp);

        lp->SetWidth(IViewGroupLayoutParams::WRAP_CONTENT);
        lp->SetHeight(IViewGroupLayoutParams::MATCH_PARENT);
        tabs->SetAllowCollapse(TRUE);
    }
    return NOERROR;
}

ECode ActionBarView::SetCallback(
    /* [in] */ IActionBarOnNavigationListener* callback)
{
    mCallback = callback;
    return NOERROR;
}

ECode ActionBarView::SetMenu(
    /* [in] */ IMenu* menu,
    /* [in] */ IMenuPresenterCallback* cb)
{
    if (menu == mOptionsMenu) return NOERROR;

    if (mOptionsMenu) {
        mOptionsMenu->RemoveMenuPresenter(mActionMenuPresenter);
        mOptionsMenu->RemoveMenuPresenter(mExpandedMenuPresenter);
    }

    AutoPtr<IMenuBuilder> builder = IMenuBuilder::Probe(menu);
    mOptionsMenu = builder;
    if (mMenuView) {
        AutoPtr<IViewParent> p;
        mMenuView->GetParent((IViewParent**)&p);
        AutoPtr<IViewGroup> oldParent = IViewGroup::Probe(p);
        if (oldParent) {
            AutoPtr<IViewManager> mg = IViewManager::Probe(oldParent);
            mg->RemoveView(mMenuView);
        }
    }
    if (mActionMenuPresenter == NULL) {
        CActionMenuPresenter::New(mContext, (IActionMenuPresenter**)&mActionMenuPresenter);
        mActionMenuPresenter->SetCallback(cb);
        mActionMenuPresenter->SetId(R::id::action_menu_presenter);
        mExpandedMenuPresenter = new ExpandedActionViewMenuPresenter(this);
    }

    AutoPtr<IActionMenuView> menuView;
    AutoPtr<IViewGroupLayoutParams> layoutParams;
    CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
           IViewGroupLayoutParams::MATCH_PARENT, (IViewGroupLayoutParams**)&layoutParams);

    if (!mSplitActionBar) {
        AutoPtr<IResources> res = GetResources();
        Boolean b = FALSE;
        res->GetBoolean(R::bool_::action_bar_expanded_action_views_exclusive, &b);
        mActionMenuPresenter->SetExpandedActionViewsExclusive(b);
        ConfigPresenters(builder);
        AutoPtr<IMenuView> v;
        mActionMenuPresenter->GetMenuView(THIS_PROBE(IViewGroup), (IMenuView**)&v);
        menuView = IActionMenuView::Probe(v);
        AutoPtr<IViewParent> p;
        menuView->GetParent((IViewParent**)&p);
        AutoPtr<IViewGroup> oldParent = IViewGroup::Probe(p);
        if (oldParent && oldParent.Get() != THIS_PROBE(IViewGroup)) {
            AutoPtr<IViewManager> mg = IViewManager::Probe(oldParent);
            mg->RemoveView(menuView);
        }
        AddView(menuView, layoutParams);
    } else {
        mActionMenuPresenter->SetExpandedActionViewsExclusive(FALSE);
        // Allow full screen width in split mode.
        AutoPtr<IResources> res;
        GetContext()->GetResources((IResources**)&res);
        AutoPtr<IDisplayMetrics> metrics;
        res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
        Int32 pixels = 0;
        metrics->GetWidthPixels(&pixels);
        mActionMenuPresenter->SetWidthLimit(pixels, TRUE);
        mActionMenuPresenter->SetItemLimit(Elastos::Core::Math::INT32_MAX_VALUE);
        layoutParams->SetWidth(IViewGroupLayoutParams::MATCH_PARENT);
        ConfigPresenters(builder);
        AutoPtr<IMenuView> v;
        mActionMenuPresenter->GetMenuView(THIS_PROBE(IViewGroup), (IMenuView**)&v);
        menuView = IActionMenuView::Probe(v);
        if (mSplitView) {
            AutoPtr<IViewParent> p;
            menuView->GetParent((IViewParent**)&p);
            AutoPtr<IViewGroup> oldParent = IViewGroup::Probe(p);
            if (oldParent && oldParent != mSplitView) {
                AutoPtr<IViewManager> mg = IViewManager::Probe(oldParent);
                mg->RemoveView(menuView);
            }
            menuView->SetVisibility(GetAnimatedVisibility());
            mSplitView->AddView(menuView, layoutParams);
        } else {
            // We'll add this later if we missed it this time.
            menuView->SetLayoutParams(layoutParams);
        }
    }
    mMenuView = menuView;
    return NOERROR;
}

void ActionBarView::ConfigPresenters(
    /* [in] */ IMenuBuilder* builder)
{
    if (builder) {
        builder->AddMenuPresenter(mActionMenuPresenter);
        builder->AddMenuPresenter(mExpandedMenuPresenter);
    } else {
        mActionMenuPresenter->InitForMenu(mContext, NULL);
        mExpandedMenuPresenter->InitForMenu(mContext, NULL);
        mActionMenuPresenter->UpdateMenuView(TRUE);
        mExpandedMenuPresenter->UpdateMenuView(TRUE);
    }
}

Boolean ActionBarView::HasExpandedActionView()
{
    return mExpandedMenuPresenter != NULL &&
            mExpandedMenuPresenter->mCurrentExpandedItem != NULL;
}

ECode ActionBarView::CollapseActionView()
{
    AutoPtr<IMenuItemImpl> item = mExpandedMenuPresenter == NULL ? NULL :
            mExpandedMenuPresenter->mCurrentExpandedItem;
    if (item) {
        Boolean res = FALSE;
        item->CollapseActionView(&res);
    }

    return NOERROR;
}

ECode ActionBarView::SetCustomNavigationView(
    /* [in] */ IView* view)
{
    Boolean showCustom = (mDisplayOptions & IActionBar::DISPLAY_SHOW_CUSTOM) != 0;
    if (mCustomNavView && showCustom) {
        RemoveView(mCustomNavView);
    }
    mCustomNavView = view;
    if (mCustomNavView && showCustom) {
        AddView(mCustomNavView);
    }

    return NOERROR;
}

AutoPtr<ICharSequence> ActionBarView::GetTitle()
{
    return mTitle;
}


ECode ActionBarView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mUserTitle = TRUE;
    SetTitleImpl(title);
    return NOERROR;
}

ECode ActionBarView::SetWindowTitle(
    /* [in] */ ICharSequence* title)
{
    if (!mUserTitle) {
        SetTitleImpl(title);
    }
    return NOERROR;
}

void ActionBarView::SetTitleImpl(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    if (mTitleView) {
        mTitleView->SetText(title);
        Boolean visible = mExpandedActionView == NULL &&
                (mDisplayOptions & IActionBar::DISPLAY_SHOW_TITLE) != 0 &&
                (!TextUtils::IsEmpty(mTitle) || !TextUtils::IsEmpty(mSubtitle));
        mTitleLayout->SetVisibility(visible ? IView::VISIBLE : IView::GONE);
    }
    if (mLogoNavItem != NULL) {
        mLogoNavItem->SetTitle(title);
    }
}

AutoPtr<ICharSequence> ActionBarView::GetSubtitle()
{
    return mSubtitle;
}

ECode ActionBarView::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    mSubtitle = subtitle;
    if (mSubtitleView != NULL) {
        mSubtitleView->SetText(subtitle);
        mSubtitleView->SetVisibility(subtitle != NULL ? IView::VISIBLE : IView::GONE);
        Boolean visible = mExpandedActionView == NULL &&
                (mDisplayOptions & IActionBar::DISPLAY_SHOW_TITLE) != 0 &&
                (!TextUtils::IsEmpty(mTitle) || !TextUtils::IsEmpty(mSubtitle));
        mTitleLayout->SetVisibility(visible ? IView::VISIBLE : IView::GONE);
    }
    return NOERROR;
}

ECode ActionBarView::SetHomeButtonEnabled(Boolean enable)
{
    return SetHomeButtonEnabled(enable, TRUE);
}

ECode ActionBarView::SetHomeButtonEnabled(
    /* [in] */ Boolean enable,
    /* [in] */ Boolean recordState)
{

    if (recordState) {
        mWasHomeEnabled = enable;
    }

    if (mExpandedActionView) {
        return NOERROR;
    }

    mUpGoerFive->SetEnabled(enable);
    mUpGoerFive->SetFocusable(enable);
    if (!enable) {
        mUpGoerFive->SetContentDescription(NULL);
        mUpGoerFive->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_NO);
    } else {
        mUpGoerFive->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO);
        if ((mDisplayOptions & IActionBar::DISPLAY_HOME_AS_UP) != 0) {
            AutoPtr<IResources> res;
            mContext->GetResources((IResources**)&res);
            AutoPtr<ICharSequence> csq;
            res->GetText(R::string::action_bar_up_description, (ICharSequence**)&csq);
            mUpGoerFive->SetContentDescription(csq);
        } else {
            AutoPtr<IResources> res;
            mContext->GetResources((IResources**)&res);
            AutoPtr<ICharSequence> csq;
            res->GetText(R::string::action_bar_home_description, (ICharSequence**)&csq);
            mUpGoerFive->SetContentDescription(csq);
        }
    }

    return NOERROR;
}

ECode ActionBarView::SetDisplayOptions(
    /* [in] */ Int32 options)
{
    Int32 flagsChanged = mDisplayOptions == -1 ? -1 : options ^ mDisplayOptions;
    mDisplayOptions = options;

    if ((flagsChanged & DISPLAY_RELAYOUT_MASK) != 0) {
        Boolean showHome = (options & IActionBar::DISPLAY_SHOW_HOME) != 0;
        Int32 vis = (showHome && mExpandedActionView == NULL) ? IView::VISIBLE : IView::GONE;
        mHomeLayout->SetVisibility(vis);

        if ((flagsChanged & IActionBar::DISPLAY_HOME_AS_UP) != 0) {
            Boolean setUp = (options & IActionBar::DISPLAY_HOME_AS_UP) != 0;
            mHomeLayout->SetUp(setUp);

            if (setUp) {
                SetHomeButtonEnabled(TRUE);
            }
        }

        if ((flagsChanged & IActionBar::DISPLAY_USE_LOGO) != 0) {
            Boolean logoVis = mLogo != NULL && (options & IActionBar::DISPLAY_USE_LOGO) != 0;
            mHomeLayout->SetIcon(logoVis ? mLogo : mIcon);
        }

        if ((flagsChanged & IActionBar::DISPLAY_SHOW_TITLE) != 0) {
            if ((options & IActionBar::DISPLAY_SHOW_TITLE) != 0) {
                InitTitle();
            } else {
                AutoPtr<IViewManager> vm = IViewManager::Probe(mUpGoerFive);
                vm->RemoveView(mTitleLayout);
            }
        }

        if (mTitleLayout && (flagsChanged & (IActionBar::DISPLAY_HOME_AS_UP | IActionBar::DISPLAY_SHOW_HOME)) != 0) {
            Boolean homeAsUp = (mDisplayOptions & IActionBar::DISPLAY_HOME_AS_UP) != 0;
            mTitleUpView->SetVisibility(!showHome ? (homeAsUp ? IView::VISIBLE : IView::INVISIBLE) : IView::GONE);
        }

        if ((flagsChanged & IActionBar::DISPLAY_SHOW_CUSTOM) != 0 && mCustomNavView) {
            if ((options & IActionBar::DISPLAY_SHOW_CUSTOM) != 0) {
                AddView(mCustomNavView);
            } else {
                RemoveView(mCustomNavView);
            }
        }

        if (mTitleLayout && (flagsChanged & IActionBar::DISPLAY_TITLE_MULTIPLE_LINES) != 0) {
            if ((options & IActionBar::DISPLAY_TITLE_MULTIPLE_LINES) != 0) {
                assert(0 && "TEXTVIEW NEED ADD INTERFACE");
                //TitleView->SetSingleLine(FALSE);
                mTitleView->SetMaxLines(2);
            } else {
                mTitleView->SetMaxLines(1);
                //mTitleView->SetSingleLine(TRUE);
            }
        }

        RequestLayout();
    } else {
        Invalidate();
    }

    // Make sure the home button has an accurate content description for accessibility.

    Boolean enable;
    mHomeLayout->IsEnabled(&enable);
    if (!enable) {
        mHomeLayout->SetContentDescription(NULL);
        mHomeLayout->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_NO);
    } else {
        mHomeLayout->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO);
        if ((options & IActionBar::DISPLAY_HOME_AS_UP) != 0) {
            AutoPtr<IResources> res;
            mContext->GetResources((IResources**)&res);
            AutoPtr<ICharSequence> csq;
            res->GetText(R::string::action_bar_up_description, (ICharSequence**)&csq);
            mHomeLayout->SetContentDescription(csq);
        } else {
            AutoPtr<IResources> res;
            mContext->GetResources((IResources**)&res);
            AutoPtr<ICharSequence> csq;
            res->GetText(R::string::action_bar_home_description, (ICharSequence**)&csq);
            mHomeLayout->SetContentDescription(csq);
        }
    }

    return NOERROR;
}

ECode ActionBarView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIcon = icon;
    if (icon && ((mDisplayOptions & IActionBar::DISPLAY_USE_LOGO) == 0 || mLogo == NULL)) {
        mHomeLayout->SetIcon(icon);
    }
    if (mExpandedActionView) {
        AutoPtr<IDrawableConstantState> state;
        mIcon->GetConstantState((IDrawableConstantState**)&state);
        AutoPtr<IDrawable> drawable;
        state->NewDrawable(GetResources(), (IDrawable**)&drawable);
        mExpandedHomeLayout->SetIcon(drawable);
    }
    return NOERROR;
}

ECode ActionBarView::SetIcon(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoPtr<IDrawable> drawable;
    res->GetDrawable(resId, (IDrawable**)&drawable);
    SetIcon(drawable);
    return NOERROR;
}

ECode ActionBarView::SetLogo(
    /* [in] */ IDrawable* logo)
{
    mLogo = logo;
    if (logo && (mDisplayOptions & IActionBar::DISPLAY_USE_LOGO) != 0) {
        mHomeLayout->SetIcon(logo);
    }
    return NOERROR;
}

ECode ActionBarView::SetLogo(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);

    AutoPtr<IDrawable> drawable;
    res->GetDrawable(resId, (IDrawable**)&drawable);

    SetLogo(drawable);
    return NOERROR;
}

ECode ActionBarView::SetNavigationMode(
    /* [in] */ Int32 mode)
{
    Int32 oldMode = mNavigationMode;
    if (mode != oldMode) {
        switch (oldMode) {
        case IActionBar::NAVIGATION_MODE_LIST:
            if (mListNavLayout) {
                RemoveView(mListNavLayout);
            }
            break;
        case IActionBar::NAVIGATION_MODE_TABS:
            if (mTabScrollView && mIncludeTabs) {
                RemoveView(mTabScrollView);
            }
        }

        switch (mode) {
        case IActionBar::NAVIGATION_MODE_LIST:
        {
            if (!mSpinner) {
                CSpinner::New(mContext, NULL, R::attr::actionDropDownStyle, (ISpinner**)&mSpinner);
                CLinearLayout::New(mContext, NULL, R::attr::actionBarTabBarStyle, (ILinearLayout**)&mListNavLayout);
                AutoPtr<ILinearLayoutLayoutParams> params;
                CLinearLayoutLayoutParams::New(
                        IViewGroupLayoutParams::WRAP_CONTENT, IViewGroupLayoutParams::MATCH_PARENT, (ILinearLayoutLayoutParams**)&params);
                params->SetGravity(IGravity::CENTER);
                mListNavLayout->AddView(mSpinner, params);
            }

            AutoPtr<ISpinnerAdapter> spinnerAdapter;
            mSpinner->GetAdapter((IAdapter**)&spinnerAdapter);
            if (spinnerAdapter != mSpinnerAdapter) {
                mSpinner->SetAdapter(mSpinnerAdapter);
            }
            mSpinner->SetOnItemSelectedListener(mNavItemSelectedListener);
            AddView(mListNavLayout);
            break;
        }
        case IActionBar::NAVIGATION_MODE_TABS:
            if (mTabScrollView && mIncludeTabs) {
                AddView(mTabScrollView);
            }
            break;
        }
        mNavigationMode = mode;
        RequestLayout();
    }
    return NOERROR;
}

ECode ActionBarView::SetDropdownAdapter(
    /* [in] */ ISpinnerAdapter* adapter)
{
    mSpinnerAdapter = adapter;
    if (mSpinner) {
        mSpinner->SetAdapter(adapter);
    }
    return NOERROR;
}

AutoPtr<ISpinnerAdapter> ActionBarView::GetDropdownAdapter()
{
    return mSpinnerAdapter;
}

ECode ActionBarView::SetDropdownSelectedPosition(
    /* [in] */ Int32 position)
{
    return mSpinner->SetSelection(position);
}

Int32 ActionBarView::GetDropdownSelectedPosition()
{
    Int32 pos = 0;
    mSpinner->GetSelectedItemPosition(&pos);

    return pos;
}

AutoPtr<IView> ActionBarView::GetCustomNavigationView()
{
    return mCustomNavView;
}

Int32 ActionBarView::GetNavigationMode() {
    return mNavigationMode;
}

Int32 ActionBarView::GetDisplayOptions()
{
    return mDisplayOptions;
}

AutoPtr<IViewGroupLayoutParams> ActionBarView::GenerateDefaultLayoutParams()
{
    AutoPtr<IActionBarLayoutParams> ap;
    CActionBarLayoutParams::New(DEFAULT_CUSTOM_GRAVITY, (IActionBarLayoutParams**)&ap);
    return ap;
}

ECode ActionBarView::OnFinishInflate()
{
    AbsActionBarView::OnFinishInflate();
    mUpGoerFive->AddView(mHomeLayout, 0);
    AddView(mUpGoerFive);

    if (mCustomNavView && (mDisplayOptions & IActionBar::DISPLAY_SHOW_CUSTOM) != 0) {
        AutoPtr<IViewParent> parent;
        mCustomNavView->GetParent((IViewParent**)&parent);

        if (parent.Get() != THIS_PROBE(IViewParent)) {
            if (IViewGroup::Probe(parent)) {
                AutoPtr<IViewManager> vg = IViewManager::Probe(parent);
                vg->RemoveView(mCustomNavView);
            }
            AddView(mCustomNavView);
        }
    }
    return NOERROR;
}

void ActionBarView::InitTitle()
{
    if (!mTitleLayout) {
        AutoPtr<ILayoutInflater> inflater;
        LayoutInflater::From(GetContext(), (ILayoutInflater**)&inflater);
        AutoPtr<IView> v;
        inflater->Inflate(R::layout::action_bar_title_item,
                THIS_PROBE(IViewGroup), FALSE, (IView**)&v);
        mTitleLayout = ILinearLayout::Probe(v);
        AutoPtr<IView> title;
        mTitleLayout->FindViewById(R::id::action_bar_title, (IView**)&title);
        mTitleView = ITextView::Probe(title);
        AutoPtr<IView> subview;
        mTitleLayout->FindViewById(R::id::action_bar_subtitle, (IView**)&subview);
        mSubtitleView = ITextView::Probe(subview);
        mTitleLayout->FindViewById(R::id::up, (IView**)&mTitleUpView);

        if (mTitleStyleRes != 0) {
            mTitleView->SetTextAppearance(mContext, mTitleStyleRes);
        }
        if (mTitle) {
            mTitleView->SetText(mTitle);
        }

        if (mSubtitleStyleRes != 0) {
            mSubtitleView->SetTextAppearance(mContext, mSubtitleStyleRes);
        }
        if (mSubtitle) {
            mSubtitleView->SetText(mSubtitle);
            mSubtitleView->SetVisibility(IView::VISIBLE);
        }

        Boolean homeAsUp = (mDisplayOptions & IActionBar::DISPLAY_HOME_AS_UP) != 0;
        Boolean showHome = (mDisplayOptions & IActionBar::DISPLAY_SHOW_HOME) != 0;
        Boolean showTitleUp = !showHome;
        mTitleUpView->SetVisibility(showTitleUp ? (homeAsUp ? IView::VISIBLE : IView::INVISIBLE) : IView::GONE);
    }

    mUpGoerFive->AddView(mTitleLayout);
    if (mExpandedActionView ||
        (TextUtils::IsEmpty(mTitle) && TextUtils::IsEmpty(mSubtitle))) {
        mTitleLayout->SetVisibility(IView::GONE);
    }
}

ECode ActionBarView::SetContextView(
    /* [in] */ IActionBarContextView* view)
{
    mContextView = view;

    return NOERROR;
}

ECode ActionBarView::SetCollapsable(
    /* [in] */ Boolean collapsable)
{
    mIsCollapsable = collapsable;

    return NOERROR;
}

Boolean ActionBarView::IsCollapsed()
{
    return mIsCollapsed;
}

Boolean ActionBarView::IsTitleTruncated()
{
    if (!mTitleView) {
        return FALSE;
    }
    AutoPtr<ILayout> titleLayout;
    mTitleView->GetLayout((ILayout**)&titleLayout);
    if (!titleLayout) {
        return FALSE;
    }

    Int32 lineCount = 0;
    titleLayout->GetLineCount(&lineCount);
    for (int i = 0; i < lineCount; i++) {
        Int32 count = 0;
        titleLayout->GetEllipsisCount(i, &count);
        if (count > 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void ActionBarView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 childCount = GetChildCount();
    if (mIsCollapsable) {
        Int32 visibleChildren = 0;
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);

            Int32 visible = 0;
            child->GetVisibility(&visible);
            Int32 count = 0;
            mMenuView->GetChildCount(&count);
            if (visible != IView::GONE && !(child == mMenuView && count == 0) && child != mUpGoerFive) {
                    visibleChildren++;
                }
            }

        Int32 upChildCount = 0;
        mUpGoerFive->GetChildCount(&upChildCount);
            for (Int32 i = 0; i < upChildCount; i++) {
                AutoPtr<IView> child = GetChildAt(i);
                Int32 visible = 0;
                child->GetVisibility(&visible);
                if (visible != IView::GONE) {
                    visibleChildren++;
                }
            }

        if (visibleChildren == 0) {
            SetMeasuredDimension(0, 0);
            mIsCollapsed = TRUE;
            return;
        }
    }
    mIsCollapsed = FALSE;

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    if (widthMode != MeasureSpec::EXACTLY) {
        //throw new IllegalStateException(getClass().getSimpleName() + " can only be used " +
        //        "with android:layout_width=\"match_parent\" (or fill_parent)");
    }

    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    if (heightMode != MeasureSpec::AT_MOST) {
        //throw new IllegalStateException(getClass().getSimpleName() + " can only be used " +
        //       "with android:layout_height=\"wrap_content\"");
    }

    Int32 contentWidth = MeasureSpec::GetSize(widthMeasureSpec);

    Int32 maxHeight = mContentHeight >= 0 ? mContentHeight : MeasureSpec::GetSize(heightMeasureSpec);

    Int32 verticalPadding = GetPaddingTop() + GetPaddingBottom();
    Int32 paddingLeft = GetPaddingLeft();
    Int32 paddingRight = GetPaddingRight();
    Int32 height = maxHeight - verticalPadding;
    Int32 childSpecHeight = MeasureSpec::MakeMeasureSpec(height, MeasureSpec::AT_MOST);
    Int32 exactHeightSpec = MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY);

    Int32 availableWidth = contentWidth - paddingLeft - paddingRight;
    Int32 leftOfCenter = availableWidth / 2;
    Int32 rightOfCenter = leftOfCenter;

    AutoPtr<IActionBarViewHomeView> homeLayout = mExpandedActionView != NULL ? mExpandedHomeLayout : mHomeLayout;

    Int32 homeWidth = 0, homevisibility = 0;
    homeLayout->GetVisibility(&homevisibility);
    AutoPtr<IViewParent> homepar;
    homeLayout->GetParent((IViewParent**)&homepar);
    if (homevisibility != IView::GONE && homepar.Get() == IViewParent::Probe(mUpGoerFive)) {
        AutoPtr<IViewGroupLayoutParams> lp;
        homeLayout->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        Int32 homeWidthSpec = 0;
        Int32 lpWidth;
        lp->GetWidth(&lpWidth);

        if (lpWidth < 0) {
            homeWidthSpec = MeasureSpec::MakeMeasureSpec(availableWidth, MeasureSpec::AT_MOST);
        } else {
            homeWidthSpec = MeasureSpec::MakeMeasureSpec(lpWidth, MeasureSpec::EXACTLY);
        }

        homeLayout->Measure(homeWidthSpec, exactHeightSpec);
        homeLayout->GetMeasuredWidth(&homeWidth);
        Int32 homeof = 0;
        homeLayout->GetStartOffset(&homeof);
        Int32 homeOffsetWidth = homeWidth + homeof;
        availableWidth = Elastos::Core::Math::Max(0, availableWidth - homeOffsetWidth);
        leftOfCenter = Elastos::Core::Math::Max(0, availableWidth - homeOffsetWidth);
    }
    AutoPtr<IViewParent> mp;
    if (mMenuView && (mMenuView->GetParent((IViewParent**)&mp), mp.Get()) == THIS_PROBE(IViewParent)) {
        availableWidth = MeasureChildView(mMenuView, availableWidth, exactHeightSpec, 0);
        Int32 menuw = 0;
        mMenuView->GetMeasuredWidth(&menuw);
        rightOfCenter = Elastos::Core::Math::Max(0, rightOfCenter - menuw);
    }
    Int32 iv = 0;
    if (mIndeterminateProgressView && (mIndeterminateProgressView->GetVisibility(&iv), iv) != IView::GONE) {
        availableWidth = MeasureChildView(mIndeterminateProgressView, availableWidth, childSpecHeight, 0);
        Int32 ipw = 0;
        mIndeterminateProgressView->GetMeasuredWidth(&ipw);
        rightOfCenter = Elastos::Core::Math::Max(0, rightOfCenter - ipw);
    }
    Int32 tv = 0;
    Boolean showTitle = mTitleLayout != NULL && (mTitleLayout->GetVisibility(&tv), tv) != IView::GONE &&
            (mDisplayOptions & IActionBar::DISPLAY_SHOW_TITLE) != 0;

    if (!mExpandedActionView) {
        switch (mNavigationMode) {
            case IActionBar::NAVIGATION_MODE_LIST:
                if (mListNavLayout) {
                    Int32 itemPaddingSize = showTitle ? mItemPadding * 2 : mItemPadding;
                    availableWidth = Elastos::Core::Math::Max(0, availableWidth - itemPaddingSize);
                    leftOfCenter = Elastos::Core::Math::Max(0, leftOfCenter - itemPaddingSize);
                    mListNavLayout->Measure(
                            MeasureSpec::MakeMeasureSpec(availableWidth, MeasureSpec::AT_MOST),
                            MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY));
                    Int32 listNavWidth = 0;
                    mListNavLayout->GetMeasuredWidth(&listNavWidth);
                    availableWidth = Elastos::Core::Math::Max(0, availableWidth - listNavWidth);
                    leftOfCenter = Elastos::Core::Math::Max(0, leftOfCenter - listNavWidth);
                }
                break;
            case IActionBar::NAVIGATION_MODE_TABS:
                if (mTabScrollView) {
                    Int32 itemPaddingSize = showTitle ? mItemPadding * 2 : mItemPadding;
                    availableWidth = Elastos::Core::Math::Max(0, availableWidth - itemPaddingSize);
                    leftOfCenter = Elastos::Core::Math::Max(0, leftOfCenter - itemPaddingSize);
                    mTabScrollView->Measure(
                            MeasureSpec::MakeMeasureSpec(availableWidth, MeasureSpec::AT_MOST),
                            MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY));
                    Int32 tabWidth = 0;
                    mTabScrollView->GetMeasuredWidth(&tabWidth);
                    availableWidth = Elastos::Core::Math::Max(0, availableWidth - tabWidth);
                    leftOfCenter = Elastos::Core::Math::Max(0, leftOfCenter - tabWidth);
                }
                break;
        }
    }

    AutoPtr<IView> customView;
    if (mExpandedActionView) {
        customView = mExpandedActionView;
    } else if ((mDisplayOptions & IActionBar::DISPLAY_SHOW_CUSTOM) != 0 && mCustomNavView) {
        customView = mCustomNavView;
    }

    if (customView) {
        AutoPtr<IViewGroupLayoutParams> cv;
        customView->GetLayoutParams((IViewGroupLayoutParams**)&cv);
        AutoPtr<IViewGroupLayoutParams> lp = GenerateLayoutParams(cv);
        AutoPtr<IActionBarLayoutParams> ablp;
        if (IActionBarLayoutParams::Probe(lp)) {
            ablp = IActionBarLayoutParams::Probe(lp);
        } else {
            ablp = NULL;
        }

        Int32 horizontalMargin = 0;
        Int32 verticalMargin = 0;
        if (ablp) {
            Int32 ablm = 0, abrm = 0, abtm = 0, abbm = 0;
            ablp->GetLeftMargin(&ablm);
            ablp->GetRightMargin(&abrm);
            ablp->GetTopMargin(&abtm);
            ablp->GetBottomMargin(&abbm);

            horizontalMargin = ablm + abrm;
            verticalMargin = abtm + abbm;
        }

        Int32 customNavHeightMode;
        if (mContentHeight <= 0) {
            customNavHeightMode = MeasureSpec::AT_MOST;
        } else {
            Int32 height = 0;
            customNavHeightMode = (lp->GetHeight(&height), height) != IViewGroupLayoutParams::WRAP_CONTENT ?
                    MeasureSpec::EXACTLY : MeasureSpec::AT_MOST;
        }
        Int32 lh = 0, lw = 0, abg = 0;
        lp->GetHeight(&lh);
        lp->GetWidth(&lw);
        ablp->GetGravity(&abg);
        Int32 customNavHeight = Elastos::Core::Math::Max(0,
                (lh >= 0 ? Elastos::Core::Math::Min(lh, height) : height) - verticalMargin);

        Int32 customNavWidthMode = lw != IViewGroupLayoutParams::WRAP_CONTENT ?
                MeasureSpec::EXACTLY : MeasureSpec::AT_MOST;
        Int32 customNavWidth = Elastos::Core::Math::Max(0,
                (lw >= 0 ? Elastos::Core::Math::Min(lw, availableWidth) : availableWidth)
                - horizontalMargin);
        Int32 hgrav = (ablp ? abg : DEFAULT_CUSTOM_GRAVITY) & IGravity::HORIZONTAL_GRAVITY_MASK;

        if (hgrav == IGravity::CENTER_HORIZONTAL && lw == IViewGroupLayoutParams::MATCH_PARENT) {
            customNavWidth = Elastos::Core::Math::Min(leftOfCenter, rightOfCenter) * 2;
        }

        customView->Measure(
                MeasureSpec::MakeMeasureSpec(customNavWidth, customNavWidthMode),
                MeasureSpec::MakeMeasureSpec(customNavHeight, customNavHeightMode));
        Int32 cmw = 0;
        customView->GetMeasuredWidth(&cmw);
        availableWidth -= horizontalMargin + cmw;
    }

    availableWidth = MeasureChildView(mUpGoerFive, availableWidth + homeWidth,
            MeasureSpec::MakeMeasureSpec(mContentHeight, MeasureSpec::EXACTLY), 0);
    if (mTitleLayout) {
        Int32 tw = 0;
        mTitleLayout->GetMeasuredWidth(&tw);
        leftOfCenter = Elastos::Core::Math::Max(0, leftOfCenter - tw);
    }

    if (mContentHeight <= 0) {
        Int32 measuredHeight = 0;
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> v = GetChildAt(i);
            Int32 vh = 0;
            v->GetMeasuredHeight(&vh);
            Int32 paddedViewHeight = vh + verticalPadding;
            if (paddedViewHeight > measuredHeight) {
                measuredHeight = paddedViewHeight;
            }
        }
        SetMeasuredDimension(contentWidth, measuredHeight);
    } else {
        SetMeasuredDimension(contentWidth, maxHeight);
    }

    if (mContextView) {
        mContextView->SetContentHeight(GetMeasuredHeight());
    }
    Int32 pv = 0;
    if (mProgressView && (mProgressView->GetVisibility(&pv), pv) != IView::GONE) {
        mProgressView->Measure(MeasureSpec::MakeMeasureSpec(
                contentWidth - mProgressBarPadding * 2, MeasureSpec::EXACTLY),
                MeasureSpec::MakeMeasureSpec(GetMeasuredHeight(), MeasureSpec::AT_MOST));
    }
}

void ActionBarView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Int32 contentHeight = b - t - GetPaddingTop() - GetPaddingBottom();

    if (contentHeight <= 0) {
        return;
    }

    Boolean isLayoutRtl = IsLayoutRtl();
    Int32 direction = isLayoutRtl ? +1 : -1;
    Int32 menuStart = isLayoutRtl ? GetPaddingLeft() : r - l - GetPaddingRight();
    Int32 x = isLayoutRtl ? r - l - GetPaddingRight() : GetPaddingLeft();
    Int32 y = GetPaddingTop();

    AutoPtr<IActionBarViewHomeView> homeLayout = mExpandedActionView ? mExpandedHomeLayout : mHomeLayout;
    Int32 hv = 0;
    homeLayout->GetVisibility(&hv);
    AutoPtr<IViewParent> p;
    homeLayout->GetParent((IViewParent**)&p);
    Int32 hlos = 0;
    homeLayout->GetStartOffset(&hlos);
    Int32 startOffset = hv != IView::GONE && p.Get() == IViewParent::Probe(mUpGoerFive) ? hlos : 0;
    x += PositionChild(mUpGoerFive,
                Next(x, startOffset, isLayoutRtl), y, contentHeight, isLayoutRtl);
    x = Next(x, startOffset, isLayoutRtl);

    if (!mExpandedActionView) {
        Int32 visible = 0;
        Boolean showTitle = mTitleLayout && (mTitleLayout->GetVisibility(&visible), visible) != IView::GONE &&
                (mDisplayOptions & IActionBar::DISPLAY_SHOW_TITLE) != 0;

        switch (mNavigationMode) {
            case IActionBar::NAVIGATION_MODE_STANDARD:
                break;
            case IActionBar::NAVIGATION_MODE_LIST:
                if (mListNavLayout) {
                    if (showTitle) {
                        x = Next(x, mItemPadding, isLayoutRtl);
                    }
                    x += PositionChild(mListNavLayout, x, y, contentHeight, isLayoutRtl);
                    x = Next(x, mItemPadding, isLayoutRtl);
                }
                break;
            case IActionBar::NAVIGATION_MODE_TABS:
                if (mTabScrollView) {
                    if (showTitle) x = Next(x, mItemPadding, isLayoutRtl);
                    x += PositionChild(mTabScrollView, x, y, contentHeight, isLayoutRtl);
                    x = Next(x, mItemPadding, isLayoutRtl);
                }
                break;
        }
    }

    if (mMenuView) {
        AutoPtr<IViewParent> p;
        mMenuView->GetParent((IViewParent**)&p);
        if (p.Get() == THIS_PROBE(IViewParent)) {
            PositionChild(mMenuView, menuStart, y, contentHeight, !isLayoutRtl);
            Int32 w = 0;
            mMenuView->GetMeasuredWidth(&w);
            menuStart += direction * w;
        }
    }


    Int32 visibility = 0;
    if (mIndeterminateProgressView && (mIndeterminateProgressView->GetVisibility(&visibility), visibility) != IView::GONE) {
        PositionChild(mIndeterminateProgressView, menuStart, y, contentHeight, !isLayoutRtl);
        Int32 w = 0;
        mIndeterminateProgressView->GetMeasuredWidth(&w);
        menuStart += direction * w;
    }

    AutoPtr<IView> customView;
    if (mExpandedActionView) {
        customView = mExpandedActionView;
    } else if ((mDisplayOptions & IActionBar::DISPLAY_SHOW_CUSTOM) != 0 && mCustomNavView) {
        customView = mCustomNavView;
    }

    if (customView) {
        Int32 layoutDirection = GetLayoutDirection();
        AutoPtr<IViewGroupLayoutParams> lp;
        customView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        AutoPtr<IActionBarLayoutParams> ablp;
        Int32 gravity = DEFAULT_CUSTOM_GRAVITY;
        if (IActionBarLayoutParams::Probe(lp)) {
            ablp = IActionBarLayoutParams::Probe(lp);
            ablp->GetGravity(&gravity);
        } else {
            ablp = NULL;
        }
        Int32 navWidth = 0;
        customView->GetMeasuredWidth(&navWidth);

        Int32 topMargin = 0;
        Int32 bottomMargin = 0;
        if (ablp) {
            Int32 start = 0, end = 0;
            ablp->GetMarginStart(&start);
            x = Next(x, start, isLayoutRtl);
            ablp->GetMarginEnd(&end);
            menuStart += direction * end;

            ablp->GetTopMargin(&topMargin);
            ablp->GetBottomMargin(&bottomMargin);
        }

        Int32 hgravity = gravity & IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK;
        if (hgravity == IGravity::CENTER_HORIZONTAL) {
            Int32 centeredLeft = ((mRight - mLeft) - navWidth) / 2;
            if (isLayoutRtl) {
                Int32 centeredStart = centeredLeft + navWidth;
                Int32 centeredEnd = centeredLeft;
                if (centeredStart > x) {
                    hgravity = IGravity::RIGHT;
                } else if (centeredEnd < menuStart) {
                    hgravity = IGravity::LEFT;
                }
            } else {
                Int32 centeredStart = centeredLeft;
                Int32 centeredEnd = centeredLeft + navWidth;
                if (centeredStart < x) {
                    hgravity = IGravity::LEFT;
                } else if (centeredEnd > menuStart) {
                    hgravity = IGravity::RIGHT;
                }
            }
        } else if (gravity == IGravity::NO_GRAVITY) {
            hgravity = IGravity::START;
        }

        Int32 xpos = 0;
        switch (Gravity::GetAbsoluteGravity(hgravity, layoutDirection)) {
            case IGravity::CENTER_HORIZONTAL:
                xpos = ((mRight - mLeft) - navWidth) / 2;
                break;
            case IGravity::LEFT:
                xpos = isLayoutRtl ? menuStart : x;
                break;
            case IGravity::RIGHT:
                xpos = isLayoutRtl ? x - navWidth : menuStart - navWidth;
                break;
        }

        Int32 vgravity = gravity & IGravity::VERTICAL_GRAVITY_MASK;

        if (gravity == IGravity::NO_GRAVITY) {
            vgravity = IGravity::CENTER_VERTICAL;
        }

        Int32 ypos = 0;
        switch (vgravity) {
            case IGravity::CENTER_VERTICAL:
            {
                Int32 paddedTop = GetPaddingTop();
                Int32 paddedBottom = mBottom - mTop - GetPaddingBottom();
                Int32 h = 0;
                customView->GetMeasuredHeight(&h);
                ypos = ((paddedBottom - paddedTop) - h) / 2;
                break;
            }
            case IGravity::TOP:
                ypos = GetPaddingTop() + topMargin;
                break;
            case IGravity::BOTTOM:
            {
                Int32 h = 0;
                customView->GetMeasuredHeight(&h);
                ypos = GetHeight() - GetPaddingBottom() - h - bottomMargin;
                break;
            }
        }
        Int32 customWidth = 0;
        customView->GetMeasuredWidth(&customWidth);
        Int32 h = 0;
        customView->GetMeasuredHeight(&h);
        customView->Layout(xpos, ypos, xpos + customWidth, ypos + h);
        x = Next(x, customWidth, isLayoutRtl);
    }

    if (mProgressView) {
        mProgressView->BringToFront();
        Int32 ph = 0;
        mProgressView->GetMeasuredHeight(&ph);
        Int32 halfProgressHeight = ph / 2;
        Int32 pw = 0;
        mProgressView->GetMeasuredWidth(&pw);
        mProgressView->Layout(mProgressBarPadding, -halfProgressHeight, mProgressBarPadding + pw, halfProgressHeight);
    }

}

ECode ActionBarView::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IActionBarLayoutParams> lp;
    FAIL_RETURN(CActionBarLayoutParams::New(GetContext(), attrs, (IActionBarLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> ActionBarView::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* lp)
{
    if (lp == NULL) {
        lp = GenerateDefaultLayoutParams();
    }
    return lp;
}

AutoPtr<IParcelable> ActionBarView::OnSaveInstanceState()
{
    /*AutoPtr<IParcelable> superState = AbsActionBarView::OnSaveInstanceState();
    SavedState* state = new SavedState(superState);

    if (mExpandedMenuPresenter != NULL && mExpandedMenuPresenter->mCurrentExpandedItem != NULL) {
        state->expandedMenuItemId = mExpandedMenuPresenter->mCurrentExpandedItem->GetItemId();
    }

    state->isOverflowOpen = IsOverflowMenuShowing();

    return state;*/
    return NULL;
}

void ActionBarView::OnRestoreInstanceState(
    /* [in] */ IParcelable* p)
{
    /*SavedState state = (SavedState) p;

    AbsActionBarView::onRestoreInstanceState(state.getSuperState());

    if (state.expandedMenuItemId != 0 &&
            mExpandedMenuPresenter != NULL && mOptionsMenu != NULL) {
        MenuItem item = mOptionsMenu.findItem(state.expandedMenuItemId);
        if (item != NULL) {
            item.expandActionView();
        }
    }

    if (state.isOverflowOpen) {
        PostShowOverflowMenu();
    }*/
}

/////////////////////////////////////////////////////////////
//             ActionBarView::NavItemSelectedListener
/////////////////////////////////////////////////////////////
CAR_INTERFACE_IMPL(ActionBarView::NavItemSelectedListener, IAdapterViewOnItemSelectedListener)

ActionBarView::NavItemSelectedListener::NavItemSelectedListener(
    /* [in] */ ActionBarView* host)
    : mHost(host)
{}

ECode ActionBarView::NavItemSelectedListener::OnItemSelected(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (mHost->mCallback) {
        Boolean res = FALSE;
        mHost->mCallback->OnNavigationItemSelected(position, id, &res);
    }
    return NOERROR;
}

ECode ActionBarView::NavItemSelectedListener::OnNothingSelected(
    /* [in] */ IAdapterView* parent)
{
    return NOERROR;
}


/////////////////////////////////////////////////////////////
//        ActionBarView::ExpandedActionViewUpListener
/////////////////////////////////////////////////////////////
CAR_INTERFACE_IMPL(ActionBarView::ExpandedActionViewUpListener, IViewOnClickListener)

ActionBarView::ExpandedActionViewUpListener::ExpandedActionViewUpListener(
    /* [in] */ ActionBarView* host)
    : mHost(host)
{}

ECode ActionBarView::ExpandedActionViewUpListener::OnClick(
    /* [in] */ IView* v)
{
    AutoPtr<IMenuItemImpl> item = mHost->mExpandedMenuPresenter->mCurrentExpandedItem;
    if (item) {
        Boolean res = FALSE;
        item->CollapseActionView(&res);
    }
    return NOERROR;
}

/////////////////////////////////////////////////////////////
//             ActionBarView::UpClickListener
/////////////////////////////////////////////////////////////
CAR_INTERFACE_IMPL(ActionBarView::UpClickListener, IViewOnClickListener)

ActionBarView::UpClickListener::UpClickListener(
    /* [in] */ ActionBarView* host)
    : mHost(host)
{}

ECode ActionBarView::UpClickListener::OnClick(
    /* [in] */ IView* v)
{
    Boolean res = FALSE;
    mHost->mWindowCallback->OnMenuItemSelected(IWindow::FEATURE_OPTIONS_PANEL, mHost->mLogoNavItem, &res);
    return NOERROR;
}

/////////////////////////////////////////////////////////////
//        ActionBarView::ExpandedActionViewMenuPresenter
/////////////////////////////////////////////////////////////
CAR_INTERFACE_IMPL_2(ActionBarView::ExpandedActionViewMenuPresenter, IMenuPresenter, IWeakReferenceSource)

ECode ActionBarView::ExpandedActionViewMenuPresenter::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ActionBarView::ExpandedActionViewMenuPresenter::ExpandedActionViewMenuPresenter(
    /* [in] */ ActionBarView* host)
    :mHost(host)
{}

ECode ActionBarView::ExpandedActionViewMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    if (mMenu && mCurrentExpandedItem) {
        Boolean res = FALSE;
        mMenu->CollapseItemActionView(mCurrentExpandedItem, &res);
    }
    mMenu = menu;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    if (mCurrentExpandedItem) {
        Boolean found = FALSE;

        if (mMenu) {
            Int32 count = 0;
            mMenu->GetSize(&count);
            for (Int32 i = 0; i < count; i++) {
                AutoPtr<IMenuItem> item;
                mMenu->GetItem(i, (IMenuItem**)&item);
                if (item == mCurrentExpandedItem) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            Boolean res = FALSE;
            CollapseItemActionView(mMenu, mCurrentExpandedItem, &res);
        }
    }
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* handled)
{
    VALIDATE_NOT_NULL(handled);
    *handled = FALSE;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::FlagActionItems(
    /* [out] */ Boolean* shown)
{
    VALIDATE_NOT_NULL(shown);
    *shown = FALSE;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* expanded)
{
    VALIDATE_NOT_NULL(expanded);
    item->GetActionView((IView**)&(mHost->mExpandedActionView));
    AutoPtr<IDrawableConstantState> state;
    mHost->mIcon->GetConstantState((IDrawableConstantState**)&state);
    AutoPtr<IDrawable> d;
    state->NewDrawable(mHost->GetResources(), (IDrawable**)&d);
    mHost->mExpandedHomeLayout->SetIcon(d);
    mCurrentExpandedItem = item;
    AutoPtr<IViewParent> eavParent;
    mHost->mExpandedActionView->GetParent((IViewParent**)&eavParent);
    if (eavParent.Get() != (IViewParent*)mHost->Probe(EIID_IViewParent)) {
        mHost->AddView(mHost->mExpandedActionView);
    }
    AutoPtr<IViewParent> ehlParent;
    mHost->mExpandedHomeLayout->GetParent((IViewParent**)&ehlParent);
    if (ehlParent.Get() != (IViewParent*)mHost->Probe(EIID_IViewParent)) {
        mHost->AddView(mHost->mExpandedHomeLayout);
    }
    mHost->mHomeLayout->SetVisibility(IView::GONE);
    if (mHost->mTitleLayout) mHost->mTitleLayout->SetVisibility(IView::GONE);
    if (mHost->mTabScrollView) mHost->mTabScrollView->SetVisibility(IView::GONE);
    if (mHost->mSpinner) mHost->mSpinner->SetVisibility(IView::GONE);
    if (mHost->mCustomNavView) mHost->mCustomNavView->SetVisibility(IView::GONE);
    mHost->RequestLayout();
    item->SetActionViewExpanded(TRUE);

    assert(0 && "TODO");
    /*if (mExpandedActionView instanceof CollapsibleActionView) {
        ((CollapsibleActionView) mExpandedActionView).onActionViewExpanded();
    }*/
    *expanded = TRUE;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* collapsed)
{
    VALIDATE_NOT_NULL(collapsed);
    assert(0 && "TODO");
    /*if (mExpandedActionView instanceof CollapsibleActionView) {
        ((CollapsibleActionView) mExpandedActionView).onActionViewCollapsed();
    }*/

    mHost->RemoveView(mHost->mExpandedActionView);
    mHost->RemoveView(mHost->mExpandedHomeLayout);
    mHost->mExpandedActionView = NULL;
    if ((mHost->mDisplayOptions & IActionBar::DISPLAY_SHOW_HOME) != 0) {
        mHost->mHomeLayout->SetVisibility(IView::VISIBLE);
    }
    if ((mHost->mDisplayOptions & IActionBar::DISPLAY_SHOW_TITLE) != 0) {
        if (!mHost->mTitleLayout) {
            mHost->InitTitle();
        } else {
            mHost->mTitleLayout->SetVisibility(IView::VISIBLE);
        }
    }
    if (mHost->mTabScrollView && mHost->mNavigationMode == IActionBar::NAVIGATION_MODE_TABS) {
        mHost->mTabScrollView->SetVisibility(IView::VISIBLE);
    }
    if (mHost->mSpinner && mHost->mNavigationMode == IActionBar::NAVIGATION_MODE_LIST) {
        mHost->mSpinner->SetVisibility(IView::VISIBLE);
    }
    if (mHost->mCustomNavView && (mHost->mDisplayOptions & IActionBar::DISPLAY_SHOW_CUSTOM) != 0) {
        mHost->mCustomNavView->SetVisibility(IView::VISIBLE);
    }
    mHost->mExpandedHomeLayout->SetIcon(NULL);
    mCurrentExpandedItem = NULL;
    mHost->RequestLayout();
    item->SetActionViewExpanded(FALSE);
    *collapsed = TRUE;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = 0;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::OnSaveInstanceState(
    /* [out] */ IParcelable** pa)
{
    VALIDATE_NOT_NULL(pa);
    *pa = NULL;
    return NOERROR;
}

ECode ActionBarView::ExpandedActionViewMenuPresenter::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos


