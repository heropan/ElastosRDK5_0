
#include "cmdef.h"
#include "Authenticator.h"

namespace Elastos {
namespace Net {

// {83E9D351-ACF2-49a2-AD77-643EC6CDDAF7}
const InterfaceID EIID_Authenticator =
    { 0x83e9d351, 0xacf2, 0x49a2, { 0xad, 0x77, 0x64, 0x3e, 0xc6, 0xcd, 0xda, 0xf7 } };

AutoPtr<Authenticator> Authenticator::sThisAuthenticator = NULL;

Mutex Authenticator::sLock;

Authenticator::Authenticator()
    : mPort(0)
{}

ECode Authenticator::GetPasswordAuthentication(
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    VALIDATE_NOT_NULL(passwordAuthentication)

    *passwordAuthentication = NULL;
    return NOERROR;
}

ECode Authenticator::GetRequestingPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port)

    *port = mPort;
    return NOERROR;
}

ECode Authenticator::GetRequestingSite(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address)

    *address = mAddr;
    INTERFACE_ADDREF(*address);
    return NOERROR;
}

ECode Authenticator::GetRequestingPrompt(
    /* [out] */ String* prompt)
{
    VALIDATE_NOT_NULL(prompt)

    *prompt = mPrompt;
    return NOERROR;
}

ECode Authenticator::GetRequestingProtocol(
    /* [out] */ String* protocol)
{
    VALIDATE_NOT_NULL(protocol)

    *protocol = mProtocol;
    return NOERROR;
}

ECode Authenticator::GetRequestingScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme)

    *scheme = mScheme;
    return NOERROR;
}

ECode Authenticator::RequestPasswordAuthentication(
    /* [in] */ IInetAddress* rAddr,
    /* [in] */ Int32 rPort,
    /* [in] */ const String& rProtocol,
    /* [in] */ const String& rPrompt,
    /* [in] */ const String& rScheme,
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    VALIDATE_NOT_NULL(passwordAuthentication)

    Mutex::Autolock lock(sLock);

//    SecurityManager sm = System.getSecurityManager();
//    if (sm != null) {
//        sm.checkPermission(requestPasswordAuthenticationPermission);
//    }
    if (sThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // set the requester info so it knows what it is requesting
    // authentication for
    sThisAuthenticator->mAddr = rAddr;
    sThisAuthenticator->mPort = rPort;
    sThisAuthenticator->mProtocol = rProtocol;
    sThisAuthenticator->mPrompt = rPrompt;
    sThisAuthenticator->mScheme = rScheme;
    sThisAuthenticator->mRt = AuthenticatorRequestorType_SERVER;

    // returns the authentication info obtained by the registered
    // Authenticator
    return sThisAuthenticator->GetPasswordAuthentication(passwordAuthentication);
}

void Authenticator::SetDefault(
    /* [in] */ IAuthenticator* a)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(setDefaultAuthenticatorPermission);
    // }
    sThisAuthenticator = a == NULL ? NULL : (Authenticator*)a->Probe(EIID_Authenticator);
}

ECode Authenticator::RequestPasswordAuthenticationEx(
    /* [in] */ const String& rHost,
    /* [in] */ IInetAddress* rAddr,
    /* [in] */ Int32 rPort,
    /* [in] */ const String& rProtocol,
    /* [in] */ const String& rPrompt,
    /* [in] */ const String& rScheme,
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    VALIDATE_NOT_NULL(passwordAuthentication)

    Mutex::Autolock lock(sLock);

    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(requestPasswordAuthenticationPermission);
    // }
    if (sThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // set the requester info so it knows what it is requesting
    // authentication for
    sThisAuthenticator->mHost = rHost;
    sThisAuthenticator->mAddr = rAddr;
    sThisAuthenticator->mPort = rPort;
    sThisAuthenticator->mProtocol = rProtocol;
    sThisAuthenticator->mPrompt = rPrompt;
    sThisAuthenticator->mScheme = rScheme;
    sThisAuthenticator->mRt = AuthenticatorRequestorType_SERVER;

    // returns the authentication info obtained by the registered
    // Authenticator
    return sThisAuthenticator->GetPasswordAuthentication(passwordAuthentication);
}

ECode Authenticator::GetRequestingHost(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName)

    *hostName = mHost;
    return NOERROR;
}

ECode Authenticator::RequestPasswordAuthenticationEx2(
    /* [in] */ const String& rHost,
    /* [in] */ IInetAddress* rAddr,
    /* [in] */ Int32 rPort,
    /* [in] */ const String& rProtocol,
    /* [in] */ const String& rPrompt,
    /* [in] */ const String& rScheme,
    /* [in] */ IURL* rURL,
    /* [in] */ AuthenticatorRequestorType reqType,
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    VALIDATE_NOT_NULL(passwordAuthentication)

    Mutex::Autolock lock(sLock);

    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(requestPasswordAuthenticationPermission);
    // }
    if (sThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // sets the requester info so it knows what it is requesting
    // authentication for
    sThisAuthenticator->mHost = rHost;
    sThisAuthenticator->mAddr = rAddr;
    sThisAuthenticator->mPort = rPort;
    sThisAuthenticator->mProtocol = rProtocol;
    sThisAuthenticator->mPrompt = rPrompt;
    sThisAuthenticator->mScheme = rScheme;
    sThisAuthenticator->mUrl = rURL;
    sThisAuthenticator->mRt = reqType;
    sThisAuthenticator->mRt = AuthenticatorRequestorType_SERVER;

    // returns the authentication info obtained by the registered
    // Authenticator
    return sThisAuthenticator->GetPasswordAuthentication(passwordAuthentication);
}

ECode Authenticator::GetRequestingURL(
    /* [out] */ IURL** url)
{
    VALIDATE_NOT_NULL(url)

    *url = mUrl;
    INTERFACE_ADDREF(*url);
    return NOERROR;
}

ECode Authenticator::GetRequestorType(
    /* [out] */ AuthenticatorRequestorType* requestorType)
{
    VALIDATE_NOT_NULL(requestorType)

    *requestorType = mRt;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
