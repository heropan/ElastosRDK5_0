
#include "ext/frameworkext.h"
#include "graphics/CInsetsHelper.h"
#include "graphics/Insets.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CInsetsHelper::Of(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ IInsets** insets)
{
    VALIDATE_NOT_NULL(insets);

    AutoPtr<Insets> o = Insets::Of(left, top, right, bottom);
    *insets = (IInsets*)o.Get();
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CInsetsHelper::Of(
    /* [in] */ IRect* r,
    /* [out] */ IInsets** insets)
{
    VALIDATE_NOT_NULL(insets);

    AutoPtr<Insets> o = Insets::Of(r);
    *insets = (IInsets*)o.Get();
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
