
#include "content/CMutableContextWrapper.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Content {

ICONTEXT_METHODS_IMPL(CMutableContextWrapper, ContextWrapper)

PInterface CMutableContextWrapper::Probe(
    /* [in] */ REIID riid)
{
    return _CMutableContextWrapper::Probe(riid);
}

ECode CMutableContextWrapper::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ContextWrapper::GetBaseContext(context);
}

ECode CMutableContextWrapper::SetBaseContext(
    /* [in] */ IContext* base)
{
    return ContextWrapper::Init(base);
}

ECode CMutableContextWrapper::constructor(
    /* [in] */ IContext* base)
{
    return ContextWrapper::Init(base);
}

}
}
}

