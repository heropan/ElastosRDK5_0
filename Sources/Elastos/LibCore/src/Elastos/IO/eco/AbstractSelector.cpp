
#include "AbstractSelector.h"
#include "CHashSet.h"

using Elastos::Utility::CHashSet;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

class WakeupRunnable
    : public ElRefBase
    , public IRunnable
{
public:
    WakeupRunnable(AbstractSelector* selector)
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
    /* [in] */REIID riid)
    {
        return NULL;
    }

    CARAPI GetInterfaceID(
            /* [in] */IInterface *pObject,
            /* [out] */InterfaceID *pIID)
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

AbstractSelector::AbstractSelector(ISelectorProvider* provider)
{
    Init(provider);
}

AbstractSelector::Init(ISelectorProvider* provider)
{
    mIsOpen = TRUE;
    mSelectorProvider = provider;
    CHashSet::New((ISet**)&mCancelledKeySet);
    mWakeupRunnable = new WakeupRunnable(this);
    assert(NULL != mWakeupRunnable);
}

AbstractSelector::~AbstractSelector()
{
}

ECode AbstractSelector::Deregister(IAbstractSelectionKey* key)
{
    VALIDATE_NOT_NULL(key);

    AutoPtr<ISelectableChannel> channel;
    FAIL_RETURN(key->Channel((ISelectableChannel**)&channel));
    AutoPtr<IAbstractSelectableChannel> asc = IAbstractSelectableChannel::Probe(channel);
    if (asc) asc->Deregister(key);
    return NOERROR;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
