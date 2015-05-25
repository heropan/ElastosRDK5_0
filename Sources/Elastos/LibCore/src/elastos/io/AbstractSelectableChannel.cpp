#include "AbstractSelectableChannel.h"
#include <Elastos.CoreLibrary_server.h>
#include <elastos/Thread.h>
#include <elastos/Mutex.h>

using Elastos::Core::IRunnable;
using Elastos::Core::Thread;
using Elastos::Core::Mutex;

namespace Elastos {
namespace IO {
namespace Channels {
namespace Spi {

AbstractSelectableChannel::AbstractSelectableChannel(
    /* [in] */ ISelectorProvider* provider)
    : mProvider(provider)
    , mIsBlocking(FALSE)
{
}

AbstractSelectableChannel::~AbstractSelectableChannel()
{
}

ECode AbstractSelectableChannel::Provider(
    /* [in] */ ISelectorProvider** provider)
{
    VALIDATE_NOT_NULL(provider);
    *provider = mProvider;
    INTERFACE_ADDREF(*provider);
    return NOERROR;
}

ECode AbstractSelectableChannel::IsRegistered(
    /* [in] */ Boolean* isRegister)
{
    VALIDATE_NOT_NULL(isRegister);
    Object::Autolock lock(mBlockingLock);
    *isRegister = !mKeyList.IsEmpty();
    return NOERROR;
}

ECode AbstractSelectableChannel::KeyFor(
    /* [in] */ ISelector* selector,
    /* [out] */ ISelectionKey** key)
{
    VALIDATE_NOT_NULL(key);
    *key = NULL;

    Object::Autolock lock(mBlockingLock);
    ISelectionKey* selKey;
    AutoPtr<ISelector> sel;
    List<AutoPtr<ISelectionKey> >::Iterator it = mKeyList.Begin();
    for (; it != mKeyList.End(); ++it) {
        selKey = *it;
        if (selKey) {
            sel = NULL;
            selKey->Selector((ISelector**)&sel);
            if (sel.Get() == selector) {
                *key = selKey;
                INTERFACE_ADDREF(*key);
                return NOERROR;
            }
        }
    }

    return NOERROR;
}

ECode AbstractSelectableChannel::Register(
    /* [in] */ ISelector* selector,
    /* [in] */ Int32 interestSet,
    /* [in] */ IObject* obj,
    /* [out] */ ISelectionKey** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    VALIDATE_NOT_NULL(selector);
    VALIDATE_NOT_NULL(obj);

    Boolean isOpen = FALSE;
    IsOpen(&isOpen);
    if (!isOpen) {
        return E_CLOSED_CHANNEL_EXCEPTION;
    }

    Int32 ops = 0;
    ValidOps(&ops);
    if (!((interestSet & ~ops) == 0)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Object::Autolock lock(mBlockingLock);
    ECode ec = NOERROR;
    if (mIsBlocking) {
        return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
    }

    selector->IsOpen(&isOpen);
    if (!isOpen) {
        if (0 == interestSet) {
            return E_ILLEGAL_SELECTOR_EXCEPTION;
        }
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<ISelectionKey> key;
    KeyFor(selector, (ISelectionKey**)&key);
    if (NULL == key) {
        IAbstractSelector* absSel = IAbstractSelector::Probe(selector);
        absSel->Register(THIS_PROBE(IAbstractSelectableChannel), interestSet, obj, (ISelectionKey**)&key);
        mKeyList.PushBack(key);
    }
    else {
        Boolean isValid = FALSE;
        key->IsValid(&isValid);
        if (!isValid) {
            return E_CANCELLED_KEY_EXCEPTION;
        }
        key->InterestOpsEx(interestSet, NULL);
        key->Attach(obj, NULL);
    }

    *result = key;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode AbstractSelectableChannel::IsBlocking(
    /* out*/ Boolean* isBlocking)
{
    Object::Autolock lock(mBlockingLock);
    *isBlocking = mIsBlocking;
    return NOERROR;
}

ECode AbstractSelectableChannel::BlockingLock(
    /* [out] */ Object** obj)
{
    VALIDATE_NOT_NULL(obj)
    *obj = &mBlockingLock;
    return NOERROR;
}

ECode AbstractSelectableChannel::ConfigureBlocking(
    /* [in] */ Boolean blockingMode,
    /* [out] */ ISelectableChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    *channel = NULL;

    Boolean isOpen = FALSE;
    IsOpen(&isOpen);
    if (!isOpen) {
        return E_CLOSED_CHANNEL_EXCEPTION;
    }

    Object::Autolock lock(mBlockingLock);

    if (mIsBlocking == blockingMode) {
        *channel = THIS_PROBE(ISelectableChannel);
        INTERFACE_ADDREF(*channel);
        return NOERROR;
    }

    Boolean contains = ContainsValidKeys();
    if (blockingMode && contains) {
        return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
    }

    ImplConfigureBlocking(blockingMode);

    *channel = THIS_PROBE(ISelectableChannel);
    INTERFACE_ADDREF(*channel);
    return NOERROR;
}

ECode AbstractSelectableChannel::Deregister(
   /* [in] */ ISelectionKey* key)
{
    Object::Autolock lock(mBlockingLock);
    mKeyList.Remove(key);
    return NOERROR;
}

ECode AbstractSelectableChannel::ImplCloseChannel()
{
    Object::Autolock lock(mBlockingLock);

    ImplCloseSelectableChannel();

    ISelectionKey* key;
    List<AutoPtr<ISelectionKey> >::Iterator it = mKeyList.Begin();
    for (; it != mKeyList.End(); ++it) {
        key = *it;
        if (key != NULL) {
            key->Cancel();
        }
    }
    return NOERROR;
}

Boolean AbstractSelectableChannel::ContainsValidKeys()
{
    Object::Autolock lock(mBlockingLock);

    Boolean bval;
    ISelectionKey* key;
    List<AutoPtr<ISelectionKey> >::Iterator it = mKeyList.Begin();
    for (; it != mKeyList.End(); ++it) {
        key = *it;
        if (key != NULL && (key->IsValid(&bval), bval)) {
            return TRUE;
        }
    }

    return FALSE;
}

} // namespace Spi
} // namespace Channels
} // namespace IO
} // namespace Elastos
