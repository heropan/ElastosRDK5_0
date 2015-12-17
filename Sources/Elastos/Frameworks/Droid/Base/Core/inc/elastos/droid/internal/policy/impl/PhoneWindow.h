#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEWINDOW_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEWINDOW_H__

#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Media.h"
#include "Elastos.Droid.Net.h"
#include "Elastos.Droid.Transition.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/Window.h"
#include "elastos/droid/widget/FrameLayout.h"

using Elastos::Droid::App::IKeyguardManager;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Internal::View::IRootViewSurfaceTaker;
using Elastos::Droid::Internal::View::Menu::IContextMenuBuilder;
using Elastos::Droid::Internal::View::Menu::IIconMenuPresenter;
using Elastos::Droid::Internal::View::Menu::IListMenuPresenter;
using Elastos::Droid::Internal::View::Menu::IMenuBuilder;
using Elastos::Droid::Internal::View::Menu::IMenuBuilderCallback;
using Elastos::Droid::Internal::View::Menu::IMenuDialogHelper;
using Elastos::Droid::Internal::View::Menu::IMenuPresenterCallback;
using Elastos::Droid::Internal::View::Menu::IMenuView;
using Elastos::Droid::View::IOnWindowDismissedCallback;
using Elastos::Droid::Internal::Widget::IActionBarContextView;
using Elastos::Droid::Internal::Widget::IBackgroundFallback;
using Elastos::Droid::Internal::Widget::IDecorContentParent;
using Elastos::Droid::Internal::Widget::IOnDismissedListener;
using Elastos::Droid::Internal::Widget::IOnSwipeProgressChangedListener;
using Elastos::Droid::Internal::Widget::ISwipeDismissLayout;
using Elastos::Droid::Media::IAudioManager;
//using Elastos::Droid::Widget::IMediaController;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Transition::IScene;
using Elastos::Droid::Transition::ITransition;
using Elastos::Droid::Transition::ITransitionManager;
using Elastos::Droid::Utility::ISparseArray;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IViewRootImpl;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IWindowInsets;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IInputQueueCallback;
using Elastos::Droid::View::ISurfaceHolderCallback2;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::Widget::IPopupWindow;
using Elastos::Droid::Widget::IProgressBar;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::Window;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Core::ICharSequence;
using Elastos::Core::IRunnable;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

/**
  * Android-specific Window.
  * <p>
  * todo: need to pull the generic functionality out into a base class
  * in android.widget.
  */
class PhoneWindow
    : public Window
    , public IPhoneWindow
    , public IMenuBuilderCallback
{
public:
    class WindowManagerHolder
    {
    private:
        static CARAPI_(AutoPtr<IIWindowManager>) MiddleInitSwindowmanager();

    public:
        static AutoPtr<IIWindowManager> sWindowManager;
    };

    class ActionMenuPresenterCallback
        : public Object
        , public IMenuPresenterCallback
    {
    public:
        CAR_INTERFACE_DECL()

        ActionMenuPresenterCallback(
            /* [in] */ PhoneWindow* owner);

        // @Override
        CARAPI OnOpenSubMenu(
            /* [in] */ IMenuBuilder* subMenu,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);
    private:
        PhoneWindow* mOwner;
    };

    class DecorView
        : public FrameLayout
        , public IRootViewSurfaceTaker
    {
    private:
        /**
          * Clears out internal reference when the action mode is destroyed.
          */
        class ActionModeCallbackWrapper
            : public Object
            , public IActionModeCallback
        {
        public:
            CAR_INTERFACE_DECL()

            ActionModeCallbackWrapper(
                /* [in] */ DecorView* owner,
                /* [in] */ IActionModeCallback* wrapped);

            virtual CARAPI OnCreateActionMode(
                /* [in] */ IActionMode* mode,
                /* [in] */ IMenu* menu,
                /* [out] */ Boolean* result);

            virtual CARAPI OnPrepareActionMode(
                /* [in] */ IActionMode* mode,
                /* [in] */ IMenu* menu,
                /* [out] */ Boolean* result);

            virtual CARAPI OnActionItemClicked(
                /* [in] */ IActionMode* mode,
                /* [in] */ IMenuItem* item,
                /* [out] */ Boolean* result);

            virtual CARAPI OnDestroyActionMode(
                /* [in] */ IActionMode* mode);

        private:
            AutoPtr<IActionModeCallback> mWrapped;
            DecorView* mOwner;
        };

    public:
        CAR_INTERFACE_DECL()

        DecorView(
            /* [in] */ PhoneWindow* owner,
            /* [in] */ IContext* context,
            /* [in] */ Int32 featureId);

        virtual CARAPI SetBackgroundFallback(
            /* [in] */ Int32 resId);

        // @Override
        CARAPI_(void) OnDraw(
            /* [in] */ ICanvas* c);

        // @Override ViewGroup
        CARAPI DispatchKeyEvent(
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI DispatchKeyShortcutEvent(
            /* [in] */ IKeyEvent* ev,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI DispatchTouchEvent(
            /* [in] */ IMotionEvent* ev,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI DispatchTrackballEvent(
            /* [in] */ IMotionEvent* ev,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI DispatchGenericMotionEvent(
            /* [in] */ IMotionEvent* ev,
            /* [out] */ Boolean* result);

        virtual CARAPI SuperDispatchKeyEvent(
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        virtual CARAPI SuperDispatchKeyShortcutEvent(
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        virtual CARAPI SuperDispatchTouchEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        virtual CARAPI SuperDispatchTrackballEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        virtual CARAPI SuperDispatchGenericMotionEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI DispatchApplyWindowInsets(
            /* [in] */ IWindowInsets* insets,
            /* [out] */ IWindowInsets** result);

        // @Override
        CARAPI OnTouchEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnInterceptTouchEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI SendAccessibilityEvent(
            /* [in] */ Int32 eventType);

        // @Override
        CARAPI DispatchPopulateAccessibilityEvent(
            /* [in] */ IAccessibilityEvent* event,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI Draw(
            /* [in] */ ICanvas* canvas);

        // @Override
        CARAPI ShowContextMenuForChild(
            /* [in] */ IView* originalView,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI StartActionModeForChild(
            /* [in] */ IView* originalView,
            /* [in] */ IActionModeCallback* callback,
            /* [out] */ IActionMode** res);

        // @Override
        CARAPI StartActionMode(
            /* [in] */ IActionModeCallback* callback,
            /* [out] */ IActionMode** res);

        virtual CARAPI StartChanging();

        virtual CARAPI FinishChanging();

        virtual CARAPI SetWindowBackground(
            /* [in] */ IDrawable* drawable);

        // @Override
        CARAPI SetBackgroundDrawable(
            /* [in] */ IDrawable* d);

        virtual CARAPI SetWindowFrame(
            /* [in] */ IDrawable* drawable);

        // @Override
        CARAPI OnWindowSystemUiVisibilityChanged(
            /* [in] */ Int32 visible);

        // @Override IView
        CARAPI OnApplyWindowInsets(
            /* [in] */ IWindowInsets* insets,
            /* [out] */ IWindowInsets** result);

        // @Override ViewGroup
        CARAPI IsTransitionGroup(
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnWindowFocusChanged(
            /* [in] */ Boolean hasWindowFocus);

        virtual CARAPI UpdateWindowResizeState();

        // @Override
        CARAPI OnCloseSystemDialogs(
            /* [in] */ const String& reason);

        virtual CARAPI WillYouTakeTheSurface(
            /* [out] */ ISurfaceHolderCallback2** result);

        virtual CARAPI WillYouTakeTheInputQueue(
            /* [out] */ IInputQueueCallback** result);

        virtual CARAPI SetSurfaceType(
            /* [in] */ Int32 type);

        virtual CARAPI SetSurfaceFormat(
            /* [in] */ Int32 format);

        virtual CARAPI SetSurfaceKeepScreenOn(
            /* [in] */ Boolean keepOn);

    protected:
        // @Override
        CARAPI_(Boolean) SetFrame(
            /* [in] */ Int32 l,
            /* [in] */ Int32 t,
            /* [in] */ Int32 r,
            /* [in] */ Int32 b);

        // @Override
        CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);

        // @Override
        CARAPI OnAttachedToWindow();

        // @Override
        CARAPI OnDetachedFromWindow();

    private:
        CARAPI_(Boolean) IsOutOfBounds(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

        CARAPI_(AutoPtr<IWindowInsets>) UpdateColorViews(
            /* [in] */ IWindowInsets* insets);

        CARAPI_(AutoPtr<IView>) UpdateColorViewInt(
            /* [in] */ IView* view,
            /* [in] */ Int32 sysUiVis,
            /* [in] */ Int32 systemUiHideFlag,
            /* [in] */ Int32 translucentFlag,
            /* [in] */ Int32 color,
            /* [in] */ Int32 height,
            /* [in] */ Int32 verticalGravity,
            /* [in] */ const String& transitionName,
            /* [in] */ Int32 id,
            /* [in] */ Boolean hiddenByWindowFlag);

        CARAPI_(AutoPtr<IWindowInsets>) UpdateStatusGuard(
            /* [in] */ IWindowInsets* insets);

        CARAPI_(void) UpdateNavigationGuard(
            /* [in] */ IWindowInsets* insets);

        CARAPI_(void) DrawableChanged();

    public:
        /* package */
        Int32 mDefaultOpacity;

    private:
        /** The feature ID of the panel, or -1 if this is the application's DecorView */
        /*const*/ Int32 mFeatureId;
        /*const*/ AutoPtr<IRect> mDrawingBounds;
        /*const*/ AutoPtr<IRect> mBackgroundPadding;
        /*const*/ AutoPtr<IRect> mFramePadding;
        /*const*/ AutoPtr<IRect> mFrameOffsets;
        Boolean mChanging;
        AutoPtr<IDrawable> mMenuBackground;
        Boolean mWatchingForMenu;
        Int32 mDownY;
        AutoPtr<IActionMode> mActionMode;
        AutoPtr<IActionBarContextView> mActionModeView;
        AutoPtr<IPopupWindow> mActionModePopup;
        AutoPtr<IRunnable> mShowActionModePopup;
        // View added at runtime to draw under the status bar area
        AutoPtr<IView> mStatusGuard;
        // View added at runtime to draw under the navigation bar area
        AutoPtr<IView> mNavigationGuard;
        AutoPtr<IView> mStatusColorView;
        AutoPtr<IView> mNavigationColorView;
        /*const*/ AutoPtr<IBackgroundFallback> mBackgroundFallback;
        Int32 mLastTopInset;
        Int32 mLastBottomInset;
        Int32 mLastRightInset;
        PhoneWindow* mOwner;
    };

    class DrawableFeatureState
        : public Object
    {
    public:
        DrawableFeatureState(
            /* [in] */ Int32 _featureId);

    public:
        /*const*/ Int32 featureId;
        Int32 resid;
        AutoPtr<IUri> uri;
        AutoPtr<IDrawable> local;
        AutoPtr<IDrawable> child;
        AutoPtr<IDrawable> def;
        AutoPtr<IDrawable> cur;
        Int32 alpha;
        Int32 curAlpha;
    };

    class PanelFeatureState
        : public Object
    {
    public:
        class SavedState
            : public Object
            , public IPhoneWindowSavedState
            , public IParcelable
        {
        public:
            CAR_INTERFACE_DECL()

            SavedState();

            CARAPI constructor();

            CARAPI constructor(
                /* [in] */ IParcelable* superState);

            virtual CARAPI DescribeContents(
                /* [out] */ Int32* result);

            CARAPI ReadFromParcel(
                /* [in] */ IParcel* in);

            //@Override
            CARAPI WriteToParcel(
                /* [in] */ IParcel* out);

            //@Override
            //CARAPI ToString(
            //    /* [out] */ String* str);

        public:
            Int32 featureId;
            Boolean isOpen;
            Boolean isInExpandedMode;
            AutoPtr<IBundle> menuState;
            //static const AutoPtr<IParcelable> AutoPtr< ::Creator<SavedState> > CREATOR;
        };

    public:
        PanelFeatureState(
            /* [in] */ PhoneWindow* owner,
            /* [in] */ Int32 featureId);

        virtual CARAPI IsInListMode(
            /* [out] */ Boolean* result);

        virtual CARAPI HasPanelItems(
            /* [out] */ Boolean* result);

        /**
          * Unregister and free attached MenuPresenters. They will be recreated as needed.
          */
        virtual CARAPI ClearMenuPresenters();

        virtual CARAPI SetStyle(
            /* [in] */ IContext* context);

        virtual CARAPI SetMenu(
            /* [in] */ IMenuBuilder* menu);

        virtual CARAPI GetListMenuView(
            /* [in] */ IContext* context,
            /* [in] */ IMenuPresenterCallback* cb,
            /* [out] */ IMenuView** result);

        virtual CARAPI GetIconMenuView(
            /* [in] */ IContext* context,
            /* [in] */ IMenuPresenterCallback* cb,
            /* [out] */ IMenuView** result);

        virtual CARAPI OnSaveInstanceState(
            /* [out] */ IParcelable** result);

        virtual CARAPI OnRestoreInstanceState(
            /* [in] */ IParcelable* state);

        virtual CARAPI ApplyFrozenState();

    public:
        /** Feature ID for this panel. */
        Int32 featureId;
        // Information pulled from the style for this panel.
        Int32 background;
        /** The background when the panel spans the entire available width. */
        Int32 fullBackground;
        Int32 gravity;
        Int32 x;
        Int32 y;
        Int32 windowAnimations;
        /** Dynamic state of the panel. */
        AutoPtr<DecorView> decorView;
        /** The panel that was returned by onCreatePanelView(). */
        AutoPtr<IView> createdPanelView;
        /** The panel that we are actually showing. */
        AutoPtr<IView> shownPanelView;
        /** Use {@link #setMenu} to set this. */
        AutoPtr<IMenuBuilder> menu;
        AutoPtr<IIconMenuPresenter> iconMenuPresenter;
        AutoPtr<IListMenuPresenter> listMenuPresenter;
        /** true if this menu will show in single-list compact mode */
        Boolean isCompact;
        /** Theme resource ID for list elements of the panel menu */
        Int32 listPresenterTheme;
        /**
          * Whether the panel has been prepared (see
          * {@link PhoneWindow#preparePanel}).
          */
        Boolean isPrepared;
        /**
          * Whether an item's action has been performed. This happens in obvious
          * scenarios (user clicks on menu item), but can also happen with
          * chording menu+(shortcut key).
          */
        Boolean isHandled;
        Boolean isOpen;
        /**
          * True if the menu is in expanded mode, false if the menu is in icon
          * mode
          */
        Boolean isInExpandedMode;
        Boolean qwertyMode;
        Boolean refreshDecorView;
        Boolean refreshMenuContent;
        Boolean wasLastOpen;
        Boolean wasLastExpanded;
        /**
          * Contains the state of the menu when told to freeze.
          */
        AutoPtr<IBundle> frozenMenuState;
        /**
          * Contains the state of associated action views when told to freeze.
          * These are saved across invalidations.
          */
        AutoPtr<IBundle> frozenActionViewState;
        PhoneWindow* mOwner;
    };

    /**
      * Simple implementation of MenuBuilder.Callback that:
      * <li> Opens a submenu when selected.
      * <li> Calls back to the callback's onMenuItemSelected when an item is
      * selected.
      */
    class DialogMenuCallback
        : public Object
        , public IMenuBuilderCallback
        , public IMenuPresenterCallback
    {
    public:
        CAR_INTERFACE_DECL()

        DialogMenuCallback(
            /* [in] */ PhoneWindow* owner,
            /* [in] */ Int32 featureId);

        virtual CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        virtual CARAPI OnCloseSubMenu(
            /* [in] */ IMenuBuilder* menu);

        virtual CARAPI OnMenuItemSelected(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* result);

        virtual CARAPI OnMenuModeChange(
            /* [in] */ IMenuBuilder* menu);

        virtual CARAPI OnOpenSubMenu(
            /* [in] */ IMenuBuilder* subMenu,
            /* [out] */ Boolean* result);

    private:
        PhoneWindow* mOwner;
        Int32 mFeatureId;
        AutoPtr<IMenuDialogHelper> mSubMenuHelper;
    };

private:
    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        InnerRunnable(
            /* [in] */ PhoneWindow* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindow* mOwner;
    };

    class PanelMenuPresenterCallback
        : public Object
        , public IMenuPresenterCallback
    {
    public:
        CAR_INTERFACE_DECL()

        PanelMenuPresenterCallback(
            /* [in] */ PhoneWindow* owner);

        // @Override
        CARAPI OnCloseMenu(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ Boolean allMenusAreClosing);

        // @Override
        CARAPI OnOpenSubMenu(
            /* [in] */ IMenuBuilder* subMenu,
            /* [out] */ Boolean* handle);

    private:
        PhoneWindow* mOwner;
    };

    class InnerSwipeDismissLayoutOnDismissedListener1
        : public Object
        , public IOnDismissedListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerSwipeDismissLayoutOnDismissedListener1(
            /* [in] */ PhoneWindow* owner);

        // @Override
        CARAPI OnDismissed(
            /* [in] */ ISwipeDismissLayout* layout);

    private:
        PhoneWindow* mOwner;
    };

    class InnerSwipeDismissLayoutOnSwipeProgressChangedListener1
        : public Object
        , public IOnSwipeProgressChangedListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerSwipeDismissLayoutOnSwipeProgressChangedListener1(
            /* [in] */ PhoneWindow* owner);

        // @Override
        CARAPI OnSwipeProgressChanged(
            /* [in] */ ISwipeDismissLayout* layout,
            /* [in] */ Float progress,
            /* [in] */ Float translate);

        // @Override
        CARAPI OnSwipeCancelled(
            /* [in] */ ISwipeDismissLayout* layout);

    private:
        static const Float ALPHA_DECREASE;
        Boolean mIsTranslucent;
        PhoneWindow* mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    PhoneWindow();

    CARAPI constructor(
        /* [in] */ IContext* context);

    // @Override
    CARAPI SetContainer(
        /* [in] */ IWindow* container);

    // @Override
    CARAPI RequestFeature(
        /* [in] */ Int32 featureId,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI SetUiOptions(
        /* [in] */ Int32 uiOptions);

    // @Override
    CARAPI SetUiOptions(
        /* [in] */ Int32 uiOptions,
        /* [in] */ Int32 mask);

    // @Override
    CARAPI GetTransitionManager(
        /* [out] */ ITransitionManager** tm);

    // @Override
    CARAPI SetTransitionManager(
        /* [in] */ ITransitionManager* tm);

    // @Override
    CARAPI GetContentScene(
        /* [out] */ IScene** scene);

    // @Override
    CARAPI SetContentView(
        /* [in] */ Int32 layoutResID);

    // @Override
    CARAPI SetContentView(
        /* [in] */ IView* view);

    // @Override
    CARAPI SetContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* viewGroup);

    // @Override
    CARAPI AddContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* viewGroup);

    // @Override
    CARAPI GetCurrentFocus(
        /* [out] */ IView** view);

    // @Override
    CARAPI TakeSurface(
        /* [in] */  ISurfaceHolderCallback2* cb);

    virtual CARAPI TakeInputQueue(
        /* [in] */  IInputQueueCallback* cb);

    // @Override
    CARAPI IsFloating(
        /* [out] */ Boolean* isFloating);

    /**
      * Return a LayoutInflater instance that can be used to inflate XML view layout
      * resources for use in this Window.
      *
      * @return LayoutInflater The shared LayoutInflater.
      */
    // @Override
    CARAPI GetLayoutInflater(
        /* [out] */ ILayoutInflater** inflater);

    // @Override
    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    // @Override
    // @Deprecated
    virtual CARAPI SetTitleColor(
        /* [in] */ Int32 textColor);

    /**
      * Prepares the panel to either be opened or chorded. This creates the Menu
      * instance for the panel and populates it via the Activity callbacks.
      *
      * @param st The panel state to prepare.
      * @param event The event that triggered the preparing of the panel.
      * @return Whether the panel was prepared. If the panel should not be shown,
      *         returns false.
      */
    virtual CARAPI PreparePanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    // @Override
    CARAPI OpenPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    // @Override
    CARAPI ClosePanel(
        /* [in] */ Int32 featureId);

    /**
      * Closes the given panel.
      *
      * @param st The panel to be closed.
      * @param doCallback Whether to notify the callback that the panel was
      *            closed. If the panel is in the process of re-opening or
      *            opening another panel (e.g., menu opening a sub menu), the
      *            callback should not happen and this variable should be false.
      *            In addition, this method internally will only perform the
      *            callback if the panel is open.
      */
    virtual CARAPI ClosePanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ Boolean doCallback);

    virtual CARAPI CheckCloseActionMenu(
        /* [in] */ IMenu* menu);

    // @Override
    CARAPI TogglePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    // @Override
    CARAPI InvalidatePanelMenu(
        /* [in] */ Int32 featureId);

    virtual CARAPI DoPendingInvalidatePanelMenu();

    virtual CARAPI DoInvalidatePanelMenu(
        /* [in] */ Int32 featureId);

    /**
      * Called when the panel key is pushed down.
      * @param featureId The feature ID of the relevant panel (defaults to FEATURE_OPTIONS_PANEL).
      * @param event The key event.
      * @return Whether the key was handled.
      */
    virtual CARAPI OnKeyDownPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    /**
      * Called when the panel key is released.
      * @param featureId The feature ID of the relevant panel (defaults to FEATURE_OPTIONS_PANEL).
      * @param event The key event.
      */
    virtual CARAPI OnKeyUpPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event);

    // @Override
    CARAPI CloseAllPanels();

    // @Override
    CARAPI PerformPanelShortcut(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI PerformPanelIdentifierAction(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    virtual CARAPI FindMenuPanel(
        /* [in] */ IMenu* menu,
        /* [out] */ PanelFeatureState** result);

    virtual CARAPI OnMenuItemSelected(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* result);

    virtual CARAPI OnMenuModeChange(
        /* [in] */ IMenuBuilder* menu);

    virtual CARAPI OnOptionsPanelRotationChanged();

    // @Override
    CARAPI PerformContextMenuIdentifierAction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* drawable);

    // @Override
    CARAPI SetFeatureDrawableResource(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 resId);

    // @Override
    CARAPI SetFeatureDrawableUri(
        /* [in] */ Int32 featureId,
        /* [in] */ IUri* uri);

    // @Override
    CARAPI SetFeatureDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable);

    // @Override
    CARAPI SetFeatureDrawableAlpha(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 alpha);

    // @Override
    CARAPI SetFeatureInt(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value);

    // @Override
    CARAPI SetIcon(
        /* [in] */ Int32 resId);

    // @Override
    CARAPI SetDefaultIcon(
        /* [in] */ Int32 resId);

    // @Override
    CARAPI SetLogo(
        /* [in] */ Int32 resId);

    // @Override
    CARAPI SetDefaultLogo(
        /* [in] */ Int32 resId);

    // @Override
    CARAPI SetLocalFocus(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean inTouchMode);

    // @Override
    CARAPI InjectInputEvent(
        /* [in] */ IInputEvent* event);

    /**
      * Request that key events come to this activity. Use this if your activity
      * has no views with focus, but the activity still wants a chance to process
      * key events.
      */
    // @Override
    CARAPI TakeKeyEvents(
        /* [in] */ Boolean get);

    // @Override
    CARAPI SuperDispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI SuperDispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI SuperDispatchTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI SuperDispatchTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI SuperDispatchGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded);

    virtual CARAPI GetAudioManager(
        /* [out] */ IAudioManager** result);

    // @Override
    CARAPI GetDecorView(
        /* [out] */ IView** view);

    // @Override
    CARAPI PeekDecorView(
        /* [out] */ IView** view);

    /** {@inheritDoc} */
    // @Override
    CARAPI SaveHierarchyState(
        /* [out] */ IBundle** instanceState);

    /** {@inheritDoc} */
    // @Override
    CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* savedInstanceState);

    /** @hide */
    virtual CARAPI AlwaysReadCloseOnTouchAttr();

    // @Override
    CARAPI SetChildDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable);

    // @Override
    CARAPI SetChildInt(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value);

    // @Override
    CARAPI IsShortcutKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isShortcutKey);

    // @Override
    CARAPI SetVolumeControlStream(
        /* [in] */ Int32 streamType);

    // @Override
    CARAPI GetVolumeControlStream(
        /* [out] */ Int32* streamType);

    // @Override
    CARAPI SetMediaController(
        /* [in] */ IMediaController* controller);

    // @Override
    CARAPI GetMediaController(
        /* [out] */ IMediaController** controller);

    // @Override
    CARAPI SetEnterTransition(
        /* [in] */ ITransition* enterTransition);

    // @Override
    CARAPI SetReturnTransition(
        /* [in] */ ITransition* transition);

    // @Override
    CARAPI SetExitTransition(
        /* [in] */ ITransition* exitTransition);

    // @Override
    CARAPI SetReenterTransition(
        /* [in] */ ITransition* transition);

    // @Override
    CARAPI SetSharedElementEnterTransition(
        /* [in] */ ITransition* sharedElementEnterTransition);

    // @Override
    CARAPI SetSharedElementReturnTransition(
        /* [in] */ ITransition* transition);

    // @Override
    CARAPI SetSharedElementExitTransition(
        /* [in] */ ITransition* sharedElementExitTransition);

    // @Override
    CARAPI SetSharedElementReenterTransition(
        /* [in] */ ITransition* transition);

    // @Override
    CARAPI GetEnterTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI GetReturnTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI GetExitTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI GetReenterTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI GetSharedElementEnterTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI GetSharedElementReturnTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI GetSharedElementExitTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI GetSharedElementReenterTransition(
        /* [out] */ ITransition** transition);

    // @Override
    CARAPI SetAllowEnterTransitionOverlap(
        /* [in] */ Boolean allow);

    // @Override
    CARAPI GetAllowEnterTransitionOverlap(
        /* [out] */ Boolean* allow);

    // @Override
    CARAPI SetAllowReturnTransitionOverlap(
        /* [in] */ Boolean allowExitTransitionOverlap);

    // @Override
    CARAPI GetAllowReturnTransitionOverlap(
        /* [out] */ Boolean* allow);

    // @Override
    CARAPI GetTransitionBackgroundFadeDuration(
        /* [out] */ Int64* duration);

    // @Override
    CARAPI SetTransitionBackgroundFadeDuration(
        /* [in] */ Int64 fadeDurationMillis);

    // @Override
    CARAPI SetSharedElementsUseOverlay(
        /* [in] */ Boolean sharedElementsUseOverlay);

    // @Override
    CARAPI GetSharedElementsUseOverlay(
        /* [out] */ Boolean* shared);

    virtual CARAPI SendCloseSystemWindows();

    virtual CARAPI SendCloseSystemWindows(
        /* [in] */ const String& reason);

    // @Override
    CARAPI GetStatusBarColor(
        /* [out] */ Int32* result);

    // @Override
    CARAPI SetStatusBarColor(
        /* [in] */ Int32 color);

    // @Override
    CARAPI GetNavigationBarColor(
        /* [out] */ Int32* result);

    // @Override
    CARAPI SetNavigationBarColor(
        /* [in] */ Int32 color);

protected:
    /**
      * Initializes the menu associated with the given panel feature state. You
      * must at the very least set PanelFeatureState.menu to the Menu to be
      * associated with the given panel state. The default implementation creates
      * a new menu for the panel state.
      *
      * @param st The panel whose menu is being initialized.
      * @return Whether the initialization was successful.
      */
    virtual CARAPI_(Boolean) InitializePanelMenu(
        /* [in] */ PanelFeatureState* st);

    /**
      * Perform initial setup of a panel. This should at the very least set the
      * style information in the PanelFeatureState and must set
      * PanelFeatureState.decor to the panel's window decor view.
      *
      * @param st The panel being initialized.
      */
    virtual CARAPI_(Boolean) InitializePanelDecor(
        /* [in] */ PanelFeatureState* st);

    /**
      * Initializes the panel associated with the panel feature state. You must
      * at the very least set PanelFeatureState.panel to the View implementing
      * its contents. The default implementation gets the panel from the menu.
      *
      * @param st The panel state being initialized.
      * @return Whether the initialization was successful.
      */
    virtual CARAPI_(Boolean) InitializePanelContent(
        /* [in] */ PanelFeatureState* st);

    virtual CARAPI_(void) SetFeatureDefaultDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable);

    /**
      * Update the state of a drawable feature. This should be called, for every
      * drawable feature supported, as part of onActive(), to make sure that the
      * contents of a containing window is properly updated.
      *
      * @see #onActive
      * @param featureId The desired drawable feature to change.
      * @param fromActive Always true when called from onActive().
      */
    virtual CARAPI_(void) UpdateDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean fromActive);

    /**
      * Called when a Drawable feature changes, for the window to update its
      * graphics.
      *
      * @param featureId The feature being changed.
      * @param drawable The new Drawable to show, or null if none.
      * @param alpha The new alpha blending of the Drawable.
      */
    virtual CARAPI_(void) OnDrawableChanged(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable,
        /* [in] */ Int32 alpha);

    /**
      * Called when an int feature changes, for the window to update its
      * graphics.
      *
      * @param featureId The feature being changed.
      * @param value The new integer value.
      */
    virtual CARAPI_(void) OnIntChanged(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value);

    /**
      * A key was pressed down and not handled by anything else in the window.
      *
      * @see #onKeyUp
      * @see android.view.KeyEvent
      */
    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
      * A key was released and not handled by anything else in the window.
      *
      * @see #onKeyDown
      * @see android.view.KeyEvent
      */
    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    // @Override
    CARAPI_(void) OnActive();

    virtual CARAPI_(AutoPtr<DecorView>) GenerateDecor();

    virtual CARAPI_(void) SetFeatureFromAttrs(
        /* [in] */ Int32 featureId,
        /* [in] */ ITypedArray* attrs,
        /* [in] */ Int32 drawableAttr,
        /* [in] */ Int32 alphaAttr);

    virtual CARAPI_(AutoPtr<IViewGroup>) GenerateLayout(
        /* [in] */ DecorView* decor);

    // @Override
    CARAPI_(void) DispatchWindowAttributesChanged(
        /* [in] */ IWindowManagerLayoutParams* attrs);

private:
    //TODO maybe remove this, check the definition for this in view/window.car
    CARAPI DispatchOnWindowDismissed(
        /* [in] */ IOnWindowDismissedCallback* cb);

    CARAPI_(void) TransitionTo(
        /* [in] */ IScene* scene);

    static CARAPI_(void) ClearMenuViews(
        /* [in] */ PanelFeatureState* st);

    CARAPI_(void) OpenPanel(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ IKeyEvent* event);

    /**
      * Closes the context menu. This notifies the menu logic of the close, along
      * with dismissing it from the UI.
      */
    // synchronized
    CARAPI_(void) CloseContextMenu();

    /**
      * Dismisses just the context menu UI. To close the context menu, use
      * {@link #closeContextMenu()}.
      */
    // synchronized
    CARAPI_(void) DismissContextMenu();

    CARAPI_(Boolean) PerformPanelShortcut(
        /* [in] */ PanelFeatureState* st,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags);

    CARAPI_(void) ReopenMenu(
        /* [in] */ Boolean toggleMenuMode);

    /**
      * Determine the gravity value for the options panel. This can
      * differ in compact mode.
      *
      * @return gravity value to use for the panel window
      */
    CARAPI_(Int32) GetOptionsPanelGravity();

    /**
      * Updates the progress bars that are shown in the title bar.
      *
      * @param value Can be one of {@link Window#PROGRESS_VISIBILITY_ON},
      *            {@link Window#PROGRESS_VISIBILITY_OFF},
      *            {@link Window#PROGRESS_INDETERMINATE_ON},
      *            {@link Window#PROGRESS_INDETERMINATE_OFF}, or a value
      *            starting at {@link Window#PROGRESS_START} through
      *            {@link Window#PROGRESS_END} for setting the default
      *            progress (if {@link Window#PROGRESS_END} is given,
      *            the progress bar widgets in the title will be hidden after an
      *            animation), a value between
      *            {@link Window#PROGRESS_SECONDARY_START} -
      *            {@link Window#PROGRESS_SECONDARY_END} for the
      *            secondary progress (if
      *            {@link Window#PROGRESS_SECONDARY_END} is given, the
      *            progress bar widgets will still be shown with the secondary
      *            progress bar will be completely filled in.)
      */
    CARAPI_(void) UpdateProgressBars(
        /* [in] */ Int32 value);

    CARAPI_(void) ShowProgressBars(
        /* [in] */ IProgressBar* horizontalProgressBar,
        /* [in] */ IProgressBar* spinnyProgressBar);

    CARAPI_(void) HideProgressBars(
        /* [in] */ IProgressBar* horizontalProgressBar,
        /* [in] */ IProgressBar* spinnyProgressBar);

    CARAPI_(AutoPtr<IViewRootImpl>) GetViewRootImpl();

    CARAPI_(AutoPtr<IKeyguardManager>) GetKeyguardManager();

    /**
      * Invoked when the panels should freeze their state.
      *
      * @param icicles Save state into this. This is usually indexed by the
      *            featureId. This will be given to {@link #restorePanelState} in the
      *            future.
      */
    CARAPI_(void) SavePanelState(
        /* [in] */ ISparseArray* icicles);//IParcelable

    /**
      * Invoked when the panels should thaw their state from a previously frozen state.
      *
      * @param icicles The state saved by {@link #savePanelState} that needs to be thawed.
      */
    CARAPI_(void) RestorePanelState(
        /* [in] */ ISparseArray* icicles);//IParcelable

    /**
      * Opens the panels that have had their state restored. This should be
      * called sometime after {@link #restorePanelState} when it is safe to add
      * to the window manager.
      */
    CARAPI_(void) OpenPanelsAfterRestore();

    CARAPI_(void) InstallDecor();

    CARAPI_(AutoPtr<ITransition>) GetTransition(
        /* [in] */ ITransition* currentValue,
        /* [in] */ ITransition* defaultValue,
        /* [in] */ Int32 id);

    CARAPI_(AutoPtr<IDrawable>) LoadImageURI(
        /* [in] */ IUri* uri);

    CARAPI_(AutoPtr<DrawableFeatureState>) GetDrawableState(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean required);

    /**
      * Gets a panel's state based on its feature ID.
      *
      * @param featureId The feature ID of the panel.
      * @param required Whether the panel is required (if it is required and it
      *            isn't in our features, this throws an exception).
      * @return The panel state.
      */
    CARAPI_(AutoPtr<PanelFeatureState>) GetPanelState(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean required);

    /**
      * Gets a panel's state based on its feature ID.
      *
      * @param featureId The feature ID of the panel.
      * @param required Whether the panel is required (if it is required and it
      *            isn't in our features, this throws an exception).
      * @param convertPanelState Optional: If the panel state does not exist, use
      *            this as the panel state.
      * @return The panel state.
      */
    CARAPI_(AutoPtr<PanelFeatureState>) GetPanelState(
        /* [in] */ Int32 featureId,
        /* [in] */ Boolean required,
        /* [in] */ PanelFeatureState* convertPanelState);

    CARAPI_(void) UpdateDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ DrawableFeatureState* st,
        /* [in] */ Boolean fromResume);

    CARAPI_(void) UpdateInt(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value,
        /* [in] */ Boolean fromResume);

    CARAPI_(AutoPtr<IImageView>) GetLeftIconView();

    CARAPI_(AutoPtr<IProgressBar>) GetCircularProgressBar(
        /* [in] */ Boolean shouldInstallDecor);

    CARAPI_(AutoPtr<IProgressBar>) GetHorizontalProgressBar(
        /* [in] */ Boolean shouldInstallDecor);

    CARAPI_(AutoPtr<IImageView>) GetRightIconView();

    CARAPI_(void) RegisterSwipeCallbacks();

    /**
      * Helper method for calling the {@link Callback#onPanelClosed(int, Menu)}
      * callback. This method will grab whatever extra state is needed for the
      * callback that isn't given in the parameters. If the panel is not open,
      * this will not perform the callback.
      *
      * @param featureId Feature ID of the panel that was closed. Must be given.
      * @param panel Panel that was closed. Optional but useful if there is no
      *            menu given.
      * @param menu The menu that was closed. Optional, but give if you have.
      */
    CARAPI_(void) CallOnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ PanelFeatureState* panel,
        /* [in] */ IMenu* menu);

    /**
      * Helper method for adding launch-search to most applications. Opens the
      * search window using default settings.
      *
      * @return true if search window opened
      */
    CARAPI_(Boolean) LaunchDefaultSearch();

    CARAPI_(Boolean) IsTranslucent();

public:
    /**
      * Simple callback used by the context menu and its submenus. The options
      * menu submenus do not use this (their behavior is more complex).
      */
    /*const*/ AutoPtr<DialogMenuCallback> mContextMenuCallback;
    /*const*/ AutoPtr<ITypedValue> mMinWidthMajor;
    /*const*/ AutoPtr<ITypedValue> mMinWidthMinor;
    AutoPtr<ITypedValue> mFixedWidthMajor;
    AutoPtr<ITypedValue> mFixedWidthMinor;
    AutoPtr<ITypedValue> mFixedHeightMajor;
    AutoPtr<ITypedValue> mFixedHeightMinor;
    AutoPtr<ITypedValue> mOutsetBottom;
    AutoPtr<ISurfaceHolderCallback2> mTakeSurfaceCallback;
    AutoPtr<IInputQueueCallback> mTakeInputQueueCallback;
    // The icon resource has been explicitly set elsewhere
    // and should not be overwritten with a default.
    static const Int32 FLAG_RESOURCE_SET_ICON = 1 << 0;
    // The logo resource has been explicitly set elsewhere
    // and should not be overwritten with a default.
    static const Int32 FLAG_RESOURCE_SET_LOGO = 1 << 1;
    // The icon resource is currently configured to use the system fallback
    // as no default was previously specified. Anything can override this.
    static const Int32 FLAG_RESOURCE_SET_ICON_FALLBACK = 1 << 2;
    Int32 mResourcesSetFlags;
    Int32 mIconRes;
    Int32 mLogoRes;
    static AutoPtr<IPhoneWindowRotationWatcher> sRotationWatcher;

private:
    static const String TAG;
    static const Boolean SWEEP_OPEN_MENU;
    static const Int32 DEFAULT_BACKGROUND_FADE_DURATION_MS = 300;
    static const Int32 CUSTOM_TITLE_COMPATIBLE_FEATURES;
    static AutoPtr<ITransition> USE_DEFAULT_TRANSITION;
    // This is the top-level view of the window, containing the window decor.
    AutoPtr<DecorView> mDecor;
    // This is the view in which the window contents are placed. It is either
    // mDecor itself, or a child of mDecor where the contents go.
    AutoPtr<IViewGroup> mContentParent;
    AutoPtr<IViewGroup> mContentRoot;
    Boolean mIsFloating;
    AutoPtr<ILayoutInflater> mLayoutInflater;
    AutoPtr<ITextView> mTitleView;
    AutoPtr<IDecorContentParent> mDecorContentParent;
    AutoPtr<ActionMenuPresenterCallback> mActionMenuPresenterCallback;
    AutoPtr<PanelMenuPresenterCallback> mPanelMenuPresenterCallback;
    AutoPtr<ITransitionManager> mTransitionManager;
    AutoPtr<IScene> mContentScene;
    AutoPtr<ArrayOf<DrawableFeatureState> > mDrawables;
    AutoPtr<ArrayOf<PanelFeatureState> > mPanels;
    /**
      * The panel that is prepared or opened (the most recent one if there are
      * multiple panels). Shortcuts will go to this panel. It gets set in
      * {@link #preparePanel} and cleared in {@link #closePanel}.
      */
    AutoPtr<PanelFeatureState> mPreparedPanel;
    /**
      * The keycode that is currently held down (as a modifier) for chording. If
      * this is 0, there is no key held down.
      */
    Int32 mPanelChordingKey;
    AutoPtr<IImageView> mLeftIconView;
    AutoPtr<IImageView> mRightIconView;
    AutoPtr<IProgressBar> mCircularProgressBar;
    AutoPtr<IProgressBar> mHorizontalProgressBar;
    Int32 mBackgroundResource;
    Int32 mBackgroundFallbackResource;
    AutoPtr<IDrawable> mBackgroundDrawable;
    Float mElevation;
    /** Whether window content should be clipped to the background outline. */
    Boolean mClipToOutline;
    Int32 mFrameResource;
    Int32 mTextColor;
    Int32 mStatusBarColor;
    Int32 mNavigationBarColor;
    Boolean mForcedStatusBarColor;
    Boolean mForcedNavigationBarColor;
    AutoPtr<ICharSequence> mTitle;
    Int32 mTitleColor;
    Boolean mAlwaysReadCloseOnTouchAttr;
    AutoPtr<IContextMenuBuilder> mContextMenu;
    AutoPtr<IMenuDialogHelper> mContextMenuHelper;
    Boolean mClosingActionMenu;
    Int32 mVolumeControlStreamType;
    AutoPtr<IMediaController> mMediaController;
    AutoPtr<IAudioManager> mAudioManager;
    AutoPtr<IKeyguardManager> mKeyguardManager;
    Int32 mUiOptions;
    Boolean mInvalidatePanelMenuPosted;
    Int32 mInvalidatePanelMenuFeatures;
    /*const*/ AutoPtr<IRunnable> mInvalidatePanelMenuRunnable;
    AutoPtr<ITransition> mEnterTransition;
    AutoPtr<ITransition> mReturnTransition;
    AutoPtr<ITransition> mExitTransition;
    AutoPtr<ITransition> mReenterTransition;
    AutoPtr<ITransition> mSharedElementEnterTransition;
    AutoPtr<ITransition> mSharedElementReturnTransition;
    AutoPtr<ITransition> mSharedElementExitTransition;
    AutoPtr<ITransition> mSharedElementReenterTransition;
    Boolean mAllowReturnTransitionOverlap;
    Boolean mAllowEnterTransitionOverlap;
    Int64 mBackgroundFadeDurationMillis;
    Boolean mSharedElementsUseOverlay;
    AutoPtr<IRect> mTempRect;
    static const String FOCUSED_ID_TAG;
    static const String VIEWS_TAG;
    static const String PANELS_TAG;
    static const String ACTION_BAR_TAG;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEWINDOW_H__

