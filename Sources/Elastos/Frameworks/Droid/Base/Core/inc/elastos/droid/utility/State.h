#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_STATE_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_STATE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/utility/IState.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

/**
 * {@hide}
 *
 * The class for implementing states in a StateMachine
 */
class State
    : public ElRefBase
    , public IState
{
public:
    PInterface Probe(
        /* [in] */ REIID riid)
    {
        if ( riid == EIID_IInterface) {
            return (IInterface*)(IState *)this;
        }
        // else if ( riid == EIID_IState) {
        //     return (IState *)this;
        // }
        return NULL;
    }

    UInt32 AddRef()
    {
        return ElRefBase::AddRef();
    }

    UInt32 Release()
    {
        return ElRefBase::Release();
    }

    ECode GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* iid)
    {
        assert(0);
        return E_NOT_IMPLEMENTED;
    }

    /* (non-Javadoc)
     * @see com.android.internal.util.IState#enter()
     */
    // @Override
    virtual CARAPI Enter() { return NOERROR; }

    /* (non-Javadoc)
     * @see com.android.internal.util.IState#exit()
     */
    // @Override
    virtual CARAPI Exit() { return NOERROR; }

    /* (non-Javadoc)
     * @see com.android.internal.util.IState#processMessage(android.os.Message)
     */
    // @Override
    virtual CARAPI ProcessMessage(
        /* [in] */ IMessage* msg,
        /* [out] */ Boolean* result)
    {
        VALIDATE_NOT_NULL(result);
        *result = FALSE;
        return NOERROR;
    }

    /**
     * Name of State for debugging purposes.
     *
     * This default implementation returns the class name, returning
     * the instance name would better in cases where a State class
     * is used for multiple states. But normally there is one class per
     * state and the class name is sufficient and easy to get. You may
     * want to provide a setName or some other mechanism for setting
     * another name if the class name is not appropriate.
     *
     * @see com.android.internal.util.IState#processMessage(android.os.Message)
     */
    // @Override
    virtual CARAPI GetName(
        /* [out] */ String* name)
    {
        // String name = getClass().getName();
        // int lastDollar = name.lastIndexOf('$');
        // return name.substring(lastDollar + 1);
        *name = "State";
        return NOERROR;
    }

    virtual CARAPI_(String) GetName()
    {
        // String name = getClass().getName();
        // int lastDollar = name.lastIndexOf('$');
        // return name.substring(lastDollar + 1);
        return String("State");
    }
protected:
    State() {}
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_STATE
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Internal::Utility::State)
#endif

#endif //__ELASTOS_DROID_INTERNAL_UTILITY_STATE_H__
