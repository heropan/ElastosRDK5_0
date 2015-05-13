
#include "media/CJetPlayerHelper.h"

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Media {

ECode CJetPlayerHelper::GetJetPlayer(
    /* [out] */ IJetPlayer** player)
{
    VALIDATE_NOT_NULL(player);

    AutoPtr<IJetPlayer> obj = CJetPlayer::GetJetPlayer();
    *player = obj;
    INTERFACE_ADDREF(*player);
    return NOERROR;
}

ECode CJetPlayerHelper::GetMaxTracks(
    /* [out] */ Int32 * tracks)
{
    VALIDATE_NOT_NULL(tracks);

    *tracks = CJetPlayer::GetMaxTracks();
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
