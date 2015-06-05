
#ifndef __EVENTLOG_H__
#define __EVENTLOG_H__

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * A simple interface to Android's EventLog to be used by native code.
 */
//@JNINamespace("base::android")
class EventLog
{
public:
    //@CalledByNative
    static CARAPI_(void) WriteEvent(
        /* [in] */ Int32 tag,
        /* [in] */ Int32 value);
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__EVENTLOG_H__
