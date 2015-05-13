
#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "ext/frameworkdef.h"
#include "os/HandlerBase.h"
#include "util/State.h"
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/Vector.h>

using Elastos::Utility::HashMap;
using Elastos::Utility::List;
using Elastos::Utility::Vector;
using Elastos::Core::Threading::Mutex;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IHandlerThread;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

class StateMachine : public ElRefBase
{
public:
    /**
     * StateMachine logging record.
     * {@hide}
     */
    class LogRec : public ElRefBase
    {
    public:
        /**
         * Constructor
         *
         * @param msg
         * @param state that handled the message
         * @param orgState is the first state the received the message but
         * did not processes the message.
         */
        LogRec(
            /* [in] */ IMessage* msg,
            /* [in] */ const String& info,
            /* [in] */ State* state,
            /* [in] */ State* orgState);

        /**
         * Update the information in the record.
         * @param state that handled the message
         * @param orgState is the first state the received the message but
         * did not processes the message.
         */
        CARAPI_(void) Update(
            /* [in] */ IMessage* msg,
            /* [in] */ const String& info,
            /* [in] */ State* state,
            /* [in] */ State* orgState);

        /**
         * @return time stamp
         */
        CARAPI_(Int64) GetTime() { return mTime; }

        /**
         * @return msg.what
         */
        CARAPI_(Int64) GetWhat() { return mWhat; }

        /**
         * @return the command that was executing
         */
        CARAPI_(String) GetInfo() { return mInfo; }

        /**
         * @return the state that handled this message
         */
        CARAPI_(AutoPtr<State>) GetState() { return mState; }

        /**
         * @return the original state that received the message.
         */
        CARAPI_(AutoPtr<State>) GetOriginalState() { return mOrgState; }

        /**
         * @return as string
         */
        CARAPI_(String) ToString(
            /* [in] */ StateMachine* sm);

    private:
        Int64 mTime;
        Int32 mWhat;
        String mInfo;
        AutoPtr<State> mState;
        AutoPtr<State> mOrgState;
    };

private:
    /**
     * A list of log records including messages recently processed by the state machine.
     *
     * The class maintains a list of log records including messages
     * recently processed. The list is finite and may be set in the
     * constructor or by calling setSize. The public interface also
     * includes size which returns the number of recent records,
     * count which is the number of records processed since the
     * the last setSize, get which returns a record and
     * add which adds a record.
     */
    class LogRecords : public ElRefBase
    {
    public:
        /**
         * Set size of messages to maintain and clears all current records.
         *
         * @param maxSize number of records to maintain at anyone time.
        */
        CARAPI_(void) SetSize(
            /* [in] */ Int32 maxSize);

        /**
         * @return the number of recent records.
         */
        CARAPI_(Int32) GetSize();

        /**
         * @return the total number of records processed since size was set.
         */
        CARAPI_(Int32) Count();

        /**
         * Clear the list of records.
         */
        CARAPI_(void) Cleanup();

        /**
         * @return the information on a particular record. 0 is the oldest
         * record and size()-1 is the newest record. If the index is to
         * large null is returned.
         */
        CARAPI_(AutoPtr<LogRec>) Get(
            /* [in] */ Int32 index);

        /**
         * Add a processed message.
         *
         * @param msg
         * @param messageInfo to be stored
         * @param state that handled the message
         * @param orgState is the first state the received the message but
         * did not processes the message.
         */
        CARAPI_(void) Add(
            /* [in] */ IMessage* msg,
            /* [in] */ const String& messageInfo,
            /* [in] */ State* state,
            /* [in] */ State* orgState);

    public:
        /**
         * private constructor use add
         */
        LogRecords()
            : mMaxSize(DEFAULT_SIZE)
            , mOldestIndex(0)
            , mCount(0)
        {}

    private:
        static const Int32 DEFAULT_SIZE = 20;

        Vector< AutoPtr<LogRec> > mLogRecords;
        Int32 mMaxSize;
        Int32 mOldestIndex;
        Int32 mCount;
        Mutex mLock;
    };

    class SmHandler
        : public HandlerBase
    {
        friend class StateMachine;

    private:
        /**
         * Information about a state.
         * Used to maintain the hierarchy.
         */
        class StateInfo : public ElRefBase
        {
        public:
            StateInfo()
                : mActive(FALSE)
            {}

            /**
             * Convert StateInfo to string
             */
            CARAPI_(String) ToString()
            {
                // return "state=" + state.getName() + ",active=" + active
                //         + ",parent=" + ((parentStateInfo == null) ?
                //                         "null" : parentStateInfo.state.getName());
                return String(NULL);
            }

        public:
            /** The state */
            AutoPtr<State> mState;

            /** The parent of this state, null if there is no parent */
            AutoPtr<StateInfo> mParentStateInfo;

            /** True when the state has been entered and on the stack */
            Boolean mActive;
        };

        /**
         * State entered when transitionToHaltingState is called.
         */
        class HaltingState : public State
        {
        public:
            HaltingState(
                /* [in] */ SmHandler* owner)
                : mOwner(owner)
            {}

            // @Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage* msg,
                /* [out] */ Boolean* result)
            {
                mOwner->mSm->HaltedProcessMessage(msg);
                *result = TRUE;
                return NOERROR;
            }

        private:
            SmHandler* mOwner;
        };

        /**
         * State entered when a valid quit message is handled.
         */
        class QuittingState : public State
        {
            // @Override
            CARAPI ProcessMessage(
                /* [in] */ IMessage* msg,
                /* [out] */ Boolean* result)
            {
                *result = NOT_HANDLED;
                return NOERROR;
            }
        };

        class Object
            : public ElRefBase
            , public IInterface
        {
        public:
            CARAPI_(PInterface) Probe(
                /* [in] */ REIID riid)
            {
                if (riid == EIID_IInterface) {
                    return (PInterface)this;
                }

                return NULL;
            }

            CARAPI_(UInt32) AddRef()
            {
                return ElRefBase::AddRef();
            }

            CARAPI_(UInt32) Release()
            {
                return ElRefBase::Release();
            }

            CARAPI GetInterfaceID(
                /* [in] */ IInterface* pObject,
                /* [in] */ InterfaceID* pIID)
            {
                return E_NOT_IMPLEMENTED;
            }
        };

    public:
        /**
         * Constructor
         *
         * @param looper for dispatching messages
         * @param sm the hierarchical state machine
         */
        SmHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ StateMachine* sm);

        /**
         * Handle messages sent to the state machine by calling
         * the current state's processMessage. It also handles
         * the enter/exit calls and placing any deferred messages
         * back onto the queue when transitioning to a new state.
         */
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        /**
         * Do any transitions
         */
        CARAPI_(void) PerformTransitions();

        /**
         * Cleanup all the static variables and the looper after the SM has been quit.
         */
        CARAPI_(void) CleanupAfterQuitting();

        /**
         * Complete the construction of the state machine.
         */
        CARAPI_(void) CompleteConstruction();

        /**
         * Process the message. If the current state doesn't handle
         * it, call the states parent and so on. If it is never handled then
         * call the state machines unhandledMessage method.
         */
        CARAPI_(void) ProcessMsg(
            /* [in] */ IMessage* msg);

        /**
         * Call the exit method for each state from the top of stack
         * up to the common ancestor state.
         */
        CARAPI_(void) InvokeExitMethods(
            /* [in] */ StateInfo* commonStateInfo);

        /**
         * Invoke the enter method starting at the entering index to top of state stack
         */
        CARAPI_(void) InvokeEnterMethods(
            /* [in] */ Int32 stateStackEnteringIndex);

        /**
         * Move the deferred message to the front of the message queue.
         */
        CARAPI_(void) MoveDeferredMessageAtFrontOfQueue();

        /**
         * Move the contents of the temporary stack to the state stack
         * reversing the order of the items on the temporary stack as
         * they are moved.
         *
         * @return index into mStateStack where entering needs to start
         */
        CARAPI_(Int32) MoveTempStateStackToStateStack();

        /**
         * Setup the mTempStateStack with the states we are going to enter.
         *
         * This is found by searching up the destState's ancestors for a
         * state that is already active i.e. StateInfo.active == true.
         * The destStae and all of its inactive parents will be on the
         * TempStateStack as the list of states to enter.
         *
         * @return StateInfo of the common ancestor for the destState and
         * current state or null if there is no common parent.
         */
        CARAPI_(AutoPtr<StateInfo>) SetupTempStateStackWithStatesToEnter(
            /* [in] */ State* destState);

        /**
         * Initialize StateStack to mInitialState.
         */
        CARAPI_(void) SetupInitialStateStack();

        /**
         * @return current message
         */
        CARAPI_(AutoPtr<IMessage>) GetCurrentMessage()
        {
            return mMsg;
        }

        /**
         * @return current state
         */
        CARAPI_(AutoPtr<IState>) GetCurrentState()
        {
            return (*mStateStack)[mStateStackTopIndex]->mState;
        }

        /**
         * Add a new state to the state machine. Bottom up addition
         * of states is allowed but the same state may only exist
         * in one hierarchy.
         *
         * @param state the state to add
         * @param parent the parent of state
         * @return stateInfo for this state
         */
        CARAPI AddState(
            /* [in] */ State* state,
            /* [in] */ State* parent,
            /* [out] */ StateInfo** info);

        /** @see StateMachine#setInitialState(State) */
        CARAPI_(void) SetInitialState(
            /* [in] */ State* initialState);

        /** @see StateMachine#transitionTo(IState) */
        CARAPI_(void) TransitionTo(
            /* [in] */ IState* destState);

        /** @see StateMachine#deferMessage(Message) */
        CARAPI_(void) DeferMessage(
            /* [in] */ IMessage* msg);

        /** @see StateMachine#quit() */
        CARAPI_(void) Quit();

        /** @see StateMachine#quitNow() */
        CARAPI_(void) QuitNow();

        /** Validate that the message was sent by quit or abort. */
        CARAPI_(Boolean) IsQuit(
            /* [in] */ IMessage* msg);

        /** @see StateMachine#isDbg() */
        CARAPI_(Boolean) IsDbg()
        {
            return mDbg;
        }

        /** @see StateMachine#setDbg(boolean) */
        CARAPI_(void) SetDbg(
            /* [in] */ Boolean dbg)
        {
            mDbg = dbg;
        }

    private:
        /** The debug flag */
        Boolean mDbg;

        /** The SmHandler object, identifies that message is internal */
        static const AutoPtr<IInterface> sSmHandlerObj;

        /** The current message */
        AutoPtr<IMessage> mMsg;

        /** A list of log records including messages this state machine has processed */
        AutoPtr<LogRecords> mLogRecords;

        /** true if construction of the state machine has not been completed */
        Boolean mIsConstructionCompleted;

        /** Stack used to manage the current hierarchy of states */
        AutoPtr< ArrayOf<StateInfo*> > mStateStack;

        /** Top of mStateStack */
        Int32 mStateStackTopIndex;

        /** A temporary stack used to manage the state stack */
        AutoPtr< ArrayOf<StateInfo*> > mTempStateStack;

        /** The top of the mTempStateStack */
        Int32 mTempStateStackCount;

        /** State used when state machine is halted */
        AutoPtr<HaltingState> mHaltingState;

        /** State used when state machine is quitting */
        AutoPtr<QuittingState> mQuittingState;

        /** Reference to the StateMachine */
        // TODO: StateMachine can't be delete before HandleMessage return
        //
        AutoPtr<StateMachine> mSm;

        /** The map of all of the states in the state machine */
        HashMap< AutoPtr<State>, AutoPtr<StateInfo> > mStateInfo;

        /** The initial state that will process the first message */
        AutoPtr<State> mInitialState;

        /** The destination state when transitionTo has been invoked */
        AutoPtr<State> mDestState;

        /** The list of deferred messages */
        List< AutoPtr<IMessage> > mDeferredMessages;
    };

public:
    /**
     * @return the name
     */
    CARAPI_(String) GetName()
    {
        return mName;
    }

    /**
     * Set number of log records to maintain and clears all current records.
     *
     * @param maxSize number of messages to maintain at anyone time.
     */
    CARAPI_(void) SetLogRecSize(
        /* [in] */ Int32 maxSize)
    {
        mSmHandler->mLogRecords->SetSize(maxSize);
    }

    /**
     * @return number of log records
     */
    CARAPI_(Int32) GetLogRecSize()
    {
        return mSmHandler->mLogRecords->GetSize();
    }

    /**
     * @return the total number of records processed
     */
    CARAPI_(Int32) GetLogRecCount()
    {
        return mSmHandler->mLogRecords->Count();
    }

    /**
     * @return a log record
     */
    CARAPI_(AutoPtr<LogRec>) GetLogRec(
        /* [in] */ Int32 index)
    {
        return mSmHandler->mLogRecords->Get(index);
    }

    /**
     * @return Handler
     */
    CARAPI_(AutoPtr<IHandler>) GetHandler()
    {
        return mSmHandler;
    }

    /**
     * Get a message and set Message.target = this.
     *
     * @return message or null if SM has quit
     */
    CARAPI_(AutoPtr<IMessage>) ObtainMessage();

    /**
     * Get a message and set Message.target = this and what
     *
     * @param what is the assigned to Message.what.
     * @return message or null if SM has quit
     */
    CARAPI_(AutoPtr<IMessage>) ObtainMessage(
        /* [in] */ Int32 what);

    /**
     * Get a message and set Message.target = this,
     * what and obj.
     *
     * @param what is the assigned to Message.what.
     * @param obj is assigned to Message.obj.
     * @return message or null if SM has quit
     */
    CARAPI_(AutoPtr<IMessage>) ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    /**
     * Get a message and set Message.target = this,
     * what, arg1 and arg2
     *
     * @param what  is assigned to Message.what
     * @param arg1  is assigned to Message.arg1
     * @param arg2  is assigned to Message.arg2
     * @return  A Message object from the global pool or null if
     *          SM has quit
     */
    CARAPI_(AutoPtr<IMessage>) ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2);

    /**
     * Get a message and set Message.target = this,
     * what, arg1, arg2 and obj
     *
     * @param what  is assigned to Message.what
     * @param arg1  is assigned to Message.arg1
     * @param arg2  is assigned to Message.arg2
     * @param obj is assigned to Message.obj
     * @return  A Message object from the global pool or null if
     *          SM has quit
     */
    CARAPI_(AutoPtr<IMessage>) ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    /**
     * Enqueue a message to this state machine.
     */
    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what);

    /**
     * Enqueue a message to this state machine.
     */
    CARAPI_(void) SendMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    /**
     * Enqueue a message to this state machine.
     */
    CARAPI_(void) SendMessage(
        /* [in] */ IMessage* msg);

    /**
     * Enqueue a message to this state machine after a delay.
     */
    CARAPI_(void) SendMessageDelayed(
        /* [in] */ Int32 what,
        /* [in] */ Int64 delayMillis);

    /**
     * Enqueue a message to this state machine after a delay.
     */
    CARAPI_(void) SendMessageDelayed(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [in] */ Int64 delayMillis);

    /**
     * Enqueue a message to this state machine after a delay.
     */
    CARAPI_(void) SendMessageDelayed(
        /* [in] */ IMessage* msg,
        /* [in] */ Int64 delayMillis);

    /**
     * @return if debugging is enabled
     */
    virtual CARAPI_(Boolean) IsDbg();

    /**
     * Set debug enable/disabled.
     *
     * @param dbg is true to enable debugging.
     */
    virtual CARAPI_(void) SetDbg(
        /* [in] */ Boolean dbg);

    /**
     * Start the state machine.
     */
    virtual CARAPI_(void) Start();

    /**
     * Dump the current state.
     *
     * @param fd
     * @param pw
     * @param args
     */
    virtual CARAPI_(void) Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

protected:
    StateMachine() {}

    /**
     * Constructor creates a StateMachine with its own thread.
     *
     * @param name of the state machine
     */
    StateMachine(
        /* [in] */ const String& name);

    /**
     * Constructor creates a StateMachine using the looper.
     *
     * @param name of the state machine
     */
    StateMachine(
        /* [in] */ const String& name,
        /* [in] */ ILooper* looper);

    /**
     * Add a new state to the state machine
     * @param state the state to add
     * @param parent the parent of state
     */
    CARAPI AddState(
        /* [in] */ State* state,
        /* [in] */ State* parent);

    /**
     * @return current message
     */
    CARAPI_(AutoPtr<IMessage>) GetCurrentMessage()
    {
        return mSmHandler->GetCurrentMessage();
    }

    /**
     * @return current state
     */
    CARAPI_(AutoPtr<IState>) GetCurrentState()
    {
        return mSmHandler->GetCurrentState();
    }

    /**
     * Add a new state to the state machine, parent will be null
     * @param state to add
     */
    CARAPI AddState(
        /* [in] */ State* state);

    /**
     * Set the initial state. This must be invoked before
     * and messages are sent to the state machine.
     *
     * @param initialState is the state which will receive the first message.
     */
    CARAPI_(void) SetInitialState(
        /* [in] */ State* initialState)
    {
        mSmHandler->SetInitialState(initialState);
    }

    /**
     * transition to destination state. Upon returning
     * from processMessage the current state's exit will
     * be executed and upon the next message arriving
     * destState.enter will be invoked.
     *
     * this function can also be called inside the enter function of the
     * previous transition target, but the behavior is undefined when it is
     * called mid-way through a previous transition (for example, calling this
     * in the enter() routine of a intermediate node when the current transition
     * target is one of the nodes descendants).
     *
     * @param destState will be the state that receives the next message.
     */
    CARAPI_(void) TransitionTo(
        /* [in] */ IState* destState)
    {
        mSmHandler->TransitionTo(destState);
    }

    /**
     * transition to halt state. Upon returning
     * from processMessage we will exit all current
     * states, execute the onHalting() method and then
     * for all subsequent messages haltedProcessMessage
     * will be called.
     */
    CARAPI_(void) TransitionToHaltingState()
    {
        mSmHandler->TransitionTo(mSmHandler->mHaltingState);
    }

    /**
     * Defer this message until next state transition.
     * Upon transitioning all deferred messages will be
     * placed on the queue and reprocessed in the original
     * order. (i.e. The next state the oldest messages will
     * be processed first)
     *
     * @param msg is deferred until the next transition.
     */
    CARAPI_(void) DeferMessage(
        /* [in] */ IMessage* msg)
    {
        mSmHandler->DeferMessage(msg);
    }

    /**
     * Called when message wasn't handled
     *
     * @param msg that couldn't be handled.
     */
    virtual CARAPI_(void) UnhandledMessage(
        /* [in] */ IMessage* msg);

    /**
     * Called for any message that is received after
     * transitionToHalting is called.
     */
    virtual CARAPI_(void) HaltedProcessMessage(
        /* [in] */ IMessage* msg)
    {}

    /**
     * This will be called once after handling a message that called
     * transitionToHalting. All subsequent messages will invoke
     * {@link StateMachine#haltedProcessMessage(Message)}
     */
    virtual CARAPI_(void) OnHalting() {}

    /**
     * This will be called once after a quit message that was NOT handled by
     * the derived StateMachine. The StateMachine will stop and any subsequent messages will be
     * ignored. In addition, if this StateMachine created the thread, the thread will
     * be stopped after this method returns.
     */
    virtual CARAPI_(void) OnQuitting() {}

    /**
     * Add the string to LogRecords.
     *
     * @param string
     */
    virtual CARAPI_(void) AddLogRec(
        /* [in] */ const String& string)
    {
        mSmHandler->mLogRecords->Add(NULL, string, NULL, NULL);
    }

    /**
     * Add the string and state to LogRecords
     *
     * @param string
     * @param state current state
     */
    virtual CARAPI_(void) AddLogRec(
        /* [in] */ const String& string,
        /* [in] */ State* state)
    {
        mSmHandler->mLogRecords->Add(NULL, string, state, NULL);
    }

    /**
     * @return true if msg should be saved in the log, default is true.
     */
    virtual CARAPI_(Boolean) RecordLogRec(
        /* [in] */ IMessage* msg)
    { return TRUE; }

    /**
     * Return a string to be logged by LogRec, default
     * is an empty string. Override if additional information is desired.
     *
     * @param msg that was processed
     * @return information to be logged as a String
     */
    virtual CARAPI_(String) GetLogRecString(
        /* [in] */ IMessage* msg)
    { return String(""); }

    /**
     * @return the string for msg.what
     */
    virtual CARAPI_(String) GetWhatToString(
        /* [in] */ Int32 what)
    { return String(NULL); }

    /**
     * Enqueue a message to the front of the queue for this state machine.
     * Protected, may only be called by instances of StateMachine.
     */
    CARAPI_(void) SendMessageAtFrontOfQueue(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    /**
     * Enqueue a message to the front of the queue for this state machine.
     * Protected, may only be called by instances of StateMachine.
     */
    CARAPI_(void) SendMessageAtFrontOfQueue(
        /* [in] */ Int32 what);

    /**
     * Enqueue a message to the front of the queue for this state machine.
     * Protected, may only be called by instances of StateMachine.
     */
    CARAPI_(void) SendMessageAtFrontOfQueue(
        /* [in] */ IMessage* msg);

    /**
     * Removes a message from the message queue.
     * Protected, may only be called by instances of StateMachine.
     */
    CARAPI_(void) RemoveMessages(
        /* [in] */ Int32 what);

    /**
     * Quit the state machine after all currently queued up messages are processed.
     */
    CARAPI_(void) Quit();

    /**
     * Quit the state machine immediately all currently queued messages will be discarded.
     */
    CARAPI_(void) QuitNow();

protected:
    /**
     * Initialize.
     *
     * @param looper for this state machine
     * @param name of the state machine
     */
    CARAPI_(void) InitStateMachine(
        /* [in] */ const String& name,
        /* [in] */ ILooper* looper);

public:
    /**
     * Convenience constant that maybe returned by processMessage
     * to indicate the the message was processed and is not to be
     * processed by parent states
     */
    static const Boolean HANDLED = TRUE;

    /**
     * Convenience constant that maybe returned by processMessage
     * to indicate the the message was NOT processed and is to be
     * processed by parent states
     */
    static const Boolean NOT_HANDLED = FALSE;

private:
    static const String TAG;
    String mName;

    /** Message.what value when quitting */
    static const Int32 SM_QUIT_CMD = -1;

    /** Message.what value when initializing */
    static const Int32 SM_INIT_CMD = -2;

    AutoPtr<SmHandler> mSmHandler;
    AutoPtr<IHandlerThread> mSmThread;
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__STATEMACHINE_H__
