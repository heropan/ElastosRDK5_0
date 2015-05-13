
#ifndef __CMEDIACRYPTOHELPER_H__
#define __CMEDIACRYPTOHELPER_H__

#include "_CMediaCryptoHelper.h"
#include <ext/frameworkext.h>

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

#endif // __CMEDIACRYPTOHELPER_H__
