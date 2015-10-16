
#include "elastos/droid/app/CReceiverRestrictedContext.h"

namespace Elastos {
namespace Droid {
namespace App {

ICONTEXT_METHODS_IMPL(CReceiverRestrictedContext, ReceiverRestrictedContext)

PInterface CReceiverRestrictedContext::Probe(
    /* [in] */ REIID riid)
{
    return _CReceiverRestrictedContext::Probe(riid);
}

ECode CReceiverRestrictedContext::constructor(
    /* [in] */ IContext* context)
{
    return ReceiverRestrictedContext::Init(context);
}

ECode CReceiverRestrictedContext::GetBaseContext(
    /* [out] */ IContext** context)
{
    return ReceiverRestrictedContext::GetBaseContext(context);
}

}
}
}