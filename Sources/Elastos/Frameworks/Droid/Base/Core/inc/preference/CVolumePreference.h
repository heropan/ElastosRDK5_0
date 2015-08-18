
#ifndef __ELASTOS_DROID_PREFERENCE_CVOLUMEPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_CVOLUMEPREFERENCE_H__

#include "_Elastos_Droid_Preference_CVolumePreference.h"
#include "SeekBarDialogPreference.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::IStringBuilder;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnMultiChoiceClickListener;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::ISharedPreferencesEditor;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * @hide
 */
CarClass(CVolumePreference), public SeekBarDialogPreference
{
    friend class CVolumePreferenceSeekBarVolumizer;

private:
    class SavedState
        : public ElRefBase
        , public PreferenceBaseSavedState
        , public IPreferenceBaseSavedState
    {
        friend class CVolumePreference;

    public:
        CAR_INTERFACE_DECL();

        SavedState(
            /* [in] */ IParcel* source);

        SavedState(
            /* [in] */ IParcelable* superState);

        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

        CARAPI GetVolumeStore(
            /* [out] */ IVolumePreferenceVolumeStore** volumeStore);

        CARAPI GetSuperState(
            /* [out] */ IParcelable** superState);

    public:
        AutoPtr<IVolumePreferenceVolumeStore> mVolumeStore;

    };

public:
    CVolumePreference();

    ~CVolumePreference();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI SetStreamType(
        /* [in] */ Int32 streamType);

    CARAPI OnBindDialogView(
        /* [in] */ IView* view);

    CARAPI OnKey(
        /* [in] */ IView* v,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnDialogClosed(
        /* [in] */ Boolean positiveResult);

    CARAPI OnActivityStop();

    CARAPI OnSampleStarting(
        /* [in] */ IVolumePreferenceSeekBarVolumizer* volumizer);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** state);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

    CARAPI OnDependencyChanged(
        /* [in] */ IPreference* dependency,
        /* [in] */ Boolean disableDependent);

    CARAPI OnGetDefaultValue(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 index,
        /* [out] */ IInterface** value);

    CARAPI SetIntent(
        /* [in] */ IIntent* intent);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI SetFragment(
        /* [in] */ const String& fragment);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI PeekExtras(
        /* [out] */ IBundle** extras);

    CARAPI SetLayoutResource(
        /* [in] */ Int32 layoutResId);

    CARAPI GetLayoutResource(
        /* [out] */ Int32* layoutResId);

    CARAPI SetWidgetLayoutResource(
        /* [in] */ Int32 widgetLayoutResId);

    CARAPI GetWidgetLayoutResource(
        /* [out] */ Int32* widgetLayoutResId);

    CARAPI GetView(
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI OnCreateView(
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI OnBindView(
        /* [in] */ IView* view);

    CARAPI SetOrder(
        /* [in] */ Int32 order);

    CARAPI GetOrder(
        /* [out] */ Int32* order);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetTitle(
        /* [in] */ Int32 titleResId);

    CARAPI GetTitleRes(
        /* [out] */ Int32* resId);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetIcon(
        /* [in] */ Int32 iconResId);

    CARAPI GetIcon(
        /* [out] */ IDrawable** drawable);

    CARAPI GetSummary(
        /* [out] */ ICharSequence** summary);

    CARAPI SetSummary(
        /* [in] */ ICharSequence* summary);

    CARAPI SetSummary(
        /* [in] */ Int32 summaryResId);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnable);

    CARAPI SetSelectable(
        /* [in] */ Boolean selectable);

    CARAPI IsSelectable(
        /* [out] */ Boolean* isSelectable);

    CARAPI SetShouldDisableView(
        /* [in] */ Boolean shouldDisableView);

    CARAPI GetShouldDisableView(
        /* [out] */ Boolean* shouldDisableView);

    CARAPI OnClick();

    CARAPI SetKey(
        /* [in] */ const String& key);

    CARAPI GetKey(
        /* [out] */ String* key);

    CARAPI RequireKey();

    CARAPI HasKey(
        /* [out] */ Boolean* hasKey);

    CARAPI IsPersistent(
        /* [out] */ Boolean* isPersistent);

    CARAPI ShouldPersist(
        /* [out] */ Boolean* shouldPersist);

    CARAPI SetPersistent(
        /* [in] */ Boolean persistent);

    CARAPI CallChangeListener(
        /* [in] */ IInterface* newValue,
        /* [out] */ Boolean* shouldSetValue);

    CARAPI SetOnPreferenceChangeListener(
        /* [in] */ IPreferenceOnPreferenceChangeListener* onPreferenceChangeListener);

    CARAPI GetOnPreferenceChangeListener(
        /* [out] */ IPreferenceOnPreferenceChangeListener** listener);

    CARAPI SetOnPreferenceClickListener(
        /* [in] */ IPreferenceOnPreferenceClickListener* onPreferenceClickListener);

    CARAPI GetOnPreferenceClickListener(
        /* [out] */ IPreferenceOnPreferenceClickListener** listener);

    CARAPI PerformClick(
        /* [in] */ IPreferenceScreen* preferenceScreen);

    CARAPI GetContext(
        /* [out] */ IContext** context);

    CARAPI GetSharedPreferences(
        /* [out] */ ISharedPreferences** preference);

    CARAPI GetEditor(
        /* [out] */ ISharedPreferencesEditor** editor);

    CARAPI ShouldCommit(
        /* [out] */ Boolean* shouldCommit);

    CARAPI NotifyChanged();

    CARAPI NotifyHierarchyChanged();

    CARAPI GetPreferenceManager(
        /* [out] */ IPreferenceManager** manager);

    CARAPI OnAttachedToHierarchy(
        /* [in] */ IPreferenceManager* preferenceManager);

    CARAPI OnAttachedToActivity();

    CARAPI FindPreferenceInHierarchy(
        /* [in] */ const String& key,
        /* [out] */ IPreference** preference);

    CARAPI NotifyDependencyChange(
        /* [in] */ Boolean disableDependents);

    CARAPI ShouldDisableDependents(
        /* [out] */ Boolean* shouldDisableDependents);

    CARAPI SetDependency(
        /* [in] */ const String& dependencyKey);

    CARAPI GetDependency(
        /* [out] */ String* dependencyKey);

    CARAPI OnPrepareForRemoval();

    CARAPI SetDefaultValue(
        /* [in] */ IInterface* defaultValue);

    CARAPI OnSetInitialValue(
        /* [in] */ Boolean restorePersistedValue,
        /* [in] */ IInterface* defaultValue);

    CARAPI PersistString(
        /* [in] */ const String& value,
        /* [out] */ Boolean* isPersistString);

    CARAPI GetPersistedString(
        /* [in] */ const String& defaultReturnValue,
        /* [out] */ String* str);

    CARAPI PersistStringSet(
        /* [in] */ ArrayOf<String>* values,
        /* [out] */ Boolean* isPersistStringSet);

    CARAPI GetPersistedStringSet(
        /* [in] */ ArrayOf<String>* defaultReturnValue,
        /* [out, callee] */ ArrayOf<String>** value);

    CARAPI PersistInt32(
        /* [in] */ Int32 value,
        /* [out] */ Boolean* isPersist);

    CARAPI GetPersistedInt32(
        /* [in] */ Int32 defaultReturnValue,
        /* [out] */ Int32* value);

    CARAPI PersistFloat(
        /* [in] */ Float value,
        /* [out] */ Boolean* isPersist);

    CARAPI GetPersistedFloat(
        /* [in] */ Float defaultReturnValue,
        /* [out] */ Float* value);

    CARAPI PersistInt64(
        /* [in] */ Int64 value,
        /* [out] */ Boolean* isPersist);

    CARAPI GetPersistedInt64(
        /* [in] */ Int64 defaultReturnValue,
        /* [out] */ Int64* value);

    CARAPI PersistBoolean(
        /* [in] */ Boolean value,
        /* [out] */ Boolean* isPersist);

    CARAPI GetPersistedBoolean(
        /* [in] */ Boolean defaultReturnValue,
        /* [out] */ Boolean* value);

    CARAPI HasSpecifiedLayout(
        /* [out] */ Boolean* hasSpecifiedLayout);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI SaveHierarchyState(
        /* [in] */ IBundle* container);

    CARAPI DispatchSaveInstanceState(
        /* [in] */ IBundle* container);

    CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* container);

    CARAPI DispatchRestoreInstanceState(
        /* [in] */ IBundle* container);

    CARAPI SetDialogTitle(
        /* [in] */ ICharSequence* dialogTitle);

    CARAPI SetDialogTitle(
        /* [in] */ Int32 dialogTitleResId);

    CARAPI GetDialogTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetDialogMessage(
        /* [in] */ ICharSequence* dialogMessage);

    CARAPI SetDialogMessage(
        /* [in] */ Int32 dialogMessageResId);

    CARAPI GetDialogMessage(
        /* [out] */ ICharSequence** message);

    CARAPI SetDialogIcon(
        /* [in] */ IDrawable* dialogIcon);

    CARAPI SetDialogIcon(
        /* [in] */ Int32 dialogIconRes);

    CARAPI GetDialogIcon(
        /* [out] */ IDrawable** icon);

    CARAPI SetPositiveButtonText(
        /* [in] */ ICharSequence* positiveButtonText);

    CARAPI SetPositiveButtonText(
        /* [in] */ Int32 positiveButtonTextResId);

    CARAPI GetPositiveButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetNegativeButtonText(
        /* [in] */ ICharSequence* negativeButtonText);

    CARAPI SetNegativeButtonText(
        /* [in] */ Int32 negativeButtonTextResId);

    CARAPI GetNegativeButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetDialogLayoutResource(
        /* [in] */ Int32 dialogLayoutResId);

    CARAPI GetDialogLayoutResource(
        /* [out] */ Int32* layoutResId);

    CARAPI OnPrepareDialogBuilder(
        /* [in] */ IAlertDialogBuilder* builder);

    CARAPI ShowDialog(
        /* [in] */ IBundle* state);

    CARAPI NeedInputMethod(
        /* [out] */ Boolean* isNeed);

    CARAPI OnCreateDialogView(
        /* [out] */ IView** view);

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

    CARAPI OnDismiss(
        /* [in] */ IDialogInterface* dialog);

    CARAPI GetDialog(
        /* [out] */ IDialog** dialog);

    CARAPI OnActivityDestroy();

    CARAPI CreateActionButtons();

private:
    /**
     * Do clean up.  This can be called multiple times!
     */
    CARAPI Cleanup();

private:
    static const String TAG;

    Int32 mStreamType;

    /** May be null if the dialog isn't visible. */
    AutoPtr<IVolumePreferenceSeekBarVolumizer> mSeekBarVolumizer;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CVOLUMEPREFERENCE_H__
