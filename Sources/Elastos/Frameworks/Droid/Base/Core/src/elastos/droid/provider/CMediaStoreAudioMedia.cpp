
#include "elastos/droid/provider/CMediaStoreAudioMedia.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/CUriHelper.h"
#include "elastos/droid/os/Environment.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::IO::IFile;
using Elastos::Droid::Os::Environment;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreAudioMedia::GetINTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("internal"), uri);
}

ECode CMediaStoreAudioMedia::GetEXTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("external"), uri);
}

ECode CMediaStoreAudioMedia::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    StringBuilder builder;
    builder += IMediaStore::CONTENT_AUTHORITY_SLASH;
    builder += volumeName;
    builder += "/audio/media";
    String str = builder.ToString();
    return helper->Parse(str, uri);
}

ECode CMediaStoreAudioMedia::GetContentUriForPath(
    /* [in] */ const String& path,
    /* [out] */ IUri** uri)
{
    String str;
    AutoPtr<IFile> file = Environment::GetExternalStorageDirectory();
    file->GetPath(&str);

    if(str.StartWith(path)) {
        return GetINTERNAL_CONTENT_URI(uri);
    } else {
        return GetEXTERNAL_CONTENT_URI(uri);
    }
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
