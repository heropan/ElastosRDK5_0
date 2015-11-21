
#include "elastos/droid/view/animation/COvershootInterpolator.h"
#include "elastos/droid/internal/view/animation/NativeInterpolatorFactoryHelper.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Animation::EIID_ITimeInterpolator;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Internal::View::Animation::EIID_INativeInterpolatorFactory;
using Elastos::Droid::Internal::View::Animation::NativeInterpolatorFactoryHelper;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(COvershootInterpolator);

CAR_INTERFACE_IMPL_4(COvershootInterpolator, Object, IOvershootInterpolator, INativeInterpolatorFactory, IInterpolator, ITimeInterpolator);

COvershootInterpolator::COvershootInterpolator()
    : mTension(0.0f)
{}

COvershootInterpolator::~COvershootInterpolator()
{}

ECode COvershootInterpolator::constructor()
{
    mTension = 2.0f;
    return NOERROR;
}

ECode COvershootInterpolator::constructor(
    /* [in] */ Float tension)
{
    mTension = tension;
    return NOERROR;
}

ECode COvershootInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IResourcesTheme> theme;
    context->GetTheme((IResourcesTheme**)&theme);
    return constructor(res, theme, attrs);
}

ECode COvershootInterpolator::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::OvershootInterpolator),
            ARRAY_SIZE(R::styleable::OvershootInterpolator));
    AutoPtr<ITypedArray> a;
    if (theme != NULL) {
        theme->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    }
    else {
        res->ObtainAttributes(attrs, attrIds, (ITypedArray**)&a);
    }

    a->GetFloat(R::styleable::OvershootInterpolator_tension, 2.0f, &mTension);

    return a->Recycle();
}

ECode COvershootInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    // _o(t) = t * t * ((tension + 1) * t + tension)
    // o(t) = _o(t - 1) + 1
    input -= 1.0f;
    *output = input * input * ((mTension + 1) * input + mTension) + 1.0f;

    return NOERROR;
}

ECode COvershootInterpolator::CreateNativeInterpolator(
    /* [out] */ Int64* interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NativeInterpolatorFactoryHelper::CreateOvershootInterpolator(mTension);
    return NOERROR;
}

ECode COvershootInterpolator::HasNativeInterpolator(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TRUE;
    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
