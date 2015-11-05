
#ifndef __ELASTOS_DROID_NET_HTTP_HTTPLOG_H__
#define __ELASTOS_DROID_NET_HTTP_HTTPLOG_H__

#include <elastos.h>

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

class HttpLog
{
public:
    static const Boolean LOGV;

public:
    static CARAPI_(void) V(
        /* [in] */ const String& logMe);

    static CARAPI_(void) E(
        /* [in] */ const String& logMe);

private:
    static const String LOGTAG;

    static const Boolean DEBUG;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_HTTPLOG_H__
