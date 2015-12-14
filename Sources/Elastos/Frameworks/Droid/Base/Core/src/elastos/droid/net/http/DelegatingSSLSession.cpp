
#include "Elastos.CoreLibrary.Security.h"
#include "elastos/droid/net/http/DelegatingSSLSession.h"

using Elastosx::Net::Ssl::EIID_ISSLSession;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL_2(DelegatingSSLSession, Object, ISSLSession, IDelegatingSSLSession)

ECode DelegatingSSLSession::constructor()
{
    return NOERROR;
}

ECode DelegatingSSLSession::GetApplicationBufferSize(
    /* [out] */ Int32* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetCipherSuite(
    /* [out] */ String* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetCreationTime(
    /* [out] */ Int64* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetId(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetLastAccessedTime(
    /* [out] */ Int64* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetLocalCertificates(
    /* [out, callee] */ ArrayOf<ICertificate*>** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetLocalPrincipal(
    /* [out] */ IPrincipal** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetPacketBufferSize(
    /* [out] */ Int32* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetPeerCertificateChain(
    /* [out, callee] */ ArrayOf<Elastosx::Security::Cert::IX509Certificate*>** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetPeerCertificates(
    /* [out, callee] */ ArrayOf<ICertificate*>** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetPeerHost(
    /* [out] */ String* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetPeerPort(
    /* [out] */ Int32* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetPeerPrincipal(
    /* [out] */ IPrincipal** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetProtocol(
    /* [out] */ String* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetSessionContext(
    /* [out] */ ISSLSessionContext** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetValue(
    /* [in] */ const String& name,
    /* [out] */ IInterface** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::GetValueNames(
    /* [out, callee] */ ArrayOf<String>** result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::Invalidate()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::IsValid(
    /* [out] */ Boolean* result)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::PutValue(
    /* [in] */ const String& name,
    /* [in] */ IInterface* value)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode DelegatingSSLSession::RemoveValue(
    /* [in] */ const String& name)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//================================================================
// DelegatingSSLSessionCertificateWrap
//================================================================
CAR_INTERFACE_IMPL(DelegatingSSLSessionCertificateWrap, DelegatingSSLSession, IDelegatingSSLSessionCertificateWrap)

ECode DelegatingSSLSessionCertificateWrap::constructor(
    /* [in] */ ICertificate* certificate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mCertificate = certificate;
#endif
}

ECode DelegatingSSLSessionCertificateWrap::GetPeerCertificates(
    /* [out, callee] */ ArrayOf<ICertificate*>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return new Certificate[] { mCertificate };
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
