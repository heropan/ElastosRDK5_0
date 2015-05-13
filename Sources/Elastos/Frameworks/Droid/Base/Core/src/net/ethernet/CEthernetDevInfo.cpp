#include "CEthernetDevInfo.h"
#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Ethernet {

CEthernetDevInfo::CEthernetDevInfo()
    : mMode(ETHERNET_CONN_MODE_DHCP)
{}

ECode CEthernetDevInfo::SetIfName(
    /* [in] */ const String& ifname)
{
    mDevName = ifname;
    return NOERROR;
}

ECode CEthernetDevInfo::GetIfName(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mDevName;
    return NOERROR;
}

ECode CEthernetDevInfo::SetIpAddress(
    /* [in] */ const String& ip)
{
    mIpaddr = ip;
    return NOERROR;
}

ECode CEthernetDevInfo::GetIpAddress(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mIpaddr;
    return NOERROR;
}

ECode CEthernetDevInfo::SetNetMask(
    /* [in] */ const String& ip)
{
    mNetmask = ip;
    return NOERROR;
}

ECode CEthernetDevInfo::GetNetMask(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mNetmask;
    return NOERROR;
}

ECode CEthernetDevInfo::SetGateWay(
    /* [in] */ const String& gw)
{
    mGw = gw;
    return NOERROR;
}

ECode CEthernetDevInfo::GetGateWay(
    /* [out] */ String* gw)
{
    VALIDATE_NOT_NULL(gw);
    *gw = mGw;
    return NOERROR;
}

ECode CEthernetDevInfo::SetDnsAddr(
    /* [in] */ const String& dns)
{
    mDns = dns;
    return NOERROR;
}

ECode CEthernetDevInfo::GetDnsAddr(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mDns;
    return NOERROR;
}

ECode CEthernetDevInfo::SetHwaddr(
    /* [in] */ const String& hwaddr)
{
    mHwaddr = hwaddr;
    return NOERROR;
}

ECode CEthernetDevInfo::GetHwaddr(
    /* [out] */ String* hwaddr)
{
    VALIDATE_NOT_NULL(hwaddr);
    *hwaddr = mHwaddr;
    return NOERROR;
}

ECode CEthernetDevInfo::SetConnectMode(
    /* [in] */ Int32 mode)
{
    mMode = mode;
    return NOERROR;
}

ECode CEthernetDevInfo::GetConnectMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = mMode;
    return NOERROR;
}

ECode CEthernetDevInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mDevName));
    FAIL_RETURN(dest->WriteString(mIpaddr));
    FAIL_RETURN(dest->WriteString(mNetmask));
    FAIL_RETURN(dest->WriteString(mGw));
    FAIL_RETURN(dest->WriteString(mDns));
    FAIL_RETURN(dest->WriteInt32(mMode));
    FAIL_RETURN(dest->WriteString(mHwaddr));

    return NOERROR;
}

ECode CEthernetDevInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadString(&mDevName));
    FAIL_RETURN(source->ReadString(&mIpaddr));
    FAIL_RETURN(source->ReadString(&mNetmask));
    FAIL_RETURN(source->ReadString(&mGw));
    FAIL_RETURN(source->ReadString(&mDns));
    FAIL_RETURN(source->ReadInt32(&mMode));
    FAIL_RETURN(source->ReadString(&mHwaddr));

    return NOERROR;
}

ECode CEthernetDevInfo::constructor()
{
    return NOERROR;
}

}
}
}
}
