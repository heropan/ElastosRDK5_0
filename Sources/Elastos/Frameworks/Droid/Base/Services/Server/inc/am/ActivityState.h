
#ifndef __ACTIVITYSTATE_H__
#define __ACTIVITYSTATE_H__

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {


typedef enum ActivityState
{
    ActivityState_INITIALIZING,
    ActivityState_RESUMED,
    ActivityState_PAUSING,
    ActivityState_PAUSED,
    ActivityState_STOPPING,
    ActivityState_STOPPED,
    ActivityState_FINISHING,
    ActivityState_DESTROYING,
    ActivityState_DESTROYED
} ActivityState;

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ACTIVITYSTATE_H__
