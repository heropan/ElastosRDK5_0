#include "CNsdServiceInfo.h"
#include "ext/frameworkdef.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

CNsdServiceInfo::CNsdServiceInfo()
    : mPort(-1)
{
}

CNsdServiceInfo::~CNsdServiceInfo()
{
}

ECode CNsdServiceInfo::constructor()
{
    return NOERROR;
}

ECode CNsdServiceInfo::constructor(
    /* [in] */ const String& sn,
    /* [in] */ const String& rt,
    /* [in] */ IDnsSdTxtRecord* tr)
{
    mServiceName = sn;
    mServiceType = rt;
    mTxtRecord = tr;
    return NOERROR;
}

/** Get the service name */
ECode CNsdServiceInfo::GetServiceName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mServiceName;
    return NOERROR;
}

/** Set the service name */
ECode CNsdServiceInfo::SetServiceName(
    /* [in] */ const String& s)
{
    mServiceName = s;
    return NOERROR;
}

/** Get the service type */
ECode CNsdServiceInfo::GetServiceType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mServiceType;
    return NOERROR;
}

/** Set the service type */
ECode CNsdServiceInfo::SetServiceType(
    /* [in] */ const String& s)
{
    mServiceType = s;
    return NOERROR;
}

/** @hide */
ECode CNsdServiceInfo::GetTxtRecord(
    /* [out] */ IDnsSdTxtRecord** rec)
{
    VALIDATE_NOT_NULL(rec);
    *rec = mTxtRecord;
    REFCOUNT_ADD(*rec);
    return NOERROR;
}

/** @hide */
ECode CNsdServiceInfo::SetTxtRecord(
    /* [in] */ IDnsSdTxtRecord* t)
{
    mTxtRecord = t;
    return NOERROR;
}

/** Get the host address. The host address is valid for a resolved service. */
ECode CNsdServiceInfo::GetHost(
    /* [out] */ IInetAddress** host)
{
    VALIDATE_NOT_NULL(host);
    *host = mHost;
    REFCOUNT_ADD(*host);
    return NOERROR;
}

/** Set the host address */
ECode CNsdServiceInfo::SetHost(
    /* [in] */ IInetAddress* s)
{
    mHost = s;
    return NOERROR;
}

/** Get port number. The port number is valid for a resolved service. */
ECode CNsdServiceInfo::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;
    return NOERROR;
}

/** Set port number */
ECode CNsdServiceInfo::SetPort(
    /* [in] */ Int32 p)
{
    mPort = p;
    return NOERROR;
}

ECode CNsdServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mServiceName));
    FAIL_RETURN(dest->WriteString(mServiceType));
    FAIL_RETURN(dest->WriteInterfacePtr(mTxtRecord.Get()));
    FAIL_RETURN(dest->WriteInterfacePtr(mHost.Get()));
    FAIL_RETURN(dest->WriteInt32(mPort));

    return NOERROR;
}

ECode CNsdServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IInterface> obj;
    FAIL_RETURN(source->ReadString(&mServiceName));
    FAIL_RETURN(source->ReadString(&mServiceType));
    FAIL_RETURN(source->ReadInterfacePtrPtr((Handle32*)&obj));
    mTxtRecord = IDnsSdTxtRecord::Probe(obj);

    obj = NULL;
    FAIL_RETURN(source->ReadInterfacePtrPtr((Handle32*)&obj));
    mHost = IInetAddress::Probe(obj);

    FAIL_RETURN(source->ReadInt32(&mPort));
    return NOERROR;
}

ECode CNsdServiceInfo::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);

    StringBuilder sb;

    sb += "name: ";
    sb += mServiceName;
    sb += "type: ";
    sb += mServiceType;

    sb += "host: ";
    String temp;
    mHost->ToString(&temp);
    sb += temp;

    sb += "port: ";
    sb += StringUtils::Int32ToString(mPort);
    sb += "txtRecord: ";
    mTxtRecord->ToString(&temp);
    sb += temp;

    *info = sb.ToString();
    return NOERROR;
}

}
}
}
}
