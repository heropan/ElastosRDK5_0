
#include "CSeekBarPreference.h"
#include "R.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;

namespace Elastos {
namespace Droid {
namespace Preference {

CAR_INTERFACE_IMPL(CSeekBarPreference::SavedState, IPreferenceBaseSavedState);

CSeekBarPreference::SavedState::SavedState(
    /* [in] */ IParcel* source)
    : PreferenceBaseSavedState(source)
    , mProgress(0)
    , mMax(0)
{
    ReadFromParcel(source);
}

CSeekBarPreference::SavedState::SavedState(
    /* [in] */ IParcelable* superState)
    : PreferenceBaseSavedState(superState)
    , mProgress(0)
    , mMax(0)
{}

ECode CSeekBarPreference::SavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    PreferenceBaseSavedState::ReadFromParcel(source);
    FAIL_RETURN(source->ReadInt32(&mProgress));
    FAIL_RETURN(source->ReadInt32(&mMax));

    return NOERROR;
}

ECode CSeekBarPreference::SavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    PreferenceBaseSavedState::WriteToParcel(dest);
    dest->WriteInt32(mProgress);
    dest->WriteInt32(mMax);

    return NOERROR;
}

ECode CSeekBarPreference::SavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState)
    return PreferenceBaseSavedState::GetSuperState(superState);
}

CSeekBarPreference::CSeekBarPreference()
    : mProgress(0)
    , mMax(0)
    , mTrackingTouch(FALSE)
{}

CSeekBarPreference::~CSeekBarPreference()
{}

ECode CSeekBarPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Preference::Init(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ProgressBar),
            ARRAY_SIZE(R::styleable::ProgressBar));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);
    Int32 max;
    a->GetInt32(R::styleable::ProgressBar_max, mMax, &max);
    SetMax(max);
    a->Recycle();
    SetLayoutResource(R::layout::preference_widget_seekbar);

    return NOERROR;
}

ECode CSeekBarPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CSeekBarPreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

PInterface CSeekBarPreference::Probe(
    /* [in]  */ REIID riid)
{
    return _CSeekBarPreference::Probe(riid);
}

ECode CSeekBarPreference::OnBindView(
    /* [in] */ IView* view)
{
    Preference::OnBindView(view);
    AutoPtr<ISeekBar> seekBar;
    FAIL_RETURN(view->FindViewById(R::id::seekbar, (IView**)&seekBar));
    seekBar->SetOnSeekBarChangeListener(this);
    seekBar->SetMax(mMax);
    seekBar->SetProgress(mProgress);
    Boolean isEnabled;
    IsEnabled(&isEnabled);
    seekBar->SetEnabled(isEnabled);

    return NOERROR;
}

ECode CSeekBarPreference::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary);
    *summary = NULL;
    return NOERROR;
}

ECode CSeekBarPreference::OnSetInitialValue(
    /* [in] */ Boolean restoreValue,
    /* [in] */ IInterface* defaultValue)
{
    Int32 progress = 0;
    ((IInteger32*)defaultValue)->GetValue(&progress);
    if (restoreValue) {
        GetPersistedInt32(mProgress, &progress);
    }
    SetProgress(progress);

    return NOERROR;
}

ECode CSeekBarPreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(a);
    VALIDATE_NOT_NULL(value);

    Int32 lvalue;
    FAIL_RETURN(a->GetInt32(index, 0, &lvalue));

    return CInteger32::New(lvalue, (IInteger32**)value);
}

ECode CSeekBarPreference::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 action;
    FAIL_RETURN(event->GetAction(&action));
    if (action != IKeyEvent::ACTION_UP) {
        if (keyCode == IKeyEvent::KEYCODE_PLUS
                || keyCode == IKeyEvent::KEYCODE_EQUALS) {
            Int32 progress;
            GetProgress(&progress);
            progress += 1;
            SetProgress(progress);
            *result = TRUE;
            return NOERROR;
        }
        if (keyCode == IKeyEvent::KEYCODE_MINUS) {
            Int32 progress;
            GetProgress(&progress);
            progress -= 1;
            SetProgress(progress);
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;

    return NOERROR;
}

ECode CSeekBarPreference::SetMax(
    /* [in] */ Int32 max)
{
    if (max != mMax) {
        mMax = max;
        NotifyChanged();
    }
    return E_NOT_IMPLEMENTED;
}

ECode CSeekBarPreference::SetProgress(
    /* [in] */ Int32 progress)
{
    return SetProgress(progress, TRUE);
}

ECode CSeekBarPreference::SetProgress(
    /* [in] */ Int32 progress,
    /* [in] */ Boolean notifyChanged)
{
    if (progress > mMax) {
        progress = mMax;
    }
    if (progress < 0) {
        progress = 0;
    }
    if (progress != mProgress) {
        mProgress = progress;
        Boolean result;
        FAIL_RETURN(PersistInt32(progress, &result));
        if (notifyChanged) {
            NotifyChanged();
        }
    }

    return NOERROR;
}

ECode CSeekBarPreference::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);
    *progress = mProgress;
    return NOERROR;
}

ECode CSeekBarPreference::SyncProgress(
    /* [in] */ ISeekBar* seekBar)
{
    Int32 progress;
    seekBar->GetProgress(&progress);
    if (progress != mProgress) {
        AutoPtr<IInteger32> iprogress;
        CInteger32::New(progress, (IInteger32**)&iprogress);
        AutoPtr<IInterface> iprogr = iprogress->Probe(EIID_IInterface);
        Boolean result;
        CallChangeListener(iprogr, &result);
        if (result) {
            SetProgress(progress, FALSE);
        } else {
            seekBar->SetProgress(mProgress);
        }
    }

    return NOERROR;
}

ECode CSeekBarPreference::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    if (fromUser && !mTrackingTouch) {
        SyncProgress(seekBar);
    }

    return NOERROR;
}

ECode CSeekBarPreference::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mTrackingTouch = TRUE;
    return NOERROR;
}

ECode CSeekBarPreference::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mTrackingTouch = FALSE;
    Int32 progress;
    seekBar->GetProgress(&progress);
    if (progress != mProgress) {
        SyncProgress(seekBar);
    }

    return NOERROR;
}

ECode CSeekBarPreference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state);

    /*
     * Suppose a client uses this preference type without persisting. We
     * must save the instance state so it is able to, for example, survive
     * orientation changes.
     */

    AutoPtr<IParcelable> superState;
    Preference::OnSaveInstanceState(state);
    Boolean isPersistent;
    IsPersistent(&isPersistent);
    if (isPersistent) {
        // No need to save instance state since it's persistent
        *state = superState;
        REFCOUNT_ADD(*state);
        return NOERROR;
    }

    // Save the instance state
    AutoPtr<SavedState> myState = new SavedState(superState);
    myState->mProgress = mProgress;
    myState->mMax = mMax;
    *state = (IParcelable*)myState->Probe(EIID_IParcelable);
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode CSeekBarPreference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return Preference::OnRestoreInstanceState(state);
    }

    AutoPtr<SavedState> myState = (SavedState*)state->Probe(state);
    if (myState == NULL) {
        // Didn't save state for us in onSaveInstanceState
        return Preference::OnRestoreInstanceState(state);
    }

    // Restore the instance state
    AutoPtr<IParcelable> superParcel;
    myState->GetSuperState((IParcelable**)&superParcel);
    Preference::OnRestoreInstanceState(superParcel);
    mProgress = myState->mProgress;
    mMax = myState->mMax;
    NotifyChanged();

    return NOERROR;
}

ECode CSeekBarPreference::OnDependencyChanged(
    /* [in] */ IPreference* dependency,
    /* [in] */ Boolean disableDependent)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSeekBarPreference::SetIntent(
    /* [in] */ IIntent* intent)
{
    return Preference::SetIntent(intent);
}

ECode CSeekBarPreference::GetIntent(
    /* [out] */ IIntent** intent)
{
    return Preference::GetIntent(intent);
}

ECode CSeekBarPreference::SetFragment(
    /* [in] */ const String& fragment)
{
    return Preference::SetFragment(fragment);
}

ECode CSeekBarPreference::GetFragment(
    /* [out] */ String* fragment)
{
    return Preference::GetFragment(fragment);
}

ECode CSeekBarPreference::GetExtras(
    /* [in] */ IBundle** extras)
{
    return Preference::GetExtras(extras);
}

ECode CSeekBarPreference::PeekExtras(
    /* [in] */ IBundle** extras)
{
    return Preference::PeekExtras(extras);
}

ECode CSeekBarPreference::SetLayoutResource(
    /* [in] */ Int32 layoutResId)
{
    return Preference::SetLayoutResource(layoutResId);
}

ECode CSeekBarPreference::GetLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    return Preference::GetLayoutResource(layoutResId);
}

ECode CSeekBarPreference::SetWidgetLayoutResource(
    /* [in] */ Int32 widgetLayoutResId)
{
    return Preference::SetWidgetLayoutResource(widgetLayoutResId);
}

ECode CSeekBarPreference::GetWidgetLayoutResource(
    /* [out] */ Int32* widgetLayoutResId)
{
    return Preference::GetWidgetLayoutResource(widgetLayoutResId);
}

ECode CSeekBarPreference::GetView(
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ IView** view)
{
    return Preference::GetView(convertView, parent, view);
}

ECode CSeekBarPreference::OnCreateView(
    /* [in] */ IViewGroup* parent,
    /* [in] */ IView** view)
{
    return Preference::OnCreateView(parent, view);
}

ECode CSeekBarPreference::SetOrder(
    /* [in] */ Int32 order)
{
    return Preference::SetOrder(order);
}

ECode CSeekBarPreference::GetOrder(
    /* [out] */ Int32* order)
{
    return Preference::GetOrder(order);
}

ECode CSeekBarPreference::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return Preference::SetTitle(title);
}

ECode CSeekBarPreference::SetTitle(
    /* [in] */ Int32 titleResId)
{
    return Preference::SetTitle(titleResId);
}

ECode CSeekBarPreference::GetTitleRes(
    /* [out] */ Int32* resId)
{
    return Preference::GetTitleRes(resId);
}

ECode CSeekBarPreference::GetTitle(
    /* [out] */ ICharSequence** title)
{
    return Preference::GetTitle(title);
}

ECode CSeekBarPreference::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return Preference::SetIcon(icon);
}

ECode CSeekBarPreference::SetIcon(
    /* [in] */ Int32 iconResId)
{
    return Preference::SetIcon(iconResId);
}

ECode CSeekBarPreference::GetIcon(
    /* [in] */ IDrawable** drawable)
{
    return Preference::GetIcon(drawable);
}

ECode CSeekBarPreference::SetSummary(
    /* [in] */ ICharSequence* summary)
{
    return Preference::SetSummary(summary);
}

ECode CSeekBarPreference::SetSummary(
    /* [in] */ Int32 summaryResId)
{
    return Preference::SetSummary(summaryResId);
}

ECode CSeekBarPreference::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return Preference::SetEnabled(enabled);
}

ECode CSeekBarPreference::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    return Preference::IsEnabled(isEnable);
}

ECode CSeekBarPreference::SetSelectable(
    /* [in] */ Boolean selectable)
{
    return Preference::SetSelectable(selectable);
}

ECode CSeekBarPreference::IsSelectable(
    /* [out] */ Boolean* isSelectable)
{
    return Preference::IsSelectable(isSelectable);
}

ECode CSeekBarPreference::SetShouldDisableView(
    /* [in] */ Boolean shouldDisableView)
{
    return Preference::SetShouldDisableView(shouldDisableView);
}

ECode CSeekBarPreference::GetShouldDisableView(
    /* [out] */ Boolean* shouldDisableView)
{
    return Preference::GetShouldDisableView(shouldDisableView);
}

ECode CSeekBarPreference::OnClick()
{
    return Preference::OnClick();
}

ECode CSeekBarPreference::SetKey(
    /* [in] */ const String& key)
{
    return Preference::SetKey(key);
}

ECode CSeekBarPreference::GetKey(
    /* [out] */ String* key)
{
    return Preference::GetKey(key);
}

ECode CSeekBarPreference::RequireKey()
{
    return Preference::RequireKey();
}

ECode CSeekBarPreference::HasKey(
    /* [out] */ Boolean* hasKey)
{
    return Preference::HasKey(hasKey);
}

ECode CSeekBarPreference::IsPersistent(
    /* [out] */ Boolean* isPersistent)
{
    return Preference::IsPersistent(isPersistent);
}

ECode CSeekBarPreference::ShouldPersist(
    /* [out] */ Boolean* shouldPersist)
{
    return Preference::ShouldPersist(shouldPersist);
}

ECode CSeekBarPreference::SetPersistent(
    /* [in] */ Boolean persistent)
{
    return Preference::SetPersistent(persistent);
}

ECode CSeekBarPreference::CallChangeListener(
    /* [in] */ IInterface* newValue,
    /* [out] */ Boolean* shouldSetValue)
{
    return Preference::CallChangeListener(newValue, shouldSetValue);
}

ECode CSeekBarPreference::SetOnPreferenceChangeListener(
    /* [in] */ IPreferenceOnPreferenceChangeListener* onPreferenceChangeListener)
{
    return Preference::SetOnPreferenceChangeListener(onPreferenceChangeListener);
}

ECode CSeekBarPreference::GetOnPreferenceChangeListener(
    /* [out] */ IPreferenceOnPreferenceChangeListener** listener)
{
    return Preference::GetOnPreferenceChangeListener(listener);
}

ECode CSeekBarPreference::SetOnPreferenceClickListener(
    /* [in] */ IPreferenceOnPreferenceClickListener* onPreferenceClickListener)
{
    return Preference::SetOnPreferenceClickListener(onPreferenceClickListener);
}

ECode CSeekBarPreference::GetOnPreferenceClickListener(
    /* [out] */ IPreferenceOnPreferenceClickListener** listener)
{
    return Preference::GetOnPreferenceClickListener(listener);
}

ECode CSeekBarPreference::PerformClick(
    /* [in] */ IPreferenceScreen* preferenceScreen)
{
    return Preference::PerformClick(preferenceScreen);
}

ECode CSeekBarPreference::GetContext(
    /* [out] */ IContext** context)
{
    return Preference::GetContext(context);
}

ECode CSeekBarPreference::GetSharedPreferences(
    /* [in] */ ISharedPreferences** preference)
{
    return Preference::GetSharedPreferences(preference);
}

ECode CSeekBarPreference::GetEditor(
    /* [in] */ ISharedPreferencesEditor** editor)
{
    return Preference::GetEditor(editor);
}

ECode CSeekBarPreference::ShouldCommit(
    /* [out] */ Boolean* shouldCommit)
{
    return Preference::ShouldCommit(shouldCommit);
}

ECode CSeekBarPreference::NotifyChanged()
{
    return Preference::NotifyChanged();
}

ECode CSeekBarPreference::NotifyHierarchyChanged()
{
    return Preference::NotifyHierarchyChanged();
}

ECode CSeekBarPreference::GetPreferenceManager(
    /* [out] */ IPreferenceManager** manager)
{
    return Preference::GetPreferenceManager(manager);
}

ECode CSeekBarPreference::OnAttachedToHierarchy(
    /* [in] */ IPreferenceManager* preferenceManager)
{
    return Preference::OnAttachedToHierarchy(preferenceManager);
}

ECode CSeekBarPreference::OnAttachedToActivity()
{
    return Preference::OnAttachedToActivity();
}

ECode CSeekBarPreference::FindPreferenceInHierarchy(
    /* [in] */ const String& key,
    /* [out] */ IPreference** preference)
{
    return Preference::FindPreferenceInHierarchy(key, preference);
}

ECode CSeekBarPreference::NotifyDependencyChange(
    /* [in] */ Boolean disableDependents)
{
    return Preference::NotifyDependencyChange(disableDependents);
}

ECode CSeekBarPreference::ShouldDisableDependents(
    /* [out] */ Boolean* shouldDisableDependents)
{
    return Preference::ShouldDisableDependents(shouldDisableDependents);
}

ECode CSeekBarPreference::SetDependency(
    /* [in] */ const String& dependencyKey)
{
    return Preference::SetDependency(dependencyKey);
}

ECode CSeekBarPreference::GetDependency(
    /* [out] */ String* dependencyKey)
{
    return Preference::GetDependency(dependencyKey);
}

ECode CSeekBarPreference::OnPrepareForRemoval()
{
    return Preference::OnPrepareForRemoval();
}

ECode CSeekBarPreference::SetDefaultValue(
    /* [in] */ IInterface* defaultValue)
{
    return Preference::SetDefaultValue(defaultValue);
}

ECode CSeekBarPreference::PersistString(
    /* [in] */ const String& value,
    /* [out] */ Boolean* isPersistString)
{
    return Preference::PersistString(value, isPersistString);
}

ECode CSeekBarPreference::GetPersistedString(
    /* [in] */ const String& defaultReturnValue,
    /* [out] */ String* str)
{
    return Preference::GetPersistedString(defaultReturnValue, str);
}

ECode CSeekBarPreference::PersistStringSet(
    /* [in] */ ArrayOf<String>* values,
    /* [out] */ Boolean* isPersistStringSet)
{
    return Preference::PersistStringSet(values, isPersistStringSet);
}

ECode CSeekBarPreference::GetPersistedStringSet(
    /* [in] */ ArrayOf<String>* defaultReturnValue,
    /* [out, callee] */ ArrayOf<String>** value)
{
    return Preference::GetPersistedStringSet(defaultReturnValue, value);
}

ECode CSeekBarPreference::PersistInt32(
    /* [in] */ Int32 value,
    /* [out] */ Boolean* isPersist)
{
    return Preference::PersistInt32(value, isPersist);
}

ECode CSeekBarPreference::GetPersistedInt32(
    /* [in] */ Int32 defaultReturnValue,
    /* [out] */ Int32* value)
{
    return Preference::GetPersistedInt32(defaultReturnValue, value);
}

ECode CSeekBarPreference::PersistFloat(
    /* [in] */ Float value,
    /* [out] */ Boolean* isPersist)
{
    return Preference::PersistFloat(value, isPersist);
}

ECode CSeekBarPreference::GetPersistedFloat(
    /* [in] */ Float defaultReturnValue,
    /* [out] */ Float* value)
{
    return Preference::GetPersistedFloat(defaultReturnValue, value);
}

ECode CSeekBarPreference::PersistInt64(
    /* [in] */ Int64 value,
    /* [out] */ Boolean* isPersist)
{
    return Preference::PersistInt64(value, isPersist);
}

ECode CSeekBarPreference::GetPersistedInt64(
    /* [in] */ Int64 defaultReturnValue,
    /* [out] */ Int64* value)
{
    return Preference::GetPersistedInt64(defaultReturnValue, value);
}

ECode CSeekBarPreference::PersistBoolean(
    /* [in] */ Boolean value,
    /* [out] */ Boolean* isPersist)
{
    return Preference::PersistBoolean(value, isPersist);
}

ECode CSeekBarPreference::GetPersistedBoolean(
    /* [in] */ Boolean defaultReturnValue,
    /* [out] */ Boolean* value)
{
    return Preference::GetPersistedBoolean(defaultReturnValue, value);
}

ECode CSeekBarPreference::HasSpecifiedLayout(
    /* [out] */ Boolean* hasSpecifiedLayout)
{
    return Preference::HasSpecifiedLayout(hasSpecifiedLayout);
}

ECode CSeekBarPreference::ToString(
    /* [out] */ String* str)
{
    return Preference::ToString(str);
}

ECode CSeekBarPreference::SaveHierarchyState(
    /* [in] */ IBundle* container)
{
    return Preference::SaveHierarchyState(container);
}

ECode CSeekBarPreference::DispatchSaveInstanceState(
    /* [in] */ IBundle* container)
{
    return Preference::DispatchSaveInstanceState(container);
}

ECode CSeekBarPreference::RestoreHierarchyState(
    /* [in] */ IBundle* container)
{
    return Preference::RestoreHierarchyState(container);
}

ECode CSeekBarPreference::DispatchRestoreInstanceState(
    /* [in] */ IBundle* container)
{
    return Preference::DispatchRestoreInstanceState(container);
}

ECode CSeekBarPreference::CompareTo(
    /* [in] */ IInterface* another,
    /* [out] */ Int32* result)
{
    return Preference::CompareTo(another, result);
}

}
}
}

