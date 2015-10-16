
#ifndef __ELASTOS_DROID_MEDIA_CMEDIACRYPTOHELPER_H__
#define __ELASTOS_DROID_MEDIA_CMEDIACRYPTOHELPER_H__

#include "_Elastos_Droid_Media_CMediaCryptoHelper.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Utility::IUUID;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaCryptoHelper)
{
public:
    /**
     * Query if the given scheme identified by its UUID is supported on
     * this device.
     * @param uuid The UUID of the crypto scheme.
     */
    CARAPI IsCryptoSchemeSupported(
        /* [in] */ IUUID* uuid,
        /* [out] */ Boolean* result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CMEDIACRYPTOHELPER_H__
