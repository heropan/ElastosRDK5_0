#ifndef __FRAGMENT_H__
#define __FRAGMENT_H__

#include "ext/frameworkext.h"
#include <elastos/HashMap.h>

using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;
using Elastos::Utility::HashMap;
using Elastos::Core::ICharSequence;
using Elastos::Utility::IObjectInt32Map;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IContextMenu;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IContextMenuInfo;
using Elastos::Droid::View::IViewOnCreateContextMenuListener;
using Elastos::Droid::View::EIID_IViewOnCreateContextMenuListener;
using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IConfiguration;

namespace Elastos {
namespace Droid {
namespace App {

class FragmentState
    : public ElRefBase
    , public IParcelable
{
public:
    class ParcelableCreatorFragmentState
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI CreateFromParcel(
            /* [in] */ IParcel* in,
            /* [out] */ FragmentState** ret);

        CARAPI NewArray(
            /* [in] */ Int32 size,
            /* [out] */ ArrayOf<FragmentState*>** ret);
    };

public:
    CAR_INTERFACE_DECL()

    FragmentState(
        /* [in] */ IFragment* frag);

    FragmentState(
        /* [in] */ IParcel* in);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* in);

    CARAPI Instantiate(
        /* [in] */ IActivity* activity,
        /* [in] */ IFragment* parent,
        /* [out] */ IFragment** fragment);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    String mClassName;
    Int32 mIndex;
    Boolean mFromLayout;
    Int32 mFragmentId;
    Int32 mContainerId;
    String mTag;
    Boolean mRetainInstance;
    Boolean mDetached;
    AutoPtr<IBundle> mArguments;

    AutoPtr<IBundle> mSavedFragmentState;

    AutoPtr<IFragment> mInstance;

    AutoPtr<ParcelableCreatorFragmentState> CREATOR;
};

class Fragment
    : public IFragment
    , public ElRefBase
    , public IComponentCallbacks2
    , public IViewOnCreateContextMenuListener
    , public IWeakReferenceSource
{
public:
    Fragment()
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

    virtual ~Fragment()
    {}

    virtual CARAPI Initialize();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    virtual CARAPI_(UInt32) AddRef();

    virtual CARAPI_(UInt32) Release();

    virtual CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    virtual CARAPI GetState(
        /* [out] */ Int32* state);

    virtual CARAPI SetState(
        /* [in] */ Int32 state);

    virtual CARAPI GetAnimatingAway(
        /* [out] */ IAnimator** animator);

    virtual CARAPI SetAnimatingAway(
        /* [in] */ IAnimator* animator);

    virtual CARAPI GetStateAfterAnimating(
        /* [out] */ Int32* state);

    virtual CARAPI SetStateAfterAnimating(
        /* [in] */ Int32 state);

    virtual CARAPI GetSavedFragmentState(
        /* [out] */ IBundle** fState);

    virtual CARAPI SetSavedFragmentState(
        /* [in] */ IBundle* fState);

    virtual CARAPI GetSavedViewState(
        /* [out] */ IObjectInt32Map** viewState);

    virtual CARAPI SetSavedViewState(
        /* [in] */ IObjectInt32Map* viewState);

    virtual CARAPI GetIndex(
        /* [out] */ Int32* index);

    virtual CARAPI GetWho(
        /* [out] */ String* who);

    virtual CARAPI SetWho(
        /* [in] */ const String& who);

    virtual CARAPI GetTarget(
        /* [out] */ IFragment** target);

    virtual CARAPI SetTarget(
        /* [in] */ IFragment* target);

    virtual CARAPI GetTargetIndex(
        /* [out] */ Int32* index);

    virtual CARAPI SetTargetIndex(
        /* [in] */ Int32 index);

    virtual CARAPI SetTargetRequestCode(
        /* [in] */ Int32 code);

    virtual CARAPI GetAdded(
        /* [out] */ Boolean* added);

    virtual CARAPI SetAdded(
        /* [in] */ Boolean added);

    virtual CARAPI SetRemoving(
        /* [in] */ Boolean removing);

    virtual CARAPI SetResumed(
        /* [in] */ Boolean resumed);

    virtual CARAPI GetFromLayout(
        /* [out] */ Boolean* fLayout);

    virtual CARAPI SetFromLayout(
        /* [in] */ Boolean fLayout);

    virtual CARAPI SetInLayout(
        /* [in] */ Boolean inLayout);

    virtual CARAPI GetRestored(
        /* [out] */ Boolean* restored);

    virtual CARAPI SetRestored(
        /* [in] */ Boolean restored);

    virtual CARAPI GetBackStackNesting(
        /* [out] */ Int32* bsNesting);

    virtual CARAPI SetBackStackNesting(
        /* [in] */ Int32 bsNesting);

    virtual CARAPI SetFragmentManager(
        /* [in] */ IFragmentManagerImpl* fManager);

    virtual CARAPI SetActivity(
        /* [in] */ IActivity* activity);

    virtual CARAPI SetChildFragmentManager(
        /* [in] */ IFragmentManagerImpl* cfManager);

    virtual CARAPI GetChildFragmentManagerValue(
        /* [out] */ IFragmentManagerImpl** cfManager);

    virtual CARAPI SetParentFragment(
        /* [in] */ IFragment* pFragment);

    virtual CARAPI GetFragmentId(
        /* [out] */ Int32* fid);

    virtual CARAPI SetFragmentId(
        /* [in] */ Int32 fid);

    virtual CARAPI GetContainerId(
        /* [out] */ Int32* cid);

    virtual CARAPI SetContainerId(
        /* [in] */ Int32 cid);

    virtual CARAPI SetTag(
        /* [in] */ const String& tag);

    virtual CARAPI SetHidden(
        /* [in] */ Boolean hidden);

    virtual CARAPI SetDetached(
        /* [in] */ Boolean detached);

    virtual CARAPI GetRetaining(
        /* [out] */ Boolean* retaining);

    virtual CARAPI SetRetaining(
        /* [in] */ Boolean retaining);

    virtual CARAPI GetHasMenu(
        /* [out] */ Boolean* hasMenu);

    virtual CARAPI SetHasMenu(
        /* [in] */ Boolean hasMenu);

    virtual CARAPI GetMenuVisible(
        /* [out] */ Boolean* visible);

    virtual CARAPI SetMenuVisible(
        /* [in] */ Boolean visible);

    virtual CARAPI GetCalled(
        /* [out] */ Boolean* called);

    virtual CARAPI SetCalled(
        /* [in] */ Boolean called);

    virtual CARAPI GetNextAnim(
        /* [out] */ Int32* anim);

    virtual CARAPI SetNextAnim(
        /* [in] */ Int32 anim);

    virtual CARAPI GetContainer(
        /* [out] */ IViewGroup** container);

    virtual CARAPI SetContainer(
        /* [in] */ IViewGroup* container);

    virtual CARAPI SetView(
        /* [in] */ IView* view);

    virtual CARAPI GetDeferStart(
        /* [out] */ Boolean* start);

    virtual CARAPI SetDeferStart(
        /* [in] */ Boolean start);

    virtual CARAPI SetLoaderManager(
        /* [in] */ ILoaderManagerImpl* lManager);

    virtual CARAPI GetLoaderManagerValue(
        /* [out] */ ILoaderManagerImpl** lManager);

    virtual CARAPI GetLoadersStarted(
        /* [out] */ Boolean* started);

    virtual CARAPI SetLoadersStarted(
        /* [in] */ Boolean started);

    virtual CARAPI GetCheckedForLoaderManager(
        /* [out] */ Boolean* cfManager);

    virtual CARAPI SetCheckedForLoaderManager(
        /* [in] */ Boolean cfManager);

    static CARAPI Instantiate(
        /* [in] */ IContext* context,
        /* [in] */ const String& fname,
        /* [out] */ IFragment** fragment);

    static CARAPI InstantiateEx(
        /* [in] */ IContext* context,
        /* [in] */ const String& fname,
        /* [in] */ IBundle* args,
        /* [out] */ IFragment** fragment);

    virtual CARAPI RestoreViewState(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI SetIndex(
        /* [in] */ Int32 index,
        /* [in] */ IFragment* parent);

    virtual CARAPI IsInBackStack(
        /* [out] */ Boolean* inbs);

    virtual CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* equal);

    virtual CARAPI GetHashCode(
        /* [out] */ Int32* code);

    virtual CARAPI ToString(
        /* [out] */ String* string);

    virtual CARAPI GetId(
        /* [out] */ Int32* id);

    virtual CARAPI GetTag(
        /* [out] */ String* tag);

    virtual CARAPI SetArguments(
        /* [in] */ IBundle* args);

    virtual CARAPI GetArguments(
        /* [out] */ IBundle** args);

    virtual CARAPI SetInitialSavedState(
        /* [in] */ IFragmentSavedState* state);

    virtual CARAPI SetTargetFragment(
        /* [in] */ IFragment* fragment,
        /* [in] */ Int32 requestCode);

    virtual CARAPI GetTargetFragment(
        /* [out] */ IFragment** fragment);

    virtual CARAPI GetTargetRequestCode(
        /* [out] */ Int32* code);

    virtual CARAPI GetActivity(
        /* [out] */ IActivity** activity);

    virtual CARAPI GetResources(
        /* [out] */ IResources** resources);

    virtual CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    virtual CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* string);

    virtual CARAPI GetStringEx(
        /* [in] */ Int32 resId,
        /* [in] */ ArrayOf<IInterface*>* formatArgs,
        /* [out] */ String* string);

    virtual CARAPI GetFragmentManager(
        /* [out] */ IFragmentManager** manager);

    virtual CARAPI GetChildFragmentManager(
        /* [out] */ IFragmentManager** manager);

    virtual CARAPI GetParentFragment(
        /* [out] */ IFragment** fragment);

    virtual CARAPI IsAdded(
        /* [out] */ Boolean* added);

    virtual CARAPI IsDetached(
        /* [out] */ Boolean* detached);

    virtual CARAPI IsRemoving(
        /* [out] */ Boolean* removing);

    virtual CARAPI IsInLayout(
        /* [out] */ Boolean* inlayout);

    virtual CARAPI IsResumed(
        /* [out] */ Boolean* resumed);

    virtual CARAPI IsVisible(
        /* [out] */ Boolean* visible);

    virtual CARAPI IsHidden(
        /* [out] */ Boolean* hidden);

    virtual CARAPI OnHiddenChanged(
        /* [in] */ Boolean hidden);

    virtual CARAPI SetRetainInstance(
        /* [in] */ Boolean retain);

    virtual CARAPI GetRetainInstance(
        /* [out] */ Boolean* retainInstance);

    virtual CARAPI SetHasOptionsMenu(
        /* [in] */ Boolean hasMenu);

    virtual CARAPI SetMenuVisibility(
        /* [in] */ Boolean menuVisible);

    virtual CARAPI SetUserVisibleHint(
        /* [in] */ Boolean isVisibleToUser);

    virtual CARAPI GetUserVisibleHint(
        /* [out] */ Boolean* hint);

    virtual CARAPI GetLoaderManager(
        /* [out] */ ILoaderManager** manager);

    virtual CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    virtual CARAPI StartActivityEx(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options);

    virtual CARAPI StartActivityForResult(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode);

    virtual CARAPI StartActivityForResultEx(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 requestCode,
        /* [in] */ IBundle* options);

    virtual CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    virtual CARAPI GetLayoutInflater(
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ ILayoutInflater** inflater);

    virtual CARAPI OnInflate(
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnInflateEx(
        /* [in] */ IActivity* activity,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnAttach(
        /* [in] */ IActivity* activity);

    virtual CARAPI OnCreateAnimator(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter,
        /* [in] */ Int32 nextAnim,
        /* [out] */ IAnimator** animator);

    virtual CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnCreateView(
        /* [in] */ ILayoutInflater* inflater,
        /* [in] */ IViewGroup* container,
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IView** view);

    virtual CARAPI OnViewCreated(
        /* [in] */ IView* view,
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI GetView(
        /* [out] */ IView** view);

    virtual CARAPI OnActivityCreated(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnViewStateRestored(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnStart();

    virtual CARAPI OnResume();

    virtual CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    virtual CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI OnPause();

    virtual CARAPI OnStop();

    virtual CARAPI OnLowMemory();

    virtual CARAPI OnTrimMemory(
        /* [in] */ Int32 level);

    virtual CARAPI OnDestroyView();

    virtual CARAPI OnDestroy();

    virtual CARAPI InitState();

    virtual CARAPI OnDetach();

    virtual CARAPI OnCreateOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [in] */ IMenuInflater* inflater);

    virtual CARAPI OnPrepareOptionsMenu(
        /* [in] */ IMenu* menu);

    virtual CARAPI OnDestroyOptionsMenu();

    virtual CARAPI OnOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* selected);

    virtual CARAPI OnOptionsMenuClosed(
        /* [in] */ IMenu* menu);

    virtual CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    virtual CARAPI RegisterForContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI UnregisterForContextMenu(
        /* [in] */ IView* view);

    virtual CARAPI OnContextItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* selected);

    virtual CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

    virtual CARAPI FindFragmentByWho(
        /* [in] */ const String& who,
        /* [out] */ IFragment** fragment);

    virtual CARAPI_(void) InstantiateChildFragmentManager();

    virtual CARAPI PerformCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI PerformCreateView(
        /* [in] */ ILayoutInflater* inflater,
        /* [in] */ IViewGroup* container,
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IView** v);

    virtual CARAPI PerformActivityCreated(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI PerformStart();

    virtual CARAPI PerformResume();

    virtual CARAPI PerformConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI PerformLowMemory();

    virtual CARAPI PerformTrimMemory(
        /* [in] */ Int32 level);

    virtual CARAPI PerformCreateOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [in] */ IMenuInflater* inflater,
        /* [out] */ Boolean* result);

    virtual CARAPI PerformPrepareOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    virtual CARAPI PerformOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* selected);

    virtual CARAPI PerformContextItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* selected);

    virtual CARAPI PerformOptionsMenuClosed(
        /* [in] */ IMenu* menu);

    virtual CARAPI PerformSaveInstanceState(
        /* [in] */ IBundle* outState);

    virtual CARAPI PerformPause();

    virtual CARAPI PerformStop();

    virtual CARAPI PerformDestroyView();

    virtual CARAPI PerformDestroy();

public:
    Int32 mState;

    // Non-null if the fragment's view hierarchy is currently animating away,
    // meaning we need to wait a bit on completely destroying it.  This is the
    // animation that is running.
    AutoPtr<IAnimator> mAnimatingAway;

    // If mAnimatingAway != null, this is the state we should move to once the
    // animation is done.
    Int32 mStateAfterAnimating;

    // When instantiated from saved state, this is the saved state.
    AutoPtr<IBundle> mSavedFragmentState;
    AutoPtr<IObjectInt32Map> mSavedViewState;

    // Index into active fragment array.
    Int32 mIndex;

    // Internal unique name for this fragment;
    String mWho;

    // Construction arguments;
    AutoPtr<IBundle> mArguments;

    // Target fragment.
    AutoPtr<IFragment> mTarget;

    // For use when retaining a fragment: this is the index of the last mTarget.
    Int32 mTargetIndex;

    // Target request code.
    Int32 mTargetRequestCode;

    // True if the fragment is in the list of added fragments.
    Boolean mAdded;

    // If set this fragment is being removed from its activity.
    Boolean mRemoving;

    // True if the fragment is in the resumed state.
    Boolean mResumed;

    // Set to true if this fragment was instantiated from a layout file.
    Boolean mFromLayout;

    // Set to true when the view has actually been inflated in its layout.
    Boolean mInLayout;

    // True if this fragment has been restored from previously saved state.
    Boolean mRestored;

    // Number of active back stack entries this fragment is in.
    Int32 mBackStackNesting;

    // The fragment manager we are associated with.  Set as soon as the
    // fragment is used in a transaction; cleared after it has been removed
    // from all transactions.
    AutoPtr<IFragmentManagerImpl> mFragmentManager;

    // Activity this fragment is attached to.
    AutoPtr<IActivity> mActivity;

    // Private fragment manager for child fragments inside of this one.
    AutoPtr<IFragmentManagerImpl> mChildFragmentManager;

    // If this Fragment is contained in another Fragment, this is that container.
    AutoPtr<IFragment> mParentFragment;

    // The optional identifier for this fragment -- either the container ID if it
    // was dynamically added to the view hierarchy, or the ID supplied in
    // layout.
    Int32 mFragmentId;

    // When a fragment is being dynamically added to the view hierarchy, this
    // is the identifier of the parent container it is being added to.
    Int32 mContainerId;

    // The optional named tag for this fragment -- usually used to find
    // fragments that are not part of the layout.
    String mTag;

    // Set to true when the app has requested that this fragment be hidden
    // from the user.
    Boolean mHidden;

    // Set to true when the app has requested that this fragment be detached.
    Boolean mDetached;

    // If set this fragment would like its instance retained across
    // configuration changes.
    Boolean mRetainInstance;

    // If set this fragment is being retained across the current config change.
    Boolean mRetaining;

    // If set this fragment has menu items to contribute.
    Boolean mHasMenu;

    // Set to true to allow the fragment's menu to be shown.
    Boolean mMenuVisible;

    // Used to verify that subclasses call through to super class.
    Boolean mCalled;

    // If app has requested a specific animation, this is the one to use.
    Int32 mNextAnim;

    // The parent container of the fragment after dynamically added to UI.
    AutoPtr<IViewGroup> mContainer;

    // The View generated for this fragment.
    AutoPtr<IView> mView;

    // Whether this fragment should defer starting until after other fragments
    // have been started and their loaders are finished.
    Boolean mDeferStart;

    // Hint provided by the app that this fragment is currently visible to the user.
    Boolean mUserVisibleHint;

    AutoPtr<ILoaderManagerImpl> mLoaderManager;
    Boolean mLoadersStarted;
    Boolean mCheckedForLoaderManager;

private:
   static HashMap<String, AutoPtr<IClassInfo> > sClassMap;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__FRAGMENT_H__
