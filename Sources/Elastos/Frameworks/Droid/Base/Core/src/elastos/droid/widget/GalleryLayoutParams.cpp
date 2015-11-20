
#include "elastos/droid/widget/GalleryLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

GalleryLayoutParams::GalleryLayoutParams()
{}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupLayoutParams(c, attrs)
{
}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
    : ViewGroupLayoutParams(w, h)
{
}

GalleryLayoutParams::GalleryLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
{
}

ECode GalleryLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupLayoutParams::Init(c, attrs);
}

ECode GalleryLayoutParams::Init(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    return ViewGroupLayoutParams::Init(w, h);
}

ECode GalleryLayoutParams::Init(
        /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
