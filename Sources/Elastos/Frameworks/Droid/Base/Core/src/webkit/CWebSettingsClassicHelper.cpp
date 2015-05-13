
#include "webkit/CWebSettingsClassicHelper.h"
#include "webkit/WebSettingsClassic.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebSettingsClassicHelper::GetDefaultUserAgentForLocale(
    /* [in] */ IContext* context,
    /* [in] */ ILocale* locale,
    /* [out] */ String* ua)
{
    VALIDATE_NOT_NULL(ua);
    *ua = WebSettingsClassic::GetDefaultUserAgentForLocale(context, locale);
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
