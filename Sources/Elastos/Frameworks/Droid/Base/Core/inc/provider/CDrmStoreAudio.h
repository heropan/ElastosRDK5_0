
#ifndef __CDRMSTOREAUDIO_H__
#define __CDRMSTOREAUDIO_H__

#include "_CDrmStoreAudio.h"

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