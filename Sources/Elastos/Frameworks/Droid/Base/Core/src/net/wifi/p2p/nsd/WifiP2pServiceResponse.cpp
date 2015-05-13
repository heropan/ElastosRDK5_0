
#include "WifiP2pServiceResponse.h"
#include <Elastos.Core.h>
#include <ext/frameworkext.h>
#include <elastos/List.h>
#include <elastos/StringUtils.h>
#include <elastos/StringBuilder.h>
#include <elastos/Slogger.h>

using Elastos::Utility::List;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::CByteArrayInputStream;
using Elastos::IO::IDataInputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::IDataInput;


namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

// 1a866fae-23e8-48c7-bd2c-37b396873151
extern "C" const InterfaceID EIID_WifiP2pServiceResponse =
    { 0x1a866fae, 0x23e8, 0x48c7, { 0xbd, 0x2c, 0x37, 0xb3, 0x96, 0x87, 0x31, 0x51 } };

WifiP2pServiceResponse::WifiP2pServiceResponse()
    : mServiceType(0)
    , mStatus(0)
    , mTransId(0)
{
}

ECode WifiP2pServiceResponse::Init(
    /* [in] */ Int32 serviceType,
    /* [in] */ Int32 status,
    /* [in] */ Int32 transId,
    /* [in] */ IWifiP2pDevice* device,
    /* [in] */ ArrayOf<Byte>* data)
{
    mServiceType = serviceType;
    mStatus = status;
    mTransId = transId;
    mDevice = device;
    mData = data;

    return NOERROR;
}

String WifiP2pServiceResponse::StatusToString(
    /* [in] */ Int32 status)
{
    switch(status) {
    case IWifiP2pServiceResponseStatus::SUCCESS:
        return String("SUCCESS");
    case IWifiP2pServiceResponseStatus::SERVICE_PROTOCOL_NOT_AVAILABLE:
        return String("SERVICE_PROTOCOL_NOT_AVAILABLE");
    case IWifiP2pServiceResponseStatus::REQUESTED_INFORMATION_NOT_AVAILABLE:
        return String("REQUESTED_INFORMATION_NOT_AVAILABLE");
    case IWifiP2pServiceResponseStatus::BAD_REQUEST:
        return String("BAD_REQUEST");
    default:
        return String("UNKNOWN");
    }
}

ECode WifiP2pServiceResponse::GetServiceType(
    /* [out] */ Int32* serviceType)
{
    VALIDATE_NOT_NULL(serviceType);

    *serviceType = mServiceType;

    return NOERROR;
}

ECode WifiP2pServiceResponse::GetStatus(
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    *status = mStatus;

    return NOERROR;
}

ECode WifiP2pServiceResponse::GetTransactionId(
    /* [out] */ Int32* transactionId)
{
    VALIDATE_NOT_NULL(transactionId);

    *transactionId = mTransId;

    return NOERROR;
}

ECode WifiP2pServiceResponse::GetRawData(
    /* [out, callee] */ ArrayOf<Byte>** rawData)
{
    VALIDATE_NOT_NULL(rawData);
    *rawData = mData;
    INTERFACE_ADDREF(*rawData);
    return NOERROR;
}

ECode WifiP2pServiceResponse::GetSrcDevice(
    /* [out] */ IWifiP2pDevice** srcDevice)
{
    VALIDATE_NOT_NULL(srcDevice);

    *srcDevice = mDevice;
    INTERFACE_ADDREF(*srcDevice);

    return NOERROR;
}

ECode WifiP2pServiceResponse::SetSrcDevice(
    /* [in] */ IWifiP2pDevice* dev)
{
    if (dev == NULL)
        return NOERROR;

    mDevice = dev;

    return NOERROR;
}

ECode WifiP2pServiceResponse::HexStr2Bin(
    /* [in] */ const String& hex,
    /* [out] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    Int32 sz = hex.GetLength() / 2;
    AutoPtr<ArrayOf<Byte> > b = ArrayOf<Byte>::Alloc(sz);
    if (b == NULL) return E_OUT_OF_MEMORY;

    String subStr;
    Int32 value;
    ECode ec = NOERROR;
    for (Int32 i = 0; i < sz; i++) {
        subStr = hex.Substring(i * 2, i * 2 + 2);
        ec = StringUtils::ParseInt32(subStr, 16, &value);
        if (FAILED(ec)) {
            Slogger::E("WifiP2pServiceResponse", "HexStr2Bin: failed to ParseInt32 %s", hex.string());
            return ec;
        }

        (*b)[i] = (Byte)value;
    }

    *array = b;
    INTERFACE_ADDREF(*array);
    return NOERROR;
}

ECode WifiP2pServiceResponse::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    StringBuilder sb("serviceType:");
    sb += mServiceType;
    sb += " status:";

    sb += StatusToString(mStatus);

    String temp;
    FAIL_RETURN(mDevice->GetDeviceAddress(&temp));
    sb += " srcAddr:";
    sb += temp;

    sb += " data:";
    sb += mData ? (const char*)mData->GetPayload() : "";

    *string = sb.ToString();
    return NOERROR;
}

ECode WifiP2pServiceResponse::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    *isEqual = FALSE;

    if (this->Probe(EIID_IInterface) == obj) {
        *isEqual = TRUE;
        return NOERROR;
    }

    if (IWifiP2pServiceResponse::Probe(obj) == NULL) {
        *isEqual = FALSE;
        return NOERROR;
    }

    WifiP2pServiceResponse* req = reinterpret_cast<WifiP2pServiceResponse*>(
        obj->Probe(EIID_WifiP2pServiceResponse));
    assert(req != NULL);

    String temp1, temp2;
    FAIL_RETURN(req->mDevice->GetDeviceAddress(&temp1));
    FAIL_RETURN(mDevice->GetDeviceAddress(&temp2));

    *isEqual = (req->mServiceType == mServiceType) &&
        (req->mStatus == mStatus) &&
        Equals(temp1, temp2) &&
        req->mData->Equals(mData);

    return NOERROR;
}

Boolean WifiP2pServiceResponse::Equals(
    /* [in] */ const String& a,
    /* [in] */ const String& b)
{
    if (a.IsNull() && b.IsNull()) {
        return TRUE;
    }
    else if (!a.IsNull()) {
        return a.Equals(b);
    }
    return FALSE;
}

ECode WifiP2pServiceResponse::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    if (hashCode == NULL) return NOERROR;

    Int32 result = 17;
    result = 31 * result + mServiceType;
    result = 31 * result + mStatus;
    result = 31 * result + mTransId;
    String temp;
    FAIL_RETURN(mDevice->GetDeviceAddress(&temp));
    result = 31 * result + (temp.IsNull() ? 0 : temp.GetHashCode());
    result = 31 * result + (Int32)mData.Get();
    return result;
}

ECode WifiP2pServiceResponse::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mServiceType));
    FAIL_RETURN(source->ReadInt32(&mStatus));
    FAIL_RETURN(source->ReadInt32(&mTransId));

    Int32 hasValue;
    FAIL_RETURN(source->ReadInt32(&hasValue));
    mDevice = NULL;
    if (hasValue != 0) {
        CWifiP2pDevice::New((IWifiP2pDevice**)&mDevice);
        IParcelable* p = IParcelable::Probe(mDevice);
        p->ReadFromParcel(source);
    }

    Int32 len;
    FAIL_RETURN(source->ReadInt32(&len));

    mData = NULL;
    if (len > 0) {
        source->ReadArrayOf((Handle32*)(&mData));
    }

    return NOERROR;
}

ECode WifiP2pServiceResponse::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(mServiceType));
    FAIL_RETURN(dest->WriteInt32(mStatus));
    FAIL_RETURN(dest->WriteInt32(mTransId));

    if (mDevice != NULL) {
        dest->WriteInt32(1);
        IParcelable* p = IParcelable::Probe(mDevice);
        p->WriteToParcel(dest);
    }
    else {
        dest->WriteInt32(0);
    }

    if (mData == NULL || mData->GetLength() == 0) {
        FAIL_RETURN(dest->WriteInt32(0));
    }
    else {
        FAIL_RETURN(dest->WriteInt32(mData->GetLength()));
        dest->WriteArrayOf((Handle32)mData.Get());
    }

    return NOERROR;
}


}
}
}
}
}
}
