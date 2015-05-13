
#include "view/animation/CRotateAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

RotateAnimation::RotateAnimation()
    : mFromDegrees(0.f)
    , mToDegrees(0.f)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{}

RotateAnimation::RotateAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mFromDegrees(0.f)
    , mToDegrees(0.f)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{
    Init(context, attrs);
}

RotateAnimation::RotateAnimation(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees)
    : mFromDegrees(fromDegrees)
    , mToDegrees(toDegrees)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{}

RotateAnimation::RotateAnimation(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
    : mFromDegrees(fromDegrees)
    , mToDegrees(toDegrees)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(pivotX)
    , mPivotYValue(pivotY)
    , mPivotX(0.f)
    , mPivotY(0.f)
{
    InitializePivotPoint();
}

RotateAnimation::RotateAnimation(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Int32 pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Int32 pivotYType,
    /* [in] */ Float pivotYValue)
    : mFromDegrees(fromDegrees)
    , mToDegrees(toDegrees)
    , mPivotXType(pivotXType)
    , mPivotYType(pivotYType)
    , mPivotXValue(pivotXValue)
    , mPivotYValue(pivotYValue)
    , mPivotX(0.f)
    , mPivotY(0.f)
{
    InitializePivotPoint();
}

AutoPtr<IAnimation> RotateAnimation::GetCloneInstance()
{
    AutoPtr<IRotateAnimation> result;
    ASSERT_SUCCEEDED(CRotateAnimation::New(0.f, 0.f, (IRotateAnimation**)&result));
    return result.Get();
}

AutoPtr<IAnimation> RotateAnimation::Clone()
{
    AutoPtr<IAnimation> result = Animation::Clone();
    if(NULL == result->Probe(EIID_Animation) || NULL ==result->Probe(EIID_IRotateAnimation))
    {
        return NULL;
    }
    Animation* temp = (Animation*)result->Probe(EIID_Animation);
    RotateAnimation* animation = (RotateAnimation*)temp;
    animation->mFromDegrees = mFromDegrees;
    animation->mToDegrees = mToDegrees;
    animation->mPivotXType = mPivotXType;
    animation->mPivotYType = mPivotYType;
    animation->mPivotXValue = mPivotXValue;
    animation->mPivotYValue = mPivotYValue;
    animation->mPivotX = mPivotX;
    animation->mPivotY = mPivotY;
    return result;
}


void RotateAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    Float degrees = mFromDegrees + ((mToDegrees - mFromDegrees) * interpolatedTime);
    Float scale = GetScaleFactor();

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    if (mPivotX == 0.0f && mPivotY == 0.0f) {
        matrix->SetRotateEx(degrees);
    }
    else {
        matrix->SetRotate(degrees, mPivotX * scale, mPivotY * scale);
    }
}

ECode RotateAnimation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Animation::Initialize(width, height, parentWidth, parentHeight);
    mPivotX = ResolveSize(mPivotXType, mPivotXValue, width, parentWidth);
    mPivotY = ResolveSize(mPivotYType, mPivotYValue, height, parentHeight);

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::RotateAnimation),
            ARRAY_SIZE(R::styleable::RotateAnimation));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::RotateAnimation_fromDegrees, 0.0f, &mFromDegrees);

    a->GetFloat(R::styleable::RotateAnimation_toDegrees, 0.0f, &mToDegrees);

    AutoPtr<ITypedValue> value;
    a->PeekValue(R::styleable::RotateAnimation_pivotX, (ITypedValue**)&value);
    AutoPtr<Animation::Description> d = Animation::Description::ParseValue(value);
    mPivotXType = d->mType;
    mPivotXValue = d->mValue;

    value = NULL;
    a->PeekValue(R::styleable::RotateAnimation_pivotY, (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mPivotYType = d->mType;
    mPivotYValue = d->mValue;

    a->Recycle();

    InitializePivotPoint();

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees)
{
    mFromDegrees = fromDegrees;
    mToDegrees = toDegrees;
    mPivotX = 0.0f;
    mPivotY = 0.0f;

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    mFromDegrees = fromDegrees;
    mToDegrees = toDegrees;

    mPivotXType = IAnimation::ABSOLUTE;
    mPivotYType = IAnimation::ABSOLUTE;
    mPivotXValue = pivotX;
    mPivotYValue = pivotY;

    return NOERROR;
}

ECode RotateAnimation::Init(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Int32 pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Int32 pivotYType,
    /* [in] */ Float pivotYValue)
{
    mFromDegrees = fromDegrees;
    mToDegrees = toDegrees;

    mPivotXValue = pivotXValue;
    mPivotXType = pivotXType;
    mPivotYValue = pivotYValue;
    mPivotYType = pivotYType;

    return NOERROR;
}

void RotateAnimation::InitializePivotPoint()
{
    if (mPivotXType == IAnimation::ABSOLUTE) {
        mPivotX = mPivotXValue;
    }
    if (mPivotYType == IAnimation::ABSOLUTE) {
        mPivotY = mPivotYValue;
    }
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
