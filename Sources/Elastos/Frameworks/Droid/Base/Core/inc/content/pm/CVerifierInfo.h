
#ifndef __CVERIFIERINFO_H__
#define __CVERIFIERINFO_H__

#include "_CVerifierInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Contains information about a package verifier as used by
 * {@code PackageManagerService} during package verification.
 *
 * @hide
 */
CarClass(CVerifierInfo)
{
public:
    CVerifierInfo();

    ~CVerifierInfo();

    /**
     * Creates an object that represents a verifier info object.
     *
     * @param packageName the package name in Java-style. Must not be {@code
     *            null} or empty.
     * @param publicKey the public key for the signer encoded in Base64. Must
     *            not be {@code null} or empty.
     * @throws IllegalArgumentException if either argument is null or empty.
     */
    CARAPI constructor(
        /* [in] */ const String& packageName,
        /* [in] */ Elastos::Security::IPublicKey* publicKey);

    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    CARAPI SetPackageName(
        /* [in] */ const String& packageName);

    CARAPI GetPublicKey(
        /* [out] */ Elastos::Security::IPublicKey** publicKey);

    CARAPI SetPublicKey(
        /* [in] */ Elastos::Security::IPublicKey* publicKey);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    /** Package name of the verifier. */
    String mPackageName;

    /** Signatures used to sign the package verifier's package. */
    AutoPtr<Elastos::Security::IPublicKey> mPublicKey;

};

}
}
}
}

#endif // __CVERIFIERINFO_H__
