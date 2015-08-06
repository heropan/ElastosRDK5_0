
#include "animation/CObjectAnimatorHelper.h"
#include "animation/CObjectAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_SINGLETON_IMPL(CObjectAnimatorHelper)
CAR_INTERFACE_IMPL(CObjectAnimatorHelper, Singleton, IObjectAnimatorHelper)

ECode CObjectAnimatorHelper::OfInt32(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Int32>* values,
    /* [out] */ IObjectAnimator** anim)
{
    AutoPtr<IObjectAnimator> animTemp = CObjectAnimator::OfInt32(target, propertyName, values);
    *anim = animTemp;
    REFCOUNT_ADD(*anim)
    return NOERROR;
}

ECode CObjectAnimatorHelper::OfInt32(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Int32>* values,
    /* [out] */ IObjectAnimator** anim)
{
    AutoPtr<IObjectAnimator> animTemp = CObjectAnimator::OfInt32(target, property, values);
    *anim = animTemp;
    REFCOUNT_ADD(*anim)
    return NOERROR;
}

ECode CObjectAnimatorHelper::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Float>* values,
    /* [out] */ IObjectAnimator** anim)
{
    AutoPtr<IObjectAnimator> animTemp = CObjectAnimator::OfFloat(target, propertyName, values);
    *anim = animTemp;
    REFCOUNT_ADD(*anim)
    return NOERROR;
}

ECode CObjectAnimatorHelper::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Float>* values,
    /* [out] */ IObjectAnimator** anim)
{
    AutoPtr<IObjectAnimator> animTemp = CObjectAnimator::OfFloat(target, property, values);
    *anim = animTemp;
    REFCOUNT_ADD(*anim)
    return NOERROR;
}

ECode CObjectAnimatorHelper::OfObject(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values,
    /* [out] */ IObjectAnimator** anim)
{
    AutoPtr<IObjectAnimator> animTemp = CObjectAnimator::OfObject(target, propertyName, evaluator, values);
    *anim = animTemp;
    REFCOUNT_ADD(*anim)
    return NOERROR;
}

ECode CObjectAnimatorHelper::OfObject(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values,
    /* [out] */ IObjectAnimator** anim)
{
    AutoPtr<IObjectAnimator> animTemp = CObjectAnimator::OfObject(target, property, evaluator, values);
    *anim = animTemp;
    REFCOUNT_ADD(*anim)
    return NOERROR;
}

ECode CObjectAnimatorHelper::OfPropertyValuesHolder(
    /* [in] */ IInterface* target,
    /* [in] */ ArrayOf<IPropertyValuesHolder*>* values,
    /* [out] */ IObjectAnimator** anim)
{
    AutoPtr<IObjectAnimator> animTemp = CObjectAnimator::OfPropertyValuesHolder(target, values);
    *anim = animTemp;
    REFCOUNT_ADD(*anim)
    return NOERROR;
}

} // namespace Animation
} // namepsace Droid
} // namespace Elastos
