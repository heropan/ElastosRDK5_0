#ifndef __ELASTOS_DROID_LOCATION_CLOCATIONPROVIDERHELPER_H__
#define __ELASTOS_DROID_LOCATION_CLOCATIONPROVIDERHELPER_H__
#include "_CLocationProviderHelper.h"

namespace Elastos {
namespace Droid {
namespace Location {

CarClass(CLocationProviderHelper)
{
public:
    CARAPI PropertiesMeetCriteria(
        /* [in] */ const String& name,
        /* [in] */ IProviderProperties* properties,
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* rst);
};


}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif
