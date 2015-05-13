
#ifndef __CMEDIASTOREAUDIOGENRES_H__
#define __CMEDIASTOREAUDIOGENRES_H__

#include "_CMediaStoreAudioGenres.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreAudioGenres)
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
     * Get the content:// style URI for the audio genres table on the
     * given volume.
     *
     * @param volumeName the name of the volume to get the URI for
     * @return the URI to the audio genres table on the given volume
     */
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [out] */ IUri** uri);

    /**
     * Get the content:// style URI for querying the genres of an audio file.
     *
     * @param volumeName the name of the volume to get the URI for
     * @param audioId the ID of the audio file for which to retrieve the genres
     * @return the URI to for querying the genres for the audio file
     * with the given the volume and audioID
     */
    CARAPI GetContentUriForAudioId(
        /* [in] */ const String& volumeName,
        /* [in] */ Int32 audioId,
        /* [out] */ IUri** uri);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__CMEDIASTOREAUDIOGENRES_H__