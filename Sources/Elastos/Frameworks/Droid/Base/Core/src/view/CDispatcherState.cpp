
#include "view/CDispatcherState.h"
#include "view/CKeyEvent.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

CDispatcherState::CDispatcherState()
{
}

CDispatcherState::~CDispatcherState()
{
}

ECode CDispatcherState::Reset()
{
    //if (DEBUG)
    //Logger::D(CKeyEvent::TAG, "Reset: " + this);

    mDownKeyCode = 0;
    mDownTarget = NULL;
    mActiveLongPresses.Clear();

    return NOERROR;
}

ECode CDispatcherState::ResetEx(
    /* [in] */ IInterface* target)
{
    if (mDownTarget.Get() == target) {
        //if (DEBUG)
        //Logger::D(TAG, "Reset in " + target + ": " + this);
        mDownKeyCode = 0;
        mDownTarget = NULL;
    }

    return NOERROR;
}

ECode CDispatcherState::StartTracking(
    /* [in] */ IKeyEvent* event,
    /* [in] */ IInterface* target)
{
    Int32 action;
    event->GetAction(&action);
    if (action != IKeyEvent::ACTION_DOWN) {
        Logger::E(
            CKeyEvent::TAG, "Can only start tracking on a down event");

        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //if (DEBUG)
    //Logger::D(CKeyEvent::TAG, "Start trackingt in " + target + ": " + this);
    event->GetKeyCode(&mDownKeyCode);
    mDownTarget = target;

    return NOERROR;
}

ECode CDispatcherState::IsTracking(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isTracking)
{
    if (isTracking == NULL) {
        return E_INVALID_ARGUMENT;
    }

    assert(event);

    Int32 keyCode;
    event->GetKeyCode(&keyCode);

    *isTracking = mDownKeyCode == keyCode;

    return NOERROR;
}

ECode CDispatcherState::PerformedLongPress(
    /* [in] */ IKeyEvent* event)
{
    assert(event);

    Int32 keyCode;
    event->GetKeyCode(&keyCode);

    mActiveLongPresses[keyCode] = 1;

    return NOERROR;
}

ECode CDispatcherState::HandleUpEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);

    //if (DEBUG)
    //    Logger::D(CKeyEvent::TAG, "Handle key up " + event + ": " + this);

    HashMap<Int32, Int32>::Iterator find = mActiveLongPresses.Find(keyCode);
    if (find != mActiveLongPresses.End()) {
        //if (DEBUG)
        //Log.v(TAG, "  Index: " + index);

        ((CKeyEvent*)event)->mFlags |=
            IKeyEvent::FLAG_CANCELED | IKeyEvent::FLAG_CANCELED_LONG_PRESS;
        mActiveLongPresses.Erase(find);
    }

    if (mDownKeyCode == keyCode) {
        if (CKeyEvent::DEBUG)
            Logger::D(CKeyEvent::TAG, "  Tracking!");

        ((CKeyEvent*)event)->mFlags |= IKeyEvent::FLAG_TRACKING;
        mDownKeyCode = 0;
        mDownTarget = NULL;
    }

    return NOERROR;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
