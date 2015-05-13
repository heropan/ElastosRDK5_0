
#ifndef __CPREFERENCEMANAGERHELPER_H__
#define __CPREFERENCEMANAGERHELPER_H__

#include "_CPreferenceManagerHelper.h"

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

    CARAPI SetDefaultValuesEx(
        /* [in] */ IContext* context,
        /* [in] */ const String& sharedPreferencesName,
        /* [in] */ Int32 sharedPreferencesMode,
        /* [in] */ Int32 resId,
        /* [in] */ Boolean readAgain);
};

}
}
}

#endif // __CPREFERENCEMANAGERHELPER_H__
