
#ifndef __AUTHENTICATOR_H__
#define __AUTHENTICATOR_H__

#include "Elastos.CoreLibrary_server.h"
#include <elautoptr.h>
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Net {

extern const InterfaceID EIID_Authenticator;

class Authenticator : public ElRefBase
{
public:
    Authenticator();

    virtual CARAPI GetPasswordAuthentication(
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    virtual CARAPI GetRequestingPort(
        /* [out] */ Int32* port);

    virtual CARAPI GetRequestingSite(
        /* [out] */ IInetAddress** address);

    virtual CARAPI GetRequestingPrompt(
        /* [out] */ String* prompt);

    virtual CARAPI GetRequestingProtocol(
        /* [out] */ String* protocol);

    virtual CARAPI GetRequestingScheme(
        /* [out] */ String* scheme);

    static CARAPI RequestPasswordAuthentication(
        /* [in] */ IInetAddress* rAddr,
        /* [in] */ Int32 rPort,
        /* [in] */ const String& rProtocol,
        /* [in] */ const String& rPrompt,
        /* [in] */ const String& rScheme,
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    static CARAPI_(void) SetDefault(
        /* [in] */ IAuthenticator* a);

    static CARAPI RequestPasswordAuthenticationEx(
        /* [in] */ const String& rHost,
        /* [in] */ IInetAddress* rAddr,
        /* [in] */ Int32 rPort,
        /* [in] */ const String& rProtocol,
        /* [in] */ const String& rPrompt,
        /* [in] */ const String& rScheme,
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    static CARAPI RequestPasswordAuthenticationEx2(
        /* [in] */ const String& rHost,
        /* [in] */ IInetAddress* rAddr,
        /* [in] */ Int32 rPort,
        /* [in] */ const String& rProtocol,
        /* [in] */ const String& rPrompt,
        /* [in] */ const String& rScheme,
        /* [in] */ IURL* rURL,
        /* [in] */ AuthenticatorRequestorType reqType,
        /* [out] */ IPasswordAuthentication** passwordAuthentication);

    virtual CARAPI GetRequestingHost(
        /* [out] */ String* hostName);

    virtual CARAPI GetRequestingURL(
        /* [out] */ IURL** url);

    virtual CARAPI GetRequestorType(
        /* [out] */ AuthenticatorRequestorType* requestorType);

private:
    // the default authenticator that needs to be set
    static AutoPtr<Authenticator> sThisAuthenticator;

    // the requester connection info
    String mHost;

    AutoPtr<IInetAddress> mAddr;

    Int32 mPort;

    String mProtocol;

    String mPrompt;

    String mScheme;

    AutoPtr<IURL> mUrl;

    AuthenticatorRequestorType mRt;

    static Mutex sLock;
};

} // namespace Net
} // namespace Elastos

#endif //__AUTHENTICATOR_H__
