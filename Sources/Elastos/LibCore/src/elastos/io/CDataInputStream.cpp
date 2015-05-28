
#include "coredef.h"
#include "CDataInputStream.h"

namespace Elastos {
namespace IO {

CDataInputStream::CDataInputStream()
{
}

CDataInputStream::~CDataInputStream()
{
}

ECode CDataInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return DataInputStream::Init(is);
}

ECode CDataInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return DataInputStream::Available(number);
}

ECode CDataInputStream::Close()
{
    return DataInputStream::Close();
}

ECode CDataInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return DataInputStream::Mark(readLimit);
}

ECode CDataInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return DataInputStream::IsMarkSupported(supported);
}

ECode CDataInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::Read(value);
}

ECode CDataInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return DataInputStream::ReadBytes(buffer, number);
}

ECode CDataInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return DataInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CDataInputStream::Reset()
{
    return DataInputStream::Reset();
}

ECode CDataInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return DataInputStream::Skip(count, number);
}

ECode CDataInputStream::ReadBoolean(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadBoolean(value);
}

ECode CDataInputStream::ReadByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadByte(value);
}

ECode CDataInputStream::ReadChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadChar(value);
}

ECode CDataInputStream::ReadDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadDouble(value);
}

ECode CDataInputStream::ReadFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadFloat(value);
}

ECode CDataInputStream::ReadFully(
    /* [out] */ ArrayOf<Byte> * buffer)
{
    VALIDATE_NOT_NULL(buffer);

    return DataInputStream::ReadFully(buffer);
}

ECode CDataInputStream::ReadFully(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    VALIDATE_NOT_NULL(buffer);

    return DataInputStream::ReadFully(buffer, offset, length);
}

ECode CDataInputStream::ReadInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadInt32(value);
}

ECode CDataInputStream::ReadLine(
     /* [out] */ String* value)
{
    return DataInputStream::ReadLine(value);
}

ECode CDataInputStream::ReadInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadInt64(value);
}

ECode CDataInputStream::ReadInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadInt16(value);
}

ECode CDataInputStream::ReadUnsignedByte(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadUnsignedByte(value);
}

ECode CDataInputStream::ReadUnsignedInt16(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return DataInputStream::ReadUnsignedInt16(value);
}

ECode CDataInputStream::ReadUTF(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return DataInputStream::ReadUTF(str);
}

ECode CDataInputStream::SkipBytes(
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return DataInputStream::SkipBytes(count, number);
}

PInterface CDataInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CDataInputStream::Probe(riid);
}

ECode CDataInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = DataInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
