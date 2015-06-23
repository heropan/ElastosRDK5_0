
#include "app/CBackStackState.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::IO::CPrintWriter;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::App::IFragmentTransaction;

namespace Elastos {
namespace Droid {
namespace App {

CBackStackState::CBackStackState()
    : mTransition(0)
    , mTransitionStyle(0)
    , mIndex(0)
    , mBreadCrumbTitleRes(0)
    , mBreadCrumbShortTitleRes(0)
{}

ECode CBackStackState::constructor()
{
    return NOERROR;
}

ECode CBackStackState::constructor(
    /* [in] */ IFragmentManagerImpl* fm,
    /* [in] */ IBackStackRecord* bse)
{
    Int32 numRemoved = 0;
    AutoPtr<BackStackRecord> bseObj = (BackStackRecord*)bse;
    AutoPtr<BackStackRecord::Op> op = bseObj->mHead;
    while (op != NULL) {
        numRemoved += op->mRemoved.GetSize();
        op = op->mNext;
    }
    mOps = ArrayOf<Int32>::Alloc(bseObj->mNumOp * 7 + numRemoved);

    if (!bseObj->mAddToBackStack) {
//        throw new IllegalStateException("Not on back stack");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    op = bseObj->mHead;
    Int32 pos = 0;
    while (op != NULL) {
        Int32 index = -1;
        if (op->mFragment != NULL) op->mFragment->GetIndex(&index);
        (*mOps)[pos++] = op->mCmd;
        (*mOps)[pos++] = index;
        (*mOps)[pos++] = op->mEnterAnim;
        (*mOps)[pos++] = op->mExitAnim;
        (*mOps)[pos++] = op->mPopEnterAnim;
        (*mOps)[pos++] = op->mPopExitAnim;
        (*mOps)[pos++] = op->mRemoved.GetSize();
        List< AutoPtr<IFragment> >::Iterator it;
        for (it = op->mRemoved.Begin(); it != op->mRemoved.End(); ++it) {
            (*it)->GetIndex(&index);
            (*mOps)[pos++] = index;
        }
        op = op->mNext;
    }
    mTransition = bseObj->mTransition;
    mTransitionStyle = bseObj->mTransitionStyle;
    mName = bseObj->mName;
    mIndex = bseObj->mIndex;
    mBreadCrumbTitleRes = bseObj->mBreadCrumbTitleRes;
    mBreadCrumbTitleText = bseObj->mBreadCrumbTitleText;
    mBreadCrumbShortTitleRes = bseObj->mBreadCrumbShortTitleRes;
    mBreadCrumbShortTitleText = bseObj->mBreadCrumbShortTitleText;
    return NOERROR;
}

ECode CBackStackState::Instantiate(
    /* [in] */ IFragmentManagerImpl* fm,
    /* [out] */ IBackStackRecord** record)
{
    VALIDATE_NOT_NULL(record);

    AutoPtr<CFragmentManagerImpl> fmObj = (CFragmentManagerImpl*)fm;
    AutoPtr<BackStackRecord> bse = new BackStackRecord(fm);
    Int32 pos = 0;
    Int32 num = 0;
    while (pos < mOps->GetLength()) {
        AutoPtr<BackStackRecord::Op> op = new BackStackRecord::Op();
        op->mCmd = (*mOps)[pos++];
        if (CFragmentManagerImpl::DEBUG) {
            Logger::V(CFragmentManagerImpl::TAG,
                "Instantiate %p op #%d base fragment #%d", bse.Get(), num, (*mOps)[pos]);
        }
        Int32 findex = (*mOps)[pos++];
        if (findex >= 0) {
            AutoPtr<IFragment> f = fmObj->mActive[findex];
            op->mFragment = f;
        }
        else {
            op->mFragment = NULL;
        }
        op->mEnterAnim = (*mOps)[pos++];
        op->mExitAnim = (*mOps)[pos++];
        op->mPopEnterAnim = (*mOps)[pos++];
        op->mPopExitAnim = (*mOps)[pos++];
        Int32 N = (*mOps)[pos++];
        if (N > 0) {
            for (Int32 i = 0; i < N; i++) {
                if (CFragmentManagerImpl::DEBUG) {
                    Logger::V(CFragmentManagerImpl::TAG,
                        "Instantiate %p set remove fragment #%d", bse.Get(), (*mOps)[pos]);
                }
                AutoPtr<IFragment> r = fmObj->mActive[(*mOps)[pos++]];
                op->mRemoved.PushBack(r);
            }
        }
        bse->AddOp(op);
        num++;
    }
    bse->mTransition = mTransition;
    bse->mTransitionStyle = mTransitionStyle;
    bse->mName = mName;
    bse->mIndex = mIndex;
    bse->mAddToBackStack = TRUE;
    bse->mBreadCrumbTitleRes = mBreadCrumbTitleRes;
    bse->mBreadCrumbTitleText = mBreadCrumbTitleText;
    bse->mBreadCrumbShortTitleRes = mBreadCrumbShortTitleRes;
    bse->mBreadCrumbShortTitleText = mBreadCrumbShortTitleText;
    bse->BumpBackStackNesting(1);
    *record = bse;
    REFCOUNT_ADD(*record);
    return NOERROR;
}

ECode CBackStackState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteArrayOf((Handle32)mOps.Get());
    dest->WriteInt32(mTransition);
    dest->WriteInt32(mTransitionStyle);
    dest->WriteString(mName);
    dest->WriteInt32(mIndex);
    dest->WriteInt32(mBreadCrumbTitleRes);
    dest->WriteInterfacePtr(mBreadCrumbTitleText);
    dest->WriteInt32(mBreadCrumbShortTitleRes);
    dest->WriteInterfacePtr(mBreadCrumbShortTitleText);
    return NOERROR;
}

ECode CBackStackState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mOps = NULL;
    source->ReadArrayOf((Handle32*)(&mOps));
    source->ReadInt32(&mTransition);
    source->ReadInt32(&mTransitionStyle);
    source->ReadString(&mName);
    source->ReadInt32(&mIndex);
    source->ReadInt32(&mBreadCrumbTitleRes);
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    mBreadCrumbTitleText = obj != NULL ? ICharSequence::Probe(obj) : NULL;
    source->ReadInt32(&mBreadCrumbShortTitleRes);
    AutoPtr<IInterface> obj_2;
    source->ReadInterfacePtr((Handle32*)&obj_2);
    mBreadCrumbShortTitleText = obj_2 != NULL ? ICharSequence::Probe(obj_2) : NULL;
    return NOERROR;
}


const String BackStackRecord::TAG = IFragmentManagerImpl::TAG;
const Int32 BackStackRecord::OP_NULL = 0;
const Int32 BackStackRecord::OP_ADD = 1;
const Int32 BackStackRecord::OP_REPLACE = 2;
const Int32 BackStackRecord::OP_REMOVE = 3;
const Int32 BackStackRecord::OP_HIDE = 4;
const Int32 BackStackRecord::OP_SHOW = 5;
const Int32 BackStackRecord::OP_DETACH = 6;
const Int32 BackStackRecord::OP_ATTACH = 7;

BackStackRecord::BackStackRecord(
    /* [in] */ IFragmentManagerImpl* manager)
    : mManager((CFragmentManagerImpl*)manager)
    , mNumOp(0)
    , mEnterAnim(0)
    , mExitAnim(0)
    , mPopEnterAnim(0)
    , mPopExitAnim(0)
    , mTransition(0)
    , mTransitionStyle(0)
    , mAddToBackStack(FALSE)
    , mAllowAddToBackStack(TRUE)
    , mCommitted(FALSE)
    , mIndex(-1)
    , mBreadCrumbTitleRes(0)
    , mBreadCrumbShortTitleRes(0)
{}

PInterface BackStackRecord::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBackStackRecord*)this;
    }
    else if (riid == EIID_IBackStackRecord) {
        return (IBackStackRecord*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }
    else if (riid == EIID_IFragmentManagerBackStackEntry) {
        return (IFragmentManagerBackStackEntry*)this;
    }
    return NULL;
}

UInt32 BackStackRecord::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BackStackRecord::Release()
{
    return ElRefBase::Release();
}

ECode BackStackRecord::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBackStackRecord *)this) {
        *pIID = EIID_IBackStackRecord;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IRunnable *)this) {
        *pIID = EIID_IRunnable;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IFragmentManagerBackStackEntry *)this) {
        *pIID = EIID_IFragmentManagerBackStackEntry;
        return NOERROR;
    }
    return E_INVALID_ARGUMENT;
}

ECode BackStackRecord::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb("BackStackEntry{");
    sb += StringUtils::Int32ToHexString((Int32)this);
    if (mIndex >= 0) {
        sb += " #";
        sb += mIndex;
    }
    if (mName != NULL) {
        sb += " ";
        sb += mName;
    }
    sb += "}";
    *str = sb.ToString();
    return NOERROR;
}

ECode BackStackRecord::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    Dump(prefix, writer, TRUE);
    return NOERROR;
}

void BackStackRecord::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ Boolean full)
{
    if (full) {
        writer->PrintString(prefix); writer->PrintString(String("mName=")); writer->PrintString(mName);
                writer->PrintString(String(" mIndex=")); writer->PrintInt32(mIndex);
                writer->PrintString(String(" mCommitted=")); writer->PrintBooleanln(mCommitted);
        if (mTransition != IFragmentTransaction::TRANSIT_NONE) {
            writer->PrintString(prefix); writer->PrintString(String("mTransition=#"));
                    writer->PrintString(StringUtils::Int32ToHexString(mTransition));
                    writer->PrintString(String(" mTransitionStyle=#"));
                    writer->PrintStringln(StringUtils::Int32ToHexString(mTransitionStyle));
        }
        if (mEnterAnim != 0 || mExitAnim != 0) {
            writer->PrintString(prefix); writer->PrintString(String("mEnterAnim=#"));
                    writer->PrintString(StringUtils::Int32ToHexString(mEnterAnim));
                    writer->PrintString(String(" mExitAnim=#"));
                    writer->PrintStringln(StringUtils::Int32ToHexString(mExitAnim));
        }
        if (mPopEnterAnim != 0 || mPopExitAnim != 0) {
            writer->PrintString(prefix); writer->PrintString(String("mPopEnterAnim=#"));
                    writer->PrintString(StringUtils::Int32ToHexString(mPopEnterAnim));
                    writer->PrintString(String(" mPopExitAnim=#"));
                    writer->PrintStringln(StringUtils::Int32ToHexString(mPopExitAnim));
        }
        if (mBreadCrumbTitleRes != 0 || mBreadCrumbTitleText != NULL) {
            writer->PrintString(prefix); writer->PrintString(String("mBreadCrumbTitleRes=#"));
                    writer->PrintString(StringUtils::Int32ToHexString(mBreadCrumbTitleRes));
                    writer->PrintString(String(" mBreadCrumbTitleText="));
                    writer->PrintObjectln(mBreadCrumbTitleText);
        }
        if (mBreadCrumbShortTitleRes != 0 || mBreadCrumbShortTitleText != NULL) {
            writer->PrintString(prefix); writer->PrintString(String("mBreadCrumbShortTitleRes=#"));
                    writer->PrintString(StringUtils::Int32ToHexString(mBreadCrumbShortTitleRes));
                    writer->PrintString(String(" mBreadCrumbShortTitleText="));
                    writer->PrintObjectln(mBreadCrumbShortTitleText);
        }
    }

    if (mHead != NULL) {
        writer->PrintString(prefix); writer->PrintStringln(String("Operations:"));
        String innerPrefix = prefix + "    ";
        AutoPtr<Op> op = mHead;
        Int32 num = 0;
        while (op != NULL) {
            String cmdStr;
            switch (op->mCmd) {
                case OP_NULL: cmdStr="NULL"; break;
                case OP_ADD: cmdStr="ADD"; break;
                case OP_REPLACE: cmdStr="REPLACE"; break;
                case OP_REMOVE: cmdStr="REMOVE"; break;
                case OP_HIDE: cmdStr="HIDE"; break;
                case OP_SHOW: cmdStr="SHOW"; break;
                case OP_DETACH: cmdStr="DETACH"; break;
                case OP_ATTACH: cmdStr="ATTACH"; break;
                default: StringBuilder sb("cmd="); sb.AppendInt32(op->mCmd); cmdStr = sb.ToString(); break;
            }
            writer->PrintString(prefix); writer->PrintString(String("  Op #")); writer->PrintInt32(num);
                    writer->PrintString(String(": ")); writer->PrintString(cmdStr);
                    writer->PrintString(String(" ")); writer->PrintObjectln(op->mFragment);
            if (full) {
                if (op->mEnterAnim != 0 || op->mExitAnim != 0) {
                    writer->PrintString(innerPrefix); writer->PrintString(String("mEnterAnim=#"));
                            writer->PrintString(StringUtils::Int32ToHexString(op->mEnterAnim));
                            writer->PrintString(String(" mExitAnim=#"));
                            writer->PrintStringln(StringUtils::Int32ToHexString(op->mExitAnim));
                }
                if (op->mPopEnterAnim != 0 || op->mPopExitAnim != 0) {
                    writer->PrintString(innerPrefix); writer->PrintString(String("mPopEnterAnim=#"));
                            writer->PrintString(StringUtils::Int32ToHexString(op->mPopEnterAnim));
                            writer->PrintString(String(" mPopExitAnim=#"));
                            writer->PrintStringln(StringUtils::Int32ToHexString(op->mPopExitAnim));
                }
            }
            if (op->mRemoved.IsEmpty() == FALSE) {
                for (Int32 i = 0; i < op->mRemoved.GetSize(); i++) {
                    writer->PrintString(innerPrefix);
                    if (op->mRemoved.GetSize() == 1) {
                        writer->PrintString(String("mRemoved: "));
                    }
                    else {
                        if (i == 0) {
                            writer->PrintStringln(String("mRemoved:"));
                        }
                        writer->PrintString(innerPrefix); writer->PrintString(String("  #")); writer->PrintInt32(i);
                                writer->PrintString(String(": "));
                    }
                    writer->PrintObjectln(op->mRemoved[i]);
                }
            }
            op = op->mNext;
            num++;
        }
    }
}

ECode BackStackRecord::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mIndex;
    return NOERROR;
}

ECode BackStackRecord::GetBreadCrumbTitleRes(
    /* [out] */ Int32* titleRes)
{
    VALIDATE_NOT_NULL(titleRes);
    *titleRes = mBreadCrumbTitleRes;
    return NOERROR;
}

ECode BackStackRecord::GetBreadCrumbShortTitleRes(
    /* [out] */ Int32* shortTitleRes)
{
    VALIDATE_NOT_NULL(shortTitleRes);
    *shortTitleRes = mBreadCrumbShortTitleRes;
    return NOERROR;
}

ECode BackStackRecord::GetBreadCrumbTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    if (mBreadCrumbTitleRes != 0) {
        return mManager->mActivity->GetText(mBreadCrumbTitleRes, title);
    }
    *title = mBreadCrumbTitleText;
    REFCOUNT_ADD(*title);
    return NOERROR;
}

ECode BackStackRecord::GetBreadCrumbShortTitle(
    /* [out] */ ICharSequence** shortTitle)
{
    VALIDATE_NOT_NULL(shortTitle);
    if (mBreadCrumbShortTitleRes != 0) {
        return mManager->mActivity->GetText(mBreadCrumbShortTitleRes, shortTitle);
    }
    *shortTitle = mBreadCrumbShortTitleText;
    REFCOUNT_ADD(*shortTitle);
    return NOERROR;
}

void BackStackRecord::AddOp(
    /* [in] */ Op* op)
{
    if (mHead == NULL) {
        mHead = mTail = op;
    }
    else {
        op->mPrev = mTail;
        mTail->mNext = op;
        mTail = op;
    }
    op->mEnterAnim = mEnterAnim;
    op->mExitAnim = mExitAnim;
    op->mPopEnterAnim = mPopEnterAnim;
    op->mPopExitAnim = mPopExitAnim;
    mNumOp++;
}


ECode BackStackRecord::Add(
    /* [in] */ IFragment* fragment,
    /* [in] */ const String& tag)
{
    return DoAddOp(0, fragment, tag, OP_ADD);
}

ECode BackStackRecord::Add(
    /* [in] */ Int32 containerViewId,
    /* [in] */ IFragment* fragment)
{
    return DoAddOp(containerViewId, fragment, String(NULL), OP_ADD);
}

ECode BackStackRecord::Add(
    /* [in] */ Int32 containerViewId,
    /* [in] */ IFragment* fragment,
    /* [in] */ const String& tag)
{
    return DoAddOp(containerViewId, fragment, tag, OP_ADD);
}

ECode BackStackRecord::DoAddOp(
    /* [in] */ Int32 containerViewId,
    /* [in] */ IFragment* fragment,
    /* [in] */ const String& tag,
    /* [in] */ Int32 opcmd)
{
    fragment->SetFragmentManager(mManager);

    if (!tag.IsNull()) {
        String memTag;
        fragment->GetTag(&memTag);
        if (!memTag.IsNull() && !tag.Equals(memTag)) {
//            throw new IllegalStateException("Can't change tag of fragment "
//                    + fragment + ": was " + fragment.mTag
//                    + " now " + tag);
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        fragment->SetTag(tag);
    }

    if (containerViewId != 0) {
        Int32 fragmentId;
        fragment->GetId(&fragmentId);
        if (fragmentId != 0 && fragmentId != containerViewId) {
//            throw new IllegalStateException("Can't change container ID of fragment "
//                    + fragment + ": was " + fragment.mFragmentId
//                    + " now " + containerViewId);
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        fragment->SetFragmentId(containerViewId);
        fragment->SetContainerId(containerViewId);
    }

    AutoPtr<Op> op = new Op();
    op->mCmd = opcmd;
    op->mFragment = fragment;
    AddOp(op);
    return NOERROR;
}

ECode BackStackRecord::Replace(
    /* [in] */ Int32 containerViewId,
    /* [in] */ IFragment* fragment)
{
    return Replace(containerViewId, fragment, String(NULL));
}

ECode BackStackRecord::Replace(
    /* [in] */ Int32 containerViewId,
    /* [in] */ IFragment* fragment,
    /* [in] */ const String& tag)
{
    if (containerViewId == 0) {
//      throw new IllegalArgumentException("Must use non-zero containerViewId");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return DoAddOp(containerViewId, fragment, tag, OP_REPLACE);
}

ECode BackStackRecord::Remove(
    /* [in] */ IFragment* fragment)
{
    AutoPtr<Op> op = new Op();
    op->mCmd = OP_REMOVE;
    op->mFragment = fragment;
    AddOp(op);

    return NOERROR;
}

ECode BackStackRecord::Hide(
    /* [in] */ IFragment* fragment)
{
    AutoPtr<Op> op = new Op();
    op->mCmd = OP_HIDE;
    op->mFragment = fragment;
    AddOp(op);

    return NOERROR;
}

ECode BackStackRecord::Show(
    /* [in] */ IFragment* fragment)
{
    AutoPtr<Op> op = new Op();
    op->mCmd = OP_SHOW;
    op->mFragment = fragment;
    AddOp(op);

    return NOERROR;
}

ECode BackStackRecord::Detach(
    /* [in] */ IFragment* fragment)
{
    AutoPtr<Op> op = new Op();
    op->mCmd = OP_DETACH;
    op->mFragment = fragment;
    AddOp(op);

    return NOERROR;
}

ECode BackStackRecord::Attach(
    /* [in] */ IFragment* fragment)
{
    AutoPtr<Op> op = new Op();
    op->mCmd = OP_ATTACH;
    op->mFragment = fragment;
    AddOp(op);

    return NOERROR;
}

ECode BackStackRecord::SetCustomAnimations(
    /* [in] */ Int32 enter,
    /* [in] */ Int32 exit)
{
    return SetCustomAnimations(enter, exit, 0, 0);
}

ECode BackStackRecord::SetCustomAnimations(
    /* [in] */ Int32 enter,
    /* [in] */ Int32 exit,
    /* [in] */ Int32 popEnter,
    /* [in] */ Int32 popExit)
{
    mEnterAnim = enter;
    mExitAnim = exit;
    mPopEnterAnim = popEnter;
    mPopExitAnim = popExit;
    return NOERROR;
}

ECode BackStackRecord::SetTransition(
    /* [in] */ Int32 transition)
{
    mTransition = transition;
    return NOERROR;
}

ECode BackStackRecord::SetTransitionStyle(
    /* [in] */ Int32 styleRes)
{
    mTransitionStyle = styleRes;
    return NOERROR;
}

ECode BackStackRecord::AddToBackStack(
    /* [in] */ const String& name)
{
    if (!mAllowAddToBackStack) {
        // throw new IllegalStateException(
        //         "This FragmentTransaction is not allowed to be added to the back stack.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mAddToBackStack = TRUE;
    mName = name;
    return NOERROR;
}

ECode BackStackRecord::IsAddToBackStackAllowed(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed);
    *allowed = mAllowAddToBackStack;
    return NOERROR;
}

ECode BackStackRecord::DisallowAddToBackStack()
{
    if (mAddToBackStack) {
        // throw new IllegalStateException(
        //         "This transaction is already being added to the back stack");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mAllowAddToBackStack = FALSE;
    return NOERROR;
}

ECode BackStackRecord::SetBreadCrumbTitle(
    /* [in] */ Int32 res)
{
    mBreadCrumbTitleRes = res;
    mBreadCrumbTitleText = NULL;
    return NOERROR;
}

ECode BackStackRecord::SetBreadCrumbTitle(
    /* [in] */ ICharSequence* text)
{
    mBreadCrumbTitleRes = 0;
    mBreadCrumbTitleText = text;
    return NOERROR;
}

ECode BackStackRecord::SetBreadCrumbShortTitle(
    /* [in] */ Int32 res)
{
    mBreadCrumbShortTitleRes = res;
    mBreadCrumbShortTitleText = NULL;
    return NOERROR;
}

ECode BackStackRecord::SetBreadCrumbShortTitle(
    /* [in] */ ICharSequence* text)
{
    mBreadCrumbShortTitleRes = 0;
    mBreadCrumbShortTitleText = text;
    return NOERROR;
}

void BackStackRecord::BumpBackStackNesting(
    /* [in] */ Int32 amt)
{
    if (!mAddToBackStack) {
        return;
    }
    if (CFragmentManagerImpl::DEBUG) {
        Logger::V(TAG, "Bump nesting in %p by %d", this, amt);
    }
    AutoPtr<Op> op = mHead;
    while (op != NULL) {
        if (op->mFragment != NULL) {
            Int32 newNesting;
            op->mFragment->GetBackStackNesting(&newNesting);
            newNesting += amt;
            op->mFragment->SetBackStackNesting(newNesting);
            if (CFragmentManagerImpl::DEBUG) {
                Logger::V(TAG, "Bump nesting of %p to %d", op->mFragment.Get(), newNesting);
            }
        }
        List< AutoPtr<IFragment> >::ReverseIterator rit;
        for (rit = op->mRemoved.RBegin(); rit != op->mRemoved.REnd(); ++rit) {
            AutoPtr<IFragment> r = *rit;
            Int32 newNesting;
            r->GetBackStackNesting(&newNesting);
            newNesting += amt;
            r->SetBackStackNesting(newNesting);
            if (CFragmentManagerImpl::DEBUG) {
                Logger::V(TAG, "Bump nesting of %p to %d", r.Get(), newNesting);
            }
        }
        op = op->mNext;
    }
}

ECode BackStackRecord::Commit(
    /* [out] */ Int32* identifier)
{
    VALIDATE_NOT_NULL(identifier);
    *identifier = CommitInternal(FALSE);
    return NOERROR;
}

ECode BackStackRecord::CommitAllowingStateLoss(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res);
    *res = CommitInternal(TRUE);
    return NOERROR;
}

Int32 BackStackRecord::CommitInternal(
    /* [in] */ Boolean allowStateLoss)
{
    if (mCommitted) return E_ILLEGAL_STATE_EXCEPTION;//throw new IllegalStateException("commit already called");

    if (CFragmentManagerImpl::DEBUG) {
        Logger::V(TAG, "Commit: %p", this);
//        LogWriter logw/* = new LogWriter(Logger::VERBOSE, TAG)*/;
//        AutoPtr<IPrintWriter> pw;
//        CPrintWriter::New(logw, (IPrintWriter**)&pw);
//        Dump(String(NULL), NULL, pw, NULL);
    }

    mCommitted = TRUE;
    if (mAddToBackStack) {
        mManager->AllocBackStackIndex(THIS_PROBE(IBackStackRecord), &mIndex);
    }
    else {
        mIndex = -1;
    }
    mManager->EnqueueAction((IRunnable*)this->Probe(EIID_IRunnable), allowStateLoss);
    return mIndex;
}

ECode BackStackRecord::Run()
{
    if (CFragmentManagerImpl::DEBUG) Logger::V(TAG, "Run: %p", this);

    if (mAddToBackStack) {
        if (mIndex < 0) {
            //throw new IllegalStateException("addToBackStack() called after commit()");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }

    BumpBackStackNesting(1);

    AutoPtr<Op> op = mHead;
    while (op != NULL) {
        switch (op->mCmd) {
            case OP_ADD: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mEnterAnim);
                mManager->AddFragment(f, FALSE);
            } break;
            case OP_REPLACE: {
                AutoPtr<IFragment> f = op->mFragment;
                List<AutoPtr<IFragment> >::Iterator it;
                for (it = mManager->mAdded.Begin(); it != mManager->mAdded.End(); ++it) {
                    AutoPtr<IFragment> old = *it;
                    if (CFragmentManagerImpl::DEBUG) {
                        Logger::V(TAG, "OP_REPLACE: adding=%p old=%p", f.Get(), old.Get());
                    }
                    Int32 oldId, fId;
                    if (f == NULL || (old->GetContainerId(&oldId), oldId) == (f->GetContainerId(&fId), fId)) {
                        if (old == f) {
                            op->mFragment = f = NULL;
                        }
                        else {
                            op->mRemoved.PushBack(old);
                            old->SetNextAnim(op->mExitAnim);
                            if (mAddToBackStack) {
                                Int32 oldNesting;
                                old->GetBackStackNesting(&oldNesting);
                                oldNesting += 1;
                                old->SetBackStackNesting(oldNesting);
                                if (CFragmentManagerImpl::DEBUG) {
                                    Logger::V(TAG, "Bump nesting of %p to %d", old.Get(), oldNesting);
                                }
                            }
                            mManager->RemoveFragment(old, mTransition, mTransitionStyle);
                        }
                    }
                }
                if (f != NULL) {
                    f->SetNextAnim(op->mEnterAnim);
                    mManager->AddFragment(f, FALSE);
                }
            } break;
            case OP_REMOVE: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mExitAnim);
                mManager->RemoveFragment(f, mTransition, mTransitionStyle);
            } break;
            case OP_HIDE: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mExitAnim);
                mManager->HideFragment(f, mTransition, mTransitionStyle);
            } break;
            case OP_SHOW: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mEnterAnim);
                mManager->ShowFragment(f, mTransition, mTransitionStyle);
            } break;
            case OP_DETACH: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mExitAnim);
                mManager->DetachFragment(f, mTransition, mTransitionStyle);
            } break;
            case OP_ATTACH: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mEnterAnim);
                mManager->AttachFragment(f, mTransition, mTransitionStyle);
            } break;
            default: {
                //throw new IllegalArgumentException("Unknown cmd: " + op.cmd);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }

        op = op->mNext;
    }

    mManager->MoveToState(mManager->mCurState, mTransition,
            mTransitionStyle, TRUE);

    if (mAddToBackStack) {
        mManager->AddBackStackState(THIS_PROBE(IBackStackRecord));
    }

    return NOERROR;
}

ECode BackStackRecord::PopFromBackStack(
    /* [in] */ Boolean doStateMove)
{
    if (CFragmentManagerImpl::DEBUG) {
        Logger::V(TAG, "PopFromBackStack: %p", this);
//        LogWriter logw/* = new LogWriter(Logger::VERBOSE, TAG)*/;
       //  AutoPtr<IPrintWriter> pw;
       //  CPrintWriter::New(logw, (IPrintWriter**)&pw);
       //  Dump(String("  "), NULL, pw, NULL);
    }

    BumpBackStackNesting(-1);

    AutoPtr<Op> op = mTail;
    while (op != NULL) {
        switch (op->mCmd) {
            case OP_ADD: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mPopExitAnim);
                Int32 newTransition;
                CFragmentManagerImpl::ReverseTransit(mTransition, &newTransition);
                mManager->RemoveFragment(f,
                        newTransition, mTransitionStyle);
            } break;
            case OP_REPLACE: {
                AutoPtr<IFragment> f = op->mFragment;
                if (f != NULL) {
                    f->SetNextAnim(op->mPopExitAnim);
                    Int32 newTransition;
                    CFragmentManagerImpl::ReverseTransit(mTransition, &newTransition);
                    mManager->RemoveFragment(f,
                            newTransition, mTransitionStyle);
                }
                List< AutoPtr<IFragment> >::Iterator it;
                for (it = op->mRemoved.Begin(); it != op->mRemoved.End(); ++it) {
                    AutoPtr<IFragment> old = *it;
                    old->SetNextAnim(op->mPopEnterAnim);
                    mManager->AddFragment(old, FALSE);
                }
            } break;
            case OP_REMOVE: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mPopEnterAnim);
                mManager->AddFragment(f, FALSE);
            } break;
            case OP_HIDE: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mPopEnterAnim);
                Int32 newTransition;
                CFragmentManagerImpl::ReverseTransit(mTransition, &newTransition);
                mManager->ShowFragment(f, newTransition, mTransitionStyle);
            } break;
            case OP_SHOW: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mPopExitAnim);
                Int32 newTransition;
                CFragmentManagerImpl::ReverseTransit(mTransition, &newTransition);
                mManager->HideFragment(f, newTransition, mTransitionStyle);
            } break;
            case OP_DETACH: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mPopEnterAnim);
                Int32 newTransition;
                CFragmentManagerImpl::ReverseTransit(mTransition, &newTransition);
                mManager->AttachFragment(f, newTransition, mTransitionStyle);
            } break;
            case OP_ATTACH: {
                AutoPtr<IFragment> f = op->mFragment;
                f->SetNextAnim(op->mPopExitAnim);
                Int32 newTransition;
                CFragmentManagerImpl::ReverseTransit(mTransition, &newTransition);
                mManager->DetachFragment(f, newTransition, mTransitionStyle);
            } break;
            default: {
                //throw new IllegalArgumentException("Unknown cmd: " + op.cmd);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }

        op = op->mPrev;
    }

    if (doStateMove) {
        Int32 newTransition;
        CFragmentManagerImpl::ReverseTransit(mTransition, &newTransition);
        mManager->MoveToState(mManager->mCurState, newTransition, mTransitionStyle, TRUE);
    }

    if (mIndex >= 0) {
        mManager->FreeBackStackIndex(mIndex);
        mIndex = -1;
    }
    return NOERROR;
}

ECode BackStackRecord::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode BackStackRecord::GetTransition(
    /* [out] */ Int32* trans)
{
    VALIDATE_NOT_NULL(trans);
    *trans = mTransition;
    return NOERROR;
}

ECode BackStackRecord::GetTransitionStyle(
    /* [out] */ Int32* transStyle)
{
    VALIDATE_NOT_NULL(transStyle);
    *transStyle = mTransitionStyle;
    return NOERROR;
}

ECode BackStackRecord::IsEmpty(
    /* [out] */ Boolean* empty)
{
    VALIDATE_NOT_NULL(empty);
    *empty = mNumOp == 0;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
