
#include "elastos/droid/app/Fragment.h"
#ifdef DROID_CORE
#include "elastos/droid/app/CFragmentManagerImpl.h"
#include "elastos/droid/app/CFragmentManagerImplHelper.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include "elastos/droid/app/Activity.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::IClassLoader;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::App::Activity;
using Elastos::Droid::App::IFragmentContainer;
using Elastos::Droid::App::EIID_IFragmentContainer;
using Elastos::Droid::App::CFragmentManagerImpl;
using Elastos::Droid::App::IFragmentManagerImplHelper;
using Elastos::Droid::App::CFragmentManagerImplHelper;
using Elastos::Droid::View::EIID_IViewOnCreateContextMenuListener;


namespace Elastos {
namespace Droid {
namespace App {

class FragmentContainerLocal
    : public ElRefBase
    , public IFragmentContainer
{
public:
    FragmentContainerLocal(
        /* [in] */ Fragment* host)
        : mHost(host)
    {}

    PInterface Probe(
    /* [in] */ REIID riid)
    {
        if (riid == EIID_IInterface) {
            return (PInterface)(IFragmentContainer*)this;
        }
        else if (riid == EIID_IFragmentContainer) {
            return (IFragmentContainer*)this;
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
        /* [in] */ IInterface* Object,
        /* [out] */ InterfaceID* iID)
    {
        VALIDATE_NOT_NULL(iID);
        if (iID == NULL) {
            return E_INVALID_ARGUMENT;
        }

        if (Object == (IInterface*)(IFragmentContainer*)this) {
            *iID = EIID_IFragmentContainer;
        }
        else {
            return E_INVALID_ARGUMENT;
        }
        return NOERROR;
    }

    ECode FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** view)
    {
        if (mHost->mView == NULL) {
//            throw new IllegalStateException("Fragment does not have a view");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        return mHost->mView->FindViewById(id, view);
    }

public:
    Fragment* mHost;
};

CAR_INTERFACE_IMPL(FragmentState::ParcelableCreatorFragmentState, IInterface)

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

CAR_INTERFACE_IMPL(FragmentState, IParcelable)

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
    VALIDATE_NOT_NULL(activity);
    VALIDATE_NOT_NULL(parent);
    VALIDATE_NOT_NULL(fragment);

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

    AutoPtr<Activity> act = reinterpret_cast<Activity*>(activity->Probe(EIID_Activity));
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

HashMap<String, AutoPtr<IClassInfo> > Fragment::sClassMap;

ECode Fragment::Initialize()
{
    return NOERROR;
}


PInterface Fragment::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IFragment*)this;
    }
    else if (riid == EIID_IFragment) {
        return (IFragment*)this;
    }
    else if (riid == EIID_IComponentCallbacks2) {
        return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IViewOnCreateContextMenuListener) {
        return (IViewOnCreateContextMenuListener*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    return NULL;
}

UInt32 Fragment::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Fragment::Release()
{
    return ElRefBase::Release();
}

ECode Fragment::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IFragment *)this) {
        *pIID = EIID_IFragment;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IComponentCallbacks2 *)this) {
        *pIID = EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IViewOnCreateContextMenuListener *)this) {
        *pIID = EIID_IViewOnCreateContextMenuListener;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWeakReferenceSource *)this) {
        *pIID = EIID_IWeakReferenceSource;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Fragment::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
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
//     return super.equals(o);
    return E_NOT_IMPLEMENTED;
}

ECode Fragment::GetHashCode(
    /* [out] */ Int32* code)
{
//     return super.hashCode();
    return E_NOT_IMPLEMENTED;
}

ECode Fragment::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    StringBuilder sb(128);
//     DebugUtils.buildShortClassTag(this, sb);
    if (mIndex >= 0) {
        sb.AppendChar(Char32(" #"));
        sb.Append(mIndex);
    }
    if (mFragmentId != 0) {
        sb.AppendChar(Char32(" id=0x"));
//        sb.append(Integer.toHexString(mFragmentId));
    }
    if (mTag != NULL) {
        sb.AppendChar(Char32(" "));
        sb.Append(mTag);
    }
    sb.AppendChar(Char32("}"));
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
    mCalled = TRUE;
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

} // namespace App
} // namespace Droid
} // namespace Elastos
