#ifndef __ELASTOS_IO_CHANNELS_SPI_ABSTRACTSELECTOR_H__
#define __ELASTOS_IO_CHANNELS_SPI_ABSTRACTSELECTOR_H__

#include <Elastos.CoreLibrary_server.h>
#include <elastos/core/Thread.h>
#include <Selector.h>
#include <stdio.h>
#include "AbstractSelectableChannel.h"
#include "AbstractSelectionKey.h"

using Elastos::Core::IRunnable;

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
        REFCOUNT_ADD(*provider);
        return NOERROR;
    }

    virtual CARAPI Register(
        /* [in] */ AbstractSelectableChannel* channel,
        /* [in] */ Int32 operations,
        /* [in] */ IObject* obj,
        /* [out] */ ISelectionKey** returnKey) = 0;

    CARAPI Deregister(
        /* [in] */ AbstractSelectionKey* key);

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
