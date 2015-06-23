#include "util/CPropertyHelper.h"
#include "util/ReflectiveProperty.h"
#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace Utility {

CPropertyHelper::CPropertyHelper()
{}

ECode CPropertyHelper::constructor()
{
   return NOERROR;
}

ECode CPropertyHelper::Of(
    /* [in] */ const ClassID& clsId,
    /* [in] */ CarDataType dataType,
    /* [in] */ const String& name,
    /* [out] */ IProperty** property)
{
    AutoPtr<IReflectiveProperty> pTemp = new ReflectiveProperty(clsId, dataType, name);
    *property = pTemp;
    REFCOUNT_ADD(*property);
    return NOERROR;
}

} // namespace Utility
} // namepsace Droid
} // namespace Elastos
