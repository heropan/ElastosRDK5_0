
#include "elastos/droid/view/animation/TranslateAnimation.h"
#include "elastos/droid/view/animation/CTranslateAnimation.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_INTERFACE_IMPL(TranslateAnimation, Animation, ITranslateAnimation);
TranslateAnimation::TranslateAnimation()
    : mFromXType(IAnimation::ABSOLUTE)
    , mToXType(IAnimation::ABSOLUTE)
    , mFromYType(IAnimation::ABSOLUTE)
    , mToYType(IAnimation::ABSOLUTE)
    , mFromXValue(0.0f)
    , mToXValue(0.0f)
    , mFromYValue(0.0f)
    , mToYValue(0.0f)
{}

ECode TranslateAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::constructor(context, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::TranslateAnimation),
            ARRAY_SIZE(R::styleable::TranslateAnimation));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    AutoPtr<ITypedValue> value;
    a->PeekValue(R::styleable::TranslateAnimation_fromXDelta, (ITypedValue**)&value);
    AutoPtr<Animation::Description> d = Animation::Description::ParseValue(value);
    mFromXType = d->mType;
    mFromXValue = d->mValue;

    value = NULL;
    a->PeekValue(R::styleable::TranslateAnimation_toXDelta, (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mToXType = d->mType;
    mToXValue = d->mValue;

    value = NULL;
    a->PeekValue(R::styleable::TranslateAnimation_fromYDelta, (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mFromYType = d->mType;
    mFromYValue = d->mValue;

    value = NULL;
    a->PeekValue(R::styleable::TranslateAnimation_toYDelta, (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mToYType = d->mType;
    mToYValue = d->mValue;

    a->Recycle();

    return NOERROR;
}

ECode TranslateAnimation::constructor(
    /* [in] */ Float fromXDelta,
    /* [in] */ Float toXDelta,
    /* [in] */ Float fromYDelta,
    /* [in] */ Float toYDelta)
{
    mFromXValue = fromXDelta;
    mToXValue = toXDelta;
    mFromYValue = fromYDelta;
    mToYValue = toYDelta;

    mFromXType = IAnimation::ABSOLUTE;
    mToXType = IAnimation::ABSOLUTE;
    mFromYType = IAnimation::ABSOLUTE;
    mToYType = IAnimation::ABSOLUTE;

    return NOERROR;
}

ECode TranslateAnimation::constructor(
   /* [in] */ Float fromXType,
   /* [in] */ Float fromXValue,
   /* [in] */ Float toXType,
   /* [in] */ Float toXValue,
   /* [in] */ Float fromYType,
   /* [in] */ Float fromYValue,
   /* [in] */ Float toYType,
   /* [in] */ Float toYValue)
{
    mFromXValue = fromXValue;
    mToXValue = toXValue;
    mFromYValue = fromYValue;
    mToYValue = toYValue;

    mFromXType = fromXType;
    mToXType = toXType;
    mFromYType = fromYType;
    mToYType = toYType;

    return NOERROR;
}

void TranslateAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    Float dx = mFromXDelta;
    Float dy = mFromYDelta;
    if (mFromXDelta != mToXDelta) {
        dx = mFromXDelta + ((mToXDelta - mFromXDelta) * interpolatedTime);
    }
    if (mFromYDelta != mToYDelta) {
        dy = mFromYDelta + ((mToYDelta - mFromYDelta) * interpolatedTime);
    }

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    matrix->SetTranslate(dx, dy);
}

ECode TranslateAnimation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Animation::Initialize(width, height, parentWidth, parentHeight);
    mFromXDelta = ResolveSize(mFromXType, mFromXValue, width, parentWidth);
    mToXDelta = ResolveSize(mToXType, mToXValue, width, parentWidth);
    mFromYDelta = ResolveSize(mFromYType, mFromYValue, height, parentHeight);
    mToYDelta = ResolveSize(mToYType, mToYValue, height, parentHeight);

    return NOERROR;
}

ECode TranslateAnimation::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);

    AutoPtr<ITranslateAnimation> obj;
    CTranslateAnimation::New(0.0f, 0.0f, 0.0f, 0.0f, (ITranslateAnimation**)&obj);
    TranslateAnimation::CloneImpl(obj);
    *object = obj;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

ECode TranslateAnimation::CloneImpl(
    /* [in] */ ITranslateAnimation* object)
{
    Animation::CloneImpl(IAnimation::Probe(object));
    TranslateAnimation* obj = (TranslateAnimation*)object;

    obj->mFromXType = mFromXType;
    obj->mToXType = mToXType;
    obj->mFromYType = mFromYType;
    obj->mToYType = mToYType;
    obj->mFromXValue = mFromXValue;
    obj->mToXValue = mToXValue;
    obj->mFromYValue = mFromYValue;
    obj->mToYValue = mToYValue;
    obj->mFromXDelta = mFromXDelta;
    obj->mToXDelta = mToXDelta;
    obj->mFromYDelta = mFromYDelta;
    obj->mToYDelta = mToYDelta;

    return NOERROR;
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
