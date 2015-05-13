
#include "ext/frameworkext.h"

using Elastos::Core::IRunnable;

#ifndef __FRAGMENTMACRO_H__
#define __FRAGMENTMACRO_H__

#define IFRAGMENT_METHODS_DECL()                                                                 \
CARAPI GetState(                                                                                 \
    /* [out] */ Int32* state);                                                                   \
                                                                                                 \
CARAPI SetState(                                                                                 \
    /* [in] */ Int32 state);                                                                     \
                                                                                                 \
CARAPI GetAnimatingAway(                                                                         \
    /* [out] */ IAnimator** animator);                                                           \
                                                                                                 \
CARAPI SetAnimatingAway(                                                                         \
    /* [in] */ IAnimator* animator);                                                             \
                                                                                                 \
CARAPI GetStateAfterAnimating(                                                                   \
    /* [out] */ Int32* state);                                                                   \
                                                                                                 \
CARAPI SetStateAfterAnimating(                                                                   \
    /* [in] */ Int32 state);                                                                     \
                                                                                                 \
CARAPI GetSavedFragmentState(                                                                    \
    /* [out] */ IBundle** fState);                                                               \
                                                                                                 \
CARAPI SetSavedFragmentState(                                                                    \
    /* [in] */ IBundle* fState);                                                                 \
                                                                                                 \
CARAPI GetSavedViewState(                                                                        \
    /* [out] */ IObjectInt32Map** viewState);                                                    \
                                                                                                 \
CARAPI SetSavedViewState(                                                                        \
    /* [in] */ IObjectInt32Map* viewState);                                                      \
                                                                                                 \
CARAPI GetIndex(                                                                                 \
    /* [out] */ Int32* index);                                                                   \
                                                                                                 \
CARAPI GetWho(                                                                                   \
    /* [out] */ String* who);                                                                    \
                                                                                                 \
CARAPI SetWho(                                                                                   \
    /* [in] */ const String& who);                                                               \
                                                                                                 \
CARAPI GetTarget(                                                                                \
    /* [out] */ IFragment** target);                                                             \
                                                                                                 \
CARAPI SetTarget(                                                                                \
    /* [in] */ IFragment* target);                                                               \
                                                                                                 \
CARAPI GetTargetIndex(                                                                           \
    /* [out] */ Int32* index);                                                                   \
                                                                                                 \
CARAPI SetTargetIndex(                                                                           \
    /* [in] */ Int32 index);                                                                     \
                                                                                                 \
CARAPI SetTargetRequestCode(                                                                     \
    /* [in] */ Int32 code);                                                                      \
                                                                                                 \
CARAPI GetAdded(                                                                                 \
    /* [out] */ Boolean* added);                                                                 \
                                                                                                 \
CARAPI SetAdded(                                                                                 \
    /* [in] */ Boolean added);                                                                   \
                                                                                                 \
CARAPI SetRemoving(                                                                              \
    /* [in] */ Boolean removing);                                                                \
                                                                                                 \
CARAPI SetResumed(                                                                               \
    /* [in] */ Boolean resumed);                                                                 \
                                                                                                 \
CARAPI GetFromLayout(                                                                            \
    /* [out] */ Boolean* fLayout);                                                               \
                                                                                                 \
CARAPI SetFromLayout(                                                                            \
    /* [in] */ Boolean fLayout);                                                                 \
                                                                                                 \
CARAPI SetInLayout(                                                                              \
    /* [in] */ Boolean inLayout);                                                                \
                                                                                                 \
CARAPI GetRestored(                                                                              \
    /* [out] */ Boolean* restored);                                                              \
                                                                                                 \
CARAPI SetRestored(                                                                              \
    /* [in] */ Boolean restored);                                                                \
                                                                                                 \
CARAPI GetBackStackNesting(                                                                      \
    /* [out] */ Int32* bsNesting);                                                               \
                                                                                                 \
CARAPI SetBackStackNesting(                                                                      \
    /* [in] */ Int32 bsNesting);                                                                 \
                                                                                                 \
CARAPI SetFragmentManager(                                                                       \
    /* [in] */ IFragmentManagerImpl* fManager);                                                  \
                                                                                                 \
CARAPI SetActivity(                                                                              \
    /* [in] */ IActivity* activity);                                                             \
                                                                                                 \
CARAPI SetChildFragmentManager(                                                                  \
    /* [in] */ IFragmentManagerImpl* cfManager);                                                 \
                                                                                                 \
CARAPI GetChildFragmentManagerValue(                                                             \
    /* [out] */ IFragmentManagerImpl** cfManager);                                               \
                                                                                                 \
CARAPI SetParentFragment(                                                                        \
    /* [in] */ IFragment* pFragment);                                                            \
                                                                                                 \
CARAPI GetFragmentId(                                                                            \
    /* [out] */ Int32* fid);                                                                     \
                                                                                                 \
CARAPI SetFragmentId(                                                                            \
    /* [in] */ Int32 fid);                                                                       \
                                                                                                 \
CARAPI GetContainerId(                                                                           \
    /* [out] */ Int32* cid);                                                                     \
                                                                                                 \
CARAPI SetContainerId(                                                                           \
    /* [in] */ Int32 cid);                                                                       \
                                                                                                 \
CARAPI SetTag(                                                                                   \
    /* [in] */ const String& tag);                                                               \
                                                                                                 \
CARAPI SetHidden(                                                                                \
    /* [in] */ Boolean hid);                                                                     \
                                                                                                 \
CARAPI SetDetached(                                                                              \
    /* [in] */ Boolean detached);                                                                \
                                                                                                 \
CARAPI GetRetaining(                                                                             \
    /* [out] */ Boolean* retaining);                                                             \
                                                                                                 \
CARAPI SetRetaining(                                                                             \
    /* [in] */ Boolean retaining);                                                               \
                                                                                                 \
CARAPI GetHasMenu(                                                                               \
    /* [out] */ Boolean* hasMenu);                                                               \
                                                                                                 \
CARAPI SetHasMenu(                                                                               \
    /* [in] */ Boolean hasMenu);                                                                 \
                                                                                                 \
CARAPI GetMenuVisible(                                                                           \
    /* [out] */ Boolean* visible);                                                               \
                                                                                                 \
CARAPI SetMenuVisible(                                                                           \
    /* [in] */ Boolean visible);                                                                 \
                                                                                                 \
CARAPI GetCalled(                                                                                \
    /* [out] */ Boolean* called);                                                                \
                                                                                                 \
CARAPI SetCalled(                                                                                \
    /* [in] */ Boolean called);                                                                  \
                                                                                                 \
CARAPI GetNextAnim(                                                                              \
    /* [out] */ Int32* anim);                                                                    \
                                                                                                 \
CARAPI SetNextAnim(                                                                              \
    /* [in] */ Int32 anim);                                                                      \
                                                                                                 \
CARAPI GetContainer(                                                                             \
    /* [out] */ IViewGroup** container);                                                         \
                                                                                                 \
CARAPI SetContainer(                                                                             \
    /* [in] */ IViewGroup* container);                                                           \
                                                                                                 \
CARAPI SetView(                                                                                  \
    /* [in] */ IView* view);                                                                     \
                                                                                                 \
CARAPI GetDeferStart(                                                                            \
    /* [out] */ Boolean* start);                                                                 \
                                                                                                 \
CARAPI SetDeferStart(                                                                            \
    /* [in] */ Boolean start);                                                                   \
                                                                                                 \
CARAPI SetLoaderManager(                                                                         \
    /* [in] */ ILoaderManagerImpl* lManager);                                                    \
                                                                                                 \
CARAPI GetLoaderManagerValue(                                                                    \
    /* [out] */ ILoaderManagerImpl** lManager);                                                  \
                                                                                                 \
CARAPI GetLoadersStarted(                                                                        \
    /* [out] */ Boolean* started);                                                               \
                                                                                                 \
CARAPI SetLoadersStarted(                                                                        \
    /* [in] */ Boolean started);                                                                 \
                                                                                                 \
CARAPI GetCheckedForLoaderManager(                                                               \
    /* [out] */ Boolean* cfManager);                                                             \
                                                                                                 \
CARAPI SetCheckedForLoaderManager(                                                               \
    /* [in] */ Boolean cfManager);                                                               \
                                                                                                 \
CARAPI SetIndex(                                                                                 \
    /* [in] */ Int32 index,                                                                      \
    /* [in] */ IFragment* parent);                                                               \
                                                                                                 \
CARAPI ToString(                                                                                 \
    /* [out] */ String* string);                                                                 \
                                                                                                 \
CARAPI GetId(                                                                                    \
    /* [out] */ Int32* id);                                                                      \
                                                                                                 \
CARAPI GetTag(                                                                                   \
    /* [out] */ String* tag);                                                                    \
                                                                                                 \
CARAPI SetArguments(                                                                             \
    /* [in] */ IBundle* args);                                                                   \
                                                                                                 \
CARAPI GetArguments(                                                                             \
    /* [out] */ IBundle** args);                                                                 \
                                                                                                 \
CARAPI SetInitialSavedState(                                                                     \
    /* [in] */ IFragmentSavedState* state);                                                      \
                                                                                                 \
CARAPI SetTargetFragment(                                                                        \
    /* [in] */ IFragment* fragment,                                                              \
    /* [in] */ Int32 requestCode);                                                               \
                                                                                                 \
CARAPI GetTargetFragment(                                                                        \
    /* [out] */ IFragment** fragment);                                                           \
                                                                                                 \
CARAPI GetTargetRequestCode(                                                                     \
    /* [out] */ Int32* code);                                                                    \
                                                                                                 \
CARAPI GetActivity(                                                                              \
    /* [out] */ IActivity** activity);                                                           \
                                                                                                 \
CARAPI GetResources(                                                                             \
    /* [out] */ IResources** resources);                                                         \
                                                                                                 \
CARAPI GetText(                                                                                  \
    /* [in] */ Int32 resId,                                                                      \
    /* [out] */ ICharSequence** text);                                                           \
                                                                                                 \
CARAPI GetString(                                                                                \
    /* [in] */ Int32 resId,                                                                      \
    /* [out] */ String* string);                                                                 \
                                                                                                 \
CARAPI GetStringEx(                                                                              \
    /* [in] */ Int32 resId,                                                                      \
    /* [in] */ ArrayOf<IInterface*>* formatArgs,                                                 \
    /* [out] */ String* string);                                                                 \
                                                                                                 \
CARAPI GetFragmentManager(                                                                       \
    /* [out] */ IFragmentManager** manager);                                                     \
                                                                                                 \
CARAPI GetChildFragmentManager(                                                                  \
    /* [out] */ IFragmentManager** manager);                                                     \
                                                                                                 \
CARAPI GetParentFragment(                                                                        \
    /* [out] */ IFragment** fragment);                                                           \
                                                                                                 \
CARAPI IsAdded(                                                                                  \
    /* [out] */ Boolean* added);                                                                 \
                                                                                                 \
CARAPI IsDetached(                                                                               \
    /* [out] */ Boolean* detached);                                                              \
                                                                                                 \
CARAPI IsRemoving(                                                                               \
    /* [out] */ Boolean* removing);                                                              \
                                                                                                 \
CARAPI IsInLayout(                                                                               \
    /* [out] */ Boolean* inlayout);                                                              \
                                                                                                 \
CARAPI IsResumed(                                                                                \
    /* [out] */ Boolean* resumed);                                                               \
                                                                                                 \
CARAPI IsVisible(                                                                                \
    /* [out] */ Boolean* visible);                                                               \
                                                                                                 \
CARAPI IsHidden(                                                                                 \
    /* [out] */ Boolean* ishide);                                                                \
                                                                                                 \
CARAPI OnHiddenChanged(                                                                          \
    /* [in] */ Boolean changed);                                                                 \
                                                                                                 \
CARAPI SetRetainInstance(                                                                        \
    /* [in] */ Boolean retain);                                                                  \
                                                                                                 \
CARAPI GetRetainInstance(                                                                        \
    /* [out] */ Boolean* instance);                                                              \
                                                                                                 \
CARAPI SetHasOptionsMenu(                                                                        \
    /* [in] */ Boolean hasMenu);                                                                 \
                                                                                                 \
CARAPI SetMenuVisibility(                                                                        \
    /* [in] */ Boolean menuVisible);                                                             \
                                                                                                 \
CARAPI SetUserVisibleHint(                                                                       \
    /* [in] */ Boolean isVisibleToUser);                                                         \
                                                                                                 \
CARAPI GetUserVisibleHint(                                                                       \
    /* [out] */ Boolean* isVisibleToUser);                                                       \
                                                                                                 \
CARAPI GetLoaderManager(                                                                         \
    /* [out] */ ILoaderManager** manager);                                                       \
                                                                                                 \
CARAPI StartActivity(                                                                            \
    /* [in] */ IIntent* intent);                                                                 \
                                                                                                 \
CARAPI StartActivityEx(                                                                          \
    /* [in] */ IIntent* intent,                                                                  \
    /* [in] */ IBundle* options);                                                                \
                                                                                                 \
CARAPI StartActivityForResult(                                                                   \
    /* [in] */ IIntent* intent,                                                                  \
    /* [in] */ Int32 requestCode);                                                               \
                                                                                                 \
CARAPI StartActivityForResultEx(                                                                 \
    /* [in] */ IIntent* intent,                                                                  \
    /* [in] */ Int32 requestCode,                                                                \
    /* [in] */ IBundle* options);                                                                \
                                                                                                 \
CARAPI OnActivityResult(                                                                         \
    /* [in] */ Int32 requestCode,                                                                \
    /* [in] */ Int32 resultCode,                                                                 \
    /* [in] */ IIntent* data);                                                                   \
                                                                                                 \
CARAPI GetLayoutInflater(                                                                        \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ ILayoutInflater** inflater);                                                     \
                                                                                                 \
CARAPI OnInflate(                                                                                \
    /* [in] */ IAttributeSet* attrs,                                                             \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI OnInflateEx(                                                                              \
    /* [in] */ IActivity* activity,                                                              \
    /* [in] */ IAttributeSet* attrs,                                                             \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI OnAttach(                                                                                 \
    /* [in] */ IActivity* activity);                                                             \
                                                                                                 \
CARAPI OnCreateAnimator(                                                                         \
    /* [in] */ Int32 transit,                                                                    \
    /* [in] */ Boolean enter,                                                                    \
    /* [in] */ Int32 nextAnim,                                                                   \
    /* [out] */ IAnimator** animator);                                                           \
                                                                                                 \
CARAPI OnCreate(                                                                                 \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI OnCreateView(                                                                             \
    /* [in] */ ILayoutInflater* inflater,                                                        \
    /* [in] */ IViewGroup* container,                                                            \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ IView** view);                                                                   \
                                                                                                 \
CARAPI OnViewCreated(                                                                            \
    /* [in] */ IView* view,                                                                      \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI GetView(                                                                                  \
    /* [out] */ IView** view);                                                                   \
                                                                                                 \
CARAPI OnActivityCreated(                                                                        \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI OnViewStateRestored(                                                                      \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI OnStart();                                                                                \
                                                                                                 \
CARAPI OnResume();                                                                               \
                                                                                                 \
CARAPI OnSaveInstanceState(                                                                      \
    /* [in] */ IBundle* outState);                                                               \
                                                                                                 \
CARAPI OnPause();                                                                                \
                                                                                                 \
CARAPI OnStop();                                                                                 \
                                                                                                 \
CARAPI OnDestroyView();                                                                          \
                                                                                                 \
CARAPI OnDestroy();                                                                              \
                                                                                                 \
CARAPI OnDetach();                                                                               \
                                                                                                 \
CARAPI OnCreateOptionsMenu(                                                                      \
    /* [in] */ IMenu* menu,                                                                      \
    /* [in] */ IMenuInflater* inflater);                                                         \
                                                                                                 \
CARAPI OnPrepareOptionsMenu(                                                                     \
    /* [in] */ IMenu* menu);                                                                     \
                                                                                                 \
CARAPI OnDestroyOptionsMenu();                                                                   \
                                                                                                 \
CARAPI OnOptionsItemSelected(                                                                    \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected);                                                              \
                                                                                                 \
CARAPI OnOptionsMenuClosed(                                                                      \
    /* [in] */ IMenu* menu);                                                                     \
                                                                                                 \
CARAPI RegisterForContextMenu(                                                                   \
    /* [in] */ IView* view);                                                                     \
                                                                                                 \
CARAPI UnregisterForContextMenu(                                                                 \
    /* [in] */ IView* view);                                                                     \
                                                                                                 \
CARAPI OnContextItemSelected(                                                                    \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected);                                                              \
                                                                                                 \
CARAPI Dump(                                                                                     \
    /* [in] */ const String& prefix,                                                             \
    /* [in] */ IFileDescriptor* fd,                                                              \
    /* [in] */ IPrintWriter* writer,                                                             \
    /* [in] */ ArrayOf<String>* args);                                                           \
                                                                                                 \
CARAPI RestoreViewState(                                                                         \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI PerformCreate(                                                                            \
    /* [in] */ IBundle* savedInstanceState);                                                     \
                                                                                                 \
CARAPI PerformCreateView(                                                                        \
    /* [in] */ ILayoutInflater* inflater,                                                        \
    /* [in] */ IViewGroup* container,                                                            \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ IView** v);                                                                      \
                                                                                                 \
CARAPI PerformActivityCreated(                                                                   \
   /* [in] */ IBundle* savedInstanceState);                                                      \
                                                                                                 \
CARAPI PerformStart();                                                                           \
                                                                                                 \
CARAPI PerformResume();                                                                          \
                                                                                                 \
CARAPI PerformPause();                                                                           \
                                                                                                 \
CARAPI PerformStop();                                                                            \
                                                                                                 \
CARAPI PerformDestroyView();                                                                     \
                                                                                                 \
CARAPI PerformDestroy();                                                                         \
                                                                                                 \
CARAPI InitState();                                                                              \
                                                                                                 \
CARAPI IsInBackStack(                                                                            \
    /* [out] */ Boolean* inbs);                                                                  \
                                                                                                 \
CARAPI FindFragmentByWho(                                                                        \
    /* [in] */ const String& who,                                                                \
    /* [out] */ IFragment** fragment);                                                           \
                                                                                                 \
CARAPI PerformSaveInstanceState(                                                                 \
    /* [in] */ IBundle* outState);                                                               \
                                                                                                 \
CARAPI PerformConfigurationChanged(                                                              \
    /* [in] */ IConfiguration* newConfig);                                                       \
                                                                                                 \
CARAPI PerformLowMemory();                                                                       \
                                                                                                 \
CARAPI PerformTrimMemory(                                                                        \
    /* [in] */ Int32 level);                                                                     \
                                                                                                 \
CARAPI PerformCreateOptionsMenu(                                                                 \
    /* [in] */ IMenu* menu,                                                                      \
    /* [in] */ IMenuInflater* inflater,                                                          \
    /* [out] */ Boolean* result);                                                                \
                                                                                                 \
CARAPI PerformPrepareOptionsMenu(                                                                \
    /* [in] */ IMenu* menu,                                                                      \
    /* [out] */ Boolean* result);                                                                \
                                                                                                 \
CARAPI PerformOptionsItemSelected(                                                               \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected);                                                              \
                                                                                                 \
CARAPI PerformContextItemSelected(                                                               \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected);                                                              \
                                                                                                 \
CARAPI PerformOptionsMenuClosed(                                                                 \
        /* [in] */ IMenu* menu);


#define IFRAGMENT_METHODS_IMPL(className, superClass)                                            \
ECode className::GetState(                                                                       \
    /* [out] */ Int32* state)                                                                    \
{                                                                                                \
    return superClass::GetState(state);                                                          \
}                                                                                                \
                                                                                                 \
ECode className::SetState(                                                                       \
    /* [in] */ Int32 state)                                                                      \
{                                                                                                \
    return superClass::SetState(state);                                                          \
}                                                                                                \
                                                                                                 \
ECode className::GetAnimatingAway(                                                               \
    /* [out] */ IAnimator** animator)                                                            \
{                                                                                                \
    return superClass::GetAnimatingAway(animator);                                               \
}                                                                                                \
                                                                                                 \
ECode className::SetAnimatingAway(                                                               \
    /* [in] */ IAnimator* animator)                                                              \
{                                                                                                \
    return superClass::SetAnimatingAway(animator);                                               \
}                                                                                                \
                                                                                                 \
ECode className::GetStateAfterAnimating(                                                         \
    /* [out] */ Int32* state)                                                                    \
{                                                                                                \
    return superClass::GetStateAfterAnimating(state);                                            \
}                                                                                                \
                                                                                                 \
ECode className::SetStateAfterAnimating(                                                         \
    /* [in] */ Int32 state)                                                                      \
{                                                                                                \
    return superClass::SetStateAfterAnimating(state);                                            \
}                                                                                                \
                                                                                                 \
ECode className::GetSavedFragmentState(                                                          \
    /* [out] */ IBundle** fState)                                                                \
{                                                                                                \
    return superClass::GetSavedFragmentState(fState);                                            \
}                                                                                                \
                                                                                                 \
ECode className::SetSavedFragmentState(                                                          \
    /* [in] */ IBundle* fState)                                                                  \
{                                                                                                \
    return superClass::SetSavedFragmentState(fState);                                            \
}                                                                                                \
                                                                                                 \
ECode className::GetSavedViewState(                                                              \
    /* [out] */ IObjectInt32Map** viewState)                                                     \
{                                                                                                \
    return superClass::GetSavedViewState(viewState);                                             \
}                                                                                                \
                                                                                                 \
ECode className::SetSavedViewState(                                                              \
    /* [in] */ IObjectInt32Map* viewState)                                                       \
{                                                                                                \
    return superClass::SetSavedViewState(viewState);                                             \
}                                                                                                \
                                                                                                 \
ECode className::GetIndex(                                                                       \
    /* [out] */ Int32* index)                                                                    \
{                                                                                                \
    return superClass::GetIndex(index);                                                          \
}                                                                                                \
                                                                                                 \
ECode className::GetWho(                                                                         \
    /* [out] */ String* who)                                                                     \
{                                                                                                \
    return superClass::GetWho(who);                                                              \
}                                                                                                \
                                                                                                 \
ECode className::SetWho(                                                                         \
    /* [in] */ const String& who)                                                                \
{                                                                                                \
    return superClass::SetWho(who);                                                              \
}                                                                                                \
                                                                                                 \
ECode className::GetTarget(                                                                      \
    /* [out] */ IFragment** target)                                                              \
{                                                                                                \
    return superClass::GetTarget(target);                                                        \
}                                                                                                \
                                                                                                 \
ECode className::SetTarget(                                                                      \
    /* [in] */ IFragment* target)                                                                \
{                                                                                                \
    return superClass::SetTarget(target);                                                        \
}                                                                                                \
                                                                                                 \
ECode className::GetTargetIndex(                                                                 \
    /* [out] */ Int32* index)                                                                    \
{                                                                                                \
    return superClass::GetTargetIndex(index);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::SetTargetIndex(                                                                 \
    /* [in] */ Int32 index)                                                                      \
{                                                                                                \
    return superClass::SetTargetIndex(index);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::SetTargetRequestCode(                                                           \
    /* [in] */ Int32 code)                                                                       \
{                                                                                                \
    return superClass::SetTargetRequestCode(code);                                               \
}                                                                                                \
                                                                                                 \
ECode className::GetAdded(                                                                       \
    /* [out] */ Boolean* added)                                                                  \
{                                                                                                \
    return superClass::GetAdded(added);                                                          \
}                                                                                                \
                                                                                                 \
ECode className::SetAdded(                                                                       \
    /* [in] */ Boolean added)                                                                    \
{                                                                                                \
    return superClass::SetAdded(added);                                                          \
}                                                                                                \
                                                                                                 \
ECode className::SetRemoving(                                                                    \
    /* [in] */ Boolean removing)                                                                 \
{                                                                                                \
    return superClass::SetRemoving(removing);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::SetResumed(                                                                     \
    /* [in] */ Boolean resumed)                                                                  \
{                                                                                                \
    return superClass::SetResumed(resumed);                                                      \
}                                                                                                \
                                                                                                 \
ECode className::GetFromLayout(                                                                  \
    /* [out] */ Boolean* fLayout)                                                                \
{                                                                                                \
    return superClass::GetFromLayout(fLayout);                                                   \
}                                                                                                \
                                                                                                 \
ECode className::SetFromLayout(                                                                  \
    /* [in] */ Boolean fLayout)                                                                  \
{                                                                                                \
    return superClass::SetFromLayout(fLayout);                                                   \
}                                                                                                \
                                                                                                 \
ECode className::SetInLayout(                                                                    \
    /* [in] */ Boolean inLayout)                                                                 \
{                                                                                                \
    return superClass::SetInLayout(inLayout);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::GetRestored(                                                                    \
    /* [out] */ Boolean* restored)                                                               \
{                                                                                                \
    return superClass::GetRestored(restored);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::SetRestored(                                                                    \
    /* [in] */ Boolean restored)                                                                 \
{                                                                                                \
    return superClass::SetRestored(restored);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::GetBackStackNesting(                                                            \
    /* [out] */ Int32* bsNesting)                                                                \
{                                                                                                \
    return superClass::GetBackStackNesting(bsNesting);                                           \
}                                                                                                \
                                                                                                 \
ECode className::SetBackStackNesting(                                                            \
    /* [in] */ Int32 bsNesting)                                                                  \
{                                                                                                \
    return superClass::SetBackStackNesting(bsNesting);                                           \
}                                                                                                \
                                                                                                 \
ECode className::SetFragmentManager(                                                             \
    /* [in] */ IFragmentManagerImpl* fManager)                                                   \
{                                                                                                \
    return superClass::SetFragmentManager(fManager);                                             \
}                                                                                                \
                                                                                                 \
ECode className::SetActivity(                                                                    \
    /* [in] */ IActivity* activity)                                                              \
{                                                                                                \
    return superClass::SetActivity(activity);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::SetChildFragmentManager(                                                        \
    /* [in] */ IFragmentManagerImpl* cfManager)                                                  \
{                                                                                                \
    return superClass::SetChildFragmentManager(cfManager);                                       \
}                                                                                                \
                                                                                                 \
ECode className::GetChildFragmentManagerValue(                                                   \
    /* [out] */ IFragmentManagerImpl** cfManager)                                                \
{                                                                                                \
    return superClass::GetChildFragmentManagerValue(cfManager);                                  \
}                                                                                                \
                                                                                                 \
ECode className::SetParentFragment(                                                              \
    /* [in] */ IFragment* pFragment)                                                             \
{                                                                                                \
    return superClass::SetParentFragment(pFragment);                                             \
}                                                                                                \
                                                                                                 \
ECode className::GetFragmentId(                                                                  \
    /* [out] */ Int32* fid)                                                                      \
{                                                                                                \
    return superClass::GetFragmentId(fid);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::SetFragmentId(                                                                  \
    /* [in] */ Int32 fid)                                                                        \
{                                                                                                \
    return superClass::SetFragmentId(fid);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::GetContainerId(                                                                 \
    /* [out] */ Int32* cid)                                                                      \
{                                                                                                \
    return superClass::GetContainerId(cid);                                                      \
}                                                                                                \
                                                                                                 \
ECode className::SetContainerId(                                                                 \
    /* [in] */ Int32 cid)                                                                        \
{                                                                                                \
    return superClass::SetContainerId(cid);                                                      \
}                                                                                                \
                                                                                                 \
ECode className::SetTag(                                                                         \
    /* [in] */ const String& tag)                                                                \
{                                                                                                \
    return superClass::SetTag(tag);                                                              \
}                                                                                                \
                                                                                                 \
ECode className::SetHidden(                                                                      \
    /* [in] */ Boolean hid)                                                                      \
{                                                                                                \
    return superClass::SetHidden(hid);                                                           \
}                                                                                                \
                                                                                                 \
ECode className::SetDetached(                                                                    \
    /* [in] */ Boolean detached)                                                                 \
{                                                                                                \
    return superClass::SetDetached(detached);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::GetRetaining(                                                                   \
    /* [out] */ Boolean* retaining)                                                              \
{                                                                                                \
    return superClass::GetRetaining(retaining);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::SetRetaining(                                                                   \
    /* [in] */ Boolean retaining)                                                                \
{                                                                                                \
    return superClass::SetRetaining(retaining);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::GetHasMenu(                                                                     \
    /* [out] */ Boolean* hasMenu)                                                                \
{                                                                                                \
    return superClass::GetHasMenu(hasMenu);                                                      \
}                                                                                                \
                                                                                                 \
ECode className::SetHasMenu(                                                                     \
    /* [in] */ Boolean hasMenu)                                                                  \
{                                                                                                \
    return superClass::SetHasMenu(hasMenu);                                                      \
}                                                                                                \
                                                                                                 \
ECode className::GetMenuVisible(                                                                 \
    /* [out] */ Boolean* visible)                                                                \
{                                                                                                \
    return superClass::GetMenuVisible(visible);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::SetMenuVisible(                                                                 \
    /* [in] */ Boolean visible)                                                                  \
{                                                                                                \
    return superClass::SetMenuVisible(visible);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::GetCalled(                                                                      \
    /* [out] */ Boolean* called)                                                                 \
{                                                                                                \
    return superClass::GetCalled(called);                                                        \
}                                                                                                \
                                                                                                 \
ECode className::SetCalled(                                                                      \
    /* [in] */ Boolean called)                                                                   \
{                                                                                                \
    return superClass::SetCalled(called);                                                        \
}                                                                                                \
                                                                                                 \
ECode className::GetNextAnim(                                                                    \
    /* [out] */ Int32* anim)                                                                     \
{                                                                                                \
    return superClass::GetNextAnim(anim);                                                        \
}                                                                                                \
                                                                                                 \
ECode className::SetNextAnim(                                                                    \
    /* [in] */ Int32 anim)                                                                       \
{                                                                                                \
    return superClass::SetNextAnim(anim);                                                        \
}                                                                                                \
                                                                                                 \
ECode className::GetContainer(                                                                   \
    /* [out] */ IViewGroup** container)                                                          \
{                                                                                                \
    return superClass::GetContainer(container);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::SetContainer(                                                                   \
    /* [in] */ IViewGroup* container)                                                            \
{                                                                                                \
    return superClass::SetContainer(container);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::SetView(                                                                        \
    /* [in] */ IView* view)                                                                      \
{                                                                                                \
    return superClass::SetView(view);                                                            \
}                                                                                                \
                                                                                                 \
ECode className::GetDeferStart(                                                                  \
    /* [out] */ Boolean* start)                                                                  \
{                                                                                                \
    return superClass::GetDeferStart(start);                                                     \
}                                                                                                \
                                                                                                 \
ECode className::SetDeferStart(                                                                  \
    /* [in] */ Boolean start)                                                                    \
{                                                                                                \
    return superClass::SetDeferStart(start);                                                     \
}                                                                                                \
                                                                                                 \
ECode className::SetLoaderManager(                                                               \
    /* [in] */ ILoaderManagerImpl* lManager)                                                     \
{                                                                                                \
    return superClass::SetLoaderManager(lManager);                                               \
}                                                                                                \
                                                                                                 \
ECode className::GetLoaderManagerValue(                                                          \
    /* [out] */ ILoaderManagerImpl** lManager)                                                   \
{                                                                                                \
    return superClass::GetLoaderManagerValue(lManager);                                          \
}                                                                                                \
                                                                                                 \
ECode className::GetLoadersStarted(                                                              \
    /* [out] */ Boolean* started)                                                                \
{                                                                                                \
    return superClass::GetLoadersStarted(started);                                               \
}                                                                                                \
                                                                                                 \
ECode className::SetLoadersStarted(                                                              \
    /* [in] */ Boolean started)                                                                  \
{                                                                                                \
    return superClass::SetLoadersStarted(started);                                               \
}                                                                                                \
                                                                                                 \
ECode className::GetCheckedForLoaderManager(                                                     \
    /* [out] */ Boolean* cfManager)                                                              \
{                                                                                                \
    return superClass::GetCheckedForLoaderManager(cfManager);                                    \
}                                                                                                \
                                                                                                 \
ECode className::SetCheckedForLoaderManager(                                                     \
    /* [in] */ Boolean cfManager)                                                                \
{                                                                                                \
    return superClass::SetCheckedForLoaderManager(cfManager);                                    \
}                                                                                                \
                                                                                                 \
ECode className::SetIndex(                                                                       \
    /* [in] */ Int32 index,                                                                      \
    /* [in] */ IFragment* parent)                                                                \
{                                                                                                \
    return superClass::SetIndex(index, parent);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::ToString(                                                                       \
    /* [out] */ String* string)                                                                  \
{                                                                                                \
    return superClass::ToString(string);                                                         \
}                                                                                                \
                                                                                                 \
ECode className::GetId(                                                                          \
    /* [out] */ Int32* id)                                                                       \
{                                                                                                \
    return superClass::GetId(id);                                                                \
}                                                                                                \
                                                                                                 \
ECode className::GetTag(                                                                         \
    /* [out] */ String* tag)                                                                     \
{                                                                                                \
    return superClass::GetTag(tag);                                                              \
}                                                                                                \
                                                                                                 \
ECode className::SetArguments(                                                                   \
    /* [in] */ IBundle* args)                                                                    \
{                                                                                                \
    return superClass::SetArguments(args);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::GetArguments(                                                                   \
    /* [out] */ IBundle** args)                                                                  \
{                                                                                                \
    return superClass::GetArguments(args);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::SetInitialSavedState(                                                           \
    /* [in] */ IFragmentSavedState* state)                                                       \
{                                                                                                \
    return superClass::SetInitialSavedState(state);                                              \
}                                                                                                \
                                                                                                 \
ECode className::SetTargetFragment(                                                              \
    /* [in] */ IFragment* fragment,                                                              \
    /* [in] */ Int32 requestCode)                                                                \
{                                                                                                \
    return superClass::SetTargetFragment(fragment, requestCode);                                 \
}                                                                                                \
                                                                                                 \
ECode className::GetTargetFragment(                                                              \
    /* [out] */ IFragment** fragment)                                                            \
{                                                                                                \
    return superClass::GetTargetFragment(fragment);                                              \
}                                                                                                \
                                                                                                 \
ECode className::GetTargetRequestCode(                                                           \
    /* [out] */ Int32* code)                                                                     \
{                                                                                                \
    return superClass::GetTargetRequestCode(code);                                               \
}                                                                                                \
                                                                                                 \
ECode className::GetActivity(                                                                    \
    /* [out] */ IActivity** activity)                                                            \
{                                                                                                \
    return superClass::GetActivity(activity);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::GetResources(                                                                   \
    /* [out] */ IResources** resources)                                                          \
{                                                                                                \
    return superClass::GetResources(resources);                                                  \
}                                                                                                \
                                                                                                 \
ECode className::GetText(                                                                        \
    /* [in] */ Int32 resId,                                                                      \
    /* [out] */ ICharSequence** text)                                                            \
{                                                                                                \
    return superClass::GetText(resId, text);                                                     \
}                                                                                                \
                                                                                                 \
ECode className::GetString(                                                                      \
    /* [in] */ Int32 resId,                                                                      \
    /* [out] */ String* string)                                                                  \
{                                                                                                \
    return superClass::GetString(resId, string);                                                 \
}                                                                                                \
                                                                                                 \
ECode className::GetStringEx(                                                                    \
    /* [in] */ Int32 resId,                                                                      \
    /* [in] */ ArrayOf<IInterface*>* formatArgs,                                                 \
    /* [out] */ String* string)                                                                  \
{                                                                                                \
    return superClass::GetStringEx(resId, formatArgs, string);                                   \
}                                                                                                \
                                                                                                 \
ECode className::GetFragmentManager(                                                             \
    /* [out] */ IFragmentManager** manager)                                                      \
{                                                                                                \
    return superClass::GetFragmentManager(manager);                                              \
}                                                                                                \
                                                                                                 \
ECode className::GetChildFragmentManager(                                                        \
    /* [out] */ IFragmentManager** manager)                                                      \
{                                                                                                \
    return superClass::GetChildFragmentManager(manager);                                         \
}                                                                                                \
                                                                                                 \
ECode className::GetParentFragment(                                                              \
    /* [out] */ IFragment** fragment)                                                            \
{                                                                                                \
    return superClass::GetParentFragment(fragment);                                              \
}                                                                                                \
                                                                                                 \
ECode className::IsAdded(                                                                        \
    /* [out] */ Boolean* added)                                                                  \
{                                                                                                \
    return superClass::IsAdded(added);                                                           \
}                                                                                                \
                                                                                                 \
ECode className::IsDetached(                                                                     \
    /* [out] */ Boolean* detached)                                                               \
{                                                                                                \
    return superClass::IsDetached(detached);                                                     \
}                                                                                                \
                                                                                                 \
ECode className::IsRemoving(                                                                     \
    /* [out] */ Boolean* removing)                                                               \
{                                                                                                \
    return superClass::IsRemoving(removing);                                                     \
}                                                                                                \
                                                                                                 \
ECode className::IsInLayout(                                                                     \
    /* [out] */ Boolean* inlayout)                                                               \
{                                                                                                \
    return superClass::IsInLayout(inlayout);                                                     \
}                                                                                                \
                                                                                                 \
ECode className::IsResumed(                                                                      \
    /* [out] */ Boolean* resumed)                                                                \
{                                                                                                \
    return superClass::IsResumed(resumed);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::IsVisible(                                                                      \
    /* [out] */ Boolean* visible)                                                                \
{                                                                                                \
    return superClass::IsVisible(visible);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::IsHidden(                                                                       \
    /* [out] */ Boolean* ishide)                                                                 \
{                                                                                                \
    return superClass::IsHidden(ishide);                                                         \
}                                                                                                \
                                                                                                 \
ECode className::OnHiddenChanged(                                                                \
    /* [in] */ Boolean changed)                                                                  \
{                                                                                                \
    return superClass::OnHiddenChanged(changed);                                                 \
}                                                                                                \
                                                                                                 \
ECode className::SetRetainInstance(                                                              \
    /* [in] */ Boolean retain)                                                                   \
{                                                                                                \
    return superClass::SetRetainInstance(retain);                                                \
}                                                                                                \
                                                                                                 \
ECode className::GetRetainInstance(                                                              \
    /* [out] */ Boolean* instance)                                                               \
{                                                                                                \
    return superClass::GetRetainInstance(instance);                                              \
}                                                                                                \
                                                                                                 \
ECode className::SetHasOptionsMenu(                                                              \
    /* [in] */ Boolean hasMenu)                                                                  \
{                                                                                                \
    return superClass::SetHasOptionsMenu(hasMenu);                                               \
}                                                                                                \
                                                                                                 \
ECode className::SetMenuVisibility(                                                              \
    /* [in] */ Boolean menuVisible)                                                              \
{                                                                                                \
    return superClass::SetMenuVisibility(menuVisible);                                           \
}                                                                                                \
                                                                                                 \
ECode className::SetUserVisibleHint(                                                             \
    /* [in] */ Boolean isVisibleToUser)                                                          \
{                                                                                                \
    return superClass::SetUserVisibleHint(isVisibleToUser);                                      \
}                                                                                                \
                                                                                                 \
ECode className::GetUserVisibleHint(                                                             \
    /* [out] */ Boolean* isVisibleToUser)                                                        \
{                                                                                                \
    return superClass::GetUserVisibleHint(isVisibleToUser);                                      \
}                                                                                                \
                                                                                                 \
ECode className::GetLoaderManager(                                                               \
    /* [out] */ ILoaderManager** manager)                                                        \
{                                                                                                \
    return superClass::GetLoaderManager(manager);                                                \
}                                                                                                \
                                                                                                 \
ECode className::StartActivity(                                                                  \
    /* [in] */ IIntent* intent)                                                                  \
{                                                                                                \
    return superClass::StartActivity(intent);                                                    \
}                                                                                                \
                                                                                                 \
ECode className::StartActivityEx(                                                                \
    /* [in] */ IIntent* intent,                                                                  \
    /* [in] */ IBundle* options)                                                                 \
{                                                                                                \
    return superClass::StartActivityEx(intent, options);                                         \
}                                                                                                \
                                                                                                 \
ECode className::StartActivityForResult(                                                         \
    /* [in] */ IIntent* intent,                                                                  \
    /* [in] */ Int32 requestCode)                                                                \
{                                                                                                \
    return superClass::StartActivityForResult(intent, requestCode);                              \
}                                                                                                \
                                                                                                 \
ECode className::StartActivityForResultEx(                                                       \
    /* [in] */ IIntent* intent,                                                                  \
    /* [in] */ Int32 requestCode,                                                                \
    /* [in] */ IBundle* options)                                                                 \
{                                                                                                \
    return superClass::StartActivityForResultEx(intent, requestCode, options);                   \
}                                                                                                \
                                                                                                 \
ECode className::OnActivityResult(                                                               \
    /* [in] */ Int32 requestCode,                                                                \
    /* [in] */ Int32 resultCode,                                                                 \
    /* [in] */ IIntent* data)                                                                    \
{                                                                                                \
    return superClass::OnActivityResult(requestCode, resultCode, data);                          \
}                                                                                                \
                                                                                                 \
ECode className::GetLayoutInflater(                                                              \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ ILayoutInflater** inflater)                                                      \
{                                                                                                \
    return superClass::GetLayoutInflater(savedInstanceState, inflater);                          \
}                                                                                                \
                                                                                                 \
ECode className::OnInflate(                                                                      \
    /* [in] */ IAttributeSet* attrs,                                                             \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::OnInflate(attrs, savedInstanceState);                                     \
}                                                                                                \
                                                                                                 \
ECode className::OnInflateEx(                                                                    \
    /* [in] */ IActivity* activity,                                                              \
    /* [in] */ IAttributeSet* attrs,                                                             \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::OnInflateEx(activity, attrs, savedInstanceState);                         \
}                                                                                                \
                                                                                                 \
ECode className::OnAttach(                                                                       \
    /* [in] */ IActivity* activity)                                                              \
{                                                                                                \
    return superClass::OnAttach(activity);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::OnCreateAnimator(                                                               \
    /* [in] */ Int32 transit,                                                                    \
    /* [in] */ Boolean enter,                                                                    \
    /* [in] */ Int32 nextAnim,                                                                   \
    /* [out] */ IAnimator** animator)                                                            \
{                                                                                                \
    return superClass::OnCreateAnimator(transit, enter, nextAnim, animator);                     \
}                                                                                                \
                                                                                                 \
ECode className::OnCreate(                                                                       \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::OnCreate(savedInstanceState);                                             \
}                                                                                                \
                                                                                                 \
ECode className::OnCreateView(                                                                   \
    /* [in] */ ILayoutInflater* inflater,                                                        \
    /* [in] */ IViewGroup* container,                                                            \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ IView** view)                                                                    \
{                                                                                                \
    return superClass::OnCreateView(inflater, container, savedInstanceState, view);              \
}                                                                                                \
                                                                                                 \
ECode className::OnViewCreated(                                                                  \
    /* [in] */ IView* view,                                                                      \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::OnViewCreated(view, savedInstanceState);                                  \
}                                                                                                \
                                                                                                 \
ECode className::GetView(                                                                        \
    /* [out] */ IView** view)                                                                    \
{                                                                                                \
    return superClass::GetView(view);                                                            \
}                                                                                                \
                                                                                                 \
ECode className::OnActivityCreated(                                                              \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::OnActivityCreated(savedInstanceState);                                    \
}                                                                                                \
                                                                                                 \
ECode className::OnViewStateRestored(                                                            \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::OnViewStateRestored(savedInstanceState);                                  \
}                                                                                                \
                                                                                                 \
ECode className::OnStart()                                                                       \
{                                                                                                \
    return superClass::OnStart();                                                                \
}                                                                                                \
                                                                                                 \
ECode className::OnResume()                                                                      \
{                                                                                                \
    return superClass::OnResume();                                                               \
}                                                                                                \
                                                                                                 \
ECode className::OnSaveInstanceState(                                                            \
    /* [in] */ IBundle* outState)                                                                \
{                                                                                                \
    return superClass::OnSaveInstanceState(outState);                                            \
}                                                                                                \
                                                                                                 \
ECode className::OnPause()                                                                       \
{                                                                                                \
    return superClass::OnPause();                                                                \
}                                                                                                \
                                                                                                 \
ECode className::OnStop()                                                                        \
{                                                                                                \
    return superClass::OnStop();                                                                 \
}                                                                                                \
                                                                                                 \
ECode className::OnDestroyView()                                                                 \
{                                                                                                \
    return superClass::OnDestroyView();                                                          \
}                                                                                                \
                                                                                                 \
ECode className::OnDestroy()                                                                     \
{                                                                                                \
    return superClass::OnDestroy();                                                              \
}                                                                                                \
                                                                                                 \
ECode className::OnDetach()                                                                      \
{                                                                                                \
    return superClass::OnDetach();                                                               \
}                                                                                                \
                                                                                                 \
ECode className::OnCreateOptionsMenu(                                                            \
    /* [in] */ IMenu* menu,                                                                      \
    /* [in] */ IMenuInflater* inflater)                                                          \
{                                                                                                \
    return superClass::OnCreateOptionsMenu(menu, inflater);                                      \
}                                                                                                \
                                                                                                 \
ECode className::OnPrepareOptionsMenu(                                                           \
    /* [in] */ IMenu* menu)                                                                      \
{                                                                                                \
    return superClass::OnPrepareOptionsMenu(menu);                                               \
}                                                                                                \
                                                                                                 \
ECode className::OnDestroyOptionsMenu()                                                          \
{                                                                                                \
    return superClass::OnDestroyOptionsMenu();                                                   \
}                                                                                                \
                                                                                                 \
ECode className::OnOptionsItemSelected(                                                          \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected)                                                               \
{                                                                                                \
    return superClass::OnOptionsItemSelected(item, selected);                                    \
}                                                                                                \
                                                                                                 \
ECode className::OnOptionsMenuClosed(                                                            \
    /* [in] */ IMenu* menu)                                                                      \
{                                                                                                \
    return superClass::OnOptionsMenuClosed(menu);                                                \
}                                                                                                \
                                                                                                 \
ECode className::RegisterForContextMenu(                                                         \
    /* [in] */ IView* view)                                                                      \
{                                                                                                \
    return superClass::RegisterForContextMenu(view);                                             \
}                                                                                                \
                                                                                                 \
ECode className::UnregisterForContextMenu(                                                       \
    /* [in] */ IView* view)                                                                      \
{                                                                                                \
    return superClass::UnregisterForContextMenu(view);                                           \
}                                                                                                \
                                                                                                 \
ECode className::OnContextItemSelected(                                                          \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected)                                                               \
{                                                                                                \
    return superClass::OnContextItemSelected(item, selected);                                    \
}                                                                                                \
                                                                                                 \
ECode className::Dump(                                                                           \
    /* [in] */ const String& prefix,                                                             \
    /* [in] */ IFileDescriptor* fd,                                                              \
    /* [in] */ IPrintWriter* writer,                                                             \
    /* [in] */ ArrayOf<String>* args)                                                            \
{                                                                                                \
    return superClass::Dump(prefix, fd, writer, args);                                           \
}                                                                                                \
                                                                                                 \
ECode className::RestoreViewState(                                                               \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::RestoreViewState(savedInstanceState);                                     \
}                                                                                                \
                                                                                                 \
ECode className::PerformCreate(                                                                  \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::PerformCreate(savedInstanceState);                                        \
}                                                                                                \
                                                                                                 \
ECode className::PerformCreateView(                                                              \
    /* [in] */ ILayoutInflater* inflater,                                                        \
    /* [in] */ IViewGroup* container,                                                            \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ IView** v)                                                                       \
{                                                                                                \
    return superClass::PerformCreateView(inflater, container, savedInstanceState, v);            \
}                                                                                                \
                                                                                                 \
ECode className::PerformActivityCreated(                                                         \
    /* [in] */ IBundle* savedInstanceState)                                                      \
{                                                                                                \
    return superClass::PerformActivityCreated(savedInstanceState);                               \
}                                                                                                \
                                                                                                 \
ECode className::PerformStart()                                                                  \
{                                                                                                \
    return superClass::PerformStart();                                                           \
}                                                                                                \
                                                                                                 \
ECode className::PerformResume()                                                                 \
{                                                                                                \
    return superClass::PerformResume();                                                          \
}                                                                                                \
                                                                                                 \
ECode className::PerformPause()                                                                  \
{                                                                                                \
    return superClass::PerformPause();                                                           \
}                                                                                                \
                                                                                                 \
ECode className::PerformStop()                                                                   \
{                                                                                                \
    return superClass::PerformStop();                                                            \
}                                                                                                \
                                                                                                 \
ECode className::PerformDestroyView()                                                            \
{                                                                                                \
    return superClass::PerformDestroyView();                                                     \
}                                                                                                \
                                                                                                 \
ECode className::PerformDestroy()                                                                \
{                                                                                                \
    return superClass::PerformDestroy();                                                         \
}                                                                                                \
                                                                                                 \
ECode className::InitState()                                                                     \
{                                                                                                \
    return superClass::InitState();                                                              \
}                                                                                                \
                                                                                                 \
ECode className::IsInBackStack(                                                                  \
    /* [out] */ Boolean* inbs)                                                                   \
{                                                                                                \
    return superClass::IsInBackStack(inbs);                                                      \
}                                                                                                \
                                                                                                 \
ECode className::FindFragmentByWho(                                                              \
    /* [in] */ const String& who,                                                                \
    /* [out] */ IFragment** fragment)                                                            \
{                                                                                                \
    return superClass::FindFragmentByWho(who, fragment);                                         \
}                                                                                                \
                                                                                                 \
ECode className::PerformSaveInstanceState(                                                       \
    /* [in] */ IBundle* outState)                                                                \
{                                                                                                \
    return superClass::PerformSaveInstanceState(outState);                                       \
}                                                                                                \
                                                                                                 \
ECode className::PerformConfigurationChanged(                                                    \
    /* [in] */ IConfiguration* newConfig)                                                        \
{                                                                                                \
    return superClass::PerformConfigurationChanged(newConfig);                                   \
}                                                                                                \
                                                                                                 \
ECode className::PerformLowMemory()                                                              \
{                                                                                                \
    return superClass::PerformLowMemory();                                                       \
}                                                                                                \
                                                                                                 \
ECode className::PerformTrimMemory(                                                              \
    /* [in] */ Int32 level)                                                                      \
{                                                                                                \
    return superClass::PerformTrimMemory(level);                                                 \
}                                                                                                \
                                                                                                 \
ECode className::PerformCreateOptionsMenu(                                                       \
    /* [in] */ IMenu* menu,                                                                      \
    /* [in] */ IMenuInflater* inflater,                                                          \
    /* [out] */ Boolean* result)                                                                 \
{                                                                                                \
    return superClass::PerformCreateOptionsMenu(menu, inflater, result);                         \
}                                                                                                \
                                                                                                 \
ECode className::PerformPrepareOptionsMenu(                                                      \
    /* [in] */ IMenu* menu,                                                                      \
    /* [out] */ Boolean* result)                                                                 \
{                                                                                                \
    return superClass::PerformPrepareOptionsMenu(menu, result);                                  \
}                                                                                                \
                                                                                                 \
ECode className::PerformOptionsItemSelected(                                                     \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected)                                                               \
{                                                                                                \
    return superClass::PerformOptionsItemSelected(item, selected);                               \
}                                                                                                \
                                                                                                 \
ECode className::PerformContextItemSelected(                                                     \
    /* [in] */ IMenuItem* item,                                                                  \
    /* [out] */ Boolean* selected)                                                               \
{                                                                                                \
    return superClass::PerformContextItemSelected(item, selected);                               \
}                                                                                                \
                                                                                                 \
ECode className::PerformOptionsMenuClosed(                                                       \
        /* [in] */ IMenu* menu)                                                                  \
{                                                                                                \
    return superClass::PerformOptionsMenuClosed(menu);                                           \
}




#define IDIALOGFRAGMENT_METHODS_DECL()                                                           \
CARAPI SetStyle(                                                                                 \
    /* [in] */ Int32 style,                                                                      \
    /* [in] */ Int32 theme);                                                                     \
                                                                                                 \
CARAPI Show(                                                                                     \
    /* [in] */ IFragmentManager* manager,                                                        \
    /* [in] */ const String& tag);                                                               \
                                                                                                 \
CARAPI ShowEx(                                                                                   \
    /* [in] */ IFragmentTransaction* transaction,                                                \
    /* [in] */ const String& tag,                                                                \
    /* [out] */ Int32* id);                                                                      \
                                                                                                 \
CARAPI Dismiss();                                                                                \
                                                                                                 \
CARAPI DismissAllowingStateLoss();                                                               \
                                                                                                 \
CARAPI GetDialog(                                                                                \
    /* [out] */ IDialog** dialog);                                                               \
                                                                                                 \
CARAPI GetTheme(                                                                                 \
    /* [out] */ Int32* theme);                                                                   \
                                                                                                 \
CARAPI SetCancelable(                                                                            \
    /* [in] */ Boolean cancelable);                                                              \
                                                                                                 \
CARAPI IsCancelable(                                                                             \
    /* [out] */ Boolean* cancelable);                                                            \
                                                                                                 \
CARAPI SetShowsDialog(                                                                           \
    /* [in] */ Boolean showsDialog);                                                             \
                                                                                                 \
CARAPI GetShowsDialog(                                                                           \
    /* [out] */ Boolean* showsDialog);                                                           \
                                                                                                 \
CARAPI OnCreateDialog(                                                                           \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ IDialog** dialog);


#define IDIALOGFRAGMENT_METHODS_IMPL(className, superClass)                                      \
ECode className::SetStyle(                                                                       \
    /* [in] */ Int32 style,                                                                      \
    /* [in] */ Int32 theme)                                                                      \
{                                                                                                \
    return superClass::SetStyle(style, theme);                                                   \
}                                                                                                \
                                                                                                 \
ECode className::Show(                                                                           \
    /* [in] */ IFragmentManager* manager,                                                        \
    /* [in] */ const String& tag)                                                                \
{                                                                                                \
    return superClass::Show(manager, tag);                                                       \
}                                                                                                \
                                                                                                 \
ECode className::ShowEx(                                                                         \
    /* [in] */ IFragmentTransaction* transaction,                                                \
    /* [in] */ const String& tag,                                                                \
    /* [out] */ Int32* id)                                                                       \
{                                                                                                \
    return superClass::ShowEx(transaction, tag, id);                                             \
}                                                                                                \
                                                                                                 \
ECode className::Dismiss()                                                                       \
{                                                                                                \
    return superClass::Dismiss();                                                                \
}                                                                                                \
                                                                                                 \
ECode className::DismissAllowingStateLoss()                                                      \
{                                                                                                \
    return superClass::DismissAllowingStateLoss();                                               \
}                                                                                                \
                                                                                                 \
ECode className::GetDialog(                                                                      \
    /* [out] */ IDialog** dialog)                                                                \
{                                                                                                \
    return superClass::GetDialog(dialog);                                                        \
}                                                                                                \
                                                                                                 \
ECode className::GetTheme(                                                                       \
    /* [out] */ Int32* theme)                                                                    \
{                                                                                                \
    return superClass::GetTheme(theme);                                                          \
}                                                                                                \
                                                                                                 \
ECode className::SetCancelable(                                                                  \
    /* [in] */ Boolean cancelable)                                                               \
{                                                                                                \
    return superClass::SetCancelable(cancelable);                                                \
}                                                                                                \
                                                                                                 \
ECode className::IsCancelable(                                                                   \
    /* [out] */ Boolean* cancelable)                                                             \
{                                                                                                \
    return superClass::IsCancelable(cancelable);                                                 \
}                                                                                                \
                                                                                                 \
ECode className::SetShowsDialog(                                                                 \
    /* [in] */ Boolean showsDialog)                                                              \
{                                                                                                \
    return superClass::SetShowsDialog(showsDialog);                                              \
}                                                                                                \
                                                                                                 \
ECode className::GetShowsDialog(                                                                 \
    /* [out] */ Boolean* showsDialog)                                                            \
{                                                                                                \
    return superClass::GetShowsDialog(showsDialog);                                              \
}                                                                                                \
                                                                                                 \
ECode className::OnCreateDialog(                                                                 \
    /* [in] */ IBundle* savedInstanceState,                                                      \
    /* [out] */ IDialog** dialog)                                                                \
{                                                                                                \
    return superClass::OnCreateDialog(savedInstanceState, dialog);                               \
}

#endif //__FRAGMENTMACRO_H__
