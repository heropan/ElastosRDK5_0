
#ifndef __CWEBSETTINGSCLASSICHELPER_H__
#define __CWEBSETTINGSCLASSICHELPER_H__

#include "_CWebSettingsClassicHelper.h"

using Elastos::Droid::Content::IContext;
using Libcore::ICU::ILocale;

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

#endif // __CWEBSETTINGSCLASSICHELPER_H__
