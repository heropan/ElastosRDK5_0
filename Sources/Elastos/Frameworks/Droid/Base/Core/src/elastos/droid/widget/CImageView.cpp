#include "ext/frameworkext.h"
#include "widget/CImageView.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos{
namespace Droid{
namespace Widget{


IVIEW_METHODS_IMPL(CImageView, ImageView)
IDRAWABLECALLBACK_METHODS_IMPL(CImageView, ImageView)
IKEYEVENTCALLBACK_METHODS_IMPL(CImageView, ImageView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CImageView, ImageView)
IIMAGEVIEW_METHODS_IMPL(CImageView, ImageView)

PInterface CImageView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CImageView::Probe(riid);
}

ECode CImageView::constructor(
    /* [in] */ IContext* context)
{
    return ImageView::Init(context);
}

ECode CImageView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ImageView::Init(context, attrs);
}

ECode CImageView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ImageView::Init(context, attrs, defStyle);
}

ECode CImageView::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return ImageView::SetAlpha(alpha);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
