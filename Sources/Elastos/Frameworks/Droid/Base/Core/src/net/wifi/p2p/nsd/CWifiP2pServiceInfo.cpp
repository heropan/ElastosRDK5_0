
#include "CWifiP2pServiceInfo.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

ECode CWifiP2pServiceInfo::constructor()
{
    return NOERROR;
}

ECode CWifiP2pServiceInfo::constructor(
    /* [in] */ ArrayOf<String>* queryList)
{
    return WifiP2pServiceInfo::Init(queryList);
}

PInterface CWifiP2pServiceInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_WifiP2pServiceInfo) {
        return reinterpret_cast<PInterface>((WifiP2pServiceInfo*)this);
    }
    return _CWifiP2pServiceInfo::Probe(riid);
}

ECode CWifiP2pServiceInfo::GetSupplicantQueryList(
    /* [out, callee] */ ArrayOf<String>** list)
{
    return WifiP2pServiceInfo::GetSupplicantQueryList(list);
}

ECode CWifiP2pServiceInfo::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    return WifiP2pServiceInfo::Equals(obj, isEqual);
}

ECode CWifiP2pServiceInfo::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return WifiP2pServiceInfo::GetHashCode(hashCode);
}

ECode CWifiP2pServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return WifiP2pServiceInfo::ReadFromParcel(source);
}

ECode CWifiP2pServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return WifiP2pServiceInfo::WriteToParcel(dest);
}

}
}
}
}
}
}
