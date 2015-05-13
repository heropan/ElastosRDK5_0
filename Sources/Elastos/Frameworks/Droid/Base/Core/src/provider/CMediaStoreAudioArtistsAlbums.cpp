
#include "provider/CMediaStoreAudioArtistsAlbums.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"

using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreAudioArtistsAlbums::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [in] */ Int64 artistId,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            + "/audio/artists/" + artistId + "/albums", uri);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
