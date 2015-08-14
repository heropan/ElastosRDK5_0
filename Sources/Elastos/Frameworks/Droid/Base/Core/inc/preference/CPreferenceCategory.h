
#ifndef __ELASTOS_DROID_PREFERENCE_CPREFERENCECATEGORY_H__
#define __ELASTOS_DROID_PREFERENCE_CPREFERENCECATEGORY_H__

#include "_CPreferenceCategory.h"
#include "PreferenceGroup.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CPreferenceCategory)
    , public PreferenceGroup
    , public IPreferenceCategory
{
public:
    CAR_INTERFACE_DECL()

    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnabled);

    CARAPI SetOrderingAsAdded(
        /* [in] */ Boolean orderingAsAdded);

    CARAPI IsOrderingAsAdded(
        /* [out] */ Boolean* added);

    CARAPI GetPreferenceCount(
        /* [out] */ Int32* count);

    CARAPI GetPreference(
        /* [in] */ Int32 index,
        /* [out] */ IPreference** preference);

    CARAPI AddPreference(
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

    CARAPI RemovePreference(
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

    CARAPI RemoveAll();

    CARAPI FindPreference(
        /* [in] */ ICharSequence* key,
        /* [out] */ IPreference** preference);

    CARAPI IsOnSameScreenAsChildren(
        /* [out] */ Boolean* isOnSameScreenAsChildren);

    CARAPI SortPreferences();

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
    CARAPI OnPrepareAddPreference(
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

private:
    const static String TAG;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CPREFERENCECATEGORY_H__
