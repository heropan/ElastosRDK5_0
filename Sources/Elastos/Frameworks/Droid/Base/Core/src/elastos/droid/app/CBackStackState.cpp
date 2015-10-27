
#include "elastos/droid/app/CBackStackState.h"
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
    mSharedElementSourceNames = bseObj->mSharedElementSourceNames;
    mSharedElementTargetNames = bseObj->mSharedElementTargetNames;
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
    bse->mSharedElementSourceNames = mSharedElementSourceNames;
    bse->mSharedElementTargetNames = mSharedElementTargetNames;
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
    mSharedElementSourceNames = source.createStringArrayList();
    mSharedElementTargetNames = source.createStringArrayList();
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
                default: StringBuilder sb("cmd="); sb.Append(op->mCmd); cmdStr = sb.ToString(); break;
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

//@Override
ECode BackStackRecord::AddSharedElement(
    /* [in] */ IView* sharedElement,
    /* [in] */ const String& name)
{
    String transitionName = sharedElement.getTransitionName();
    if (transitionName == null) {
        throw new IllegalArgumentException("Unique transitionNames are required for all" +
                " sharedElements");
    }
    if (mSharedElementSourceNames == null) {
        mSharedElementSourceNames = new ArrayList<String>();
        mSharedElementTargetNames = new ArrayList<String>();
    }
    mSharedElementSourceNames.add(transitionName);
    mSharedElementTargetNames.add(name);
    return this;
}

/** TODO: remove this */
//@Override
ECode BackStackRecord::SetSharedElement(
    /* [in] */ IView* sharedElement,
    /* [in] */ const String& name)
{
    String transitionName = sharedElement.getTransitionName();
    if (transitionName == null) {
        throw new IllegalArgumentException("Unique transitionNames are required for all" +
                " sharedElements");
    }
    mSharedElementSourceNames = new ArrayList<String>(1);
    mSharedElementSourceNames.add(transitionName);

    mSharedElementTargetNames = new ArrayList<String>(1);
    mSharedElementTargetNames.add(name);
    return this;
}

/** TODO: remove this */
//@Override
ECode BackStackRecord::SetSharedElements(
    /* Pair<View, String>... sharedElements*/
    ArrayOf<IPair*>* sharedElements)
{
    if (sharedElements == null || sharedElements.length == 0) {
        mSharedElementSourceNames = null;
        mSharedElementTargetNames = null;
    } else {
        ArrayList<String> sourceNames = new ArrayList<String>(sharedElements.length);
        ArrayList<String> targetNames = new ArrayList<String>(sharedElements.length);
        for (int i = 0; i < sharedElements.length; i++) {
            String transitionName = sharedElements[i].first.getTransitionName();
            if (transitionName == null) {
                throw new IllegalArgumentException("Unique transitionNames are required for all"
                        + " sharedElements");
            }
            sourceNames.add(transitionName);
            targetNames.add(sharedElements[i].second);
        }
        mSharedElementSourceNames = sourceNames;
        mSharedElementTargetNames = targetNames;
    }
    return this;
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
//        FastPrintWriter::New(logw, FALSE, 1024, (IPrintWriter**)&pw);
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

    SparseArray<Fragment> firstOutFragments = new SparseArray<Fragment>();
    SparseArray<Fragment> lastInFragments = new SparseArray<Fragment>();
    calculateFragments(firstOutFragments, lastInFragments);
    beginTransition(firstOutFragments, lastInFragments, false);

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

//=======================

private static void setFirstOut(SparseArray<Fragment> fragments, Fragment fragment) {
    if (fragment != null) {
        int containerId = fragment.mContainerId;
        if (containerId != 0 && !fragment.isHidden() && fragment.isAdded() &&
                fragment.getView() != null && fragments.get(containerId) == null) {
            fragments.put(containerId, fragment);
        }
    }
}

private void setLastIn(SparseArray<Fragment> fragments, Fragment fragment) {
    if (fragment != null) {
        int containerId = fragment.mContainerId;
        if (containerId != 0) {
            fragments.put(containerId, fragment);
        }
    }
}

/**
 * Finds the first removed fragment and last added fragments when going forward.
 * If none of the fragments have transitions, then both lists will be empty.
 *
 * @param firstOutFragments The list of first fragments to be removed, keyed on the
 *                          container ID. This list will be modified by the method.
 * @param lastInFragments The list of last fragments to be added, keyed on the
 *                        container ID. This list will be modified by the method.
 */
private void calculateFragments(SparseArray<Fragment> firstOutFragments,
        SparseArray<Fragment> lastInFragments) {
    if (!mManager.mContainer.hasView()) {
        return; // nothing to see, so no transitions
    }
    Op op = mHead;
    while (op != null) {
        switch (op.cmd) {
            case OP_ADD:
                setLastIn(lastInFragments, op.fragment);
                break;
            case OP_REPLACE: {
                Fragment f = op.fragment;
                if (mManager.mAdded != null) {
                    for (int i = 0; i < mManager.mAdded.size(); i++) {
                        Fragment old = mManager.mAdded.get(i);
                        if (f == null || old.mContainerId == f.mContainerId) {
                            if (old == f) {
                                f = null;
                            } else {
                                setFirstOut(firstOutFragments, old);
                            }
                        }
                    }
                }
                setLastIn(lastInFragments, f);
                break;
            }
            case OP_REMOVE:
                setFirstOut(firstOutFragments, op.fragment);
                break;
            case OP_HIDE:
                setFirstOut(firstOutFragments, op.fragment);
                break;
            case OP_SHOW:
                setLastIn(lastInFragments, op.fragment);
                break;
            case OP_DETACH:
                setFirstOut(firstOutFragments, op.fragment);
                break;
            case OP_ATTACH:
                setLastIn(lastInFragments, op.fragment);
                break;
        }

        op = op.next;
    }
}

/**
 * Finds the first removed fragment and last added fragments when popping the back stack.
 * If none of the fragments have transitions, then both lists will be empty.
 *
 * @param firstOutFragments The list of first fragments to be removed, keyed on the
 *                          container ID. This list will be modified by the method.
 * @param lastInFragments The list of last fragments to be added, keyed on the
 *                        container ID. This list will be modified by the method.
 */
public void calculateBackFragments(SparseArray<Fragment> firstOutFragments,
        SparseArray<Fragment> lastInFragments) {
    if (!mManager.mContainer.hasView()) {
        return; // nothing to see, so no transitions
    }
    Op op = mHead;
    while (op != null) {
        switch (op.cmd) {
            case OP_ADD:
                setFirstOut(firstOutFragments, op.fragment);
                break;
            case OP_REPLACE:
                if (op.removed != null) {
                    for (int i = op.removed.size() - 1; i >= 0; i--) {
                        setLastIn(lastInFragments, op.removed.get(i));
                    }
                }
                setFirstOut(firstOutFragments, op.fragment);
                break;
            case OP_REMOVE:
                setLastIn(lastInFragments, op.fragment);
                break;
            case OP_HIDE:
                setLastIn(lastInFragments, op.fragment);
                break;
            case OP_SHOW:
                setFirstOut(firstOutFragments, op.fragment);
                break;
            case OP_DETACH:
                setLastIn(lastInFragments, op.fragment);
                break;
            case OP_ATTACH:
                setFirstOut(firstOutFragments, op.fragment);
                break;
        }

        op = op.next;
    }
}

/**
 * When custom fragment transitions are used, this sets up the state for each transition
 * and begins the transition. A different transition is started for each fragment container
 * and consists of up to 3 different transitions: the exit transition, a shared element
 * transition and an enter transition.
 *
 * <p>The exit transition operates against the leaf nodes of the first fragment
 * with a view that was removed. If no such fragment was removed, then no exit
 * transition is executed. The exit transition comes from the outgoing fragment.</p>
 *
 * <p>The enter transition operates against the last fragment that was added. If
 * that fragment does not have a view or no fragment was added, then no enter
 * transition is executed. The enter transition comes from the incoming fragment.</p>
 *
 * <p>The shared element transition operates against all views and comes either
 * from the outgoing fragment or the incoming fragment, depending on whether this
 * is going forward or popping the back stack. When going forward, the incoming
 * fragment's enter shared element transition is used, but when going back, the
 * outgoing fragment's return shared element transition is used. Shared element
 * transitions only operate if there is both an incoming and outgoing fragment.</p>
 *
 * @param firstOutFragments The list of first fragments to be removed, keyed on the
 *                          container ID.
 * @param lastInFragments The list of last fragments to be added, keyed on the
 *                        container ID.
 * @param isBack true if this is popping the back stack or false if this is a
 *               forward operation.
 * @return The TransitionState used to complete the operation of the transition
 * in {@link #setNameOverrides(android.app.BackStackRecord.TransitionState, java.util.ArrayList,
 * java.util.ArrayList)}.
 */
private TransitionState beginTransition(SparseArray<Fragment> firstOutFragments,
        SparseArray<Fragment> lastInFragments, boolean isBack) {
    TransitionState state = new TransitionState();

    // Adding a non-existent target view makes sure that the transitions don't target
    // any views by default. They'll only target the views we tell add. If we don't
    // add any, then no views will be targeted.
    state.nonExistentView = new View(mManager.mActivity);

    // Go over all leaving fragments.
    for (int i = 0; i < firstOutFragments.size(); i++) {
        int containerId = firstOutFragments.keyAt(i);
        configureTransitions(containerId, state, isBack, firstOutFragments,
                lastInFragments);
    }

    // Now go over all entering fragments that didn't have a leaving fragment.
    for (int i = 0; i < lastInFragments.size(); i++) {
        int containerId = lastInFragments.keyAt(i);
        if (firstOutFragments.get(containerId) == null) {
            configureTransitions(containerId, state, isBack, firstOutFragments,
                    lastInFragments);
        }
    }
    return state;
}

private static Transition cloneTransition(Transition transition) {
    if (transition != null) {
        transition = transition.clone();
    }
    return transition;
}

private static Transition getEnterTransition(Fragment inFragment, boolean isBack) {
    if (inFragment == null) {
        return null;
    }
    return cloneTransition(isBack ? inFragment.getReenterTransition() :
            inFragment.getEnterTransition());
}

private static Transition getExitTransition(Fragment outFragment, boolean isBack) {
    if (outFragment == null) {
        return null;
    }
    return cloneTransition(isBack ? outFragment.getReturnTransition() :
            outFragment.getExitTransition());
}

private static Transition getSharedElementTransition(Fragment inFragment, Fragment outFragment,
        boolean isBack) {
    if (inFragment == null || outFragment == null) {
        return null;
    }
    return cloneTransition(isBack ? outFragment.getSharedElementReturnTransition() :
            inFragment.getSharedElementEnterTransition());
}

private static ArrayList<View> captureExitingViews(Transition exitTransition,
        Fragment outFragment, ArrayMap<String, View> namedViews) {
    ArrayList<View> viewList = null;
    if (exitTransition != null) {
        viewList = new ArrayList<View>();
        View root = outFragment.getView();
        root.captureTransitioningViews(viewList);
        if (namedViews != null) {
            viewList.removeAll(namedViews.values());
        }
        addTargets(exitTransition, viewList);
    }
    return viewList;
}

private ArrayMap<String, View> remapSharedElements(TransitionState state, Fragment outFragment,
        boolean isBack) {
    ArrayMap<String, View> namedViews = new ArrayMap<String, View>();
    if (mSharedElementSourceNames != null) {
        outFragment.getView().findNamedViews(namedViews);
        if (isBack) {
            namedViews.retainAll(mSharedElementTargetNames);
        } else {
            namedViews = remapNames(mSharedElementSourceNames, mSharedElementTargetNames,
                    namedViews);
        }
    }

    if (isBack) {
        outFragment.mEnterTransitionCallback.onMapSharedElements(
                mSharedElementTargetNames, namedViews);
        setBackNameOverrides(state, namedViews, false);
    } else {
        outFragment.mExitTransitionCallback.onMapSharedElements(
                mSharedElementTargetNames, namedViews);
        setNameOverrides(state, namedViews, false);
    }

    return namedViews;
}

/**
 * Prepares the enter transition by adding a non-existent view to the transition's target list
 * and setting it epicenter callback. By adding a non-existent view to the target list,
 * we can prevent any view from being targeted at the beginning of the transition.
 * We will add to the views before the end state of the transition is captured so that the
 * views will appear. At the start of the transition, we clear the list of targets so that
 * we can restore the state of the transition and use it again.
 *
 * <p>The shared element transition maps its shared elements immediately prior to
 * capturing the final state of the Transition.</p>
 */
private ArrayList<View> addTransitionTargets(final TransitionState state,
        final Transition enterTransition, final Transition sharedElementTransition,
        final Transition overallTransition, final View container,
        final Fragment inFragment, final Fragment outFragment,
        final ArrayList<View> hiddenFragmentViews, final boolean isBack,
        final ArrayList<View> sharedElementTargets) {
    if (enterTransition == null && sharedElementTransition == null &&
            overallTransition == null) {
        return null;
    }
    final ArrayList<View> enteringViews = new ArrayList<View>();
    container.getViewTreeObserver().addOnPreDrawListener(
            new ViewTreeObserver.OnPreDrawListener() {
                @Override
                public boolean onPreDraw() {
                    container.getViewTreeObserver().removeOnPreDrawListener(this);

                    // Don't include any newly-hidden fragments in the transition.
                    excludeHiddenFragments(hiddenFragmentViews, inFragment.mContainerId,
                            overallTransition);

                    ArrayMap<String, View> namedViews = null;
                    if (sharedElementTransition != null) {
                        namedViews = mapSharedElementsIn(state, isBack, inFragment);
                        removeTargets(sharedElementTransition, sharedElementTargets);
                        sharedElementTargets.clear();
                        if (namedViews.isEmpty()) {
                            sharedElementTargets.add(state.nonExistentView);
                        } else {
                            sharedElementTargets.addAll(namedViews.values());
                        }

                        addTargets(sharedElementTransition, sharedElementTargets);

                        setEpicenterIn(namedViews, state);

                        callSharedElementEnd(state, inFragment, outFragment, isBack,
                                namedViews);
                    }

                    if (enterTransition != null) {
                        View view = inFragment.getView();
                        if (view != null) {
                            view.captureTransitioningViews(enteringViews);
                            if (namedViews != null) {
                                enteringViews.removeAll(namedViews.values());
                            }
                            addTargets(enterTransition, enteringViews);
                        }
                        setSharedElementEpicenter(enterTransition, state);
                    }
                    return true;
                }
            });
    return enteringViews;
}

private void callSharedElementEnd(TransitionState state, Fragment inFragment,
        Fragment outFragment, boolean isBack, ArrayMap<String, View> namedViews) {
    SharedElementCallback sharedElementCallback = isBack ?
            outFragment.mEnterTransitionCallback :
            inFragment.mEnterTransitionCallback;
    ArrayList<String> names = new ArrayList<String>(namedViews.keySet());
    ArrayList<View> views = new ArrayList<View>(namedViews.values());
    sharedElementCallback.onSharedElementEnd(names, views, null);
}

private void setEpicenterIn(ArrayMap<String, View> namedViews, TransitionState state) {
    if (mSharedElementTargetNames != null && !namedViews.isEmpty()) {
        // now we know the epicenter of the entering transition.
        View epicenter = namedViews
                .get(mSharedElementTargetNames.get(0));
        if (epicenter != null) {
            state.enteringEpicenterView = epicenter;
        }
    }
}

private ArrayMap<String, View> mapSharedElementsIn(TransitionState state,
        boolean isBack, Fragment inFragment) {
    // Now map the shared elements in the incoming fragment
    ArrayMap<String, View> namedViews = mapEnteringSharedElements(state, inFragment, isBack);

    // remap shared elements and set the name mapping used
    // in the shared element transition.
    if (isBack) {
        inFragment.mExitTransitionCallback.onMapSharedElements(
                mSharedElementTargetNames, namedViews);
        setBackNameOverrides(state, namedViews, true);
    } else {
        inFragment.mEnterTransitionCallback.onMapSharedElements(
                mSharedElementTargetNames, namedViews);
        setNameOverrides(state, namedViews, true);
    }
    return namedViews;
}

private static Transition mergeTransitions(Transition enterTransition,
        Transition exitTransition, Transition sharedElementTransition, Fragment inFragment,
        boolean isBack) {
    boolean overlap = true;
    if (enterTransition != null && exitTransition != null) {
        overlap = isBack ? inFragment.getAllowReturnTransitionOverlap() :
                inFragment.getAllowEnterTransitionOverlap();
    }

    // Wrap the transitions. Explicit targets like in enter and exit will cause the
    // views to be targeted regardless of excluded views. If that happens, then the
    // excluded fragments views (hidden fragments) will still be in the transition.

    Transition transition;
    if (overlap) {
        // Regular transition -- do it all together
        TransitionSet transitionSet = new TransitionSet();
        if (enterTransition != null) {
            transitionSet.addTransition(enterTransition);
        }
        if (exitTransition != null) {
            transitionSet.addTransition(exitTransition);
        }
        if (sharedElementTransition != null) {
            transitionSet.addTransition(sharedElementTransition);
        }
        transition = transitionSet;
    } else {
        // First do exit, then enter, but allow shared element transition to happen
        // during both.
        Transition staggered = null;
        if (exitTransition != null && enterTransition != null) {
            staggered = new TransitionSet()
                    .addTransition(exitTransition)
                    .addTransition(enterTransition)
                    .setOrdering(TransitionSet.ORDERING_SEQUENTIAL);
        } else if (exitTransition != null) {
            staggered = exitTransition;
        } else if (enterTransition != null) {
            staggered = enterTransition;
        }
        if (sharedElementTransition != null) {
            TransitionSet together = new TransitionSet();
            if (staggered != null) {
                together.addTransition(staggered);
            }
            together.addTransition(sharedElementTransition);
            transition = together;
        } else {
            transition = staggered;
        }
    }
    return transition;
}

/**
 * Configures custom transitions for a specific fragment container.
 *
 * @param containerId The container ID of the fragments to configure the transition for.
 * @param state The Transition State keeping track of the executing transitions.
 * @param firstOutFragments The list of first fragments to be removed, keyed on the
 *                          container ID.
 * @param lastInFragments The list of last fragments to be added, keyed on the
 *                        container ID.
 * @param isBack true if this is popping the back stack or false if this is a
 *               forward operation.
 */
private void configureTransitions(int containerId, TransitionState state, boolean isBack,
        SparseArray<Fragment> firstOutFragments, SparseArray<Fragment> lastInFragments) {
    ViewGroup sceneRoot = (ViewGroup) mManager.mContainer.findViewById(containerId);
    if (sceneRoot != null) {
        Fragment inFragment = lastInFragments.get(containerId);
        Fragment outFragment = firstOutFragments.get(containerId);

        Transition enterTransition = getEnterTransition(inFragment, isBack);
        Transition sharedElementTransition = getSharedElementTransition(inFragment, outFragment,
                isBack);
        Transition exitTransition = getExitTransition(outFragment, isBack);

        if (enterTransition == null && sharedElementTransition == null &&
                exitTransition == null) {
            return; // no transitions!
        }
        if (enterTransition != null) {
            enterTransition.addTarget(state.nonExistentView);
        }
        ArrayMap<String, View> namedViews = null;
        ArrayList<View> sharedElementTargets = new ArrayList<View>();
        if (sharedElementTransition != null) {
            namedViews = remapSharedElements(state, outFragment, isBack);
            if (namedViews.isEmpty()) {
                sharedElementTargets.add(state.nonExistentView);
            } else {
                sharedElementTargets.addAll(namedViews.values());
            }
            addTargets(sharedElementTransition, sharedElementTargets);

            // Notify the start of the transition.
            SharedElementCallback callback = isBack ?
                    outFragment.mEnterTransitionCallback :
                    inFragment.mEnterTransitionCallback;
            ArrayList<String> names = new ArrayList<String>(namedViews.keySet());
            ArrayList<View> views = new ArrayList<View>(namedViews.values());
            callback.onSharedElementStart(names, views, null);
        }

        ArrayList<View> exitingViews = captureExitingViews(exitTransition, outFragment,
                namedViews);
        if (exitingViews == null || exitingViews.isEmpty()) {
            exitTransition = null;
        }

        // Set the epicenter of the exit transition
        if (mSharedElementTargetNames != null && namedViews != null) {
            View epicenterView = namedViews.get(mSharedElementTargetNames.get(0));
            if (epicenterView != null) {
                if (exitTransition != null) {
                    setEpicenter(exitTransition, epicenterView);
                }
                if (sharedElementTransition != null) {
                    setEpicenter(sharedElementTransition, epicenterView);
                }
            }
        }

        Transition transition = mergeTransitions(enterTransition, exitTransition,
                sharedElementTransition, inFragment, isBack);

        if (transition != null) {
            ArrayList<View> hiddenFragments = new ArrayList<View>();
            ArrayList<View> enteringViews = addTransitionTargets(state, enterTransition,
                    sharedElementTransition, transition, sceneRoot, inFragment, outFragment,
                    hiddenFragments, isBack, sharedElementTargets);

            transition.setNameOverrides(state.nameOverrides);
            // We want to exclude hidden views later, so we need a non-null list in the
            // transition now.
            transition.excludeTarget(state.nonExistentView, true);
            // Now exclude all currently hidden fragments.
            excludeHiddenFragments(hiddenFragments, containerId, transition);
            TransitionManager.beginDelayedTransition(sceneRoot, transition);
            // Remove the view targeting after the transition starts
            removeTargetedViewsFromTransitions(sceneRoot, state.nonExistentView,
                    enterTransition, enteringViews, exitTransition, exitingViews,
                    sharedElementTransition, sharedElementTargets, transition, hiddenFragments);
        }
    }
}

/**
 * After the transition has started, remove all targets that we added to the transitions
 * so that the transitions are left in a clean state.
 */
private void removeTargetedViewsFromTransitions(
        final ViewGroup sceneRoot, final View nonExistingView,
        final Transition enterTransition, final ArrayList<View> enteringViews,
        final Transition exitTransition, final ArrayList<View> exitingViews,
        final Transition sharedElementTransition, final ArrayList<View> sharedElementTargets,
        final Transition overallTransition, final ArrayList<View> hiddenViews) {
    if (overallTransition != null) {
        sceneRoot.getViewTreeObserver().addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {
            @Override
            public boolean onPreDraw() {
                sceneRoot.getViewTreeObserver().removeOnPreDrawListener(this);
                if (enterTransition != null) {
                    enterTransition.removeTarget(nonExistingView);
                    removeTargets(enterTransition, enteringViews);
                }
                if (exitTransition != null) {
                    removeTargets(exitTransition, exitingViews);
                }
                if (sharedElementTransition != null) {
                    removeTargets(sharedElementTransition, sharedElementTargets);
                }
                int numViews = hiddenViews.size();
                for (int i = 0; i < numViews; i++) {
                    overallTransition.excludeTarget(hiddenViews.get(i), false);
                }
                overallTransition.excludeTarget(nonExistingView, false);
                return true;
            }
        });
    }
}

private static void removeTargets(Transition transition, ArrayList<View> views) {
    int numViews = views.size();
    for (int i = 0; i < numViews; i++) {
        transition.removeTarget(views.get(i));
    }
}

private static void addTargets(Transition transition, ArrayList<View> views) {
    int numViews = views.size();
    for (int i = 0; i < numViews; i++) {
        transition.addTarget(views.get(i));
    }
}

/**
 * Remaps a name-to-View map, substituting different names for keys.
 *
 * @param inMap A list of keys found in the map, in the order in toGoInMap
 * @param toGoInMap A list of keys to use for the new map, in the order of inMap
 * @param namedViews The current mapping
 * @return a new Map after it has been mapped with the new names as keys.
 */
private static ArrayMap<String, View> remapNames(ArrayList<String> inMap,
        ArrayList<String> toGoInMap, ArrayMap<String, View> namedViews) {
    ArrayMap<String, View> remappedViews = new ArrayMap<String, View>();
    if (!namedViews.isEmpty()) {
        int numKeys = inMap.size();
        for (int i = 0; i < numKeys; i++) {
            View view = namedViews.get(inMap.get(i));

            if (view != null) {
                remappedViews.put(toGoInMap.get(i), view);
            }
        }
    }
    return remappedViews;
}

/**
 * Maps shared elements to views in the entering fragment.
 *
 * @param state The transition State as returned from {@link #beginTransition(
 * android.util.SparseArray, android.util.SparseArray, boolean)}.
 * @param inFragment The last fragment to be added.
 * @param isBack true if this is popping the back stack or false if this is a
 *               forward operation.
 */
private ArrayMap<String, View> mapEnteringSharedElements(TransitionState state,
        Fragment inFragment, boolean isBack) {
    ArrayMap<String, View> namedViews = new ArrayMap<String, View>();
    View root = inFragment.getView();
    if (root != null) {
        if (mSharedElementSourceNames != null) {
            root.findNamedViews(namedViews);
            if (isBack) {
                namedViews = remapNames(mSharedElementSourceNames,
                        mSharedElementTargetNames, namedViews);
            } else {
                namedViews.retainAll(mSharedElementTargetNames);
            }
        }
    }
    return namedViews;
}

private void excludeHiddenFragments(final ArrayList<View> hiddenFragmentViews, int containerId,
        Transition transition) {
    if (mManager.mAdded != null) {
        for (int i = 0; i < mManager.mAdded.size(); i++) {
            Fragment fragment = mManager.mAdded.get(i);
            if (fragment.mView != null && fragment.mContainer != null &&
                    fragment.mContainerId == containerId) {
                if (fragment.mHidden) {
                    if (!hiddenFragmentViews.contains(fragment.mView)) {
                        transition.excludeTarget(fragment.mView, true);
                        hiddenFragmentViews.add(fragment.mView);
                    }
                } else {
                    transition.excludeTarget(fragment.mView, false);
                    hiddenFragmentViews.remove(fragment.mView);
                }
            }
        }
    }
}

private static void setEpicenter(Transition transition, View view) {
    final Rect epicenter = new Rect();
    view.getBoundsOnScreen(epicenter);

    transition.setEpicenterCallback(new Transition.EpicenterCallback() {
        @Override
        public Rect onGetEpicenter(Transition transition) {
            return epicenter;
        }
    });
}

private void setSharedElementEpicenter(Transition transition, final TransitionState state) {
    transition.setEpicenterCallback(new Transition.EpicenterCallback() {
        private Rect mEpicenter;

        @Override
        public Rect onGetEpicenter(Transition transition) {
            if (mEpicenter == null && state.enteringEpicenterView != null) {
                mEpicenter = new Rect();
                state.enteringEpicenterView.getBoundsOnScreen(mEpicenter);
            }
            return mEpicenter;
        }
    });
}


//=======================

ECode BackStackRecord::PopFromBackStack(
        /* [in] */ Boolean doStateMove,
        /* [in] */ ITransitionState* state,
        /* [in] */ ISparseArray* /*<Fragment>*/ firstOutFragments,
        /* [in] */ ISparseArray* /*<Fragment>*/ lastInFragments,
        /* [out] */ ITransitionState** result)
{
    if (CFragmentManagerImpl::DEBUG) {
        Logger::V(TAG, "PopFromBackStack: %p", this);
//        LogWriter logw/* = new LogWriter(Logger::VERBOSE, TAG)*/;
       //  AutoPtr<IPrintWriter> pw;
       //  FastPrintWriter::New(logw, FALSE, 1024, (IPrintWriter**)&pw);
       //  Dump(String("  "), NULL, pw, NULL);
    }

    if (state == null) {
        if (firstOutFragments.size() != 0 || lastInFragments.size() != 0) {
            state = beginTransition(firstOutFragments, lastInFragments, true);
        }
    } else if (!doStateMove) {
        setNameOverrides(state, mSharedElementTargetNames, mSharedElementSourceNames);
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
        state = null;
    }

    if (mIndex >= 0) {
        mManager->FreeBackStackIndex(mIndex);
        mIndex = -1;
    }
    return state;
    return NOERROR;
}

//===========================

private static void setNameOverride(ArrayMap<String, String> overrides,
        String source, String target) {
    if (source != null && target != null && !source.equals(target)) {
        for (int index = 0; index < overrides.size(); index++) {
            if (source.equals(overrides.valueAt(index))) {
                overrides.setValueAt(index, target);
                return;
            }
        }
        overrides.put(source, target);
    }
}

private static void setNameOverrides(TransitionState state, ArrayList<String> sourceNames,
        ArrayList<String> targetNames) {
    if (sourceNames != null) {
        for (int i = 0; i < sourceNames.size(); i++) {
            String source = sourceNames.get(i);
            String target = targetNames.get(i);
            setNameOverride(state.nameOverrides, source, target);
        }
    }
}

private void setBackNameOverrides(TransitionState state, ArrayMap<String, View> namedViews,
        boolean isEnd) {
    int count = mSharedElementTargetNames.size();
    for (int i = 0; i < count; i++) {
        String source = mSharedElementSourceNames.get(i);
        String originalTarget = mSharedElementTargetNames.get(i);
        View view = namedViews.get(originalTarget);
        if (view != null) {
            String target = view.getTransitionName();
            if (isEnd) {
                setNameOverride(state.nameOverrides, source, target);
            } else {
                setNameOverride(state.nameOverrides, target, source);
            }
        }
    }
}

private void setNameOverrides(TransitionState state, ArrayMap<String, View> namedViews,
        boolean isEnd) {
    int count = namedViews.size();
    for (int i = 0; i < count; i++) {
        String source = namedViews.keyAt(i);
        String target = namedViews.valueAt(i).getTransitionName();
        if (isEnd) {
            setNameOverride(state.nameOverrides, source, target);
        } else {
            setNameOverride(state.nameOverrides, target, source);
        }
    }
}
//===========================

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
