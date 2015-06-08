
#include "CSocketPermission.h"
#include "SocketPermissionCollection.h"
#include "InetAddress.h"
#include <StringBuffer.h>


const Int32 CSocketPermission::SP_CONNECT = 1;
const Int32 CSocketPermission::SP_LISTEN = 2;
const Int32 CSocketPermission::SP_ACCEPT = 4;
const Int32 CSocketPermission::SP_RESOLVE = 8;

const Int64 CSocketPermission::sSerialVersionUID = -7204263841984476862ll;

const CString CSocketPermission::sActionNames[] = {
        "", "connect", "listen", "", "accept", "", "", "", "resolve" };

const Int32 CSocketPermission::HIGHEST_PORT = 65535;
const Int32 CSocketPermission::LOWEST_PORT = 0;

CAR_INTERFACE_IMPL_2(CSocketPermission, Permission, ICSocketPermission, ISerializable)

CAR_OBJECT_IMPL(CSocketPermission)

CSocketPermission::CSocketPermission()
    : mResolved(FALSE)
    , mPortMin(LOWEST_PORT)
    , mPortMax(HIGHEST_PORT)
    , mActionsMask(SP_RESOLVE)
    , mIsPartialWild(FALSE)
    , mIsWild(FALSE)
{}

ECode CSocketPermission::constructor(
    /* [in] */ const String& host,
    /* [in] */ const String& action)
{
    FAIL_RETURN(Permission::Init(host.IsEmpty() ? String("localhost") : host));
    GetHostString(host, &mHostName);
    if (action.IsNull()) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (action.IsEmpty()) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SetActions(action);
    mActions = ToCanonicalActionString(action);
    // Use host since we are only checking for port presence
    return ParsePort(host, mHostName);
}

ECode CSocketPermission::GetActions(
    /* [out] */ String* actions)
{
    VALIDATE_NOT_NULL(actions);
    *actions = mActions;

    return NOERROR;
}

ECode CSocketPermission::SetActions(
    /* [in] */ const String& actions)
{
    if (actions.IsEmpty()) {
        return NOERROR;
    }
    Boolean parsing = TRUE;
    String action;
    StringBuffer sb;
    Int32 pos = 0, length = actions.GetLength();
    while (parsing) {
        Char32 c;
        sb.SetLength(0);
        while (pos < length && (c = actions.GetChar(pos++)) != ',') {
            sb += c;
        }
        if (pos == length) {
            parsing = FALSE;
        }
        action = ((String)sb).Trim();
        action.ToLowerCase();
        if (action.Equals(sActionNames[SP_CONNECT])) {
            mActionsMask |= SP_CONNECT;
        }
        else if (action.Equals(sActionNames[SP_LISTEN])) {
            mActionsMask |= SP_LISTEN;
        }
        else if (action.Equals(sActionNames[SP_ACCEPT])) {
            mActionsMask |= SP_ACCEPT;
        }
        else if (action.Equals(sActionNames[SP_RESOLVE])) {
            // do nothing
        }
        else {
//            throw new IllegalArgumentException("Invalid action: " + action);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode CSocketPermission::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean* isImplied)
{
    VALIDATE_NOT_NULL(isImplied);

    ISocketPermission* sp = ISocketPermission::Probe(permission);
//    try {
    if (sp == NULL) {
        *isImplied = FALSE;
        return NOERROR;
    }
//    } catch (ClassCastException e) {
//        return false;
//    }

    // tests if the action list of p is the subset of the one of the
    // receiver
    CSocketPermission* spObj = (CSocketPermission*)sp;
    if ((mActionsMask & spObj->mActionsMask) != spObj->mActionsMask) {
        *isImplied = FALSE;
        return NOERROR;
    }

    // only check the port range if the action string of the current object
    // is not "resolve"
    String actions;
    //Todo:
    assert(0);
    // permission->GetActions(&actions);
    if (!actions.Equals("resolve")) {
        if ((spObj->mPortMin < mPortMin) || (spObj->mPortMax > mPortMax)) {
            *isImplied = FALSE;
            return NOERROR;
        }
    }

    // Verify the host is valid
    *isImplied = CheckHost(sp);
    return NOERROR;
}

ECode CSocketPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** permissionCollection)
{
    VALIDATE_NOT_NULL(permissionCollection);

    *permissionCollection = new SocketPermissionCollection();
    REFCOUNT_ADD(*permissionCollection);
    return NOERROR;
}

ECode CSocketPermission::ParsePort(
    /* [in] */ const String& hostPort,
    /* [in] */ const String& host)
{
    String port = hostPort.Substring(host.GetLength());
    String emptyString("");

    if (emptyString.Equals(port)) {
        // Not specified
        mPortMin = 80;
        mPortMax = 80;
        return NOERROR;
    }

    if (CString(":*").Equals(port)) {
        // The port range should be 0-65535
        mPortMin = 0;
        mPortMax = 65535;
        return NOERROR;
    }

    // Omit ':'
    port = port.Substring(1);
    Int32 negIdx = port.IndexOf('-');
    String strPortMin = emptyString;
    String strPortMax = emptyString;
    if (-1 == negIdx) {
        // No neg mark, only one number
        strPortMin = port;
        strPortMax = port;
    }
    else {
        strPortMin = port.Substring(0, negIdx);
        strPortMax = port.Substring(negIdx + 1);
        if (emptyString.Equals(strPortMin)) {
            strPortMin = "0";
        }
        if (emptyString.Equals(strPortMax)) {
            strPortMax = "65535";
        }
    }
//    try {
    mPortMin = strPortMin.ToInt32();//Integer.valueOf(strPortMin).intValue();
    mPortMax = strPortMax.ToInt32();//Integer.valueOf(strPortMax).intValue();

    if (mPortMin == -1 || mPortMax == -1 ||  mPortMin > mPortMax) {
//        throw new IllegalArgumentException("MinPort is greater than MaxPort: " + port);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // } catch (NumberFormatException e) {
    //     throw new IllegalArgumentException("Invalid port number: " + port);
    // }

    return NOERROR;
}

String CSocketPermission::ToCanonicalActionString(
    /* [in] */ const String& action)
{
    if (action.IsNull() || action.IsEmpty() || mActionsMask == SP_RESOLVE) {
        return String(sActionNames[SP_RESOLVE]); // If none specified return the
    }
    // implied action resolve
    StringBuffer sb;
    if ((mActionsMask & SP_CONNECT) == SP_CONNECT) {
        sb += ',';
        sb += sActionNames[SP_CONNECT];
    }
    if ((mActionsMask & SP_LISTEN) == SP_LISTEN) {
        sb += ',';
        sb += sActionNames[SP_LISTEN];
    }
    if ((mActionsMask & SP_ACCEPT) == SP_ACCEPT) {
        sb += ',';
        sb += sActionNames[SP_ACCEPT];
    }
    sb += ',';
    sb += sActionNames[SP_RESOLVE];// Resolve is always implied
    // Don't copy the first ','.
    return mActions = sb.Substring(1, sb.GetLength());
}

String CSocketPermission::GetIPString(
    /* [in] */ Boolean isCheck)
{
    if (!mResolved) {
//        try {
        // TODO:
        // InetAddress::GetHostNameInternal(mHostName, isCheck, &mIpString);
//        } catch (UnknownHostException e) {
            // ignore
//        }
        mResolved = TRUE;
    }
    return mIpString;
}

ECode CSocketPermission::GetHostString(
    /* [in] */ const String& host,
    /* [out] */ String* hostName)
{
    assert(hostName != NULL);

    String hostTemp = host;
    hostTemp = hostTemp.Trim();
    Int32 idx = -1;
    idx = hostTemp.IndexOf(':');
    mIsPartialWild = (hostTemp.GetCharCount() > 0 && hostTemp.GetChar(0) == '*');
    if (mIsPartialWild) {
        mResolved = TRUE;
        mIsWild = (hostTemp.GetCharCount() == 1);
        if (mIsWild) {
            *hostName = hostTemp;
            return NOERROR;
        }
        if (idx > -1) {
            hostTemp = hostTemp.Substring(0, idx);
        }
        *hostName = hostTemp;
        hostName->ToLowerCase();
        return NOERROR;
    }

    Int32 lastIdx = hostTemp.LastIndexOf(':');

    if (idx == lastIdx) {
        if (-1 != idx) {
            // only one colon, should be port
            hostTemp = hostTemp.Substring(0, idx);
        }
        *hostName = hostTemp;
        hostName->ToLowerCase();
        return NOERROR;
    }
        // maybe ipv6
    Boolean isFirstBracket = (hostTemp.GetChar(0) == '[');
    if (!isFirstBracket) {
        // No bracket, should be in full form
        Int32 colonNum = 0;
        for (Int32 i = 0; (UInt32)i < hostTemp.GetCharCount(); ++i) {
            if (hostTemp.GetChar(i) == ':') {
                colonNum++;
            }
        }
        // Get rid of the colon before port
        if (8 == colonNum) {
            hostTemp = hostTemp.Substring(0, lastIdx);
        }
        if (IsIP6AddressInFullForm(hostTemp)) {
            *hostName = hostTemp;
            hostName->ToLowerCase();
            return NOERROR;
        }
//        throw new IllegalArgumentException("Invalid port number: " + host);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // forward bracket found
    Int32 bbracketIdx = hostTemp.IndexOf(']');
    if (-1 == bbracketIdx) {
        // no back bracket found, wrong
//        throw new IllegalArgumentException("Invalid port number: " + host);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    hostTemp = hostTemp.Substring(0, bbracketIdx + 1);
    if (IsValidIP6Address(hostTemp)) {
        *hostName = hostTemp;
        hostName->ToLowerCase();
        return NOERROR;
    }
//    throw new IllegalArgumentException("Invalid port number: " + host);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

Boolean CSocketPermission::IsValidHexChar(
    /* [in] */ const Char32& c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

Boolean CSocketPermission::IsValidIP4Word(
    /* [in] */ const String& word)
{
    Char32 c;
    if (word.GetCharCount() < 1 || word.GetCharCount() > 3) {
        return FALSE;
    }
    for (Int32 i = 0; (UInt32)i < word.GetCharCount(); i++) {
        c = word.GetChar(i);
        if (!(c >= '0' && c <= '9')) {
            return FALSE;
        }
    }
    if (word.ToInt32()/*Integer.parseInt(word)*/ > 255) {
        return FALSE;
    }
    return TRUE;
}

Boolean CSocketPermission::IsIP6AddressInFullForm(
    /* [in] */ const String& ipAddress)
{
    if (IsValidIP6Address(ipAddress)) {
        Int32 doubleColonIndex = ipAddress.IndexOf("::");
        if (doubleColonIndex >= 0) {
            // Simplified form which contains ::
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

Boolean CSocketPermission::IsValidIP6Address(
    /* [in] */ const String& ipAddress)
{
    Int32 length = ipAddress.GetCharCount();
    Boolean doubleColon = FALSE;
    Int32 numberOfColons = 0;
    Int32 numberOfPeriods = 0;
    Int32 numberOfPercent = 0;
    String word("");
    Char32 c = 0;
    Char32 prevChar = 0;
    Int32 offset = 0; // offset for [] IP addresses

    if (length < 2) {
        return FALSE;
    }

    for (Int32 i = 0; i < length; i++) {
        prevChar = c;
        c = ipAddress.GetChar(i);
        switch (c) {

            // case for an open bracket [x:x:x:...x]
        case '[':
            if (i != 0) {
                return FALSE; // must be first character
            }
            if (ipAddress.GetChar(length - 1) != ']') {
                return FALSE; // must have a close ]
            }
            offset = 1;
            if (length < 4) {
                return FALSE;
            }
            break;

            // case for a closed bracket at end of IP [x:x:x:...x]
        case ']':
            if (i != length - 1) {
                return FALSE; // must be last character
            }
            if (ipAddress.GetChar(0) != '[') {
                return FALSE; // must have a open [
            }
            break;

            // case for the last 32-bits represented as IPv4 x:x:x:x:x:x:d.d.d.d
        case '.':
            numberOfPeriods++;
            if (numberOfPeriods > 3) {
                return FALSE;
            }
            if (!IsValidIP4Word(word)) {
                return FALSE;
            }
            if (numberOfColons != 6 && !doubleColon) {
                return FALSE;
            }
            // a special case ::1:2:3:4:5:d.d.d.d allows 7 colons with an
            // IPv4 ending, otherwise 7 :'s is bad
            if (numberOfColons == 7 && ipAddress.GetChar(0 + offset) != ':'
                    && ipAddress.GetChar(1 + offset) != ':') {
                return FALSE;
            }
            word = "";
            break;

        case ':':
            numberOfColons++;
            if (numberOfColons > 7) {
                return FALSE;
            }
            if (numberOfPeriods > 0) {
                return FALSE;
            }
            if (prevChar == ':') {
                if (doubleColon) {
                    return FALSE;
                }
                doubleColon = TRUE;
            }
            word = "";
            break;
        case '%':
            if (numberOfColons == 0) {
                return FALSE;
            }
            numberOfPercent++;

            // validate that the stuff after the % is valid
            if ((i + 1) >= length) {
                // in this case the percent is there but no number is
                // available
                return FALSE;
            }
//            try {
            //Integer.parseInt(ipAddress.substring(i + 1));
            if (ipAddress.Substring(i + 1).ToInt32() == -1) {
                return FALSE;
            }
//            } catch (NumberFormatException e) {
                // right now we just support an integer after the % so if
                // this is not
                // what is there then return
//                return FALSE;
//            }
            break;

        default:
            if (numberOfPercent == 0) {
                if (word.GetCharCount() > 3) {
                    return FALSE;
                }
                if (!IsValidHexChar(c)) {
                    return FALSE;
                }
            }
            word.AppendFormat("%c", c);
        }
    }

    // Check if we have an IPv4 ending
    if (numberOfPeriods > 0) {
        if (numberOfPeriods != 3 || !IsValidIP4Word(word)) {
            return FALSE;
        }
    }
    else {
        // If we're at then end and we haven't had 7 colons then there is a
        // problem unless we encountered a doubleColon
        if (numberOfColons != 7 && !doubleColon) {
            return FALSE;
        }

        // If we have an empty word at the end, it means we ended in either
        // a : or a .
        // If we did not end in :: then this is invalid
        if (numberOfPercent == 0) {
            if (word == "" && ipAddress.GetChar(length - 1 - offset) == ':'
                    && ipAddress.GetChar(length - 2 - offset) != ':') {
                return FALSE;
            }
        }
    }

    return TRUE;
}

Boolean CSocketPermission::CheckHost(
    /* [in] */ ISocketPermission* sp)
{
    CSocketPermission* spObj = (CSocketPermission*)sp;
    if (mIsPartialWild) {
        if (mIsWild) {
            return TRUE; // Match on any host
        }
        Int32 length = mHostName.GetCharCount() - 1;
        String s = spObj->mHostName.Substring(spObj->mHostName.GetCharCount() - length);
        return s.Equals(mHostName.Substring(1, length));
    }
    // The ipString may not be the same, some hosts resolve to
    // multiple ips
    return (!GetIPString(FALSE).IsNull() && mIpString.Equals(spObj->GetIPString(FALSE)))
            || mHostName.Equals(spObj->mHostName);
}

