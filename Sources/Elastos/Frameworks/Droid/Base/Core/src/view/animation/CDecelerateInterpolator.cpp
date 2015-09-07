
#include "view/animation/CDecelerateInterpolator.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {


CAR_OBJECT_IMPL(CDecelerateInterpolator);
CAR_INTERFACE_IMPL_4(CDecelerateInterpolator, Object, IDecelerateInterpolator,INativeInterpolatorFactory,IInterpolator,ITimeInterpolator);

ECode CDecelerateInterpolator::constructor()
{
    mFactor = 1.0f;

    return NOERROR;
}

ECode CDecelerateInterpolator::constructor(
    /* [in] */ Float factor)
{
    mFactor = factor;

    return NOERROR;
}

ECode CDecelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IResourcesTheme> theme;
    context->GetTheme((IResourcesTheme**)&theme);
    return constructor(res, theme, attrs);
}

ECode CDecelerateInterpolator::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::DecelerateInterpolator),
            ARRAY_SIZE(R::styleable::DecelerateInterpolator));
    AutoPtr<ITypedArray> a;
    if (theme != null) {
        theme->btainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    } else {
        res->btainAttributes(attrs, attrIds, (ITypedArray**)&a);
    }

    mFactor = a.getFloat(R.styleable.DecelerateInterpolator_factor, 1.0f);

    a->GetFloat(R::styleable::DecelerateInterpolator_factor, 1.0f, &mFactor);

    return a->Recycle();
}

ECode CDecelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    if (mFactor == 1.0f) {
        *output = (Float)(1.0f - (1.0f - input) * (1.0f - input));
    }
    else {
        *output = (Float)(1.0f - Elastos::Core::Math::Pow((1.0f - input), 2 * mFactor));
    }

    return NOERROR;
}

ECode CDecelerateInterpolator::CreateNativeInterpolator(
    /* [out] */ Int64* interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NativeInterpolatorFactoryHelper::CreateDecelerateInterpolator(mFactor);
    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
