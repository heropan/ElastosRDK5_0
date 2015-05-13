
#include "content/CContextWrapper.h"

namespace Elastos {
namespace Droid {
namespace Content {

ICONTEXT_METHODS_IMPL(CContextWrapper, ContextWrapper)

PInterface CContextWrapper::Probe(
    /* [in] */ REIID riid)
{
    return _CContextWrapper::Probe(riid);
}

ECode CContextWrapper::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextWrapper::GetBaseContext(context);
}

ECode CContextWrapper::constructor(
    /* [in] */ IContext* context)
{
    return ContextWrapper::Init(context);
}

}
}
}