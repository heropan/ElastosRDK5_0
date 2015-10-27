
#include "elastos/droid/provider/CMediaStoreAudioArtists.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/CUriHelper.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreAudioArtists::GetINTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("internal"), uri);
}

ECode CMediaStoreAudioArtists::GetEXTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("external"), uri);
}

ECode CMediaStoreAudioArtists::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    StringBuilder builder;
    builder += IMediaStore::CONTENT_AUTHORITY_SLASH;
    builder += volumeName;
    builder += "/audio/artists";
    String str = builder.ToString();
    return helper->Parse(str, uri);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
