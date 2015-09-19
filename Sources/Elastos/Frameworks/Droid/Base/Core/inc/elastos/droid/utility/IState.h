#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_ISTATE_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_ISTATE_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Droid::Os::IMessage;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

// extern const InterfaceID EIID_IState;

/**
 * {@hide}
 *
 * The interface for implementing states in a {@link StateMachine}
 */
interface IState : public IInterface
{
public:
    /**
     * Called when a state is entered.
     */
    virtual CARAPI Enter() = 0;

    /**
     * Called when a state is exited.
     */
    virtual CARAPI Exit() = 0;

    /**
     * Called when a message is to be processed by the
     * state machine.
     *
     * This routine is never reentered thus no synchronization
     * is needed as only one processMessage method will ever be
     * executing within a state machine at any given time. This
     * does mean that processing by this routine must be completed
     * as expeditiously as possible as no subsequent messages will
     * be processed until this routine returns.
     *
     * @param msg to process
     * @return HANDLED if processing has completed and NOT_HANDLED
     *         if the message wasn't processed.
     */
    virtual ProcessMessage(
        /* [in] */ IMessage* msg,
        /* [out] */ Boolean* result) = 0;

    /**
     * Name of State for debugging purposes.
     *
     * @return name of state.
     */
    virtual GetName(
        /* [out] */ String* name) = 0;

public:
    /**
     * Returned by processMessage to indicate the the message was processed.
     */
    static const Boolean HANDLED = TRUE;

    /**
     * Returned by processMessage to indicate the the message was NOT processed.
     */
    static const Boolean NOT_HANDLED = FALSE;
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_UTILITY_ISTATE_H__
