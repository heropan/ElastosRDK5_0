
#include "CCodeSigner.h"
#include "cmdef.h"
#include "StringBuilder.h"

using Elastos::Core::StringBuilder;
using Elastos::Utility::IList;

namespace Elastos {
namespace Security {

static const Int64 sSerialVersionUID = 6819288105193937581L;

CCodeSigner::CCodeSigner()
    : mHash(0)
{}

/**
 * Compares the specified object with this {@code CodeSigner} for equality.
 * Returns {@code true} if the specified object is also an instance of
 * {@code CodeSigner}, the two {@code CodeSigner} encapsulate the same
 * certificate path and the same time stamp, if present in both.
 *
 * @param obj
 *            object to be compared for equality with this {@code
 *            CodeSigner}.
 * @return {@code true} if the specified object is equal to this {@code
 *         CodeSigner}, otherwise {@code false}.
 */
ECode CCodeSigner::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (THIS_PROBE(IInterface) == obj) {
        *result = TRUE;
        return NOERROR;
    }
    if (ICodeSigner::Probe(obj)) {
        AutoPtr<ICodeSigner> that = ICodeSigner::Probe(obj);
        Boolean isEqual;
        if ((mSignerCertPath->Equals(((CCodeSigner*)that.Get())->mSignerCertPath, &isEqual), !isEqual)) {
            *result = FALSE;
            return NOERROR;
        }
        if (mTimestamp == NULL) {
            *result = ((CCodeSigner*)that.Get())->mTimestamp == NULL;
        }
        else {
            mTimestamp->Equals(((CCodeSigner*)that.Get())->mTimestamp, result);
        }
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

/**
 * Returns the certificate path associated with this {@code CodeSigner}.
 *
 * @return the certificate path associated with this {@code CodeSigner}.
 */
ECode CCodeSigner::GetSignerCertPath(
    /* [out] */ ICertPath** certPath)
{
    VALIDATE_NOT_NULL(certPath)
    *certPath = mSignerCertPath;
    return NOERROR;
}

/**
 * Returns the time stamp associated with this {@code CodeSigner}.
 *
 * @return the time stamp associated with this {@code CodeSigner}, maybe
 *         {@code null}.
 */
ECode CCodeSigner::GetTimestamp(
    /* [out] */ ITimestamp** timestamp)
{
    VALIDATE_NOT_NULL(timestamp)
    *timestamp = mTimestamp;
    return NOERROR;
}

/**
 * Returns the hash code value for this {@code CodeSigner}. Returns the same
 * hash code for {@code CodeSigner}s that are equal to each other as
 * required by the general contract of {@link Object#hashCode}.
 *
 * @return the hash code value for this {@code CodeSigner}.
 * @see Object#equals(Object)
 * @see CodeSigner#equals(Object)
 */
ECode CCodeSigner::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    if (mHash == 0) {
        mSignerCertPath->GetHashCode(&mHash);
        Int32 hash = mTimestamp == NULL ? 0 : mTimestamp->GetHashCode(&hash);
        mHash ^= hash;
    }
    *hashCode = mHash;
    return NOERROR;
}

/**
 * Returns a string containing a concise, human-readable description of the
 * this {@code CodeSigner} including its first certificate and its time
 * stamp, if present.
 *
 * @return a printable representation for this {@code CodeSigner}.
 */
ECode CCodeSigner::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    // There is no any special reason for '256' here, it's taken abruptly
    StringBuilder* buf = new StringBuilder(256);
    // The javadoc says nothing, and the others implementations behavior seems as
    // dumping only the first certificate. Well, let's do the same.
    buf->AppendCStr("CodeSigner [");
    AutoPtr<IList> certificates;
    mSignerCertPath->GetCertificates((IList**)&certificates);
    AutoPtr<IInterface> cert;
    certificates->Get(0, (IInterface**)&cert);
    buf->AppendObject(cert);
    if (mTimestamp != NULL) {
        buf->AppendCStr("; ");
        buf->AppendObject(mTimestamp.Get());
    }
    buf->AppendCStr("]");
    buf->ToString(str);
    delete buf;
    return NOERROR;
}

ECode CCodeSigner::constructor()
{
    return NOERROR;
}

/**
 * Constructs a new instance of {@code CodeSigner}.
 *
 * @param signerCertPath
 *            the certificate path associated with this code signer.
 * @param timestamp
 *            the time stamp associated with this code signer, maybe {@code
 *            null}.
 * @throws NullPointerException
 *             if {@code signerCertPath} is {@code null}.
 */
ECode CCodeSigner::constructor(
    /* [in] */ ICertPath* signerCertPath,
    /* [in] */ ITimestamp* timestamp)
{
    if (signerCertPath == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mSignerCertPath = signerCertPath;
    mTimestamp = timestamp;
    return NOERROR;
}

} // end Security
} // end Elastos
