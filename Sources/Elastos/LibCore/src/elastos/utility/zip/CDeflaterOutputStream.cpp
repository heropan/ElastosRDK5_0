
#include "CDeflaterOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {


PInterface CDeflaterOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CDeflaterOutputStream::Probe(riid);
}

ECode CDeflaterOutputStream::Close()
{
    return DeflaterOutputStream::Close();
}

ECode CDeflaterOutputStream::Finish()
{
    return DeflaterOutputStream::Finish();
}

ECode CDeflaterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return DeflaterOutputStream::Write(oneByte);
}

ECode CDeflaterOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return DeflaterOutputStream::WriteBytes(buffer);
}

ECode CDeflaterOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return DeflaterOutputStream::WriteBytes(buffer, offset, count);
}

ECode CDeflaterOutputStream::Flush()
{
    return DeflaterOutputStream::Flush();
}

ECode CDeflaterOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    return DeflaterOutputStream::CheckError(hasError);
}

ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return DeflaterOutputStream::Init(os);
}

ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def)
{
    return DeflaterOutputStream::Init(os, def);
}

ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Int32 bsize)
{
    return DeflaterOutputStream::Init(os, def, bsize);
}

ECode CDeflaterOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = DeflaterOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
