
#ifndef __ELASTOS_DROID_PREFERENCE_CRINGTONEPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_CRINGTONEPREFERENCE_H__

#include "_CRingtonePreference.h"
#include "Preference.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CRingtonePreference)
    , public Preference
    , public IRingtonePreference
    , public IPreferenceManagerOnActivityResultListener
{
public:
    CRingtonePreference();

    CAR_INTERFACE_DECL()

    CARAPI GetRingtoneType(
        /* [out] */ Int32* type);

    CARAPI SetRingtoneType(
        /* [in] */ Int32 type);

    CARAPI GetShowDefault(
        /* [out] */ Boolean* showDefault);

    CARAPI SetShowDefault(
        /* [in] */ Boolean showDefault);

    CARAPI GetShowSilent(
        /* [out] */ Boolean* showSilent);

    CARAPI SetShowSilent(
        /* [in] */ Boolean showSilent);

    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data,
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context);

protected:
    CARAPI OnClick();

    CARAPI OnPrepareRingtonePickerIntent(
        /* [in] */ IIntent* ringtonePickerIntent);

    CARAPI OnSaveRingtone(
        /* [in] */ IUri* ringtoneUri);

    CARAPI OnRestoreRingtone(
        /* [out] */ IUri** uri);

    CARAPI OnGetDefaultValue(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 index,
        /* [out] */ IInterface** value);

    CARAPI OnSetInitialValue(
        /* [in] */ Boolean restorePersistedValue,
        /* [in] */ IInterface* defaultValue);

    CARAPI OnAttachedToHierarchy(
        /* [in] */ IPreferenceManager* preferenceManager);

private:
    static const String TAG;
    // TODO: Add your private member variables here.
    Int32 mRingtoneType;
    Boolean mShowDefault;
    Boolean mShowSilent;

    Int32 mRequestCode;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CRINGTONEPREFERENCE_H__
