#ifndef __ELASTOS_DROID_SETTINGS_VOICEINPUTOUTPUTSETTINGS_H__
#define __ELASTOS_DROID_SETTINGS_VOICEINPUTOUTPUTSETTINGS_H__

#include "Elastos.Droid.Speech.h"
#include "elastos/droid/settings/SettingsPreferenceFragment.h"

using Elastos::Droid::Preference::IPreference;
using Elastos::Droid::Preference::IPreferenceCategory;
using Elastos::Droid::Preference::IPreferenceGroup;
using Elastos::Droid::Speech::Tts::ITtsEngines;

namespace Elastos {
namespace Droid {
namespace Settings {

/**
 * Settings screen for voice input/output.
 */
class VoiceInputOutputSettings
    : public Object
{
public:
    VoiceInputOutputSettings(
        /* [in] */ SettingsPreferenceFragment* fragment);

    ~VoiceInputOutputSettings();

    CARAPI OnCreate();

private:
    CARAPI_(void) PopulateOrRemovePreferences();

    CARAPI_(Boolean) PopulateOrRemoveVoiceInputPrefs();

    CARAPI_(Boolean) PopulateOrRemoveTtsPrefs();

private:
    static const String TAG;

    static const String KEY_VOICE_CATEGORY;
    static const String KEY_VOICE_INPUT_SETTINGS;
    static const String KEY_TTS_SETTINGS;

    AutoPtr<IPreferenceGroup> mParent;
    AutoPtr<IPreferenceCategory> mVoiceCategory;
    AutoPtr<IPreference> mVoiceInputSettingsPref;
    AutoPtr<IPreference> mTtsSettingsPref;
    AutoPtr<SettingsPreferenceFragment> mFragment;
    AutoPtr<ITtsEngines> mTtsEngines;
};

} // namespace Settings
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SETTINGS_VOICEINPUTOUTPUTSETTINGS_H__
