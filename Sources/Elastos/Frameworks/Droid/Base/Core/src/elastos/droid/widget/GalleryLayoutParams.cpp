
#include "elastos/droid/widget/GalleryLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

GalleryLayoutParams::GalleryLayoutParams()
{}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : LayoutParams(c, attrs)
{
}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
    : LayoutParams(w, h)
{
}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ ViewGroup::LayoutParams* source)
    : LayoutParams(source)
{
}

ECode GalleryLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return LayoutParams::Init(c, attrs);
}

ECode GalleryLayoutParams::Init(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    return LayoutParams::Init(w, h);
}

ECode GalleryLayoutParams::Init(
        /* [in] */ IViewGroupLayoutParams* source)
{
    return LayoutParams::Init(source);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
