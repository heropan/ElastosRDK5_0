
#ifndef __ELASTOS_NET_CSOCKETPERMISSION_H__
#define __ELASTOS_NET_CSOCKETPERMISSION_H__

#include "_ELASTOS_NET_CSocketPermission.h"
#include "Permission.h"
#include <elastos/AutoPtr.h>

CarClass(CSocketPermission)
    , public Permission
    , public ISocketPermission
    , public ISerializable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CSocketPermission();

    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ const String& action);

    CARAPI GetActions(
        /* [out] */ String* actions);

    CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* isImplied);

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection** permissionCollection);

    CARAPI_(Boolean) CheckHost(
        /* [in] */ ISocketPermission* sp);

private:
    CARAPI SetActions(
        /* [in] */ const String& actions);

    CARAPI ParsePort(
        /* [in] */ const String& hostPort,
        /* [in] */ const String& host);

    CARAPI_(String) ToCanonicalActionString(
        /* [in] */ const String& action);

    CARAPI_(String) GetIPString(
        /* [in] */ Boolean isCheck);

    CARAPI GetHostString(
        /* [in] */ const String& host,
        /* [out] */ String* hostName);

    static CARAPI_(Boolean) IsValidHexChar(
        /* [in] */ const Char32& c);

    static CARAPI_(Boolean) IsValidIP4Word(
        /* [in] */ const String& word);

    static CARAPI_(Boolean) IsIP6AddressInFullForm(
        /* [in] */ const String& ipAddress);

    static CARAPI_(Boolean) IsValidIP6Address(
        /* [in] */ const String& ipAddress);

public:
    // Bit masks for each of the possible actions
    static const Int32 SP_CONNECT;

    static const Int32 SP_LISTEN;

    static const Int32 SP_ACCEPT;

    static const Int32 SP_RESOLVE;

    String mHostName; // Host name as returned by InetAddress

    String mIpString; // IP address as returned by InetAddress

    Boolean mResolved; // IP address has been resolved

    // the port range;
    Int32 mPortMin;

    Int32 mPortMax;

    Int32 mActionsMask;

private:
    static const Int64 sSerialVersionUID;

    static const CString sActionNames[];

    // If a wildcard is present store the information
    Boolean mIsPartialWild;

    Boolean mIsWild;

    // The highest port number
    static const Int32 HIGHEST_PORT;

    // The lowest port number
    static const Int32 LOWEST_PORT;

    String mActions; // List of all actions allowed by this permission
};

#endif //__ELASTOS_NET_CSOCKETPERMISSION_H__
