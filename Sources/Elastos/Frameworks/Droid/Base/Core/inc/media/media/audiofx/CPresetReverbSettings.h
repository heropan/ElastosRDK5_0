
#ifndef __CPRESETREVERBSETTINGS_H__
#define __CPRESETREVERBSETTINGS_H__

#include "_CPresetReverbSettings.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

/**
 * The Settings class regroups all preset reverb parameters. It is used in
 * conjuntion with getProperties() and setProperties() methods to backup and restore
 * all parameters in a single call.
 */
CarClass(CPresetReverbSettings)
{
public:
    CPresetReverbSettings();

    CARAPI constructor();

    /**
     * Settings class constructor from a key=value; pairs formatted string. The string is
     * typically returned by Settings.toString() method.
     * @throws IllegalArgumentException if the string is not correctly formatted.
     */
    CARAPI constructor(
        /* [in] */ const String& settings);

    CARAPI GetPreset(
        /* [out] */ Int16* result);

    CARAPI SetPreset(
        /* [in] */ Int16 result);

    CARAPI ToString(
        /* [out] */ String* result);

public:
    Int16 mPreset;
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__CPRESETREVERBSETTINGS_H__
