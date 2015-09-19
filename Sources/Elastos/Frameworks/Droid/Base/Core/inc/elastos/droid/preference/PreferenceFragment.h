/**
 * Shows a hierarchy of {@link Preference} objects as
 * lists. These preferences will
 * automatically save to {@link SharedPreferences} as the user interacts with
 * them. To retrieve an instance of {@link SharedPreferences} that the
 * preference hierarchy in this fragment will use, call
 * {@link PreferenceManager#getDefaultSharedPreferences(android.content.Context)}
 * with a context in the same package as this fragment.
 * <p>
 * Furthermore, the preferences shown will follow the visual style of system
 * preferences. It is easy to create a hierarchy of preferences (that can be
 * shown on multiple screens) via XML. For these reasons, it is recommended to
 * use this fragment (as a superclass) to deal with preferences in applications.
 * <p>
 * A {@link PreferenceScreen} object should be at the top of the preference
 * hierarchy. Furthermore, subsequent {@link PreferenceScreen} in the hierarchy
 * denote a screen break--that is the preferences contained within subsequent
 * {@link PreferenceScreen} should be shown on another screen. The preference
 * framework handles showing these other screens from the preference hierarchy.
 * <p>
 * The preference hierarchy can be formed in multiple ways:
 * <li> From an XML file specifying the hierarchy
 * <li> From different {@link Activity Activities} that each specify its own
 * preferences in an XML file via {@link Activity} meta-data
 * <li> From an object hierarchy rooted with {@link PreferenceScreen}
 * <p>
 * To inflate from XML, use the {@link #addPreferencesFromResource(int)}. The
 * root element should be a {@link PreferenceScreen}. Subsequent elements can point
 * to actual {@link Preference} subclasses. As mentioned above, subsequent
 * {@link PreferenceScreen} in the hierarchy will result in the screen break.
 * <p>
 * To specify an {@link Intent} to query {@link Activity Activities} that each
 * have preferences, use {@link #addPreferencesFromIntent}. Each
 * {@link Activity} can specify meta-data in the manifest (via the key
 * {@link PreferenceManager#METADATA_KEY_PREFERENCES}) that points to an XML
 * resource. These XML resources will be inflated into a single preference
 * hierarchy and shown by this fragment.
 * <p>
 * To specify an object hierarchy rooted with {@link PreferenceScreen}, use
 * {@link #setPreferenceScreen(PreferenceScreen)}.
 * <p>
 * As a convenience, this fragment implements a click listener for any
 * preference in the current hierarchy, see
 * {@link #onPreferenceTreeClick(PreferenceScreen, Preference)}.
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For information about using {@code PreferenceFragment},
 * read the <a href="{@docRoot}guide/topics/ui/settings.html">Settings</a>
 * guide.</p>
 * </div>
 *
 * <a name="SampleCode"></a>
 * <h3>Sample Code</h3>
 *
 * <p>The following sample code shows a simple preference fragment that is
 * populated from a resource.  The resource it loads is:</p>
 *
 * {@sample development/samples/ApiDemos/res/xml/preferences.xml preferences}
 *
 * <p>The fragment implementation itself simply populates the preferences
 * when created.  Note that the preferences framework takes care of loading
 * the current values out of the app preferences and writing them when changed:</p>
 *
 * {@sample development/samples/ApiDemos/src/com/example/android/apis/preference/FragmentPreferences.java
 *      fragment}
 *
 * @see Preference
 * @see PreferenceScreen
 */

#include "app/Fragment.h"
#include "os/HandlerBase.h"
#include "os/Runnable.h"

using Elastos::Droid::App::Fragment;
using Elastos::Droid::View::IViewOnKeyListener;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::Runnable;

namespace Elastos {
namespace Droid {
namespace Preference {

class PreferenceFragment : public Fragment
{
private:
    class PreferenceFragmentHandler : public HandlerBase
    {
    public:
        PreferenceFragmentHandler(
            /* [in] */ PreferenceFragment* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        PreferenceFragment* mHost;
    };

    class RequestFocus : public Runnable
    {
    public:
        RequestFocus(
            /* [in] */ PreferenceFragment* host);

        CARAPI Run();

    private:
        PreferenceFragment* mHost;
    };

    class PreferenceFragmentOnKeyListener
        : public ElRefBase
        , public IViewOnKeyListener
    {
    public:
        PreferenceFragmentOnKeyListener(
            /* [in] */ PreferenceFragment* host);

        CAR_INTERFACE_DECL()

        CARAPI OnKey(
            /* [in] */ IView* v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out]*/ Boolean* result);

    private:
        PreferenceFragment* mHost;
    };

public:
    PreferenceFragment();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */REIID riid) = 0;

    CARAPI OnCreate(
        /*[in]*/ IBundle* savedInstanceState);

    CARAPI OnCreateView(
        /*[in]*/ ILayoutInflater* inflater,
        /*[in]*/ IViewGroup* container,
        /*[in]*/ IBundle* savedInstanceState,
        /*[out]*/ IView** view);

    CARAPI OnActivityCreated(
        /*[in]*/ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnStop();

    CARAPI OnDestroyView();

    CARAPI OnDestroy();

    CARAPI OnSaveInstanceState(
        /*[in]*/ IBundle* outState);

    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    /**
     * Returns the {@link PreferenceManager} used by this fragment.
     * @return The {@link PreferenceManager}.
     */
    CARAPI GetPreferenceManager(
        /*[out]*/ IPreferenceManager** preferencemanager);

    /**
     * Sets the root of the preference hierarchy that this fragment is showing.
     *
     * @param preferenceScreen The root {@link PreferenceScreen} of the preference hierarchy.
     */
    CARAPI SetPreferenceScreen(
        /*[in]*/ IPreferenceScreen* preferenceScreen);

    /**
     * Gets the root of the preference hierarchy that this fragment is showing.
     *
     * @return The {@link PreferenceScreen} that is the root of the preference
     *         hierarchy.
     */
    CARAPI GetPreferenceScreen(
        /*[out]*/ IPreferenceScreen** preferencescreen);

    /**
     * Adds preferences from activities that match the given {@link Intent}.
     *
     * @param intent The {@link Intent} to query activities.
     */
    CARAPI AddPreferencesFromIntent(
        /*[in]*/ IIntent* intent);

    /**
     * Inflates the given XML resource and adds the preference hierarchy to the current
     * preference hierarchy.
     *
     * @param preferencesResId The XML resource ID to inflate.
     */
    CARAPI AddPreferencesFromResource(
        /*[in]*/ Int32 preferencesResId);

    /**
     * {@inheritDoc}
     */
    CARAPI OnPreferenceTreeClick(
        /*[in]*/ IPreferenceScreen* preferenceScreen,
        /*[in]*/ IPreference* preference,
        /*[out]*/ Boolean* result);

    /**
     * Finds a {@link Preference} based on its key.
     *
     * @param key The key of the preference to retrieve.
     * @return The {@link Preference} with the key, or null.
     * @see PreferenceGroup#findPreference(CharSequence)
     */
    CARAPI FindPreference(
        /*[in]*/ const String& key,
        /*[out]*/ IPreference** preference);

    /** @hide */
    CARAPI GetListView(
        /*[out]*/ IListView** listview);

private:
    CARAPI RequirePreferenceManager();

    CARAPI_(void) PostBindPreferences();

    CARAPI_(void) BindPreferences();

    CARAPI EnsureList();

private:
    static const String PREFERENCES_TAG;
    /**
     * The starting request code given out to preference framework.
     */
    static const Int32 FIRST_REQUEST_CODE = 100;

    static const Int32 MSG_BIND_PREFERENCES = 1;

    AutoPtr<IPreferenceManager>  mPreferenceManager;
    AutoPtr<IListView> mList;
    Boolean mHavePrefs;
    Boolean mInitDone;
    AutoPtr<IHandler> mHandler;
    AutoPtr<IRunnable> mRequestFocus;
    AutoPtr<IViewOnKeyListener> mListOnKeyListener;
};

}
}
}
