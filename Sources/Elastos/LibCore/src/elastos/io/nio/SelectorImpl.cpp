
#include "SelectorImpl.h"
// #include "CLibcore.h"
//#include "IoUtils.h"
#include "CStructPollfd.h"
// #include "COsConstants.h"
// #include "CIoBridge.h"
#include "SelectionKeyImpl.h"
#include "Autolock.h"

using Libcore::IO::IOs;
using Libcore::IO::ILibcore;
// using Libcore::IO::CLibcore;
using Elastos::Droid::System::IStructPollfd;
using Elastos::Droid::System::CStructPollfd;
// using Libcore::IO::COsConstants;
// using Libcore::IO::CIoBridge;
using Elastos::IO::Channels::EIID_ISelector;
using Elastos::IO::Channels::Spi::AbstractSelectionKey;
using Elastos::IO::Channels::Spi::EIID_IAbstractSelector;
using Elastos::Utility::ICollection;
using Elastos::Utility::IList;

namespace Elastos {
namespace IO {

ECode SelectorImpl::constructor()
{
    return NOERROR;
}

ECode SelectorImpl::constructor(
    /* [in] */ ISelectorProvider* selectorProvider)
{
    AbstractSelector::constructor(selectorProvider);
    ECode result = NOERROR;
    AutoPtr<ArrayOf<Int32> > fds;
    assert(0 && "TODO");
    // result = CLibcore::sOs->Pipe((ArrayOf<Int32>**)&fds);
    assert(result == NOERROR);

    mWakeupIn->SetDescriptor((*fds)[0]);
    mWakeupOut->SetDescriptor((*fds)[1]);
    assert(0 && "TODO");
    // result = IoUtils::SetBlocking(mWakeupIn, FALSE);
    assert(result == NOERROR);

    AutoPtr<IStructPollfd> structfd;
    CStructPollfd::New((IStructPollfd**)&structfd);
    Boolean isflag = FALSE;
    ICollection::Probe(mPollFds)->Add(structfd, &isflag);
    assert(0 && "TODO");
    // SetPollFd(0, mWakeupIn, COsConstants::sPOLLIN, NULL);
    return NOERROR;
}

ECode SelectorImpl::Cancel(
    /* [in] */ ISelectionKey* key)
{
    return E_NOT_IMPLEMENTED;
}

ECode SelectorImpl::Close()
{
    return AbstractSelector::Close();
}

ECode SelectorImpl::IsOpen(
    /* [out] */ Boolean* isOpen)
{
    return AbstractSelector::IsOpen(isOpen);
}

ECode SelectorImpl::GetProvider(
    /* [out] */ ISelectorProvider** provider)
{
    return AbstractSelector::GetProvider(provider);
}

ECode SelectorImpl::ImplCloseSelector()
{
    Wakeup();

    Autolock lock(mLock);
    assert(0 && "TODO");
    // FAIL_RETURN(IoUtils::Close(mWakeupIn));
    // FAIL_RETURN(IoUtils::Close(mWakeupOut));
    DoCancel();

    AutoPtr< ArrayOf<IInterface*> > outarr;
    ICollection::Probe(mMutableKeys)->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        assert(0 && "TODO");
        // AutoPtr<AbstractSelectionKey> ask = (AbstractSelectionKey*)(*outarr)[i];
        // Deregister(ask);
    }
    return NOERROR;
}

ECode SelectorImpl::Register(
    /* [in] */ IAbstractSelectableChannel* asc,
    /* [in] */ Int32 operations,
    /* [in] */ IObject* attachment,
    /* [out] */ ISelectionKey** key)
{
    VALIDATE_NOT_NULL(key)
    *key = NULL;
    VALIDATE_NOT_NULL(asc)

    AbstractSelectableChannel* channel = (AbstractSelectableChannel*)asc;

    AutoPtr<ISelectorProvider> sp1;
    GetProvider((ISelectorProvider**)&sp1);
    AutoPtr<ISelectorProvider> sp2;
    channel->GetProvider((ISelectorProvider**)&sp2);
    if (!Object::Equals(sp1,sp2)) {
        // throw new IllegalSelectorException();
        return E_ILLEGAL_SELECTOR_EXCEPTION;
    }
    Autolock lock(mLock);
    assert(0 && "TODO");
    // SelectionKeyImpl selectionKey = new SelectionKeyImpl(channel, operations,
    //         attachment, this);
    // mutableKeys.add(selectionKey);
    // ensurePollFdsCapacity();
    // return selectionKey;
    return E_NOT_IMPLEMENTED;
}

ECode SelectorImpl::GetKeys(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    FAIL_RETURN(CheckClosed());
    *keySet = mUnmodifiableKeys;
    REFCOUNT_ADD(*keySet);
    return NOERROR;
}

ECode SelectorImpl::CheckClosed()
{
    Boolean ret;
    IsOpen(&ret);
    if(!ret) {
        return E_CLOSED_SELECTOR_EXCEPTION;
    }
    return NOERROR;
}

ECode SelectorImpl::Select(
    /* [out] */ Int32* ret)
{
    return SelectInternal(-1, ret);
}

ECode SelectorImpl::Select(
    /* [in] */ Int64 timeout,
    /* [out] */ Int32* ret)
{
    if(timeout < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return SelectInternal((timeout == 0) ? -1 : timeout, ret);
}

ECode SelectorImpl::SelectNow(
    /* [out] */ Int32* ret)
{
    return SelectInternal(0, ret);
}

ECode SelectorImpl::SelectInternal(
    /* [in] */ Int64 timeout,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)

    FAIL_RETURN(CheckClosed());
    Autolock lock(mLock);
    DoCancel();
    Boolean isBlock = (timeout != 0);
    PreparePollFds();
    Int32 rc = -1;
    if(isBlock) {
        assert(0 && "TODO");
        // Begin();
    }
    AutoPtr< ArrayOf<IInterface*> > outarr;
    ICollection::Probe(mPollFds)->ToArray((ArrayOf<IInterface*>**)&outarr);
    AutoPtr<ArrayOf<IStructPollfd*> > array = ArrayOf<IStructPollfd*>::Alloc(outarr->GetLength());
    for (Int32 i = 0; i < outarr->GetLength(); ++i) {
        AutoPtr<IStructPollfd> it = IStructPollfd::Probe((*outarr)[i]);
        array->Set(i, it);
    }
    assert(0 && "TODO");
    // FAIL_RETURN(CLibcore::sOs->Poll(*array, (Int32)timeout, &rc));
    if (isBlock) {
        assert(0 && "TODO");
        // End();
    }
    Int32 readyCount = (rc > 0) ? ProcessPollFds() : 0;
    readyCount -= DoCancel();
    *ret = readyCount;
    return NOERROR;
}

void SelectorImpl::SetPollFd(
    /* [in] */ Int32 index,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 events,
    /* [in] */ IInterface* object)
{
    AutoPtr<IInterface> outface;
    IList::Probe(mPollFds)->Get(index, (IInterface**)&outface);
    AutoPtr<IStructPollfd> pollFd = IStructPollfd::Probe(outface);
    if (pollFd != NULL) {
        pollFd->SetFd(fd);
        pollFd->SetEvents(events);
        pollFd->SetUserData(object);
    }
}

void SelectorImpl::PreparePollFds()
{
    AutoPtr< ArrayOf<IInterface*> > outarr;
    ICollection::Probe(mMutableKeys)->ToArray((ArrayOf<IInterface*>**)&outarr);
    assert(0 && "TODO");
    // if (pollFds.get(0).revents == POLLIN) {
    //     // Read bytes from the wakeup pipe until the pipe is empty.
    //     byte[] buffer = new byte[8];
    //     while (IoBridge.read(wakeupIn, buffer, 0, 1) > 0) {
    //     }
    // }

    for (Int32 i = 1; i < outarr->GetLength(); i++) {
        AutoPtr<ISelectionKey> key = ISelectionKey::Probe((*outarr)[i]);
        if (key != NULL) {
            Int32 interestOps = 0;
            assert(0 && "TODO");
            // key->InterestOpsNoCheck(&interestOps);
            Int16 eventMask = 0;
            assert(0 && "TODO");
            // if (((ISelectionKey::OP_ACCEPT | ISelectionKey::OP_READ) & interestOps) != 0) {
            //     eventMask |= COsConstants::sPOLLIN;
            // }
            // if (((ISelectionKey::OP_CONNECT | ISelectionKey::OP_WRITE) & interestOps) != 0) {
            //     eventMask |= COsConstants::sPOLLOUT;
            // }
            if (eventMask != 0) {
                AutoPtr<ISelectableChannel> sc;
                ISelectionKey::Probe(key)->GetChannel((ISelectableChannel**)&sc);
                AutoPtr<IFileDescriptor> outfd;
                if (IFileDescriptorChannel::Probe(sc) != NULL) {
                    IFileDescriptorChannel::Probe(sc)->GetFD((IFileDescriptor**)&outfd);
                }
                SetPollFd(i, outfd, eventMask, key);
            }
        }
    }
}

void SelectorImpl::EnsurePollFdsCapacity()
{
    // We need one slot for each element of mutableKeys, plus one for the wakeup pipe.
    Int32 fdsvalue = 0;
    Int32 keysvalue = 0;
    ICollection::Probe(mPollFds)->GetSize(&fdsvalue);
    ICollection::Probe(mMutableKeys)->GetSize(&keysvalue);
    while (fdsvalue < keysvalue + 1) {
        AutoPtr<IStructPollfd> res;
        CStructPollfd::New((IStructPollfd**)&res);
        Boolean isflag = FALSE;
        ICollection::Probe(mPollFds)->Add(res, &isflag);
        ICollection::Probe(mPollFds)->GetSize(&fdsvalue);
        ICollection::Probe(mMutableKeys)->GetSize(&keysvalue);
    }
}

Int32 SelectorImpl::ProcessPollFds()
{
    AutoPtr< ArrayOf<IInterface*> > outarr;
    ICollection::Probe(mPollFds)->ToArray((ArrayOf<IInterface*>**)&outarr);
    Int16 revents = 0;
    if (IStructPollfd::Probe((*outarr)[0]) != NULL) {
        IStructPollfd::Probe((*outarr)[0])->GetRevents(&revents);
    }
    assert(0 && "TODO");
    // if (revents == COsConstants::sPOLLIN) {
    //     // Read bytes from the wakeup pipe until the pipe is empty.
    //     AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(8);
    //     Int32 readnum = 0;
    //     Int32 infd = 0;
    //     mWakeupIn->GetDescriptor(&infd);
    //     while (CIoBridge::_Read(infd, buffer, 0, 1, &readnum), readnum > 0) {
    //     }
    // }

    Int32 readyKeyCount = 0;
    AutoPtr<IFileDescriptor> fdvalue = 0;
    for (Int32 i = 1; i < outarr->GetLength(); ++i) {
        AutoPtr<IStructPollfd> pollFd = IStructPollfd::Probe((*outarr)[i]);
        if (pollFd != NULL) {
            pollFd->GetRevents(&revents);
            pollFd->GetFd((IFileDescriptor**)&fdvalue);
            if (revents == 0) {
                continue;
            }
            if (fdvalue == 0) {
                break;
            }

            AutoPtr<IInterface> outface;
            pollFd->GetUserData((IInterface**)&outface);
            AutoPtr<ISelectionKey> key = ISelectionKey::Probe(outface);

            pollFd->SetFd(0);
            pollFd->SetUserData(NULL);

            Int32 ops = 0;
            // key->InterestOpsNoCheck(&ops);
            Int32 selectedOp = 0;
            Boolean isflag = FALSE;
            assert(0 && "TODO");
            // if ((revents & COsConstants::sPOLLIN) != 0) {
            //     selectedOp = ops & (ISelectionKey::OP_ACCEPT | ISelectionKey::OP_READ);
            // }
            // else if ((revents & COsConstants::sPOLLOUT) != 0) {

            //     if (key->IsConnectable(&isflag), isflag) {
            //         selectedOp = ops & ISelectionKey::OP_WRITE;
            //     }
            //     else {
            //         selectedOp = ops & ISelectionKey::OP_CONNECT;
            //     }
            // }

            if (selectedOp != 0) {
                Boolean wasSelected = 0;
                ICollection::Probe(mMutableSelectedKeys)->Contains(key, &wasSelected);
                Int32 opsvalue = 0;
                ISelectionKey::Probe(key)->GetReadyOps(&opsvalue);
                if (wasSelected && opsvalue != selectedOp) {
                    assert(0 && "TODO");
                    // key->SetReadyOps(opsvalue | selectedOp);
                    ++readyKeyCount;
                }
                else if (!wasSelected) {
                    assert(0 && "TODO");
                    // key->SetReadyOps(selectedOp);
                    ICollection::Probe(mMutableSelectedKeys)->Add(key, &isflag);
                    ++readyKeyCount;
                }
            }
        }
    }

    return readyKeyCount;
}

ECode SelectorImpl::SelectedKeys(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    FAIL_RETURN(CheckClosed());
    *keySet = mSelectedKeys;
    REFCOUNT_ADD(*keySet);
    return NOERROR;
}

Int32 SelectorImpl::DoCancel()
{
    Int32 deselected = 0;
    AutoPtr<ISet> cancelledKeys;
    CancelledKeys((ISet**)&cancelledKeys);
    Int32 sizelen = 0;
    ICollection::Probe(cancelledKeys)->GetSize(&sizelen);
    if (sizelen > 0) {
        AutoPtr< ArrayOf<IInterface*> > outarr;
        ICollection::Probe(cancelledKeys)->ToArray((ArrayOf<IInterface*>**)&outarr);
        for (Int32 i = 0; i < outarr->GetLength(); i++) {
            AutoPtr<IInterface> currentKey = (*outarr)[i];
            Boolean isflag = FALSE;
            ICollection::Probe(mMutableKeys)->Remove(currentKey, &isflag);
            assert(0 && "TODO");
            // Deregister((AbstractSelectionKey*)currentKey.Get());
            if (ICollection::Probe(mMutableSelectedKeys)->Remove(currentKey, &isflag), isflag) {
                deselected++;
            }
        }
        ICollection::Probe(cancelledKeys)->Clear();
    }

    return deselected;
}

ECode SelectorImpl::Wakeup()
{
    Int32 fd, nWrite;
    mWakeupOut->GetDescriptor(&fd);
    AutoPtr< ArrayOf<Byte> > inbyte = ArrayOf<Byte>::Alloc(1);
    (*inbyte)[0] = 1;
    assert(0 && "TODO");
    // FAIL_RETURN(CLibcore::sOs->Write(fd, *inbyte, 0, 1, &nWrite));

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
