
#ifndef __ELASTOS_DROID_MEDIA_CJETPLAYERHELPER_H__
#define __ELASTOS_DROID_MEDIA_CJETPLAYERHELPER_H__

#include "_CJetPlayerHelper.h"
#include "CJetPlayer.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CJetPlayerHelper)
{
public:
    /**
     * Factory method for the JetPlayer class.
     * @return the singleton JetPlayer instance
     */
    CARAPI GetJetPlayer(
        /* [out] */ IJetPlayer** player);

    /**
     * Returns the maximum number of simultaneous MIDI tracks supported by JetPlayer
     */
    CARAPI GetMaxTracks(
        /* [out] */ Int32* tracks);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_CJETPLAYERHELPER_H__
