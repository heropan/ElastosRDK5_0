#include "CTaskStackBuilderHelper.h"
#include "elastos/droid/app/CTaskStackBuilder.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CTaskStackBuilderHelper::Create(
    /* [in] */ IContext* context,
    /* [out] */ ITaskStackBuilder** builder)
{
    VALIDATE_NOT_NULL(builder);
    AutoPtr<CTaskStackBuilder> cb;
    CTaskStackBuilder::NewByFriend((CTaskStackBuilder**)&cb);
    cb->SetContext(context);
    *builder = (ITaskStackBuilder*)cb.Get();
    REFCOUNT_ADD(*builder);
    return NOERROR;
}

}
}
}
