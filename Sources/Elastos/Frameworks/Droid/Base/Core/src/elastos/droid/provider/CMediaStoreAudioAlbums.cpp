
#include "elastos/droid/provider/CMediaStoreAudioAlbums.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/CUriHelper.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreAudioAlbums::GetINTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("internal"), uri);
}

ECode CMediaStoreAudioAlbums::GetEXTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("external"), uri);
}

ECode CMediaStoreAudioAlbums::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    StringBuilder builder;
    builder += IMediaStore::CONTENT_AUTHORITY_SLASH;
    builder += volumeName;
    builder += "/audio/albums";
    String str = builder.ToString();
    return helper->Parse(str, uri);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
