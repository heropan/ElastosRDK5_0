
#include <Elastos.Droid.Net.h>
#include <Elastos.Droid.Os.h>
#include <Elastos.Droid.Text.h>
#include <Elastos.Droid.Utility.h>
#include <Elastos.CoreLibrary.IO.h>
#include <Elastos.CoreLibrary.Net.h>
#include <Elastos.CoreLibrary.Utility.h>
#include "elastos/droid/server/net/IpConfigStore.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/droid/net/ReturnOutValue.h>

using Elastos::Droid::Net::CIpConfiguration;
using Elastos::Droid::Net::CLinkAddress;
using Elastos::Droid::Net::CNetworkUtils;
using Elastos::Droid::Net::CProxyInfo;
using Elastos::Droid::Net::CRouteInfo;
using Elastos::Droid::Net::CStaticIpConfiguration;
using Elastos::Droid::Net::IIpConfiguration;
using Elastos::Droid::Net::ILinkAddress;
using Elastos::Droid::Net::INetworkUtils;
using Elastos::Droid::Net::IProxyInfo;
using Elastos::Droid::Net::IRouteInfo;
using Elastos::Droid::Net::IStaticIpConfiguration;
using Elastos::Droid::Net::IpConfigurationIpAssignment;
using Elastos::Droid::Net::IpConfigurationProxySettings;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Text::ITextUtils;
using Elastos::Droid::Utility::CSparseArray;
using Elastos::Droid::Utility::ILog;
using Elastos::Droid::Utility::ISparseArray;

using Elastos::Core::CInteger32;
using Elastos::Core::IInteger32;
using Elastos::Core::StringUtils;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::CFile;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IBuffer;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::ICloseable;
using Elastos::IO::IDataInput;
using Elastos::IO::IDataInputStream;
using Elastos::IO::IDataOutput;
using Elastos::IO::IDataOutputStream;
using Elastos::IO::IFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IInputStream;
using Elastos::Net::CProxy;
using Elastos::Net::IInet4Address;
using Elastos::Net::IInetAddress;
using Elastos::Utility::IMap;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

//============================================================================
// IpConfigStore::InnerSub_Writer
//============================================================================
IpConfigStore::InnerSub_Writer::InnerSub_Writer(
    /* [in] */ IpConfigStore* host,
    /* [in] */ ISparseArray* networks)
    : mHost(host)
    , mNetworks(networks)
{}

ECode IpConfigStore::InnerSub_Writer::OnWriteCalled(
    /* [in] */ IDataOutputStream* out)
{
    FAIL_RETURN(out->WriteInt32(IPCONFIG_FILE_VERSION))
    for (Int32 i = 0; i < Ptr(mNetworks)->Func(mNetworks->GetSize); i++) {
        Int32 key;
        AutoPtr<IInterface> value;
        mNetworks->KeyAt(i, &key);
        mNetworks->ValueAt(i, (IInterface**)&value);
        Boolean b;
        FAIL_RETURN(mHost->WriteConfig(out, key, IIpConfiguration::Probe(value), &b))
    }
    return NOERROR;
}
//============================================================================
// IpConfigStore
//============================================================================
const String IpConfigStore::TAG("IpConfigStore");
const Boolean IpConfigStore::DBG = FALSE;
const String IpConfigStore::ID_KEY("id");
const String IpConfigStore::IP_ASSIGNMENT_KEY("ipAssignment");
const String IpConfigStore::LINK_ADDRESS_KEY("linkAddress");
const String IpConfigStore::GATEWAY_KEY("gateway");
const String IpConfigStore::DNS_KEY("dns");
const String IpConfigStore::PROXY_SETTINGS_KEY("proxySettings");
const String IpConfigStore::PROXY_HOST_KEY("proxyHost");
const String IpConfigStore::PROXY_PORT_KEY("proxyPort");
const String IpConfigStore::PROXY_PAC_FILE("proxyPac");
const String IpConfigStore::EXCLUSION_LIST_KEY("exclusionList");
const String IpConfigStore::EOS("eos");
const Int32 IpConfigStore::IPCONFIG_FILE_VERSION = 2;

ECode IpConfigStore::constructor()
{
    mWriter = new DelayedDiskWrite();
    return NOERROR;
}

ECode IpConfigStore::WriteConfig(
    /* [in] */ IDataOutputStream* out,
    /* [in] */ Int32 configKey,
    /* [in] */ IIpConfiguration* config,
    /* [out] */ Boolean* result)
{
    Boolean written = FALSE;
    // try {
    ECode ecode;
    Boolean needCatch = FALSE;
    TRY {
        switch (Ptr(config)->Func(config->GetIpAssignment)) {
            case Elastos::Droid::Net::STATIC_IpAssignment: {
                    JUDGE(label, ecode = out->WriteUTF(IP_ASSIGNMENT_KEY), needCatch)
                    JUDGE(label, ecode = out->WriteUTF(StringUtils::ToString(Ptr(config)->Func(config->GetIpAssignment))), needCatch)
                    AutoPtr<IStaticIpConfiguration> staticIpConfiguration;
                    config->GetStaticIpConfiguration((IStaticIpConfiguration**)&staticIpConfiguration);
                    if (staticIpConfiguration != NULL) {
                        if (Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetIpAddress) != NULL) {
                            AutoPtr<ILinkAddress> ipAddress;
                            staticIpConfiguration->GetIpAddress((ILinkAddress**)&ipAddress);
                            JUDGE(label, ecode = out->WriteUTF(LINK_ADDRESS_KEY), needCatch)
                            String s = Ptr(ipAddress)->Ptr(ipAddress->GetAddress)->Func(IInetAddress::GetHostAddress);
                            JUDGE(label, ecode = out->WriteUTF(s), needCatch)
                            JUDGE(label, ecode = out->WriteInt32(Ptr(ipAddress)->Func(ipAddress->GetPrefixLength)), needCatch)
                        }
                        if (Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetGateway) != NULL) {
                            JUDGE(label, ecode = out->WriteUTF(GATEWAY_KEY), needCatch)
                            JUDGE(label, ecode = out->WriteInt32(0), needCatch)  // Default route.
                            JUDGE(label, ecode = out->WriteInt32(1), needCatch)  // Have a gateway.
                            String s;
                            Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetGateway)->GetHostAddress(&s);
                            JUDGE(label, ecode = out->WriteUTF(s), needCatch)
                        }
                        FOR_EACH(iter, Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetDnsServers)) {
                            AutoPtr<IInetAddress> inetAddr = IInetAddress::Probe(Ptr(iter)->Func(iter->GetNext));
                            JUDGE(label, ecode = out->WriteUTF(DNS_KEY), needCatch)
                            JUDGE(label, ecode = out->WriteUTF(Ptr(inetAddr)->Func(inetAddr->GetHostAddress)), needCatch)
                        }
                    }
                    written = TRUE;
                }
                break;
            case Elastos::Droid::Net::DHCP_IpAssignment:
                JUDGE(label, ecode = out->WriteUTF(IP_ASSIGNMENT_KEY), needCatch)
                JUDGE(label, ecode = out->WriteUTF(StringUtils::ToString(Ptr(config)->Func(config->GetIpAssignment))), needCatch)
                written = TRUE;
                break;
            case Elastos::Droid::Net::UNASSIGNED_IpAssignment:
                /* Ignore */
                break;
            default:
                Loge("Ignore invalid ip assignment while writing");
                break;
        }
        switch (Ptr(config)->Func(config->GetProxySettings)) {
            case Elastos::Droid::Net::STATIC_ProxySettings: {
                    AutoPtr<IProxyInfo> proxyProperties;
                    config->GetHttpProxy((IProxyInfo**)&proxyProperties);
                    String exclusionList;
                    proxyProperties->GetExclusionListAsString(&exclusionList);
                    out->WriteUTF(PROXY_SETTINGS_KEY);
                    JUDGE(label, ecode = out->WriteUTF(StringUtils::ToString(Ptr(config)->Func(config->GetProxySettings))), needCatch)
                    JUDGE(label, ecode = out->WriteUTF(PROXY_HOST_KEY), needCatch)
                    JUDGE(label, ecode = out->WriteUTF(Ptr(proxyProperties)->Func(proxyProperties->GetHost)), needCatch)
                    JUDGE(label, ecode = out->WriteUTF(PROXY_PORT_KEY), needCatch)
                    JUDGE(label, ecode = out->WriteInt32(Ptr(proxyProperties)->Func(proxyProperties->GetPort)), needCatch)
                    if (exclusionList != NULL) {
                        JUDGE(label, ecode = out->WriteUTF(EXCLUSION_LIST_KEY), needCatch)
                        JUDGE(label, ecode = out->WriteUTF(exclusionList), needCatch)
                    }
                    written = TRUE;
                }
                break;
            case Elastos::Droid::Net::PAC_ProxySettings: {
                    AutoPtr<IProxyInfo> proxyPacProperties;
                    config->GetHttpProxy((IProxyInfo**)&proxyPacProperties);
                    JUDGE(label, ecode = out->WriteUTF(PROXY_SETTINGS_KEY), needCatch)
                    JUDGE(label, ecode = out->WriteUTF(StringUtils::ToString(Ptr(config)->Func(config->GetProxySettings))), needCatch)
                    JUDGE(label, ecode = out->WriteUTF(PROXY_PAC_FILE), needCatch)
                    JUDGE(label, ecode = out->WriteUTF(Object::ToString(Ptr(proxyPacProperties)->Func(proxyPacProperties->GetPacFileUrl))), needCatch)
                    written = TRUE;
                }
                break;
            case Elastos::Droid::Net::NONE_ProxySettings:
                JUDGE(label, ecode = out->WriteUTF(PROXY_SETTINGS_KEY), needCatch)
                JUDGE(label, ecode = out->WriteUTF(StringUtils::ToString(Ptr(config)->Func(config->GetProxySettings))), needCatch)
                written = TRUE;
                break;
            case Elastos::Droid::Net::UNASSIGNED_ProxySettings:
                /* Ignore */
                break;
            default:
                Loge("Ignore invalid proxy settings while writing");
                break;
        }
        if (written) {
            JUDGE(label, ecode = out->WriteUTF(ID_KEY), needCatch)
            JUDGE(label, ecode = out->WriteInt32(configKey), needCatch)
        }
    }
    // } catch (NullPointerException e) {
    label:
    CATCH(E_NULL_POINTER_EXCEPTION, ecode, needCatch) {
        Loge(TAG, "Failure in writing %s%d", Object::ToString(config).string(), ecode);
    }
    TRY_END(ecode, needCatch)
    // }
    out->WriteUTF(EOS);
    FUNC_RETURN(written)
}

ECode IpConfigStore::WriteIpAndProxyConfigurations(
    /* [in] */ String filePath,
    /* [in] */ ISparseArray* networks)
{
    return mWriter->Write(filePath, new InnerSub_Writer(this, networks));
}

ECode IpConfigStore::ReadIpAndProxyConfigurations(
    /* [in] */ String filePath,
    /* [out] */ ISparseArray** result)
{
        AutoPtr<ISparseArray> networks;
        CSparseArray::New((ISparseArray**)&networks);
        AutoPtr<IDataInputStream> in;
        ECode ec;
        Boolean isReturnAfterFinally = FALSE;
        Boolean needCatch = FALSE;
        TRY {
            AutoPtr<IFileInputStream> fileInputStream;
            JUDGE(out_try, ec = CFileInputStream::New(filePath, (IFileInputStream**)&fileInputStream), needCatch)
            AutoPtr<IBufferedInputStream> bufferedInputStream;
            JUDGE(out_try, ec = CBufferedInputStream::New(IInputStream::Probe(fileInputStream), (IBufferedInputStream**)&bufferedInputStream), needCatch)
            JUDGE(out_try, ec = CDataInputStream::New(IInputStream::Probe(bufferedInputStream), (IDataInputStream**)&in), needCatch)
            Int32 version;
            JUDGE(out_try, ec = in->ReadInt32(&version), needCatch)
            if (version != 2 && version != 1) {
                Loge("Bad version on IP configuration file, ignore read");
                *result = NULL;
                TRY_RETURN(out_try_final, isReturnAfterFinally)
            }
            while (TRUE) {
                Int32 id = -1;
                // Default is DHCP with no proxy
                IpConfigurationIpAssignment ipAssignment = Elastos::Droid::Net::DHCP_IpAssignment;
                IpConfigurationProxySettings proxySettings = Elastos::Droid::Net::NONE_ProxySettings;
                AutoPtr<IStaticIpConfiguration> staticIpConfiguration;
                CStaticIpConfiguration::New((IStaticIpConfiguration**)&staticIpConfiguration);
                String proxyHost(NULL);
                String pacFileUrl(NULL);
                Int32 proxyPort = -1;
                String exclusionList(NULL);
                String key;
                do {
                    JUDGE(out_try, ec = in->ReadUTF(&key), needCatch)
                    ECode ec_inner;
                    Boolean inner_needCatch = FALSE;
                    TRY {
                        AutoPtr<INetworkUtils> helper;
                        CNetworkUtils::AcquireSingleton((INetworkUtils**)&helper);
                        if (key.Equals(ID_KEY)) {
                            JUDGE(inner_try, ec_inner = in->ReadInt32(&id), inner_needCatch)
                        }
                        else if (key.Equals(IP_ASSIGNMENT_KEY)) {
                            ipAssignment = StringUtils::ParseInt32(Ptr(in)->Func(in->ReadUTF));
                        }
                        else if (key.Equals(LINK_ADDRESS_KEY)) {
                            AutoPtr<IInetAddress> inetAddress;
                            JUDGE(inner_try, ec_inner = helper->NumericToInetAddress(Ptr(in)->Func(in->ReadUTF), (IInetAddress**)&inetAddress), inner_needCatch)
                            AutoPtr<ILinkAddress> linkAddr;
                            CLinkAddress::New(inetAddress, Ptr(in)->Func(in->ReadInt32), (ILinkAddress**)&linkAddr);
                            if (IInet4Address::Probe(Ptr(linkAddr)->Func(linkAddr->GetAddress)) != NULL &&
                                    Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetIpAddress) == NULL) {
                                staticIpConfiguration->SetIpAddress(linkAddr);
                            }
                            else {
                                Loge("Non-IPv4 or duplicate address: %s", Object::ToString(linkAddr).string());
                            }
                        }
                        else if (key.Equals(GATEWAY_KEY)) {
                            AutoPtr<ILinkAddress> dest;
                            AutoPtr<IInetAddress> gateway;
                            if (version == 1) {
                                // only supported default gateways - leave the dest/prefix empty
                                JUDGE(inner_try, ec_inner = helper->NumericToInetAddress(Ptr(in)->Func(in->ReadUTF), (IInetAddress**)&gateway), inner_needCatch)
                                if (Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetGateway) == NULL) {
                                    staticIpConfiguration->SetGateway(gateway);
                                } else {
                                    Loge("Duplicate gateway: %s", Ptr(gateway)->Func(gateway->GetHostAddress).string());
                                }
                            } else {
                                if (Ptr(in)->Func(in->ReadInt32) == 1) {
                                    AutoPtr<IInetAddress> inetAddress;
                                    JUDGE(inner_try, ec_inner = helper->NumericToInetAddress(Ptr(in)->Func(in->ReadUTF),
                                        (IInetAddress**)&inetAddress), inner_needCatch)
                                    CLinkAddress::New(inetAddress, Ptr(in)->Func(in->ReadInt32), (ILinkAddress**)&dest);
                                }
                                if (Ptr(in)->Func(in->ReadInt32) == 1) {
                                    JUDGE(inner_try, ec_inner = helper->NumericToInetAddress(Ptr(in)->Func(in->ReadUTF), (IInetAddress**)&gateway), inner_needCatch)
                                }
                                AutoPtr<IRouteInfo> route;
                                CRouteInfo::New(dest, gateway, (IRouteInfo**)&route);
                                if (Ptr(route)->Func(route->IsIPv4Default) &&
                                        Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetGateway) == NULL) {
                                    staticIpConfiguration->SetGateway(gateway);
                                } else {
                                    Loge("Non-IPv4 default or duplicate route: %s", Object::ToString(route).string());
                                }
                            }
                        } else if (key.Equals(DNS_KEY)) {
                            AutoPtr<IInetAddress> inetAddress;
                            JUDGE(inner_try, ec_inner = helper->NumericToInetAddress(Ptr(in)->Func(in->ReadUTF), (IInetAddress**)&inetAddress), inner_needCatch)
                            Ptr(staticIpConfiguration)->Func(staticIpConfiguration->GetDnsServers)->Add(inetAddress);
                        } else if (key.Equals(PROXY_SETTINGS_KEY)) {
                            proxySettings = StringUtils::ParseInt32(Ptr(in)->Func(in->ReadUTF));
                        } else if (key.Equals(PROXY_HOST_KEY)) {
                            JUDGE(inner_try, ec_inner = in->ReadUTF(&proxyHost), inner_needCatch)
                        } else if (key.Equals(PROXY_PORT_KEY)) {
                            JUDGE(inner_try, ec_inner = in->ReadInt32(&proxyPort), inner_needCatch)
                        } else if (key.Equals(PROXY_PAC_FILE)) {
                            JUDGE(inner_try, ec_inner = in->ReadUTF(&pacFileUrl), inner_needCatch)
                        } else if (key.Equals(EXCLUSION_LIST_KEY)) {
                            JUDGE(inner_try, ec_inner = in->ReadUTF(&exclusionList), inner_needCatch)
                        } else if (key.Equals(EOS)) {
                            break;
                        } else {
                            Loge("Ignore unknown key %s while reading", key.string());
                        }
                    }
                    inner_try:
                    CATCH(E_ILLEGAL_ARGUMENT_EXCEPTION, ec_inner, inner_needCatch) {
                        Loge("Ignore invalid address while reading %d", ec_inner);
                    }
                    TRY_END(ec_inner, inner_needCatch)
                } while (TRUE);
                if (id != -1) {
                    AutoPtr<IIpConfiguration> config;
                    CIpConfiguration::New((IIpConfiguration**)&config);
                    networks->Put(id, config);
                    switch (ipAssignment) {
                        case Elastos::Droid::Net::STATIC_IpAssignment:
                            config->SetStaticIpConfiguration(staticIpConfiguration);
                            config->SetIpAssignment(ipAssignment);
                            break;
                        case Elastos::Droid::Net::DHCP_IpAssignment:
                            config->SetIpAssignment(ipAssignment);
                            break;
                        case Elastos::Droid::Net::UNASSIGNED_IpAssignment:
                            Loge("BUG: Found UNASSIGNED IP on file, use DHCP");
                            config->SetIpAssignment(Elastos::Droid::Net::DHCP_IpAssignment);
                            break;
                        default:
                            Loge("Ignore invalid ip assignment while reading.");
                            config->SetIpAssignment(Elastos::Droid::Net::UNASSIGNED_IpAssignment);
                            break;
                    }
                    switch (proxySettings) {
                        case Elastos::Droid::Net::STATIC_ProxySettings: {
                                AutoPtr<IProxyInfo> proxyInfo;
                                CProxyInfo::New(proxyHost, proxyPort, exclusionList, (IProxyInfo**)&proxyInfo);
                                config->SetProxySettings(proxySettings);
                                config->SetHttpProxy(proxyInfo);
                            }
                            break;
                        case Elastos::Droid::Net::PAC_ProxySettings: {
                                AutoPtr<IProxyInfo> proxyPacProperties;
                                CProxyInfo::New(pacFileUrl, (IProxyInfo**)&proxyPacProperties);
                                config->SetProxySettings(proxySettings);
                                config->SetHttpProxy(proxyPacProperties);
                            }
                            break;
                        case Elastos::Droid::Net::NONE_ProxySettings:
                            config->SetProxySettings(proxySettings);
                            break;
                        case Elastos::Droid::Net::UNASSIGNED_ProxySettings:
                            Loge("BUG: Found UNASSIGNED proxy on file, use NONE");
                            config->SetProxySettings(Elastos::Droid::Net::NONE_ProxySettings);
                            break;
                        default:
                            Loge("Ignore invalid proxy settings while reading");
                            config->SetProxySettings(Elastos::Droid::Net::UNASSIGNED_ProxySettings);
                            break;
                    }
                } else {
                    if (DBG) Log("Missing id while parsing configuration");
                }
            }
        }
        out_try:
        CATCH(E_EOF_EXCEPTION, ec, needCatch) {
        }
        CATCH(E_IO_EXCEPTION, ec, needCatch) {
            Loge("Error parsing configuration: %d", ec);
        }
        FINALLY(out_try_final) {
            if (in != NULL) {
                // try {
                ICloseable::Probe(in)->Close();
                // } catch (Exception e) {}
            }
        }
        FINALLY_END(isReturnAfterFinally, ec)
        TRY_END(ec, needCatch)
        FUNC_RETURN(networks)
}

ECode IpConfigStore::Loge(
    /* [in] */ const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Logger::E(TAG, fmt, ap);
    va_end(ap);
    return NOERROR;
}

ECode IpConfigStore::Log(
    /* [in] */ const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Logger::D(TAG, fmt, ap);
    va_end(ap);
    return NOERROR;
}

} // namespace Net
} // namespace Server
} // namespace Droid
} // namespace Elastos
