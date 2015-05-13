
#ifndef  __CPINYINSETTINGS_H__
#define  __CPINYINSETTINGS_H__

#include "ext/frameworkext.h"
#include "_CPinyinSettings.h"

using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::ISharedPreferencesEditor;

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

class PinyinSettings: public ElLightRefBase
{
public:
    PinyinSettings(
        /* [in] */ ISharedPreferences* pref);
};

CarClass(CPinyinSettings)
{
public:
    CARAPI GetInstance(
        /* [in] */ ISharedPreferences* pref);

    CARAPI WriteBack();

    CARAPI ReleaseInstance();

    CARAPI GetKeySound(
        /* [out] */ Boolean* result);

    CARAPI SetKeySound(
        /* [in] */ Boolean v);

    CARAPI GetVibrate(
        /* [out] */ Boolean* result);

    CARAPI SetVibrate(
        /* [in] */ Boolean v);

    CARAPI GetPrediction(
        /* [out] */ Boolean* result);

    CARAPI SetPrediction(
        /* [in] */ Boolean v);

private:
    void InitConfs();

private:
    static String ANDPY_CONFS_KEYSOUND_KEY;
    static String ANDPY_CONFS_VIBRATE_KEY;
    static String ANDPY_CONFS_PREDICTION_KEY;

    static Boolean sKeySound;
    static Boolean sVibrate;
    static Boolean sPrediction;

    static AutoPtr<PinyinSettings> sInstance;
    static Int32 sRefCount;
    static AutoPtr<ISharedPreferences> sSharedPref;
    friend class PinyinSettings;
};

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos

#endif  // __CPINYINSETTINGS_H__
