
#ifndef __ELASTOS_DROID_PROVIDER_CDRMSTOREIMAGES_H__
#define __ELASTOS_DROID_PROVIDER_CDRMSTOREIMAGES_H__

#include "_Elastos_Droid_Provider_CDrmStoreImages.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CDrmStoreImages)
{
public:
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__ELASTOS_DROID_PROVIDER_CDRMSTOREIMAGES_H__
