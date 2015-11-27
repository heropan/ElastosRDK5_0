#ifndef __ELASTOS_DROID_SPEECH_TTS_TtsEngines_H__
#define __ELASTOS_DROID_SPEECH_TTS_TtsEngines_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/core/Object.h"
#include "elastos/droid/speech/tts/TextToSpeech.h"
#include <elastos/utility/etl/List.h>

//using Elastos::Utility::IComparator;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::IContext;
namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Support class for querying the list of available engines
 * on the device and deciding which one to use etc.
 *
 * Comments in this class the use the shorthand "system engines" for engines that
 * are a part of the system image.
 *
 * @hide
 */
//public class
class TtsEngines
    : public Object
    , public ITtsEngines
{
private:
    //private static class
    class EngineInfoComparator
        : public Object
//        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL();

        EngineInfoComparator();

        virtual ~EngineInfoComparator();

        CARAPI constructor();

    public:
        /**
         * Engines that are a part of the system image are always lesser
         * than those that are not. Within system engines / non system engines
         * the engines are sorted in order of their declared priority.
         */
        //@Override
        //public int compare(EngineInfo lhs, EngineInfo rhs);
        CARAPI Compare(
            /* [in] */ IInterface* object1,
            /* [in] */ IInterface* object2,
            /* [out] */ Int32* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* isEqual);

    public:
        static Int32 Comparator(
            /* [in] */ AutoPtr<TextToSpeech::TextToSpeechEngineInfo> lhs,
            /* [in] */ AutoPtr<TextToSpeech::TextToSpeechEngineInfo> rhs);

    private:
        //private
        EngineInfoComparator();

    public:
        static AutoPtr<EngineInfoComparator> INSTANCE;// = new EngineInfoComparator();
    };

public:
    CAR_INTERFACE_DECL();

    TtsEngines();

    virtual ~TtsEngines();

    constructor();

    //public
    TtsEngines(
        /* [in] */ IContext* ctx);

    CARAPI_(void) Init(
        /* [in] */ IContext* ctx);

    /**
     * @return the default TTS engine. If the user has set a default, and the engine
     *         is available on the device, the default is returned. Otherwise,
     *         the highest ranked engine is returned as per {@link EngineInfoComparator}.
     */
    //public
    CARAPI_(String) GetDefaultEngine();

    /**
     * @return the package name of the highest ranked system engine, {@code null}
     *         if no TTS engines were present in the system image.
     */
    //public
    CARAPI_(String) GetHighestRankedEngineName();

    /**
     * Returns the engine info for a given engine name. Note that engines are
     * identified by their package name.
     */
    //public
    CARAPI_(AutoPtr<TextToSpeech::TextToSpeechEngineInfo>) GetEngineInfo(
        /* [in] */ const String& packageName);

    /**
     * Gets a list of all installed TTS engines.
     *
     * @return A list of engine info objects. The list can be empty, but never {@code null}.
     */
    //public
    CARAPI_(AutoPtr< List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> > >) GetEngines();

public:
    /**
     * @return true if a given engine is installed on the system.
     */
    //public
    CARAPI_(Boolean) IsEngineInstalled(
        /* [in] */ const String& engine);

    /**
     * @return an intent that can launch the settings activity for a given tts engine.
     */
    //public
    CARAPI_(AutoPtr<IIntent>) GetSettingsIntent(
        /* [in] */ const String& engine);

public:
    /**
     * Returns the default locale for a given TTS engine. Attempts to read the
     * value from {@link Settings.Secure#TTS_DEFAULT_LOCALE}, failing which the
     * default phone locale is returned.
     *
     * @param engineName the engine to return the locale for.
     * @return the locale preference for this engine. Will be non null.
     */
    //public
    CARAPI GetLocalePrefForEngine(
        /* [in] */ const String& engineName,
        /* [in] */ ILocale** locale);

    /**
     * Returns the default locale for a given TTS engine from given settings string. */
    CARAPI GetLocalePrefForEngine(
        /* [in] */ const String& engineName,
        /* [in] */ const String& prefValue,
        /* [in] */ ILocale** locale);

    /**
     * Parses a locale encoded as a string, and tries its best to return a valid {@link Locale}
     * object, even if the input string is encoded using the old-style 3 character format e.g.
     * "deu-deu". At the end, we test if the resulting locale can return ISO3 language and
     * country codes ({@link Locale#getISO3Language()} and {@link Locale#getISO3Country()}),
     * if it fails to do so, we return null.
     */
    //public
    static CARAPI_(AutoPtr<ILocale>) parseLocaleString(
        /* [in] */ const String& localeString);

    //public synchronized
    CARAPI UpdateLocalePrefForEngine(
        /* [in] */ const String& name,
        /* [in] */ const ILocale* newLocale);

protected:
    TtsEngines();

private:
    //private
    CARAPI_(Boolean) IsSystemEngine(
        /* [in] */ IServiceInfo* info);

private:
    //private
    CARAPI_(String) SettingsActivityFromServiceInfo(
        /* [in] */ IServiceInfo* si,
        /* [in] */ IPackageManager* pm);

    //private
    CARAPI_(AutoPtr<TextToSpeech::TextToSpeechEngineInfo>) GetEngineInfo(
        /* [in] */ IResolveInfo* resolve,
        /* [in] */ IPackageManager* pm);

private:
    /**
     * @return the old style locale string constructed from
     *         {@link Settings.Secure#TTS_DEFAULT_LANG},
     *         {@link Settings.Secure#TTS_DEFAULT_COUNTRY} and
     *         {@link Settings.Secure#TTS_DEFAULT_VARIANT}. If no such locale is set,
     *         then return the default phone locale.
     */
    //private
    CARAPI_(String) GetV1Locale();

    //private
    CARAPI_(String) GetDefaultLocale();

    /**
     * Parses a comma separated list of engine locale preferences. The list is of the
     * form {@code "engine_name_1:locale_1,engine_name_2:locale2"} and so on and
     * so forth. Returns null if the list is empty, malformed or if there is no engine
     * specific preference in the list.
     */
    //private
    static CARAPI_(String) ParseEnginePrefFromList(
        /* [in] */ const String& prefValue,
        /* [in] */ const String& engineName);

private:
    /**
     * Updates the value for a given key in a comma separated list of key value pairs,
     * each of which are delimited by a colon. If no value exists for the given key,
     * the kay value pair are appended to the end of the list.
     */
    //private
    CARAPI_(String) UpdateValueInCommaSeparatedList(
        /* [in] */ const String& list,
        /* [in] */ const String& key,
        /* [in] */ const String& newValue);

protected:
    Object mLock;

private:
    //private
    static const String TAG;                // = "TtsEngines";
    //private
    static const Boolean DBG;               // = FALSE;

    /** Locale delimiter used by the old-style 3 char locale string format (like "eng-usa") */
    static const String LOCALE_DELIMITER_OLD;   // = "-";

    /** Locale delimiter used by the new-style locale string format (Locale.toString() results,
     * like "en_US") */
    static const String LOCALE_DELIMITER_NEW;   // = "-";

    /** Mapping of various language strings to the normalized Locale form */
    static final Map<String, String> sNormalizeLanguage;

    /** Mapping of various country strings to the normalized Locale form */
    static final Map<String, String> sNormalizeCountry;

    //private final
    AutoPtr<IContext> mContext;

private:
    /**
     * The name of the XML tag that text to speech engines must use to
     * declare their meta data.
     *
     * {@link com.android.internal.R.styleable#TextToSpeechEngine}
     */
    //private
    static const String XML_TAG_NAME;       // = "tts-engine";
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_TtsEngines_H__
