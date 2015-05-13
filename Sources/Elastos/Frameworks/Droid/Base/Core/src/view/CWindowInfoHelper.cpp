#include "view/CWindowInfoHelper.h"
#include "view/CWindowInfo.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CWindowInfoHelper::Obtain(
    /* [out] */ IWindowInfo** info)
{
    return CWindowInfo::Obtain(info);
}

ECode CWindowInfoHelper::ObtainEx(
    /* [in] */ IWindowInfo *other,
    /* [out] */ IWindowInfo** info)
{
    return CWindowInfo::ObtainEx(other, info);
}

} // namespace View
} // namespace Droid
} // namespace Elastos