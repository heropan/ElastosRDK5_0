
#ifndef __PREFERENCEACTIVITY_H__
#define __PREFERENCEACTIVITY_H__

#include "ext/frameworkdef.h"
#include "app/ListActivity.h"
#include "os/HandlerBase.h"
#include <elastos/List.h>

using Elastos::Core::ICharSequence;
using Elastos::Utility::List;
using Elastos::Droid::App::ListActivity;
using Elastos::Droid::App::IFragment;
using Elastos::Droid::App::IFragmentManager;
using Elastos::Droid::App::IFragmentBreadCrumbs;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::IButton;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::IListView;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * This is the base class for an activity to show a hierarchy of preferences
* to the user.  Prior to {@link android.os.Build.VERSION_CODES#HONEYCOMB}
* this class only allowed the display of a single set of preference; this
* functionality should now be found in the new {@link PreferenceFragment}
* class.  If you are using PreferenceActivity in its old mode, the documentation
* there applies to the deprecated APIs here.
 *
* <p>This activity shows one or more headers of preferences, each of which
* is associated with a {@link PreferenceFragment} to display the preferences
* of that header.  The actual layout and display of these associations can
* however vary; currently there are two major approaches it may take:
 *
* <ul>
* <li>On a small screen it may display only the headers as a single list
* when first launched.  Selecting one of the header items will re-launch
* the activity with it only showing the PreferenceFragment of that header.
* <li>On a large screen in may display both the headers and current
* PreferenceFragment together as panes.  Selecting a header item switches
* to showing the correct PreferenceFragment for that item.
* </ul>
 *
* <p>Subclasses of PreferenceActivity should implement
* {@link #onBuildHeaders} to populate the header list with the desired
* items.  Doing this implicitly switches the class into its new "headers
* + fragments" mode rather than the old style of just showing a single
* preferences list.
 *
* <div class="special reference">
* <h3>Developer Guides</h3>
* <p>For information about using {@code PreferenceActivity},
* read the <a href="{@docRoot}guide/topics/ui/settings.html">Settings</a>
* guide.</p>
* </div>
 *
* <a name="SampleCode"></a>
* <h3>Sample Code</h3>
 *
* <p>The following sample code shows a simple preference activity that
* has two different sets of preferences.  The implementation, consisting
* of the activity itself as well as its two preference fragments is:</p>
 *
* {@sample development/samples/ApiDemos/src/com/example/android/apis/preference/PreferenceWithHeaders.java
*      activity}
 *
* <p>The preference_headers resource describes the headers to be displayed
* and the fragments associated with them.  It is:
 *
* {@sample development/samples/ApiDemos/res/xml/preference_headers.xml headers}
 *
* <p>The first header is shown by Prefs1Fragment, which populates itself
* from the following XML resource:</p>
 *
* {@sample development/samples/ApiDemos/res/xml/fragmented_preferences.xml preferences}
 *
* <p>Note that this XML resource contains a preference screen holding another
* fragment, the Prefs1FragmentInner implemented here.  This allows the user
* to traverse down a hierarchy of preferences; pressing back will pop each
* fragment off the stack to return to the previous preferences.
 *
* <p>See {@link PreferenceFragment} for information on implementing the
* fragments themselves.
 */
class PreferenceActivity
    : public IPreferenceActivity
    , public ListActivity
    , public IPreferenceManagerOnPreferenceTreeClickListener
    , public IPreferenceFragmentOnPreferenceStartFragmentCallback
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ PreferenceActivity* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        PreferenceActivity* mHost;
    };

    class BackButtonListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        BackButtonListener(
            /* [in] */ PreferenceActivity* parent);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        PreferenceActivity* mParent;
    };

    class SkipButtonListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        SkipButtonListener(
            /* [in] */ PreferenceActivity* parent);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        PreferenceActivity* mParent;
    };

    class NextButtonListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        NextButtonListener(
            /* [in] */ PreferenceActivity* parent);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        PreferenceActivity* mParent;
    };

public:
    PreferenceActivity();

    virtual ~PreferenceActivity();

    CAR_INTERFACE_DECL()

    virtual CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    /**
     * Returns true if this activity is currently showing the header list.
     */
    virtual CARAPI HasHeaders(
        /* [out] */ Boolean* hasHeaders);

    /**
     * Returns the Header list
     * @hide
     */
    CARAPI_(List<AutoPtr<IPreferenceActivityHeader> >&) GetHeaders();

    /**
     * Returns true if this activity is showing multiple panes -- the headers
     * and a preference fragment.
     */
    virtual CARAPI IsMultiPane(
        /* [out] */ Boolean* isMultiPane);

    /**
     * Called to determine if the activity should run in multi-pane mode.
     * The default implementation returns true if the screen is large
     * enough.
     */
    virtual CARAPI OnIsMultiPane(
        /* [out] */ Boolean* result);

    /**
     * Called to determine whether the header list should be hidden.
     * The default implementation returns the
     * value given in {@link #EXTRA_NO_HEADERS} or false if it is not supplied.
     * This is set to false, for example, when the activity is being re-launched
     * to show a particular preference activity.
     */
    virtual CARAPI OnIsHidingHeaders(
        /* [out] */ Boolean* result);

    /**
     * Called to determine the initial header to be shown.  The default
     * implementation simply returns the fragment of the first header.  Note
     * that the returned Header object does not actually need to exist in
     * your header list -- whatever its fragment is will simply be used to
     * show for the initial UI.
     */
    virtual CARAPI OnGetInitialHeader(
        /* [out] */ IPreferenceActivityHeader** header);

    /**
     * Called after the header list has been updated ({@link #onBuildHeaders}
     * has been called and returned due to {@link #invalidateHeaders()}) to
     * specify the header that should now be selected.  The default implementation
     * returns null to keep whatever header is currently selected.
     */
    virtual CARAPI OnGetNewHeader(
        /* [out] */ IPreferenceActivityHeader** header);

    /**
     * Called when the activity needs its list of headers build.  By
     * implementing this and adding at least one item to the list, you
     * will cause the activity to run in its modern fragment mode.  Note
     * that this function may not always be called; for example, if the
     * activity has been asked to display a particular fragment without
     * the header list, there is no need to build the headers.
     *
     * <p>Typical implementations will use {@link #loadHeadersFromResource}
     * to fill in the list from a resource.
     *
     * @param target The list in which to place the headers.
     */
    virtual CARAPI OnBuildHeaders(
        /* [in] */ IObjectContainer* target);

    /**
     * Call when you need to change the headers being displayed.  Will result
     * in onBuildHeaders() later being called to retrieve the new list.
     */
    virtual CARAPI InvalidateHeaders();

    /**
     * Parse the given XML file as a header description, adding each
     * parsed Header into the target list.
     *
     * @param resid The XML resource to load and parse.
     * @param target The list in which the parsed headers should be placed.
     */
    virtual CARAPI LoadHeadersFromResource(
        /* [in] */ Int32 resid,
        /* [in] */ IObjectContainer* target);

    /**
     * Set a footer that should be shown at the bottom of the header list.
     */
    virtual CARAPI SetListFooter(
        /* [in] */ IView* view);

    virtual CARAPI OnStop();

    virtual CARAPI OnDestroy();

    virtual CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* outState);

    virtual CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    virtual CARAPI OnContentChanged();

    virtual CARAPI OnListItemClick(
        /* [in] */ IListView* l,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    /**
     * Called when the user selects an item in the header list.  The default
     * implementation will call either
     * {@link #startWithFragment(String, Bundle, Fragment, int, int, int)}
     * or {@link #switchToHeader(Header)} as appropriate.
     *
     * @param header The header that was selected.
     * @param position The header's position in the list.
     */
    virtual CARAPI OnHeaderClick(
        /* [in] */ IPreferenceActivityHeader* header,
        /* [in] */ Int32 position);

    /**
     * Called by {@link #startWithFragment(String, Bundle, Fragment, int, int, int)} when
     * in single-pane mode, to build an Intent to launch a new activity showing
     * the selected fragment.  The default implementation constructs an Intent
     * that re-launches the current activity with the appropriate arguments to
     * display the fragment.
     *
     * @param fragmentName The name of the fragment to display.
     * @param args Optional arguments to supply to the fragment.
     * @param titleRes Optional resource ID of title to show for this item.
     * @param shortTitleRes Optional resource ID of short title to show for this item.
     * @return Returns an Intent that can be launched to display the given
     * fragment.
     */
    virtual CARAPI OnBuildStartFragmentIntent(
        /* [in] */ const String& fragmentName,
        /* [in] */ IBundle* args,
        /* [in] */ Int32 titleRes,
        /* [in] */ Int32 shortTitleRes,
        /* [out] */ IIntent** intent);

    /**
     * Like {@link #startWithFragment(String, Bundle, Fragment, int, int, int)}
     * but uses a 0 titleRes.
     */
    virtual CARAPI StartWithFragment(
        /* [in] */ const String& fragmentName,
        /* [in] */ IBundle* args,
        /* [in] */ IFragment* resultTo,
        /* [in] */ Int32 resultRequestCode);

    /**
     * Start a new instance of this activity, showing only the given
     * preference fragment.  When launched in this mode, the header list
     * will be hidden and the given preference fragment will be instantiated
     * and fill the entire activity.
     *
     * @param fragmentName The name of the fragment to display.
     * @param args Optional arguments to supply to the fragment.
     * @param resultTo Option fragment that should receive the result of
     * the activity launch.
     * @param resultRequestCode If resultTo is non-null, this is the request
     * code in which to report the result.
     * @param titleRes Resource ID of string to display for the title of
     * this set of preferences.
     * @param shortTitleRes Resource ID of string to display for the short title of
     * this set of preferences.
     */
    virtual CARAPI StartWithFragmentEx(
        /* [in] */ const String& fragmentName,
        /* [in] */ IBundle* args,
        /* [in] */ IFragment* resultTo,
        /* [in] */ Int32 resultRequestCode,
        /* [in] */ Int32 titleRes,
        /* [in] */ Int32 shortTitleRes);

    /**
     * Change the base title of the bread crumbs for the current preferences.
     * This will normally be called for you.  See
     * {@link android.app.FragmentBreadCrumbs} for more information.
     */
    virtual CARAPI ShowBreadCrumbs(
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* shortTitle);

    /**
     * Should be called after onCreate to ensure that the breadcrumbs, if any, were created.
     * This prepends a title to the fragment breadcrumbs and attaches a listener to any clicks
     * on the parent entry.
     * @param title the title for the breadcrumb
     * @param shortTitle the short title for the breadcrumb
     */
    virtual CARAPI SetParentTitle(
        /* [in] */ ICharSequence* title,
        /* [in] */ ICharSequence* shortTitle,
        /* [in] */ IViewOnClickListener* listener);

    /**
     * When in two-pane mode, switch the fragment pane to show the given
     * preference fragment.
     *
     * @param fragmentName The name of the fragment to display.
     * @param args Optional arguments to supply to the fragment.
     */
    virtual CARAPI SwitchToHeader(
        /* [in] */ const String& fragmentName,
        /* [in] */ IBundle* args);

    /**
     * When in two-pane mode, switch to the fragment pane to show the given
     * preference fragment.
     *
     * @param header The new header to display.
     */
    virtual CARAPI SwitchToHeaderEx(
        /* [in] */ IPreferenceActivityHeader* header);

    /**
     * Start a new fragment.
     *
     * @param fragment The fragment to start
     * @param push If true, the current fragment will be pushed onto the back stack.  If false,
     * the current fragment will be replaced.
     */
    virtual CARAPI StartPreferenceFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Boolean push);

    /**
     * Start a new fragment containing a preference panel.  If the prefences
     * are being displayed in multi-pane mode, the given fragment class will
     * be instantiated and placed in the appropriate pane.  If running in
     * single-pane mode, a new activity will be launched in which to show the
     * fragment.
     *
     * @param fragmentClass Full name of the class implementing the fragment.
     * @param args Any desired arguments to supply to the fragment.
     * @param titleRes Optional resource identifier of the title of this
     * fragment.
     * @param titleText Optional text of the title of this fragment.
     * @param resultTo Optional fragment that result data should be sent to.
     * If non-null, resultTo.onActivityResult() will be called when this
     * preference panel is done.  The launched panel must use
     * {@link #finishPreferencePanel(Fragment, int, Intent)} when done.
     * @param resultRequestCode If resultTo is non-null, this is the caller's
     * request code to be received with the resut.
     */
    virtual CARAPI StartPreferencePanel(
        /* [in] */ const String& fragmentClass,
        /* [in] */ IBundle* args,
        /* [in] */ Int32 titleRes,
        /* [in] */ ICharSequence* titleText,
        /* [in] */ IFragment* resultTo,
        /* [in] */ Int32 resultRequestCode);

    /**
     * Called by a preference panel fragment to finish itself.
     *
     * @param caller The fragment that is asking to be finished.
     * @param resultCode Optional result code to send back to the original
     * launching fragment.
     * @param resultData Optional result data to send back to the original
     * launching fragment.
     */
    virtual CARAPI FinishPreferencePanel(
        /* [in] */ IFragment* caller,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* resultData);

    virtual CARAPI OnPreferenceStartFragment(
        /* [in] */ IPreferenceFragment* caller,
        /* [in] */ IPreference* pref,
        /* [out] */ Boolean* result);

    /**
     * Returns the {@link PreferenceManager} used by this activity.
     * @return The {@link PreferenceManager}.
     *
     * @deprecated This function is not relevant for a modern fragment-based
     * PreferenceActivity.
     *
     * @Deprecated
     */
    virtual CARAPI GetPreferenceManager(
        /* [out] */ IPreferenceManager** manager);

    /**
     * Sets the root of the preference hierarchy that this activity is showing.
     *
     * @param preferenceScreen The root {@link PreferenceScreen} of the preference hierarchy.
     *
     * @deprecated This function is not relevant for a modern fragment-based
     * PreferenceActivity.
     *
     * @Deprecated
     */
    virtual CARAPI SetPreferenceScreen(
        /* [in] */ IPreferenceScreen* preferenceScreen);

    /**
     * Gets the root of the preference hierarchy that this activity is showing.
     *
     * @return The {@link PreferenceScreen} that is the root of the preference
     *         hierarchy.
     *
     * @deprecated This function is not relevant for a modern fragment-based
     * PreferenceActivity.
     *
     * @Deprecated
     */
    virtual CARAPI GetPreferenceScreen(
        /* [out] */ IPreferenceScreen** screen);

    /**
     * Adds preferences from activities that match the given {@link Intent}.
     *
     * @param intent The {@link Intent} to query activities.
     *
     * @deprecated This function is not relevant for a modern fragment-based
     * PreferenceActivity.
     *
     * @Deprecated
     */
    virtual CARAPI AddPreferencesFromIntent(
        /* [in] */ IIntent* intent);

    /**
     * Inflates the given XML resource and adds the preference hierarchy to the current
     * preference hierarchy.
     *
     * @param preferencesResId The XML resource ID to inflate.
     *
     * @deprecated This function is not relevant for a modern fragment-based
     * PreferenceActivity.
     *
     * @Deprecated
     */
    virtual CARAPI AddPreferencesFromResource(
        /* [in] */ Int32 preferencesResId);

    /**
     * {@inheritDoc}
     *
     * @deprecated This function is not relevant for a modern fragment-based
     * PreferenceActivity.
     *
     * @Deprecated
     */
    virtual CARAPI OnPreferenceTreeClick(
        /* [in] */ IPreferenceScreen* preferenceScreen,
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

    /**
     * Finds a {@link Preference} based on its key.
     *
     * @param key The key of the preference to retrieve.
     * @return The {@link Preference} with the key, or null.
     * @see PreferenceGroup#findPreference(CharSequence)
     *
     * @deprecated This function is not relevant for a modern fragment-based
     * PreferenceActivity.
     *
     * @Deprecated
     */
    virtual CARAPI FindPreference(
        /* [in] */ const String& key,
        /* [out] */ IPreference** preference);

    virtual CARAPI OnNewIntent(
        /* [in] */ IIntent* intent);

protected:
    // give subclasses access to the Next button
    /** @hide */
    CARAPI_(Boolean) HasNextButton();

    /** @hide */
    CARAPI_(AutoPtr<IButton>) GetNextButton();

private:
    CARAPI_(void) SetSelectedHeader(
        /* [in] */ IPreferenceActivityHeader* header);

    CARAPI_(void) ShowBreadCrumbsEx(
        /* [in] */ IPreferenceActivityHeader* header);

    CARAPI_(void) SwitchToHeaderInner(
        /* [in] */ const String& fragmentName,
        /* [in] */ IBundle* args,
        /* [in] */ Int32 direction);

    CARAPI_(AutoPtr<IPreferenceActivityHeader>) FindBestMatchingHeader(
        /* [in] */ IPreferenceActivityHeader* cur,
        /* [in] */ List<AutoPtr<IPreferenceActivityHeader> >& from);

    /**
     * Posts a message to bind the preferences to the list view.
     * <p>
     * Binding late is preferred as any custom preference types created in
     * {@link #onCreate(Bundle)} are able to have their views recycled.
     */
    CARAPI_(void) PostBindPreferences();

    CARAPI_(void) BindPreferences();

    CARAPI RequirePreferenceManager();

    CARAPI HandleBuildHeaders();

private:
    // Constants for state save/restore
    static const String HEADERS_TAG;
    static const String CUR_HEADER_TAG;
    static const String PREFERENCES_TAG;

    static const String BACK_STACK_PREFS;

    // show Back and Next buttons? takes boolean parameter
    // Back will then return RESULT_CANCELED and Next RESULT_OK
    static const String EXTRA_PREFS_SHOW_BUTTON_BAR ;

    // add a Skip button?
    static const String EXTRA_PREFS_SHOW_SKIP;

    // specify custom text for the Back or Next buttons, or cause a button to not appear
    // at all by setting it to null
    static const String EXTRA_PREFS_SET_NEXT_TEXT;
    static const String EXTRA_PREFS_SET_BACK_TEXT;

    /**
     * The starting request code given out to preference framework.
     */
    static const Int32 FIRST_REQUEST_CODE = 100;

    static const Int32 MSG_BIND_PREFERENCES = 1;
    static const Int32 MSG_BUILD_HEADERS = 2;

    List<AutoPtr<IPreferenceActivityHeader> > mHeaders;

    AutoPtr<IFrameLayout> mListFooter;

    AutoPtr<IViewGroup> mPrefsContainer;

    AutoPtr<IFragmentBreadCrumbs> mFragmentBreadCrumbs;

    Boolean mSinglePane;

    AutoPtr<IPreferenceActivityHeader> mCurHeader;

    // --- State for old mode when showing a single preference list

    AutoPtr<IPreferenceManager> mPreferenceManager;

    AutoPtr<IBundle> mSavedInstanceState;

    // --- Common state

    AutoPtr<IButton> mNextButton;

    AutoPtr<IHandler> mHandler;
};

} // Preference
} // Droid
} // Elastos

#endif // __PREFERENCEACTIVITY_H__
