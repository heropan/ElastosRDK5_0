#include "AbstractInterruptibleChannel.h"
#include <elastos/Thread.h>
#include "cmdef.h"

using Elastos::Core::Threading::Thread;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

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
    mMutex.Lock();
    Boolean ret = !mClosed;
    mMutex.Unlock();
    return ret;
}

ECode AbstractInterruptibleChannel::IsOpen(
    /* [in] */ Boolean* isOpen)
{
    VALIDATE_NOT_NULL(isOpen);
    *isOpen = IsOpen();
    return NOERROR;
}

ECode AbstractInterruptibleChannel::Close()
{
    ECode retCode = E_INTERRUPTED;

    if (!mClosed)
    {
        mMutex.Lock();
        if (!mClosed)
        {
            mClosed = true;
            retCode = this->ImplCloseChannel();
        }
        mMutex.Unlock();
    }

    return retCode;
}

void AbstractInterruptibleChannel::Begin()
{
    Thread::GetCurrentThread()->PushInterruptAction(mInterruptAndCloseRunnable);
}

ECode AbstractInterruptibleChannel::End(Boolean success)
{
    Thread::GetCurrentThread()->PopInterruptAction(mInterruptAndCloseRunnable);

    if (mInterrupted)
    {
        mInterrupted = false;
        return E_CLOSED_BY_INTERRUPT_EXCEPTION;
    }
    if (!success && mClosed)
    {
        return E_ASYNCHRONOUS_CLOSE_EXCEPTION;
    }
    return NOERROR;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
