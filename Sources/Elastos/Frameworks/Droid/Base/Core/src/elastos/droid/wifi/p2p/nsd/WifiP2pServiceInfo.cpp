
#include "WifiP2pServiceInfo.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Slogger;

// e6a7bf0e-cd3f-4fb3-8029-739415e17bfd
extern "C" const InterfaceID EIID_WifiP2pServiceInfo =
    { 0xe6a7bf0e, 0xcd3f, 0x4fb3, { 0x80, 0x29, 0x73, 0x94, 0x15, 0xe1, 0x7b, 0xfd } };

namespace Elastos {
namespace Droid {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode WifiP2pServiceInfo::Init(
    /* [in] */ ArrayOf<String>* queryList)
{
    if (queryList == NULL) {
        Slogger::E("WifiP2pServiceInfo", "query list cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mQueryList = queryList;
    return NOERROR;
}

ECode WifiP2pServiceInfo::GetSupplicantQueryList(
    /* [out, callee] */ ArrayOf<String>** list)
{
    VALIDATE_NOT_NULL(list);
    *list = mQueryList;
    REFCOUNT_ADD(*list);
    return NOERROR;
}

ECode WifiP2pServiceInfo::Bin2HexStr(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [out] */ String* hexStr)
{
    VALIDATE_NOT_NULL(hexStr);
    *hexStr = String(NULL);

    StringBuilder sb;
    String s;

    for (Int32 i = 0; i < data.GetLength(); ++i) {
       // try {
       s = StringUtils::Int32ToHexString((data[i] & 0xff));
       // } catch (Exception e) {
       //     e.printStackTrace();
       //     return null;
       // }

       //add 0 padding
       if (s.GetLength() == 1) {
            sb += "0";
       }
       sb += s;
    }

    *hexStr = sb.ToString();
    return NOERROR;
}

ECode WifiP2pServiceInfo::Bin2HexStr(
    /* [in] */ const char * data,
    /* [in] */ Int32 length,
    /* [out] */ String* hexStr)
{
    VALIDATE_NOT_NULL(hexStr);
    *hexStr = String(NULL);

    StringBuilder sb;
    String s;
    for (Int32 i = 0; i < length; ++i) {
       // try {
      s = StringUtils::Int32ToHexString((data[i] & 0xff));
       // } catch (Exception e) {
       //     e.printStackTrace();
       //     return null;
       // }

       //add 0 padding
       if (s.GetLength() == 1) {
           sb += "0";
       }
       sb += s;
    }

    *hexStr = sb.ToString();
    return NOERROR;
}

ECode WifiP2pServiceInfo::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

    if (obj == this->Probe(EIID_IInterface)) {
       *isEqual = TRUE;
       return NOERROR;
    }

    if (IWifiP2pServiceInfo::Probe(obj) == NULL) {
       *isEqual = FALSE;
       return NOERROR;
    }

    WifiP2pServiceInfo* servInfo = reinterpret_cast<WifiP2pServiceInfo*>(
      obj->Probe(EIID_WifiP2pServiceInfo));
    assert(servInfo != NULL);
    *isEqual = ArrayOf<String>::Equals(mQueryList, servInfo->mQueryList);
    return NOERROR;
}

ECode WifiP2pServiceInfo::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    Int32 result = 17;
    result = 31 * result + ((mQueryList == NULL) ? 0 : (Int32)mQueryList.Get());
    *hashCode = result;
    return NOERROR;
}

ECode WifiP2pServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    AutoPtr<ArrayOf<String> > list;
    FAIL_RETURN(source->ReadArrayOfString((ArrayOf<String>**)&list));
    mQueryList = list;

    return NOERROR;
}

ECode WifiP2pServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteArrayOfString(mQueryList);
}


}
}
}
}
}
