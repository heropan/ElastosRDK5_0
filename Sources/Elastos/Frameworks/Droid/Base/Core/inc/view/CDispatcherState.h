
#ifndef __ELASTOS_DROID_VIEW_CDISPATCHERSTATE_H__
#define __ELASTOS_DROID_VIEW_CDISPATCHERSTATE_H__

#include "_Elastos_Droid_View_CDispatcherState.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace View {

/**
* Use with {@link KeyEvent#dispatch(Callback, DispatcherState, Object)}
* for more advanced key dispatching, such as Int64 presses.
*/
CarClass(CDispatcherState)
{
public:
    CDispatcherState();

    ~CDispatcherState();

    /**
     * Reset back to initial state.
     */
    CARAPI Reset();

    /**
     * Stop any tracking associated with this target.
     */
    CARAPI Reset(
        /* [in] */ IInterface* target);

    /**
     * Start tracking the key code associated with the given event.  This
     * can only be called on a key down.  It will allow you to see any
     * long press associated with the key, and will result in
     * {@link KeyEvent#isTracking} return true on the long press and up
     * events.
     *
     * <p>This is only needed if you are directly dispatching events, rather
     * than handling them in {@link Callback#onKeyDown}.
     */
    CARAPI StartTracking(
        /* [in] */ IKeyEvent* event,
        /* [in] */ IInterface* target);

    /**
     * Return true if the key event is for a key code that is currently
     * being tracked by the dispatcher.
     */
    CARAPI IsTracking(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isTracking);

    /**
     * Keep track of the given event's key code as having performed an
     * action with a long press, so no action should occur on the up.
     * <p>This is only needed if you are directly dispatching events, rather
     * than handling them in {@link Callback#onKeyLongPress}.
     */
    CARAPI PerformedLongPress(
        /* [in] */ IKeyEvent* event);

    /**
     * Handle key up event to stop tracking.  This resets the dispatcher state,
     * and updates the key event state based on it.
     * <p>This is only needed if you are directly dispatching events, rather
     * than handling them in {@link Callback#onKeyUp}.
     */
    CARAPI HandleUpEvent(
        /* [in] */ IKeyEvent* event);

private:
    Int32 mDownKeyCode;
    AutoPtr<IInterface> mDownTarget;
    HashMap<Int32, Int32> mActiveLongPresses;
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CDISPATCHERSTATE_H__
