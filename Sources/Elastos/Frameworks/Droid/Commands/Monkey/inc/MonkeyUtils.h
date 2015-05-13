#ifndef __MONKEYUTILS_H__
#define __MONKEYUTILS_H__

#include "ext/frameworkext.h"
#include <elastos/Mutex.h>

using Elastos::Utility::IDate;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Droid {
namespace Commands {
namespace Monkey {

class MonkeyUtils {
public:
    static void ToCalendarTime(
            /* [in] */ Int64 time,
            /* [out] */ String *formatedTime);

    static AutoPtr<IDate> InitStatics();

private:
    static Mutex mLockLock;
    MonkeyUtils();
};

} // namespace Monkey
} // namespace Commands
} // namespace Droid
} // namespace Elastos

#endif // __MONKEYUTILS_H__