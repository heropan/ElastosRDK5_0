#ifndef __ELASTOS_IO_CHANNELS_SELECTIONKEYIMPL_H__
#define __ELASTOS_IO_CHANNELS_SELECTIONKEYIMPL_H__

#include "AbstractSelectionKey.h"

using Elastos::Core::IRunnable;
using Elastos::IO::Channels::ISelector;
using Elastos::IO::Channels::ISocketChannel;
using Elastos::IO::Channels::ISelectableChannel;
using Elastos::IO::Channels::Spi::AbstractSelectionKey;
using Elastos::IO::Channels::Spi::IAbstractSelectableChannel;

namespace Elastos {
namespace IO {
/**
 * Default implementation of SelectionKey
 */
class SelectionKeyImpl : public AbstractSelectionKey
{
public:
    SelectionKeyImpl(
        /* [in] */ IAbstractSelectableChannel* channel,
        /* [in] */ Int32 opts,
        /* [in] */ IObject* attach,
        /* [in] */ ISelectorImpl* selector);

    CARAPI Channel(
        /* [out] */ ISelectableChannel** channel);

    CARAPI InterestOps(
        /* [out] */ Int32* opts);

    Int32 InterestOpsNoCheck();

    CARAPI InterestOps(
        /* [in] */ Int32 operations,
        /* [out] */ SelectionKey** key);

    CARAPI ReadyOps(
        /* [out] */ Int32* ret);

    CARAPI Selector(
        /* [in] */ ISelector** selector);

    CARAPI SetReadyOps(
        /* [in] */ Int32 readyOps);

    CARAPI CheckValid();

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

private:
    IAbstractSelectableChannel* mChannel;
    Int32 mInterestOps;
    Int32 mReadyOps;
    ISelectorImpl* mSelector;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHANNELS_SELECTIONKEYIMPL_H__
