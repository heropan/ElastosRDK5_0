
#include "FilterOutputStream.h"

namespace Elastos {
namespace IO {

extern "C" const InterfaceID EIID_FilterOutputStream =
    { 0x1e66b62f, 0x2203, 0x4ca0, { 0xa5, 0xff, 0xc1, 0x1c, 0x50, 0x0a, 0x31, 0x01 } };

CAR_INTERFACE_IMPL(FilterOutputStream, IFilterOutputStream)

FilterOutputStream::FilterOutputStream()
{
}

FilterOutputStream::~FilterOutputStream()
{
}

ECode FilterOutputStream::constructor(
    /* [in] */ IOutputStream* outs)
{
    VALIDATE_NOT_NULL(out);
    mOut = outs;
    return NOERROR;
}

ECode FilterOutputStream::Close()
{
    ECode ec = IFlushable::Probe(mOut)->Flush();
    ECode ec2 = ICloseable::Probe(mOut)->Close();
    if(ec != NOERROR || ec2 != NOERROR){
        return ec != NOERROR ? ec : ec2;
    }
    return NOERROR;
}

ECode FilterOutputStream::Flush()
{
    return IFlushable::Probe(mOut)->Flush();
}

ECode FilterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return mOut->Write(oneByte);
}

ECode FilterOutputStream::Write(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // Force null buffer check first!
    if (offset > buffer.GetLength() || offset < 0) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (count < 0 || count > buffer.GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    for (Int32 i = 0; i < count; i++) {
        // Call write() instead of out.write() since subclasses could
        // override the write() method.
        Write(buffer[offset + i]);
    }

    return NOERROR;
}

ECode FilterOutputStream::ToString(
/* [out] */ String* s)
{
    assert(s != NULL);
    *s = String("FilterOutputStream");
    return NOERROR;
}



ECode FilterOutputStream::GetClassID(
/* [out] */ ClassID *pCLSID)
{
    assert(pCLSID != NULL);
    *pCLSID = EIID_FilterOutputStream;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
