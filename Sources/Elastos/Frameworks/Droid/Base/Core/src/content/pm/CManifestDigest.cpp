
#include "content/pm/CManifestDigest.h"
#include "util/CBase64.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/IntegralToString.h>


using Elastos::Droid::Utility::CBase64;
using Elastos::Core::IntegralToString;
using Elastos::Core::StringBuilder;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::Utility::IArrays;
using Elastos::Utility::CArrays;
using Elastos::Security::IMessageDigest;
using Elastos::Security::IMessageDigestHelper;
using Elastos::Security::CMessageDigestHelper;
using Libcore::IO::IIoUtils;
using Libcore::IO::CIoUtils;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {


/** What we print out first when toString() is called. */
const String CManifestDigest::TO_STRING_PREFIX("ManifestDigest {mDigest=");

const String CManifestDigest::DIGEST_ALGORITHM("SHA-256");

ECode CManifestDigest::FromInputStream(
    /* [in] */ IInputStream* fileIs,
    /* [out] */ IManifestDigest** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    if (fileIs == NULL) {
        return NOERROR;
    }

    AutoPtr<IManifestDigest> md;
    AutoPtr<IManifestDigestHelper> helper;
    CMessageDigestHelper::AcquireSingleton((IManifestDigestHelper**)&helper);
    ECode ec = helper->GetInstance(DIGEST_ALGORITHM, (IManifestDigest**)&md);
    if (ec == (ECode)E_NO_SUCH_ALGORITHM_EXCEPTION) {
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IBufferedInputStream> bis;
    CBufferedInputStream::New(fileIs, (IBufferedInputStream**)&bis);

    AutoPtr<IDigestInputStream> dis;
    CDigestInputStream::New(bis, md, (IDigestInputStream**)&dis);

    // try {
    AutoPtr<ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(8192);
    Int32 count;
    while (dis->Read(readBuffer, 0, readBuffer->GetLength(), &count), count != -1) {
        // not using
    }
    // } catch (IOException e) {
    //     Slog.w(TAG, "Could not read manifest");
    //     return null;
    // }
    // finally {
    AutoPtr<IIoUtils> iou;
    CIoUtils::AcquireSingleton((IIoUtils**)&iou)
    iou->CloseQuietly(ICloseable::Probe(dis));
    // }

    if (ec == (ECode)E_IO_EXCEPTION) {
        return ec;
    }

    AutoPtr<ArrayOf<Byte> > digest;
    md->Digest((ArrayOf<Byte>**)&digest);
    AutoPtr<CManifestDigest> cmd;
    CManifestDigest::NewByFriend(digest, (CManifestDigest**)&cmd);
    *result = (IManifestDigest*)cmd.Get();
    REFCOUNT_ADD(*result)
    return NOERROR;
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
    sb.Append(TO_STRING_PREFIX);
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


}
}
}
}
