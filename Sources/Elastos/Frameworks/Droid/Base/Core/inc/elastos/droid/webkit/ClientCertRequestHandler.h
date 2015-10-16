
#ifndef __ELASTOS_DROID_WEBKIT_CLIENTCERTREQUESTHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_CLIENTCERTREQUESTHANDLER_H__

#include "elastos/droid/os/Handler.h"

#include "elastos/droid/webkit/SslClientCertLookupTable.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Os::Handler;
using Elastos::Security::Cert::IX509Certificate;

namespace Elastos {
namespace Droid {
namespace Webkit {

class BrowserFrame;

/**
 * ClientCertRequestHandler: class responsible for handling client
 * certificate requests.  This class is passed as a parameter to
 * BrowserCallback.displayClientCertRequestDialog and is meant to
 * receive the user's response.
 *
 * @hide
 */
class ClientCertRequestHandler
    : public ElRefBase
    , public IClientCertRequestHandler
    , public Handler
{
private:
    class IgnoreRunnable
        : public ElLightRefBase
        , public IRunnable
    {
    public:
        IgnoreRunnable(
            /* [in] */ ClientCertRequestHandler* owner);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        ClientCertRequestHandler* mOwner;
    };

    class CancelRunnable
        : public ElLightRefBase
        , public IRunnable
    {
    public:
        CancelRunnable(
            /* [in] */ ClientCertRequestHandler* owner);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        ClientCertRequestHandler* mOwner;
    };

    class CtxRunnable
        : public ElLightRefBase
        , public IRunnable
    {
    public:
        CtxRunnable(
            /* [in] */ ClientCertRequestHandler* owner,
            /* [in] */ Int32 ctx,
            /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        ClientCertRequestHandler* mOwner;
        Int32 mCtx;
        ArrayOf< AutoPtr< ArrayOf<Byte> > >* mChainBytes;
    };

    class PKCS8Runnable
        : public ElLightRefBase
        , public IRunnable
    {
    public:
        PKCS8Runnable(
            /* [in] */ ClientCertRequestHandler* owner,
            /* [in] */ ArrayOf<Byte>* key,
            /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes);

        CAR_INTERFACE_DECL();

        CARAPI Run();

    private:
        ClientCertRequestHandler* mOwner;
        ArrayOf<Byte>* mKey;
        ArrayOf< AutoPtr< ArrayOf<Byte> > >* mChainBytes;
    };

public:
    ClientCertRequestHandler(
        /* [in] */ BrowserFrame* browserFrame,
        /* [in] */ Int32 handle,
        /* [in] */ const String& host_and_port,
        /* [in] */ SslClientCertLookupTable* table);

    CAR_INTERFACE_DECL();

    IHANDLER_METHODS_DECL();

    /**
     * Proceed with the specified private key and client certificate chain.
     */
//    CARAPI Proceed(
//        /* [in] */ IPrivateKey* privateKey,
//        /* [in] */ ArrayOf<IX509Certificate>* chain);

    /**
     * Igore the request for now, the user may be prompted again.
     */
    CARAPI Ignore();

    /**
     * Cancel this request, remember the users negative choice.
     */
    CARAPI Cancel();

private:
    /**
     * Proceed with the specified private key bytes and client certificate chain.
     */
    CARAPI_(void) SetSslClientCertFromCtx(
        /* [in] */ const Int32 ctx,
        /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes);

    /**
     * Proceed with the specified private key context and client certificate chain.
     */
    CARAPI_(void) SetSslClientCertFromPKCS8(
        /* [in] */ ArrayOf<Byte>* key,
        /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chainBytes);

private:
    const AutoPtr<BrowserFrame> mBrowserFrame;
    Int32 mHandle;
    String mHostAndPort;
    AutoPtr<SslClientCertLookupTable> mTable;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_CLIENTCERTREQUESTHANDLER_H__
