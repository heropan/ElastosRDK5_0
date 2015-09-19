
#ifndef __ELASTOS_DROID_NET_CSSLSESSIONCACHE_H__
#define __ELASTOS_DROID_NET_CSSLSESSIONCACHE_H__

#include "_Elastos_Droid_Net_CSSLSessionCache.h"


namespace Elastos {
namespace Droid {
namespace Net {

/**
 * File-based cache of established SSL sessions.  When re-establishing a
 * connection to the same server, using an SSL session cache can save some time,
 * power, and bandwidth by skipping directly to an encrypted stream.
 * This is a persistent cache which can span executions of the application.
 *
 * @see SSLCertificateSocketFactory
 */

CarClass(CSSLSessionCache)
{
public:
    constructor(
        /* [in] */ IFile* dir);

    constructor(
        /* [in] */ IContext* context);

private:
    static const String TAG;// = "SSLSessionCache";
    /* package */
    AutoPtr<ISSLClientSessionCache> mSessionCache;
};

} //namespace Net
} //namespace Droid
} //namespace Elastos

#endif // __ELASTOS_DROID_NET_CSSLSESSIONCACHE_H__
