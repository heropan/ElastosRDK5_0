
#ifndef __ELASTOS_SECURITY_CMESSAGEDIGESTHELPER_H__
#define __ELASTOS_SECURITY_CMESSAGEDIGESTHELPER_H__

#include "_Elastos_Security_CMessageDigestHelper.h"

namespace Elastos {
namespace Security {

CarClass(CMessageDigestHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& algorithm,
        /* [out] */ IMessageDigest** instance);

    CARAPI GetInstanceEx(
        /* [in] */ const String& algorithm,
        /* [in] */ const String& provider,
        /* [out] */ IMessageDigest** instance);

    CARAPI GetInstanceEx2(
        /* [in] */ const String& algorithm,
        /* [in] */ IProvider* provider,
        /* [out] */ IMessageDigest** instance);

    CARAPI IsEqual(
        /* [in] */ ArrayOf<Byte>* digesta,
        /* [in] */ ArrayOf<Byte>* digestb,
        /* [out] */ Boolean* result);
};

} // namespace Security
} // namespace Elastos

#endif // __ELASTOS_SECURITY_CMESSAGEDIGESTHELPER_H__
