#include "elastos/droid/location/CLocationProviderHelper.h"
#include "elastos/droid/location/CLocationProvider.h"

namespace Elastos {
namespace Droid {
namespace Location {

ECode CLocationProviderHelper::PropertiesMeetCriteria(
        /* [in] */ const String& name,
        /* [in] */ IProviderProperties* properties,
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* rst)
{
    *rst = CLocationProvider::PropertiesMeetCriteria(name, properties, criteria);
    return NOERROR;
}

} // namespace Location
} // namepsace Droid
} // namespace Elastos
