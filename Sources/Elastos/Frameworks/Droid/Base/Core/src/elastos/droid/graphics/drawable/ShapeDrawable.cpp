
#include "elastos/droid/graphics/drawable/ShapeDrawable.h"
#include "elastos/droid/graphics/drawable/CShapeDrawable.h"
// #include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Core::ICloneable;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

ShapeDrawable::ShapeState::ShapeState(
    /* [in] */ ShapeState* orig)
    : mChangingConfigurations(0)
    , mIntrinsicWidth(0)
    , mIntrinsicHeight(0)
    , mAlpha(255)
{
    if (orig != NULL) {
        mPaint = orig->mPaint;
        mShape = orig->mShape;
        mPadding = orig->mPadding;
        mIntrinsicWidth = orig->mIntrinsicWidth;
        mIntrinsicHeight = orig->mIntrinsicHeight;
        mAlpha = orig->mAlpha;
        mShaderFactory = orig->mShaderFactory;
    }
    else {
        assert(0 && "TODO");
        // CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mPaint);
    }
}

ECode ShapeDrawable::ShapeState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CShapeDrawable::New((Handle32)this, (IShapeDrawable**)drawable);
}

//@Override
ECode ShapeDrawable::ShapeState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CShapeDrawable::New((Handle32)this, (IShapeDrawable**)drawable);
}

ECode ShapeDrawable::ShapeState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

CAR_INTERFACE_IMPL(ShapeDrawable, Drawable, IShapeDrawable);
ShapeDrawable::ShapeDrawable()
    : mMutated(FALSE)
{
}

ShapeDrawable::ShapeDrawable(
    /* [in] */ IShape* s)
    : mMutated(FALSE)
{
    mShapeState = new ShapeState(NULL);
    mShapeState->mShape = s;
}

ShapeDrawable::ShapeDrawable(
    /* [in] */ ShapeState* state)
    : mMutated(FALSE)
{
    mShapeState = new ShapeState(state);
}

ECode ShapeDrawable::GetShape(
    /* [out] */ IShape** shape)
{
    VALIDATE_NOT_NULL(shape);
    *shape = mShapeState->mShape;
    REFCOUNT_ADD(*shape);
    return NOERROR;
}

ECode ShapeDrawable::SetShape(
    /* [in] */ IShape* s)
{
    mShapeState->mShape = s;
    UpdateShape();
    return NOERROR;
}

ECode ShapeDrawable::SetShaderFactory(
    /* [in] */ IShaderFactory* fact)
{
    mShapeState->mShaderFactory = fact;
    return NOERROR;
}

ECode ShapeDrawable::GetShaderFactory(
    /* [out] */ IShaderFactory** fact)
{
    VALIDATE_NOT_NULL(fact);
    *fact = mShapeState->mShaderFactory;
    REFCOUNT_ADD(*fact);
    return NOERROR;
}

ECode ShapeDrawable::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);
    *paint = mShapeState->mPaint;
    REFCOUNT_ADD(*paint);
    return NOERROR;
}

ECode ShapeDrawable::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if ((left | top | right | bottom) == 0) {
        mShapeState->mPadding = NULL;
    }
    else {
        if (mShapeState->mPadding == NULL) {
             CRect::New((IRect**)&mShapeState->mPadding);
        }
        mShapeState->mPadding->Set(left, top, right, bottom);
    }
    return InvalidateSelf();
}

ECode ShapeDrawable::SetPadding(
    /* [in] */ IRect* padding)
{
    if (padding == NULL) {
        mShapeState->mPadding = NULL;
    }
    else {
        if (mShapeState->mPadding == NULL) {
            CRect::New((IRect**)&mShapeState->mPadding);
        }
        mShapeState->mPadding->Set(padding);
    }
    return InvalidateSelf();
}

ECode ShapeDrawable::SetIntrinsicWidth(
    /* [in] */ Int32 width)
{
    mShapeState->mIntrinsicWidth = width;
    return InvalidateSelf();
}

ECode ShapeDrawable::SetIntrinsicHeight(
    /* [in] */ Int32 height)
{
    mShapeState->mIntrinsicHeight = height;
    return InvalidateSelf();
}

ECode ShapeDrawable::GetIntrinsicWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mShapeState->mIntrinsicWidth;
    return NOERROR;
}

ECode ShapeDrawable::GetIntrinsicHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mShapeState->mIntrinsicHeight;
    return NOERROR;
}

ECode ShapeDrawable::GetPadding(
    /* [in] */ IRect* padding,
    /* [out] */ Boolean* isPadding)
{
    VALIDATE_NOT_NULL(isPadding);
    if (mShapeState->mPadding != NULL) {
        padding->Set(mShapeState->mPadding);
        *isPadding = TRUE;
        return NOERROR;
    }

    return Drawable::GetPadding(padding, isPadding);
}

Int32 ShapeDrawable::ModulateAlpha(
    /* [in] */ Int32 paintAlpha,
    /* [in] */ Int32 alpha)
{
    UInt32 u = alpha;
    Int32 scale = alpha + (u >> 7);  // convert to 0..256
    u = paintAlpha * scale;
    return (Int32)(u >> 8);
}

void ShapeDrawable::OnDraw(
    /* [in] */ IShape* shape,
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    shape->Draw(canvas, paint);
}

ECode ShapeDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IRect> r;
    GetBounds((IRect**)&r);
    AutoPtr<IPaint> paint = mShapeState->mPaint;

    Int32 prevAlpha;
    paint->GetAlpha(&prevAlpha);
    paint->SetAlpha(ModulateAlpha(prevAlpha, mShapeState->mAlpha));

    if (mShapeState->mShape != NULL) {
        // need the save both for the translate, and for the (unknown) Shape
        Int32 count;
        canvas->Save(&count);
        canvas->Translate(
                static_cast<CRect*>(r.Get())->mLeft,
                static_cast<CRect*>(r.Get())->mTop);
        OnDraw(mShapeState->mShape, canvas, paint);
        canvas->RestoreToCount(count);
    }
    else {
        canvas->DrawRect(r, paint);
    }

    // restore
    paint->SetAlpha(prevAlpha);
    return NOERROR;
}

ECode ShapeDrawable::GetChangingConfigurations(
    /* [out] */ Int32* configuration)
{
    VALIDATE_NOT_NULL(configuration);
    Drawable::GetChangingConfigurations(configuration);
    *configuration = (*configuration)
            | mShapeState->mChangingConfigurations;
    return NOERROR;
}

ECode ShapeDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    mShapeState->mAlpha = alpha;
    return InvalidateSelf();
}

ECode ShapeDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    mShapeState->mPaint->SetColorFilter(cf);
    return InvalidateSelf();
}

ECode ShapeDrawable::GetOpacity(
    /* [out] */ Int32* opacity)
{
    VALIDATE_NOT_NULL(opacity);
    if (mShapeState->mShape == NULL) {
        AutoPtr<IPaint> p = mShapeState->mPaint;
        AutoPtr<IXfermode> mode;
        if (p->GetXfermode((IXfermode**)&mode), mode == NULL) {
            Int32 alpha;
            p->GetAlpha(&alpha);
            if (alpha == 0) {
                *opacity = IPixelFormat::TRANSPARENT;
                return NOERROR;
            }
            if (alpha == 255) {
                *opacity = IPixelFormat::OPAQUE;
                return NOERROR;
            }
        }
    }
    // not sure, so be safe
    *opacity = IPixelFormat::TRANSLUCENT;
    return NOERROR;
}

ECode ShapeDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    mShapeState->mPaint->SetDither(dither);
    return InvalidateSelf();
}

void ShapeDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    Drawable::OnBoundsChange(bounds);
    UpdateShape();
}

Boolean ShapeDrawable::InflateTag(
    /* [in] */ const String& name,
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    if (name.Equals("padding")) {
        Int32 size = ARRAY_SIZE(R::styleable::ShapeDrawablePadding);
        AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
        layout->Copy(R::styleable::ShapeDrawablePadding, size);

        AutoPtr<ITypedArray> a;
        r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
        Int32 left, top, right, bottom;
        a->GetDimensionPixelOffset(
                R::styleable::ShapeDrawablePadding_left, 0, &left);
        a->GetDimensionPixelOffset(
                R::styleable::ShapeDrawablePadding_top, 0, &top);
        a->GetDimensionPixelOffset(
                R::styleable::ShapeDrawablePadding_right, 0, &right);
        a->GetDimensionPixelOffset(
                R::styleable::ShapeDrawablePadding_bottom, 0, &bottom);
        SetPadding(left, top, right, bottom);
        a->Recycle();
        return TRUE;
    }

    return FALSE;
}

ECode ShapeDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Drawable::Inflate(r, parser, attrs));

    Int32 size = ARRAY_SIZE(R::styleable::ShapeDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::ShapeDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

    Int32 color;
    mShapeState->mPaint->GetColor(&color);
    a->GetColor(R::styleable::ShapeDrawable_color, color, &color);
    mShapeState->mPaint->SetColor(color);

    Boolean dither = FALSE;
    a->GetBoolean(R::styleable::ShapeDrawable_dither, FALSE, &dither);
    mShapeState->mPaint->SetDither(dither);

    Float width, height;
    a->GetDimension(
            R::styleable::ShapeDrawable_width, 0.0f, &width);
    a->GetDimension(
            R::styleable::ShapeDrawable_height, 0.0f, &height);
    SetIntrinsicWidth((Int32)width);
    SetIntrinsicHeight((Int32)height);

    a->Recycle();

    Int32 type, outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT)
           && (type != IXmlPullParser::END_TAG ||
              (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);
        // call our subclass
        if (!InflateTag(name, r, parser, attrs)) {
//            android.util.Log.w("drawable", "Unknown element: " + name +
//                    " for ShapeDrawable " + this);
        }
    }

    return NOERROR;
}

void ShapeDrawable::UpdateShape()
{
    if (mShapeState->mShape != NULL) {
        AutoPtr<IRect> r;
        GetBounds((IRect**)&r);
        Int32 w, h;
        r->GetWidth(&w);
        r->GetHeight(&h);

        mShapeState->mShape->Resize(w, h);
        if (mShapeState->mShaderFactory != NULL) {
            AutoPtr<IShader> shader;
            mShapeState->mShaderFactory->Resize(w, h, (IShader**)&shader);
            mShapeState->mPaint->SetShader(shader);
        }
    }
    InvalidateSelf();
}

ECode ShapeDrawable::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    Drawable::GetChangingConfigurations(&mShapeState->mChangingConfigurations);
    *state = (IDrawableConstantState*)mShapeState->Probe(EIID_IDrawableConstantState);
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode ShapeDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> tmp;
    if (!mMutated &&
            (Drawable::Mutate((IDrawable**)&tmp), tmp.Get()) == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<IPaint> paint;
        assert(0 && "TODO");
        // if (mShapeState->mPaint != NULL) {
        //     CPaint::New(mShapeState->mPaint, (IPaint**)&paint);
        // } else {
        //     CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&paint);
        // }
        mShapeState->mPaint = paint;

        AutoPtr<IRect> padding;
        if (mShapeState->mPadding != NULL) {
            CRect::New(mShapeState->mPadding, (IRect**)&padding);
        } else {
            CRect::New((IRect**)&padding);
        }
        mShapeState->mPadding = padding;

//        try {
        AutoPtr<IShape> shape;
        ICloneable::Probe(mShapeState->mShape)->Clone((IInterface**)&shape);
        mShapeState->mShape = shape;
//        } catch (CloneNotSupportedException e) {
//            return null;
//        }
        mMutated = TRUE;
    }
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode ShapeDrawable::constructor()
{
    return constructor((ShapeState*)NULL);
}

ECode ShapeDrawable::constructor(
    /* [in] */ IShape* s)
{
    constructor((ShapeState*)NULL);
    mShapeState->mShape = s;
    return NOERROR;
}

ECode ShapeDrawable::constructor(
    /* [in] */ ShapeState* state)
{
    mShapeState = new ShapeState(state);
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
