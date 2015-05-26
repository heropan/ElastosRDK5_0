#include "CBlobW.h"
#include <coredef.h>

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CBlobW);

CBlobW::CBlobW()
    : mPos(0)
{
}

PInterface CBlobW::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IBlobW) {
        return (IInterface*)(IBlobW*)this;
    }
    return OutputStream::Probe(riid);
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

ECode CBlobW::Close()
{
    mBlob->Close();
    mBlob = NULL;
    mPos = 0;
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
