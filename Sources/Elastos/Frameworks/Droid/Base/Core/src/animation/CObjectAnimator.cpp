
#include "animation/CObjectAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_OBJECT_IMPL(CObjectAnimator);

ECode CObjectAnimator::constructor()
{
    return NOERROR;
}

AutoPtr<IObjectAnimator> CObjectAnimator::OfInt32(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->Init(target, propertyName);
    anim->SetInt32Values(values);
    return anim;
}

AutoPtr<IObjectAnimator> CObjectAnimator::OfInt32(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->Init(target, property);
    anim->SetInt32Values(values);
    return anim;
}

AutoPtr<IObjectAnimator> CObjectAnimator::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Float>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->Init(target, propertyName);
    anim->SetFloatValues(values);
    return anim;
}

AutoPtr<IObjectAnimator> CObjectAnimator::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Float>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->Init(target, property);
    anim->SetFloatValues(values);
    return anim;
}

AutoPtr<IObjectAnimator> CObjectAnimator::OfObject(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->Init(target, propertyName);
    anim->SetObjectValues(values);
    anim->SetEvaluator(evaluator);
    return anim;
}

AutoPtr<IObjectAnimator> CObjectAnimator::OfObject(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->Init(target, property);
    anim->SetObjectValues(values);
    anim->SetEvaluator(evaluator);
    return anim;
}

AutoPtr<IObjectAnimator> CObjectAnimator::OfPropertyValuesHolder(
    /* [in] */ IInterface* target,
    /* [in] */ ArrayOf<IPropertyValuesHolder*>* values)
{
    AutoPtr<IObjectAnimator> anim;
    CObjectAnimator::New((IObjectAnimator**)&anim);
    anim->SetTarget(target);
    anim->SetValues(values);
    return anim;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
