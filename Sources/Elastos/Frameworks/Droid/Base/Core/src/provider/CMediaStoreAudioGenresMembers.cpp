
#include "provider/CMediaStoreAudioGenresMembers.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreAudioGenresMembers::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [in] */ Int64 genreId,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            "/audio/genres/" + StringUtils::Int64ToString(genreId) + "/members", uri);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
