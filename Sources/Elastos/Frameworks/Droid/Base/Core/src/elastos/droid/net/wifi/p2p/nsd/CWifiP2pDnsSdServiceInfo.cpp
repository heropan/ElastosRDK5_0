
#include "CWifiP2pDnsSdServiceInfo.h"
#include <elastos/core/StringBuffer.h>
#include <elastos/core/StringBuilder.h>
#include "WifiP2pServiceInfo.h"
#include "CWifiP2pServiceInfo.h"
#include "elastos/droid/net/nsd/CDnsSdTxtRecord.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Net::Nsd::CDnsSdTxtRecord;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CWifiP2pDnsSdServiceInfo::CStatic CWifiP2pDnsSdServiceInfo::sStatic;

PInterface CWifiP2pDnsSdServiceInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_WifiP2pServiceInfo) {
        return reinterpret_cast<PInterface>((WifiP2pServiceInfo*)this);
    }
    return _CWifiP2pDnsSdServiceInfo::Probe(riid);
}

ECode CWifiP2pDnsSdServiceInfo::constructor()
{
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceInfo::constructor(
    /* [in] */ ArrayOf<String>* queryList)
{
    return WifiP2pServiceInfo::Init(queryList);
}

ECode CWifiP2pDnsSdServiceInfo::NewInstance(
    /* [in] */ const String& instanceName,
    /* [in] */ const String& serviceType,
    /* [in] */ IObjectStringMap* txtMap,
    /* [out] */ IWifiP2pDnsSdServiceInfo** instance)
{
    if (instanceName.IsNullOrEmpty() || serviceType.IsNullOrEmpty()) {
        Slogger::E("CWifiP2pDnsSdServiceInfo", "instance name or service type cannot be empty");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IDnsSdTxtRecord> txtRecord;
    CDnsSdTxtRecord::New((IDnsSdTxtRecord**)&txtRecord);
    if (txtMap != NULL) {
        AutoPtr<ArrayOf<String> > keys;
        txtMap->GetKeys((ArrayOf<String> **)&keys);
        if (keys != NULL) {
            String key;
            ICharSequence* seq;
            for (Int32 i = 0; i < keys->GetLength(); ++i) {
                key = (*keys)[i];
                AutoPtr<IInterface> obj;
                txtMap->Get(key, (IInterface**)&obj);
                seq = ICharSequence::Probe(obj);
                String value;
                if (seq != NULL) {
                    seq->ToString(&value);
                }
                txtRecord->Set(key, value);
            }
        }
    }

    String ptrQ, txtQ;
    CreatePtrServiceQuery(instanceName, serviceType, &ptrQ);
    CreateTxtServiceQuery(instanceName, serviceType, txtRecord, &txtQ);
    AutoPtr<ArrayOf<String> > queries = ArrayOf<String>::Alloc(2);
    queries->Set(0, ptrQ);
    queries->Set(1, txtQ);

    AutoPtr<CWifiP2pDnsSdServiceInfo> info;
    CWifiP2pDnsSdServiceInfo::NewByFriend(queries, (CWifiP2pDnsSdServiceInfo**)&info);
    *instance = (IWifiP2pDnsSdServiceInfo*)info.Get();
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceInfo::CreatePtrServiceQuery(
    /* [in] */ const String& instanceName,
    /* [in] */ const String& serviceType,
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = String(NULL);

    StringBuffer sb("bonjour ");
    String temp;
    FAIL_RETURN(CreateRequest(
        serviceType + String(".local."),
        IWifiP2pDnsSdServiceInfo::DNS_TYPE_PTR,
        IWifiP2pDnsSdServiceInfo::VERSION_1,
        &temp));
    sb += temp;
    sb += " ";

    String lenStr;
    lenStr.AppendFormat("%02x", instanceName.GetByteLength());
    sb += lenStr;

    FAIL_RETURN(WifiP2pServiceInfo::Bin2HexStr(
        instanceName, instanceName.GetByteLength(), &temp));
    sb += temp;
    // This is the start point of this response.
    // Therefore, it indicates the request domain name.
    sb += "c027";

    *query = sb.ToString();
    return NOERROR;
}


ECode CWifiP2pDnsSdServiceInfo::CreateTxtServiceQuery(
    /* [in] */ const String& instanceName,
    /* [in] */ const String& serviceType,
    /* [in] */ IDnsSdTxtRecord* txtRecord,
    /* [out] */ String* query)
{
    VALIDATE_NOT_NULL(query);
    *query = String(NULL);

    StringBuilder ssb(instanceName);
    ssb += ".";
    ssb += serviceType;
    ssb += ".local.";

    StringBuffer sb("bonjour ");
    String temp;
    FAIL_RETURN(CreateRequest(
        ssb.ToString(),
        IWifiP2pDnsSdServiceInfo::DNS_TYPE_TXT,
        IWifiP2pDnsSdServiceInfo::VERSION_1,
        &temp));
    sb += temp;
    sb += " ";

    AutoPtr<ArrayOf<Byte> > rawData;
    txtRecord->GetRawData((ArrayOf<Byte> **)&rawData);


    Int32 byteLength = rawData ? rawData->GetLength() : 0;
    if (byteLength == 0) {
        sb += "00";
    }
    else {
        FAIL_RETURN(WifiP2pServiceInfo::Bin2HexStr(
            *rawData, &temp));
        sb += temp;
    }

    *query = sb.ToString();
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceInfo::CreateRequest(
    /* [in] */ const String& dnsName,
    /* [in] */ const Int32 dnsType,
    /* [in] */ const Int32 version,
    /* [out] */ String* request)
{
    VALIDATE_NOT_NULL(request);

    String temp = dnsName;

    /*
     * The request format is as follows.
     * ________________________________________________
     * |  Encoded and Compressed dns name (variable)  |
     * ________________________________________________
     * |   Type (2)           | Version (1) |
     */
    if (dnsType == IWifiP2pDnsSdServiceInfo::DNS_TYPE_TXT) {
        temp = dnsName.ToLowerCase();
    }

    String result;
    FAIL_RETURN(CompressDnsName(temp, &result));
    result.AppendFormat("%04x", dnsType);
    result.AppendFormat("%02x", version);

    *request = result;
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceInfo::CompressDnsName(
    /* [in] */ const String& inDnsName,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    String dnsName = inDnsName, temp;
    StringBuffer sb;

    // The domain name is replaced with a pointer to a prior
    // occurrence of the same name in virtual memory packet.
    while (TRUE) {
        if (sStatic.mVmPacket.Find(dnsName) != sStatic.mVmPacket.End()) {
            temp = sStatic.mVmPacket[dnsName];
            if (!temp.IsNull()) {
                sb += temp;
            }
            break;
        }

        Int32 i = dnsName.IndexOf('.');
        if (i == -1) {
            if (!dnsName.IsNullOrEmpty()) {
                String value;
                value.AppendFormat("%02x", dnsName.GetLength());
                sb += value;

                FAIL_RETURN(WifiP2pServiceInfo::Bin2HexStr(
                    dnsName.string(), dnsName.GetByteLength(), &temp));
                sb += temp;
            }
            // for a sequence of labels ending in a zero octet
            sb += "00";
            break;
        }

        String name = dnsName.Substring(0, i);
        dnsName = dnsName.Substring(i + 1);
        String value;
        value.AppendFormat("%02x", name.GetLength());
        sb += value;

        FAIL_RETURN(WifiP2pServiceInfo::Bin2HexStr(
            name.string(), name.GetByteLength(), &temp));
        sb += temp;
    }

    *result = sb.ToString();
    return NOERROR;
}

ECode CWifiP2pDnsSdServiceInfo::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    return WifiP2pServiceInfo::Equals(obj, isEqual);
}

ECode CWifiP2pDnsSdServiceInfo::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return WifiP2pServiceInfo::GetHashCode(hashCode);
}

ECode CWifiP2pDnsSdServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return WifiP2pServiceInfo::ReadFromParcel(source);
}

ECode CWifiP2pDnsSdServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return WifiP2pServiceInfo::WriteToParcel(dest);
}

ECode CWifiP2pDnsSdServiceInfo::GetSupplicantQueryList(
    /* [out, callee] */ ArrayOf<String>** list)
{
    return WifiP2pServiceInfo::GetSupplicantQueryList(list);
}

}
}
}
}
}
}
