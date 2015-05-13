
#include "media/CMediaCryptoHelper.h"
#include "media/CMediaCrypto.h"

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Media {

ECode CMediaCryptoHelper::IsCryptoSchemeSupported(
    /* [in] */ IUUID* uuid,
    /* [out] */ Boolean* result)
{
    return CMediaCrypto::IsCryptoSchemeSupported(uuid, result);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
