#include "ext/frameworkext.h"
#include "graphics/drawable/PaintDrawable.h"
#include "graphics/drawable/shapes/CRoundRectShape.h"
#include "R.h"


using Elastos::Droid::Graphics::Drawable::Shapes::IRoundRectShape;
using Elastos::Droid::Graphics::Drawable::Shapes::CRoundRectShape;
using Elastos::Droid::R;


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

PaintDrawable::PaintDrawable()
{}

PaintDrawable::PaintDrawable(
    /* [in] */ Int32 color)
{
    GetPaint()->SetColor(color);
}

ECode PaintDrawable::SetCornerRadius(
    /* [in] */ Float radius)
{
    AutoPtr< ArrayOf<Float> > radii;
    if (radius > 0) {
        radii = ArrayOf<Float>::Alloc(8);
        for (Int32 i = 0; i < 8; i++) {
            (*radii)[i] = radius;
        }
    }
    SetCornerRadii(radii);
    return NOERROR;
}

ECode PaintDrawable::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    if (radii == NULL) {
        if (GetShape() != NULL) {
            SetShape(NULL);
        }
    }
    else {
        AutoPtr<IShape> shape;
        CRoundRectShape::New(radii, NULL, NULL, (IRoundRectShape**)&shape);
        SetShape(shape);
    }
    return InvalidateSelf();;
}

Boolean PaintDrawable::InflateTag(
    /* [in] */ const String& name,
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    if (name.Equals("corners")) {
        Int32 size = ARRAY_SIZE(R::styleable::DrawableCorners);
        AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
        layout->Copy(R::styleable::DrawableCorners, size);

        AutoPtr<ITypedArray> a;
        r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
        Int32 radius;
        a->GetDimensionPixelSize(R::styleable::DrawableCorners_radius, 0, &radius);
        SetCornerRadius(radius);

        // now check of they have any per-corner radii

        Int32 topLeftRadius, topRightRadius, bottomLeftRadius, bottomRightRadius;
        a->GetDimensionPixelSize(
                R::styleable::DrawableCorners_topLeftRadius, radius, &topLeftRadius);
        a->GetDimensionPixelSize(
                R::styleable::DrawableCorners_topRightRadius, radius, &topRightRadius);
        a->GetDimensionPixelSize(
                R::styleable::DrawableCorners_bottomLeftRadius, radius, &bottomLeftRadius);
        a->GetDimensionPixelSize(
                R::styleable::DrawableCorners_bottomRightRadius, radius, &bottomRightRadius);

        if (topLeftRadius != radius || topRightRadius != radius ||
                bottomLeftRadius != radius || bottomRightRadius != radius) {
            AutoPtr< ArrayOf<Float> > radii = ArrayOf<Float>::Alloc(8);
            (*radii)[0] = topLeftRadius; (*radii)[1] = topLeftRadius;
            (*radii)[2] = topRightRadius; (*radii)[3] = topRightRadius;
            (*radii)[4] = bottomLeftRadius; (*radii)[5] = bottomLeftRadius;
            (*radii)[6] = bottomRightRadius; (*radii)[7] = bottomRightRadius;
            SetCornerRadii(radii);
        }
        a->Recycle();
        return TRUE;
    }
    return ShapeDrawable::InflateTag(name, r, parser, attrs);
}

ECode PaintDrawable::Init()
{
    return ShapeDrawable::Init();
}

ECode PaintDrawable::Init(
    /* [in] */ Int32 color)
{
    GetPaint()->SetColor(color);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
