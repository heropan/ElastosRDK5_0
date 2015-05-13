
#ifndef  __CSOUNDMANAGER_H__
#define  __CSOUNDMANAGER_H__

#include "_CSoundManager.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Media::IAudioManager;

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

/**
 * Class used to manage related sound resources.
 */
CarClass(CSoundManager)
{
public:
    CSoundManager();

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI UpdateRingerMode();

    CARAPI PlayKeyDown();

private:
    AutoPtr<IContext> mContext;
    AutoPtr<IAudioManager> mAudioManager;
    // Align sound effect volume on music volume
    const Float FX_VOLUME;
    Boolean mSilentMode;
};

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos

#endif  //__CSOUNDMANAGER_H__
