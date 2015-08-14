
#ifndef __ELASTOS_DROID_CONTENT_PM_CMANIFESTDIGEST_H__
#define __ELASTOS_DROID_CONTENT_PM_CMANIFESTDIGEST_H__

#include "_CManifestDigest.h"

using Elastos::Utility::Jar::IAttributes;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CManifestDigest)
{
public:
    static CARAPI FromAttributes(
            /* [in] */ IAttributes* attributes,
            /* [out] */ IManifestDigest** digest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* digest);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    static CARAPI_(String) InitStatic();

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    /** The digest of the manifest in our preferred order. */
    AutoPtr<ArrayOf<Byte> > mDigest;

    /** Digest field names to look for in preferred order. */
    static AutoPtr<ArrayOf<String> > DIGEST_TYPES;

    /** What we print out first when toString() is called. */
    static const String TO_STRING_PREFIX;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CMANIFESTDIGEST_H__
