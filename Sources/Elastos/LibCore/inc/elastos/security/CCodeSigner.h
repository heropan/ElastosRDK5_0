
#ifndef __ELASTOS_SECURITY_CCODESIGNER_H__
#define __ELASTOS_SECURITY_CCODESIGNER_H__

#include "_CCodeSigner.h"



using Elastos::Security::Cert::ICertPath;

namespace Elastos {
namespace Security {

CarClass(CCodeSigner)
{
public:
    CCodeSigner();

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI GetSignerCertPath(
        /* [out] */ ICertPath** certPath);

    CARAPI GetTimestamp(
        /* [out] */ ITimestamp** timestamp);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICertPath* signerCertPath,
        /* [in] */ ITimestamp* timestamp);

private:
    AutoPtr<ICertPath> mSignerCertPath;

    AutoPtr<ITimestamp> mTimestamp;

    // Cached hash code value
    Int32 mHash;
};

} // end Security
} // end Elastos
#endif // __ELASTOS_SECURITY_CCODESIGNER_H__
