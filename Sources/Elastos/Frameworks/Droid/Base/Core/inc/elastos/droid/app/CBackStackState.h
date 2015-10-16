#ifndef __ELASTOS_DROID_APP_CBACKSTACKSTATE_H__
#define __ELASTOS_DROID_APP_CBACKSTACKSTATE_H__

#include "_Elastos_Droid_App_CBackStackState.h"
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/app/CFragmentManagerImpl.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::IRunnable;
using Elastos::Core::ICharSequence;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;
using Elastos::Droid::App::IFragmentManagerImpl;
using Elastos::Droid::App::IFragmentManagerBackStackEntry;
using Elastos::Droid::App::IBackStackRecord;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CBackStackState)
{
public:
    CBackStackState();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IFragmentManagerImpl* fm,
        /* [in] */ IBackStackRecord* bse);

    CARAPI Instantiate(
        /* [in] */ IFragmentManagerImpl* fm,
        /* [out] */ IBackStackRecord** record);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

public:
    AutoPtr< ArrayOf<Int32> > mOps;
    Int32 mTransition;
    Int32 mTransitionStyle;
    String mName;
    Int32 mIndex;
    Int32 mBreadCrumbTitleRes;
    AutoPtr<ICharSequence> mBreadCrumbTitleText;
    Int32 mBreadCrumbShortTitleRes;
    AutoPtr<ICharSequence> mBreadCrumbShortTitleText;
};

/**
 * @hide Entry of an operation on the fragment back stack.
 */
class BackStackRecord
    : public IBackStackRecord
    , public IRunnable
    , public IFragmentManagerBackStackEntry
    , public ElRefBase
{
public:
    class Op : public ElLightRefBase
    {
    public:
        Op()
            : mCmd(0)
            , mEnterAnim(0)
            , mExitAnim(0)
            , mPopEnterAnim(0)
            , mPopExitAnim(0)
        {}

    public:
        AutoPtr<Op> mNext;
        AutoPtr<Op> mPrev;
        Int32 mCmd;
        AutoPtr<IFragment> mFragment;
        Int32 mEnterAnim;
        Int32 mExitAnim;
        Int32 mPopEnterAnim;
        Int32 mPopExitAnim;
        List< AutoPtr<IFragment> > mRemoved;
    };

public:
    BackStackRecord(
        /* [in] */ IFragmentManagerImpl* manager);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI ToString(
        /* [out] */ String* str);

    virtual CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

    virtual CARAPI GetId(
        /* [out] */ Int32* id);

    virtual CARAPI GetBreadCrumbTitleRes(
        /* [out] */ Int32* titleRes);

    virtual CARAPI GetBreadCrumbShortTitleRes(
        /* [out] */ Int32* shortTitleRes);

    virtual CARAPI GetBreadCrumbTitle(
        /* [out] */ ICharSequence** title);

    virtual CARAPI GetBreadCrumbShortTitle(
        /* [out] */ ICharSequence** shortTitle);

    virtual CARAPI Add(
        /* [in] */ IFragment* fragment,
        /* [in] */ const String& tag);

    virtual CARAPI Add(
        /* [in] */ Int32 containerViewId,
        /* [in] */ IFragment* fragment);

    virtual CARAPI Add(
        /* [in] */ Int32 containerViewId,
        /* [in] */ IFragment* fragment,
        /* [in] */ const String& tag);

    virtual CARAPI Replace(
        /* [in] */ Int32 containerViewId,
        /* [in] */ IFragment* fragment);

    virtual CARAPI Replace(
        /* [in] */ Int32 containerViewId,
        /* [in] */ IFragment* fragment,
        /* [in] */ const String& tag);

    virtual CARAPI Remove(
        /* [in] */ IFragment* fragment);

    virtual CARAPI Hide(
        /* [in] */ IFragment* fragment);

    virtual CARAPI Show(
        /* [in] */ IFragment* fragment);

    virtual CARAPI Detach(
        /* [in] */ IFragment* fragment);

    virtual CARAPI Attach(
        /* [in] */ IFragment* fragment);

    virtual CARAPI SetCustomAnimations(
        /* [in] */ Int32 enter,
        /* [in] */ Int32 exit);

    virtual CARAPI SetCustomAnimations(
        /* [in] */ Int32 enter,
        /* [in] */ Int32 exit,
        /* [in] */ Int32 popEnter,
        /* [in] */ Int32 popExit);

    virtual CARAPI SetTransition(
        /* [in] */ Int32 transition);

    virtual CARAPI SetTransitionStyle(
        /* [in] */ Int32 styleRes);

    virtual CARAPI AddToBackStack(
        /* [in] */ const String& name);

    virtual CARAPI IsAddToBackStackAllowed(
        /* [out] */ Boolean* allowed);

    virtual CARAPI DisallowAddToBackStack();

    virtual CARAPI SetBreadCrumbTitle(
        /* [in] */ Int32 res);

    virtual CARAPI SetBreadCrumbTitle(
        /* [in] */ ICharSequence* text);

    virtual CARAPI SetBreadCrumbShortTitle(
        /* [in] */ Int32 res);

    virtual CARAPI SetBreadCrumbShortTitle(
        /* [in] */ ICharSequence* text);

    virtual CARAPI Commit(
        /* [out] */ Int32* identifier);

    virtual CARAPI CommitAllowingStateLoss(
        /* [out] */ Int32* res);

    virtual CARAPI Run();

    virtual CARAPI PopFromBackStack(
        /* [in] */ Boolean doStateMove);

    virtual CARAPI GetName(
        /* [out] */ String* name);

    virtual CARAPI GetTransition(
        /* [out] */ Int32* trans);

    virtual CARAPI GetTransitionStyle(
        /* [out] */ Int32* transStyle);

    virtual CARAPI IsEmpty(
        /* [out] */ Boolean* empty);

protected:
    CARAPI_(void) Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ Boolean full);

    CARAPI_(void) AddOp(
        /* [in] */ Op* op);

    CARAPI_(void) BumpBackStackNesting(
        /* [in] */ Int32 amt);

    CARAPI_(Int32) CommitInternal(
        /* [in] */ Boolean allowStateLoss);

private:
     CARAPI DoAddOp(
        /* [in] */ Int32 containerViewId,
        /* [in] */ IFragment* fragment,
        /* [in] */ const String& tag,
        /* [in] */ Int32 opcmd);

public:
    static const String TAG;

    AutoPtr<CFragmentManagerImpl> mManager;

    static const Int32 OP_NULL;
    static const Int32 OP_ADD;
    static const Int32 OP_REPLACE;
    static const Int32 OP_REMOVE;
    static const Int32 OP_HIDE;
    static const Int32 OP_SHOW;
    static const Int32 OP_DETACH;
    static const Int32 OP_ATTACH;

    AutoPtr<Op> mHead;
    AutoPtr<Op> mTail;
    Int32 mNumOp;
    Int32 mEnterAnim;
    Int32 mExitAnim;
    Int32 mPopEnterAnim;
    Int32 mPopExitAnim;
    Int32 mTransition;
    Int32 mTransitionStyle;
    Boolean mAddToBackStack;
    Boolean mAllowAddToBackStack;
    String mName;
    Boolean mCommitted;
    Int32 mIndex;

    Int32 mBreadCrumbTitleRes;
    AutoPtr<ICharSequence> mBreadCrumbTitleText;
    Int32 mBreadCrumbShortTitleRes;
    AutoPtr<ICharSequence> mBreadCrumbShortTitleText;

    friend class CBackStackState;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CBACKSTACKSTATE_H__
