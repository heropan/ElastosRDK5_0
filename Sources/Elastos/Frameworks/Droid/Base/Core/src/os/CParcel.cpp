
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

ECode CParcel::ReadCString(
    /* [out] */ CString* str)
{
    return mData->ReadCString(str);
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

ECode CParcel::ReadArrayOfCString(
    /* [out, callee] */ ArrayOf<CString>** array)
{
    return mData->ReadArrayOfCString(array);
}

ECode CParcel::ReadArrayOfString(
    /* [out, callee] */ ArrayOf<String>** array)
{
    return mData->ReadArrayOfString(array);
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

ECode CParcel::WriteCString(
    /* [in] */ CString str)
{
    return mData->WriteCString(str);
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

ECode CParcel::WriteArrayOfCString(
    /* [in] */ ArrayOf<CString>* array)
{
    return mData->WriteArrayOfCString(array);
}

ECode CParcel::WriteArrayOfString(
    /* [in] */ ArrayOf<String>* array)
{
    return mData->WriteArrayOfString(array);
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
