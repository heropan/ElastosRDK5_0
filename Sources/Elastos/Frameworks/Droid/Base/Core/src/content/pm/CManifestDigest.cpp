
#include "content/pm/CManifestDigest.h"
#include "util/CBase64.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/IntegralToString.h>

using Elastos::Core::IntegralToString;
using Elastos::Core::StringBuilder;

using Elastos::Droid::Utility::CBase64;
using Elastos::Utility::IArrays;
using Elastos::Utility::CArrays;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/** Digest field names to look for in preferred order. */
AutoPtr<ArrayOf<String> > CManifestDigest::DIGEST_TYPES;

/** What we print out first when toString() is called. */
const String CManifestDigest::TO_STRING_PREFIX = InitStatic();

ECode CManifestDigest::FromAttributes(
        /* [in] */ IAttributes* attributes,
        /* [out] */ IManifestDigest** digest)
{
    VALIDATE_NOT_NULL(digest)
    if (attributes == NULL) {
        return NOERROR;
    }
    String encodedDigest;
    for (Int32 i = 0; i < DIGEST_TYPES->GetLength(); i++) {
        String value;
        attributes->GetValue((*DIGEST_TYPES)[i], &value);
        if (!value.IsNull()) {
            encodedDigest = value;
            break;
        }
    }

    if (encodedDigest.IsNull()) {
        return NOERROR;
    }

    AutoPtr<ArrayOf<Byte> > dgt;
    AutoPtr<IBase64> base64;
    CBase64::AcquireSingleton((IBase64**)&base64);
    base64->Decode(encodedDigest, IBase64::DEFAULT, (ArrayOf<Byte>**)&dgt);
    return CManifestDigest::New(dgt, digest);
}

ECode CManifestDigest::constructor()
{
    return NOERROR;
}

ECode CManifestDigest::constructor(
    /* [in] */ ArrayOf<Byte>* digest)
{
    VALIDATE_NOT_NULL(digest)
    mDigest = digest;
    return NOERROR;
}

ECode CManifestDigest::constructor(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source)
    source->ReadArrayOf((Handle32*)&mDigest);
    return NOERROR;
}

ECode CManifestDigest::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    const AutoPtr<IManifestDigest> other = IManifestDigest::Probe(obj);
    if (other == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    return this == (CManifestDigest*)other.Get()
        || mDigest->Equals(((CManifestDigest*)other.Get())->mDigest);
}

ECode CManifestDigest::GetHashCode(
    /* [out] */ Int32* code)
{
    AutoPtr<IArrays> arr;
    CArrays::AcquireSingleton((IArrays**)&arr);
    return arr->HashCodeByte(mDigest, code);
}

ECode CManifestDigest::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder sb(TO_STRING_PREFIX.GetLength()
        + mDigest->GetLength() * 3 + 1);
    sb.AppendString(TO_STRING_PREFIX);
    const Int32 N = mDigest->GetLength();
    for (Int32 i = 0; i < N; i++) {
        const Byte b = (*mDigest)[i];
        IntegralToString::AppendByteAsHex(sb, b, FALSE);
        sb.AppendChar(',');
    }
    sb.AppendChar('}');
    return sb.ToString(str);
}

ECode CManifestDigest::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return constructor(source);
}

ECode CManifestDigest::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest)
    return dest->WriteArrayOf((Handle32)mDigest.Get());
}

String CManifestDigest::InitStatic()
{
    AutoPtr<ArrayOf<String> > DIGEST_TYPES = ArrayOf<String>::Alloc(3);
    (*DIGEST_TYPES)[0] = String("SHA1-Digest");
    (*DIGEST_TYPES)[1] = String("SHA-Digest");
    (*DIGEST_TYPES)[2] = String("MD5-Digest");
    return String("ManifestDigest {mDigest=");
}

}
}
}
}
