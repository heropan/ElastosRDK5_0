
#include "content/CContentUris.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Content {

ECode CContentUris::ParseId(
    /* [in] */ IUri* contentUri,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(contentUri)
    VALIDATE_NOT_NULL(id)
    String last;
    FAIL_RETURN(contentUri->GetLastPathSegment(&last));
    if (last.IsNull()) {
        *id = -1;
        return NOERROR;
    }
    return StringUtils::ParseInt64(last, id);
}

ECode CContentUris::AppendId(
    /* [in] */ IUriBuilder* builder,
    /* [in] */ Int64 id)
{
    VALIDATE_NOT_NULL(builder)
    return builder->AppendEncodedPath(StringUtils::Int64ToString(id));
}

ECode CContentUris::WithAppendedId(
    /* [in] */ IUri* contentUri,
    /* [in] */ Int64 id,
    /* [out] */ IUri** newUri)
{
    VALIDATE_NOT_NULL(newUri)
    *newUri = NULL;

    VALIDATE_NOT_NULL(contentUri)
    AutoPtr<IUriBuilder> ub;
    FAIL_RETURN(contentUri->BuildUpon((IUriBuilder**)&ub));
    FAIL_RETURN(AppendId(ub, id));
    return ub->Build(newUri);
}

}
}
}

