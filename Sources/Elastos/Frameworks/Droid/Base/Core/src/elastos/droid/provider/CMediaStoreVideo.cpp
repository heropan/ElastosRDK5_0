
#include "elastos/droid/provider/CMediaStoreVideo.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreVideo::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    return cr->Query(uri, projection, String(NULL), NULL,
            IMediaStoreVideo::DEFAULT_SORT_ORDER, cursor);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
