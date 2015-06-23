
#include "view/animation/CAlphaAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

AlphaAnimation::AlphaAnimation()
              : mFromAlpha(0.f)
              , mToAlpha(0.f)
{}

/**
 * Constructor used when an AlphaAnimation is loaded from a resource.
 *
 * @param context Application context to use
 * @param attrs Attribute set from which to read values
 */
AlphaAnimation::AlphaAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
              : mFromAlpha(0.f)
              , mToAlpha(0.f)
{
    Init(context, attrs);
}

/**
 * Constructor to use when building an AlphaAnimation from code
 *
 * @param fromAlpha Starting alpha value for the animation, where 1.0 means
 *        fully opaque and 0.0 means fully transparent.
 * @param toAlpha Ending alpha value for the animation.
 */
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

AutoPtr<IAnimation> AlphaAnimation::Clone()
{
    AutoPtr<IAnimation> result = Animation::Clone();
    if(NULL == result->Probe(EIID_Animation) || NULL ==result->Probe(EIID_IAlphaAnimation))
    {
        return NULL;
    }
    Animation* temp = (Animation*)result->Probe(EIID_Animation);
    AlphaAnimation* animation = (AlphaAnimation*)temp;
    animation->mFromAlpha = mFromAlpha;
    animation->mToAlpha = mToAlpha;
    return result;
}

/**
 * Changes the alpha property of the supplied {@link Transformation}
 */
//@Override
void AlphaAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    t->SetAlpha(mFromAlpha + ((mToAlpha - mFromAlpha) * interpolatedTime));
}

//@Override
Boolean AlphaAnimation::WillChangeTransformationMatrix()
{
    return FALSE;
}

//@Override
Boolean AlphaAnimation::WillChangeBounds()
{
    return FALSE;
}

//@Override
Boolean AlphaAnimation::HasAlpha()
{
    return TRUE;
}

ECode AlphaAnimation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

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

/**
 * Constructor to use when building an AlphaAnimation from code
 *
 * @param fromAlpha Starting alpha value for the animation, where 1.0 means
 *        fully opaque and 0.0 means fully transparent.
 * @param toAlpha Ending alpha value for the animation.
 */
ECode AlphaAnimation::Init(
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
