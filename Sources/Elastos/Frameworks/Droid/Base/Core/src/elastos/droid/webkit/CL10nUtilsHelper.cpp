
#include "elastos/droid/webkit/CL10nUtilsHelper.h"
#include "elastos/droid/webkit/L10nUtils.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_INTERFACE_IMPL(CL10nUtilsHelper, Object, IL10nUtilsHelper);

CAR_SINGLETON_IMPL(CL10nUtilsHelper);

ECode CL10nUtilsHelper::SetApplicationContext(
    /* [in] */ IContext* applicationContext)
{
    L10nUtils::SetApplicationContext(applicationContext);
    return NOERROR;
}

ECode CL10nUtilsHelper::GetLocalisedString(
    /* [in] */ Int32 id,
    /* [out] */ String* localisedString)
{
    VALIDATE_NOT_NULL(localisedString);
    *localisedString = L10nUtils::GetLocalisedString(id);
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos