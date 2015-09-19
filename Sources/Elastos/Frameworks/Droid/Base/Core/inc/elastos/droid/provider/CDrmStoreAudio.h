
#ifndef __ELASTOS_DROID_PROVIDER_CDRMSTOREAUDIO_H__
#define __ELASTOS_DROID_PROVIDER_CDRMSTOREAUDIO_H__

#include "_Elastos_Droid_Provider_CDrmStoreAudio.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CDrmStoreAudio)
{
public:
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CDRMSTOREIMAGES_H__
