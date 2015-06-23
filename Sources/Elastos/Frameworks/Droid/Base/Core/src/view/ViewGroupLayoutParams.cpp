
#include "view/ViewGroupLayoutParams.h"
#include "ext/frameworkdef.h"
#include "R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
namespace Elastos {
namespace Droid {
namespace View {

ViewGroupLayoutParams::~ViewGroupLayoutParams()
{
}

ViewGroupLayoutParams::ViewGroupLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : mWidth(0)
    , mHeight(0)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

ViewGroupLayoutParams::ViewGroupLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : mWidth(width)
    , mHeight(height)
{}

ViewGroupLayoutParams::ViewGroupLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : mWidth(source->mWidth)
    , mHeight(source->mHeight)
{}

ViewGroupLayoutParams::ViewGroupLayoutParams()
    : mWidth(0)
    , mHeight(0)
{}

ECode ViewGroupLayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 widthAttr,
    /* [in] */ Int32 heightAttr)
{
    FAIL_RETURN(a->GetLayoutDimension(widthAttr, String("layout_width"), &mWidth));
    return a->GetLayoutDimension(heightAttr, String("layout_height"), &mHeight);
}

ECode ViewGroupLayoutParams::SizeToString(
    /* [in] */ Int32 size,
    /* [out] */ String* des)
{
    VALIDATE_NOT_NULL(des);

    if (size == IViewGroupLayoutParams::WRAP_CONTENT) {
        *des = String("wrap-content");
        return NOERROR;
    }
    if (size == IViewGroupLayoutParams::MATCH_PARENT) {
        *des = String("match-parent");
        return NOERROR;
    }
//    return String.valueOf(size);
    return E_NOT_IMPLEMENTED;
}

ECode ViewGroupLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ViewGroup_Layout),
        ARRAY_SIZE(R::styleable::ViewGroup_Layout));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a));

    ECode ec = SetBaseAttributes(a,
            R::styleable::ViewGroup_Layout_layout_width,
            R::styleable::ViewGroup_Layout_layout_height);
    a->Recycle();
    return ec;
}

ECode ViewGroupLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;
    return NOERROR;
}

ECode ViewGroupLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    source->GetWidth(&mWidth);
    source->GetHeight(&mHeight);
    return NOERROR;
}

ECode ViewGroupLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode ViewGroupLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

Int32 ViewGroupLayoutParams::GetWidth()
{
    return mWidth;
}

Int32 ViewGroupLayoutParams::GetHeight()
{
    return mHeight;
}

ECode ViewGroupLayoutParams::SetLayoutAnimationParameters(
    /* [in] */ IAnimationParameters* ap)
{
    mLayoutAnimationParameters = ap;
    return NOERROR;
}

AutoPtr<IAnimationParameters> ViewGroupLayoutParams::GetLayoutAnimationParameters()
{
    return mLayoutAnimationParameters;
}

ECode ViewGroupLayoutParams::GetLayoutAnimationParameters(
    /* [out] */ IAnimationParameters** ap)
{
    VALIDATE_NOT_NULL(ap);
    *ap = mLayoutAnimationParameters;
    REFCOUNT_ADD(*ap);

    return NOERROR;
}

ECode ViewGroupLayoutParams::ResolveLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    return NOERROR;
}

ECode ViewGroupLayoutParams::OnDebugDraw(
    /* [in] */ IView* view,
    /* [in] */ ICanvas* canvas)
{
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
