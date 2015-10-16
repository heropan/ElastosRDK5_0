
#include "elastos/droid/app/CInstrumentationHelper.h"
#include "elastos/droid/app/CInstrumentation.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CInstrumentationHelper::NewApplication(
    /* [in] */ IClassInfo* clazz,
    /* [in] */ IContext* context,
    /* [out] */ IApplication** app)
{
    return CInstrumentation::NewApplication(clazz, context, app);
}

ECode CInstrumentationHelper::NewApplication(
    /* [in] */ const ClassID& clsid,
    /* [in] */ IContext* context,
    /* [out] */ IApplication** app)
{
    return CInstrumentation::NewApplication(clsid, context, app);
}

ECode CInstrumentationHelper::CheckStartActivityResult(
    /* [in] */ Int32 res,
    /* [in] */ IIntent* intent)
{
    return CInstrumentation::CheckStartActivityResult(res, intent);
}

} // namespace App
} // namespace Droid
} // namespace Elastos

