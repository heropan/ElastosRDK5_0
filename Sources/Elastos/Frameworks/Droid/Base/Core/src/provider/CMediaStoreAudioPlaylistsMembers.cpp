
#include "provider/CMediaStoreAudioPlaylistsMembers.h"
#include "ext/frameworkext.h"
#include <elastos/StringUtils.h>
#include "net/CUriHelper.h"
#include "content/CContentValues.h"

using Elastos::Core::CInteger32;
using Elastos::Core::StringUtils;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Net::IUriBuilder;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreAudioPlaylistsMembers::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [in] */ Int64 playlistId,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            + "/audio/playlists/" + playlistId + "/members", uri);
}

ECode CMediaStoreAudioPlaylistsMembers::MoveItem(
    /* [in] */ IContentResolver* res,
    /* [in] */ Int64 playlistId,
    /* [in] */ Int32 from,
    /* [in] */ Int32 to,
    /* [out] */ Boolean* result)
{
    AutoPtr<IUri> uri;
    /*AutoPtr<IMediaStoreAudioPlaylistsMembers> members;
    CMediaStoreAudioPlaylistsMembers::AcquireSingleton((IMediaStoreAudioPlaylistsMembers**)&members);
    members->*/
    GetContentUri(String("external"), playlistId, (IUri**)&uri);

    AutoPtr<IUriBuilder> builder;
    uri->BuildUpon((IUriBuilder**)&builder);
    builder->AppendEncodedPath(StringUtils::Int32ToString(from));
    builder->AppendQueryParameter(String("move"), String("true"));
    uri = NULL;
    builder->Build((IUri**)&uri);

    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    AutoPtr<IInteger32> iTo;
    CInteger32::New(to, (IInteger32**)&iTo);
    values->PutInt32(IMediaStoreAudioPlaylistsMembers::PLAY_ORDER, iTo);
    Int32 rows;
    res->Update(uri, values, String(NULL), NULL, &rows);
    *result = rows != 0;
    return NOERROR;
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
