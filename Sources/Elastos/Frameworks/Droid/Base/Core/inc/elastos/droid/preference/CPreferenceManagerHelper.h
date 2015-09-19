
#ifndef __ELASTOS_DROID_PREFERENCE_CPREFERENCEMANAGERHELPER_H__
#define __ELASTOS_DROID_PREFERENCE_CPREFERENCEMANAGERHELPER_H__

#include "_Elastos_Droid_Preference_CPreferenceManagerHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::ISharedPreferences;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CPreferenceManagerHelper)
{
public:
    CARAPI GetDefaultSharedPreferences(
        /* [in] */ IContext* context,
        /* [out] */ ISharedPreferences** sp);

    CARAPI SetDefaultValues(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resId,
        /* [in] */ Boolean readAgain);

    CARAPI SetDefaultValues(
        /* [in] */ IContext* context,
        /* [in] */ const String& sharedPreferencesName,
        /* [in] */ Int32 sharedPreferencesMode,
        /* [in] */ Int32 resId,
        /* [in] */ Boolean readAgain);
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CPREFERENCEMANAGERHELPER_H__
