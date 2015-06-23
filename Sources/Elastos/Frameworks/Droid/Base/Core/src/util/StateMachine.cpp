
#include "ext/frameworkdef.h"
#include "util/StateMachine.h"
#ifdef DROID_CORE
#include "os/CHandlerThread.h"
#endif
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::EIID_IHandler;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

const String StateMachine::TAG("StateMachine");

//====================================================
// StateMachine::LogRec
//====================================================
StateMachine::LogRec::LogRec(
    /* [in] */ IMessage* msg,
    /* [in] */ const String& info,
    /* [in] */ State* state,
    /* [in] */ State* orgState)
    : mTime(0)
    , mWhat(0)
{
    Update(msg, info, state, orgState);
}

void StateMachine::LogRec::Update(
    /* [in] */ IMessage* msg,
    /* [in] */ const String& info,
    /* [in] */ State* state,
    /* [in] */ State* orgState)
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&mTime);
    if (msg != NULL) msg->GetWhat(&mWhat);
    mInfo = info;
    mState = state;
    mOrgState = orgState;
}


//====================================================
// StateMachine::LogRecords
//====================================================
void StateMachine::LogRecords::SetSize(
    /* [in] */ Int32 maxSize)
{
    Mutex::Autolock lock(mLock);

    mMaxSize = maxSize;
    mCount = 0;
    mLogRecords.Clear();
}

Int32 StateMachine::LogRecords::GetSize()
{
    Mutex::Autolock lock(mLock);
    return mLogRecords.GetSize();
}

Int32 StateMachine::LogRecords::Count()
{
    Mutex::Autolock lock(mLock);
    return mCount;
}

void StateMachine::LogRecords::Cleanup()
{
    Mutex::Autolock lock(mLock);
    mLogRecords.Clear();
}

AutoPtr<StateMachine::LogRec> StateMachine::LogRecords::Get(
    /* [in] */ Int32 index)
{
    Mutex::Autolock lock(mLock);
    Int32 nextIndex = mOldestIndex + index;
    if (nextIndex >= mMaxSize) {
        nextIndex -= mMaxSize;
    }
    if (nextIndex >= GetSize()) {
        return NULL;
    }
    else {
        return mLogRecords[nextIndex];
    }
}

void StateMachine::LogRecords::Add(
    /* [in] */ IMessage* msg,
    /* [in] */ const String& messageInfo,
    /* [in] */ State* state,
    /* [in] */ State* orgState)
{
    Mutex::Autolock lock(mLock);
    mCount += 1;
    if (mLogRecords.GetSize() < mMaxSize) {
        AutoPtr<LogRec> pmi = new LogRec(msg, messageInfo, state, orgState);
        mLogRecords.PushBack(pmi);
    }
    else {
        AutoPtr<LogRec> pmi = mLogRecords[mOldestIndex];
        mOldestIndex += 1;
        if (mOldestIndex >= mMaxSize) {
            mOldestIndex = 0;
        }
        pmi->Update(msg, messageInfo, state, orgState);
    }
}


//====================================================
// StateMachine::SmHandler
//====================================================

const AutoPtr<IInterface> StateMachine::SmHandler::sSmHandlerObj = new StateMachine::SmHandler::Object();

StateMachine::SmHandler::SmHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ StateMachine* sm)
    : HandlerBase(looper)
    , mDbg(FALSE)
    , mIsConstructionCompleted(FALSE)
    , mLogRecords(new LogRecords())
    , mStateStackTopIndex(-1)
    , mHaltingState(new HaltingState(this))
    , mQuittingState(new QuittingState())
    , mSm(sm)
{
    AutoPtr<StateInfo> stateInfo;
    AddState(mHaltingState, NULL, (StateInfo**)&stateInfo);
    stateInfo = NULL;
    AddState(mQuittingState, NULL, (StateInfo**)&stateInfo);
}

ECode StateMachine::SmHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    // if (mDbg) Log.d(TAG, "handleMessage: E msg.what=" + msg.what);
    /** Save the current message */
    mMsg = msg;

    Int32 what;
    mMsg->GetWhat(&what);

    AutoPtr<IInterface> obj;
    if (mIsConstructionCompleted) {
        /** Normal path */
        ProcessMsg(msg);
    }
    else if (!mIsConstructionCompleted && (what == SM_INIT_CMD) &&
        (mMsg->GetObj((IInterface**)&obj), obj == sSmHandlerObj)) {
        /** Initial one time path. */
        mIsConstructionCompleted = TRUE;
        InvokeEnterMethods(0);
    }
    else {
        // throw new RuntimeException("StateMachine.handleMessage: " +
        //             "The start method not called, received msg: " + msg);
        return E_RUNTIME_EXCEPTION;
    }
    PerformTransitions();

    // if (mDbg) Log.d(TAG, "handleMessage: X");
    return NOERROR;
}

void StateMachine::SmHandler::PerformTransitions()
{
    /**
     * If transitionTo has been called, exit and then enter
     * the appropriate states. We loop on this to allow
     * enter and exit methods to use transitionTo.
     */
    AutoPtr<State> destState;
    while (mDestState != NULL) {
        // if (mDbg) Log.d(TAG, "handleMessage: new destination call exit");

        /**
         * Save mDestState locally and set to null
         * to know if enter/exit use transitionTo.
         */
        destState = mDestState;
        mDestState = NULL;

        /**
         * Determine the states to exit and enter and return the
         * common ancestor state of the enter/exit states. Then
         * invoke the exit methods then the enter methods.
         */
        AutoPtr<StateInfo> commonStateInfo = SetupTempStateStackWithStatesToEnter(destState);
        InvokeExitMethods(commonStateInfo);
        Int32 stateStackEnteringIndex = MoveTempStateStackToStateStack();
        InvokeEnterMethods(stateStackEnteringIndex);


        /**
         * Since we have transitioned to a new state we need to have
         * any deferred messages moved to the front of the message queue
         * so they will be processed before any other messages in the
         * message queue.
         */
        MoveDeferredMessageAtFrontOfQueue();
    }

    /**
     * After processing all transitions check and
     * see if the last transition was to quit or halt.
     */
    if (destState != NULL) {
        if (destState == mQuittingState) {
            /**
             * Call onQuitting to let subclasses cleanup.
             */
            mSm->OnQuitting();
            CleanupAfterQuitting();
        }
        else if (destState == mHaltingState) {
            /**
             * Call onHalting() if we've transitioned to the halting
             * state. All subsequent messages will be processed in
             * in the halting state which invokes haltedProcessMessage(msg);
             */
            mSm->OnHalting();
        }
    }
}

void StateMachine::SmHandler::CleanupAfterQuitting()
{
    if (mSm->mSmThread != NULL) {
        // If we made the thread then quit looper which stops the thread.
        AutoPtr<ILooper> looper;
        GetLooper((ILooper**)&looper);
        looper->Quit();
        mSm->mSmThread = NULL;
    }

    mSm->mSmHandler = NULL;
    mSm = NULL;
    mMsg = NULL;
    mLogRecords->Cleanup();
    mStateStack = NULL;
    mTempStateStack = NULL;
    mStateInfo.Clear();
    mInitialState = NULL;
    mDestState = NULL;
    mDeferredMessages.Clear();
}

void StateMachine::SmHandler::CompleteConstruction()
{
    if (mDbg) Logger::D(TAG, "completeConstruction: E");

    /**
     * Determine the maximum depth of the state hierarchy
     * so we can allocate the state stacks.
     */
    Int32 maxDepth = 0;
    HashMap< AutoPtr<State>, AutoPtr<StateInfo> >::Iterator it;
    for (it = mStateInfo.Begin(); it != mStateInfo.End(); ++it) {
        AutoPtr<StateInfo> si = it->mSecond;
        Int32 depth = 0;
        for (AutoPtr<StateInfo> i = si; i != NULL; depth++) {
            i = i->mParentStateInfo;
        }
        if (maxDepth < depth) {
            maxDepth = depth;
        }
    }
    if (mDbg) Logger::D(TAG, "completeConstruction: maxDepth=%d", maxDepth);

    mStateStack = ArrayOf<StateInfo*>::Alloc(maxDepth);
    mTempStateStack = ArrayOf<StateInfo*>::Alloc(maxDepth);
    SetupInitialStateStack();

    /** Sending SM_INIT_CMD message to invoke enter methods asynchronously */
    AutoPtr<IMessage> msg = mSm->ObtainMessage(SM_INIT_CMD, sSmHandlerObj);
    mSm->SendMessageAtFrontOfQueue(msg);

    if (mDbg) Logger::D(TAG, "completeConstruction: X");
}

void StateMachine::SmHandler::ProcessMsg(
    /* [in] */ IMessage* msg)
{
    AutoPtr<StateInfo> curStateInfo = (*mStateStack)[mStateStackTopIndex];
    if (mDbg) {
        // Log.d(TAG, "processMsg: " + curStateInfo.state.getName());
    }

    if (IsQuit(msg)) {
        TransitionTo(mQuittingState);
    }
    else {
        Boolean result;
        while (curStateInfo->mState->ProcessMessage(msg, &result), !result) {
            /**
             * Not processed
             */
            curStateInfo = curStateInfo->mParentStateInfo;
            if (curStateInfo == NULL) {
                /**
                 * No parents left so it's not handled
                 */
                mSm->UnhandledMessage(msg);
                break;
            }
            if (mDbg) {
                // Log.d(TAG, "processMsg: " + curStateInfo.state.getName());
            }
        }

        /**
         * Record that we processed the message
         */
        if (mSm->RecordLogRec(msg)) {
            if (curStateInfo != NULL) {
                AutoPtr<State> orgState = (*mStateStack)[mStateStackTopIndex]->mState;
                mLogRecords->Add(msg, mSm->GetLogRecString(msg), curStateInfo->mState,
                        orgState);
            }
            else {
                mLogRecords->Add(msg, mSm->GetLogRecString(msg), NULL, NULL);
            }
        }
    }
}

void StateMachine::SmHandler::InvokeExitMethods(
    /* [in] */ StateInfo* commonStateInfo)
{
    while ((mStateStackTopIndex >= 0) &&
            ((*mStateStack)[mStateStackTopIndex] != commonStateInfo)) {
        AutoPtr<State> curState = (*mStateStack)[mStateStackTopIndex]->mState;
        // if (mDbg) Log.d(TAG, "invokeExitMethods: " + curState.getName());
        curState->Exit();
        (*mStateStack)[mStateStackTopIndex]->mActive = FALSE;
        mStateStackTopIndex -= 1;
    }
}

void StateMachine::SmHandler::InvokeEnterMethods(
    /* [in] */ Int32 stateStackEnteringIndex)
{
    for (Int32 i = stateStackEnteringIndex; i <= mStateStackTopIndex; i++) {
        if (mDbg) {
            String name;
            (*mStateStack)[i]->mState->GetName(&name);
            Logger::D(TAG, "invokeEnterMethods: %s", name.string());
        }
        (*mStateStack)[i]->mState->Enter();
        (*mStateStack)[i]->mActive = TRUE;
    }
}

void StateMachine::SmHandler::MoveDeferredMessageAtFrontOfQueue()
{
    /**
     * The oldest messages on the deferred list must be at
     * the front of the queue so start at the back, which
     * as the most resent message and end with the oldest
     * messages at the front of the queue.
     */
    List< AutoPtr<IMessage> >::ReverseIterator rit;
    for (rit = mDeferredMessages.RBegin(); rit != mDeferredMessages.REnd(); ++rit) {
        AutoPtr<IMessage> curMsg = *rit;
        // if (mDbg) Log.d(TAG, "moveDeferredMessageAtFrontOfQueue; what=" + curMsg.what);
        mSm->SendMessageAtFrontOfQueue(curMsg);
    }
    mDeferredMessages.Clear();
}

Int32 StateMachine::SmHandler::MoveTempStateStackToStateStack()
{
    Int32 startingIndex = mStateStackTopIndex + 1;
    Int32 i = mTempStateStackCount - 1;
    Int32 j = startingIndex;
    while (i >= 0) {
        if (mDbg) Logger::D(TAG, "moveTempStackToStateStack: i=%d,j=%d", i, j);
        mStateStack->Set(j, (*mTempStateStack)[i]);
        j += 1;
        i -= 1;
    }

    mStateStackTopIndex = j - 1;
    if (mDbg) {
        // Log.d(TAG, "moveTempStackToStateStack: X mStateStackTop="
        //       + mStateStackTopIndex + ",startingIndex=" + startingIndex
        //       + ",Top=" + mStateStack[mStateStackTopIndex].state.getName());
    }
    return startingIndex;
}

AutoPtr<StateMachine::SmHandler::StateInfo>
StateMachine::SmHandler::SetupTempStateStackWithStatesToEnter(
    /* [in] */ State* destState)
{
    /**
     * Search up the parent list of the destination state for an active
     * state. Use a do while() loop as the destState must always be entered
     * even if it is active. This can happen if we are exiting/entering
     * the current state.
     */
    mTempStateStackCount = 0;
    AutoPtr<StateInfo> curStateInfo;
    HashMap< AutoPtr<State>, AutoPtr<StateInfo> >::Iterator it = mStateInfo.Find(destState);
    if (it != mStateInfo.End()) {
        curStateInfo = it->mSecond;
    }
    do {
        mTempStateStack->Set(mTempStateStackCount++, curStateInfo);
        curStateInfo = curStateInfo->mParentStateInfo;
    } while ((curStateInfo != NULL) && !curStateInfo->mActive);

    if (mDbg) {
        // Log.d(TAG, "setupTempStateStackWithStatesToEnter: X mTempStateStackCount="
        //       + mTempStateStackCount + ",curStateInfo: " + curStateInfo);
    }
    return curStateInfo;
}

void StateMachine::SmHandler::SetupInitialStateStack()
{
    if (mDbg) {
        String name;
        mInitialState->GetName(&name);
        Logger::D(TAG, "setupInitialStateStack: E mInitialState= %s",
            name.string());
    }

    AutoPtr<StateInfo> curStateInfo;
    HashMap< AutoPtr<State>, AutoPtr<StateInfo> >::Iterator it = mStateInfo.Find(mInitialState);
    if (it != mStateInfo.End()) {
        curStateInfo = it->mSecond;
    }
    for (mTempStateStackCount = 0; curStateInfo != NULL; mTempStateStackCount++) {
        mTempStateStack->Set(mTempStateStackCount, curStateInfo);
        curStateInfo = curStateInfo->mParentStateInfo;
    }

    // Empty the StateStack
    mStateStackTopIndex = -1;

    MoveTempStateStackToStateStack();
}

ECode StateMachine::SmHandler::AddState(
    /* [in] */ State* state,
    /* [in] */ State* parent,
    /* [out] */ StateInfo** info)
{
    if (mDbg) {
        // Log.d(TAG, "addStateInternal: E state=" + state.getName()
        //         + ",parent=" + ((parent == null) ? "" : parent.getName()));
    }
    AutoPtr<StateInfo> parentStateInfo;
    if (parent != NULL) {
        HashMap< AutoPtr<State>, AutoPtr<StateInfo> >::Iterator it =
                mStateInfo.Find(parent);
        if (it != mStateInfo.End()) {
            parentStateInfo = it->mSecond;
        }
        if (parentStateInfo == NULL) {
            // Recursively add our parent as it's not been added yet.
            FAIL_RETURN(AddState(parent, NULL, (StateInfo**)&parentStateInfo));
        }
    }
    AutoPtr<StateInfo> stateInfo;
    HashMap< AutoPtr<State>, AutoPtr<StateInfo> >::Iterator it =
            mStateInfo.Find(state);
    if (it != mStateInfo.End()) {
        stateInfo = it->mSecond;
    }
    if (stateInfo == NULL) {
        stateInfo = new StateInfo();
        mStateInfo[state] = stateInfo;
    }

    // Validate that we aren't adding the same state in two different hierarchies.
    if ((stateInfo->mParentStateInfo != NULL) &&
            (stateInfo->mParentStateInfo != parentStateInfo)) {
        // throw new RuntimeException("state already added");
        return E_RUNTIME_EXCEPTION;
    }
    stateInfo->mState = state;
    stateInfo->mParentStateInfo = parentStateInfo;
    stateInfo->mActive = FALSE;
    // if (mDbg) Log.d(TAG, "addStateInternal: X stateInfo: " + stateInfo);
    *info = stateInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

void StateMachine::SmHandler::SetInitialState(
    /* [in] */ State* initialState)
{
    // if (mDbg) Log.d(TAG, "setInitialState: initialState=" + initialState.getName());
    mInitialState = initialState;
}

void StateMachine::SmHandler::TransitionTo(
    /* [in] */ IState* destState)
{
    mDestState = (State*)destState;
    // if (mDbg) Log.d(TAG, "transitionTo: destState=" + mDestState.getName());
}

void StateMachine::SmHandler::DeferMessage(
    /* [in] */ IMessage* msg)
{
    // if (mDbg) Log.d(TAG, "deferMessage: msg=" + msg.what);

    /* Copy the "msg" to "newMsg" as "msg" will be recycled */
    AutoPtr<IMessage> newMsg = mSm->ObtainMessage();
    newMsg->CopyFrom(msg);

    mDeferredMessages.PushBack(newMsg);
}

void StateMachine::SmHandler::Quit()
{
    if (mDbg) Logger::D(TAG, "quit:");
    AutoPtr<IMessage> msg = mSm->ObtainMessage(SM_QUIT_CMD, sSmHandlerObj);
    mSm->SendMessage(msg);
}

void StateMachine::SmHandler::QuitNow()
{
    if (mDbg) Logger::D(TAG, "abort:");
    AutoPtr<IMessage> msg = mSm->ObtainMessage(SM_QUIT_CMD, sSmHandlerObj);
    mSm->SendMessageAtFrontOfQueue(msg);
}

Boolean StateMachine::SmHandler::IsQuit(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    AutoPtr<IInterface> obj;
    msg->GetWhat(&what);
    msg->GetObj((IInterface**)&obj);
    return (what == SM_QUIT_CMD) && (obj == sSmHandlerObj);
}


//====================================================
// StateMachine
//====================================================
StateMachine::StateMachine(
    /* [in] */ const String& name)
{
    CHandlerThread::New(name, (IHandlerThread**)&mSmThread);
    mSmThread->Start();
    AutoPtr<ILooper> looper;
    mSmThread->GetLooper((ILooper**)&looper);

    InitStateMachine(name, looper);
}

StateMachine::StateMachine(
    /* [in] */ const String& name,
    /* [in] */ ILooper* looper)
{
    InitStateMachine(name, looper);
}

void StateMachine::InitStateMachine(
    /* [in] */ const String& name,
    /* [in] */ ILooper* looper)
{
    mName = name;
    mSmHandler = new SmHandler(looper, this);
}

ECode StateMachine::AddState(
    /* [in] */ State* state,
    /* [in] */ State* parent)
{
    AutoPtr<SmHandler::StateInfo> sinfo;
    return mSmHandler->AddState(state, parent, (SmHandler::StateInfo**)&sinfo);
}

ECode StateMachine::AddState(
    /* [in] */ State* state)
{
    AutoPtr<SmHandler::StateInfo> sinfo;
    return mSmHandler->AddState(state, NULL, (SmHandler::StateInfo**)&sinfo);
}

void StateMachine::UnhandledMessage(
    /* [in] */ IMessage* msg)
{
    // if (mSmHandler.mDbg) Log.e(TAG, mName + " - unhandledMessage: msg.what=" + msg.what);
}

AutoPtr<IMessage> StateMachine::ObtainMessage()
{
    return ObtainMessage(0);
}

AutoPtr<IMessage> StateMachine::ObtainMessage(
    /* [in] */ Int32 what)
{
    return ObtainMessage(what, NULL);
}

AutoPtr<IMessage> StateMachine::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    return ObtainMessage(what, 0, 0, obj);
}

AutoPtr<IMessage> StateMachine::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2)
{
    return ObtainMessage(what, arg1, arg2, NULL);
}

AutoPtr<IMessage> StateMachine::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    if (mSmHandler == NULL) return NULL;

    AutoPtr<IMessage> m;
    mSmHandler->ObtainMessage(what, arg1, arg2, obj, (IMessage**)&m);
    return m;
}

void StateMachine::SendMessage(
    /* [in] */ Int32 what)
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    Boolean result;
    mSmHandler->SendEmptyMessage(what, &result);
}

void StateMachine::SendMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    AutoPtr<IMessage> msg;
    mSmHandler->ObtainMessage(what, obj, (IMessage**)&msg);
    Boolean result;
    mSmHandler->SendMessage(msg, &result);
}

void StateMachine::SendMessage(
    /* [in] */ IMessage* msg)
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    Boolean result;
    mSmHandler->SendMessage(msg, &result);
}

void StateMachine::SendMessageDelayed(
    /* [in] */ Int32 what,
    /* [in] */ Int64 delayMillis)
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    Boolean result;
    mSmHandler->SendEmptyMessageDelayed(what, delayMillis, &result);
}

void StateMachine::SendMessageDelayed(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 delayMillis)
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    AutoPtr<IMessage> msg;
    mSmHandler->ObtainMessage(what, obj, (IMessage**)&msg);
    Boolean result;
    mSmHandler->SendMessageDelayed(msg, delayMillis, &result);
}

void StateMachine::SendMessageDelayed(
    /* [in] */ IMessage* msg,
    /* [in] */ Int64 delayMillis)
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    Boolean result;
    mSmHandler->SendMessageDelayed(msg, delayMillis, &result);
}

void StateMachine::SendMessageAtFrontOfQueue(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    AutoPtr<IMessage> msg;
    mSmHandler->ObtainMessage(what, obj, (IMessage**)&msg);
    Boolean result;
    mSmHandler->SendMessageAtFrontOfQueue(msg, &result);
}

void StateMachine::SendMessageAtFrontOfQueue(
    /* [in] */ Int32 what)
{
    AutoPtr<IMessage> msg;
    mSmHandler->ObtainMessage(what, (IMessage**)&msg);
    Boolean result;
    mSmHandler->SendMessageAtFrontOfQueue(msg, &result);
}

void StateMachine::SendMessageAtFrontOfQueue(
    /* [in] */ IMessage* msg)
{
    Boolean result;
    mSmHandler->SendMessageAtFrontOfQueue(msg, &result);
}

void StateMachine::RemoveMessages(
    /* [in] */ Int32 what)
{
    mSmHandler->RemoveMessages(what);
}

void StateMachine::Quit()
{
    // mSmHandler can be null if the state machine is already stopped.
    if (mSmHandler == NULL) return;

    mSmHandler->Quit();
}

void StateMachine::QuitNow()
{
    // mSmHandler can be null if the state machine is already stopped.
    if (mSmHandler == NULL) return;

    mSmHandler->QuitNow();
}

Boolean StateMachine::IsDbg()
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return FALSE;

    return mSmHandler->IsDbg();
}

void StateMachine::SetDbg(
    /* [in] */ Boolean dbg)
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    mSmHandler->SetDbg(dbg);
}

void StateMachine::Start()
{
    // mSmHandler can be null if the state machine has quit.
    if (mSmHandler == NULL) return;

    /** Send the complete construction message */
    mSmHandler->CompleteConstruction();
}

void StateMachine::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    // pw.println(getName() + ":");
    // pw.println(" total records=" + getLogRecCount());
    // for (int i=0; i < getLogRecSize(); i++) {
    //     pw.printf(" rec[%d]: %s\n", i, getLogRec(i).toString(this));
    //     pw.flush();
    // }
    // pw.println("curState=" + getCurrentState().getName());
}


} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
