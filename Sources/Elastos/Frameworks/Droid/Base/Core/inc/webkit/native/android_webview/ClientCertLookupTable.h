
#ifndef __CLIENTCERTLOOKUPTABLE_H__
#define __CLIENTCERTLOOKUPTABLE_H__

// import org.chromium.net.AndroidPrivateKey;

// import java.util.Arrays;
// import java.util.HashMap;
// import java.util.HashSet;
// import java.util.Map;
// import java.util.Set;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Store user's client certificate decision for a host and port pair. Not
 * thread-safe. All accesses are done on UI thread.
 */
class ClientCertLookupTable
{
public:
    /**
     * A container for the certificate data.
     */
    class Cert
    {
    public:
        Cert(
            /* [in] */ AndroidPrivateKey* privateKey,
            /* [in] */ ArrayOf< ArrayOf<Byte> >* certChain);

    public:
        AutoPtr<AndroidPrivateKey> privateKey;
        AutoPtr< ArrayOf< ArrayOf<Byte> > > certChain;
    };

public:
    ClientCertLookupTable();

    // Clear client certificate preferences
    CARAPI_(void) Clear();

    CARAPI_(void) Allow(
        /* [in] */ String host,
        /* [in] */ Int32 port,
        /* [in] */ AndroidPrivateKey* privateKey,
        /* [in] */ ArrayOf< ArrayOf<Byte> >* chain);

    CARAPI_(void) Deny(
        /* [in] */ String host,
        /* [in] */ Int32 port);

    CARAPI_(AutoPtr<Cert>) GetCertData(
        /* [in] */ String host,
        /* [in] */ Int32 port);

    CARAPI_(Boolean) IsDenied(
        /* [in] */ String host,
        /* [in] */ Int32 port);

private:
    // TODO(sgurun) add a test for this. Not separating host and pair properly will be
    // a security issue.
    static CARAPI_(String) HostAndPort(
        /* [in] */ String host,
        /* [in] */ Int32 port);

private:
    const Map<String, Cert> mCerts;
    const Set<String> mDenieds;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__CLIENTCERTLOOKUPTABLE_H__
