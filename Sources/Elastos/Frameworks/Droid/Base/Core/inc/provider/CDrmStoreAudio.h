
#ifndef __CDRMSTOREAUDIO_H__
#define __CDRMSTOREAUDIO_H__

#include "_CDrmStoreAudio.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CDrmStoreAudio)
{
public:
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CDRMSTOREIMAGES_H__