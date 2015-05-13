
#ifndef __CAUDIOEFFECTHELPER_H__
#define __CAUDIOEFFECTHELPER_H__

#include "_CAudioEffectHelper.h"
#include "ext/frameworkext.h"

using Elastos::Utility::IUUID;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

CarClass(CAudioEffectHelper)
{
public:
    /**
     * Query all effects available on the platform. Returns an array of
     * {@link android.media.audiofx.AudioEffect.Descriptor} objects
     *
     * @throws IllegalStateException
     */
    CARAPI QueryEffects(
        /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptors);

    /**
     * Query all audio pre processing effects applied to the AudioRecord with the supplied
     * audio session ID. Returns an array of {@link android.media.audiofx.AudioEffect.Descriptor}
     * objects.
     * @param audioSession system wide unique audio session identifier.
     * @throws IllegalStateException
     * @hide
     */

    CARAPI QueryPreProcessings(
        /* [in] */ Int32 audioSession,
        /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptors);

    /**
     * Checks if the device implements the specified effect type.
     * @param type the requested effect type.
     * @return true if the device implements the specified effect type, false otherwise.
     * @hide
     */
    CARAPI IsEffectTypeAvailable(
        /* [in] */ IUUID* type,
        /* [out] */ Boolean* result);

    CARAPI IsError(
        /* [in] */ Int32 status,
        /* [out] */ Boolean* result);
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__CAUDIOEFFECTHELPER_H__
