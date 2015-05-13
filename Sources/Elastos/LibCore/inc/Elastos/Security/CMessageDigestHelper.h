
#ifndef __CMESSAGEDIGESTHELPER_H__
#define __CMESSAGEDIGESTHELPER_H__

#include "_CMessageDigestHelper.h"

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
#endif

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

#endif // __CMESSAGEDIGESTHELPER_H__
