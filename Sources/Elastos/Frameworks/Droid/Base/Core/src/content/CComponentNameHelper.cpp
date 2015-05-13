
#include "content/CComponentNameHelper.h"
#include "content/CComponentName.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CComponentNameHelper::UnflattenFromString(
    /* [in] */ const String& str,
    /* [out] */ IComponentName** componentName)
{
    VALIDATE_NOT_NULL(componentName)
    return CComponentName::UnflattenFromString(str, componentName);
}

ECode CComponentNameHelper::WriteToParcel(
    /* [in] */ IComponentName* cn,
    /* [in] */ IParcel* dest)
{
    return CComponentName::WriteToParcel(cn, dest);
}

ECode CComponentNameHelper::ReadFromParcel(
    /* [in] */ IParcel* source,
    /* [out] */ IComponentName** cn)
{
    VALIDATE_NOT_NULL(cn)
    return CComponentName::ReadFromParcel(source, cn);
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
