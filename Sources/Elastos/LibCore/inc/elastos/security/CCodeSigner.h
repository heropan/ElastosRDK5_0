
#ifndef __CCODESIGNER_H__
#define __CCODESIGNER_H__

#include "_CCodeSigner.h"

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

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
#endif // __CCODESIGNER_H__
