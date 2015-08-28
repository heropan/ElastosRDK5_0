
#ifndef __ELASTOS_DROID_WEBKIT_BASE_APPLICATIONSTATE_H__
#define __ELASTOS_DROID_WEBKIT_BASE_APPLICATIONSTATE_H__

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

class ApplicationState
{
public:
    const Int32 HAS_RUNNING_ACTIVITIES = 1;
    const Int32 HAS_PAUSED_ACTIVITIES = 2;
    const Int32 HAS_STOPPED_ACTIVITIES = 3;
    const Int32 HAS_DESTROYED_ACTIVITIES = 4;
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_BASE_APPLICATIONSTATE_H__
