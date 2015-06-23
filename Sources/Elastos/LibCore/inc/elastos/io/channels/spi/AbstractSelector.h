#ifndef __ELASTOS_IO_CHANNELS_SPI_ABSTRACTSELECTOR_H__
#define __ELASTOS_IO_CHANNELS_SPI_ABSTRACTSELECTOR_H__

#include "Selector.h"

using Elastos::Core::IRunnable;
using Elastos::IO::Channels::Selector;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

/**
 * {@code AbstractSelector} is the base implementation class for selectors.
 * It realizes the interruption of selection by {@code begin} and
 * {@code end}. It also holds the cancellation and the deletion of the key
 * set.
 */
class AbstractSelector
    : public Selector
    , public IAbstractSelector
{
protected:
    AbstractSelector();

    AbstractSelector(
        /* [in] */ ISelectorProvider* provider);

    virtual ~AbstractSelector();

public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ ISelectorProvider* provider);

    CARAPI Close();

    CARAPI IsOpen(
        /* [out] */ Boolean* isOpen);

    CARAPI GetProvider(
        /* [out] */ ISelectorProvider** provider);

    virtual CARAPI Register(
        /* [in] */ IAbstractSelectableChannel* channel,
        /* [in] */ Int32 operations,
        /* [in] */ IObject* obj,
        /* [out] */ ISelectionKey** returnKey) = 0;

protected:
    virtual CARAPI ImplCloseSelector() = 0;

    CARAPI CancelledKeys(
        /* [out] */ ISet** keySet);

    CARAPI Deregister(
        /* [in] */ ISelectionKey* key);

    CARAPI Begin();

    CARAPI End();

    CARAPI Cancel(
        /* [in] */ ISelectionKey* key);

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
