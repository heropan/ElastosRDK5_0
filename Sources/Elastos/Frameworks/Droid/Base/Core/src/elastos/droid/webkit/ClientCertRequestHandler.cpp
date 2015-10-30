
#include "elastos/droid/webkit/ClientCertRequestHandler.h"
#include "elastos/droid/webkit/BrowserFrame.h"

using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//          ClientCertRequestHandler::IgnoreRunnable
//===============================================================

ClientCertRequestHandler::IgnoreRunnable::IgnoreRunnable(
    /* [in] */ ClientCertRequestHandler* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(ClientCertRequestHandler::IgnoreRunnable, IRunnable);

ECode ClientCertRequestHandler::IgnoreRunnable::Run()
{
    mOwner->mBrowserFrame->NativeSslClientCert(mOwner->mHandle, 0, NULL);
    return NOERROR;
}

ECode ClientCertRequestHandler::IgnoreRunnable::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "ClientCertRequestHandler::IgnoreRunnable";
    return NOERROR;
}

//===============================================================
//          ClientCertRequestHandler::CancelRunnable
//===============================================================

ClientCertRequestHandler::CancelRunnable::CancelRunnable(
    /* [in] */ ClientCertRequestHandler* owner)
    : mOwner(owner)
{
}

CAR_INTERFACE_IMPL_LIGHT(ClientCertRequestHandler::CancelRunnable, IRunnable);

ECode ClientCertRequestHandler::CancelRunnable::Run()
{
    mOwner->mBrowserFrame->NativeSslClientCert(mOwner->mHandle, 0, NULL);
    return NOERROR;
}

ECode ClientCertRequestHandler::CancelRunnable::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "ClientCertRequestHandler::CancelRunnable";
    return NOERROR;
}

//===============================================================
//          ClientCertRequestHandler::CtxRunnable
//===============================================================

ClientCertRequestHandler::CtxRunnable::CtxRunnable(
    /* [in] */ ClientCertRequestHandler* owner,
    /* [in] */ Int32 ctx,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes)
    : mOwner(owner)
    , mCtx(ctx)
    , mChainBytes(chainBytes)
{
}

CAR_INTERFACE_IMPL_LIGHT(ClientCertRequestHandler::CtxRunnable, IRunnable);

ECode ClientCertRequestHandler::CtxRunnable::Run()
{
    assert(0);
//    mOwner->mBrowserFrame->NativeSslClientCert(mOwner->mHandle, ctx, chainBytes);
    return NOERROR;
}

ECode ClientCertRequestHandler::CtxRunnable::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "ClientCertRequestHandler::CtxRunnable";
    return NOERROR;
}

//===============================================================
//          ClientCertRequestHandler::PKCS8Runnable
//===============================================================

ClientCertRequestHandler::PKCS8Runnable::PKCS8Runnable(
    /* [in] */ ClientCertRequestHandler* owner,
    /* [in] */ ArrayOf<Byte>* key,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes)
    : mOwner(owner)
    , mKey(key)
    , mChainBytes(chainBytes)
{
}

CAR_INTERFACE_IMPL_LIGHT(ClientCertRequestHandler::PKCS8Runnable, IRunnable);

ECode ClientCertRequestHandler::PKCS8Runnable::Run()
{
    return NOERROR;
}

ECode ClientCertRequestHandler::PKCS8Runnable::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "ClientCertRequestHandler::PKCS8Runnable";
    return NOERROR;
}

//===============================================================
//                   ClientCertRequestHandler
//===============================================================

ClientCertRequestHandler::ClientCertRequestHandler(
    /* [in] */ BrowserFrame* browserFrame,
    /* [in] */ Int32 handle,
    /* [in] */ const String& host_and_port,
    /* [in] */ SslClientCertLookupTable* table)
      : mBrowserFrame(browserFrame)
      , mHandle(handle)
      , mHostAndPort(host_and_port)
      , mTable(table)
{
    Handler::Init();
}

CAR_INTERFACE_IMPL(ClientCertRequestHandler, IClientCertRequestHandler);

IHANDLER_METHODS_IMPL(ClientCertRequestHandler, Handler);

/**
 * Proceed with the specified private key and client certificate chain.
 */
//ECode ClientCertRequestHandler::Proceed(
//    /* [in] */ IPrivateKey* privateKey,
//    /* [in] */ ArrayOf<X509Certificate>* chain)
//{}

/**
 * Igore the request for now, the user may be prompted again.
 */
ECode ClientCertRequestHandler::Ignore()
{
    AutoPtr<IRunnable> runnable = new IgnoreRunnable(this);
    Boolean result = FALSE;
    Post(runnable, &result);

    return NOERROR;
}

/**
 * Cancel this request, remember the users negative choice.
 */
ECode ClientCertRequestHandler::Cancel()
{
    AutoPtr<IRunnable> runnable = new CancelRunnable(this);
    Boolean result = FALSE;
    Post(runnable, &result);

    return NOERROR;
}

/**
 * Proceed with the specified private key bytes and client certificate chain.
 */
void ClientCertRequestHandler::SetSslClientCertFromCtx(
    /* [in] */ const Int32 ctx,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes)
{
    AutoPtr<IRunnable> runnable = new CtxRunnable(this, ctx, chainBytes);
    Boolean result = FALSE;
    Post(runnable, &result);
}

/**
 * Proceed with the specified private key context and client certificate chain.
 */
void ClientCertRequestHandler::SetSslClientCertFromPKCS8(
    /* [in] */ ArrayOf<Byte>* key,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes)
{
    AutoPtr<IRunnable> runnable = new PKCS8Runnable(this, key, chainBytes);
    Boolean result = FALSE;
    Post(runnable, &result);
}

ECode ClientCertRequestHandler::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "ClientCertRequestHandler";
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos