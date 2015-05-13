
#ifndef __TIMER_H__
#define __TIMER_H__

#include "elastos.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * {@hide}
 * Debugging tool
 */
class Timer
{
public:
    Timer();

    ~Timer();

    CARAPI Mark(
        /* [in] */ const String& message);

private:
    Int64 mStart;

    Int64 mLast;
};

}
}
}
}

#endif // __TIMER_H__
