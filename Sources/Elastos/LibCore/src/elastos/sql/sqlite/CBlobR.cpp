#include "CBlobR.h"
#include <cmdef.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

PInterface CBlobR::Probe(
    /* [in] */ REIID riid)
{
    return _CBlobR::Probe(riid);
}

ECode CBlobR::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

ECode CBlobR::constructor(
    /* [in] */ Elastos::Sql::SQLite::IBlob* blob)
{
    mBlob = (CBlob *)blob;
    pos = 0;
    return NOERROR;
}

ECode CBlobR::Available(
    /* [out] */ Int32* number)
{
    Int32 ret = mBlob->size - pos;
    *number = (ret < 0) ? 0 : ret;
    return NOERROR;
}

ECode CBlobR::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode CBlobR::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    *supported = FALSE;
    return NOERROR;
}

ECode CBlobR::Read(
    /* [out] */ Int32* value)
{
    AutoPtr<ArrayOf<Byte> > b = ArrayOf<Byte>::Alloc(1);
    Int32 n = mBlob->Read(b, 0, pos, b->GetLength());
    if (n > 0) {
        pos += n;
        *value = (*b)[0];
    }
    *value = -1;
    return NOERROR;
}

ECode CBlobR::ReadBytes(
    /* [out] */ ArrayOf<Byte> * buffer,
    /* [out] */ Int32* number)
{
    Int32 n = mBlob->Read(buffer, 0, pos, buffer->GetLength());
    if (n > 0) {
        pos += n;
        *number = n;
    }
    *number = -1;
    return NOERROR;
}

ECode CBlobR::ReadBytes(
    /* [out] */ ArrayOf<Byte> * b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ Int32* number)
{
    if (off + len > b->GetLength()) {
        len = b->GetLength() - off;
    }
    if (len < 0) {
        *number = -1;
    }
    if (len == 0) {
        *number = 0;
    }
    Int32 n = mBlob->Read(b, off, pos, len);
    if (n > 0) {
        pos += n;
        *number = n;
    }
    *number = -1;
    return NOERROR;
}

ECode CBlobR::Reset()
{
    return NOERROR;
}

ECode CBlobR::Close()
{
    mBlob->Close();
    mBlob = NULL;
    pos = 0;
    return NOERROR;
}

ECode CBlobR::Skip(
    /* [in] */ Int64 n,
    /* [out] */ Int64* number)
{
    Int64 ret = pos + n;
    if (ret < 0) {
        ret = 0;
        pos = 0;
    } else if (ret > mBlob->size) {
        ret = mBlob->size;
        pos = mBlob->size;
    } else {
        pos = (Int32) ret;
    }
    *number = ret;
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
