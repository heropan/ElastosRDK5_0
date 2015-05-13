
#ifndef __HTTPLOG_H__
#define __HTTPLOG_H__

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

#endif // __HTTPLOG_H__
