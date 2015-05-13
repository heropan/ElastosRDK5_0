
#ifndef __CMEDIASTOREAUDIOARTISTSALBUMS_H__
#define __CMEDIASTOREAUDIOARTISTSALBUMS_H__

#include "_CMediaStoreAudioArtistsAlbums.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreAudioArtistsAlbums)
{
public:
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [in] */ Int64 artistId,
        /* [out] */ IUri** uri);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__CMEDIASTOREAUDIOARTISTSALBUMS_H__