
#include "view/animation/CAlphaAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_INTERFACE_IMPL(AlphaAnimation, Animation, IAlphaAnimation);
AlphaAnimation::AlphaAnimation()
    : mFromAlpha(0.f)
    , mToAlpha(0.f)
{}

AlphaAnimation::AlphaAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mFromAlpha(0.f)
    , mToAlpha(0.f)
{
    constructor(context, attrs);
}

AlphaAnimation::AlphaAnimation(
    /* [in] */ Float fromAlpha,
    /* [in] */ Float toAlpha)
    : mFromAlpha(fromAlpha)
    , mToAlpha(toAlpha)
{}

AutoPtr<IAnimation> AlphaAnimation::GetCloneInstance()
{
    AutoPtr<IAlphaAnimation> result;
    ASSERT_SUCCEEDED(CAlphaAnimation::New(0.f, 0.f, (IAlphaAnimation**)&result));
    return result.Get();
}

ECode AlphaAnimation::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = NULL;
    AutoPtr<IAnimation> result;
    Animation::Clone((IAnimation**)&result);
    if(NULL == result->Probe(EIID_Animation) || NULL ==result->Probe(EIID_IAlphaAnimation)) {
        return NOERROR;
    }
    Animation* temp = (Animation*)result->Probe(EIID_Animation);
    AlphaAnimation* animation = (AlphaAnimation*)temp;
    animation->mFromAlpha = mFromAlpha;
    animation->mToAlpha = mToAlpha;
    *object = result;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

void AlphaAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    t->SetAlpha(mFromAlpha + ((mToAlpha - mFromAlpha) * interpolatedTime));
}

ECode AlphaAnimation::WillChangeTransformationMatrix(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode AlphaAnimation::WillChangeBounds(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode AlphaAnimation::HasAlpha(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = TRUE;
    return NOERROR;
}

ECode AlphaAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::constructor(context, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AlphaAnimation),
            ARRAY_SIZE(R::styleable::AlphaAnimation));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
        attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::AlphaAnimation_fromAlpha, 1.0f, &mFromAlpha);

    a->GetFloat(R::styleable::AlphaAnimation_toAlpha, 1.0f, &mToAlpha);

    a->Recycle();

    return NOERROR;
}

ECode AlphaAnimation::constructor(
    /* [in] */ Float fromAlpha,
    /* [in] */ Float toAlpha)
{
    mFromAlpha = fromAlpha;
    mToAlpha = toAlpha;

    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
