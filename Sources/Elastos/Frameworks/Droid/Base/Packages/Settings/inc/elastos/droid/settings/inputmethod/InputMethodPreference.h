#ifndef __ELASTOS_DROID_SETTINGS_INPUTMETHOD_INPUTMETHODPREFERENCE_H__
#define __ELASTOS_DROID_SETTINGS_INPUTMETHOD_INPUTMETHODPREFERENCE_H__

#include "Elastos.Droid.App.h"
#include "Elastos.Droid.View.h"
#include "elastos/droid/preference/SwitchPreference.h"
#include "elastos/droid/settings/inputmethod/InputMethodSettingValuesWrapper.h"
#include "_Settings.h"

using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Preference::IPreference;
using Elastos::Droid::Preference::IPreferenceOnPreferenceChangeListener;
using Elastos::Droid::Preference::IPreferenceOnPreferenceClickListener;
using Elastos::Droid::Preference::SwitchPreference;
using Elastos::Droid::View::InputMethod::IInputMethodInfo;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Text::ICollator;

namespace Elastos {
namespace Droid {
namespace Settings {
namespace Inputmethod {

/**
 * Input method preference.
 *
 * This preference represents an IME. It is used for two purposes. 1) An instance with a switch
 * is used to enable or disable the IME. 2) An instance without a switch is used to invoke the
 * setting activity of the IME.
 */
class InputMethodPreference
    : public SwitchPreference
    , public IInputMethodPreference
    , public IPreferenceOnPreferenceClickListener
    , public IPreferenceOnPreferenceChangeListener
{
private:
    class DialogInterfaceOnClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        DialogInterfaceOnClickListener(
            /* [in] */ InputMethodPreference* host,
            /* [in] */ Int32 id);

        ~DialogInterfaceOnClickListener();

        //@Override
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        InputMethodPreference* mHost;
        Int32 mId;
    };

public:
    CAR_INTERFACE_DECL();

    /**
     * A preference entry of an input method.
     *
     * @param context The Context this is associated with.
     * @param imi The {@link InputMethodInfo} of this preference.
     * @param isImeEnabler TRUE if this preference is the IME enabler that has enable/disable
     *     switches for all available IMEs, not the list of enabled IMEs.
     * @param isAllowedByOrganization FALSE if the IME has been disabled by a device or profile
           owner.
     * @param onSaveListener The listener called when this preference has been changed and needs
     *     to save the state to shared preference.
     */
    InputMethodPreference(
        /* [in] */ IContext* context,
        /* [in] */ IInputMethodInfo* imi,
        /* [in] */ Boolean isImeEnabler,
        /* [in] */ Boolean isAllowedByOrganization,
        /* [in] */ IOnSavePreferenceListener* onSaveListener);

    ~InputMethodPreference();

    CARAPI_(AutoPtr<IInputMethodInfo>) GetInputMethodInfo();

    //@Override
    CARAPI OnPreferenceChange(
        /* [in] */ IPreference* preference,
        /* [in] */ IInterface* newValue,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI OnPreferenceClick(
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

    CARAPI_(void) UpdatePreferenceViews();

    CARAPI_(Int32) CompareTo(
        /* [in] */ InputMethodPreference* rhs,
        /* [in] */ ICollator* collator);

private:
    CARAPI_(Boolean) IsImeEnabler();

    CARAPI_(AutoPtr<IInputMethodManager>) GetInputMethodManager();

    CARAPI_(String) GetSummaryString();

    CARAPI_(void) ShowSecurityWarnDialog(
        /* [in] */ IInputMethodInfo* imi);

private:
    static const String TAG;
    static const String EMPTY_TEXT;

    AutoPtr<IInputMethodInfo> mImi;
    Boolean mHasPriorityInSorting;
    AutoPtr<IOnSavePreferenceListener> mOnSaveListener;
    AutoPtr<InputMethodSettingValuesWrapper> mInputMethodSettingValues;
    Boolean mIsAllowedByOrganization;

    AutoPtr<IAlertDialog> mDialog;
};

} // namespace Inputmethod
} // namespace Settings
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SETTINGS_INPUTMETHOD_INPUTMETHODPREFERENCE_H__
