#include "AbstractInterruptibleChannel.h"
#include "Thread.h"
#include "AutoLock.h"

using Elastos::Core::Thread;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

// {da62f295-36e3-4f0c-b46d-edd00ffa9626}
extern "C" const InterfaceID EIID_AbstractInterruptibleChannel =
        { 0xda62f295, 0x36e3, 0x4f0c, { 0xb4, 0x6d, 0xed, 0xd0, 0x0f, 0xfa, 0x96, 0x26 } };

CAR_INTERFACE_IMPL_4(AbstractInterruptibleChannel, Object, IChannel, ICloseable, IInterruptibleChannel, IAbstractInterruptibleChannel)

class ActionRunnable
    : public ElRefBase
    , public IRunnable
{
public:
    ActionRunnable(AbstractInterruptibleChannel *channel)
    {
        this->mChannel = channel;
    }

    ~ActionRunnable()
    {
    }

    ECode Run()
    {
        mChannel->SetInterrupted(TRUE);
        return mChannel->Close();
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
    AbstractInterruptibleChannel *mChannel;
};

AbstractInterruptibleChannel::AbstractInterruptibleChannel()
    : mClosed(FALSE)
    , mInterrupted(FALSE)
{
    mInterruptAndCloseRunnable = new ActionRunnable(this);
}

AbstractInterruptibleChannel::~AbstractInterruptibleChannel()
{
}

Boolean AbstractInterruptibleChannel::IsOpen()
{
    AutoLock lock(this);
    Boolean ret = !mClosed;
    return ret;
}

ECode AbstractInterruptibleChannel::IsOpen(
    /* [out] */ Boolean* isOpen)
{
    VALIDATE_NOT_NULL(isOpen);
    *isOpen = IsOpen();
    return NOERROR;
}

ECode AbstractInterruptibleChannel::Close()
{
    ECode retCode = E_INTERRUPTED;

    if (!mClosed) {
        AutoLock lock(this);
        if (!mClosed) {
            mClosed = TRUE;
            retCode = this->ImplCloseChannel();
        }
    }

    return retCode;
}

void AbstractInterruptibleChannel::SetInterrupted(
    /* [in] */ Boolean value)
{
    mInterrupted = value;
}

void AbstractInterruptibleChannel::Begin()
{
    Thread::GetCurrentThread()->PushInterruptAction(mInterruptAndCloseRunnable);
}

ECode AbstractInterruptibleChannel::End(Boolean success)
{
    Thread::GetCurrentThread()->PopInterruptAction(mInterruptAndCloseRunnable);

    if (mInterrupted) {
        mInterrupted = FALSE;
        return E_CLOSED_BY_INTERRUPT_EXCEPTION;
    }
    if (!success && mClosed) {
        return E_ASYNCHRONOUS_CLOSE_EXCEPTION;
    }
    return NOERROR;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
