
#include "ext/frameworkext.h"
#include "os/CParcel.h"
#include <elastos/Slogger.h>
#include <fcntl.h>
#include <sys/stat.h>

using Elastos::IO::CFileDescriptor;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Os {

ECode CParcel::constructor()
{
    return Elastos::CParcel::New((IParcel**)&mData);
}

ECode CParcel::Marshall(
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    return mData->Marshall(bytes);
}

ECode CParcel::Unmarshall(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return mData->Unmarshall(data, offset, length);
}

ECode CParcel::AppendFrom(
    /* [in] */ IParcel* parcel,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return mData->AppendFrom(parcel, offset, length);
}

ECode CParcel::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    return mData->HasFileDescriptors(result);
}

ECode CParcel::ReadByte(
    /* [out] */ Byte* value)
{
   return mData->ReadByte(value);
}

ECode CParcel::ReadBoolean(
    /* [out] */ Boolean* value)
{
    return mData->ReadBoolean(value);
}

ECode CParcel::ReadChar8(
    /* [out] */ Char8* value)
{
    return mData->ReadChar8(value);
}

ECode CParcel::ReadChar16(
    /* [out] */ Char16* value)
{
    return mData->ReadChar16(value);
}

ECode CParcel::ReadChar32(
    /* [out] */ Char32* value)
{
    return mData->ReadChar32(value);
}

ECode CParcel::ReadInt16(
    /* [out] */ Int16* value)
{
    return mData->ReadInt16(value);
}

ECode CParcel::ReadInt32(
    /* [out] */ Int32* value)
{
    return mData->ReadInt32(value);
}

ECode CParcel::ReadInt64(
    /* [out] */ Int64* value)
{
    return mData->ReadInt64(value);
}

ECode CParcel::ReadFloat(
    /* [out] */ Float* value)
{
    return mData->ReadFloat(value);
}

ECode CParcel::ReadDouble(
    /* [out] */ Double* value)
{
    return mData->ReadDouble(value);
}

ECode CParcel::ReadString(
    /* [out] */ String* str)
{
    return mData->ReadString(str);
}

ECode CParcel::ReadStruct(
    /* [out] */ Handle32* addr)
{
    return mData->ReadStruct(addr);
}

ECode CParcel::ReadEMuid(
    /* [out] */ EMuid* id)
{
    return mData->ReadEMuid(id);
}

ECode CParcel::ReadEGuid(
    /* [out] */ EGuid* id)
{
    return mData->ReadEGuid(id);
}

ECode CParcel::ReadInterfacePtr(
    /* [out] */ Handle32* itfPtr)
{
    return mData->ReadInterfacePtr(itfPtr);
}

ECode CParcel::ReadArrayOf(
    /* [out] */ Handle32* array)
{
    return mData->ReadArrayOf(array);
}

ECode CParcel::ReadArrayOfString(
    /* [out, callee] */ ArrayOf<String>** array)
{
    return mData->ReadArrayOfString(array);
}

ECode CParcel::ReadBufferOf(
    /* [out] */ Handle32* buffer)
{
    return mData->ReadBufferOf(buffer);
}

ECode CParcel::ReadBufferOfCString(
    /* [out] */ Handle32* buffer)
{
    return mData->ReadBufferOfCString(buffer);
}

ECode CParcel::ReadBufferOfString(
    /* [out] */ Handle32* buffer)
{
    return mData->ReadBufferOfString(buffer);
}

ECode CParcel::ReadMemoryBuf(
    /* [out] */ Handle32* buffer)
{
    return mData->ReadMemoryBuf(buffer);
}

ECode CParcel::ReadStringBuf(
    /* [out] */ Handle32* buffer)
{
    return mData->ReadStringBuf(buffer);
}

ECode CParcel::ReadBytePtr(
    /* [out] */ Handle32* bypp)
{
    return mData->ReadBytePtr(bypp);
}

ECode CParcel::ReadBooleanPtr(
    /* [out] */ Handle32* bopp)
{
    return mData->ReadBooleanPtr(bopp);
}

ECode CParcel::ReadChar8Ptr(
    /* [out] */ Handle32* c8pp)
{
    return mData->ReadChar8Ptr(c8pp);
}

ECode CParcel::ReadChar16Ptr(
    /* [out] */ Handle32* c16pp)
{
    return mData->ReadChar16Ptr(c16pp);
}

ECode CParcel::ReadInt16Ptr(
    /* [out] */ Handle32* i16pp)
{
    return mData->ReadInt16Ptr(i16pp);
}

ECode CParcel::ReadInt32Ptr(
    /* [out] */ Handle32* i32pp)
{
    return mData->ReadInt32Ptr(i32pp);
}

ECode CParcel::ReadInt64Ptr(
    /* [out] */ Handle32* i64pp)
{
    return mData->ReadInt64Ptr(i64pp);
}

ECode CParcel::ReadFloatPtr(
    /* [out] */ Handle32* fpp)
{
    return mData->ReadFloatPtr(fpp);
}

ECode CParcel::ReadDoublePtr(
    /* [out] */ Handle32* dpp)
{
    return mData->ReadDoublePtr(dpp);
}

ECode CParcel::ReadStringPtr(
    /* [out] */ Handle32* spp)
{
    return mData->ReadStringPtr(spp);
}

ECode CParcel::ReadStructPtr(
    /* [out] */ Handle32* addr)
{
    return mData->ReadStructPtr(addr);
}

ECode CParcel::ReadEMuidPtr(
    /* [out] */ Handle32* idpp)
{
    return mData->ReadEMuidPtr(idpp);
}

ECode CParcel::ReadEGuidPtr(
    /* [out] */ Handle32* idpp)
{
    return mData->ReadEGuidPtr(idpp);
}

ECode CParcel::ReadInterfacePtrPtr(
    /* [out] */ Handle32* itfpp)
{
    return mData->ReadInterfacePtrPtr(itfpp);
}

ECode CParcel::ReadBufferOfPtr(
    /* [out] */ Handle32* buffer)
{
    return mData->ReadBufferOfPtr(buffer);
}

// Retrieve a file descriptor from the parcel.  This returns the raw fd
// in the parcel, which you do not own -- use dup() to get your own copy.
ECode CParcel::ReadFileDescriptor(
    /* [out] */ Int32* fd)
{
    return mData->ReadFileDescriptor(fd);
}

ECode CParcel::WriteByte(
    /* [in] */ Byte value)
{
    return mData->WriteByte(value);
}

ECode CParcel::WriteBoolean(
    /* [in] */ Boolean value)
{
    return mData->WriteBoolean(value);
}

ECode CParcel::WriteChar8(
    /* [in] */ Char8 value)
{
    return mData->WriteChar8(value);
}

ECode CParcel::WriteChar16(
    /* [in] */ Char16 value)
{
    return mData->WriteChar16(value);
}

ECode CParcel::WriteChar32(
    /* [in] */ Char32 value)
{
    return mData->WriteChar32(value);
}

ECode CParcel::WriteInt16(
    /* [in] */ Int16 value)
{
    return mData->WriteInt16(value);
}

ECode CParcel::WriteInt32(
    /* [in] */ Int32 value)
{
    return mData->WriteInt32(value);
}

ECode CParcel::WriteInt64(
    /* [in] */ Int64 value)
{
    return mData->WriteInt64(value);
}

ECode CParcel::WriteFloat(
    /* [in] */ Float value)
{
    return mData->WriteFloat(value);
}

ECode CParcel::WriteDouble(
    /* [in] */ Double value)
{
    return mData->WriteDouble(value);
}

ECode CParcel::WriteString(
    /* [in] */ const String& str)
{
    return mData->WriteString(str);
}

ECode CParcel::WriteInterfacePtr(
    /* [in] */ IInterface* value)
{
    return mData->WriteInterfacePtr(value);
}

ECode CParcel::WriteStruct(
    /* [in] */ Handle32 value,
    /* [in] */ Int32 size)
{
    return mData->WriteStruct(value, size);
}

ECode CParcel::WriteEMuid(
    /* [in] */ const EMuid& id)
{
    return mData->WriteEMuid(id);
}

ECode CParcel::WriteEGuid(
    /* [in] */ const EGuid& id)
{
    return mData->WriteEGuid(id);
}

ECode CParcel::WriteArrayOf(
    /* [in] */ Handle32 array)
{
    return mData->WriteArrayOf(array);
}

ECode CParcel::WriteArrayOfString(
    /* [in] */ ArrayOf<String>* array)
{
    return mData->WriteArrayOfString(array);
}

ECode CParcel::WriteBufferOf(
    /* [in] */ Handle32 buffer)
{
    return mData->WriteBufferOf(buffer);
}

ECode CParcel::WriteBufferOfCString(
    /* [in] */ const BufferOf<CString>& buffer)
{
    return mData->WriteBufferOfCString(buffer);
}

ECode CParcel::WriteBufferOfString(
    /* [in] */ const BufferOf<String>& buffer)
{
    return mData->WriteBufferOfString(buffer);
}

ECode CParcel::WriteMemoryBuf(
    /* [in] */ Handle32 buffer)
{
    return mData->WriteMemoryBuf(buffer);
}

ECode CParcel::WriteStringBuf(
    /* [in] */ Handle32 buffer)
{
    return mData->WriteStringBuf(buffer);
}

ECode CParcel::WriteBytePtr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteBytePtr(addr);
}

ECode CParcel::WriteBooleanPtr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteBooleanPtr(addr);
}

ECode CParcel::WriteChar8Ptr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteChar8Ptr(addr);
}

ECode CParcel::WriteChar16Ptr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteChar16Ptr(addr);
}

ECode CParcel::WriteInt16Ptr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteInt16Ptr(addr);
}

ECode CParcel::WriteInt32Ptr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteInt32Ptr(addr);
}

ECode CParcel::WriteInt64Ptr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteInt64Ptr(addr);
}

ECode CParcel::WriteFloatPtr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteFloatPtr(addr);
}

ECode CParcel::WriteDoublePtr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteDoublePtr(addr);
}

ECode CParcel::WriteStringPtr(
    /* [in] */ Handle32 addr)
{
    return mData->WriteStringPtr(addr);
}

ECode CParcel::WriteEMuidPtr(
    /* [in] */ Handle32 idPtr)
{
    return mData->WriteEMuidPtr(idPtr);
}

ECode CParcel::WriteEGuidPtr(
    /* [in] */ Handle32 idPtr)
{
    return mData->WriteEGuidPtr(idPtr);
}

ECode CParcel::WriteInterfacePtrPtr(
    /* [in] */ Handle32 itfpp)
{
    return mData->WriteInterfacePtrPtr(itfpp);
}

ECode CParcel::WriteFileDescriptor(
    /* [in] */ Int32 fd)
{
    return mData->WriteFileDescriptor(fd);
}

ECode CParcel::WriteDupFileDescriptor(
    /* [in] */ Int32 fd)
{
    return mData->WriteDupFileDescriptor(fd);
}

ECode CParcel::Clone(
    /* [in] */ IParcel* srcParcel)
{
    return mData->Clone(srcParcel);
}

ECode CParcel::GetDataPosition(
    /* [out] */ Int32* position)
{
    return mData->GetDataPosition(position);
}

ECode CParcel::SetDataPosition(
    /* [in] */ Int32 position)
{
    return mData->SetDataPosition(position);
}

ECode CParcel::GetElementPayload(
    /* [out] */ Handle32* buffer)
{
    return mData->GetElementPayload(buffer);
}

ECode CParcel::GetElementSize(
    /* [out] */ Int32* size)
{
    return mData->GetElementSize(size);
}

ECode CParcel::OpenFileDescriptor(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFileDescriptor** desc)
{
    VALIDATE_NOT_NULL(desc);
    *desc = NULL;

    if (name.IsNull()) {
        Slogger::E("CParcel", "OpenFileDescriptor filename is null");
        return E_NULL_POINTER_EXCEPTION;
    }

    int flags = 0;
    switch (mode & 0x30000000) {
        case 0:
        case 0x10000000:
            flags = O_RDONLY;
            break;
        case 0x20000000:
            flags = O_WRONLY;
            break;
        case 0x30000000:
            flags = O_RDWR;
            break;
    }

    if (mode & 0x08000000) flags |= O_CREAT;
    if (mode & 0x04000000) flags |= O_TRUNC;
    if (mode & 0x02000000) flags |= O_APPEND;

    int realMode = S_IRWXU | S_IRWXG;
    if (mode & 0x00000001) realMode |= S_IROTH;
    if (mode & 0x00000002) realMode |= S_IWOTH;

    int fd = open(name.string(), flags, realMode);
    if (fd < 0) {
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    AutoPtr<IFileDescriptor> object = CreateFileDescriptor(fd);
    if (object == NULL) {
        close(fd);
    }
    *desc = object;
    INTERFACE_ADDREF(*desc);
    return NOERROR;
}

ECode CParcel::DupFileDescriptor(
    /* [in] */ IFileDescriptor* orig,
    /* [out] */ IFileDescriptor** desc)
{
    VALIDATE_NOT_NULL(desc);
    *desc = NULL;

    if (orig == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    int origfd = GetFDFromFileDescriptor(orig);
    if (origfd < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    int fd = dup(origfd);
    if (fd < 0) {
        return E_IO_EXCEPTION;
    }

    AutoPtr<IFileDescriptor> object = CreateFileDescriptor(fd);
    if (object == NULL) {
        close(fd);
    }

    *desc = object;
    INTERFACE_ADDREF(*desc);
    return NOERROR;
}

ECode CParcel::CloseFileDescriptor(
    /* [in] */ IFileDescriptor* desc)
{
    if (desc == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    int fd = GetFDFromFileDescriptor(desc);
    if (fd >= 0) {
        SetFileDescriptorOfFD(desc, -1);
        close(fd);
    }

    return NOERROR;
}

ECode CParcel::ClearFileDescriptor(
    /* [in] */ IFileDescriptor* object)
{
    if (object == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    int fd = GetFDFromFileDescriptor(object);
    if (fd >= 0) {
        SetFileDescriptorOfFD(object, -1);
    }
    return NOERROR;
}

AutoPtr<IFileDescriptor> CParcel::CreateFileDescriptor(
    /* [in] */ Int32 fd)
{
    AutoPtr<IFileDescriptor> fileDescriptor;
    CFileDescriptor::New((IFileDescriptor**)&fileDescriptor);
    SetFileDescriptorOfFD(fileDescriptor, fd);
    return fileDescriptor;
}

Int32 CParcel::GetFDFromFileDescriptor(
    /* [in] */ IFileDescriptor* fileDescriptor)
{
    assert(fileDescriptor != NULL);
    Int32 descriptorField = 0;
    fileDescriptor->GetDescriptor(&descriptorField);
    return descriptorField;
}

void CParcel::SetFileDescriptorOfFD(
    /* [in] */ IFileDescriptor* fileDescriptor,
    /* [in] */ Int32 value)
{
    assert(fileDescriptor != NULL);
    fileDescriptor->SetDescriptor(value);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
