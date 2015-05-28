
#include "coredef.h"
#include "CDataOutputStream.h"

namespace Elastos {
namespace IO {

CDataOutputStream::CDataOutputStream()
{
}
CDataOutputStream::~CDataOutputStream()
{
}

ECode CDataOutputStream::constructor(
    /* [in] */ IOutputStream* out)
{
    return DataOutputStream::Init(out);
}

PInterface CDataOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CDataOutputStream::Probe(riid);
}

ECode CDataOutputStream::Close()
{
    return DataOutputStream::Close();
}

ECode CDataOutputStream::Flush()
{
    return DataOutputStream::Flush();
}

ECode CDataOutputStream::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return DataOutputStream::GetSize(size);
}

ECode CDataOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return DataOutputStream::Write(oneByte);
}

ECode CDataOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return DataOutputStream::WriteBytes(buffer);
}

ECode CDataOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return DataOutputStream::WriteBytes(buffer, offset, count);
}

ECode CDataOutputStream::CheckError(
     /* [out] */ Boolean* checkerror)
{
    VALIDATE_NOT_NULL(checkerror);

    return DataOutputStream::CheckError(checkerror);
}

ECode CDataOutputStream::WriteBoolean(
    /* [in] */ Boolean val)
{
    return DataOutputStream::WriteBoolean(val);
}

ECode CDataOutputStream::WriteByte(
    /* [in] */ Int32 val)
{
    return DataOutputStream::WriteByte(val);
}

ECode CDataOutputStream::WriteBytesFromString(
    /* [in] */ const String& str)
{
    return DataOutputStream::WriteBytesFromString(str);
}

ECode CDataOutputStream::WriteChar(
    /* [in] */ Int32 val)
{
    return DataOutputStream::WriteChar(val);
}

ECode CDataOutputStream::WriteChars(
    /* [in] */ const String& str)
{
    return DataOutputStream::WriteChars(str);
}

ECode CDataOutputStream::WriteDouble(
    /* [in] */ Double val)
{
    return DataOutputStream::WriteDouble(val);
}

ECode CDataOutputStream::WriteFloat(
    /* [in] */ Float val)
{
    return DataOutputStream::WriteFloat(val);
}

ECode CDataOutputStream::WriteInt32(
    /* [in] */ Int32 val)
{
    return DataOutputStream::WriteInt32(val);
}

ECode CDataOutputStream::WriteInt64(
    /* [in] */ Int64 val)
{
    return DataOutputStream::WriteInt64(val);
}

ECode CDataOutputStream::WriteInt16(
    /* [in] */ Int32 val)
{
    return DataOutputStream::WriteInt16(val);
}

ECode CDataOutputStream::WriteUTF(
    /* [in] */ const String& str)
{
    return DataOutputStream::WriteUTF(str);
}

ECode CDataOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = DataOutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
