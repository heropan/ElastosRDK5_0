#ifndef __CPARCEL_H__
#define __CPARCEL_H__

#include "_CParcel.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CParcel)
{
public:
    CARAPI constructor();

    CARAPI Marshall(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI Unmarshall(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI AppendFrom(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* result);

    CARAPI ReadByte(
        /* [out] */ Byte* value);

    CARAPI WriteByte(
        /* [in] */ Byte value);

    CARAPI ReadBoolean(
        /* [out] */ Boolean* value);

    CARAPI WriteBoolean(
        /* [in] */ Boolean value);

    CARAPI ReadChar8(
        /* [out] */ Char8* value);

    CARAPI WriteChar8(
        /* [in] */ Char8 value);

    CARAPI ReadChar16(
        /* [out] */ Char16* value);

    CARAPI WriteChar16(
        /* [in] */ Char16 value);

    CARAPI ReadChar32(
        /* [out] */ Char32* value);

    CARAPI WriteChar32(
        /* [in] */ Char32 value);

    CARAPI ReadInt16(
        /* [out] */ Int16* value);

    CARAPI WriteInt16(
        /* [in] */ Int16 value);

    CARAPI ReadInt32(
        /* [out] */ Int32* value);

    CARAPI WriteInt32(
        /* [in] */ Int32 value);

    CARAPI ReadInt64(
        /* [out] */ Int64* value);

    CARAPI WriteInt64(
        /* [in] */ Int64 value);

    CARAPI ReadFloat(
        /* [out] */ Float* value);

    CARAPI WriteFloat(
        /* [in] */ Float value);

    CARAPI ReadDouble(
        /* [out] */ Double* value);

    CARAPI WriteDouble(
        /* [in] */ Double value);

    CARAPI ReadString(
        /* [out] */ String* str);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI ReadStruct(
        /* [out] */ Handle32* address);

    CARAPI WriteStruct(
        /* [in] */ Handle32 value,
        /* [in] */ Int32 size);

    CARAPI ReadEMuid(
        /* [out] */ EMuid* id);

    CARAPI WriteEMuid(
        /* [in] */ const EMuid& id);

    CARAPI ReadEGuid(
        /* [out] */ EGuid* id);

    CARAPI WriteEGuid(
        /* [in] */ const EGuid& id);

    CARAPI ReadArrayOf(
        /* [out] */ Handle32* array);

    CARAPI WriteArrayOf(
        /* [in] */ Handle32 array);

    CARAPI ReadArrayOfString(
        /* [out, callee] */ ArrayOf<String>** array);

    CARAPI WriteArrayOfString(
        /* [in] */ ArrayOf<String>* array);

    CARAPI ReadBufferOf(
        /* [out] */ Handle32* buffer);

    CARAPI WriteBufferOf(
        /* [in] */ Handle32 buffer);

    CARAPI ReadBufferOfCString(
        /* [out] */ Handle32* buffer);

    CARAPI WriteBufferOfCString(
        /* [in] */ const BufferOf<CString>& buffer);

    CARAPI ReadBufferOfString(
        /* [out] */ Handle32* buffer);

    CARAPI WriteBufferOfString(
        /* [in] */ const BufferOf<String>& buffer);

    CARAPI ReadStringBuf(
        /* [out] */ Handle32* buffer);

    CARAPI WriteStringBuf(
        /* [in] */ Handle32 buffer);

    CARAPI ReadMemoryBuf(
        /* [out] */ Handle32* buffer);

    CARAPI WriteMemoryBuf(
        /* [in] */ Handle32 buffer);

    CARAPI ReadInterfacePtr(
        /* [out] */ Handle32* itfPtr);

    CARAPI WriteInterfacePtr(
        /* [in] */ IInterface* value);

    CARAPI ReadBytePtr(
        /* [out] */ Handle32* bypp);

    CARAPI WriteBytePtr(
        /* [in] */ Handle32 address);

    CARAPI ReadBooleanPtr(
        /* [out] */ Handle32* bopp);

    CARAPI WriteBooleanPtr(
        /* [in] */ Handle32 address);

    CARAPI ReadChar8Ptr(
        /* [out] */ Handle32* c8pp);

    CARAPI WriteChar8Ptr(
        /* [in] */ Handle32 address);

    CARAPI ReadChar16Ptr(
        /* [out] */ Handle32* c16pp);

    CARAPI WriteChar16Ptr(
        /* [in] */ Handle32 address);

    CARAPI ReadInt16Ptr(
        /* [out] */ Handle32* i16pp);

    CARAPI WriteInt16Ptr(
        /* [in] */ Handle32 address);

    CARAPI ReadInt32Ptr(
        /* [out] */ Handle32* i32ptr);

    CARAPI WriteInt32Ptr(
        /* [in] */ Handle32 address);

    CARAPI ReadInt64Ptr(
        /* [out] */ Handle32* i64ptr);

    CARAPI WriteInt64Ptr(
        /* [in] */ Handle32 address);

    CARAPI ReadFloatPtr(
        /* [out] */ Handle32* fpp);

    CARAPI WriteFloatPtr(
        /* [in] */ Handle32 address);

    CARAPI ReadDoublePtr(
        /* [out] */ Handle32* dpp);

    CARAPI WriteDoublePtr(
        /* [in] */ Handle32 address);

    CARAPI ReadStringPtr(
        /* [out] */ Handle32* spp);

    CARAPI WriteStringPtr(
        /* [in] */ Handle32 address);

    CARAPI ReadStructPtr(
        /* [ou] */ Handle32* address);

    CARAPI ReadEMuidPtr(
        /* [out] */ Handle32* idpp);

    CARAPI WriteEMuidPtr(
        /* [in] */ Handle32 idPtr);

    CARAPI ReadEGuidPtr(
        /* [out] */ Handle32* idpp);

    CARAPI WriteEGuidPtr(
        /* [in] */ Handle32 idPtr);

    CARAPI ReadInterfacePtrPtr(
        /* [out] */ Handle32* itfpp);

    CARAPI WriteInterfacePtrPtr(
        /* [in] */ Handle32 itfpp);

    CARAPI ReadBufferOfPtr(
        /* [out] */ Handle32* buffer);

    // Place a file descriptor into the parcel.  The given fd must remain
    // valid for the lifetime of the parcel.
    CARAPI WriteFileDescriptor(
        /* [in] */ Int32 fd);

    // Place a file descriptor into the parcel.  A dup of the fd is made, which
    // will be closed once the parcel is destroyed.
    CARAPI WriteDupFileDescriptor(
        /* [in] */ Int32 fd);

    // Retrieve a file descriptor from the parcel.  This returns the raw fd
    // in the parcel, which you do not own -- use dup() to get your own copy.
    CARAPI ReadFileDescriptor(
        /* [out] */ Int32* fd);

    CARAPI Clone(
        /* [in] */ IParcel* srcParcel);

    CARAPI GetDataPosition(
        /* [out] */ Int32* position);

    CARAPI SetDataPosition(
        /* [in] */ Int32 position);

    CARAPI GetElementSize(
        /* [out] */ Int32* size);

    CARAPI GetElementPayload(
        /* [out] */ Handle32* buffer);

public:
    static CARAPI OpenFileDescriptor(
        /* [in] */ const String& file,
        /* [in] */ Int32 mode,
        /* [out] */ IFileDescriptor** desc);

    static CARAPI DupFileDescriptor(
        /* [in] */ IFileDescriptor* orig,
        /* [out] */ IFileDescriptor** desc);

    static CARAPI CloseFileDescriptor(
        /* [in] */ IFileDescriptor* desc);

    static CARAPI ClearFileDescriptor(
        /* [in] */ IFileDescriptor* desc);

    static AutoPtr<IFileDescriptor> CreateFileDescriptor(
        /* [in] */ Int32 fd);

    static Int32 GetFDFromFileDescriptor(
        /* [in] */ IFileDescriptor* fileDescriptor);

    static void SetFileDescriptorOfFD(
        /* [in] */ IFileDescriptor* fileDescriptor,
        /* [in] */ Int32 value);

private:
    CARAPI ReadValue(
        /* [in] */ PVoid value,
        /* [in] */ Int32 type);

    CARAPI WriteValue(
        /* [in] */ PVoid value,
        /* [in] */ Int32 type,
        /* [in] */ Int32 size);

private:
    AutoPtr<IParcel> mData;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CPARCEL_H__
