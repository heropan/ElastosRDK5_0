
#ifndef __ELASTOS_DROID_MEDIA_AUDIOFX_CEQUALIZERSETTINGS_H__
#define __ELASTOS_DROID_MEDIA_AUDIOFX_CEQUALIZERSETTINGS_H__

#include "_CEqualizerSettings.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

/**
 * The Settings class regroups all bass boost parameters. It is used in
 * conjuntion with getProperties() and setProperties() methods to backup and restore
 * all parameters in a single call.
 */
CarClass(CEqualizerSettings)
{
public:
    CEqualizerSettings();

    CARAPI constructor();

    /**
     * Settings class constructor from a key=value; pairs formatted string. The string is
     * typically returned by Settings.toString() method.
     * @throws IllegalArgumentException if the string is not correctly formatted.
     */
    CARAPI constructor(
        /* [in] */ const String& settings);

    CARAPI GetCurPreset(
        /* [out] */ Int16* curPreset);

    CARAPI SetCurPreset(
        /* [in] */ Int16 curPreset);

    CARAPI GetNumBands(
        /* [out] */ Int16* numBands);

    CARAPI SetNumBands(
        /* [in] */ Int16 numBands);

    CARAPI GetBandLevels(
        /* [out, callee] */ ArrayOf<Int16>** levels);

    CARAPI SetBandLevels(
        /* [in] */ ArrayOf<Int16>* levels);

    CARAPI ToString(
        /* [out] */ String* result);

public:
    Int16 mCurPreset;
    Int16 mNumBands;
    AutoPtr<ArrayOf<Int16> > mBandLevels;
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_MEDIA_AUDIOFX_CEQUALIZERSETTINGS_H__
