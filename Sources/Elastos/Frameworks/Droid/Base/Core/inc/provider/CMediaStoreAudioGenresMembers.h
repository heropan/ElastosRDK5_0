
#ifndef __CMEDIASTOREAUDIOGENRESMEMBERS_H__
#define __CMEDIASTOREAUDIOGENRESMEMBERS_H__

#include "_CMediaStoreAudioGenresMembers.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreAudioGenresMembers)
{
public:
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [in] */ Int64 genreId,
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CMEDIASTOREAUDIOGENRESMEMBERS_H__