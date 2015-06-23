
#ifndef __CFRAGMENTMANAGERIMPL_H__
#define __CFRAGMENTMANAGERIMPL_H__

#include "_CFragmentManagerImpl.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include "animation/AnimatorListenerAdapter.h"

using Elastos::Core::IInteger32;
using Elastos::Core::IRunnable;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::Mutex;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;
using Elastos::Utility::IObjectInt32Map;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::App::IBackStackRecord;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Content::Res::IConfiguration;

namespace Elastos {
namespace Droid {
namespace App {

class RunnableLocal
    : public ElRefBase
    , public IRunnable
{
public:
    PInterface Probe(
        /* [in]  */ REIID riid)
    {
        if (riid == EIID_IInterface) {
            return (PInterface)this;
        }
        else if (riid == EIID_IRunnable) {
            return (IRunnable*)this;
        }

        return NULL;

    }

    UInt32 AddRef()
    {
        return ElRefBase::AddRef();
    }

    UInt32 Release()
    {
        return ElRefBase::Release();
    }

    ECode GetInterfaceID(
        /* [in] */ IInterface *Object,
        /* [out] */ InterfaceID *IID)
    {
        VALIDATE_NOT_NULL(IID);

        if (Object == (IInterface*)(IRunnable*)this) {
            *IID = EIID_IRunnable;
        }
        return E_INVALID_ARGUMENT;
    }

    virtual ECode Run() = 0;
};

class ExecCommitRunnable
    : public RunnableLocal
{
public:
    ExecCommitRunnable(
        /* [in] */ CFragmentManagerImpl* host)
        : mHost(host)
    {}

    CARAPI Run();

private:
    CFragmentManagerImpl* mHost;
};


class Runnable1
    : public RunnableLocal
{
public:
    Runnable1(
        CFragmentManagerImpl* host)
        : mHost(host)
    {}

    CARAPI Run();

private:
    CFragmentManagerImpl* mHost;
};

class Runnable2
    : public RunnableLocal
{
public:
    Runnable2(
        /* [in] */ CFragmentManagerImpl* host,
        /* [in] */ const String& name,
        /* [in] */ Int32 flags)
        : mHost(host)
        , mName(name)
        , mFlags(flags)
    {}

    CARAPI Run();

private:
    CFragmentManagerImpl* mHost;
    String mName;
    Int32 mFlags;
};

class Runnable3
    : public RunnableLocal
{
public:
    Runnable3(
        CFragmentManagerImpl* host,
        Int32 id,
        Int32 flags)
        : mHost(host)
        , mId(id)
        , mFlags(flags)
    {}

    CARAPI Run();

private:
    CFragmentManagerImpl* mHost;
    Int32 mId;
    Int32 mFlags;
};

class AnimatorListener : public AnimatorListenerAdapter
{
public:
    AnimatorListener(
        /* [in] */ IViewGroup* container,
        /* [in] */ IView* view,
        /* [in] */ IFragment* fragment,
        /* [in] */ CFragmentManagerImpl* host);

    virtual CARAPI OnAnimationEnd(
        /* [in] */ IAnimator* animation);

private:
    AutoPtr<IViewGroup> mContainer;
    AutoPtr<IView> mView;
    AutoPtr<IFragment> mFragment;
    CFragmentManagerImpl* mHost;
};

class AnimatorListenerEx : public AnimatorListenerAdapter
{
public:
    AnimatorListener(
        /* [in] */ IFragment* fragment);

    virtual CARAPI OnAnimationEnd(
        /* [in] */ IAnimator* animation);

private:
    AutoPtr<IFragment> mFragment;
};

CarClass(CFragmentManagerImpl)
{
public:
    CFragmentManagerImpl();

    ~CFragmentManagerImpl();

    CARAPI BeginTransaction(
        /* [out] */ IFragmentTransaction** transaction);

    CARAPI OpenTransaction(
        /* [out] */ IFragmentTransaction** transaction);

    CARAPI ExecutePendingTransactions(
        /* [out] */ Boolean* executed);

    CARAPI PopBackStack();

    CARAPI PopBackStackImmediate(
        /* [out] */ Boolean* hasPopped);

    CARAPI PopBackStack(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags);

    CARAPI PopBackStackImmediate(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* hasPopped);

    CARAPI PopBackStack(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags);

    CARAPI PopBackStackImmediate(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* hasPopped);

    CARAPI GetBackStackEntryCount(
        /* [out] */ Int32* count);

    CARAPI GetBackStackEntryAt(
        /* [in] */ Int32 index,
        /* [out] */ IFragmentManagerBackStackEntry** bsEntry);

    CARAPI AddOnBackStackChangedListener(
        /* [in] */ IFragmentManagerOnBackStackChangedListener* listener);

    CARAPI RemoveOnBackStackChangedListener(
        /* [in] */ IFragmentManagerOnBackStackChangedListener* listener);

    CARAPI PutFragment(
        /* [in] */ IBundle* bundle,
        /* [in] */ const String& key,
        /* [in] */ IFragment* fragment);

    CARAPI GetFragment(
        /* [in] */ IBundle* bundle,
        /* [in] */ const String& key,
        /* [out] */ IFragment** fragment);

    CARAPI SaveFragmentInstanceState(
        /* [in] */ IFragment* fragment,
        /* [out] */ IFragmentSavedState** state);

    CARAPI IsDestroyed(
        /* [out] */ Boolean* destroyed);

    CARAPI ToString(
        /* [out] */ String* s);

    CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

    CARAPI LoadAnimator(
        /* [in] */ IFragment* fragment,
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter,
        /* [in] */ Int32 transitionStyle,
        /* [out] */ IAnimator** animator);

    CARAPI PerformPendingDeferredStart(
        /* [in] */ IFragment* f);

    CARAPI MoveToState(
        /* [in] */ IFragment* f,
        /* [in] */ Int32 newState,
        /* [in] */ Int32 transit,
        /* [in] */ Int32 transitionStyle,
        /* [in] */ Boolean keepActive);

    CARAPI MoveToState(
        /* [in] */ IFragment* f);

    CARAPI MoveToState(
        /* [in] */ Int32 newState,
        /* [in] */ Boolean always);

    CARAPI MoveToState(
        /* [in] */ Int32 newState,
        /* [in] */ Int32 transit,
        /* [in] */ Int32 transitStyle,
        /* [in] */ Boolean always);

    CARAPI StartPendingDeferredFragments();

    CARAPI MakeActive(
        /* [in] */ IFragment* f);

    CARAPI MakeInactive(
        /* [in] */ IFragment* f);

    CARAPI AddFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Boolean moveToStateNow);

    CARAPI RemoveFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Int32 transition,
        /* [in] */ Int32 transitionStyle);

    CARAPI HideFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Int32 transition,
        /* [in] */ Int32 transitionStyle);

    CARAPI ShowFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Int32 transition,
        /* [in] */ Int32 transitionStyle);

    CARAPI DetachFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Int32 transition,
        /* [in] */ Int32 transitionStyle);

    CARAPI AttachFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Int32 transition,
        /* [in] */ Int32 transitionStyle);

    CARAPI FindFragmentById(
        /* [in] */ Int32 id,
        /* [out] */ IFragment** fragment);

    CARAPI FindFragmentByTag(
        /* [in] */ const String& tag,
        /* [out] */ IFragment** fragment);

    CARAPI FindFragmentByWho(
        /* [in] */ const String& who,
        /* [out] */ IFragment** fragment);

    CARAPI EnqueueAction(
        /* [in] */ IRunnable* action,
        /* [in] */ Boolean allowStateLoss);

    CARAPI AllocBackStackIndex(
        /* [in] */ IBackStackRecord* bse,
        /* [out] */ Int32* index);

    CARAPI SetBackStackIndex(
        /* [in] */ Int32 index,
        /* [in] */ IBackStackRecord* bse);

    CARAPI FreeBackStackIndex(
        /* [in] */ Int32 index);

    CARAPI ExecPendingActions(
        /* [out] */ Boolean* result);

    CARAPI ReportBackStackChanged();

    CARAPI AddBackStackState(
        /* [in] */ IBackStackRecord* state);

    CARAPI PopBackStackState(
        /* [in] */ IHandler* handler,
        /* [in] */ const String& name,
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* result);

    CARAPI RetainNonConfig(
        /* [out] */ IObjectContainer** retains);

    CARAPI SaveFragmentViewState(
        /* [in] */ IFragment* f);

    CARAPI SaveFragmentBasicState(
        /* [in] */ IFragment* f,
        /* [out] */ IBundle** rst);

    CARAPI SaveAllState(
        /* [out] */ IParcelable** state);

    CARAPI RestoreAllState(
        /* [in] */ IParcelable* state,
        /* [in] */ IObjectContainer* nonConfig);

    CARAPI AttachActivity(
        /* [in] */ IActivity* activity,
        /* [in] */ IFragmentContainer* container,
        /* [in] */ IFragment* parent);

    CARAPI NoteStateNotSaved();

    CARAPI DispatchCreate();

    CARAPI DispatchActivityCreated();

    CARAPI DispatchStart();

    CARAPI DispatchResume();

    CARAPI DispatchPause();

    CARAPI DispatchStop();

    CARAPI DispatchDestroyView();

    CARAPI DispatchDestroy();

    CARAPI DispatchConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI DispatchLowMemory();

    CARAPI DispatchTrimMemory(
        /* [in] */ Int32 level);

    CARAPI DispatchCreateOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [in] */ IMenuInflater* inflater,
        /* [out] */ Boolean* rst);

    CARAPI DispatchPrepareOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* rst);

    CARAPI DispatchOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* rst);

    CARAPI DispatchContextItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* rst);

    CARAPI DispatchOptionsMenuClosed(
        /* [in] */ IMenu* menu);

    CARAPI InvalidateOptionsMenu();

    static CARAPI ReverseTransit(
        /* [in] */ Int32 transit,
        /* [out] */ Int32* reverse);

    static CARAPI TransitToStyleIndex(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter,
        /* [out] */ Int32* index);

private:
    CARAPI CheckStateLoss();

public:
    static Boolean DEBUG;

    List<AutoPtr<IRunnable> > mPendingActions;
    AutoPtr< ArrayOf<IRunnable* > > mTmpActions;
    Boolean mExecutingActions;

    List<AutoPtr<IFragment> > mActive;
    List<AutoPtr<IFragment> > mAdded;
    List<Int32> mAvailIndices;
    List<AutoPtr<IBackStackRecord> > mBackStack;
    List<AutoPtr<IFragment> > mCreatedMenus;

    // Must be accessed while locked.
    List<AutoPtr<IBackStackRecord> > mBackStackIndices;
    List<Int32> mAvailBackStackIndices;

    List<AutoPtr<IFragmentManagerOnBackStackChangedListener> > mBackStackChangeListeners;

    Int32 mCurState;
    IActivity* mActivity;
    AutoPtr<IFragmentContainer> mContainer;
    AutoPtr<IFragment> mParent;

    Boolean mNeedMenuInvalidate;
    Boolean mStateSaved;
    Boolean mDestroyed;
    String mNoTransactionsBecause;
    Boolean mHavePendingDeferredStart;

    // Temporary vars for state save and restore.
    AutoPtr<IBundle> mStateBundle;
    AutoPtr<IObjectInt32Map> mStateArray;
    AutoPtr<ExecCommitRunnable> mExecCommit;
    Mutex mThisLock;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CFRAGMENTMANAGERIMPL_H__
