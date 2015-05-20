#ifndef __CACTIONBARIMPL_H__
#define __CACTIONBARIMPL_H__

#include "_CActionBarImpl.h"
#include "ext/frameworkext.h"
#include "view/ActionMode.h"
#include "view/menu/CActionBarPolicy.h"
#include "animation/AnimatorListenerAdapter.h"
#include "animation/AnimatorListenerAdapter.h"
#include <elastos/List.h>

using namespace Elastos;
using namespace Elastos::Core;

using Elastos::Utility::List;
using Elastos::Core::IRunnable;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::View::ActionMode;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::EIID_IActionMode;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::Menu::IMenuBuilder;
using Elastos::Droid::View::Menu::ISubMenuBuilder;
using Elastos::Droid::View::Menu::IMenuBuilderCallback;
using Elastos::Droid::View::Menu::CActionBarPolicy;
using Elastos::Droid::View::Menu::IActionBarPolicy;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Widget::ISpinnerAdapter;
using Elastos::Droid::Widget::Internal::IActionBarView;
using Elastos::Droid::Widget::Internal::IActionBarContainer;
using Elastos::Droid::Widget::Internal::IActionBarContextView;
using Elastos::Droid::Widget::Internal::IActionBarOverlayLayout;
using Elastos::Droid::Widget::Internal::IScrollingTabContainerView;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IActionBarTab;
using Elastos::Droid::App::IActionBarTabListener;
using Elastos::Droid::App::IActionBarLayoutParams;
using Elastos::Droid::App::IActionBarOnNavigationListener;
using Elastos::Droid::App::IActionBarOnMenuVisibilityListener;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Animation::IAnimatorListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

CarClass(CActionBarImpl)
{
public:
    class ActionModeImpl
        : public ElRefBase
        , public ActionMode
        , public IMenuBuilderCallback
    {
    public:
        CAR_INTERFACE_DECL()

        ActionModeImpl(
            /* [in] */ IActionModeCallback* callback,
            /* [in] */ CActionBarImpl* actionBarImpl);

        ~ActionModeImpl();

        CARAPI GetMenuInflater(
            /* [out] */ IMenuInflater** inflater);

        CARAPI GetMenu(
            /* [out] */ IMenu** menu);

        CARAPI Finish();

        CARAPI Invalidate();

        CARAPI DispatchOnCreate(
            /* [out] */ Boolean* result);

        CARAPI SetCustomView(
            /* [in] */ IView* view);

        CARAPI SetSubtitle(
            /* [in] */ ICharSequence* subtitle);

        CARAPI SetTitle(
            /* [in] */ ICharSequence* title);

        CARAPI SetTitleEx(
            /* [in] */ Int32 resId);

        CARAPI SetSubtitleEx(
            /* [in] */ Int32 resId);

        CARAPI GetTitle(
            /* [out] */ ICharSequence** title);

        CARAPI GetSubtitle(
            /* [out] */ ICharSequence** title);

        CARAPI SetTitleOptionalHint(
            /* [in] */ Boolean titleOptional);

        CARAPI IsTitleOptional(
            /* [out] */ Boolean* result);

        CARAPI GetCustomView(
            /* [out] */ IView** view);

        CARAPI OnMenuItemSelected(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* result);

        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        CARAPI OnSubMenuSelected(
            /* [in] */ ISubMenuBuilder* subMenu,
            /* [out] */ Boolean* result);

        CARAPI OnCloseSubMenu(
            /* [in] */ ISubMenuBuilder* menu);

        CARAPI OnMenuModeChange(
            /* [in] */ IMenuBuilder* menu);

    private:
        AutoPtr<IActionModeCallback> mCallback;
        AutoPtr<IMenuBuilder> mMenu;
        AutoPtr<IView> mCustomView;
        CActionBarImpl* mActionBarImpl;
    };

    class TabImpl
        : public IActionBarTab
        , public ElRefBase
        , public IObject
    {
    public:
        CAR_INTERFACE_DECL()

        TabImpl(
            /* [in] */ CActionBarImpl* actionBarImpl);

        CARAPI Aggregate(
            /* [in] */ AggrType aggrType,
            /* [in] */ PInterface pObject);

        CARAPI GetDomain(
            /* [out] */ PInterface *ppObject);

        CARAPI GetClassID(
            /* [out] */ ClassID *pCLSID);

        CARAPI Equals(
            /* [in] */ IInterface* other,
            /* [out] */ Boolean * result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hash);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI GetTag(
            /* [out] */ IInterface** obj);

        CARAPI SetTag(
            /* [in] */ IInterface* tag);

        CARAPI GetCallback(
            /* [out] */ IActionBarTabListener** cb);

        CARAPI SetTabListener(
            /* [in] */ IActionBarTabListener* listener);

        CARAPI GetCustomView(
            /* [out] */ IView** view);

        CARAPI SetCustomView(
            /* [in] */ IView* view);

        CARAPI SetCustomViewEx(
            /* [in] */ Int32 layoutResId);

        CARAPI GetIcon(
            /* [out] */ IDrawable** icon);

        CARAPI GetPosition(
            /* [out] */ Int32* position);

        CARAPI SetPosition(
            /* [in] */ Int32 position);

        CARAPI GetText(
            /* [out] */ ICharSequence** text);

        CARAPI SetIcon(
            /* [in] */ IDrawable* icon);

        CARAPI SetIconEx(
            /* [in] */ Int32 resId);

        CARAPI SetText(
            /* [in] */ ICharSequence* text);

        CARAPI SetTextEx(
            /* [in] */ Int32 resId);

        CARAPI Select();

        CARAPI SetContentDescription(
            /* [in] */ Int32 resId);

        CARAPI SetContentDescriptionEx(
            /* [in] */ ICharSequence* contentDesc);

        CARAPI GetContentDescription(
            /* [out] */ ICharSequence** description);

    private:
        AutoPtr<IActionBarTabListener> mCallback;
        AutoPtr<IInterface> mTag;
        AutoPtr<IDrawable> mIcon;
        AutoPtr<ICharSequence> mText;
        AutoPtr<ICharSequence> mContentDesc;
        Int32 mPosition;
        AutoPtr<IView> mCustomView;
        CActionBarImpl* mActionBarImpl;
    };

private:
    class HideAnimatorListenerAdapter
        : public AnimatorListenerAdapter
    {
    public:
        HideAnimatorListenerAdapter(
            /* [in] */ CActionBarImpl* host)
            : mHost(host)
        {}

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        CActionBarImpl* mHost;
    };

    class ShowAnimatorListenerAdapter
        : public AnimatorListenerAdapter
    {
    public:
        ShowAnimatorListenerAdapter(
            /* [in] */ CActionBarImpl* host)
            : mHost(host)
        {}

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        CActionBarImpl* mHost;
    };

public:
    CActionBarImpl();

    CARAPI constructor(
        /* [in] */ IActivity* activity);

    CARAPI constructor(
        /* [in] */ IDialog* dialog);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI HasNonEmbeddedTabs(
        /* [out] */ Boolean* result);

    CARAPI SetWindowVisibility(
        /* [in] */ Int32 visibility);

    CARAPI SetShowHideAnimationEnabled(
        /* [in] */ Boolean enabled);

    CARAPI AddOnMenuVisibilityListener(
        /* [in] */ IActionBarOnMenuVisibilityListener* listener);

    CARAPI RemoveOnMenuVisibilityListener(
        /* [in] */ IActionBarOnMenuVisibilityListener* listener);

    CARAPI DispatchMenuVisibilityChanged(
        /* [in] */ Boolean isVisible);

    CARAPI SetCustomViewEx2(
        /* [in] */ Int32 resId);

    CARAPI SetDisplayUseLogoEnabled(
        /* [in] */ Boolean useLogo);

    CARAPI SetDisplayShowHomeEnabled(
        /* [in] */ Boolean showHome);

    CARAPI SetDisplayHomeAsUpEnabled(
        /* [in] */ Boolean showHomeAsUp);

    CARAPI SetDisplayShowTitleEnabled(
        /* [in] */ Boolean showTitle);

    CARAPI SetDisplayShowCustomEnabled(
        /* [in] */ Boolean showCustom);

    CARAPI SetHomeButtonEnabled(
        /* [in] */ Boolean enable);

    CARAPI SetTitleEx(
        /* [in] */ Int32 resId);

    CARAPI SetSubtitleEx(
        /* [in] */ Int32 resId);

    CARAPI SetSelectedNavigationItem(
        /* [in] */ Int32 position);

    CARAPI RemoveAllTabs();

    CARAPI CleanupTabs();

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetSubtitle(
        /* [in] */ ICharSequence* subtitle);

    CARAPI SetDisplayOptions(
        /* [in] */ Int32 options);

    CARAPI SetDisplayOptionsEx(
        /* [in] */ Int32 options,
        /* [in] */ Int32 mask);

    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetStackedBackgroundDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetSplitBackgroundDrawable(
        /* [in] */ IDrawable* d);

    CARAPI GetCustomView(
        /* [out] */ IView** view);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetSubtitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetNavigationMode(
        /* [out] */ Int32* mode);

    CARAPI GetDisplayOptions(
        /* [out] */ Int32* opt);

    CARAPI StartActionMode(
        /* [in] */ IActionModeCallback* callback,
        /* [out] */ IActionMode** actionmode);

    CARAPI AddTab(
        /* [in] */ IActionBarTab* tab);

    CARAPI AddTabEx2(
        /* [in] */ IActionBarTab* tab,
        /* [in] */ Int32 position);

    CARAPI AddTabEx(
        /* [in] */ IActionBarTab* tab,
        /* [in] */ Boolean setSelected);

    CARAPI AddTabEx3(
        /* [in] */ IActionBarTab* tab,
        /* [in] */ Int32 position,
        /* [in] */ Boolean setSelected);

    CARAPI NewTab(
        /* [out] */ IActionBarTab** tab);

    CARAPI RemoveTab(
        /* [in] */ IActionBarTab* tab);

    CARAPI RemoveTabAt(
        /* [in] */ Int32 position);

    CARAPI SelectTab(
        /* [in] */ IActionBarTab* tab);

    CARAPI GetSelectedTab(
        /* [out] */ IActionBarTab** tab);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI Show();

    CARAPI ShowForSystem();

    CARAPI Hide();

    CARAPI HideForSystem();

    CARAPI DoShow(
        /* [in] */ Boolean fromSystem);

    CARAPI DoHide(
        /* [in] */ Boolean fromSystem);

    CARAPI IsShowing(
        /* [out] */ Boolean* isShow);

    CARAPI IsSystemShowing(
        /* [out] */ Boolean* isShow);

    CARAPI GetThemedContext(
        /* [out] */ IContext** context);

    CARAPI IsTitleTruncated(
        /* [out] */ Boolean* isTruncated);

    CARAPI SetCustomView(
        /* [in] */ IView* view);

    CARAPI SetCustomViewEx(
        /* [in] */ IView* view,
        /* [in] */ IActionBarLayoutParams* layoutParams);

    CARAPI SetListNavigationCallbacks(
        /* [in] */ ISpinnerAdapter* adapter,
        /* [in] */ IActionBarOnNavigationListener* callback);

    CARAPI GetSelectedNavigationIndex(
        /* [out] */ Int32* index);

    CARAPI GetNavigationItemCount(
        /* [out] */ Int32* count);

    CARAPI GetTabCount(
        /* [out] */ Int32* count);

    CARAPI SetNavigationMode(
        /* [in] */ Int32 mode);

    CARAPI GetTabAt(
        /* [in] */ Int32 index,
        /* [out] */ IActionBarTab** tab);

    CARAPI SetIcon(
        /* [in] */ Int32 resId);

    CARAPI SetIconEx(
        /* [in] */ IDrawable* icon);

    CARAPI SetLogo(
        /* [in] */ Int32 resId);

    CARAPI SetLogoEx(
        /* [in] */ IDrawable* logo);


    CARAPI SetDefaultDisplayHomeAsUpEnabled(
        /* [in] */ Boolean enable);

public:
    CARAPI CompleteDeferredDestroyActionMode();

    CARAPI AnimateToMode(
        /* [in] */ Boolean toActionMode);

private:
    CARAPI Init(
        /* [in] */ IView* decor);

    CARAPI_(void) SetHasEmbeddedTabs(
        /* [in] */ Boolean hasEmbeddedTabs);

    CARAPI_(void) EnsureTabsExist();

    CARAPI_(void) ConfigureTab(
        /* [in] */ IActionBarTab* tab,
        /* [in] */ Int32 position);

    CARAPI_(void) ShowForActionMode();

    CARAPI_(void) HideForActionMode();

    static CARAPI_(Boolean) CheckShowingFlags(
        /* [in] */ Boolean hiddenByApp,
        /* [in] */ Boolean hiddenBySystem,
        /* [in] */ Boolean showingForMode);

    CARAPI_(void) UpdateVisibility(
        /* [in] */ Boolean fromSystem);

private:
    static const String TAG;

    AutoPtr<IContext> mContext;
    AutoPtr<IContext> mThemedContext;
    AutoPtr<IActivity> mActivity;
    AutoPtr<IDialog> mDialog;

    AutoPtr<IActionBarOverlayLayout> mOverlayLayout;
    AutoPtr<IActionBarContainer> mContainerView;
    AutoPtr<IViewGroup> mTopVisibilityView;
    AutoPtr<IActionBarView> mActionView;
    AutoPtr<IActionBarContextView> mContextView;
    AutoPtr<IActionBarContainer> mSplitView;
    AutoPtr<IView> mContentView;
    AutoPtr<IScrollingTabContainerView> mTabScrollView;

    List< AutoPtr<TabImpl> > mTabs;

    AutoPtr<TabImpl> mSelectedTab;
    Int32 mSavedTabPosition; //= INVALID_POSITION;

    Boolean mDisplayHomeAsUpSet;

    AutoPtr<ActionModeImpl> mActionMode;
    AutoPtr<IActionMode> mDeferredDestroyActionMode;
    AutoPtr<IActionModeCallback> mDeferredModeDestroyCallback;

    Boolean mLastMenuVisibility;
    List< AutoPtr<IActionBarOnMenuVisibilityListener> > mMenuVisibilityListeners;

    static const Int32 CONTEXT_DISPLAY_NORMAL = 0;
    static const Int32 CONTEXT_DISPLAY_SPLIT = 1;
    static const Int32 INVALID_POSITION = -1;

    Int32 mContextDisplayMode;
    Boolean mHasEmbeddedTabs;

    AutoPtr<IHandler> mHandler; //= new Handler();
    AutoPtr<IRunnable> mTabSelector;

    Int32 mCurWindowVisibility;

    Boolean mHiddenByApp;
    Boolean mHiddenBySystem;
    Boolean mShowingForMode;

    Boolean mNowShowing; //= true;

    AutoPtr<IAnimator> mCurrentShowAnim;
    Boolean mShowHideAnimationEnabled;

    AutoPtr<IAnimatorListener> mHideListener;
    AutoPtr<IAnimatorListener> mShowListener;

};

}
}
}
}

#endif //__CACTIONBARIMPL_H__
