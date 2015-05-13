
#ifndef __CMEDIASTOREAUDIOARTISTS_H__
#define __CMEDIASTOREAUDIOARTISTS_H__

#include "_CMediaStoreAudioArtists.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreAudioArtists)
{
public:
    /**
     * The content:// style URI for the internal storage.
     */
    CARAPI GetINTERNALCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for the "primary" external storage
     * volume.
     */
    CARAPI GetEXTERNALCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * Get the content:// style URI for the artists table on the
     * given volume.
     *
     * @param volumeName the name of the volume to get the URI for
     * @return the URI to the audio artists table on the given volume
     */
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [out] */ IUri** uri);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__CMEDIASTOREAUDIOARTISTS_H__