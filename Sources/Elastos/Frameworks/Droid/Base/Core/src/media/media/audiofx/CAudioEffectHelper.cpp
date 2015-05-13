#include "media/media/audiofx/CAudioEffectHelper.h"
#include "media/media/audiofx/AudioEffect.h"

using Elastos::Droid::Media::Audiofx::AudioEffect;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

ECode CAudioEffectHelper::QueryEffects(
    /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptors)
{
    return AudioEffect::QueryEffects(descriptors);
}

/**
 * Query all audio pre processing effects applied to the AudioRecord with the supplied
 * audio session ID. Returns an array of {@link android.media.audiofx.AudioEffect.Descriptor}
 * objects.
 * @param audioSession system wide unique audio session identifier.
 * @throws IllegalStateException
 * @hide
 */

ECode CAudioEffectHelper::QueryPreProcessings(
    /* [in] */ Int32 audioSession,
    /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptors)
{
    return AudioEffect::QueryPreProcessings(audioSession, descriptors);
}

/**
 * Checks if the device implements the specified effect type.
 * @param type the requested effect type.
 * @return true if the device implements the specified effect type, false otherwise.
 * @hide
 */
ECode CAudioEffectHelper::IsEffectTypeAvailable(
    /* [in] */ IUUID* type,
    /* [out] */ Boolean* result)
{
    return AudioEffect::IsEffectTypeAvailable(type, result);
}

ECode CAudioEffectHelper::IsError(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* result)
{
    return AudioEffect::IsError(status, result);
}

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
