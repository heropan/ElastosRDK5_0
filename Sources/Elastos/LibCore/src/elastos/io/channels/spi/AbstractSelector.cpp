
#include "AbstractSelector.h"
#include "AbstractSelectableChannel.h"
// #include "CHashSet.h"

// using Elastos::Utility::CHashSet;

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
    mIsOpen = TRUE;
    mSelectorProvider = provider;
    assert(0 && "TODO"); // wanguangming
    // CHashSet::New((ISet**)&mCancelledKeySet);
    mWakeupRunnable = new WakeupRunnable(this);
    assert(NULL != mWakeupRunnable);
    return NOERROR;
}

ECode AbstractSelector::Close()
{
    ALOGW("Warning:It should be atomic boolean in AbstractSelector::Close()\n");
    if (mIsOpen)
    {
        mIsOpen = FALSE;
        ImplCloseSelector();
    }
    return NOERROR;
}

ECode AbstractSelector::IsOpen(
    /* [out] */ Boolean* isOpen)
{
    VALIDATE_NOT_NULL(isOpen)
    ALOGW("Warning:It should be atomic boolean in AbstractSelector::IsOpen()\n");
    *isOpen = mIsOpen;
    return NOERROR;
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
    // Thread.currentThread().pushInterruptAction$(wakeupRunnable);
    return E_NOT_IMPLEMENTED;
}

ECode AbstractSelector::End()
{
    // Thread.currentThread().popInterruptAction$(wakeupRunnable);
    return E_NOT_IMPLEMENTED;
}

ECode AbstractSelector::Cancel(
    /* [in] */ ISelectionKey* key)
{
    // synchronized (cancelledKeysSet) {
    //     cancelledKeysSet.add(key);
    // }
    return E_NOT_IMPLEMENTED;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
