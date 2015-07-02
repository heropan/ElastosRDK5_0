
#ifndef __CERTIFICATEX_H__
#define __CERTIFICATEX_H__


#include "Object.h"

using Elastos::Security::IPublicKey;

namespace Elastosx {
namespace Security {
namespace Cert {

extern "C" const InterfaceID EIID_XCertificate;

class Certificate
    : public ElLightRefBase {
public:
    virtual CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    virtual CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encode) = 0;

    virtual CARAPI Verify(
        /* [in] */ IPublicKey* key) = 0;

    virtual CARAPI Verify(
        /* [in] */ IPublicKey* key,
        /* [in] */ const String& sigProvider) = 0;

    virtual CARAPI ToString(
        /* [out] */ String* str) = 0;

    virtual CARAPI GetPublicKey(
        /* [out] */ IPublicKey** pubkey) = 0;

};

} // end Cert
} // end Security
} // end Elastosx

#endif // __CERTIFICATEX_H__