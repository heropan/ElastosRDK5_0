#include "widget/internal/CSizeAdaptiveLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEWGROUPLP_METHODS_IMPL(CSizeAdaptiveLayoutLayoutParams, SizeAdaptiveLayoutLayoutParams);

ECode CSizeAdaptiveLayoutLayoutParams::constructor()
{
    return SizeAdaptiveLayoutLayoutParams::Init();
}

ECode CSizeAdaptiveLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return SizeAdaptiveLayoutLayoutParams::Init(width, height);
}

ECode CSizeAdaptiveLayoutLayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minHeight,
    /* [in] */ Int32 maxHeight)
{
    return SizeAdaptiveLayoutLayoutParams::Init(width, height, minHeight, maxHeight);
}

ECode CSizeAdaptiveLayoutLayoutParams::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return SizeAdaptiveLayoutLayoutParams::Init(ctx, attrs);
}

ECode CSizeAdaptiveLayoutLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return SizeAdaptiveLayoutLayoutParams::Init();
}

ECode CSizeAdaptiveLayoutLayoutParams::SetMinHeight(
    /* [in] */ Int32 mh)
{
    mMinHeight = mh;
    return NOERROR;
}

ECode CSizeAdaptiveLayoutLayoutParams::SetMaxHeight(
    /* [in] */ Int32 mh)
{
    mMaxHeight = mh;
    return NOERROR;
}

ECode CSizeAdaptiveLayoutLayoutParams::GetMinHeight(
    /* [out] */ Int32* mh)
{
    VALIDATE_NOT_NULL(mh);
    *mh = mMinHeight;
    return NOERROR;
}

ECode CSizeAdaptiveLayoutLayoutParams::GetMaxHeight(
    /* [out] */ Int32* mh)
{
    VALIDATE_NOT_NULL(mh);
    *mh = mMaxHeight;
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
