
#include "elastos/droid/view/animation/CAnticipateInterpolator.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CAnticipateInterpolator);
CAR_INTERFACE_IMPL_4(CAnticipateInterpolator, Object, IAnticipateInterpolator,INativeInterpolatorFactory,IInterpolator,ITimeInterpolator);

ECode CAnticipateInterpolator::constructor()
{
    mTension = 2.0f;
    return NOERROR;
}

ECode CAnticipateInterpolator::constructor(
    /* [in] */ Float tension)
{
    mTension = tension;

    return NOERROR;
}

ECode CAnticipateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IResourcesTheme> theme;
    context->GetTheme((IResourcesTheme**)&theme);

    return constructor(res, theme, attrs);
}

ECode CAnticipateInterpolator::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AnticipateInterpolator),
            ARRAY_SIZE(R::styleable::AnticipateInterpolator));

    AutoPtr<ITypedArray> a;
    if (theme != NULL) {
        theme->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    } else {
        res->ObtainAttributes(attrs, attrIds, (ITypedArray**)&a);
    }

    a->GetFloat(R::styleable::AnticipateInterpolator_tension, 2.0f, &mTension);

    return a->Recycle();
}

ECode CAnticipateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    // a(t) = t * t * ((tension + 1) * t - tension)
    *output = input * input * ((mTension + 1) * input - mTension);
    return NOERROR;
}

ECode CAnticipateInterpolator::CreateNativeInterpolator(
    /* [out] */ Int64* interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NativeInterpolatorFactoryHelper::CreateAnticipateInterpolator(mTension);
    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
