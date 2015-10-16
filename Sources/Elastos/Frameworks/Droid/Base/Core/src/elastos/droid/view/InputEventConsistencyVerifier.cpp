
#include "elastos/droid/view/InputEventConsistencyVerifier.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/CKeyEvent.h"
#include "elastos/droid/view/CMotionEvent.h"
#include "elastos/droid/os/Build.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::Build;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 InputEventConsistencyVerifier::FLAG_RAW_DEVICE_INPUT;

const Boolean InputEventConsistencyVerifier::IS_ENG_BUILD = String("eng").Equals(Os::Build::TYPE);

const String InputEventConsistencyVerifier::EVENT_TYPE_KEY("KeyEvent");
const String InputEventConsistencyVerifier::EVENT_TYPE_TRACKBALL("TrackballEvent");
const String InputEventConsistencyVerifier::EVENT_TYPE_TOUCH("TouchEvent");
const String InputEventConsistencyVerifier::EVENT_TYPE_GENERIC_MOTION("GenericMotionEvent");
const Int32 InputEventConsistencyVerifier::RECENT_EVENTS_TO_LOG;

Mutex InputEventConsistencyVerifier::KeyState::mRecycledListLock;
AutoPtr<InputEventConsistencyVerifier::KeyState> InputEventConsistencyVerifier::KeyState::mRecycledList;

AutoPtr<InputEventConsistencyVerifier::KeyState> InputEventConsistencyVerifier::KeyState::Obtain(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 source,
    /* [in] */ Int32 keyCode)
{
    AutoPtr<KeyState> state;
    {
        AutoLock lock(mRecycledListLock);
        state = mRecycledList;
        if (state != NULL) {
            mRecycledList = state->mNext;
        }
        else {
            state = new KeyState();
        }
    }
    state->mDeviceId = deviceId;
    state->mSource = source;
    state->mKeyCode = keyCode;
    state->mUnhandled = FALSE;
    return state;
}

void InputEventConsistencyVerifier::KeyState::Recycle()
{
    AutoLock lock(mRecycledListLock);
    mNext = mRecycledList;
    mRecycledList = mNext;
}

InputEventConsistencyVerifier::InputEventConsistencyVerifier(
    /* [in] */ IInterface* caller,
    /* [in] */ Int32 flags,
    /* [in] */ const String& logTag/* = NULL*/)
    : mCaller(caller)
    , mFlags(flags)
    , mLastEventSeq(0)
    , mLastNestingLevel(0)
    , mMostRecentEventIndex(0)
    , mTrackballDown(FALSE)
    , mTrackballUnhandled(FALSE)
    , mTouchEventStreamPointers(0)
    , mTouchEventStreamDeviceId(-1)
    , mTouchEventStreamSource(0)
    , mTouchEventStreamIsTainted(FALSE)
    , mTouchEventStreamUnhandled(FALSE)
    , mHoverEntered(FALSE)
{
    mLogTag = (logTag.IsNull()) ? String("InputEventConsistencyVerifier") : logTag;
}

Boolean InputEventConsistencyVerifier::IsInstrumentationEnabled()
{
    return IS_ENG_BUILD;
}

void InputEventConsistencyVerifier::Reset()
{
    mLastEventSeq = -1;
    mLastNestingLevel = 0;
    mTrackballDown = FALSE;
    mTrackballUnhandled = FALSE;
    mTouchEventStreamPointers = 0;
    mTouchEventStreamIsTainted = FALSE;
    mTouchEventStreamUnhandled = FALSE;
    mHoverEntered = FALSE;

    while (mKeyStateList != NULL) {
        AutoPtr<KeyState> state = mKeyStateList;
        mKeyStateList = state->mNext;
        state->Recycle();
    }
}

void InputEventConsistencyVerifier::OnInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 nestingLevel)
{
    if (IKeyEvent::Probe(event)) {
        OnKeyEvent(IKeyEvent::Probe(event), nestingLevel);
    }
    else {
        AutoPtr<IMotionEvent> motionEvent = IMotionEvent::Probe(event);
        Boolean isTouchEvent;
        motionEvent->IsTouchEvent(&isTouchEvent);
        Int32 source;
        if (isTouchEvent) {
            OnTouchEvent(motionEvent, nestingLevel);
        }
        else if (((motionEvent->GetSource(&source), source)
            & IInputDevice::SOURCE_CLASS_TRACKBALL) != 0) {
            OnTrackballEvent(motionEvent, nestingLevel);
        }
        else {
            OnGenericMotionEvent(motionEvent, nestingLevel);
        }
    }
}

void InputEventConsistencyVerifier::OnKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 nestingLevel)
{
    if (!StartEvent(event, nestingLevel, EVENT_TYPE_KEY)) {
        return;
    }

    //try {
        Int32 metaState;
        event->GetMetaState(&metaState);
        EnsureMetaStateIsNormalized(metaState);

        Int32 action;
        event->GetAction(&action);
        Int32 deviceId;
        event->GetDeviceId(&deviceId);
        Int32 source;
        event->GetSource(&source);
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        switch (action) {
            case IKeyEvent::ACTION_DOWN: {
                AutoPtr<KeyState> state =
                    FindKeyState(deviceId, source, keyCode, /*remove*/ FALSE);
                Int32 repeatCount;
                if (state != NULL) {
                    // If the key is already down, ensure it is a repeat.
                    // We don't perform this check when processing raw device input
                    // because the input dispatcher itself is responsible for setting
                    // the key repeat count before it delivers input events.
                    if (state->mUnhandled) {
                        state->mUnhandled = FALSE;
                    }
                    else if ((mFlags & FLAG_RAW_DEVICE_INPUT) == 0
                        && (event->GetRepeatCount(&repeatCount), repeatCount) == 0) {
                        Problem(String("ACTION_DOWN but key is already down and this event ")
                            + "is not a key repeat.");
                    }
                }
                else {
                    AddKeyState(deviceId, source, keyCode);
                }
                break;
            }
            case IKeyEvent::ACTION_UP: {
                AutoPtr<KeyState> state =
                    FindKeyState(deviceId, source, keyCode, /*remove*/ TRUE);
                if (state == NULL) {
                    Problem(String("ACTION_UP but key was not down."));
                }
                else {
                    state->Recycle();
                }
                break;
            }
            case IKeyEvent::ACTION_MULTIPLE:
                break;
            default:
                Problem(String("Invalid action ") + CKeyEvent::ActionToString(action)
                    + " for key event.");
                break;
        }
    //} finally {
        FinishEvent();
    //}
}

void InputEventConsistencyVerifier::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 nestingLevel)
{
    if (!StartEvent(event, nestingLevel, EVENT_TYPE_TRACKBALL)) {
        return;
    }

    //try {
        Int32 metaState;
        event->GetMetaState(&metaState);
        EnsureMetaStateIsNormalized(metaState);

        Int32 action;
        event->GetAction(&action);
        Int32 source;
        event->GetSource(&source);
        if ((source & IInputDevice::SOURCE_CLASS_TRACKBALL) != 0) {
            switch (action) {
                case IMotionEvent::ACTION_DOWN:
                    if (mTrackballDown && !mTrackballUnhandled) {
                        Problem(String("ACTION_DOWN but trackball is already down."));
                    }
                    else {
                        mTrackballDown = TRUE;
                        mTrackballUnhandled = FALSE;
                    }
                    EnsureHistorySizeIsZeroForThisAction(event);
                    EnsurePointerCountIsOneForThisAction(event);
                    break;
                case IMotionEvent::ACTION_UP:
                    if (!mTrackballDown) {
                        Problem(String("ACTION_UP but trackball is not down."));
                    }
                    else {
                        mTrackballDown = FALSE;
                        mTrackballUnhandled = FALSE;
                    }
                    EnsureHistorySizeIsZeroForThisAction(event);
                    EnsurePointerCountIsOneForThisAction(event);
                    break;
                case IMotionEvent::ACTION_MOVE:
                    EnsurePointerCountIsOneForThisAction(event);
                    break;
                default:
                    Problem(String("Invalid action ") + CMotionEvent::ActionToString(action)
                            + " for trackball event.");
                    break;
            }

            Float pressure;
            event->GetPressure(&pressure);
            if (mTrackballDown && pressure <= 0) {
                Problem(String("Trackball is down but pressure is not greater than 0."));
            }
            else if (!mTrackballDown && pressure != 0) {
                Problem(String("Trackball is up but pressure is not equal to 0."));
            }
        }
        else {
            Problem(String("Source was not SOURCE_CLASS_TRACKBALL."));
        }
    //} finally {
        FinishEvent();
    //}
}

void InputEventConsistencyVerifier::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 nestingLevel)
{
    if (!StartEvent(event, nestingLevel, EVENT_TYPE_TOUCH)) {
        return;
    }

    Int32 action;
    event->GetAction(&action);
    Boolean newStream = action == IMotionEvent::ACTION_DOWN
        || action == IMotionEvent::ACTION_CANCEL || action == IMotionEvent::ACTION_OUTSIDE;
    if (newStream && (mTouchEventStreamIsTainted || mTouchEventStreamUnhandled)) {
        mTouchEventStreamIsTainted = FALSE;
        mTouchEventStreamUnhandled = FALSE;
        mTouchEventStreamPointers = 0;
    }
    if (mTouchEventStreamIsTainted) {
        event->SetTainted(TRUE);
    }

    //try {
        Int32 metaState;
        event->GetMetaState(&metaState);
        EnsureMetaStateIsNormalized(metaState);

        Int32 deviceId;
        event->GetDeviceId(&deviceId);
        Int32 source;
        event->GetSource(&source);

        if (!newStream && mTouchEventStreamDeviceId != -1
            && (mTouchEventStreamDeviceId != deviceId
            || mTouchEventStreamSource != source)) {
            Problem(String("Touch event stream contains events from multiple sources: ")
                + "previous device id " + StringUtils::Int32ToString(mTouchEventStreamDeviceId)
                + ", previous source " + StringUtils::Int32ToString(mTouchEventStreamSource, 16)
                + ", new device id " + StringUtils::Int32ToString(deviceId)
                + ", new source " + StringUtils::Int32ToString(source, 16));
        }
        mTouchEventStreamDeviceId = deviceId;
        mTouchEventStreamSource = source;

        Int32 pointerCount;
        event->GetPointerCount(&pointerCount);
        if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
            switch (action) {
                case IMotionEvent::ACTION_DOWN:
                    if (mTouchEventStreamPointers != 0) {
                        Problem(String("ACTION_DOWN but pointers are already down.  ")
                            + "Probably missing ACTION_UP from previous gesture.");
                    }
                    EnsureHistorySizeIsZeroForThisAction(event);
                    EnsurePointerCountIsOneForThisAction(event);
                    Int32 pointerId;
                    event->GetPointerId(0, &pointerId);
                    mTouchEventStreamPointers = 1 << pointerId;
                    break;
                case IMotionEvent::ACTION_UP:
                    EnsureHistorySizeIsZeroForThisAction(event);
                    EnsurePointerCountIsOneForThisAction(event);
                    mTouchEventStreamPointers = 0;
                    mTouchEventStreamIsTainted = FALSE;
                    break;
                case IMotionEvent::ACTION_MOVE:
                {
                    Int32 expectedPointerCount =
                        Elastos::Core::Math::BitCount(mTouchEventStreamPointers);
                    if (pointerCount != expectedPointerCount) {
                        Problem(String("ACTION_MOVE contained ")
                            + StringUtils::Int32ToString(pointerCount)
                            + " pointers but there are currently "
                            + StringUtils::Int32ToString(expectedPointerCount)
                            + " pointers down.");
                        mTouchEventStreamIsTainted = TRUE;
                    }
                    break;
                }
                case IMotionEvent::ACTION_CANCEL:
                    mTouchEventStreamPointers = 0;
                    mTouchEventStreamIsTainted = FALSE;
                    break;
                case IMotionEvent::ACTION_OUTSIDE:
                    if (mTouchEventStreamPointers != 0) {
                        Problem(String("ACTION_OUTSIDE but pointers are still down."));
                    }
                    EnsureHistorySizeIsZeroForThisAction(event);
                    EnsurePointerCountIsOneForThisAction(event);
                    mTouchEventStreamIsTainted = FALSE;
                    break;
                default: {
                    Int32 actionMasked;
                    event->GetActionMasked(&actionMasked);
                    Int32 actionIndex;
                    event->GetActionIndex(&actionIndex);
                    if (actionMasked == IMotionEvent::ACTION_POINTER_DOWN) {
                        if (mTouchEventStreamPointers == 0) {
                            Problem(String("ACTION_POINTER_DOWN but no other pointers were down."));
                            mTouchEventStreamIsTainted = TRUE;
                        }
                        if (actionIndex < 0 || actionIndex >= pointerCount) {
                            Problem(String("ACTION_POINTER_DOWN index is ")
                                + StringUtils::Int32ToString(actionIndex)
                                + " but the pointer count is "
                                + StringUtils::Int32ToString(pointerCount) + ".");
                            mTouchEventStreamIsTainted = TRUE;
                        }
                        else {
                            Int32 id;
                            event->GetPointerId(actionIndex, &id);
                            Int32 idBit = 1 << id;
                            if ((mTouchEventStreamPointers & idBit) != 0) {
                                Problem(String("ACTION_POINTER_DOWN specified pointer id ")
                                    + StringUtils::Int32ToString(id) + " which is already down.");
                                mTouchEventStreamIsTainted = TRUE;
                            }
                            else {
                                mTouchEventStreamPointers |= idBit;
                            }
                        }
                        EnsureHistorySizeIsZeroForThisAction(event);
                    }
                    else if (actionMasked == IMotionEvent::ACTION_POINTER_UP) {
                        if (actionIndex < 0 || actionIndex >= pointerCount) {
                            Problem(String("ACTION_POINTER_UP index is ")
                                + StringUtils::Int32ToString(actionIndex)
                                + " but the pointer count is "
                                + StringUtils::Int32ToString(pointerCount) + ".");
                            mTouchEventStreamIsTainted = TRUE;
                        }
                        else {
                            Int32 id;
                            event->GetPointerId(actionIndex, &id);
                            Int32 idBit = 1 << id;
                            if ((mTouchEventStreamPointers & idBit) == 0) {
                                Problem(String("ACTION_POINTER_UP specified pointer id ")
                                 + StringUtils::Int32ToString(id) + " which is not currently down.");
                                mTouchEventStreamIsTainted = TRUE;
                            }
                            else {
                                mTouchEventStreamPointers &= ~idBit;
                            }
                        }
                        EnsureHistorySizeIsZeroForThisAction(event);
                    }
                    else {
                        Problem(String("Invalid action ") + CMotionEvent::ActionToString(action)
                            + " for touch event.");
                    }
                    break;
                }
            }
        }
        else {
            Problem(String("Source was not SOURCE_CLASS_POINTER."));
        }
    //} finally {
        FinishEvent();
    //}
}

void InputEventConsistencyVerifier::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 nestingLevel)
{
    if (!StartEvent(event, nestingLevel, EVENT_TYPE_GENERIC_MOTION)) {
        return;
    }

    //try {
        Int32 metaState;
        event->GetMetaState(&metaState);
        EnsureMetaStateIsNormalized(metaState);

        Int32 action;
        event->GetAction(&action);
        Int32 source;
        event->GetSource(&source);
        if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
            switch (action) {
                case IMotionEvent::ACTION_HOVER_ENTER:
                    EnsurePointerCountIsOneForThisAction(event);
                    mHoverEntered = TRUE;
                    break;
                case IMotionEvent::ACTION_HOVER_MOVE:
                    EnsurePointerCountIsOneForThisAction(event);
                    break;
                case IMotionEvent::ACTION_HOVER_EXIT:
                    EnsurePointerCountIsOneForThisAction(event);
                    if (!mHoverEntered) {
                        Problem(String("ACTION_HOVER_EXIT without prior ACTION_HOVER_ENTER"));
                    }
                    mHoverEntered = FALSE;
                    break;
                case IMotionEvent::ACTION_SCROLL:
                    EnsureHistorySizeIsZeroForThisAction(event);
                    EnsurePointerCountIsOneForThisAction(event);
                    break;
                default:
                    Problem(String("Invalid action for generic pointer event."));
                    break;
            }
        }
        else if ((source & IInputDevice::SOURCE_CLASS_JOYSTICK) != 0) {
            switch (action) {
                case IMotionEvent::ACTION_MOVE:
                    EnsurePointerCountIsOneForThisAction(event);
                    break;
                default:
                    Problem(String("Invalid action for generic joystick event."));
                    break;
            }
        }
    //} finally {
        FinishEvent();
    //}
}

void InputEventConsistencyVerifier::OnUnhandledEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 nestingLevel)
{
    if (nestingLevel != mLastNestingLevel) {
        return;
    }

    if (mRecentEventsUnhandled != NULL) {
        (*mRecentEventsUnhandled)[mMostRecentEventIndex] = TRUE;
    }

    if (IKeyEvent::Probe(event)) {
        AutoPtr<IKeyEvent> keyEvent = IKeyEvent::Probe(event);
        Int32 deviceId;
        keyEvent->GetDeviceId(&deviceId);
        Int32 source;
        keyEvent->GetSource(&source);
        Int32 keyCode;
        keyEvent->GetKeyCode(&keyCode);
        AutoPtr<KeyState> state = FindKeyState(deviceId, source, keyCode, /*remove*/ FALSE);
        if (state != NULL) {
            state->mUnhandled = TRUE;
        }
    }
    else {
        AutoPtr<IMotionEvent> motionEvent = IMotionEvent::Probe(event);
        Boolean isTouchEvent;
        motionEvent->IsTouchEvent(&isTouchEvent);
        Int32 source;
        if (isTouchEvent) {
            mTouchEventStreamUnhandled = TRUE;
        }
        else if (((motionEvent->GetSource(&source), source)
            & IInputDevice::SOURCE_CLASS_TRACKBALL) != 0) {
            if (mTrackballDown) {
                mTrackballUnhandled = TRUE;
            }
        }
    }
}

void InputEventConsistencyVerifier::EnsureMetaStateIsNormalized(
    /* [in] */ Int32 metaState)
{
    Int32 normalizedMetaState = CKeyEvent::NormalizeMetaState(metaState);
    if (normalizedMetaState != metaState) {
        char str[80];
        sprintf(str, "Metastate not normalized.  Was 0x%08x but expected 0x%08x.",
            metaState, normalizedMetaState);

        Problem(String(str, strlen(str)));
    }
}

void InputEventConsistencyVerifier::EnsurePointerCountIsOneForThisAction(
    /* [in] */ IMotionEvent* event)
{
    Int32 pointerCount;
    event->GetPointerCount(&pointerCount);
    if (pointerCount != 1) {
        Int32 action;
        event->GetAction(&action);
        Problem(String("Pointer count is ")
            + StringUtils::Int32ToString(pointerCount) + " but it should always be 1 for "
            + CMotionEvent::ActionToString(action));
    }
}

void InputEventConsistencyVerifier::EnsureHistorySizeIsZeroForThisAction(
    /* [in] */ IMotionEvent* event)
{
    Int32 historySize;
    event->GetHistorySize(&historySize);
    if (historySize != 0) {
        Int32 action;
        event->GetAction(&action);
        Problem(String("History size is ")
            + StringUtils::Int32ToString(historySize) + " but it should always be 0 for "
            + CMotionEvent::ActionToString(action));
    }
}

Boolean InputEventConsistencyVerifier::StartEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 nestingLevel,
    /* [in] */ const String& eventType)
{
    // Ignore the event if we already checked it at a higher nesting level.
    Int32 seq;
    event->GetSequenceNumber(&seq);
    if (seq == mLastEventSeq && nestingLevel < mLastNestingLevel
            && eventType == mLastEventType) {
        return FALSE;
    }

    if (nestingLevel > 0) {
        mLastEventSeq = seq;
        mLastEventType = eventType;
        mLastNestingLevel = nestingLevel;
    }
    else {
        mLastEventSeq = -1;
        mLastEventType = NULL;
        mLastNestingLevel = 0;
    }

    mCurrentEvent = event;
    mCurrentEventType = eventType;
    return TRUE;
}

void InputEventConsistencyVerifier::FinishEvent()
{
    if (mViolationMessage != NULL && mViolationMessage->GetLength() != 0) {
        Boolean isTainted;
        mCurrentEvent->IsTainted(&isTainted);
        if (!isTainted) {
            // Write a log message only if the event was not already tainted.
            mViolationMessage->AppendCStr("\n  in ");
            //mViolationMessage->Append(mCaller);
            mViolationMessage->AppendCStr("\n  ");
            AppendEvent(mViolationMessage, 0, mCurrentEvent, FALSE);

            if (RECENT_EVENTS_TO_LOG != 0 && mRecentEvents != NULL) {
                mViolationMessage->AppendCStr("\n  -- recent events --");
                for (Int32 i = 0; i < RECENT_EVENTS_TO_LOG; i++) {
                    Int32 index = (mMostRecentEventIndex + RECENT_EVENTS_TO_LOG - i)
                            % RECENT_EVENTS_TO_LOG;
                    IInputEvent* event = (*mRecentEvents)[index];
                    if (event == NULL) {
                        break;
                    }
                    mViolationMessage->AppendCStr("\n  ");
                    AppendEvent(mViolationMessage, i + 1, event, (*mRecentEventsUnhandled)[index]);
                }
            }

            Logger::D(mLogTag, mViolationMessage->ToString());

            // Taint the event so that we do not generate additional violations from it
            // further downstream.
            mCurrentEvent->SetTainted(TRUE);
        }
        mViolationMessage->Reset();
    }

    if (RECENT_EVENTS_TO_LOG != 0) {
        if (mRecentEvents == NULL) {
            mRecentEvents = ArrayOf<IInputEvent*>::Alloc(RECENT_EVENTS_TO_LOG);
            mRecentEventsUnhandled = ArrayOf<Boolean>::Alloc(RECENT_EVENTS_TO_LOG);
        }
        Int32 index = (mMostRecentEventIndex + 1) % RECENT_EVENTS_TO_LOG;
        mMostRecentEventIndex = index;
        if ((*mRecentEvents)[index] != NULL) {
            (*mRecentEvents)[index]->Recycle();
        }
        AutoPtr<IInputEvent> copyEvent;
        mCurrentEvent->Copy((IInputEvent**)&copyEvent);
        mRecentEvents->Set(index, copyEvent);
        (*mRecentEventsUnhandled)[index] = FALSE;
    }

    mCurrentEvent = NULL;
    mCurrentEventType = NULL;
}

void InputEventConsistencyVerifier::AppendEvent(
    /* [in] */ StringBuilder* message,
    /* [in] */ Int32 index,
    /* [in] */ IInputEvent* event,
    /* [in] */ Boolean unhandled)
{
    message->AppendInt32(index);
    message->AppendCStr(": sent at ");
    Int64 timeNano;
    event->GetEventTimeNano(&timeNano);
    message->AppendInt64(timeNano);
    message->AppendCStr(", ");
    if (unhandled) {
        message->AppendCStr("(unhandled) ");
    }
    if (IKeyEvent::Probe(event))
        message->AppendString(((CKeyEvent*)event)->ToString());
    else
        message->AppendString(((CMotionEvent*)event)->ToString());
}

void InputEventConsistencyVerifier::Problem(
    /* [in] */ const String& message)
{
    if (mViolationMessage == NULL) {
        mViolationMessage = new StringBuilder();
    }
    if (mViolationMessage->GetLength() == 0) {
        mViolationMessage->AppendString(mCurrentEventType);
        mViolationMessage->AppendCStr(": ");
    }
    else {
        mViolationMessage->AppendCStr("\n  ");
    }
    mViolationMessage->AppendString(message);
}

AutoPtr<InputEventConsistencyVerifier::KeyState> InputEventConsistencyVerifier::FindKeyState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 source,
    /* [in] */ Int32 keyCode,
    /* [in] */ Boolean remove)
{
    AutoPtr<KeyState> last;
    AutoPtr<KeyState> state = mKeyStateList;
    while (state != NULL) {
        if (state->mDeviceId == deviceId && state->mSource == source
            && state->mKeyCode == keyCode) {
            if (remove) {
                if (last != NULL) {
                    last->mNext = state->mNext;
                }
                else {
                    mKeyStateList = state->mNext;
                }
                state->mNext = NULL;
            }
            return state;
        }
        last = state;
        state = state->mNext;
    }
    return NULL;
}

void InputEventConsistencyVerifier::AddKeyState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 source,
    /* [in] */ Int32 keyCode)
{
    AutoPtr<KeyState> state = KeyState::Obtain(deviceId, source, keyCode);
    state->mNext = mKeyStateList;
    mKeyStateList = state;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
