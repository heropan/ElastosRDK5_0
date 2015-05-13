
#include "content/CContentResolverOpenResourceIdResult.h"
#include <ext/frameworkdef.h>

namespace Elastos {
namespace Droid {
namespace Content {

CContentResolverOpenResourceIdResult::CContentResolverOpenResourceIdResult()
    : mRes(NULL)
    , mId(0)
{}

CContentResolverOpenResourceIdResult::~CContentResolverOpenResourceIdResult()
{}

ECode CContentResolverOpenResourceIdResult::GetResources(
    /* [out] */ IResources** resources)
{
    VALIDATE_NOT_NULL(resources)
    *resources = mRes;
    INTERFACE_ADDREF(*resources)
    return NOERROR;
}

ECode CContentResolverOpenResourceIdResult::SetResources(
    /* [in] */ IResources* resources)
{
    mRes = resources;
    return NOERROR;
}

ECode CContentResolverOpenResourceIdResult::GetResourceId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id)
    *id = mId;
    return NOERROR;
}

ECode CContentResolverOpenResourceIdResult::SetResourceId(
    /* [in] */ Int32 id)
{
    mId = id;
    return NOERROR;
}

}
}
}

