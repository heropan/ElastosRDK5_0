
#include "CInflaterOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {


ECode CInflaterOutputStream::Close()
{
    return InflaterOutputStream::Close();
}

ECode CInflaterOutputStream::Flush()
{
    return InflaterOutputStream::Flush();
}

ECode CInflaterOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return InflaterOutputStream::Write(oneByte);
}

ECode CInflaterOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return InflaterOutputStream::WriteBytes(buffer);
}

ECode CInflaterOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return InflaterOutputStream::WriteBytes(buffer, offset, count);
}

ECode CInflaterOutputStream::Finish()
{
    return InflaterOutputStream::Finish();
}

ECode CInflaterOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    return InflaterOutputStream::CheckError(hasError);
}

ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    return InflaterOutputStream::Init(out);
}

ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf)
{
    return InflaterOutputStream::Init(out, inf);
}

ECode CInflaterOutputStream::constructor(
    /* [in] */ IOutputStream* out,
    /* [in] */ IInflater* inf,
    /* [in] */ Int32 bufferSize)
{
    return InflaterOutputStream::Init(out, inf, bufferSize);
}

ECode CInflaterOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InflaterOutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

PInterface CInflaterOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CInflaterOutputStream::Probe(riid);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
