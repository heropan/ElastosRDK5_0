
#ifndef __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCE_H__

#include "_Elastos_Droid_Preference_CMultiCheckPreference.h"
#include "elastos/droid/preference/MultiCheckPreference.h"

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * @hide
* A {@link Preference} that displays a list of entries as
* a dialog which allow the user to toggle each individually on and off.
 *
* @attr ref android.R.styleable#ListPreference_entries
* @attr ref android.R.styleable#ListPreference_entryValues
 */
CarClass(CMultiCheckPreference)
    , public MultiCheckPreference
{
public:
    CAR_OBJECT_DECL()

};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCE_H__
