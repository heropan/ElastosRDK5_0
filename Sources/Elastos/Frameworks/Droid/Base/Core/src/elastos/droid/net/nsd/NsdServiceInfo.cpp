
#include "elastos/droid/net/nsd/NsdServiceInfo.h"
#include "elastos/droid/utility/CArrayMap.h"

using Elastos::Droid::Utility::ILog;
using Elastos::Droid::Utility::CArrayMap;

using Elastos::IO::Charset::IStandardCharsets;
using Elastos::Net::IInetAddress;
using Elastos::Utility::ICollections;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

CAR_INTERFACE_IMPL_2(NsdServiceInfo, Object, IParcelable, INsdServiceInfo)

const String NsdServiceInfo::TAG("NsdServiceInfo");

NsdServiceInfo::NsdServiceInfo()
    : mPort(0)
{
    CArrayMap::New((IArrayMap**)&mTxtRecord);
    REFCOUNT_ADD(mTxtRecord)
}

ECode NsdServiceInfo::constructor()
{
    return NOERROR;
}

ECode NsdServiceInfo::constructor(
    /* [in] */ const String& sn,
    /* [in] */ const String& rt)
{
    mServiceName = sn;
    mServiceType = rt;
    return NOERROR;
}

ECode NsdServiceInfo::GetServiceName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mServiceName;
    return NOERROR;
}

ECode NsdServiceInfo::SetServiceName(
    /* [in] */ const String& s)
{
    mServiceName = s;
    return NOERROR;
}

ECode NsdServiceInfo::GetServiceType(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mServiceType;
    return NOERROR;
}

ECode NsdServiceInfo::SetServiceType(
    /* [in] */ const String& s)
{
    mServiceType = s;
    return NOERROR;
}

ECode NsdServiceInfo::GetHost(
    /* [out] */ IInetAddress** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHost;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode NsdServiceInfo::SetHost(
    /* [in] */ IInetAddress* s)
{
    mHost = s;
    return NOERROR;
}

ECode NsdServiceInfo::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;
    return NOERROR;
}

ECode NsdServiceInfo::SetPort(
    /* [in] */ Int32 p)
{
    mPort = p;
    return NOERROR;
}

ECode NsdServiceInfo::SetAttribute(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate code below.
    // Key must be printable US-ASCII, excluding =.
    for (Int32 i = 0; i < key.GetLength(); ++i) {
        Char32 character = key.CharAt(i);
        if (character < 0x20 || character > 0x7E) {
                // throw new IllegalArgumentException("Key strings must be printable US-ASCII");
            Logger::E(TAG, "Key strings must be printable US-ASCII");
            return E_ILLEGAL_ARGUMENT;
        } else if (character == 0x3D) {
                // throw new IllegalArgumentException("Key strings must not include '='");
            Logger::E(TAG, "Key strings must not include '='");
            return E_ILLEGAL_ARGUMENT;
        }
    }
    // Key length + value length must be < 255.
    if (key.GetLength() + (value == NULL ? 0 : value.GetLength()) >= 255) {
            // throw new IllegalArgumentException("Key length + value length must be < 255 bytes");
        Logger::E(TAG, "Key length + value length must be < 255 bytes");
        return E_ILLEGAL_ARGUMENT;
    }
    // Warn if key is > 9 characters, as recommended by RFC 6763 section 6.4.
    if (key.GetLength() > 9) {
        Logger::W(TAG, "Key lengths > 9 are discouraged: %s", key.string());
    }
    // Check against total TXT record size limits.
    // Arbitrary 400 / 1300 byte limits taken from RFC 6763 section 6.2.
    Int32 txtRecordSize;
    GetTxtRecordSize(&txtRecordSize);
    Int32 futureSize = txtRecordSize + key.GetLength() + (value == NULL ? 0 : value.GetLength()) + 2;
    if (futureSize > 1300) {
            // throw new IllegalArgumentException("Total length of attributes must be < 1300 bytes");
        Logger::E(TAG, "Total length of attributes must be < 1300 bytes");
        return E_ILLEGAL_ARGUMENT;
    } else if (futureSize > 400) {
            Logger::W(TAG, "Total length of all attributes exceeds 400 bytes; truncation may occur");
    }
    mTxtRecord->Put(key, value);
#endif
}

ECode NsdServiceInfo::SetAttribute(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // try {
    ECode ec = SetAttribute(key, value.IsNull() ? ArrayOf<Byte>::Alloc(0) : value.GetBytes("UTF-8"));
        // } catch (UnsupportedEncodingException e) {
    if (ec != NOERROR) {
            // throw new IllegalArgumentException("Value must be UTF-8");
        Logger::E(TAG, "Value must be UTF-8");
        return E_ILLEGAL_ARGUMENT;
    }
        // }
#endif
}

ECode NsdServiceInfo::RemoveAttribute(
    /* [in] */ const String& key)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(key, (ICharSequence**)&csq);
    mTxtRecord->Remove(IInterface::Probe(csq));
    return NOERROR;
#endif
}

ECode NsdServiceInfo::GetAttributes(
    /* [out, callee] */ IMap** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(result)

    AutoPtr<ICollections> collections;
    CCollections::AcquireSingleton((ICollections**)&collections);
    collections->UnmodifiableMap(IMap::Probe(mTxtRecord), result);
    REFCOUNT_ADD(*result)
    return NOERROR;
#endif
}

ECode NsdServiceInfo::GetTxtRecordSize(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Int32 txtRecordSize = 0;
    AutoPtr<ISet> entrySet;
    mTxtRecord->GetEntrySet((ISet**)&entrySet);
    AutoPtr<ArrayOf<IInterface*> > array;
    entrySet->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        AutoPtr<IMapEntry> entry;
        entry = IMapEntry::Probe((*array)[i]);
        txtRecordSize += 2;  // One for the length byte, one for the = between key and value.
        AutoPtr<IInterface> key;
        entry->GetKey((IInterface**)&key);
        Int32 length;
        ICharSequence::Probe(key)->GetLength(&length);
        txtRecordSize += length;
        AutoPtr<IArrayOf> value;
        entry->getValue((IArrayOf**)&value);
        length = 0;
        if (value != NULL)
            value->GetLength(&length);
        txtRecordSize += length;
    }
    *result = txtRecordSize;
    return NOERROR;
#endif
}

ECode NsdServiceInfo::GetTxtRecord(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Int32 txtRecordSize;
    GetTxtRecordSize(&txtRecordSize);
    if (txtRecordSize == 0) {
        *result = NULL;
        return NOERROR;
    }
    AutoPtr<ArrayOf<Byte> > txtRecord = ArrayOf<Byte>::Alloc(txtRecordSize);
    Int32 ptr = 0;
        for (Map.Entry<String, byte[]> entry : mTxtRecord.entrySet()) {
            String key = entry.getKey();
            byte[] value = entry.getValue();
            // One byte to record the length of this key/value pair.
            txtRecord[ptr++] = (byte) (key.length() + (value == null ? 0 : value.length) + 1);
            // The key, in US-ASCII.
            // Note: use the StandardCharsets const here because it doesn't raise exceptions and we
            // already know the key is ASCII at this point.
            System.arraycopy(key.getBytes(StandardCharsets.US_ASCII), 0, txtRecord, ptr,
                    key.length());
            ptr += key.length();
            // US-ASCII '=' character.
            txtRecord[ptr++] = (byte)'=';
            // The value, as any raw bytes.
            if (value != null) {
                System.arraycopy(value, 0, txtRecord, ptr, value.length);
                ptr += value.length;
            }
        }
        return txtRecord;

#endif
}

ECode NsdServiceInfo::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(info);

    StringBuilder sb;

    sb += "name: ";
    sb += mServiceName;
    sb += ", type: ";
    sb += mServiceType;

    sb += ", host: ";
    String temp;
    mHost->ToString(&temp);
    sb += temp;

    sb += ", port: ";
    sb += StringUtils::ToString(mPort);
    sb += "txtRecord: ";
    mTxtRecord->ToString(&temp);
    sb += temp;

    AutoPtr<ArrayOf<Byte> > txtRecord;
    GetTxtRecord((ArrayOf<Byte>**)&txtRecord);
    if (txtRecord != NULL) {
        sb += ", txtRecord: ";
        String s(txtRecord, StandardCharsets::UTF_8);
        sb += s;
    }

    *info = sb.ToString();
    return NOERROR;
#endif
}

ECode NsdServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below. Source code changed.
    FAIL_RETURN(dest->WriteString(mServiceName));
    FAIL_RETURN(dest->WriteString(mServiceType));
    FAIL_RETURN(dest->WriteInterfacePtr(mTxtRecord.Get()));
    FAIL_RETURN(dest->WriteInterfacePtr(mHost.Get()));
    FAIL_RETURN(dest->WriteInt32(mPort));

    return NOERROR;
#endif
}

ECode NsdServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<IInterface> obj;
    FAIL_RETURN(source->ReadString(&mServiceName));
    FAIL_RETURN(source->ReadString(&mServiceType));
    FAIL_RETURN(source->ReadInterfacePtrPtr((IInterface**)&obj));
    mTxtRecord = IDnsSdTxtRecord::Probe(obj);

    obj = NULL;
    FAIL_RETURN(source->ReadInterfacePtrPtr((IInterface**)&obj));
    mHost = IInetAddress::Probe(obj);

    FAIL_RETURN(source->ReadInt32(&mPort));

    return NOERROR;
#endif
}

} // namespace Nsd
} // namespace Net
} // namespace Droid
} // namespace Elastos
