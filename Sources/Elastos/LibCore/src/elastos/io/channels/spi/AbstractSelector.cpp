
#include "AbstractSelector.h"
#include "AbstractSelectableChannel.h"
#include "CHashSet.h"
#include "Thread.h"
#include "CAtomicBoolean.h"
#include "AutoLock.h"

using Elastos::Core::Thread;
using Elastos::Utility::CHashSet;
using Elastos::Utility::Concurrent::Atomic::CAtomicBoolean;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

//==================================================================
//　WakeupRunnable
//==================================================================
class WakeupRunnable
    : public ElRefBase
    , public IRunnable
{
public:
    WakeupRunnable(
        /* [in] */ AbstractSelector* selector)
    {
        this->mSelector = selector;
    }

    ~WakeupRunnable()
    {

    }

    ECode Run()
    {
        mSelector->Wakeup();
        return NOERROR;
    }

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid)
    {
        return NULL;
    }

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID)
    {
        return E_NOT_IMPLEMENTED;
    }

    CARAPI_(UInt32) AddRef()
    {
        return ElRefBase::AddRef();
    }

    CARAPI_(UInt32) Release()
    {
        return ElRefBase::Release();
    }

private:
    AbstractSelector *mSelector;
};

//==================================================================
//　AbstractSelector
//==================================================================

CAR_INTERFACE_IMPL(AbstractSelector, Object, IAbstractSelector)

AbstractSelector::AbstractSelector()
{
}

AbstractSelector::AbstractSelector(
    /* [in] */ ISelectorProvider* provider)
{
    constructor(provider);
}

AbstractSelector::~AbstractSelector()
{
}

ECode AbstractSelector::constructor(
    /* [in] */ ISelectorProvider* provider)
{
    CAtomicBoolean::New(TRUE, (IAtomicBoolean**)&mIsOpen);
    mSelectorProvider = provider;
    CHashSet::New((ISet**)&mCancelledKeySet);
    mWakeupRunnable = new WakeupRunnable(this);
    assert(NULL != mWakeupRunnable);
    return NOERROR;
}

ECode AbstractSelector::Close()
{
    Boolean isflag = FALSE;
    mIsOpen->GetAndSet(FALSE, &isflag);
    if (isflag) {
        ImplCloseSelector();
    }
    return NOERROR;
}

ECode AbstractSelector::IsOpen(
    /* [out] */ Boolean* isOpen)
{
    return mIsOpen->Get(isOpen);
}

ECode AbstractSelector::GetProvider(
    /* [out] */ ISelectorProvider** provider)
{
    VALIDATE_NOT_NULL(provider)
    *provider = mSelectorProvider;
    REFCOUNT_ADD(*provider);
    return NOERROR;
}

ECode AbstractSelector::Deregister(
    /* [in] */ ISelectionKey* key)
{
    VALIDATE_NOT_NULL(key);

    AutoPtr<ISelectableChannel> channel;
    FAIL_RETURN(key->GetChannel((ISelectableChannel**)&channel));
    if (channel) {
        AbstractSelectableChannel* asc = (AbstractSelectableChannel*)channel.Get();
        asc->Deregister(key);
    }
    return NOERROR;
}

ECode AbstractSelector::CancelledKeys(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)
    *keySet = mCancelledKeySet;
    REFCOUNT_ADD(*keySet);
    return NOERROR;
}

ECode AbstractSelector::Begin()
{
    return Thread::GetCurrentThread()->PushInterruptAction(mWakeupRunnable);
}

ECode AbstractSelector::End()
{
    return Thread::GetCurrentThread()->PopInterruptAction(mWakeupRunnable);
}

ECode AbstractSelector::Cancel(
    /* [in] */ ISelectionKey* key)
{
    synchronized (mCancelledKeySet) {
        Boolean isflag = FALSE;
        mCancelledKeySet->Add(key, &isflag);
    }
    return NOERROR;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
