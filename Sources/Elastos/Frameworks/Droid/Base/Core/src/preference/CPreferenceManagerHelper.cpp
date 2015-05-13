
#include "CPreferenceManagerHelper.h"
#include "CPreferenceManager.h"

namespace Elastos {
namespace Droid {
namespace Preference {

ECode CPreferenceManagerHelper::GetDefaultSharedPreferences(
    /* [in] */ IContext* context,
    /* [out] */ ISharedPreferences** sp)
{
    VALIDATE_NOT_NULL(sp)
    AutoPtr<ISharedPreferences> temp = CPreferenceManager::GetDefaultSharedPreferences(context);
    *sp = temp;
    INTERFACE_ADDREF(*sp)
    return NOERROR;
}

ECode CPreferenceManagerHelper::SetDefaultValues(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Boolean readAgain)
{
    CPreferenceManager::SetDefaultValues(context, resId, readAgain);
    return NOERROR;
}

ECode CPreferenceManagerHelper::SetDefaultValuesEx(
    /* [in] */ IContext* context,
    /* [in] */ const String& sharedPreferencesName,
    /* [in] */ Int32 sharedPreferencesMode,
    /* [in] */ Int32 resId,
    /* [in] */ Boolean readAgain)
{
    CPreferenceManager::SetDefaultValues(context, sharedPreferencesName, sharedPreferencesMode, resId, readAgain);
    return NOERROR;
}

}
}
}
