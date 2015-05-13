
#include "provider/CMediaStoreAudioMedia.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"
#include "os/Environment.h"

using Elastos::IO::IFile;
using Elastos::Droid::Os::Environment;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreAudioMedia::GetINTERNALCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("internal"), uri);
}

ECode CMediaStoreAudioMedia::GetEXTERNALCONTENTURI(
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
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            "/audio/media", uri);
}

ECode CMediaStoreAudioMedia::GetContentUriForPath(
    /* [in] */ const String& path,
    /* [out] */ IUri** uri)
{
    String str;
    AutoPtr<IFile> file = Environment::GetExternalStorageDirectory();
    file->GetPath(&str);

    if(str.StartWith(path)) {
        return GetINTERNALCONTENTURI(uri);
    } else {
        return GetEXTERNALCONTENTURI(uri);
    }
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
