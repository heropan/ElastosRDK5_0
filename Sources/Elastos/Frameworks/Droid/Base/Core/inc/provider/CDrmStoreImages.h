
#ifndef __CDRMSTOREIMAGES_H__
#define __CDRMSTOREIMAGES_H__

#include "_CDrmStoreImages.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CDrmStoreImages)
{
public:
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CDRMSTOREIMAGES_H__