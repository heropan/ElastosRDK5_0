
#include "CRandomAccessFile.h"
#include "CFile.h"
#include "IoUtils.h"
#include "NioUtils.h"
#include <Math.h>
#include "Character.h"
#include <StringBuilder.h>
#include "COsConstants.h"
#include "CIoBridge.h"
#include "CLibcore.h"
#include "CModifiedUtf8.h"

using Elastos::Droid::System::IOsConstants;
using Libcore::IO::COsConstants;
using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IStructStat;
using Libcore::IO::IOs;
using Elastos::Core::Character;
using Elastos::Core::StringBuilder;
using Elastos::IO::Charset::IModifiedUtf8;
using Elastos::IO::Charset::CModifiedUtf8;

namespace Elastos {
namespace IO {

CAR_OBJECT_IMPL(CRandomAccessFile)

CAR_INTERFACE_IMPL(CRandomAccessFile, Object, IRandomAccessFile)

CRandomAccessFile::CRandomAccessFile()
    : mSyncMetadata(FALSE)
    , mMode(0)
{
    mScratch = ArrayOf<Byte>::Alloc(8);
}

CRandomAccessFile::~CRandomAccessFile()
{
    mScratch = NULL;
    // if (guard != null) {
    //     guard.warnIfOpen();
    // }
    Close();
}

ECode CRandomAccessFile::constructor(
    /* [in] */ IFile* file,
    /* [in] */ const String& mode)
{
    AutoPtr<COsConstants> obj;
    FAIL_RETURN(COsConstants::AcquireSingletonByFriend((COsConstants**)&obj));
    AutoPtr<IOsConstants> osConstans = (IOsConstants*)obj.Get();
    Int32 flags;
    if (mode.Equals("r")) {
        osConstans->GetOsConstant(String("O_RDONLY"), &flags);
    }
    else if (mode.Equals("rw") || mode.Equals("rws") || mode.Equals("rwd")) {
        Int32 flag1, flag2;
        osConstans->GetOsConstant(String("O_RDWR"), &flag1);
        osConstans->GetOsConstant(String("O_CREAT"), &flag2);
        flags = flag1 | flag2;

        if (mode.Equals("rws")) {
            // Sync file and metadata with every write
            mSyncMetadata = TRUE;
        }
        else if (mode.Equals("rwd")) {
            // Sync file, but not necessarily metadata
            osConstans->GetOsConstant(String("O_SYNC"), &flag1);
            flags |= flag1;
        }
    }
    else {
//        throw new IllegalArgumentException("Invalid mode: " + mode);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMode = flags;

    AutoPtr<CIoBridge> ioObj;
    FAIL_RETURN(CIoBridge::AcquireSingletonByFriend((CIoBridge**)&ioObj));
    AutoPtr<IIoBridge> ioBridge = (IIoBridge*)ioObj.Get();
    String path;
    file->GetPath(&path);
    Int32 fd;
    FAIL_RETURN(ioBridge->Open(path, flags, &fd));

    CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
    mFd->SetDescriptor(fd);

    // if we are in "rws" mode, attempt to sync file+metadata
    if (mSyncMetadata) {
        mFd->Sync();
    }

    return NOERROR;
}

ECode CRandomAccessFile::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ const String& mode)
{
    AutoPtr<CFile> obj;
    FAIL_RETURN(CFile::NewByFriend(fileName, (CFile**)&obj));
    AutoPtr<IFile> file = (IFile*)obj;
    return constructor(file, mode);
}

ECode CRandomAccessFile::Close()
{
    // guard.close();
    Object::Autolock lock(_m_syncLock);

    Boolean isflag(FALSE);
    if (mChannel != NULL && (IChannel::Probe(mChannel)->IsOpen(&isflag) , isflag)) {
        IChannel::Probe(mChannel)->Close();
        mChannel = NULL;
    }

    return IoBridge::CloseAndSignalBlockedThreads(mFd);
}

ECode CRandomAccessFile::GetChannel(
    /* [out] */ IFileChannel **channel)
{
    VALIDATE_NOT_NULL(channel)
    Object::Autolock lock(_m_syncLock);

    // BEGIN android-added
    if(mChannel == NULL) {
        mChannel = NioUtils::NewFileChannel(IObject::Probe(this), mFd, mMode);
    }
    // END android-added
    *channel = mChannel;
    REFCOUNT_ADD(*channel)
    return NOERROR;
}

ECode CRandomAccessFile::GetFD(
    /* [out] */ IFileDescriptor ** fd)
{
    VALIDATE_NOT_NULL(fd);

    *fd = mFd;
    REFCOUNT_ADD(*fd);
    return NOERROR;
}

ECode CRandomAccessFile::GetFilePointer(
    /* [out] */ Int64* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = -1;

    AutoPtr<COsConstants> obj;
    FAIL_RETURN(COsConstants::AcquireSingletonByFriend((COsConstants**)&obj));
    AutoPtr<IOsConstants> osConstans = (IOsConstants*)obj.Get();
    Int32 mode;
    osConstans->GetOsConstant(String("SEEK_CUR"), &mode);

    AutoPtr<CLibcore> ioObj;
    CLibcore::AcquireSingletonByFriend((CLibcore**)&ioObj);
    AutoPtr<ILibcore> libcore = (ILibcore*)ioObj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    return IoUtils::Libcore2IoECode(os->Lseek(mFd->mDescriptor, 0ll, mode, offset));
}

ECode CRandomAccessFile::GetLength(
    /* [out] */ Int64* len)
{
    VALIDATE_NOT_NULL(len);

    AutoPtr<CLibcore> ioObj;
    CLibcore::AcquireSingletonByFriend((CLibcore**)&ioObj);
    AutoPtr<ILibcore> libcore = (ILibcore*)ioObj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    AutoPtr<IStructStat> stat;
    FAIL_RETURN(IoUtils::Libcore2IoECode(os->Fstat(mFd->mDescriptor, (IStructStat**)&stat)));

    return stat->GetSize(len);
}

ECode CRandomAccessFile::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 byteCount;
    FAIL_RETURN(Read(mScratch, 0, 1, &byteCount))
    *value = byteCount != -1 ? (*mScratch)[0] & 0xff : -1;

    return NOERROR;
}

ECode CRandomAccessFile::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return Read(buffer, 0, buffer->GetLength(), number);
}

ECode CRandomAccessFile::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    AutoPtr<CIoBridge> obj;
    FAIL_RETURN(CIoBridge::AcquireSingletonByFriend((CIoBridge**)&obj));
    AutoPtr<IIoBridge> ioBridge = (IIoBridge*)obj.Get();

    return ioBridge->Read(mFd->mDescriptor, buffer, offset, length, number);
}

ECode CRandomAccessFile::ReadBoolean(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 temp;
    FAIL_RETURN(Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = temp != 0;
    return NOERROR;
}

ECode CRandomAccessFile::ReadByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 temp;
    FAIL_RETURN(Read(&temp));
    if (temp < 0) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    *value = (Byte)temp;
    return NOERROR;
}

ECode CRandomAccessFile::ReadChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    Byte firstChar;
    FAIL_RETURN(ReadByte(&firstChar));
    if ((firstChar & 0x80) == 0) { // ASCII
        *value = (Char32)firstChar;
        return NOERROR;
    }

    Char32 mask, toIgnoreMask;
    Int32 numToRead = 1;
    Char32 utf32 = firstChar;
    for (mask = 0x40, toIgnoreMask = 0xFFFFFF80;
         (firstChar & mask);
         numToRead++, toIgnoreMask |= mask, mask >>= 1) {
        // 0x3F == 00111111
        Byte ch;
        FAIL_RETURN(ReadByte(&ch));
        utf32 = (utf32 << 6) + (ch & 0x3F);
    }
    toIgnoreMask |= mask;
    utf32 &= ~(toIgnoreMask << (6 * (numToRead - 1)));

    *value = utf32;
    return NOERROR;
}

ECode CRandomAccessFile::ReadDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    Int64 i64Value;
    FAIL_RETURN(ReadInt64(&i64Value));
    *value = Elastos::Core::Math::Int64BitsToDouble(i64Value);
    return NOERROR;
}

ECode CRandomAccessFile::ReadFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 i32Value;
    FAIL_RETURN(ReadInt32(&i32Value));
    *value = Elastos::Core::Math::Int32BitsToFloat(i32Value);
    return NOERROR;
}

ECode CRandomAccessFile::ReadFully(
    /* [out] */ ArrayOf<Byte>* buffer)
{
    return ReadFully(buffer, 0, buffer->GetLength());
}

ECode CRandomAccessFile::ReadFully(
    /* [out] */ ArrayOf<byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (buffer == NULL) {
        // throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // used (offset | length) < 0 instead of separate (offset < 0) and
    // (length < 0) check to safe one operation
    if ((offset | length) < 0 || offset > buffer->GetLength() - length) {
        // throw new IndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    Int32 number;
    while (length > 0) {
        FAIL_RETURN(Read(buffer, offset, length, &number));
        if (number < 0) {
            // throw new EOFException();
            return E_EOF_EXCEPTION;
        }
        offset += number;
        length -= number;
    }
    return NOERROR;
}

ECode CRandomAccessFile::ReadInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    //TODO
    //
    // readFully(scratch, 0, SizeOf.INT);
    // return Memory.peekInt(scratch, 0, ByteOrder.BIG_ENDIAN);
    FAIL_RETURN(ReadFully(mScratch, 0, sizeof(Int32)));
    *value = (((*mScratch)[0] & 0xff) << 24) + (((*mScratch)[1] & 0xff) << 16)
            + (((*mScratch)[2] & 0xff) << 8) + ((*mScratch)[3] & 0xff);
    return NOERROR;
}

ECode CRandomAccessFile::ReadLine(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<StringBuilder> line = new StringBuilder(80);
    Boolean foundTerminator = FALSE;
    Int64 unreadPosition = 0;
    while (TRUE) {
        Int32 nextByte;
        FAIL_RETURN(Read(&nextByte));
        switch (nextByte) {
        case -1:
            if (line->GetLength() != 0) {
                *str = line->ToString();
            }
            else {
                *str = NULL;
            }
            return NOERROR;
        case (Byte)'\r':
            if (foundTerminator) {
                Seek(unreadPosition);
                *str = line->ToString();
                return NOERROR;
            }
            foundTerminator = TRUE;
            /* Have to be able to peek ahead one byte */
            GetFilePointer(&unreadPosition);
            break;
        case (Byte)'\n':
            *str = line->ToString();
            return NOERROR;
        default:
            if (foundTerminator) {
                Seek(unreadPosition);
                *str = line->ToString();
                return NOERROR;
            }
            line->AppendChar((Char32)nextByte);
        }
    }
    return NOERROR;
}

ECode CRandomAccessFile::ReadInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    // TODO:
    //
    // readFully(scratch, 0, SizeOf.LONG);
    // return Memory.peekLong(scratch, 0, ByteOrder.BIG_ENDIAN);

    FAIL_RETURN(ReadFully(mScratch, 0, sizeof(Int64)));

    *value = ((Int64)((((*mScratch)[0] & 0xff) << 24) + (((*mScratch)[1] & 0xff) << 16)
            + (((*mScratch)[2] & 0xff) << 8) + ((*mScratch)[3] & 0xff)) << 32)
            + ((Int64)((*mScratch)[4] & 0xff) << 24)
            + (((*mScratch)[5] & 0xff) << 16)
            + (((*mScratch)[6] & 0xff) << 8)
            + ((*mScratch)[7] & 0xff);
    return NOERROR;
}

ECode CRandomAccessFile::ReadInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    // TODO:
    //
    // readFully(scratch, 0, SizeOf.SHORT);
    // return Memory.peekLong(scratch, 0, ByteOrder.BIG_ENDIAN);

    FAIL_RETURN(ReadFully(mScratch, 0, sizeof(Int16)));

    *value = (Int16)((((*mScratch)[0] & 0xff) << 8) + ((*mScratch)[1] & 0xff));
    return NOERROR;
}

ECode CRandomAccessFile::ReadUnsignedByte(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(Read(value));
    return *value < 0 ? E_EOF_EXCEPTION : NOERROR;
}

ECode CRandomAccessFile::ReadUnsignedInt16(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int16 temp;
    FAIL_RETURN(ReadInt16(&temp));
    *value = ((Int32)temp) & 0xFFFF;

    return NOERROR;
}

ECode CRandomAccessFile::ReadUTF(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Int32 utfSize = 0;
    FAIL_RETURN(ReadUnsignedInt16(&utfSize));
    if (utfSize == 0) {
        *str = "";
        return NOERROR;
    }
    AutoPtr< ArrayOf<Byte> > buf = ArrayOf<Byte>::Alloc(utfSize);
    Int32 readsize = 0;
    ReadBytes(buf, 0, buf->GetLength(), &readsize);
    if (readsize != buf->GetLength()) {
        // throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    AutoPtr<IModifiedUtf8> mutf8help;
    CModifiedUtf8::AcquireSingleton((IModifiedUtf8**)&mutf8help);
    AutoPtr< ArrayOf<Char8> > charbuf = ArrayOf<Char8>::Alloc(utfSize);
    return mutf8help->Decode(buf, charbuf, 0, utfSize, str);
}

ECode CRandomAccessFile::Seek(
    /* [in] */ Int64 offset)
{
    if (offset < 0) {
        // seek position is negative
        // throw new IOException("offset < 0");
        return E_IO_EXCEPTION;
    }

    AutoPtr<COsConstants> obj;
    FAIL_RETURN(COsConstants::AcquireSingletonByFriend((COsConstants**)&obj));
    AutoPtr<IOsConstants> osConstans = (IOsConstants*)obj.Get();
    Int32 mode;
    osConstans->GetOsConstant(String("SEEK_SET"), &mode);

    AutoPtr<CLibcore> ioObj;
    CLibcore::AcquireSingletonByFriend((CLibcore**)&ioObj);
    AutoPtr<ILibcore> libcore = (ILibcore*)ioObj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    Int64 res;
    return IoUtils::Libcore2IoECode(os->Lseek(mFd->mDescriptor, offset, mode, &res));
}

ECode CRandomAccessFile::SetLength(
   /* [in] */ Int64 newLength)
{
    if (newLength < 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<CLibcore> ioObj;
    CLibcore::AcquireSingletonByFriend((CLibcore**)&ioObj);
    AutoPtr<ILibcore> libcore = (ILibcore*)ioObj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    FAIL_RETURN(IoUtils::Libcore2IoECode(os->Ftruncate(mFd->mDescriptor, newLength)));

    Int64 filePointer;
    FAIL_RETURN(GetFilePointer(&filePointer));
    if (filePointer > newLength) {
        FAIL_RETURN(Seek(newLength));
    }

    // if we are in "rws" mode, attempt to sync file+metadata
    if (mSyncMetadata) {
        mFd->Sync();
    }
    return NOERROR;
}

ECode CRandomAccessFile::SkipBytes(
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (count > 0) {
        Int64 currentPos, eof;
        GetFilePointer(&currentPos);
        GetLength(&eof);
        Int32 newCount = (Int32)((currentPos + count > eof) ? eof - currentPos
                : count);
        FAIL_RETURN(Seek(currentPos + newCount));
        *number = newCount;
        return NOERROR;
    }
    *number = 0;
    return NOERROR;
}

ECode CRandomAccessFile::Write(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    return Write(buffer, 0, buffer->GetLength());
}

ECode CRandomAccessFile::Write(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    AutoPtr<CIoBridge> obj;
    FAIL_RETURN(CIoBridge::AcquireSingletonByFriend((CIoBridge**)&obj));
    AutoPtr<IIoBridge> ioBridge = (IIoBridge*)obj.Get();

    FAIL_RETURN(ioBridge->Write(mFd->mDescriptor, buffer, offset, count));

    // if we are in "rws" mode, attempt to sync file+metadata
    if (mSyncMetadata) {
        mFd->Sync();
    }
    return NOERROR;
}

ECode CRandomAccessFile::Write(
    /* [in] */ Int32 oneByte)
{
    (*mScratch)[0] = (Byte)(oneByte & 0xFF);
    return Write(mScratch, 0, 1);
}

ECode CRandomAccessFile::WriteBoolean(
    /* [in] */ Boolean value)
{
    return Write(value? 1 : 0);
}

ECode CRandomAccessFile::WriteByte(
    /* [in] */ Int32 value)
{
    return Write(value & 0xff);
}

ECode CRandomAccessFile::WriteChar(
    /* [in] */ Int32 value)
{
    ArrayOf_<Byte, 4> buffer;
    Int32 len;
    Character::ToChars((Char32)value, *(ArrayOf<Char8>*)&buffer, 0, &len);
    return Write(buffer, 0, len);
}

ECode CRandomAccessFile::WriteBytes(
    /* [in] */ const String& str)
{
    if (str.IsNullOrEmpty())
        return NOERROR;

    AutoPtr<ArrayOf<Char32> > chs = str.GetChars();
    return Write((ArrayOf<Byte>*)chs.Get());
}

ECode CRandomAccessFile::WriteDouble(
    /* [in] */ Double value)
{
    return WriteInt64(Elastos::Core::Math::DoubleToInt64Bits(value));
}

ECode CRandomAccessFile::WriteFloat(
    /* [in] */ Float value)
{
    return WriteInt32(Elastos::Core::Math::FloatToInt32Bits(value));
}

ECode CRandomAccessFile::WriteInt32(
    /* [in] */ Int32 value)
{
    //TODO:
    //
    // Memory.pokeInt(scratch, 0, val, ByteOrder.BIG_ENDIAN);
    // write(scratch, 0, SizeOf.INT);

    (*mScratch)[0] = (Byte)(value >> 24);
    (*mScratch)[1] = (Byte)(value >> 16);
    (*mScratch)[2] = (Byte)(value >> 8);
    (*mScratch)[3] = (Byte)value;

    return Write(mScratch, 0, sizeof(Int32));
}

ECode CRandomAccessFile::WriteInt64(
    /* [in] */ Int64 value)
{
    //TODO:
    //
    // Memory.pokeLong(scratch, 0, val, ByteOrder.BIG_ENDIAN);
    // write(scratch, 0, SizeOf.LONG);

    Int32 t = (Int32)(value >> 32);
    (*mScratch)[0] = (Byte)(t >> 24);
    (*mScratch)[1] = (Byte)(t >> 16);
    (*mScratch)[2] = (Byte)(t >> 8);
    (*mScratch)[3] = (Byte)t;
    (*mScratch)[4] = (Byte)(value >> 24);
    (*mScratch)[5] = (Byte)(value >> 16);
    (*mScratch)[6] = (Byte)(value >> 8);
    (*mScratch)[7] = (Byte)value;

    return Write(mScratch, 0, sizeof(Int64));
}

ECode CRandomAccessFile::WriteInt16(
    /* [in] */ Int32 value)
{
    //TODO:
    //
    // Memory.pokeShort(scratch, 0, val, ByteOrder.BIG_ENDIAN);
    // write(scratch, 0, SizeOf.SHORT);

    (*mScratch)[0] = (Byte)(value >> 8);
    (*mScratch)[1] = (Byte)value;

    return Write(mScratch, 0, sizeof(Int32));
}

ECode CRandomAccessFile::WriteUTF(
    /* [in] */ const String& str)
{
    AutoPtr<IModifiedUtf8> utf8help;
    CModifiedUtf8::AcquireSingleton((IModifiedUtf8**)&utf8help);
    AutoPtr<ArrayOf<Byte> > bytes;
    utf8help->Encode(&str, (ArrayOf<Byte>**)&bytes);
    return Write(bytes);
}

} // namespace IO
} // namespace Elastos
