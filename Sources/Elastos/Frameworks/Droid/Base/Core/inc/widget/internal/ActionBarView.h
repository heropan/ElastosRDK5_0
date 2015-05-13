
#ifndef __ACTIONBARVIEW_H__
#define __ACTIONBARVIEW_H__

#include "AbsActionBarView.h"

using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IWindowCallback;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::Menu::IMenuItemImpl;
using Elastos::Droid::View::Menu::ISubMenuBuilder;
using Elastos::Droid::View::Menu::IActionMenuItem;
using Elastos::Droid::Widget::Internal::IScrollingTabContainerView;
using Elastos::Droid::Widget::Internal::IActionBarViewHomeView;
using Elastos::Droid::Widget::ISpinnerAdapter;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::IAdapterViewOnItemSelectedListener;
using Elastos::Droid::App::IActionBar;
using Elastos::Droid::App::IActionBarOnNavigationListener;

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

class ActionBarView : public AbsActionBarView
{
public:
    ActionBarView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI SetWindowCallback(
        /* [in] */ IWindowCallback* cb);

    virtual CARAPI OnDetachedFromWindow();

    virtual CARAPI_(Boolean) ShouldDelayChildPressedState();

    virtual CARAPI InitProgress();

    virtual CARAPI InitIndeterminateProgress();

    virtual CARAPI SetSplitActionBar(
        /* [in] */ Boolean splitActionBar);

    virtual CARAPI_(Boolean) IsSplitActionBar();

    virtual CARAPI_(Boolean) HasEmbeddedTabs();

    virtual CARAPI SetEmbeddedTabView(
        /* [in] */ IScrollingTabContainerView* tabs);

    virtual CARAPI SetCallback(
        /* [in] */ IActionBarOnNavigationListener* callback);

    virtual CARAPI SetMenu(
        /* [in] */ IMenu* menu,
        /* [in] */ IMenuPresenterCallback* cb);

    virtual CARAPI_(Boolean) HasExpandedActionView();

    virtual CARAPI CollapseActionView();

    virtual CARAPI SetCustomNavigationView(
        /* [in] */ IView* view);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetTitle();

    virtual CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI SetWindowTitle(
        /* [in] */ ICharSequence* title);

    virtual CARAPI_(AutoPtr<ICharSequence>) GetSubtitle();

    virtual CARAPI SetSubtitle(
        /* [in] */ ICharSequence* subtitle);

    virtual CARAPI SetHomeButtonEnabled(
        /* [in] */ Boolean enable);

    virtual CARAPI SetHomeButtonEnabled(
        /* [in] */ Boolean enable,
        /* [in] */ Boolean recordState);

    virtual CARAPI SetDisplayOptions(
        /* [in] */ Int32 options);

    virtual CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    virtual CARAPI SetIcon(
        /* [in] */ Int32 resId);

    virtual CARAPI SetLogo(
        /* [in] */ IDrawable* logo);

    virtual CARAPI SetLogo(
        /* [in] */ Int32 resId);

    virtual CARAPI SetNavigationMode(
        /* [in] */ Int32 mode);

    virtual CARAPI SetDropdownAdapter(
        /* [in] */ ISpinnerAdapter* adapter);

    virtual CARAPI_(AutoPtr<ISpinnerAdapter>) GetDropdownAdapter();

    virtual CARAPI SetDropdownSelectedPosition(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int32) GetDropdownSelectedPosition();

    virtual CARAPI_(AutoPtr<IView>) GetCustomNavigationView();

    virtual CARAPI_(Int32) GetNavigationMode();

    virtual CARAPI_(Int32) GetDisplayOptions();

    virtual CARAPI SetContextView(
        /* [in] */ IActionBarContextView* view);

    virtual CARAPI SetCollapsable(
        /* [in] */ Boolean collapsable);

    virtual CARAPI_(Boolean) IsCollapsed();

    virtual CARAPI_(Boolean) IsTitleTruncated();

    virtual CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    virtual CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* lp);

    virtual CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    virtual CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* p);

protected:
    ActionBarView();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateDefaultLayoutParams();

    CARAPI OnFinishInflate();

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

private:
    CARAPI_(void) SetTitleImpl(
        /* [in] */ ICharSequence* title);

    CARAPI_(void) ConfigPresenters(
        /* [in] */ IMenuBuilder* builder);

    CARAPI_(void) InitTitle();

public:
/*    static class SavedState extends BaseSavedState {
        int expandedMenuItemId;
        boolean isOverflowOpen;

        SavedState(Parcelable superState) {
            super(superState);
        }

        private SavedState(Parcel in) {
            super(in);
            expandedMenuItemId = in.readInt();
            isOverflowOpen = in.readInt() != 0;
        }

        @Override
        public void writeToParcel(Parcel out, int flags) {
            super.writeToParcel(out, flags);
            out.writeInt(expandedMenuItemId);
            out.writeInt(isOverflowOpen ? 1 : 0);
        }

        public static final Parcelable.Creator<SavedState> CREATOR =
                new Parcelable.Creator<SavedState>() {
            public SavedState createFromParcel(Parcel in) {
                return new SavedState(in);
            }

            public SavedState[] newArray(int size) {
                return new SavedState[size];
            }
        };
    }*/

private:
    class NavItemSelectedListener
        : public IAdapterViewOnItemSelectedListener
        , public ElRefBase
    {
    public:
        NavItemSelectedListener(
            /* [in] */ ActionBarView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnItemSelected(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnNothingSelected(
            /* [in] */ IAdapterView* parent);
    private:
        ActionBarView* mHost;
    };

    class ExpandedActionViewUpListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        ExpandedActionViewUpListener(
            /* [in] */ ActionBarView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);
    private:
        ActionBarView* mHost;
    };

    class UpClickListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        UpClickListener(
            /* [in] */ ActionBarView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);
    private:
        ActionBarView* mHost;
    };

    class ExpandedActionViewMenuPresenter
        : public IMenuPresenter
        , public IWeakReferenceSource
        , public ElRefBase
    {
    public:
        AutoPtr<IMenuBuilder> mMenu;
        AutoPtr<IMenuItemImpl> mCurrentExpandedItem;

    public:
        ExpandedActionViewMenuPresenter(
            /* [in] */ ActionBarView* host);

        CAR_INTERFACE_DECL()

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        CARAPI InitForMenu(
            /* [in] */ IContext* context,
            /* [in] */ IMenuBuilder* menu);

        CARAPI GetMenuView(
            /* [in] */ IViewGroup* root,
            /* [out] */ IMenuView** view);

        CARAPI UpdateMenuView(
            /* [in] */ Boolean cleared);

        CARAPI SetCallback(
            /* [in] */ IMenuPresenterCallback* cb);

        CARAPI OnSubMenuSelected(
            /* [in] */ ISubMenuBuilder* subMenu,
            /* [out] */ Boolean* handled);

        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        CARAPI FlagActionItems(
            /* [out] */ Boolean* shown);

        CARAPI ExpandItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* expanded);

        CARAPI CollapseItemActionView(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItemImpl* item,
            /* [out] */ Boolean* collapsed);

        CARAPI GetId(
            /* [out] */ Int32* id);

        CARAPI OnSaveInstanceState(
            /* [out] */ IParcelable** pa);

        CARAPI OnRestoreInstanceState(
            /* [in] */ IParcelable* state);

    private:
        ActionBarView* mHost;
    };


public:
    static const Int32 DISPLAY_DEFAULT = 0;
    AutoPtr<IWindowCallback> mWindowCallback;

private:
    static const String TAG; //= "ActionBarView";

    static const Int32 DISPLAY_RELAYOUT_MASK =
            IActionBar::DISPLAY_SHOW_HOME |
            IActionBar::DISPLAY_USE_LOGO |
            IActionBar::DISPLAY_HOME_AS_UP |
            IActionBar::DISPLAY_SHOW_CUSTOM |
            IActionBar::DISPLAY_SHOW_TITLE |
            IActionBar::DISPLAY_TITLE_MULTIPLE_LINES;

    static const Int32 DEFAULT_CUSTOM_GRAVITY = IGravity::START | IGravity::CENTER_VERTICAL;
    static const Int32 MAX_HOME_SLOP = 32; // dp

    Int32 mNavigationMode;
    Int32 mDisplayOptions;// = -1;
    AutoPtr<ICharSequence> mTitle;
    AutoPtr<ICharSequence> mSubtitle;
    AutoPtr<IDrawable> mIcon;
    AutoPtr<IDrawable> mLogo;

    AutoPtr<IActionBarViewHomeView> mHomeLayout;
    AutoPtr<IActionBarViewHomeView> mExpandedHomeLayout;
    AutoPtr<ILinearLayout> mTitleLayout;
    AutoPtr<ITextView> mTitleView;
    AutoPtr<ITextView> mSubtitleView;
    AutoPtr<IView> mTitleUpView;
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
    Boolean mIsCollapsable;
    Boolean mIsCollapsed;
    Boolean mWasHomeEnabled; // Was it enabled before action view expansion?

    AutoPtr<IMenuBuilder> mOptionsMenu;
    AutoPtr<IActionBarContextView> mContextView;
    AutoPtr<IActionMenuItem> mLogoNavItem;
    AutoPtr<ISpinnerAdapter> mSpinnerAdapter;
    AutoPtr<IActionBarOnNavigationListener> mCallback;
    AutoPtr<IRunnable> mTabSelector;
    AutoPtr<ExpandedActionViewMenuPresenter> mExpandedMenuPresenter;
    AutoPtr<IView> mExpandedActionView;



    AutoPtr<IRect> mTempRect;// = new Rect();
    Int32 mMaxHomeSlop;

    AutoPtr<IAdapterViewOnItemSelectedListener> mNavItemSelectedListener;
    AutoPtr<IViewOnClickListener> mExpandedActionViewUpListener;
    AutoPtr<IViewOnClickListener> mUpClickListener;

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ACTIONBARVIEW_H__
