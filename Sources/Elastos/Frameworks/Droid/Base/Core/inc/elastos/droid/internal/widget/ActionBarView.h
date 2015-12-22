
#ifndef __ELASTOS_DROID_INTERNAL_WIDGET_ACTIONBARVIEW_H__
#define __ELASTOS_DROID_INTERNAL_WIDGET_ACTIONBARVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/internal/widget/AbsActionBarView.h"
//#include "elastos/droid/internal/widget/DecorToolbar.h"
#include "elastos/droid/widget/FrameLayout.h"
#include "Elastos.Droid.App.h"

using Elastos::Droid::App::IActionBar;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Internal::View::Menu::IActionMenuItem;
using Elastos::Droid::Internal::View::Menu::IMenuBuilder;
using Elastos::Droid::Internal::View::Menu::IMenuItemImpl;
using Elastos::Droid::Internal::View::Menu::IMenuPresenter;
using Elastos::Droid::Internal::View::Menu::IMenuPresenterCallback;
using Elastos::Droid::Internal::View::Menu::IMenuView;
using Elastos::Droid::Internal::View::Menu::ISubMenuBuilder;
using Elastos::Droid::Internal::Widget::AbsActionBarView;
//using Elastos::Droid::Internal::Widget::DecorToolbar;
using Elastos::Droid::Internal::Widget::IActionBarContextView;
using Elastos::Droid::Internal::Widget::IActionBarView;
using Elastos::Droid::Internal::Widget::IDecorToolbar;
using Elastos::Droid::Internal::Widget::IScrollingTabContainerView;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IWindowCallback;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IAdapterViewOnItemSelectedListener;
using Elastos::Droid::Widget::IAdapterViewOnItemSelectedListener;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Droid::Widget::IProgressBar;
using Elastos::Droid::Widget::ISpinner;
using Elastos::Droid::Widget::ISpinnerAdapter;
using Elastos::Droid::Widget::ITextView;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Widget {

/**
  * @hide
  */
class ActionBarView
    : public AbsActionBarView
    //, public DecorToolbar
    , public IActionBarView
{
public:
    class SavedState
        : public View::BaseSavedState
    {
        friend class InnerParcelableCreator;
    public:
        SavedState(
            /* [in] */ IParcelable* superState);

        // @Override
        CARAPI WriteToParcel(
            /* [in] */ IParcel* out,
            /* [in] */ Int32 flags);

    private:
        SavedState(
            /* [in] */ IParcel* in);

    public:
        Int32 expandedMenuItemId;
        Boolean isOverflowOpen;
        static const AutoPtr<IParcelable> CREATOR;
    };

private:
    class InnerOnClickListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerOnClickListener(
            /* [in] */ ActionBarView* owner);

        // @Override
        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        ActionBarView* mOwner;
    };

    class InnerOnClickListener1
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerOnClickListener1(
            /* [in] */ ActionBarView* owner);

        virtual CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        ActionBarView* mOwner;
    };

    class InnerParcelableCreator
        : public Object
        , public IParcelable
    {
    public:
        CAR_INTERFACE_DECL()

        InnerParcelableCreator();

        virtual CARAPI CreateFromParcel(
            /* [in] */ IParcel* in,
            /* [out] */ SavedState** result);

        virtual CARAPI NewArray(
            /* [in] */ Int32 size,
            /* [out] */ ArrayOf<SavedState*>** result);

        // override for compile
        virtual CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        // override for compile
        virtual CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    private:
        SavedState* mOwner;
    };

    class HomeView
        : public FrameLayout
    {
    public:
        HomeView();

        CARAPI constructor(
            /* [in] */ IContext* context);

        CARAPI constructor(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs);

        virtual CARAPI SetShowUp(
            /* [in] */ Boolean isUp);

        virtual CARAPI SetShowIcon(
            /* [in] */ Boolean showIcon);

        virtual CARAPI SetIcon(
            /* [in] */ IDrawable* icon);

        virtual CARAPI SetUpIndicator(
            /* [in] */ IDrawable* d);

        virtual CARAPI SetDefaultUpIndicator(
            /* [in] */ IDrawable* d);

        virtual CARAPI SetUpIndicator(
            /* [in] */ Int32 resId);

        // @Override
        CARAPI DispatchPopulateAccessibilityEvent(
            /* [in] */ IAccessibilityEvent* event,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnPopulateAccessibilityEvent(
            /* [in] */ IAccessibilityEvent* event);

        // @Override
        CARAPI DispatchHoverEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        virtual CARAPI GetStartOffset(
            /* [out] */ Int32* result);

        virtual CARAPI GetUpWidth(
            /* [out] */ Int32* result);

    protected:
        // @Override
        CARAPI_(void) OnConfigurationChanged(
            /* [in] */ IConfiguration* newConfig);

        // @Override
        CARAPI OnFinishInflate();

        // @Override
        CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);

        // @Override
        CARAPI OnLayout(
            /* [in] */ Boolean changed,
            /* [in] */ Int32 l,
            /* [in] */ Int32 t,
            /* [in] */ Int32 r,
            /* [in] */ Int32 b);

    private:
        CARAPI_(void) UpdateUpIndicator();

    private:
        AutoPtr<IImageView> mUpView;
        AutoPtr<IImageView> mIconView;
        Int32 mUpWidth;
        Int32 mStartOffset;
        Int32 mUpIndicatorRes;
        AutoPtr<IDrawable> mDefaultUpIndicator;
        AutoPtr<IDrawable> mUpIndicator;
        static const Int64 DEFAULT_TRANSITION_DURATION = 150;
    };

    class ExpandedActionViewMenuPresenter
        : public Object
        , public IMenuPresenter
    {
    public:
        CAR_INTERFACE_DECL()

        ExpandedActionViewMenuPresenter(
            /* [in] */ ActionBarView* owner);

        // @Override
        CARAPI InitForMenu(
            /* [in] */ IContext* context,
            /* [in] */ IMenuBuilder* menu);

        // @Override
        CARAPI GetMenuView(
            /* [in] */ IViewGroup* root,
            /* [out] */ IMenuView** result);

        // @Override
        CARAPI UpdateMenuView(
            /* [in] */ Boolean cleared);

        // @Override
        CARAPI SetCallback(
            /* [in] */ IMenuPresenterCallback* cb);

        // @Override
        CARAPI OnSubMenuSelected(
            /* [in] */ ISubMenuBuilder* subMenu,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        // @Override
        CARAPI FlagActionItems(
            /* [out] */ Boolean* result);

        // @Override
        CARAPI ExpandItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI CollapseItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI GetId(
            /* [out] */ Int32* result);

        // @Override
        CARAPI OnSaveInstanceState(
            /* [out] */ IParcelable** result);

        // @Override
        CARAPI OnRestoreInstanceState(
            /* [in] */ IParcelable* state);

    public:
        AutoPtr<IMenuBuilder> mMenu;
        AutoPtr<IMenuItemImpl> mCurrentExpandedItem;

    private:
        ActionBarView* mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    ActionBarView();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
      * Set the window callback used to invoke menu items; used for dispatching home button presses.
      * @param cb Window callback to dispatch to
      */
    virtual CARAPI SetWindowCallback(
        /* [in] */ IWindowCallback* cb);

    // @Override
    CARAPI OnDetachedFromWindow();

    // @Override
    CARAPI ShouldDelayChildPressedState(
        /* [out] */ Boolean* result);

    virtual CARAPI InitProgress();

    virtual CARAPI InitIndeterminateProgress();

    // @Override
    CARAPI SetSplitToolbar(
        /* [in] */ Boolean splitActionBar);

    virtual CARAPI IsSplit(
        /* [out] */ Boolean* result);

    virtual CARAPI CanSplit(
        /* [out] */ Boolean* result);

    virtual CARAPI HasEmbeddedTabs(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI SetEmbeddedTabView(
        /* [in] */ IScrollingTabContainerView* tabs);

    virtual CARAPI SetMenuPrepared();

    virtual CARAPI SetMenu(
        /* [in] */ IMenu* menu,
        /* [in] */ IMenuPresenterCallback* cb);

    virtual CARAPI HasExpandedActionView(
        /* [out] */ Boolean* result);

    virtual CARAPI CollapseActionView();

    virtual CARAPI SetCustomView(
        /* [in] */ IView* view);

    virtual CARAPI GetTitle(
        /* [out] */ ICharSequence** result);

    /**
      * Set the action bar title. This will always replace or override window titles.
      * @param title Title to set
      *
      * @see #setWindowTitle(CharSequence)
      */
    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    /**
      * Set the window title. A window title will always be replaced or overridden by a user title.
      * @param title Title to set
      *
      * @see #setTitle(CharSequence)
      */
    virtual CARAPI SetWindowTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI GetSubtitle(
        /* [out] */ ICharSequence** result);

    virtual CARAPI SetSubtitle(
        /* [in] */ ICharSequence* subtitle);

    virtual CARAPI SetHomeButtonEnabled(
        /* [in] */ Boolean enable);

    virtual CARAPI SetDisplayOptions(
        /* [in] */ Int32 options);

    virtual CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    virtual CARAPI SetIcon(
        /* [in] */ Int32 resId);

    virtual CARAPI HasIcon(
        /* [out] */ Boolean* result);

    virtual CARAPI SetLogo(
        /* [in] */ IDrawable* logo);

    virtual CARAPI SetLogo(
        /* [in] */ Int32 resId);

    virtual CARAPI HasLogo(
        /* [out] */ Boolean* result);

    virtual CARAPI SetNavigationMode(
        /* [in] */ Int32 mode);

    virtual CARAPI SetDropdownParams(
        /* [in] */ ISpinnerAdapter* adapter,
        /* [in] */ IAdapterViewOnItemSelectedListener* l);

    virtual CARAPI GetDropdownItemCount(
        /* [out] */ Int32* result);

    virtual CARAPI SetDropdownSelectedPosition(
        /* [in] */ Int32 position);

    virtual CARAPI GetDropdownSelectedPosition(
        /* [out] */ Int32* result);

    virtual CARAPI GetCustomView(
        /* [out] */ IView** result);

    virtual CARAPI GetNavigationMode(
        /* [out] */ Int32* result);

    virtual CARAPI GetDisplayOptions(
        /* [out] */ Int32* result);

    // @Override
    CARAPI GetViewGroup(
        /* [out] */ IViewGroup** result);

    virtual CARAPI SetContextView(
        /* [in] */ IActionBarContextView* view);

    virtual CARAPI SetCollapsible(
        /* [in] */ Boolean collapsible);

    /**
      * @return True if any characters in the title were truncated
      */
    virtual CARAPI IsTitleTruncated(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** result);

    // @Override
    CARAPI GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* lp,
        /* [out] */ IViewGroupLayoutParams** result);

    // @Override
    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** result);

    // @Override
    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* p);

    virtual CARAPI SetNavigationIcon(
        /* [in] */ IDrawable* indicator);

    // @Override
    CARAPI SetDefaultNavigationIcon(
        /* [in] */ IDrawable* icon);

    virtual CARAPI SetNavigationIcon(
        /* [in] */ Int32 resId);

    virtual CARAPI SetNavigationContentDescription(
        /* [in] */ ICharSequence* description);

    virtual CARAPI SetNavigationContentDescription(
        /* [in] */ Int32 resId);

    // @Override
    CARAPI SetDefaultNavigationContentDescription(
        /* [in] */ Int32 defaultNavigationContentDescription);

protected:
    // @Override
    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    // @Override
    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ IViewGroupLayoutParams** result);

    // @Override
    CARAPI OnFinishInflate();

    // @Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    // @Override
    CARAPI OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

private:
    CARAPI_(void) ConfigPresenters(
        /* [in] */ IMenuBuilder* builder);

    CARAPI_(void) SetTitleImpl(
        /* [in] */ ICharSequence* title);

    CARAPI_(void) SetHomeButtonEnabled(
        /* [in] */ Boolean enable,
        /* [in] */ Boolean recordState);

    CARAPI_(void) UpdateHomeAccessibility(
        /* [in] */ Boolean homeEnabled);

    /**
      * Compose a content description for the Home/Up affordance.
      *
      * <p>As this encompasses the icon/logo, title and subtitle all in one, we need
      * a description for the whole wad of stuff that can be localized properly.</p>
      */
    CARAPI_(AutoPtr<ICharSequence>) BuildHomeContentDescription();

    CARAPI_(void) InitTitle();

public:
    /**
      * Display options applied by default
      */
    static const Int32 DISPLAY_DEFAULT = 0;
    AutoPtr<IView> mExpandedActionView;
    AutoPtr<IWindowCallback> mWindowCallback;

private:
    static const String TAG;
    static const Int32 DISPLAY_RELAYOUT_MASK = IActionBar::DISPLAY_SHOW_HOME |
        IActionBar::DISPLAY_USE_LOGO |
        IActionBar::DISPLAY_HOME_AS_UP |
        IActionBar::DISPLAY_SHOW_CUSTOM |
        IActionBar::DISPLAY_SHOW_TITLE |
        IActionBar::DISPLAY_TITLE_MULTIPLE_LINES;
    static const Int32 DEFAULT_CUSTOM_GRAVITY = IGravity::START | IGravity::CENTER_VERTICAL;
    Int32 mNavigationMode;
    Int32 mDisplayOptions;
    AutoPtr<ICharSequence> mTitle;
    AutoPtr<ICharSequence> mSubtitle;
    AutoPtr<IDrawable> mIcon;
    AutoPtr<IDrawable> mLogo;
    AutoPtr<ICharSequence> mHomeDescription;
    Int32 mHomeDescriptionRes;
    AutoPtr<HomeView> mHomeLayout;
    AutoPtr<HomeView> mExpandedHomeLayout;
    AutoPtr<ILinearLayout> mTitleLayout;
    AutoPtr<ITextView> mTitleView;
    AutoPtr<ITextView> mSubtitleView;
    AutoPtr<IViewGroup> mUpGoerFive;
    AutoPtr<ISpinner> mSpinner;
    AutoPtr<ILinearLayout> mListNavLayout;
    AutoPtr<IScrollingTabContainerView> mTabScrollView;
    AutoPtr<IView> mCustomNavView;
    AutoPtr<IProgressBar> mProgressView;
    AutoPtr<IProgressBar> mIndeterminateProgressView;
    Int32 mProgressBarPadding;
    Int32 mItemPadding;
    Int32 mTitleStyleRes;
    Int32 mSubtitleStyleRes;
    Int32 mProgressStyle;
    Int32 mIndeterminateProgressStyle;
    Boolean mUserTitle;
    Boolean mIncludeTabs;
    Boolean mIsCollapsible;
    Boolean mWasHomeEnabled;
    AutoPtr<IMenuBuilder> mOptionsMenu;
    Boolean mMenuPrepared;
    AutoPtr<IActionBarContextView> mContextView;
    AutoPtr<IActionMenuItem> mLogoNavItem;
    AutoPtr<ISpinnerAdapter> mSpinnerAdapter;
    AutoPtr<IAdapterViewOnItemSelectedListener> mNavItemSelectedListener;
    AutoPtr<IRunnable> mTabSelector;
    AutoPtr<ExpandedActionViewMenuPresenter> mExpandedMenuPresenter;
    Int32 mDefaultUpDescription;
    /*const*/ AutoPtr<IViewOnClickListener> mExpandedActionViewUpListener;
    /*const*/ AutoPtr<IViewOnClickListener> mUpClickListener;
};

} // namespace Widget
} // namespace Internal
} // namespace Droid
} // namespace Elastos

template <>
struct Conversion<Elastos::Droid::Internal::Widget::ActionBarView::SavedState*, IInterface*>
{
    enum { exists = TRUE, exists2Way = FALSE, sameType = FALSE };
};

#endif // __ELASTOS_DROID_INTERNAL_WIDGET_ACTIONBARVIEW_H__

