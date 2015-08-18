
#ifndef __ELASTOS_DROID_SERVICE_DREAMS_CDREAMSERVICE_H__
#define __ELASTOS_DROID_SERVICE_DREAMS_CDREAMSERVICE_H__

#include "_Elastos_Droid_Service_Dreams_CDreamService.h"
#include <ext/frameworkext.h>


namespace Elastos {
namespace Droid {
namespace Service {
namespace Dreams {

/**
 * Extend this class to implement a custom Dream (displayed to the user as a "Sleep Mode").
 *
 * <p>Dreams are interactive screensavers launched when a charging device is idle, or docked in a
 * desk dock. Dreams provide another modality for apps to express themselves, tailored for
 * an exhibition/lean-back experience.</p>
 *
 * <p>The Dream lifecycle is as follows:</p>
 * <ol>
 *   <li>{@link #onAttachedToWindow}
 *     <p>Use this for initial setup, such as calling {@link #setContentView setContentView()}.</li>
 *   <li>{@link #onDreamingStarted}
 *     <p>Your dream has started, so you should begin animations or other behaviors here.</li>
 *   <li>{@link #onDreamingStopped}
 *     <p>Use this to stop the things you started in {@link #onDreamingStarted}.</li>
 *   <li>{@link #onDetachedFromWindow}
 *     <p>Use this to dismantle resources your dream set up. For example, detach from handlers
 *        and listeners.</li>
 * </ol>
 *
 * <p>In addition, onCreate and onDestroy (from the Service interface) will also be called, but
 * initialization and teardown should be done by overriding the hooks above.</p>
 *
 * <p>To be available to the system, Dreams should be declared in the manifest as follows:</p>
 * <pre>
 * &lt;service
 *     android:name=".MyDream"
 *     android:exported="true"
 *     android:icon="@drawable/my_icon"
 *     android:label="@string/my_dream_label" >
 *
 *     &lt;intent-filter>
 *         &lt;action android:name="android.service.dreams.DreamService" />
 *         &lt;category android:name="android.intent.category.DEFAULT" />
 *     &lt;/intent-filter>
 *
 *     &lt;!-- Point to additional information for this dream (optional) -->
 *     &lt;meta-data
 *         android:name="android.service.dream"
 *         android:resource="@xml/my_dream" />
 * &lt;/service>
 * </pre>
 *
 * <p>If specified with the {@code &lt;meta-data&gt;} element,
 * additional information for the dream is defined using the
 * {@link android.R.styleable#Dream &lt;dream&gt;} element in a separate XML file.
 * Currently, the only addtional
 * information you can provide is for a settings activity that allows the user to configure
 * the dream behavior. For example:</p>
 * <p class="code-caption">res/xml/my_dream.xml</p>
 * <pre>
 * &lt;dream xmlns:android="http://schemas.android.com/apk/res/android"
 *     android:settingsActivity="com.example.app/.MyDreamSettingsActivity" />
 * </pre>
 * <p>This makes a Settings button available alongside your dream's listing in the
 * system settings, which when pressed opens the specified activity.</p>
 *
 *
 * <p>To specify your dream layout, call {@link #setContentView}, typically during the
 * {@link #onAttachedToWindow} callback. For example:</p>
 * <pre>
 * public class MyDream extends DreamService {
 *
 *     &#64;Override
 *     public void onAttachedToWindow() {
 *         super.onAttachedToWindow();
 *
 *         // Exit dream upon user touch
 *         setInteractive(false);
 *         // Hide system UI
 *         setFullscreen(true);
 *         // Set the dream layout
 *         setContentView(R.layout.dream);
 *     }
 * }
 * </pre>
 */
CarClass(CDreamService)
{
public:
    /**
     * @hide
     */
    CARAPI SetDebug(
        /* [in] */ Boolean dbg);

    // begin Window.Callback methods
    /** {@inheritDoc} */
    CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI DispatchKeyShortcutEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI DispatchTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI DispatchTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI DispatchGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI OnCreatePanelView(
        /* [in] */ Int32 featureId,
        /* [out] */ IView** result);

    /** {@inheritDoc} */
    CARAPI OnCreatePanelMenu(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI OnPreparePanel(
        /* [in] */ Int32 featureId,
        /* [in] */ IView* view,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI OnMenuOpened(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI OnMenuItemSelected(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI OnWindowAttributesChanged(
        /* [in] */ ILayoutParams* attrs);

    /** {@inheritDoc} */
    CARAPI OnContentChanged();

    /** {@inheritDoc} */
    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    /** {@inheritDoc} */
    CARAPI OnAttachedToWindow();

    /** {@inheritDoc} */
    CARAPI OnDetachedFromWindow();

    /** {@inheritDoc} */
    CARAPI OnPanelClosed(
        /* [in] */ Int32 featureId,
        /* [in] */ IMenu* menu);

    /** {@inheritDoc} */
    CARAPI OnSearchRequested(
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    CARAPI OnWindowStartingActionMode(
        /* [in] */ ActionMode.Callback callback,
        /* [out] */ IActionMode** result);

    /** {@inheritDoc} */
    CARAPI OnActionModeStarted(
        /* [in] */ IActionMode* mode);

    /** {@inheritDoc} */
    CARAPI OnActionModeFinished(
        /* [in] */ IActionMode* mode);

    // end Window.Callback methods

    // begin public api
    /**
     * Retrieves the current {@link android.view.WindowManager} for the dream.
     * Behaves similarly to {@link android.app.Activity#getWindowManager()}.
     *
     * @return The current window manager, or null if the dream is not started.
     */
    CARAPI GetWindowManager(
        /* [out] */ IWindowManager** result);

    /**
     * Retrieves the current {@link android.view.Window} for the dream.
     * Behaves similarly to {@link android.app.Activity#getWindow()}.
     *
     * @return The current window, or null if the dream is not started.
     */
    CARAPI GetWindow(
        /* [out] */ IWindow** result);

   /**
     * Inflates a layout resource and set it to be the content view for this Dream.
     * Behaves similarly to {@link android.app.Activity#setContentView(int)}.
     *
     * <p>Note: Requires a window, do not call before {@link #onAttachedToWindow()}</p>
     *
     * @param layoutResID Resource ID to be inflated.
     *
     * @see #setContentView(android.view.View)
     * @see #setContentView(android.view.View, android.view.ViewGroup.LayoutParams)
     */
    CARAPI SetContentView(
        /* [in] */ Int32 layoutResID);

    /**
     * Sets a view to be the content view for this Dream.
     * Behaves similarly to {@link android.app.Activity#setContentView(android.view.View)} in an activity,
     * including using {@link ViewGroup.LayoutParams#MATCH_PARENT} as the layout height and width of the view.
     *
     * <p>Note: This requires a window, so you should usually call it during
     * {@link #onAttachedToWindow()} and never earlier (you <strong>cannot</strong> call it
     * during {@link #onCreate}).</p>
     *
     * @see #setContentView(int)
     * @see #setContentView(android.view.View, android.view.ViewGroup.LayoutParams)
     */
    CARAPI SetContentView(
        /* [in] */ IView* view);

    /**
     * Sets a view to be the content view for this Dream.
     * Behaves similarly to
     * {@link android.app.Activity#setContentView(android.view.View, android.view.ViewGroup.LayoutParams)}
     * in an activity.
     *
     * <p>Note: This requires a window, so you should usually call it during
     * {@link #onAttachedToWindow()} and never earlier (you <strong>cannot</strong> call it
     * during {@link #onCreate}).</p>
     *
     * @param view The desired content to display.
     * @param params Layout parameters for the view.
     *
     * @see #setContentView(android.view.View)
     * @see #setContentView(int)
     */
    CARAPI SetContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroup.LayoutParams* params);

    /**
     * Adds a view to the Dream's window, leaving other content views in place.
     *
     * <p>Note: Requires a window, do not call before {@link #onAttachedToWindow()}</p>
     *
     * @param view The desired content to display.
     * @param params Layout parameters for the view.
     */
    CARAPI AddContentView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroup.LayoutParams* params);

    /**
     * Finds a view that was identified by the id attribute from the XML that
     * was processed in {@link #onCreate}.
     *
     * <p>Note: Requires a window, do not call before {@link #onAttachedToWindow()}</p>
     *
     * @return The view if found or null otherwise.
     */
    CARAPI FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** result);

    /**
     * Marks this dream as interactive to receive input events.
     *
     * <p>Non-interactive dreams (default) will dismiss on the first input event.</p>
     *
     * <p>Interactive dreams should call {@link #finish()} to dismiss themselves.</p>
     *
     * @param interactive True if this dream will handle input events.
     */
    CARAPI SetInteractive(
        /* [in] */ Boolean interactive);

    /**
     * Returns whether or not this dream is interactive.  Defaults to false.
     *
     * @see #setInteractive(boolean)
     */
    CARAPI IsInteractive(
        /* [out] */ Boolean* result);

    /**
     * Sets View.SYSTEM_UI_FLAG_LOW_PROFILE on the content view.
     *
     * @param lowProfile True to set View.SYSTEM_UI_FLAG_LOW_PROFILE
     * @hide There is no reason to have this -- dreams can set this flag
     * on their own content view, and from there can actually do the
     * correct interactions with it (seeing when it is cleared etc).
     */
    CARAPI SetLowProfile(
        /* [in] */ Boolean lowProfile);

    /**
     * Returns whether or not this dream is in low profile mode. Defaults to true.
     *
     * @see #setLowProfile(boolean)
     * @hide
     */
    CARAPI IsLowProfile(
        /* [out] */ Boolean* result);

    /**
     * Controls {@link android.view.WindowManager.LayoutParams#FLAG_FULLSCREEN}
     * on the dream's window.
     *
     * @param fullscreen If true, the fullscreen flag will be set; else it
     * will be cleared.
     */
    CARAPI SetFullscreen(
        /* [in] */ Boolean fullscreen);

    /**
     * Returns whether or not this dream is in fullscreen mode. Defaults to false.
     *
     * @see #setFullscreen(boolean)
     */
    CARAPI IsFullscreen(
        /* [out] */ Boolean* result);

    /**
     * Marks this dream as keeping the screen bright while dreaming.
     *
     * @param screenBright True to keep the screen bright while dreaming.
     */
    CARAPI SetScreenBright(
        /* [in] */ Boolean screenBright);

    /**
     * Returns whether or not this dream keeps the screen bright while dreaming. Defaults to false,
     * allowing the screen to dim if necessary.
     *
     * @see #setScreenBright(boolean)
     */
    CARAPI IsScreenBright(
        /* [out] */ Boolean* result);

    /**
     * Called when this Dream is constructed.
     */
    CARAPI OnCreate();

    /**
     * Called when the dream's window has been created and is visible and animation may now begin.
     */
    CARAPI OnDreamingStarted();

    /**
     * Called when this Dream is stopped, either by external request or by calling finish(),
     * before the window has been removed.
     */
    CARAPI OnDreamingStopped();

    /** {@inheritDoc} */
    const CARAPI OnBind(
        /* [in] */ Intent intent,
        /* [out] */ IIBinder** result);

    /**
     * Stops the dream, detaches from the window, and wakes up.
     */
    const CARAPI Finish();

    /** {@inheritDoc} */
    CARAPI OnDestroy();

private:

    CARAPI_(void) LoadSandman();

    /**
     * Called by DreamController.stopDream() when the Dream is about to be unbound and destroyed.
     *
     * Must run on mHandler.
     */
    const CARAPI_(void) Detach();

    /**
     * Called when the Dream is ready to be shown.
     *
     * Must run on mHandler.
     *
     * @param windowToken A window token that will allow a window to be created in the correct layer.
     */
    const CARAPI_(void) Attach(
        /* [in] */ IIBinder* windowToken);

    CARAPI_(void) SafelyFinish();

    CARAPI_(void) FinishInternal();

    CARAPI_(Boolean) GetWindowFlagValue(
        /* [in] */ Int32 flag,
        /* [in] */ Boolean defaultValue);

    CARAPI_(void) ApplyWindowFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    CARAPI_(Boolean) GetSystemUiVisibilityFlagValue(
        /* [in] */ Int32 flag,
        /* [in] */ Boolean defaultValue);

    CARAPI_(void) ApplySystemUiVisibilityFlags(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    CARAPI_(Int32) ApplyFlags(
        /* [in] */ Int32 oldFlags,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mask);

    class DreamServiceWrapper extends IDreamServiceStub
    {
    public:
        CARAPI Attach(
            /* [in] */ const IIBinder* windowToken);

        CARAPI Detach();
    };
protected:
    CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String> args);

private:
    static const String TAG; // = DreamService.class.getSimpleName() + "[" + getClass().getSimpleName() + "]";
    const AutoPtr<IHandler> mHandler; // = new Handler();
    AutoPtr<IIBinder> mWindowToken;
    AutoPtr<IWindow> mWindow;
    AutoPtr<IWindowManager> mWindowManager;
    AutoPtr<IIDreamManager> mSandman;
    Boolean mInteractive; // = false;
    Boolean mLowProfile; // = true;
    Boolean mFullscreen; // = false;
    Boolean mScreenBright; // = true;
    Boolean mFinished;

    Boolean mDebug; // = false;
};

} // namespace Dreams
} // namespace Service
} // namepsace Droid
} // namespace Elastos

#endif // __CSANDMAN_H__
