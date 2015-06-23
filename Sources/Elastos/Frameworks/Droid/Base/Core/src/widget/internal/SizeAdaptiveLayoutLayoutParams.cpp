

#include "widget/internal/SizeAdaptiveLayoutLayoutParams.h"
#include <elastos/core/StringBuilder.h>
#include "view/View.h"
#include "R.h"
#include "widget/internal/SizeAdaptiveLayout.h"

using Elastos::Core::StringBuilder;
using Elastos::Droid::R;
using Elastos::Droid::View::View;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

SizeAdaptiveLayoutLayoutParams::SizeAdaptiveLayoutLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroupLayoutParams(c, attrs)
{
    InitImpl(c, attrs);
}

SizeAdaptiveLayoutLayoutParams::SizeAdaptiveLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minHeight,
    /* [in] */ Int32 maxHeight)
    : ViewGroupLayoutParams(width, height)
{
    InitImpl(width, height, minHeight, maxHeight);
}

SizeAdaptiveLayoutLayoutParams::SizeAdaptiveLayoutLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupLayoutParams(width, height)
{
    InitImpl(width, height);
}

SizeAdaptiveLayoutLayoutParams::SizeAdaptiveLayoutLayoutParams()
    : ViewGroupLayoutParams(
        View::View::MeasureSpec::UNSPECIFIED,
        View::View::MeasureSpec::UNSPECIFIED)
{
    InitImpl();
}

SizeAdaptiveLayoutLayoutParams::SizeAdaptiveLayoutLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
    : ViewGroupLayoutParams((ViewGroupLayoutParams*)p)
{
    InitImpl(p);
}

ECode SizeAdaptiveLayoutLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroupLayoutParams::Init(c, attrs);
    return InitImpl(c, attrs);
}

ECode SizeAdaptiveLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minHeight,
    /* [in] */ Int32 maxHeight)
{
    ViewGroupLayoutParams::Init(width, height);
    return InitImpl(width, height, minHeight, maxHeight);
}

ECode SizeAdaptiveLayoutLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    ViewGroupLayoutParams::Init(width, height);
    return InitImpl(width, height);
}

ECode SizeAdaptiveLayoutLayoutParams::Init()
{
    ViewGroupLayoutParams::Init(
        View::View::MeasureSpec::UNSPECIFIED,
        View::View::MeasureSpec::UNSPECIFIED);
    return InitImpl();
}

ECode SizeAdaptiveLayoutLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* p)
{
    ViewGroupLayoutParams::Init(p);
    return InitImpl(p);
}

ECode SizeAdaptiveLayoutLayoutParams::InitImpl(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    // if (DEBUG) {
    //     Log.d(TAG, "construct layout from attrs");
    //     Int32 count;
    //     attrs->GetAttributeCount(&count);

    //     for (Int32 i = 0; i < count; i++) {
    //         Log.d(TAG, " " + attrs.getAttributeName(i) + " = " +
    //               attrs.getAttributeValue(i));
    //     }
    // }

    Int32 size = ARRAY_SIZE(R::styleable::SizeAdaptiveLayout_Layout);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(size);
    attrIds->Copy(R::styleable::SizeAdaptiveLayout_Layout, size);

    AutoPtr<ITypedArray> a;
    FAIL_RETURN(c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a));
    FAIL_RETURN(a->GetDimensionPixelSize(
            SizeAdaptiveLayout::MIN_VALID_HEIGHT, 0, &mMinHeight));
    FAIL_RETURN(a->GetLayoutDimension(
            SizeAdaptiveLayout::MAX_VALID_HEIGHT,
            ISizeAdaptiveLayoutLayoutParams::UNBOUNDED, &mMaxHeight));

    a->Recycle();
    return NOERROR;
}

ECode SizeAdaptiveLayoutLayoutParams::InitImpl(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minHeight,
    /* [in] */ Int32 maxHeight)
{
    mMinHeight = minHeight;
    mMaxHeight = maxHeight;
    return NOERROR;
}

ECode SizeAdaptiveLayoutLayoutParams::InitImpl(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return InitImpl(width, height,
        ISizeAdaptiveLayoutLayoutParams::UNBOUNDED,
        ISizeAdaptiveLayoutLayoutParams::UNBOUNDED);
}

ECode SizeAdaptiveLayoutLayoutParams::InitImpl()
{
    return InitImpl(View::View::MeasureSpec::UNSPECIFIED, View::View::MeasureSpec::UNSPECIFIED);
}

ECode SizeAdaptiveLayoutLayoutParams::InitImpl(
    /* [in] */ IViewGroupLayoutParams* p)
{
    mMinHeight = ISizeAdaptiveLayoutLayoutParams::UNBOUNDED;
    mMaxHeight = ISizeAdaptiveLayoutLayoutParams::UNBOUNDED;
    return NOERROR;
}

String SizeAdaptiveLayoutLayoutParams::Debug(
    /* [in] */ const String& output)
{
    StringBuilder sb(output);
    sb += "SizeAdaptiveLayout.LayoutParams={";
    sb += ", max=";
    sb += mMaxHeight;
    sb += ", min=";
    sb += mMinHeight;
    sb += "}";
    return sb.ToString();
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

