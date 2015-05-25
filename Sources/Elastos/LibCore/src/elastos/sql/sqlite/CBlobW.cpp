#include "CBlobW.h"
#include <cmdef.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

CBlobW::CBlobW()
    : mPos(0)
{
}

ECode CBlobW::constructor(
    /* [in] */ Elastos::Sql::SQLite::IBlob* blob)
{
    mBlob = (CBlob *)blob;
    return NOERROR;
}

ECode CBlobW::Write(
    /* [in] */ Int32 oneByte)
{
    AutoPtr<ArrayOf<Byte> > b = ArrayOf<Byte>::Alloc(1);
    (*b)[0] = (Byte) oneByte;
    mPos += mBlob->Write(b, 0, mPos, 1);
    return NOERROR;
}

ECode CBlobW::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    if (buffer.GetLength() > 0) {
        mPos += mBlob->Write(const_cast<ArrayOf<Byte> *>(&buffer), 0, mPos, buffer.GetLength());
    }
    return NOERROR;
}

ECode CBlobW::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    if (off + len > b.GetLength()) {
        len = b.GetLength() - off;
    }
    if (len <= 0) {
       return NOERROR;
    }
    mPos += mBlob->Write(const_cast<ArrayOf<Byte> *>(&b), off, mPos, len);

    return NOERROR;
}

ECode CBlobW::CheckError(
    /* [out] */ Boolean* hasError)
{
    return OutputStream::CheckError(hasError);
}

ECode CBlobW::Close()
{
    mBlob->Close();
    mBlob = NULL;
    mPos = 0;
    return NOERROR;
}

ECode CBlobW::Flush()
{
    return NOERROR;
}

ECode CBlobW::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = OutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

PInterface CBlobW::Probe(
    /* [in] */ REIID riid)
{
    return _CBlobW::Probe(riid);
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
