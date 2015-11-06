
#ifndef __ELASTOS_DROID_NET_HTTP_HTTPLOG_H__
#define __ELASTOS_DROID_NET_HTTP_HTTPLOG_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * {@hide}
 */
class HttpLog
    : public Object
{
public:
    static CARAPI V(
        /* [in] */ const String& logMe);

    static CARAPI E(
        /* [in] */ const String& logMe);

    static const Boolean LOGV;

private:
    static const String LOGTAG;

    static const Boolean DEBUG;
};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_HTTP_HTTPLOG_H__
