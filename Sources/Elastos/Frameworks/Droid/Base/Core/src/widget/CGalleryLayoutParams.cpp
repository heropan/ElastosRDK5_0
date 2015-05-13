#include "ext/frameworkext.h"
#include "widget/CGalleryLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEWGROUPLP_METHODS_IMPL(CGalleryLayoutParams, GalleryLayoutParams);

ECode CGalleryLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return GalleryLayoutParams::Init(c, attrs);
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    return GalleryLayoutParams::Init(w, h);
}

ECode CGalleryLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return GalleryLayoutParams::Init(source);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
