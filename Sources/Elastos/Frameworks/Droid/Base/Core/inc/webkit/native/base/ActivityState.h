
#ifndef __ACTIVITYSTATE_H__
#define __ACTIVITYSTATE_H__

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * A set of states that represent the last state change of an Activity.
 */
class ActivityState
{
public:
    ActivityState()
        : CREATED(1)
        , STARTED(2)
        , RESUMED(3)
        , PAUSED(4)
        , STOPPED(5)
        , DESTROYED(6)
    {
    }

    /**
     * Represents Activity#onCreate().
     */
    const Int32 CREATED;

    /**
     * Represents Activity#onStart().
     */
    const Int32 STARTED;

    /**
     * Represents Activity#onResume().
     */
    const Int32 RESUMED;

    /**
     * Represents Activity#onPause().
     */
    const Int32 PAUSED;

    /**
     * Represents Activity#onStop().
     */
    const Int32 STOPPED;

    /**
     * Represents Activity#onDestroy().  This is also used when the state of an Activity is unknown.
     */
    const Int32 DESTROYED;
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ACTIVITYSTATE_H__
