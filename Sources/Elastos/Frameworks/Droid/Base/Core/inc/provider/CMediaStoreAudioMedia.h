
#ifndef __CMEDIASTOREAUDIOMEDIA_H__
#define __CMEDIASTOREAUDIOMEDIA_H__

#include "_CMediaStoreAudioMedia.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreAudioMedia)
{
public:
    /**
     * The content:// style URI for the internal storage.
     */
    CARAPI GetINTERNAL_CONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for the "primary" external storage
     * volume.
     */
    CARAPI GetEXTERNAL_CONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * Get the content:// style URI for the audio media table on the
     * given volume.
     *
     * @param volumeName the name of the volume to get the URI for
     * @return the URI to the audio media table on the given volume
     */
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [out] */ IUri** uri);

    CARAPI GetContentUriForPath(
        /* [in] */ const String& path,
        /* [out] */ IUri** uri);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__CMEDIASTOREAUDIOMEDIA_H__