#include "SelectionKeyImpl.h"
#include "cmdef.h"

namespace Elastos {
namespace IO {

SelectionKeyImpl::SelectionKeyImpl(
    /* [in] */ IAbstractSelectableChannel* channel,
    /* [in] */ Int32 ops,
    /* [in] */ IObject* attachment,
    /* [in] */ ISelectorImpl* selector)
{
    INTERFACE_ADDREF(channel);
    this->mChannel = channel;
    this->mInterestOps = ops;
    INTERFACE_ADDREF(selector);
    this->mSelector = selector;
    AutoPtr<IObject> tempObj;
    Attach(attachment, (IObject**)&tempObj);
}

ECode SelectionKeyImpl::Channel(
    /* [in] */ ISelectableChannel** channel)
{
    *channel = mChannel;
    INTERFACE_ADDREF(*channel);
    return NOERROR;
}

ECode SelectionKeyImpl::InterestOps(
    /* [out] */ Int32* opts)
{
    assert(NULL != opts);
    ECode ecRet = CheckValid();
    if(NOERROR != ecRet)
    {
        return ecRet;
    }
 //   mSelector->mKeysLock;
    printf("WARNING: Not synchronized yet\n");
    *opts = mInterestOps;
    return NOERROR;
}

Int32 SelectionKeyImpl::InterestOpsNoCheck()
{
    printf("WARNING: Not synchronized yet\n");
//    mSelector->mKeysLock.Lock();
    Int32 ret = mInterestOps;
//    mSelector->mKeysLock.Unlock();
    return ret;
}

ECode SelectionKeyImpl::InterestOpsEx(
    /* [in] */ Int32 operations,
    /* [out] */ SelectionKey** key)
{
    VALIDATE_NOT_NULL(key)

    ECode ecRet = CheckValid();
    if(ecRet != CheckValid())
    {
        return ecRet;
    }

    Int32 ops;
    mChannel->ValidOps(&ops);
    if ((operations & ~ops) != 0)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    printf("WARNING: Not synchronized yet\n");
//          mSelector->KeyLock;
    mInterestOps = operations;
    *key = this;

    return NOERROR;
}

ECode SelectionKeyImpl::ReadyOps(
    /* [out] */ Int32* ret)
{
    assert(NULL != ret);
    ECode ecRet = CheckValid();
    if(NOERROR != ecRet)
    {
        return ecRet;
    }
    *ret = mReadyOps;
    return NOERROR;
}

ECode SelectionKeyImpl::Selector(
    /* [out] */ ISelector** selector)
{
      *selector = mSelector;
      INTERFACE_ADDREF(*selector);
      return NOERROR;
}

ECode SelectionKeyImpl::SetReadyOps(
    /* [in] */ Int32 readyOps)
{
    this->mReadyOps = readyOps;
    return NOERROR;
}

ECode SelectionKeyImpl::CheckValid()
{
    Boolean isValid;
    IsValid(&isValid);
    if (FALSE == isValid)
    {
        return E_CANCELLED_KEY_EXCEPTION;
    }
    return NOERROR;;
}

ECode SelectionKeyImpl::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    assert(NULL != isConnected);
    if (ISocketChannel::Probe(mChannel) == NULL)
    {
        *isConnected = TRUE;
        return NOERROR;
    }

    Boolean isConn = FALSE;
    ((ISocketChannel*) mChannel)->IsConnected(isConnected);
    *isConnected = isConn;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
