
#include "content/pm/CMacAuthenticatedInputStream.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CMacAuthenticatedInputStream::CMacAuthenticatedInputStream()
{}

CMacAuthenticatedInputStream::~CMacAuthenticatedInputStream()
{}

PInterface CMacAuthenticatedInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CMacAuthenticatedInputStream::Probe(riid);
}

ECode CMacAuthenticatedInputStream::constructor(
    /* [in] */ IInputStream* inStream,
    /* [in] */ IMac* mac)
{
    FilterInputStream::Init(inStream);

    mMac = mac;

    return NOERROR;
}

ECode CMacAuthenticatedInputStream::IsTagEqual(
    /* [in] */ ArrayOf<Byte>* tag,
    /* [out] */ Boolean* result)
{
    AutoPtr<ArrayOf<Byte> > actualTag;
    // TODO:
    // mMac->DoFinal(&actualTag);

    if (tag == NULL || actualTag == NULL || tag->GetLength() != actualTag->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }

    /*
     * Attempt to prevent timing attacks by doing the same amount of work
     * whether the first byte matches or not. Do not change this to a loop
     * that exits early when a byte does not match.
     */
    Int32 value = 0;
    for (Int32 i = 0; i < tag->GetLength(); i++) {
        value |= (*tag)[i] ^ (*actualTag)[i];
    }

    *result = value == 0;
    return NOERROR;
}

ECode CMacAuthenticatedInputStream::Read(
    /* [out] */ Int32* value)
{
    Int32 b;
    FilterInputStream::Read(&b);
    if (b >= 0) {
        // TODO:
        // mMac->Update((Byte) b);
    }
    *value = b;
    return NOERROR;
}

ECode CMacAuthenticatedInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    Int32 numRead;
    FilterInputStream::ReadBytes(buffer, offset, count, &numRead);
    if (numRead > 0) {
        // TODO:
        // mMac->Update(buffer, offset, numRead);
    }
    *number = numRead;
    return NOERROR;
}

ECode CMacAuthenticatedInputStream::Close()
{
    return FilterInputStream::Close();
}

ECode CMacAuthenticatedInputStream::Available(
    /* [out] */ Int32* number)
{
    return FilterInputStream::Available(number);
}

ECode CMacAuthenticatedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return FilterInputStream::Mark(readLimit);
}

ECode CMacAuthenticatedInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return FilterInputStream::IsMarkSupported(supported);
}

ECode CMacAuthenticatedInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return FilterInputStream::ReadBytes(buffer, number);
}

ECode CMacAuthenticatedInputStream::Reset()
{
    return FilterInputStream::Reset();
}

ECode CMacAuthenticatedInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return FilterInputStream::Skip(byteCount, number);
}

ECode CMacAuthenticatedInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FilterInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

}
}
}
}

