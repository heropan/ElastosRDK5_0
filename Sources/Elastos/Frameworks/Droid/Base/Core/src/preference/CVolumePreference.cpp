
#include "CVolumePreference.h"
#include "CVolumePreferenceSeekBarVolumizer.h"
#include "R.h"

using Elastos::Droid::View::IWindow;

namespace Elastos {
namespace Droid {
namespace Preference {

const String CVolumePreference::TAG("VolumePreference");

CAR_INTERFACE_IMPL(CVolumePreference::SavedState, IPreferenceBaseSavedState);

CVolumePreference::SavedState::SavedState(
    /* [in] */ IParcel* source)
    : PreferenceBaseSavedState(source)
{
    ReadFromParcel(source);
}

CVolumePreference::SavedState::SavedState(
    /* [in] */ IParcelable* superState)
    : PreferenceBaseSavedState(superState)
{}

ECode CVolumePreference::SavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    PreferenceBaseSavedState::ReadFromParcel(source);
    Int32 temp;
    FAIL_RETURN(source->ReadInt32(&temp));
    mVolumeStore->SetVolume(temp);
    FAIL_RETURN(source->ReadInt32(&temp));
    mVolumeStore->SetOriginalVolume(temp);

    return NOERROR;
}

ECode CVolumePreference::SavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    Int32 temp;
    mVolumeStore->GetVolume(&temp);
    dest->WriteInt32(temp);
    mVolumeStore->GetOriginalVolume(&temp);
    dest->WriteInt32(temp);

    return NOERROR;
}

ECode CVolumePreference::SavedState::GetVolumeStore(
    /* [out] */ IVolumePreferenceVolumeStore** volumeStore)
{
    VALIDATE_NOT_NULL(volumeStore);
    *volumeStore = mVolumeStore;
    return NOERROR;
}

ECode CVolumePreference::SavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState)
    return PreferenceBaseSavedState::GetSuperState(superState);
}

CVolumePreference::CVolumePreference()
    : mStreamType(0)
{}

CVolumePreference::~CVolumePreference()
{}

ECode CVolumePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    SeekBarDialogPreference::Init(context, attrs);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::VolumePreference),
            ARRAY_SIZE(R::styleable::VolumePreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    a->GetInt32(R::styleable::VolumePreference_streamType, 0, &mStreamType);
    a->Recycle();

    return NOERROR;
}

PInterface CVolumePreference::Probe(
    /* [in]  */ REIID riid)
{
    return _CVolumePreference::Probe(riid);
}

ECode CVolumePreference::SetStreamType(
    /* [in] */ Int32 streamType)
{
    mStreamType = streamType;
    return NOERROR;
}

ECode CVolumePreference::OnBindDialogView(
    /* [in] */ IView* view)
{
    SeekBarDialogPreference::OnBindDialogView(view);

    AutoPtr<ISeekBar> seekBar;
    FAIL_RETURN(view->FindViewById(R::id::seekbar, (IView**)&seekBar));
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IVolumePreference> preference = (IVolumePreference*)this;
    mSeekBarVolumizer = NULL;
    CVolumePreferenceSeekBarVolumizer::New(preference, context, seekBar, mStreamType,
        (IVolumePreferenceSeekBarVolumizer**)&mSeekBarVolumizer);

    AutoPtr<IPreferenceManager> manager;
    FAIL_RETURN(GetPreferenceManager((IPreferenceManager**)&manager));
    AutoPtr<IPreferenceManagerOnActivityStopListener> listener = (IPreferenceManagerOnActivityStopListener*)this;
    // manager->RegisterOnActivityStopListener(listener);

    // grab focus and key events so that pressing the volume buttons in the
    // dialog doesn't also show the normal volume adjust toast.
    AutoPtr<IViewOnKeyListener> keyListener = (IViewOnKeyListener*)this;
    view->SetOnKeyListener(keyListener);
    view->SetFocusableInTouchMode(TRUE);
    Boolean result;
    view->RequestFocus(&result);

    return NOERROR;
}

ECode CVolumePreference::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // If key arrives immediately after the activity has been cleaned up.
    if (mSeekBarVolumizer == NULL) {
        *result = TRUE;
        return NOERROR;
    }
    Int32 action;
    event->GetAction(&action);
    Boolean isdown = (action == IKeyEvent::ACTION_DOWN);
    switch (keyCode) {
        case IKeyEvent::KEYCODE_VOLUME_DOWN: {
            if (isdown) {
                mSeekBarVolumizer->ChangeVolumeBy(-1);
            }
            *result = TRUE;

            return NOERROR;
        }
        case IKeyEvent::KEYCODE_VOLUME_UP: {
            if (isdown) {
                mSeekBarVolumizer->ChangeVolumeBy(1);
            }
            *result = TRUE;

            return NOERROR;
        }
        case IKeyEvent::KEYCODE_VOLUME_MUTE: {
            if (isdown) {
                mSeekBarVolumizer->MuteVolume();
            }
            *result = TRUE;

            return NOERROR;
        }
        default: {
            *result = FALSE;

            return NOERROR;
        }
    }

    return NOERROR;
}

ECode CVolumePreference::OnDialogClosed(
    /* [in] */ Boolean positiveResult)
{
    FAIL_RETURN(SeekBarDialogPreference::OnDialogClosed(positiveResult));

    if (!positiveResult && mSeekBarVolumizer != NULL) {
        mSeekBarVolumizer->RevertVolume();
    }

    Cleanup();

    return NOERROR;
}

ECode CVolumePreference::OnActivityStop()
{
    if (mSeekBarVolumizer != NULL) {
        mSeekBarVolumizer->StopSample();
    }

    return NOERROR;
}

ECode CVolumePreference::Cleanup()
{
    AutoPtr<IPreferenceManager> manager;
    FAIL_RETURN(GetPreferenceManager((IPreferenceManager**)&manager));
    AutoPtr<IPreferenceManagerOnActivityStopListener> listener = (IPreferenceManagerOnActivityStopListener*)this;
    // manager->UnregisterOnActivityStopListener(listener);

   if (mSeekBarVolumizer != NULL) {
       AutoPtr<IDialog> dialog;
       GetDialog((IDialog**)&dialog);
       if (dialog != NULL) {
            Boolean isShowing;
            dialog->IsShowing(&isShowing);
            if (isShowing) {
                AutoPtr<IWindow> window;
                dialog->GetWindow((IWindow**)&window);
                AutoPtr<IView> decorView;
                window->GetDecorView((IView**)&decorView);
                AutoPtr<IView> view;
                decorView->FindViewById(R::id::seekbar, (IView**)&view);
                if (view != NULL) {
                    view->SetOnKeyListener(NULL);
                }
               // Stopped while dialog was showing, revert changes
               mSeekBarVolumizer->RevertVolume();
            }
       }
       mSeekBarVolumizer->Stop();
       mSeekBarVolumizer = NULL;
   }

    return NOERROR;
}

ECode CVolumePreference::OnSampleStarting(
    /* [in] */ IVolumePreferenceSeekBarVolumizer* volumizer)
{
    if (mSeekBarVolumizer != NULL && volumizer != mSeekBarVolumizer.Get()) {
        mSeekBarVolumizer->StopSample();
    }

    return NOERROR;
}

ECode CVolumePreference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state);

    AutoPtr<IParcelable> superState;
    FAIL_RETURN(SeekBarDialogPreference::OnSaveInstanceState((IParcelable**)&superState));
    Boolean isPersistent;
    FAIL_RETURN(IsPersistent(&isPersistent));
    if (isPersistent) {
        // No need to save instance state since it's persistent
        *state = superState;
        return NOERROR;
    }

    AutoPtr<SavedState> myState = new SavedState(superState);
    if (mSeekBarVolumizer != NULL) {
        AutoPtr<IVolumePreferenceVolumeStore> store;
        myState->GetVolumeStore((IVolumePreferenceVolumeStore**)&store);
        mSeekBarVolumizer->OnSaveInstanceState(store);
    }
    *state = (IParcelable*)myState->Probe(EIID_IParcelable);

    return NOERROR;
}

ECode CVolumePreference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return SeekBarDialogPreference::OnRestoreInstanceState(state);
    }

    AutoPtr<SavedState> myState = (SavedState*)state->Probe(state);
    if (myState == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return SeekBarDialogPreference::OnRestoreInstanceState(state);
    }

    AutoPtr<IParcelable> superParcel;
    myState->GetSuperState((IParcelable**)&superParcel);
    SeekBarDialogPreference::OnRestoreInstanceState(superParcel);
    if (mSeekBarVolumizer != NULL) {
        AutoPtr<IVolumePreferenceVolumeStore> store;
        myState->GetVolumeStore((IVolumePreferenceVolumeStore**)&store);
        mSeekBarVolumizer->OnRestoreInstanceState(store);
    }

    return NOERROR;
}

ECode CVolumePreference::OnDependencyChanged(
    /* [in] */ IPreference* dependency,
    /* [in] */ Boolean disableDependent)
{
    return E_NOT_IMPLEMENTED;
}

ECode CVolumePreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    return SeekBarDialogPreference::OnGetDefaultValue(a, index, value);
}

ECode CVolumePreference::SetIntent(
    /* [in] */ IIntent* intent)
{
    return SeekBarDialogPreference::SetIntent(intent);
}

ECode CVolumePreference::GetIntent(
    /* [out] */ IIntent** intent)
{
    return SeekBarDialogPreference::GetIntent(intent);;
}

ECode CVolumePreference::SetFragment(
    /* [in] */ const String& fragment)
{
    return SeekBarDialogPreference::SetFragment(fragment);;
}

ECode CVolumePreference::GetFragment(
    /* [out] */ String* fragment)
{
    return SeekBarDialogPreference::GetFragment(fragment);
}

ECode CVolumePreference::GetExtras(
    /* [out] */ IBundle** extras)
{
    return SeekBarDialogPreference::GetExtras(extras);
}

ECode CVolumePreference::PeekExtras(
    /* [out] */ IBundle** extras)
{
    return SeekBarDialogPreference::PeekExtras(extras);
}

ECode CVolumePreference::SetLayoutResource(
    /* [in] */ Int32 layoutResId)
{
    return SeekBarDialogPreference::SetLayoutResource(layoutResId);
}

ECode CVolumePreference::GetLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    return SeekBarDialogPreference::GetLayoutResource(layoutResId);
}

ECode CVolumePreference::SetWidgetLayoutResource(
    /* [in] */ Int32 widgetLayoutResId)
{
    return SeekBarDialogPreference::SetWidgetLayoutResource(widgetLayoutResId);
}

ECode CVolumePreference::GetWidgetLayoutResource(
    /* [out] */ Int32* widgetLayoutResId)
{
    return SeekBarDialogPreference::GetWidgetLayoutResource(widgetLayoutResId);
}

ECode CVolumePreference::GetView(
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    return SeekBarDialogPreference::GetView(convertView, parent, view);
}

ECode CVolumePreference::OnCreateView(
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    return SeekBarDialogPreference::OnCreateView(parent, view);
}

ECode CVolumePreference::OnBindView(
    /* [in] */ IView* view)
{
    return SeekBarDialogPreference::OnBindView(view);
}

ECode CVolumePreference::SetOrder(
    /* [in] */ Int32 order)
{
    return SeekBarDialogPreference::SetOrder(order);
}

ECode CVolumePreference::GetOrder(
    /* [out] */ Int32* order)
{
    return SeekBarDialogPreference::GetOrder(order);
}

ECode CVolumePreference::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return SeekBarDialogPreference::SetTitle(title);
}

ECode CVolumePreference::SetTitleEx(
    /* [in] */ Int32 titleResId)
{
    return SeekBarDialogPreference::SetTitleEx(titleResId);
}

ECode CVolumePreference::GetTitleRes(
    /* [out] */ Int32* resId)
{
    return SeekBarDialogPreference::GetTitleRes(resId);
}

ECode CVolumePreference::GetTitle(
    /* [out] */ ICharSequence** title)
{
    return SeekBarDialogPreference::GetTitle(title);
}

ECode CVolumePreference::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return SeekBarDialogPreference::SetIcon(icon);
}

ECode CVolumePreference::SetIconEx(
    /* [in] */ Int32 iconResId)
{
    return SeekBarDialogPreference::SetIconEx(iconResId);
}

ECode CVolumePreference::GetIcon(
    /* [out] */ IDrawable** drawable)
{
    return SeekBarDialogPreference::GetIcon(drawable);
}

ECode CVolumePreference::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    return SeekBarDialogPreference::GetSummary(summary);
}

ECode CVolumePreference::SetSummary(
    /* [in] */ ICharSequence* summary)
{
    return SeekBarDialogPreference::SetSummary(summary);
}

ECode CVolumePreference::SetSummaryEx(
    /* [in] */ Int32 summaryResId)
{
    return SeekBarDialogPreference::SetSummaryEx(summaryResId);
}

ECode CVolumePreference::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return SeekBarDialogPreference::SetEnabled(enabled);
}

ECode CVolumePreference::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    return SeekBarDialogPreference::IsEnabled(isEnable);
}

ECode CVolumePreference::SetSelectable(
    /* [in] */ Boolean selectable)
{
    return SeekBarDialogPreference::SetSelectable(selectable);
}

ECode CVolumePreference::IsSelectable(
    /* [out] */ Boolean* isSelectable)
{
    return SeekBarDialogPreference::IsSelectable(isSelectable);
}

ECode CVolumePreference::SetShouldDisableView(
    /* [in] */ Boolean shouldDisableView)
{
    return SeekBarDialogPreference::SetShouldDisableView(shouldDisableView);
}

ECode CVolumePreference::GetShouldDisableView(
    /* [out] */ Boolean* shouldDisableView)
{
    return SeekBarDialogPreference::GetShouldDisableView(shouldDisableView);
}

ECode CVolumePreference::OnClick()
{
    return SeekBarDialogPreference::OnClick();
}

ECode CVolumePreference::SetKey(
    /* [in] */ const String& key)
{
    return SeekBarDialogPreference::SetKey(key);
}

ECode CVolumePreference::GetKey(
    /* [out] */ String* key)
{
    return SeekBarDialogPreference::GetKey(key);
}

ECode CVolumePreference::RequireKey()
{
    return SeekBarDialogPreference::RequireKey();
}

ECode CVolumePreference::HasKey(
    /* [out] */ Boolean* hasKey)
{
    return SeekBarDialogPreference::HasKey(hasKey);
}

ECode CVolumePreference::IsPersistent(
    /* [out] */ Boolean* isPersistent)
{
    return SeekBarDialogPreference::IsPersistent(isPersistent);
}

ECode CVolumePreference::ShouldPersist(
    /* [out] */ Boolean* shouldPersist)
{
    return SeekBarDialogPreference::ShouldPersist(shouldPersist);
}

ECode CVolumePreference::SetPersistent(
    /* [in] */ Boolean persistent)
{
    return SeekBarDialogPreference::SetPersistent(persistent);
}

ECode CVolumePreference::CallChangeListener(
    /* [in] */ IInterface* newValue,
    /* [out] */ Boolean* shouldSetValue)
{
    return SeekBarDialogPreference::CallChangeListener(newValue, shouldSetValue);
}

ECode CVolumePreference::SetOnPreferenceChangeListener(
    /* [in] */ IPreferenceOnPreferenceChangeListener* onPreferenceChangeListener)
{
    return SeekBarDialogPreference::SetOnPreferenceChangeListener(onPreferenceChangeListener);
}

ECode CVolumePreference::GetOnPreferenceChangeListener(
    /* [out] */ IPreferenceOnPreferenceChangeListener** listener)
{
    return SeekBarDialogPreference::GetOnPreferenceChangeListener(listener);
}

ECode CVolumePreference::SetOnPreferenceClickListener(
    /* [in] */ IPreferenceOnPreferenceClickListener* onPreferenceClickListener)
{
    return SeekBarDialogPreference::SetOnPreferenceClickListener(onPreferenceClickListener);
}

ECode CVolumePreference::GetOnPreferenceClickListener(
    /* [out] */ IPreferenceOnPreferenceClickListener** listener)
{
    return SeekBarDialogPreference::GetOnPreferenceClickListener(listener);
}

ECode CVolumePreference::PerformClick(
    /* [in] */ IPreferenceScreen* preferenceScreen)
{
    return SeekBarDialogPreference::PerformClick(preferenceScreen);
}

ECode CVolumePreference::GetContext(
    /* [out] */ IContext** context)
{
    return SeekBarDialogPreference::GetContext(context);
}

ECode CVolumePreference::GetSharedPreferences(
    /* [out] */ ISharedPreferences** preference)
{
    return SeekBarDialogPreference::GetSharedPreferences(preference);
}

ECode CVolumePreference::GetEditor(
    /* [out] */ ISharedPreferencesEditor** editor)
{
    return SeekBarDialogPreference::GetEditor(editor);
}

ECode CVolumePreference::ShouldCommit(
    /* [out] */ Boolean* shouldCommit)
{
    return SeekBarDialogPreference::ShouldCommit(shouldCommit);
}

ECode CVolumePreference::NotifyChanged()
{
    return SeekBarDialogPreference::NotifyChanged();
}

ECode CVolumePreference::NotifyHierarchyChanged()
{
    return SeekBarDialogPreference::NotifyHierarchyChanged();
}

ECode CVolumePreference::GetPreferenceManager(
    /* [out] */ IPreferenceManager** manager)
{
    return SeekBarDialogPreference::GetPreferenceManager(manager);
}

ECode CVolumePreference::OnAttachedToHierarchy(
    /* [in] */ IPreferenceManager* preferenceManager)
{
    return SeekBarDialogPreference::OnAttachedToHierarchy(preferenceManager);
}

ECode CVolumePreference::OnAttachedToActivity()
{
    return SeekBarDialogPreference::OnAttachedToActivity();
}

ECode CVolumePreference::FindPreferenceInHierarchy(
    /* [in] */ const String& key,
    /* [out] */ IPreference** preference)
{
    return SeekBarDialogPreference::FindPreferenceInHierarchy(key, preference);
}

ECode CVolumePreference::NotifyDependencyChange(
    /* [in] */ Boolean disableDependents)
{
    return SeekBarDialogPreference::NotifyDependencyChange(disableDependents);
}

ECode CVolumePreference::ShouldDisableDependents(
    /* [out] */ Boolean* shouldDisableDependents)
{
    return SeekBarDialogPreference::ShouldDisableDependents(shouldDisableDependents);
}

ECode CVolumePreference::SetDependency(
    /* [in] */ const String& dependencyKey)
{
    return SeekBarDialogPreference::SetDependency(dependencyKey);
}

ECode CVolumePreference::GetDependency(
    /* [out] */ String* dependencyKey)
{
    return SeekBarDialogPreference::GetDependency(dependencyKey);
}

ECode CVolumePreference::OnPrepareForRemoval()
{
    return SeekBarDialogPreference::OnPrepareForRemoval();
}

ECode CVolumePreference::SetDefaultValue(
    /* [in] */ IInterface* defaultValue)
{
    return SeekBarDialogPreference::SetDefaultValue(defaultValue);
}

ECode CVolumePreference::OnSetInitialValue(
    /* [in] */ Boolean restorePersistedValue,
    /* [in] */ IInterface* defaultValue)
{
    return SeekBarDialogPreference::OnSetInitialValue(restorePersistedValue, defaultValue);
}

ECode CVolumePreference::PersistString(
    /* [in] */ const String& value,
    /* [out] */ Boolean* isPersistString)
{
    return SeekBarDialogPreference::PersistString(value, isPersistString);
}

ECode CVolumePreference::GetPersistedString(
    /* [in] */ const String& defaultReturnValue,
    /* [out] */ String* str)
{
    return SeekBarDialogPreference::GetPersistedString(defaultReturnValue, str);
}

ECode CVolumePreference::PersistStringSet(
    /* [in] */ ArrayOf<String>* values,
    /* [out] */ Boolean* isPersistStringSet)
{
    return SeekBarDialogPreference::PersistStringSet(values, isPersistStringSet);
}

ECode CVolumePreference::GetPersistedStringSet(
    /* [in] */ ArrayOf<String>* defaultReturnValue,
    /* [out, callee] */ ArrayOf<String>** value)
{
    return SeekBarDialogPreference::GetPersistedStringSet(defaultReturnValue, value);
}

ECode CVolumePreference::PersistInt32(
    /* [in] */ Int32 value,
    /* [out] */ Boolean* isPersist)
{
    return SeekBarDialogPreference::PersistInt32(value, isPersist);
}

ECode CVolumePreference::GetPersistedInt32(
    /* [in] */ Int32 defaultReturnValue,
    /* [out] */ Int32* value)
{
    return SeekBarDialogPreference::GetPersistedInt32(defaultReturnValue, value);
}

ECode CVolumePreference::PersistFloat(
    /* [in] */ Float value,
    /* [out] */ Boolean* isPersist)
{
    return SeekBarDialogPreference::PersistFloat(value, isPersist);
}

ECode CVolumePreference::GetPersistedFloat(
    /* [in] */ Float defaultReturnValue,
    /* [out] */ Float* value)
{
    return SeekBarDialogPreference::GetPersistedFloat(defaultReturnValue, value);
}

ECode CVolumePreference::PersistInt64(
    /* [in] */ Int64 value,
    /* [out] */ Boolean* isPersist)
{
    return SeekBarDialogPreference::PersistInt64(value, isPersist);
}

ECode CVolumePreference::GetPersistedInt64(
    /* [in] */ Int64 defaultReturnValue,
    /* [out] */ Int64* value)
{
    return SeekBarDialogPreference::GetPersistedInt64(defaultReturnValue, value);
}

ECode CVolumePreference::PersistBoolean(
    /* [in] */ Boolean value,
    /* [out] */ Boolean* isPersist)
{
    return SeekBarDialogPreference::PersistBoolean(value, isPersist);
}

ECode CVolumePreference::GetPersistedBoolean(
    /* [in] */ Boolean defaultReturnValue,
    /* [out] */ Boolean* value)
{
    return SeekBarDialogPreference::GetPersistedBoolean(defaultReturnValue, value);
}

ECode CVolumePreference::HasSpecifiedLayout(
    /* [out] */ Boolean* hasSpecifiedLayout)
{
    return SeekBarDialogPreference::HasSpecifiedLayout(hasSpecifiedLayout);
}

ECode CVolumePreference::ToString(
    /* [out] */ String* str)
{
    return SeekBarDialogPreference::ToString(str);
}

ECode CVolumePreference::SaveHierarchyState(
    /* [in] */ IBundle* container)
{
    return SeekBarDialogPreference::SaveHierarchyState(container);
}

ECode CVolumePreference::DispatchSaveInstanceState(
    /* [in] */ IBundle* container)
{
    return SeekBarDialogPreference::DispatchSaveInstanceState(container);
}

ECode CVolumePreference::RestoreHierarchyState(
    /* [in] */ IBundle* container)
{
    return SeekBarDialogPreference::RestoreHierarchyState(container);
}

ECode CVolumePreference::DispatchRestoreInstanceState(
    /* [in] */ IBundle* container)
{
    return SeekBarDialogPreference::DispatchRestoreInstanceState(container);
}

ECode CVolumePreference::SetDialogTitle(
    /* [in] */ ICharSequence* dialogTitle)
{
    return SeekBarDialogPreference::SetDialogTitle(dialogTitle);
}

ECode CVolumePreference::SetDialogTitleEx(
    /* [in] */ Int32 dialogTitleResId)
{
    return SeekBarDialogPreference::SetDialogTitleEx(dialogTitleResId);
}

ECode CVolumePreference::GetDialogTitle(
    /* [out] */ ICharSequence** title)
{
    return SeekBarDialogPreference::GetDialogTitle(title);
}

ECode CVolumePreference::SetDialogMessage(
    /* [in] */ ICharSequence* dialogMessage)
{
    return SeekBarDialogPreference::SetDialogMessage(dialogMessage);
}

ECode CVolumePreference::SetDialogMessageEx(
    /* [in] */ Int32 dialogMessageResId)
{
    return SeekBarDialogPreference::SetDialogMessageEx(dialogMessageResId);
}

ECode CVolumePreference::GetDialogMessage(
    /* [out] */ ICharSequence** message)
{
    return SeekBarDialogPreference::GetDialogMessage(message);
}

ECode CVolumePreference::SetDialogIcon(
    /* [in] */ IDrawable* dialogIcon)
{
    return SeekBarDialogPreference::SetDialogIcon(dialogIcon);
}

ECode CVolumePreference::SetDialogIconEx(
    /* [in] */ Int32 dialogIconRes)
{
    return SeekBarDialogPreference::SetDialogIconEx(dialogIconRes);
}

ECode CVolumePreference::GetDialogIcon(
    /* [out] */ IDrawable** icon)
{
    return SeekBarDialogPreference::GetDialogIcon(icon);
}

ECode CVolumePreference::SetPositiveButtonText(
    /* [in] */ ICharSequence* positiveButtonText)
{
    return SeekBarDialogPreference::SetPositiveButtonText(positiveButtonText);
}

ECode CVolumePreference::SetPositiveButtonTextEx(
    /* [in] */ Int32 positiveButtonTextResId)
{
    return SeekBarDialogPreference::SetPositiveButtonTextEx(positiveButtonTextResId);
}

ECode CVolumePreference::GetPositiveButtonText(
    /* [out] */ ICharSequence** text)
{
    return SeekBarDialogPreference::GetPositiveButtonText(text);
}

ECode CVolumePreference::SetNegativeButtonText(
    /* [in] */ ICharSequence* negativeButtonText)
{
    return SeekBarDialogPreference::SetNegativeButtonText(negativeButtonText);
}

ECode CVolumePreference::SetNegativeButtonTextEx(
    /* [in] */ Int32 negativeButtonTextResId)
{
    return SeekBarDialogPreference::SetNegativeButtonTextEx(negativeButtonTextResId);
}

ECode CVolumePreference::GetNegativeButtonText(
    /* [out] */ ICharSequence** text)
{
    return SeekBarDialogPreference::GetNegativeButtonText(text);
}

ECode CVolumePreference::SetDialogLayoutResource(
    /* [in] */ Int32 dialogLayoutResId)
{
    return SeekBarDialogPreference::SetDialogLayoutResource(dialogLayoutResId);
}

ECode CVolumePreference::GetDialogLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    return SeekBarDialogPreference::GetDialogLayoutResource(layoutResId);
}

ECode CVolumePreference::OnPrepareDialogBuilder(
    /* [in] */ IAlertDialogBuilder* builder)
{
    return SeekBarDialogPreference::OnPrepareDialogBuilder(builder);
}

ECode CVolumePreference::ShowDialog(
    /* [in] */ IBundle* state)
{
    return SeekBarDialogPreference::ShowDialog(state);
}

ECode CVolumePreference::NeedInputMethod(
    /* [out] */ Boolean* isNeed)
{
    return SeekBarDialogPreference::NeedInputMethod(isNeed);
}

ECode CVolumePreference::OnCreateDialogView(
    /* [out] */ IView** view)
{
    return SeekBarDialogPreference::OnCreateDialogView(view);
}

ECode CVolumePreference::OnClickEx(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    return SeekBarDialogPreference::OnClickEx(dialog, which);
}

ECode CVolumePreference::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    return SeekBarDialogPreference::OnDismiss(dialog);
}

ECode CVolumePreference::GetDialog(
    /* [out] */ IDialog** dialog)
{
    return SeekBarDialogPreference::GetDialog(dialog);
}

ECode CVolumePreference::OnActivityDestroy()
{
    return SeekBarDialogPreference::OnActivityDestroy();
}

ECode CVolumePreference::CreateActionButtons()
{
    return SeekBarDialogPreference::CreateActionButtons();
}

}
}
}

