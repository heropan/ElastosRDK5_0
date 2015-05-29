
#include "AbstractSelector.h"
// #include "CHashSet.h"

// using Elastos::Utility::CHashSet;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

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
        mSelector->Wakeup(NULL);
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

AbstractSelector::AbstractSelector()
{
}

AbstractSelector::AbstractSelector(
    /* [in] */ ISelectorProvider* provider)
{
    Init(provider);
}

ECode AbstractSelector::Init(
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

AbstractSelector::~AbstractSelector()
{
}

ECode AbstractSelector::Deregister(
    /* [in] */ AbstractSelectionKey* key)
{
    VALIDATE_NOT_NULL(key);

    AutoPtr<ISelectableChannel> channel;
    FAIL_RETURN(key->Channel((ISelectableChannel**)&channel));
    AbstractSelectableChannel* asc = (AbstractSelectableChannel*)channel.Get();
    if (asc) asc->Deregister((ISelectionKey*)key);
    return NOERROR;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
