
#ifndef __ELASTOS_DROID_WEBKIT_CWEBSETTINGSCLASSICHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CWEBSETTINGSCLASSICHELPER_H__

#include "_Elastos_Droid_Webkit_CWebSettingsClassicHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Utility::ILocale;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebSettingsClassicHelper)
{
public:
    CARAPI GetDefaultUserAgentForLocale(
        /* [in] */ IContext* context,
        /* [in] */ ILocale* locale,
        /* [out] */ String* ua);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBSETTINGSCLASSICHELPER_H__
