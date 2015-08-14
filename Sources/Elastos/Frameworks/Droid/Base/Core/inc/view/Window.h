
#ifndef __ELASTOS_DROID_VIEW_WINDOW_H__
#define __ELASTOS_DROID_VIEW_WINDOW_H__

#include "Elastos.Droid.Core_server.h"
#include "view/CWindowManagerLayoutParams.h"

using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace View {

extern "C" const InterfaceID EIID_Window;

class Window : public IWindow
{
    friend class CWindowManagerGlobal;

public:
    Window();

    Window(
        /* [in] */ IContext* context);

    virtual ~Window();

    /**
     * Return the Context this window policy is running in, for retrieving
     * resources and other information.
     *
     * @return Context The Context that was supplied to the constructor.
     */
    CARAPI GetContext(
        /* [out] */ IContext** context);

    /**
     * Return the {@link android.R.styleable#Window} attributes from this
     * window's theme.
     */
    CARAPI GetWindowStyle(
        /* [out] */ ITypedArray** attrs);

    /**
     * Set the container for this window.  If not set, the DecorWindow
     * operates as a top-level window; otherwise, it negotiates with the
     * container to display itself appropriately.
     *
     * @param container The desired containing Window.
     */
    CARAPI SetContainer(
        /* [in] */ IWindow* container);

    /**
     * Return the container for this Window.
     *
     * @return Window The containing window, or null if this is a
     *         top-level window.
     */
    CARAPI GetContainer(
        /* [out] */ IWindow** container);

    CARAPI HasChildren(
        /* [out] */  Boolean* hasChildren);

    /** @hide */
    CARAPI Destroy();

    /** @hide */
    CARAPI IsDestroyed(
        /* [out] */ Boolean* isDestroyed);

    /**
     * Set the window manager for use by this Window to, for example,
     * display panels.  This is <em>not</em> used for displaying the
     * Window itself -- that must be done by the client.
     *
     * @param wm The ViewManager for adding new windows.
     */
    CARAPI SetWindowManager(
        /* [in] */ IWindowManager* wm,
        /* [in] */ IBinder* appToken,
        /* [in] */ const String& appName);

    /**
     * Set the window manager for use by this Window to, for example,
     * display panels.  This is <em>not</em> used for displaying the
     * Window itself -- that must be done by the client.
     *
     * @param wm The ViewManager for adding new windows.
     */
    CARAPI SetWindowManager(
        /* [in] */ IWindowManager* wm,
        /* [in] */ IBinder* appToken,
        /* [in] */ const String& appName,
        /* [in] */ Boolean hardwareAccelerated);

    /**
     * Return the window manager allowing this Window to display its own
     * windows.
     *
     * @return WindowManager The ViewManager.
     */
    CARAPI GetWindowManager(
        /* [out] */ IWindowManager** wm);

    CARAPI SetCallback(
        /* [in] */ IWindowCallback* cb);

    CARAPI GetCallback(
        /* [out] */ IWindowCallback** cb);

    /**
     * Take ownership of this window's surface.  The window's view hierarchy
     * will no longer draw into the surface, though it will otherwise continue
     * to operate (such as for receiving input events).  The given SurfaceHolder
     * callback will be used to tell you about state changes to the surface.
     */
    CARAPI TakeSurface(
        /* [in] */ ISurfaceHolderCallback2* cb) = 0;

    /**
     * Take ownership of this window's InputQueue.  The window will no
     * longer read and dispatch input events from the queue; it is your
     * responsibility to do so.
     */
    CARAPI TakeInputQueue(
        /* [in] */ IInputQueueCallback* cb) = 0;

    /**
     * Return whether this window is being displayed with a floating style
     * (based on the {@link android.R.attr#windowIsFloating} attribute in
     * the style/theme).
     *
     * @return Returns true if the window is configured to be displayed floating
     * on top of whatever is behind it.
     */
    CARAPI IsFloating(
        /* [out] */ Boolean* isFloating) = 0;

    /**
     * Set the width and height layout parameters of the window.  The default
     * for both of these is MATCH_PARENT; you can change them to WRAP_CONTENT to
     * make a window that is not full-screen.
     *
     * @param width The desired layout width of the window.
     * @param height The desired layout height of the window.
     */
    CARAPI SetLayout(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Set the gravity of the window, as per the Gravity constants.  This
     * controls how the window manager is positioned in the overall window; it
     * is only useful when using WRAP_CONTENT for the layout width or height.
     *
     * @param gravity The desired gravity constant.
     *
     * @see Gravity
     * @see #setLayout
     */
    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    /**
     * Set the type of the window, as per the WindowManager.LayoutParams
     * types.
     *
     * @param type The new window type (see WindowManager.LayoutParams).
     */
    CARAPI SetType(
        /* [in] */ Int32 type);

    /**
     * Set the format of window, as per the PixelFormat types.  This overrides
     * the default format that is selected by the Window based on its
     * window decorations.
     *
     * @param format The new window format (see PixelFormat).  Use
     *               PixelFormat.UNKNOWN to allow the Window to select
     *               the format.
     *
     * @see PixelFormat
     */
    CARAPI SetFormat(
        /* [in] */ Int32 format);

    /**
     * Specify custom animations to use for the window, as per
     * {@link WindowManager.LayoutParams#windowAnimations
     * WindowManager.LayoutParams.windowAnimations}.  Providing anything besides
     * 0 here will override the animations the window would
     * normally retrieve from its theme.
     */
    CARAPI SetWindowAnimations(
        /* [in] */ Int32 resId);

    /**
     * Specify an explicit soft input mode to use for the window, as per
     * {@link WindowManager.LayoutParams#softInputMode
     * WindowManager.LayoutParams.softInputMode}.  Providing anything besides
     * "unspecified" here will override the input mode the window would
     * normally retrieve from its theme.
     */
    CARAPI SetSoftInputMode(
        /* [in] */ Int32 mode);

    /**
     * Convenience function to set the flag bits as specified in flags, as
     * per {@link #setFlags}.
     * @param flags The flag bits to be set.
     * @see #setFlags
     */
    CARAPI AddFlags(
        /* [in] */ Int32 flags);

    /**
     * Convenience function to clear the flag bits as specified in flags, as
     * per {@link #setFlags}.
     * @param flags The flag bits to be cleared.
     * @see #setFlags
     */
    CARAPI ClearFlags(
        /* [in] */ Int32 flags);

    /**
     * Set the flags of the window, as per the
     * {@link WindowManager.LayoutParams WindowManager.LayoutParams}
     * flags.
     *
     * <p>Note that some flags must be set before the window decoration is
     * created (by the first call to
     * {@link #setContentView(View, android.view.ViewGroup.LayoutParams)} or
     * {@link #getDecorView()}:
     * {@link WindowManager.LayoutParams#FLAG_LAYOUT_IN_SCREEN} and
     * {@link WindowManager.LayoutParams#FLAG_LAYOUT_INSET_DECOR}.  These
     * will be set for you based on the {@link android.R.attr#windowIsFloating}
     * attribute.
     *
     * @param flags The new window flags (see WindowManager.LayoutParams).
     * @param mask Which of the window flag bits to modify.
     */
    CARAPI SetFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    /**
     * Set the amount of dim behind the window when using
     * {@link WindowManager.LayoutParams#FLAG_DIM_BEHIND}.  This overrides
     * the default dim amount of that is selected by the Window based on
     * its theme.
     *
     * @param amount The new dim amount, from 0 for no dim to 1 for full dim.
     */
    CARAPI SetDimAmount(
        /* [in] */ Float amount);

    /**
     * Specify custom window attributes.  <strong>PLEASE NOTE:</strong> the
     * layout params you give here should generally be from values previously
     * retrieved with {@link #getAttributes()}; you probably do not want to
     * blindly create and apply your own, since this will blow away any values
     * set by the framework that you are not interested in.
     *
     * @param a The new window attributes, which will completely override any
     *          current values.
     */
    CARAPI SetAttributes(
        /* [in] */ IWindowManagerLayoutParams* a);

    CARAPI GetAttributes(
        /* [out] */ IWindowManagerLayoutParams** params);

    /** @hide */
    CARAPI SetCloseOnTouchOutside(
        /* [in] */ Boolean close);

    /** @hide */
    CARAPI SetCloseOnTouchOutsideIfNotSet(
        /* [in] */ Boolean close);

    /** @hide */
    CARAPI AlwaysReadCloseOnTouchAttr() = 0;

    /** @hide */
    CARAPI ShouldCloseOnTouch(
        /* [in] */ IContext* context,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

    /**
     * Enable extended screen features.  This must be called before
     * setContentView().  May be called as many times as desired as long as it
     * is before setContentView().  If not called, no extended features
     * will be available.  You can not turn off a feature once it is requested.
     * You canot use other title features with {@link #FEATURE_CUSTOM_TITLE}.
     *
     * @param featureId The desired features, defined as constants by Window.
     * @return The features that are now set.
     */
    CARAPI RequestFeature(
        /* [in] */ Int32 featureId,
        /* [out] */ Boolean* result);

    CARAPI MakeActive();

    CARAPI IsActive(
        /* [out] */ Boolean* isActive);

    CARAPI FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** view);

    CARAPI SetContentView(
        /* [in] */ Int32 layoutResID) = 0;

    CARAPI SetContentView(
        /* [in] */ IView* view) = 0;

    CARAPI SetContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params) = 0;

    /**
     * Variation on
     * {@link #setContentView(View, android.view.ViewGroup.LayoutParams)}
     * to add an additional content view to the screen.  Added after any existing
     * ones in the screen -- existing views are NOT removed.
     *
     * @param view The desired content to display.
     * @param params Layout parameters for the view.
     */
    CARAPI AddContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params) = 0;

    /**
     * Return the view in this Window that currently has focus, or null if
     * there are none.  Note that this does not look in any containing
     * Window.
     *
     * @return View The current View with focus or null.
     */
    CARAPI GetCurrentFocus(
        /* [out] */ IView** view) = 0;

    /**
     * Quick access to the {@link LayoutInflater} instance that this Window
     * retrieved from its Context.
     *
     * @return LayoutInflater The shared LayoutInflater.
     */
    CARAPI GetLayoutInflater(
        /* [out] */ ILayoutInflater** inflater) = 0;

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title) = 0;

    CARAPI SetTitleColor(
        /* [in] */ Int32 textColor) = 0;

    CARAPI OpenPanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event) = 0;

    CARAPI ClosePanel(
        /* [in] */ Int32 featureId) = 0;

    CARAPI TogglePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IKeyEvent* event) = 0;

    CARAPI InvalidatePanelMenu(
        /* [in] */ Int32 featureId) = 0;

    CARAPI PerformPanelShortcut(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded) = 0;

    CARAPI PerformPanelIdentifierAction(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded) = 0;

    CARAPI CloseAllPanels() = 0;

    CARAPI PerformContextMenuIdentifierAction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded) = 0;

    /**
     * Should be called when the configuration is changed.
     *
     * @param newConfig The new configuration.
     */
    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig) = 0;

    /**
     * Change the background of this window to a Drawable resource. Setting the
     * background to null will make the window be opaque. To make the window
     * transparent, you can use an empty drawable (for instance a ColorDrawable
     * with the color 0 or the system drawable android:drawable/empty.)
     *
     * @param resid The resource identifier of a drawable resource which will be
     *              installed as the new background.
     */
    CARAPI SetBackgroundDrawableResource(
        /* [in] */ Int32 resid);

    /**
     * Change the background of this window to a custom Drawable. Setting the
     * background to null will make the window be opaque. To make the window
     * transparent, you can use an empty drawable (for instance a ColorDrawable
     * with the color 0 or the system drawable android:drawable/empty.)
     *
     * @param drawable The new Drawable to use for this window's background.
     */
    CARAPI SetBackgroundDrawable(
        /* [in] */ IDrawable* drawable) = 0;

    /**
     * Set the value for a drawable feature of this window, from a resource
     * identifier.  You must have called requestFeauture(featureId) before
     * calling this function.
     *
     * @see android.content.res.Resources#getDrawable(int)
     *
     * @param featureId The desired drawable feature to change, defined as a
     * constant by Window.
     * @param resId Resource identifier of the desired image.
     */
    CARAPI SetFeatureDrawableResource(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 resId) = 0;

    /**
     * Set the value for a drawable feature of this window, from a URI. You
     * must have called requestFeature(featureId) before calling this
     * function.
     *
     * <p>The only URI currently supported is "content:", specifying an image
     * in a content provider.
     *
     * @see android.widget.ImageView#setImageURI
     *
     * @param featureId The desired drawable feature to change. Features are
     * constants defined by Window.
     * @param uri The desired URI.
     */
    CARAPI SetFeatureDrawableUri(
        /* [in] */ Int32 featureId,
        /* [in] */ IUri* uri) = 0;

    /**
     * Set an explicit Drawable value for feature of this window. You must
     * have called requestFeature(featureId) before calling this function.
     *
     * @param featureId The desired drawable feature to change.
     * Features are constants defined by Window.
     * @param drawable A Drawable object to display.
     */
    CARAPI SetFeatureDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable) = 0;

    /**
     * Set a custom alpha value for the given drawale feature, controlling how
     * much the background is visible through it.
     *
     * @param featureId The desired drawable feature to change.
     * Features are constants defined by Window.
     * @param alpha The alpha amount, 0 is completely transparent and 255 is
     *              completely opaque.
     */
    CARAPI SetFeatureDrawableAlpha(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 alpha) = 0;

    /**
     * Set the integer value for a feature.  The range of the value depends on
     * the feature being set.  For FEATURE_PROGRESSS, it should go from 0 to
     * 10000. At 10000 the progress is complete and the indicator hidden.
     *
     * @param featureId The desired feature to change.
     * Features are constants defined by Window.
     * @param value The value for the feature.  The interpretation of this
     *              value is feature-specific.
     */
    CARAPI SetFeatureInt(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value) = 0;

    /**
     * Request that key events come to this activity. Use this if your
     * activity has no views with focus, but the activity still wants
     * a chance to process key events.
     */
    CARAPI TakeKeyEvents(
        /* [in] */ Boolean get) = 0;

    /**
     * Used by custom windows, such as Dialog, to pass the key press event
     * further down the view hierarchy. Application developers should
     * not need to implement or call this.
     *
     */
    CARAPI SuperDispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* succeeded) = 0;

    /**
     * Used by custom windows, such as Dialog, to pass the key shortcut press event
     * further down the view hierarchy. Application developers should
     * not need to implement or call this.
     *
     */
    CARAPI SuperDispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* succeeded) = 0;

    /**
     * Used by custom windows, such as Dialog, to pass the touch screen event
     * further down the view hierarchy. Application developers should
     * not need to implement or call this.
     *
     */
    CARAPI SuperDispatchTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded) = 0;

    /**
     * Used by custom windows, such as Dialog, to pass the trackball event
     * further down the view hierarchy. Application developers should
     * not need to implement or call this.
     *
     */
    CARAPI SuperDispatchTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded) = 0;

    /**
     * Used by custom windows, such as Dialog, to pass the generic motion event
     * further down the view hierarchy. Application developers should
     * not need to implement or call this.
     *
     */
    CARAPI SuperDispatchGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* succeeded) = 0;

    CARAPI GetDecorView(
        /* [out] */ IView** view) = 0;

    CARAPI PeekDecorView(
        /* [out] */ IView** view) = 0;

    CARAPI SaveHierarchyState(
        /* [out] */ IBundle** instanceState) = 0;

    CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* savedInstanceState) = 0;

    /**
     * Query for the availability of a certain feature.
     *
     * @param feature The feature ID to check
     * @return true if the feature is enabled, false otherwise.
     */
    CARAPI HasFeature(
        /* [in] */ Int32 feature,
        /* [out] */ Boolean* hasFeature);

    CARAPI SetChildDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable) = 0;

    CARAPI SetChildInt(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 value) = 0;

    /**
     * Is a keypress one of the defined shortcut keys for this window.
     * @param keyCode the key code from {@link android.view.KeyEvent} to check.
     * @param event the {@link android.view.KeyEvent} to use to help check.
     */
    CARAPI IsShortcutKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isShortcutKey) = 0;

    /**
     * @see android.app.Activity#setVolumeControlStream(int)
     */
    CARAPI SetVolumeControlStream(
        /* [in] */ Int32 streamType) = 0;

    /**
     * @see android.app.Activity#getVolumeControlStream()
     */
    CARAPI GetVolumeControlStream(
        /* [out] */ Int32* streamType) = 0;

    /**
     * Set extra options that will influence the UI for this window.
     * @param uiOptions Flags specifying extra options for this window.
     */
    CARAPI SetUiOptions(
        /* [in] */ Int32 uiOptions);

    /**
     * Set extra options that will influence the UI for this window.
     * Only the bits filtered by mask will be modified.
     * @param uiOptions Flags specifying extra options for this window.
     * @param mask Flags specifying which options should be modified. Others will remain unchanged.
     */
    CARAPI SetUiOptions(
        /* [in] */ Int32 uiOptions,
        /* [in] */ Int32 mask);

    CARAPI AdjustLayoutParamsForSubWindow(
        /* [in] */ IWindowManagerLayoutParams* wp);

protected:
    static const Int32 DEFAULT_FEATURES;

private:
    static const String PROPERTY_HARDWARE_UI;

protected:
    /**
     * Return the window flags that have been explicitly set by the client,
     * so will not be modified by {@link #getDecorView}.
     */
    CARAPI_(Int32) GetForcedWindowFlags();

    /**
     * Has the app specified their own soft input mode?
     */
    CARAPI_(Boolean) HasSoftInputMode();

    /**
     * @hide Used internally to help resolve conflicting features.
     */
    virtual CARAPI_(void) RemoveFeature(
        /* [in] */ Int32 featureId);

    virtual CARAPI_(void) OnActive() = 0;

    /**
     * Return the feature bits that are enabled.  This is the set of features
     * that were given to requestFeature(), and are being handled by this
     * Window itself or its container.  That is, it is the set of
     * requested features that you can actually use.
     *
     * <p>To do: add a public version of this API that allows you to check for
     * features by their feature ID.
     *
     * @return int The feature bits.
     */
    CARAPI_(Int32) GetFeatures();

    /**
     * Return the feature bits that are being implemented by this Window.
     * This is the set of features that were given to requestFeature(), and are
     * being handled by only this Window itself, not by its containers.
     *
     * @return int The feature bits.
     */
    CARAPI_(Int32) GetLocalFeatures();

    /**
     * Set the default format of window, as per the PixelFormat types.  This
     * is the format that will be used unless the client specifies in explicit
     * format with setFormat();
     *
     * @param format The new window format (see PixelFormat).
     *
     * @see #setFormat
     * @see PixelFormat
     */
    virtual CARAPI_(void) SetDefaultWindowFormat(
        /* [in] */ Int32 format);

    /** @hide */
    virtual CARAPI_(Boolean) HaveDimAmount();

    virtual CARAPI_(Mutex&) GetSelfSyncLock() = 0;

private:
    Boolean IsOutOfBounds(
        /* [in] */ IContext* context,
        /* [in] */ IMotionEvent* event);

protected:
    AutoPtr<ITypedArray> mWindowStyle;
    AutoPtr<IWindowManager> mWindowManager;
    AutoPtr<IBinder> mAppToken;
    String mAppName;
    AutoPtr<Window> mContainer;
    AutoPtr<Window> mActiveChild;

    Int32 mForcedWindowFlags;

    Int32 mFeatures;
    Int32 mLocalFeatures;
    Int32 mDefaultWindowFormat;

    // The current window attributes.
    AutoPtr<CWindowManagerLayoutParams> mWindowAttributes;

    IWindowCallback* mCallback;

    // if mContext is IActiviy, this must not have it's reference
    //
    IContext* mContext;
    Boolean mRetainContext;

    Boolean mHaveWindowFormat;
    Boolean mHaveDimAmount;
    Boolean mHasSoftInputMode;
    Boolean mDestroyed;

    Boolean mHardwareAccelerated;
    Boolean mIsActive;
    Boolean mHasChildren;
    Boolean mCloseOnTouchOutside;
    Boolean mSetCloseOnTouchOutside;
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_WINDOW_H__
