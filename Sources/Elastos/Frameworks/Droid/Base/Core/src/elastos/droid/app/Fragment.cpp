
#include "elastos/droid/app/Fragment.h"
#include "elastos/droid/app/CFragmentManagerImpl.h"
#include "elastos/droid/app/CFragmentManagerImplHelper.h"
// #include "elastos/droid/app/Activity.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Droid::View::EIID_IViewOnCreateContextMenuListener;
using Elastos::Droid::Content::EIID_IComponentCallbacks;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::IClassLoader;
using Elastos::Utility::Logging::Slogger;


namespace Elastos {
namespace Droid {
namespace App {

//============================================================================
// FragmentContainerLocal
//============================================================================
class FragmentContainerLocal
    : public Object
    , public IFragmentContainer
{
public:
    CAR_INTERFACE_DECL()

    FragmentContainerLocal(
        /* [in] */ Fragment* host)
        : mHost(host)
    {}

    ECode FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** view)
    {
        VALIDATE_NOT_NULL(view)
        *view = NULL;

        if (mHost->mView == NULL) {
//            throw new IllegalStateException("Fragment does not have a view");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        return mHost->mView->FindViewById(id, view);
    }

    CARAPI HasView(
        /* [out] */ Boolean* result)
    {
        VALIDATE_NOT_NULL(result)
        *result = (mHost->mView != NULL);
        return NOERROR;
    }

public:
    Fragment* mHost;
};

CAR_INTERFACE_IMPL(FragmentContainerLocal, Object, IFragmentContainer)

//============================================================================
// FragmentState::ParcelableCreatorFragmentState
//============================================================================

FragmentState::ParcelableCreatorFragmentState::CreateFromParcel(
    /* [in] */ IParcel* in,
    /* [out] */ FragmentState** ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = new FragmentState(in);
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

FragmentState::ParcelableCreatorFragmentState::NewArray(
    /* [in] */ Int32 size,
    /* [out] */ ArrayOf<FragmentState*>** ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = ArrayOf<FragmentState*>::Alloc(size);
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

//===================================================================
// FragmentState
//===================================================================

CAR_INTERFACE_IMPL(FragmentState, Object, IParcelable)

FragmentState::FragmentState(
    /* [in] */ IFragment* frag)
{
//     mClassName = frag.getClass().getName();
    frag->GetIndex(&mIndex);
    frag->GetFromLayout(&mFromLayout);
    frag->GetFragmentId(&mFragmentId);
    frag->GetContainerId(&mContainerId);
    frag->GetTag(&mTag);
    frag->GetRetainInstance(&mRetainInstance);
    frag->IsDetached(&mDetached);
    frag->GetArguments((IBundle**)&mArguments);
}

FragmentState::FragmentState(
    /* [in] */ IParcel* in)
{
    ReadFromParcel(in);
}

ECode FragmentState::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    FAIL_RETURN(in->ReadString(&mClassName));
    FAIL_RETURN(in->ReadInt32(&mIndex));
    Int32 value;
    FAIL_RETURN(in->ReadInt32(&value))
    mFromLayout = value != 0;
    FAIL_RETURN(in->ReadInt32(&mFragmentId));
    FAIL_RETURN(in->ReadInt32(&mContainerId));
    FAIL_RETURN(in->ReadString(&mTag));
    FAIL_RETURN(in->ReadInt32(&value))
    mRetainInstance = value != 0;
    FAIL_RETURN(in->ReadInt32(&value))
    mDetached = value != 0;
    FAIL_RETURN(in->ReadInterfacePtr((Handle32*)(IBundle**)&mArguments));
    FAIL_RETURN(in->ReadInterfacePtr((Handle32*)(IBundle**)&mSavedFragmentState));
    return NOERROR;
}

ECode FragmentState::Instantiate(
    /* [in] */ IActivity* activity,
    /* [in] */ IFragment* parent,
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);
    *fragment = NULL;
    VALIDATE_NOT_NULL(activity);
    VALIDATE_NOT_NULL(parent);

    if (mInstance != NULL) {
        *fragment = mInstance;
        REFCOUNT_ADD(*fragment);
        return NOERROR;
    }

    if (mArguments != NULL) {
        AutoPtr<IClassLoader> classLoader;
        activity->GetClassLoader((IClassLoader**)&classLoader);
        mArguments->SetClassLoader(classLoader);
    }

    Fragment::Instantiate(activity, mClassName, mArguments, (IFragment**)&mInstance);

    if (mSavedFragmentState != NULL) {
        AutoPtr<IClassLoader> classLoader;
        activity->GetClassLoader((IClassLoader**)&classLoader);
        mSavedFragmentState->SetClassLoader(classLoader);
        mInstance->SetSavedFragmentState(mSavedFragmentState);
    }

    AutoPtr<Activity> act = (Activity*)activity;
    mInstance->SetIndex(mIndex, parent);
    mInstance->SetFromLayout(mFromLayout);
    mInstance->SetRestored(TRUE);
    mInstance->SetFragmentId(mFragmentId);
    mInstance->SetContainerId(mContainerId);
    mInstance->SetTag(mTag);
    mInstance->SetRetainInstance(mRetainInstance);
    mInstance->SetDetached(mDetached);
    mInstance->SetFragmentManager(act->mFragments);
    AutoPtr<IFragmentManagerImplHelper> fHelper;
    CFragmentManagerImplHelper::AcquireSingleton((IFragmentManagerImplHelper**)&fHelper);
    Boolean debug;
    fHelper->GetDEBUG(&debug);
    if (debug) Slogger::V(IFragmentManagerImpl::TAG,
            "Instantiated fragment %p", mInstance.Get());

    *fragment = mInstance;
    REFCOUNT_ADD(*fragment);
    return NOERROR;
}

ECode FragmentState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    FAIL_RETURN(dest->WriteString(mClassName));
    FAIL_RETURN(dest->WriteInt32(mIndex));
    FAIL_RETURN(dest->WriteInt32(mFromLayout ? 1 : 0));
    FAIL_RETURN(dest->WriteInt32(mFragmentId));
    FAIL_RETURN(dest->WriteInt32(mContainerId));
    FAIL_RETURN(dest->WriteString(mTag));
    FAIL_RETURN(dest->WriteInt32(mRetainInstance ? 1 : 0));
    FAIL_RETURN(dest->WriteInt32(mDetached ? 1 : 0));
    FAIL_RETURN(dest->WriteInterfacePtr(mArguments));
    FAIL_RETURN(dest->WriteInterfacePtr(mSavedFragmentState));
    return NOERROR;
}

//===================================================================
// Fragment
//===================================================================

AutoPtr<ITransition> InitUSE_DEFAULT_TRANSITION()
{
    AutoPtr<CTransition> transit;
    CTransition::NewByFriend((CTransition**)&transit);
    return (ITransition*)transit.Get();
}

AutoPtr<ITransition> Fragment::USE_DEFAULT_TRANSITION = InitUSE_DEFAULT_TRANSITION();

HashMap<String, AutoPtr<IClassInfo> > Fragment::sClassMap;

    , public IFragment
    , public IComponentCallbacks
    , public IComponentCallbacks2
    , public IViewOnCreateContextMenuListener

CAR_INTERFACE_IMPL_4(Fragment, Object, IFragment, IComponentCallbacks, IComponentCallbacks2, IViewOnCreateContextMenuListener)

Fragment::Fragment()
    : mState(INITIALIZING)
    , mAnimatingAway(NULL)
    , mStateAfterAnimating(0)
    , mSavedFragmentState(NULL)
    , mSavedViewState(NULL)
    , mIndex(-1)
    , mWho(String(NULL))
    , mArguments(NULL)
    , mTarget(NULL)
    , mTargetIndex(-1)
    , mTargetRequestCode(0)
    , mAdded(FALSE)
    , mRemoving(FALSE)
    , mResumed(FALSE)
    , mFromLayout(FALSE)
    , mInLayout(FALSE)
    , mRestored(FALSE)
    , mBackStackNesting(0)
    , mFragmentManager(NULL)
    , mActivity(NULL)
    , mChildFragmentManager(NULL)
    , mParentFragment(NULL)
    , mFragmentId(0)
    , mContainerId(0)
    , mTag(String(NULL))
    , mHidden(FALSE)
    , mDetached(FALSE)
    , mRetainInstance(FALSE)
    , mRetaining(FALSE)
    , mHasMenu(FALSE)
    , mMenuVisible(TRUE)
    , mCalled(FALSE)
    , mNextAnim(0)
    , mContainer(NULL)
    , mView(NULL)
    , mDeferStart(FALSE)
    , mUserVisibleHint(TRUE)
    , mLoaderManager(NULL)
    , mLoadersStarted(FALSE)
    , mCheckedForLoaderManager(FALSE)
{}

Fragment~Fragment()
{}

ECode Fragment::Initialize()
{
    return NOERROR;
}


ECode Fragment::GetState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;
    return NOERROR;
}

ECode Fragment::SetState(
    /* [in] */ Int32 state)
{
    mState = state;
    return NOERROR;
}

ECode Fragment::GetAnimatingAway(
    /* [out] */ IAnimator** animator)
{
    VALIDATE_NOT_NULL(animator);
    *animator = mAnimatingAway;
    REFCOUNT_ADD(*animator);
    return NOERROR;
}

ECode Fragment::SetAnimatingAway(
    /* [in] */ IAnimator* animator)
{
    mAnimatingAway = animator;
    return NOERROR;
}

ECode Fragment::GetStateAfterAnimating(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mStateAfterAnimating;
    return NOERROR;
}

ECode Fragment::SetStateAfterAnimating(
    /* [in] */ Int32 state)
{
    mStateAfterAnimating = state;
    return NOERROR;
}

ECode Fragment::GetSavedFragmentState(
    /* [out] */ IBundle** fState)
{
    VALIDATE_NOT_NULL(fState);
    *fState = mSavedFragmentState;
    REFCOUNT_ADD(*fState);
    return NOERROR;
}

ECode Fragment::SetSavedFragmentState(
    /* [in] */ IBundle* fState)
{
    mSavedFragmentState = fState;
    return NOERROR;
}

ECode Fragment::GetSavedViewState(
    /* [out] */ IObjectInt32Map** viewState)
{
    VALIDATE_NOT_NULL(viewState);
    *viewState = mSavedViewState;
    REFCOUNT_ADD(*viewState);
    return NOERROR;
}

ECode Fragment::SetSavedViewState(
    /* [in] */ IObjectInt32Map* viewState)
{
    mSavedViewState = viewState;
    return NOERROR;
}

ECode Fragment::GetIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mIndex;
    return NOERROR;
}

ECode Fragment::GetWho(
    /* [out] */ String* who)
{
    VALIDATE_NOT_NULL(who);
    *who = mWho;
    return NOERROR;
}

ECode Fragment::SetWho(
    /* [in] */ const String& who)
{
    mWho = who;
    return NOERROR;
}

ECode Fragment::GetTarget(
    /* [out] */ IFragment** target)
{
    VALIDATE_NOT_NULL(target);
    *target = mTarget;
    REFCOUNT_ADD(*target);
    return NOERROR;
}

ECode Fragment::SetTarget(
    /* [in] */ IFragment* target)
{
    mTarget = target;
    return NOERROR;
}

ECode Fragment::GetTargetIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mTargetIndex;
    return NOERROR;
}

ECode Fragment::SetTargetIndex(
    /* [in] */ Int32 index)
{
    mTargetIndex = index;
    return NOERROR;
}

ECode Fragment::SetTargetRequestCode(
    /* [in] */ Int32 code)
{
    mTargetRequestCode = code;
    return NOERROR;
}

ECode Fragment::GetAdded(
    /* [out] */ Boolean* added)
{
    VALIDATE_NOT_NULL(added);
    *added = mAdded;
    return NOERROR;
}

ECode Fragment::SetAdded(
    /* [in] */ Boolean added)
{
    mAdded = added;
    return NOERROR;
}

ECode Fragment::SetRemoving(
    /* [in] */ Boolean removing)
{
    mRemoving = removing;
    return NOERROR;
}

ECode Fragment::SetResumed(
    /* [in] */ Boolean resumed)
{
    mResumed = resumed;
    return NOERROR;
}

ECode Fragment::GetFromLayout(
    /* [out] */ Boolean* fLayout)
{
    VALIDATE_NOT_NULL(fLayout);
    *fLayout = mFromLayout;
    return NOERROR;
}

ECode Fragment::SetFromLayout(
    /* [in] */ Boolean fLayout)
{
    mFromLayout = fLayout;
    return NOERROR;
}

ECode Fragment::SetInLayout(
    /* [in] */ Boolean inLayout)
{
    mInLayout = inLayout;
    return NOERROR;
}

ECode Fragment::GetRestored(
    /* [out] */ Boolean* restored)
{
    VALIDATE_NOT_NULL(restored);
    *restored = mRestored;
    return NOERROR;
}

ECode Fragment::SetRestored(
    /* [in] */ Boolean restored)
{
    mRestored = restored;
    return NOERROR;
}

ECode Fragment::GetBackStackNesting(
    /* [out] */ Int32* bsNesting)
{
    VALIDATE_NOT_NULL(bsNesting);
    *bsNesting = mBackStackNesting;
    return NOERROR;
}

ECode Fragment::SetBackStackNesting(
    /* [in] */ Int32 bsNesting)
{
    mBackStackNesting = bsNesting;
    return NOERROR;
}

ECode Fragment::SetFragmentManager(
    /* [in] */ IFragmentManagerImpl* fManager)
{
    mFragmentManager = fManager;
    return NOERROR;
}

ECode Fragment::SetActivity(
    /* [in] */ IActivity* activity)
{
    mActivity = activity;
    return NOERROR;
}

ECode Fragment::SetChildFragmentManager(
    /* [in] */ IFragmentManagerImpl* cfManager)
{
    mChildFragmentManager = cfManager;
    return NOERROR;
}

ECode Fragment::GetChildFragmentManagerValue(
    /* [out] */ IFragmentManagerImpl** cfManager)
{
    VALIDATE_NOT_NULL(cfManager);
    *cfManager = mChildFragmentManager;
    REFCOUNT_ADD(*cfManager);
    return NOERROR;
}

ECode Fragment::SetParentFragment(
    /* [in] */ IFragment* pFragment)
{
    mParentFragment = pFragment;
    return NOERROR;
}

ECode Fragment::GetFragmentId(
    /* [out] */ Int32* fid)
{
    VALIDATE_NOT_NULL(fid);
    *fid = mFragmentId;
    return NOERROR;
}

ECode Fragment::SetFragmentId(
    /* [in] */ Int32 fid)
{
    mFragmentId = fid;
    return NOERROR;
}

ECode Fragment::GetContainerId(
    /* [out] */ Int32* cid)
{
    VALIDATE_NOT_NULL(cid);
    *cid = mContainerId;
    return NOERROR;
}

ECode Fragment::SetContainerId(
    /* [in] */ Int32 cid)
{
    mContainerId = cid;
    return NOERROR;
}

ECode Fragment::SetTag(
    /* [in] */ const String& tag)
{
    mTag = tag;
    return NOERROR;
}

ECode Fragment::SetHidden(
    /* [in] */ Boolean hidden)
{
    mHidden = hidden;
    return NOERROR;
}

ECode Fragment::SetDetached(
    /* [in] */ Boolean detached)
{
    mDetached = detached;
    return NOERROR;
}

ECode Fragment::GetRetaining(
    /* [out] */ Boolean* retaining)
{
    VALIDATE_NOT_NULL(retaining);
    *retaining = mRetaining;
    return NOERROR;
}

ECode Fragment::SetRetaining(
    /* [in] */ Boolean retaining)
{
    mRetaining = retaining;
    return NOERROR;
}

ECode Fragment::GetHasMenu(
    /* [out] */ Boolean* hasMenu)
{
    VALIDATE_NOT_NULL(hasMenu);
    *hasMenu = mHasMenu;
    return NOERROR;
}

ECode Fragment::SetHasMenu(
    /* [in] */ Boolean hasMenu)
{
    mHasMenu = hasMenu;
    return NOERROR;
}

ECode Fragment::GetMenuVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = mMenuVisible;
    return NOERROR;
}

ECode Fragment::SetMenuVisible(
    /* [in] */ Boolean visible)
{
    mMenuVisible = visible;
    return NOERROR;
}

ECode Fragment::GetCalled(
    /* [out] */ Boolean* called)
{
    VALIDATE_NOT_NULL(called);
    *called = mCalled;
    return NOERROR;
}

ECode Fragment::SetCalled(
    /* [in] */ Boolean called)
{
    mCalled = called;
    return NOERROR;
}

ECode Fragment::GetNextAnim(
    /* [out] */ Int32* anim)
{
    VALIDATE_NOT_NULL(anim);
    *anim = mNextAnim;
    return NOERROR;
}

ECode Fragment::SetNextAnim(
    /* [in] */ Int32 anim)
{
    mNextAnim = anim;
    return NOERROR;
}

ECode Fragment::GetContainer(
    /* [out] */ IViewGroup** container)
{
    VALIDATE_NOT_NULL(container);
    *container = mContainer;
    REFCOUNT_ADD(*container);
    return NOERROR;
}

ECode Fragment::SetContainer(
    /* [in] */ IViewGroup* container)
{
    mContainer = container;
    return NOERROR;
}

ECode Fragment::SetView(
    /* [in] */ IView* view)
{
    mView = view;
    return NOERROR;
}

ECode Fragment::GetDeferStart(
    /* [out] */ Boolean* start)
{
    VALIDATE_NOT_NULL(start);
    *start = mDeferStart;
    return NOERROR;
}

ECode Fragment::SetDeferStart(
    /* [in] */ Boolean start)
{
    mDeferStart = start;
    return NOERROR;
}

ECode Fragment::SetLoaderManager(
    /* [in] */ ILoaderManagerImpl* lManager)
{
    mLoaderManager = lManager;
    return NOERROR;
}

ECode Fragment::GetLoaderManagerValue(
    /* [out] */ ILoaderManagerImpl** lManager)
{
    VALIDATE_NOT_NULL(lManager);
    *lManager = mLoaderManager;
    REFCOUNT_ADD(*lManager);
    return NOERROR;
}

ECode Fragment::GetLoadersStarted(
    /* [out] */ Boolean* started)
{
    VALIDATE_NOT_NULL(started);
    *started = mLoadersStarted;
    return NOERROR;
}

ECode Fragment::SetLoadersStarted(
    /* [in] */ Boolean started)
{
    mLoadersStarted = started;
    return NOERROR;
}

ECode Fragment::GetCheckedForLoaderManager(
    /* [out] */ Boolean* cfManager)
{
    VALIDATE_NOT_NULL(cfManager);
    *cfManager = mCheckedForLoaderManager;
    return NOERROR;
}

ECode Fragment::SetCheckedForLoaderManager(
    /* [in] */ Boolean cfManager)
{
    mCheckedForLoaderManager = cfManager;
    return NOERROR;
}

ECode Fragment::Instantiate(
    /* [in] */ IContext* context,
    /* [in] */ const String& fname,
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);
    return Instantiate(context, fname, NULL, fragment);
}

ECode Fragment::Instantiate(
    /* [in] */ IContext* context,
    /* [in] */ const String& fname,
    /* [in] */ IBundle* args,
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);
//     try {
        AutoPtr<IClassInfo> clazz;
        HashMap<String, AutoPtr<IClassInfo> >::Iterator it = sClassMap.Find(fname);
        if (it == sClassMap.End()) {
//             // Class not found in the cache, see if it's real, and try to add it
            AutoPtr<IClassLoader> classLoader;
            context->GetClassLoader((IClassLoader**)&classLoader);
            classLoader->LoadClass(fname, (IClassInfo**)&clazz);
            if (!Fragment.class.isAssignableFrom(clazz)) {
                throw new InstantiationException("Trying to instantiate a class " + fname
                        + " that is not a Fragment", new ClassCastException());
            }

            sClassMap[fname] = clazz;
        }
        else {
            clazz = it->mSecond;
        }

        AutoPtr<IInterface> obj;
        ECode ec = clazz->CreateObject((IInterface**)&obj);
        AutoPtr<IFragment> f = IFragment::Probe(obj);
        if (args != NULL) {
            AutoPtr<IClassLoader> cLoader;
//TODO:
//            args.setClassLoader(f.getClass().getClassLoader());
            f->SetArguments(args);
        }
        *fragment = f;
        REFCOUNT_ADD(*fragment);
        return NOERROR;
//     } catch (ClassNotFoundException e) {
//         throw new InstantiationException("Unable to instantiate fragment " + fname
//                 + ": make sure class name exists, is public, and has an"
//                 + " empty constructor that is public", e);
//     } catch (java.lang.InstantiationException e) {
//         throw new InstantiationException("Unable to instantiate fragment " + fname
//                 + ": make sure class name exists, is public, and has an"
//                 + " empty constructor that is public", e);
//     } catch (IllegalAccessException e) {
//         throw new InstantiationException("Unable to instantiate fragment " + fname
//                 + ": make sure class name exists, is public, and has an"
//                 + " empty constructor that is public", e);
//     }
}

ECode Fragment::RestoreViewState(
    /* [in] */ IBundle* savedInstanceState)
{
    if (mSavedViewState != NULL) {
        mView->RestoreHierarchyState(mSavedViewState);
        mSavedViewState = NULL;
    }
    mCalled = FALSE;
    OnViewStateRestored(savedInstanceState);
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onViewStateRestored()");
    }
    return NOERROR;
}

ECode Fragment::SetIndex(
    /* [in] */ Int32 index,
    /* [in] */ IFragment* parent)
{
    mIndex = index;
    if (parent != NULL) {
        String who;
        parent->GetWho(&who);
        StringBuilder sb(who);
        sb += ":";
        sb += mIndex;
        mWho =  sb.ToString();
    } else {
        StringBuilder sb("android:fragment:");
        sb += mIndex;
        mWho = sb.ToString();
    }
    return NOERROR;
}

Fragment::IsInBackStack(
    /* [out] */ Boolean* inbs)
{
    VALIDATE_NOT_NULL(inbs);
    *inbs = mBackStackNesting > 0;
    return NOERROR;
}

ECode Fragment::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* equal)
{
    return Object::Equals(o, equal);
}

ECode Fragment::GetHashCode(
    /* [out] */ Int32* code)
{
    return Object::GetHashCode(code);
}

ECode Fragment::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    StringBuilder sb(128);
//     DebugUtils.buildShortClassTag(this, sb);
    if (mIndex >= 0) {
        sb.Append(" #");
        sb.Append(mIndex);
    }
    if (mFragmentId != 0) {
        sb.Append(" id=0x");
        sb::Append(StringUtils::ToHexString(mFragmentId));
    }
    if (mTag != NULL) {
        sb.Append(" ");
        sb.Append(mTag);
    }
    sb.Append("}");
    *string = sb.ToString();
    return NOERROR;
}

ECode Fragment::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);

    *id = mFragmentId;
    return NOERROR;
}

ECode Fragment::GetTag(
    /* [out] */ String* tag)
{
    VALIDATE_NOT_NULL(tag);

    *tag = mTag;
    return NOERROR;
}

ECode Fragment::SetArguments(
    /* [in] */ IBundle* args)
{
    if (mIndex >= 0) {
//         throw new IllegalStateException("Fragment already active");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mArguments = args;
    return NOERROR;
}

ECode Fragment::GetArguments(
    /* [out] */ IBundle** args)
{
    VALIDATE_NOT_NULL(args);

    *args = mArguments;
    REFCOUNT_ADD(*args);
    return NOERROR;
}

ECode Fragment::SetInitialSavedState(
    /* [in] */ IFragmentSavedState* state)
{
    if (mIndex >= 0) {
//         throw new IllegalStateException("Fragment already active");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<IBundle> fState;
    mSavedFragmentState = state != NULL && (state->GetState((IBundle**)&fState), fState != NULL)
            ? fState : NULL;
    return NOERROR;
}

ECode Fragment::SetTargetFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Int32 requestCode)
{
    mTarget = fragment;
    mTargetRequestCode = requestCode;
    return NOERROR;
}

ECode Fragment::GetTargetFragment(
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);

    *fragment = mTarget;
    REFCOUNT_ADD(*fragment);
    return NOERROR;
}

ECode Fragment::GetTargetRequestCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);

    *code = mTargetRequestCode;
    return NOERROR;
}

ECode Fragment::GetActivity(
    /* [out] */ IActivity** activity)
{
    VALIDATE_NOT_NULL(activity);

    *activity = mActivity;
    REFCOUNT_ADD(*activity);
    return NOERROR;
}

ECode Fragment::GetResources(
    /* [out] */ IResources** resources)
{
    VALIDATE_NOT_NULL(resources);

    if (mActivity == NULL) {
//         throw new IllegalStateException("Fragment " + this + " not attached to Activity");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mActivity->GetResources(resources);
    return NOERROR;
}

ECode Fragment::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);

    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetText(resId, text);
    return NOERROR;
}

ECode Fragment::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetString(resId, string);
    return NOERROR;
}

ECode Fragment::GetString(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetString(resId, formatArgs, string);
    return NOERROR;
}

ECode Fragment::GetFragmentManager(
    /* [out] */ IFragmentManager** manager)
{
    VALIDATE_NOT_NULL(manager);

    *manager = mFragmentManager;
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

ECode Fragment::GetChildFragmentManager(
    /* [out] */ IFragmentManager** manager)
{
    VALIDATE_NOT_NULL(manager);

    if (mChildFragmentManager == NULL) {
        InstantiateChildFragmentManager();
        if (mState >= IFragment::RESUMED) {
            mChildFragmentManager->DispatchResume();
        } else if (mState >= IFragment::STARTED) {
            mChildFragmentManager->DispatchStart();
        } else if (mState >= IFragment::ACTIVITY_CREATED) {
            mChildFragmentManager->DispatchActivityCreated();
        } else if (mState >= IFragment::CREATED) {
            mChildFragmentManager->DispatchCreate();
        }
    }
    *manager = mChildFragmentManager;
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

ECode Fragment::GetParentFragment(
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);

    *fragment = mParentFragment;
    REFCOUNT_ADD(*fragment);
    return NOERROR;
}

ECode Fragment::IsAdded(
    /* [out] */ Boolean* added)
{
    VALIDATE_NOT_NULL(added);

    *added = mActivity != NULL && mAdded;
    return NOERROR;
}

ECode Fragment::IsDetached(
    /* [out] */ Boolean* detached)
{
    VALIDATE_NOT_NULL(detached);

    *detached = mDetached;
    return NOERROR;
}

ECode Fragment::IsRemoving(
    /* [out] */ Boolean* removing)
{
    VALIDATE_NOT_NULL(removing);

    *removing = mRemoving;
    return NOERROR;
}

ECode Fragment::IsInLayout(
    /* [out] */ Boolean* inlayout)
{
    VALIDATE_NOT_NULL(inlayout);
    *inlayout = mInLayout;
    return NOERROR;
}

ECode Fragment::IsResumed(
    /* [out] */ Boolean* resumed)
{
    VALIDATE_NOT_NULL(resumed);

    *resumed = mResumed;
    return NOERROR;
}

ECode Fragment::IsVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);

    Boolean added, hidden;
    IsAdded(&added);
    IsHidden(&hidden);
    AutoPtr<IBinder> token;
    mView->GetWindowToken((IBinder**)&token);
    Int32 visibility;
    mView->GetVisibility(&visibility);
    *visible = added && !hidden && mView != NULL
            && token != NULL && visibility == IView::VISIBLE;
    return NOERROR;
}

ECode Fragment::IsHidden(
    /* [out] */ Boolean* hidden)
{
    VALIDATE_NOT_NULL(hidden);
    *hidden = mHidden;
    return NOERROR;
}

ECode Fragment::OnHiddenChanged(
    /* [in] */ Boolean hidden)
{
    return NOERROR;
}

ECode Fragment::SetRetainInstance(
    /* [in] */ Boolean retain)
{
    if (retain && mParentFragment != NULL) {
//         throw new IllegalStateException(
//                 "Can't retain fragements that are nested in other fragments");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mRetainInstance = retain;
    return NOERROR;
}

ECode Fragment::GetRetainInstance(
    /* [out] */ Boolean* retainInstance)
{
    VALIDATE_NOT_NULL(retainInstance);
    *retainInstance = mRetainInstance;
    return NOERROR;
}

ECode Fragment::SetHasOptionsMenu(
    /* [in] */ Boolean hasMenu)
{
    if (mHasMenu != hasMenu) {
        mHasMenu = hasMenu;
        Boolean added, hidden;
        IsAdded(&added);
        IsHidden(&hidden);
        if (added && !hidden) {
            mFragmentManager->InvalidateOptionsMenu();
        }
    }
    return NOERROR;
}

ECode Fragment::SetMenuVisibility(
    /* [in] */ Boolean menuVisible)
{
    if (mMenuVisible != menuVisible) {
        mMenuVisible = menuVisible;
        Boolean added, hidden;
        IsAdded(&added);
        IsHidden(&hidden);
        if (mHasMenu && added && !hidden) {
            mFragmentManager->InvalidateOptionsMenu();
        }
    }
    return NOERROR;
}

ECode Fragment::SetUserVisibleHint(
    /* [in] */ Boolean isVisibleToUser)
{
    if (!mUserVisibleHint && isVisibleToUser && mState < IFragment::STARTED) {
        mFragmentManager->PerformPendingDeferredStart((IFragment*)this->Probe(EIID_IFragment));
    }
    mUserVisibleHint = isVisibleToUser;
    mDeferStart = !isVisibleToUser;
    return NOERROR;
}

ECode Fragment::GetUserVisibleHint(
    /* [out] */ Boolean* hint)
{
    VALIDATE_NOT_NULL(hint);
    *hint = mUserVisibleHint;
    return NOERROR;
}

ECode Fragment::GetLoaderManager(
    /* [out] */ ILoaderManager** manager)
{
    VALIDATE_NOT_NULL(manager);

    if (mLoaderManager != NULL) {
        *manager = mLoaderManager;
        REFCOUNT_ADD(*manager);
    }
    if (mActivity == NULL) {
//         throw new IllegalStateException("Fragment " + this + " not attached to Activity");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mCheckedForLoaderManager = TRUE;
    AutoPtr<Activity> act = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
    mLoaderManager = act->GetLoaderManager(mWho, mLoadersStarted, TRUE);
    *manager = mLoaderManager;
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

ECode Fragment::StartActivity(
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent);

    StartActivity(intent, NULL);
    return NOERROR;
}

ECode Fragment::StartActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
    VALIDATE_NOT_NULL(intent);
    VALIDATE_NOT_NULL(options);

    if (mActivity == NULL) {
//         throw new IllegalStateException("Fragment " + this + " not attached to Activity");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (options != NULL) {
        mActivity->StartActivityFromFragment((IFragment*)this->Probe(EIID_IFragment), intent, -1, options);
    } else {
        // Note we want to go through this call for compatibility with
        // applications that may have overridden the method.
        mActivity->StartActivityFromFragment((IFragment*)this->Probe(EIID_IFragment), intent, -1);
    }
    return NOERROR;
}

ECode Fragment::StartActivityForResult(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode)
{
    return StartActivityForResult(intent, requestCode, NULL);
}

ECode Fragment::StartActivityForResult(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options)
{
    if (mActivity == NULL) {
//         throw new IllegalStateException("Fragment " + this + " not attached to Activity");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (options != NULL) {
        mActivity->StartActivityFromFragment((IFragment*)this->Probe(EIID_IFragment), intent, requestCode, options);
    } else {
        // Note we want to go through this call for compatibility with
        // applications that may have overridden the method.
        mActivity->StartActivityFromFragment((IFragment*)this->Probe(EIID_IFragment), intent, requestCode, options);
    }
    return NOERROR;
}

ECode Fragment::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    return NOERROR;
}

ECode Fragment::GetLayoutInflater(
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);

    // Newer platform versions use the child fragment manager's LayoutInflaterFactory.
    if (mActivity.getApplicationInfo().targetSdkVersion >= Build.VERSION_CODES.LOLLIPOP) {
        LayoutInflater result = mActivity.getLayoutInflater().cloneInContext(mActivity);
        getChildFragmentManager(); // Init if needed; use raw implementation below.
        result.setPrivateFactory(mChildFragmentManager.getLayoutInflaterFactory());
        return result;
    }

    return mActivity->GetLayoutInflater(inflater);
}

ECode Fragment::OnInflate(
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IBundle* savedInstanceState)
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnInflate(
    /* [in] */ IActivity* activity,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IBundle* savedInstanceState)
{
    OnInflate(attrs, savedInstanceState);
    mCalled = true;

    TypedArray a = activity.obtainStyledAttributes(attrs,
            com.android.internal.R.styleable.Fragment);
    mEnterTransition = loadTransition(activity, a, mEnterTransition, null,
            com.android.internal.R.styleable.Fragment_fragmentEnterTransition);
    mReturnTransition = loadTransition(activity, a, mReturnTransition, USE_DEFAULT_TRANSITION,
            com.android.internal.R.styleable.Fragment_fragmentReturnTransition);
    mExitTransition = loadTransition(activity, a, mExitTransition, null,
            com.android.internal.R.styleable.Fragment_fragmentExitTransition);
    mReenterTransition = loadTransition(activity, a, mReenterTransition, USE_DEFAULT_TRANSITION,
            com.android.internal.R.styleable.Fragment_fragmentReenterTransition);
    mSharedElementEnterTransition = loadTransition(activity, a, mSharedElementEnterTransition,
            null, com.android.internal.R.styleable.Fragment_fragmentSharedElementEnterTransition);
    mSharedElementReturnTransition = loadTransition(activity, a, mSharedElementReturnTransition,
            USE_DEFAULT_TRANSITION,
            com.android.internal.R.styleable.Fragment_fragmentSharedElementReturnTransition);
    if (mAllowEnterTransitionOverlap == null) {
        mAllowEnterTransitionOverlap = a.getBoolean(
                com.android.internal.R.styleable.Fragment_fragmentAllowEnterTransitionOverlap, true);
    }
    if (mAllowReturnTransitionOverlap == null) {
        mAllowReturnTransitionOverlap = a.getBoolean(
                com.android.internal.R.styleable.Fragment_fragmentAllowReturnTransitionOverlap, true);
    }
    a.recycle();
    return NOERROR;
}

ECode Fragment::OnAttach(
    /* [in] */ IActivity* activity)
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnCreateAnimator(
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter,
    /* [in] */ Int32 nextAnim,
    /* [out] */ IAnimator** animator)
{
    VALIDATE_NOT_NULL(animator);
    *animator = NULL;
    return NOERROR;
}

ECode Fragment::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnCreateView(
    /* [in] */ ILayoutInflater* inflater,
    /* [in] */ IViewGroup* container,
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;
    return NOERROR;
}

ECode Fragment::OnViewCreated(
    /* [in] */ IView* view,
    /* [in] */ IBundle* savedInstanceState)
{
    return NOERROR;
}

ECode Fragment::GetView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode Fragment::OnActivityCreated(
    /* [in] */ IBundle* savedInstanceState)
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnViewStateRestored(
    /* [in] */ IBundle* savedInstanceState)
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnStart()
{
    mCalled = TRUE;

    if (!mLoadersStarted) {
        mLoadersStarted = TRUE;
        if (!mCheckedForLoaderManager) {
            mCheckedForLoaderManager = TRUE;
            AutoPtr<Activity> act = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
            mLoaderManager = act->GetLoaderManager(mWho, mLoadersStarted, FALSE);
        }
        if (mLoaderManager != NULL) {
            mLoaderManager->DoStart();
        }
    }
    return NOERROR;
}

ECode Fragment::OnResume()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    return NOERROR;
}

ECode Fragment::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnPause()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnStop()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnLowMemory()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnTrimMemory(
        /* [in] */ Int32 level)
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnDestroyView()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnDestroy()
{
    mCalled = TRUE;
    //Log.v("foo", "onDestroy: mCheckedForLoaderManager=" + mCheckedForLoaderManager
    //        + " mLoaderManager=" + mLoaderManager);
    if (!mCheckedForLoaderManager) {
        mCheckedForLoaderManager = TRUE;
        AutoPtr<Activity> act = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
        mLoaderManager = act->GetLoaderManager(mWho, mLoadersStarted, FALSE);
    }
    if (mLoaderManager != NULL) {
        mLoaderManager->DoDestroy();
    }
    return NOERROR;
}

ECode Fragment::InitState()
{
    mIndex = -1;
    mWho = NULL;
    mAdded = FALSE;
    mRemoving = FALSE;
    mResumed = FALSE;
    mFromLayout = FALSE;
    mInLayout = FALSE;
    mRestored = FALSE;
    mBackStackNesting = 0;
    mFragmentManager = NULL;
    mChildFragmentManager = NULL;
    mActivity = NULL;
    mFragmentId = 0;
    mContainerId = 0;
    mTag = NULL;
    mHidden = FALSE;
    mDetached = FALSE;
    mRetaining = FALSE;
    mLoaderManager = NULL;
    mLoadersStarted = FALSE;
    mCheckedForLoaderManager = FALSE;
    return NOERROR;
}

ECode Fragment::OnDetach()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Fragment::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [in] */ IMenuInflater* inflater)
{
    return NOERROR;
}

ECode Fragment::OnPrepareOptionsMenu(
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

ECode Fragment::OnDestroyOptionsMenu()
{
    return NOERROR;
}

ECode Fragment::OnOptionsItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* selected)
{
    VALIDATE_NOT_NULL(selected);
    *selected = FALSE;
    return NOERROR;
}

ECode Fragment::OnOptionsMenuClosed(
    /* [in] */ IMenu* menu)
{
    return NOERROR;
}

ECode Fragment::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    AutoPtr<IActivity> activity;
    GetActivity((IActivity**)&activity);
    activity->OnCreateContextMenu(menu, v, menuInfo);
    return NOERROR;
}

ECode Fragment::RegisterForContextMenu(
    /* [in] */ IView* view)
{
    view->SetOnCreateContextMenuListener(
        (IViewOnCreateContextMenuListener*)this->Probe(EIID_IViewOnCreateContextMenuListener));
    return NOERROR;
}

ECode Fragment::UnregisterForContextMenu(
    /* [in] */ IView* view)
{
    view->SetOnCreateContextMenuListener(NULL);
    return NOERROR;
}

ECode Fragment::OnContextItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* selected)
{
    VALIDATE_NOT_NULL(selected);
    *selected = FALSE;
    return NOERROR;
}

//=====================================================

/**
 * When custom transitions are used with Fragments, the enter transition callback
 * is called when this Fragment is attached or detached when not popping the back stack.
 *
 * @param callback Used to manipulate the shared element transitions on this Fragment
 *                 when added not as a pop from the back stack.
 */
public void setEnterSharedElementCallback(SharedElementCallback callback) {
    if (callback == null) {
        callback = SharedElementCallback.NULL_CALLBACK;
    }
    mEnterTransitionCallback = callback;
}

/**
 * @hide
 */
public void setEnterSharedElementTransitionCallback(SharedElementCallback callback) {
    setEnterSharedElementCallback(callback);
}

/**
 * When custom transitions are used with Fragments, the exit transition callback
 * is called when this Fragment is attached or detached when popping the back stack.
 *
 * @param callback Used to manipulate the shared element transitions on this Fragment
 *                 when added as a pop from the back stack.
 */
public void setExitSharedElementCallback(SharedElementCallback callback) {
    if (callback == null) {
        callback = SharedElementCallback.NULL_CALLBACK;
    }
    mExitTransitionCallback = callback;
}

/**
 * @hide
 */
public void setExitSharedElementTransitionCallback(SharedElementCallback callback) {
    setExitSharedElementCallback(callback);
}

/**
 * Sets the Transition that will be used to move Views into the initial scene. The entering
 * Views will be those that are regular Views or ViewGroups that have
 * {@link ViewGroup#isTransitionGroup} return true. Typical Transitions will extend
 * {@link android.transition.Visibility} as entering is governed by changing visibility from
 * {@link View#INVISIBLE} to {@link View#VISIBLE}. If <code>transition</code> is null,
 * entering Views will remain unaffected.
 *
 * @param transition The Transition to use to move Views into the initial Scene.
 * @attr ref android.R.styleable#Fragment_fragmentEnterTransition
 */
public void setEnterTransition(Transition transition) {
    mEnterTransition = transition;
}

/**
 * Returns the Transition that will be used to move Views into the initial scene. The entering
 * Views will be those that are regular Views or ViewGroups that have
 * {@link ViewGroup#isTransitionGroup} return true. Typical Transitions will extend
 * {@link android.transition.Visibility} as entering is governed by changing visibility from
 * {@link View#INVISIBLE} to {@link View#VISIBLE}.
 *
 * @return the Transition to use to move Views into the initial Scene.
 * @attr ref android.R.styleable#Fragment_fragmentEnterTransition
 */
public Transition getEnterTransition() {
    return mEnterTransition;
}

/**
 * Sets the Transition that will be used to move Views out of the scene when the Fragment is
 * preparing to be removed, hidden, or detached because of popping the back stack. The exiting
 * Views will be those that are regular Views or ViewGroups that have
 * {@link ViewGroup#isTransitionGroup} return true. Typical Transitions will extend
 * {@link android.transition.Visibility} as entering is governed by changing visibility from
 * {@link View#VISIBLE} to {@link View#INVISIBLE}. If <code>transition</code> is null,
 * entering Views will remain unaffected. If nothing is set, the default will be to
 * use the same value as set in {@link #setEnterTransition(android.transition.Transition)}.
 *
 * @param transition The Transition to use to move Views out of the Scene when the Fragment
 *                   is preparing to close.
 * @attr ref android.R.styleable#Fragment_fragmentExitTransition
 */
public void setReturnTransition(Transition transition) {
    mReturnTransition = transition;
}

/**
 * Returns the Transition that will be used to move Views out of the scene when the Fragment is
 * preparing to be removed, hidden, or detached because of popping the back stack. The exiting
 * Views will be those that are regular Views or ViewGroups that have
 * {@link ViewGroup#isTransitionGroup} return true. Typical Transitions will extend
 * {@link android.transition.Visibility} as entering is governed by changing visibility from
 * {@link View#VISIBLE} to {@link View#INVISIBLE}. If <code>transition</code> is null,
 * entering Views will remain unaffected.
 *
 * @return the Transition to use to move Views out of the Scene when the Fragment
 *         is preparing to close.
 * @attr ref android.R.styleable#Fragment_fragmentExitTransition
 */
public Transition getReturnTransition() {
    return mReturnTransition == USE_DEFAULT_TRANSITION ? getEnterTransition()
            : mReturnTransition;
}

/**
 * Sets the Transition that will be used to move Views out of the scene when the
 * fragment is removed, hidden, or detached when not popping the back stack.
 * The exiting Views will be those that are regular Views or ViewGroups that
 * have {@link ViewGroup#isTransitionGroup} return true. Typical Transitions will extend
 * {@link android.transition.Visibility} as exiting is governed by changing visibility
 * from {@link View#VISIBLE} to {@link View#INVISIBLE}. If transition is null, the views will
 * remain unaffected.
 *
 * @param transition The Transition to use to move Views out of the Scene when the Fragment
 *                   is being closed not due to popping the back stack.
 * @attr ref android.R.styleable#Fragment_fragmentExitTransition
 */
public void setExitTransition(Transition transition) {
    mExitTransition = transition;
}

/**
 * Returns the Transition that will be used to move Views out of the scene when the
 * fragment is removed, hidden, or detached when not popping the back stack.
 * The exiting Views will be those that are regular Views or ViewGroups that
 * have {@link ViewGroup#isTransitionGroup} return true. Typical Transitions will extend
 * {@link android.transition.Visibility} as exiting is governed by changing visibility
 * from {@link View#VISIBLE} to {@link View#INVISIBLE}. If transition is null, the views will
 * remain unaffected.
 *
 * @return the Transition to use to move Views out of the Scene when the Fragment
 *         is being closed not due to popping the back stack.
 * @attr ref android.R.styleable#Fragment_fragmentExitTransition
 */
public Transition getExitTransition() {
    return mExitTransition;
}

/**
 * Sets the Transition that will be used to move Views in to the scene when returning due
 * to popping a back stack. The entering Views will be those that are regular Views
 * or ViewGroups that have {@link ViewGroup#isTransitionGroup} return true. Typical Transitions
 * will extend {@link android.transition.Visibility} as exiting is governed by changing
 * visibility from {@link View#VISIBLE} to {@link View#INVISIBLE}. If transition is null,
 * the views will remain unaffected. If nothing is set, the default will be to use the same
 * transition as {@link #setExitTransition(android.transition.Transition)}.
 *
 * @param transition The Transition to use to move Views into the scene when reentering from a
 *                   previously-started Activity.
 * @attr ref android.R.styleable#Fragment_fragmentReenterTransition
 */
public void setReenterTransition(Transition transition) {
    mReenterTransition = transition;
}

/**
 * Returns the Transition that will be used to move Views in to the scene when returning due
 * to popping a back stack. The entering Views will be those that are regular Views
 * or ViewGroups that have {@link ViewGroup#isTransitionGroup} return true. Typical Transitions
 * will extend {@link android.transition.Visibility} as exiting is governed by changing
 * visibility from {@link View#VISIBLE} to {@link View#INVISIBLE}. If transition is null,
 * the views will remain unaffected. If nothing is set, the default will be to use the same
 * transition as {@link #setExitTransition(android.transition.Transition)}.
 *
 * @return the Transition to use to move Views into the scene when reentering from a
 *                   previously-started Activity.
 * @attr ref android.R.styleable#Fragment_fragmentReenterTransition
 */
public Transition getReenterTransition() {
    return mReenterTransition == USE_DEFAULT_TRANSITION ? getExitTransition()
            : mReenterTransition;
}

/**
 * Sets the Transition that will be used for shared elements transferred into the content
 * Scene. Typical Transitions will affect size and location, such as
 * {@link android.transition.ChangeBounds}. A null
 * value will cause transferred shared elements to blink to the final position.
 *
 * @param transition The Transition to use for shared elements transferred into the content
 *                   Scene.
 * @attr ref android.R.styleable#Fragment_fragmentSharedElementEnterTransition
 */
public void setSharedElementEnterTransition(Transition transition) {
    mSharedElementEnterTransition = transition;
}

/**
 * Returns the Transition that will be used for shared elements transferred into the content
 * Scene. Typical Transitions will affect size and location, such as
 * {@link android.transition.ChangeBounds}. A null
 * value will cause transferred shared elements to blink to the final position.
 *
 * @return The Transition to use for shared elements transferred into the content
 *                   Scene.
 * @attr ref android.R.styleable#Fragment_fragmentSharedElementEnterTransition
 */
public Transition getSharedElementEnterTransition() {
    return mSharedElementEnterTransition;
}

/**
 * Sets the Transition that will be used for shared elements transferred back during a
 * pop of the back stack. This Transition acts in the leaving Fragment.
 * Typical Transitions will affect size and location, such as
 * {@link android.transition.ChangeBounds}. A null
 * value will cause transferred shared elements to blink to the final position.
 * If no value is set, the default will be to use the same value as
 * {@link #setSharedElementEnterTransition(android.transition.Transition)}.
 *
 * @param transition The Transition to use for shared elements transferred out of the content
 *                   Scene.
 * @attr ref android.R.styleable#Fragment_fragmentSharedElementReturnTransition
 */
public void setSharedElementReturnTransition(Transition transition) {
    mSharedElementReturnTransition = transition;
}

/**
 * Return the Transition that will be used for shared elements transferred back during a
 * pop of the back stack. This Transition acts in the leaving Fragment.
 * Typical Transitions will affect size and location, such as
 * {@link android.transition.ChangeBounds}. A null
 * value will cause transferred shared elements to blink to the final position.
 * If no value is set, the default will be to use the same value as
 * {@link #setSharedElementEnterTransition(android.transition.Transition)}.
 *
 * @return The Transition to use for shared elements transferred out of the content
 *                   Scene.
 * @attr ref android.R.styleable#Fragment_fragmentSharedElementReturnTransition
 */
public Transition getSharedElementReturnTransition() {
    return mSharedElementReturnTransition == USE_DEFAULT_TRANSITION ?
            getSharedElementEnterTransition() : mSharedElementReturnTransition;
}

/**
 * Sets whether the the exit transition and enter transition overlap or not.
 * When true, the enter transition will start as soon as possible. When false, the
 * enter transition will wait until the exit transition completes before starting.
 *
 * @param allow true to start the enter transition when possible or false to
 *              wait until the exiting transition completes.
 * @attr ref android.R.styleable#Fragment_fragmentAllowEnterTransitionOverlap
 */
public void setAllowEnterTransitionOverlap(boolean allow) {
    mAllowEnterTransitionOverlap = allow;
}

/**
 * Returns whether the the exit transition and enter transition overlap or not.
 * When true, the enter transition will start as soon as possible. When false, the
 * enter transition will wait until the exit transition completes before starting.
 *
 * @return true when the enter transition should start as soon as possible or false to
 * when it should wait until the exiting transition completes.
 * @attr ref android.R.styleable#Fragment_fragmentAllowEnterTransitionOverlap
 */
public boolean getAllowEnterTransitionOverlap() {
    return (mAllowEnterTransitionOverlap == null) ? true : mAllowEnterTransitionOverlap;
}

/**
 * Sets whether the the return transition and reenter transition overlap or not.
 * When true, the reenter transition will start as soon as possible. When false, the
 * reenter transition will wait until the return transition completes before starting.
 *
 * @param allow true to start the reenter transition when possible or false to wait until the
 *              return transition completes.
 * @attr ref android.R.styleable#Fragment_fragmentAllowReturnTransitionOverlap
 */
public void setAllowReturnTransitionOverlap(boolean allow) {
    mAllowReturnTransitionOverlap = allow;
}

/**
 * Returns whether the the return transition and reenter transition overlap or not.
 * When true, the reenter transition will start as soon as possible. When false, the
 * reenter transition will wait until the return transition completes before starting.
 *
 * @return true to start the reenter transition when possible or false to wait until the
 *         return transition completes.
 * @attr ref android.R.styleable#Fragment_fragmentAllowReturnTransitionOverlap
 */
public boolean getAllowReturnTransitionOverlap() {
    return (mAllowReturnTransitionOverlap == null) ? true : mAllowReturnTransitionOverlap;
}

//=====================================================

ECode Fragment::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    writer->PrintString(prefix); writer->PrintString(String("mFragmentId=#"));
//             writer.print(Integer.toHexString(mFragmentId));
    writer->PrintString(String(" mContainerId=#"));
//             writer.print(Integer.toHexString(mContainerId));
    writer->PrintString(String(" mTag=")); writer->PrintStringln(mTag);
    writer->PrintString(prefix); writer->PrintString(String("mState="));
    writer->PrintInt32(mState);
    writer->PrintString(String(" mIndex=")); writer->PrintInt32(mIndex);
    writer->PrintString(String(" mWho=")); writer->PrintString(mWho);
    writer->PrintString(String(" mBackStackNesting="));
    writer->PrintInt32ln(mBackStackNesting);
    writer->PrintString(prefix); writer->PrintString(String("mAdded="));
    writer->PrintBoolean(mAdded);
            writer->PrintString(String(" mRemoving=")); writer->PrintBoolean(mRemoving);
            writer->PrintString(String(" mResumed=")); writer->PrintBoolean(mResumed);
            writer->PrintString(String(" mFromLayout=")); writer->PrintBoolean(mFromLayout);
            writer->PrintString(String(" mInLayout=")); writer->PrintBooleanln(mInLayout);
    writer->PrintString(prefix); writer->PrintString(String("mHidden="));
    writer->PrintBoolean(mHidden);
            writer->PrintString(String(" mDetached=")); writer->PrintBoolean(mDetached);
            writer->PrintString(String(" mMenuVisible=")); writer->PrintBoolean(mMenuVisible);
            writer->PrintString(String(" mHasMenu=")); writer->PrintBooleanln(mHasMenu);
    writer->PrintString(prefix); writer->PrintString(String("mRetainInstance="));
    writer->PrintBoolean(mRetainInstance);
            writer->PrintString(String(" mRetaining=")); writer->PrintBoolean(mRetaining);
            writer->PrintString(String(" mUserVisibleHint=")); writer->PrintBooleanln(mUserVisibleHint);
    if (mFragmentManager != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mFragmentManager="));
                writer->PrintObjectln((IInterface*)mFragmentManager);
    }
    if (mActivity != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mActivity="));
                writer->PrintObjectln((IInterface*)mActivity);
    }
    if (mParentFragment != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mParentFragment="));
                writer->PrintObjectln((IInterface*)mParentFragment);
    }
    if (mArguments != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mArguments="));
        writer->PrintObjectln((IInterface*)mArguments);
    }
    if (mSavedFragmentState != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mSavedFragmentState="));
                writer->PrintObjectln((IInterface*)mSavedFragmentState);
    }
    if (mSavedViewState != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mSavedViewState="));
                writer->PrintObjectln((IInterface*)mSavedViewState);
    }
    if (mTarget != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mTarget="));
        writer->PrintObject((IInterface*)mTarget);
                writer->PrintString(String(" mTargetRequestCode="));
                writer->PrintInt32ln(mTargetRequestCode);
    }
    if (mNextAnim != 0) {
        writer->PrintString(prefix); writer->PrintString(String("mNextAnim="));
        writer->PrintInt32ln(mNextAnim);
    }
    if (mContainer != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mContainer="));
        writer->PrintObjectln((IInterface*)mContainer);
    }
    if (mView != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mView="));
        writer->PrintObjectln((IInterface*)mView);
    }
    if (mAnimatingAway != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("mAnimatingAway="));
        writer->PrintObjectln((IInterface*)mAnimatingAway);
        writer->PrintString(prefix); writer->PrintString(String("mStateAfterAnimating="));
                writer->PrintInt32ln(mStateAfterAnimating);
    }
    if (mLoaderManager != NULL) {
        writer->PrintString(prefix); writer->PrintStringln(String("Loader Manager:"));
        mLoaderManager->Dump(prefix + "  ", fd, writer, args);
    }
    if (mChildFragmentManager != NULL) {
        StringBuilder sb("Child ");
        sb += mChildFragmentManager;
        sb += ":";
        writer->PrintString(prefix); writer->PrintStringln(sb.ToString());
        mChildFragmentManager->Dump(prefix + "  ", fd, writer, args);
    }
    return NOERROR;
}

Fragment::FindFragmentByWho(
    /* [in] */ const String& who,
    /* [out] */ IFragment** f)
{
    VALIDATE_NOT_NULL(f);
    if (who.Equals(mWho)) {
        *f = (IFragment*)this;
        REFCOUNT_ADD(*f);
        return NOERROR;
    }
    if (mChildFragmentManager != NULL) {
        AutoPtr<IFragment> fragment;
        mChildFragmentManager->FindFragmentByWho(who, (IFragment**)&fragment);
        *f = fragment;
        REFCOUNT_ADD(*f);
        return NOERROR;
    }
    *f = NULL;
    return NOERROR;
}

void Fragment::InstantiateChildFragmentManager()
{
    mChildFragmentManager = NULL;
    CFragmentManagerImpl::New((IFragmentManagerImpl**)&mChildFragmentManager);
    AutoPtr<FragmentContainerLocal> fc = new FragmentContainerLocal(this);
    mChildFragmentManager->AttachActivity(mActivity, IFragmentContainer::Probe(fc), (IFragment*)this->Probe(EIID_IFragment));
}

ECode Fragment::PerformCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->NoteStateNotSaved();
    }
    mCalled = FALSE;
    OnCreate(savedInstanceState);
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onCreate()");
    }
    if (savedInstanceState != NULL) {
        AutoPtr<IParcelable> p;
        savedInstanceState->GetParcelable(Activity::FRAGMENTS_TAG, (IParcelable**)&p);
        if (p != NULL) {
            if (mChildFragmentManager == NULL) {
                InstantiateChildFragmentManager();
            }
            mChildFragmentManager->RestoreAllState(p, NULL);
            mChildFragmentManager->DispatchCreate();
        }
    }
    return NOERROR;
}

ECode Fragment::PerformCreateView(
    /* [in] */ ILayoutInflater* inflater,
    /* [in] */ IViewGroup* container,
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ IView** v)
{
    VALIDATE_NOT_NULL(v);
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->NoteStateNotSaved();
    }
    AutoPtr<IView> view;
    OnCreateView(inflater, container, savedInstanceState, (IView**)&view);
    *v = view;
    REFCOUNT_ADD(*v);
    return NOERROR;
}

ECode Fragment::PerformActivityCreated(
    /* [in] */ IBundle* savedInstanceState)
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->NoteStateNotSaved();
    }
    mCalled = FALSE;
    OnActivityCreated(savedInstanceState);
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onActivityCreated()");
    }
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchActivityCreated();
    }
    return NOERROR;
}

ECode Fragment::PerformStart()
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->NoteStateNotSaved();
        Boolean hasexec;
        mChildFragmentManager->ExecPendingActions(&hasexec);
    }
    mCalled = FALSE;
    OnStart();
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onStart()");
    }
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchStart();
    }
    if (mLoaderManager != NULL) {
        mLoaderManager->DoReportStart();
    }
    return NOERROR;
}

ECode Fragment::PerformResume()
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->NoteStateNotSaved();
        Boolean hasexec;
        mChildFragmentManager->ExecPendingActions(&hasexec);
    }
    mCalled = FALSE;
    OnResume();
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onResume()");
    }
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchResume();
        Boolean hasexec;
        mChildFragmentManager->ExecPendingActions(&hasexec);
    }
    return NOERROR;
}

ECode Fragment::PerformConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    OnConfigurationChanged(newConfig);
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchConfigurationChanged(newConfig);
    }
    return NOERROR;
}

ECode Fragment::PerformLowMemory()
{
    OnLowMemory();
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchLowMemory();
    }
    return NOERROR;
}

ECode Fragment::PerformTrimMemory(
    /* [in] */ Int32 level)
{
    OnTrimMemory(level);
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchTrimMemory(level);
    }
    return NOERROR;
}

ECode Fragment::PerformCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [in] */ IMenuInflater* inflater,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Boolean show = FALSE;
    if (!mHidden) {
        if (mHasMenu && mMenuVisible) {
            show = TRUE;
            OnCreateOptionsMenu(menu, inflater);
        }
        if (mChildFragmentManager != NULL) {
            Boolean dispatch;
            mChildFragmentManager->DispatchCreateOptionsMenu(menu, inflater, &dispatch);
            show |= dispatch;
        }
    }
    *result = show;
    return NOERROR;
}

ECode Fragment::PerformPrepareOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean show = FALSE;
    if (!mHidden) {
        if (mHasMenu && mMenuVisible) {
            show = TRUE;
            OnPrepareOptionsMenu(menu);
        }
        if (mChildFragmentManager != NULL) {
            Boolean dispatch;
            mChildFragmentManager->DispatchPrepareOptionsMenu(menu, &dispatch);
            show |= dispatch;
        }
    }
    *result = show;
    return NOERROR;
}

ECode Fragment::PerformOptionsItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (!mHidden) {
        if (mHasMenu && mMenuVisible) {
            Boolean selected;
            OnOptionsItemSelected(item, &selected);
            if (selected) {
                *result = TRUE;
                return NOERROR;
            }
        }
        if (mChildFragmentManager != NULL) {
            Boolean itSelected;
            mChildFragmentManager->DispatchOptionsItemSelected(item, &itSelected);
            if (itSelected) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode Fragment::PerformContextItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (!mHidden) {
        Boolean selected;
        OnContextItemSelected(item, &selected);
        if (selected) {
            *result = TRUE;
            return NOERROR;
        }
        if (mChildFragmentManager != NULL) {
            Boolean itSelected;
            mChildFragmentManager->DispatchContextItemSelected(item, &itSelected);
            if (itSelected) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode Fragment::PerformOptionsMenuClosed(
    /* [in] */ IMenu* menu)
{
    if (!mHidden) {
        if (mHasMenu && mMenuVisible) {
            OnOptionsMenuClosed(menu);
        }
        if (mChildFragmentManager != NULL) {
            mChildFragmentManager->DispatchOptionsMenuClosed(menu);
        }
    }
    return NOERROR;
}

ECode Fragment::PerformSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    OnSaveInstanceState(outState);
    if (mChildFragmentManager != NULL) {
        AutoPtr<IParcelable> p;
        mChildFragmentManager->SaveAllState((IParcelable**)&p);
        if (p != NULL) {
            outState->PutParcelable(Activity::FRAGMENTS_TAG, p);
        }
    }
    return NOERROR;
}

ECode Fragment::PerformPause()
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchPause();
    }
    mCalled = FALSE;
    OnPause();
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onPause()");
    }
    return NOERROR;
}

ECode Fragment::PerformStop()
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchStop();
    }
    mCalled = FALSE;
    OnStop();
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onStop()");
    }

    if (mLoadersStarted) {
        mLoadersStarted = FALSE;
        AutoPtr<Activity> act = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
        if (!mCheckedForLoaderManager) {
            mCheckedForLoaderManager = TRUE;
            mLoaderManager = act->GetLoaderManager(mWho, mLoadersStarted, FALSE);
        }
        if (mLoaderManager != NULL) {
            if (mActivity == NULL || !act->mChangingConfigurations) {
                mLoaderManager->DoStop();
            } else {
                mLoaderManager->DoRetain();
            }
        }
    }
    return NOERROR;
}

ECode Fragment::PerformDestroyView()
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchDestroyView();
    }
    mCalled = FALSE;
    OnDestroyView();
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onDestroyView()");
    }
    if (mLoaderManager != NULL) {
        mLoaderManager->DoReportNextStart();
    }
    return NOERROR;
}

ECode Fragment::PerformDestroy()
{
    if (mChildFragmentManager != NULL) {
        mChildFragmentManager->DispatchDestroy();
    }
    mCalled = FALSE;
    OnDestroy();
    if (!mCalled) {
//         throw new SuperNotCalledException("Fragment " + this
//                 + " did not call through to super.onDestroy()");
    }
    return NOERROR;
}

AutoPtr<ITransition> Fragment::LoadTransition(
    /* [in] */ IContext* context,
    /* [in] */ ITypedArray* typedArray,
    /* [in] */ ITransition* currentValue,
    /* [in] */ ITransition* defaultValue,
    /* [in] */ Int32 id)
{
    if (currentValue != defaultValue) {
        return currentValue;
    }
    int transitionId = typedArray.getResourceId(id, 0);
    Transition transition = defaultValue;
    if (transitionId != 0 && transitionId != com.android.internal.R.transition.no_transition) {
        TransitionInflater inflater = TransitionInflater.from(context);
        transition = inflater.inflateTransition(transitionId);
        if (transition instanceof TransitionSet &&
                ((TransitionSet)transition).getTransitionCount() == 0) {
            transition = null;
        }
    }
    return transition;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
