#ifndef __ELASTOS_IO_CHANNELS_SPI_ABSTRACTSELECTOR_H__
#define __ELASTOS_IO_CHANNELS_SPI_ABSTRACTSELECTOR_H__

#include <cmdef.h>
#include <Elastos.CoreLibrary_server.h>
#include <elastos/Thread.h>
#include <elastos/Mutex.h>
#include <Selector.h>
#include <stdio.h>

using Elastos::Core::IRunnable;
using Elastos::Core::Mutex;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

/**
 * {@code AbstractSelectionKey} is the base implementation class for selection keys.
 * It implements validation and cancellation methods.
 */
class AbstractSelector : public Selector
{
public:
    AbstractSelector();

    AbstractSelector(
        /* [in] */ ISelectorProvider* provider);

    Init(
        /* [in] */ ISelectorProvider* provider);

    virtual ~AbstractSelector();

    ECode Close()
    {
        printf(
                "Warning:It should be atomic boolean in AbstractSelector::Close()\n");
        if (mIsOpen)
        {
            mIsOpen = FALSE;
            ImplCloseSelector();
        }
        return NOERROR;
    }

    ECode IsOpen(
        /* [out] */ Boolean* isOpen)
    {
        assert(NULL != isOpen);
        printf(
                "Warning:It should be atomic boolean in AbstractSelector::IsOpen()\n");
        *isOpen = mIsOpen;
        return NOERROR;
    }

    ECode Provider(
        /* [out] */ ISelectorProvider** provider)
    {
        *provider = mSelectorProvider;
        INTERFACE_ADDREF(*provider);
        return NOERROR;
    }

    virtual CARAPI Register(
        /* [in] */ IAbstractSelectableChannel* channel,
        /* [in] */ Int32 operations,
        /* [in] */ IObject* obj,
        /* [out] */ ISelectionKey** returnKey) = 0;

    CARAPI Deregister(
        /* [in] */ IAbstractSelectionKey* key);

protected:
    virtual CARAPI ImplCloseSelector() = 0;

    ECode CancelledKeys(
        /* [out] */ ISet** keySet)
    {
        *keySet = mCancelledKeySet;
        return NOERROR;
    }

private:
    Boolean mIsOpen;
    AutoPtr<ISelectorProvider> mSelectorProvider;
    AutoPtr<ISet> mCancelledKeySet;
    AutoPtr<IRunnable> mWakeupRunnable;
};

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif
