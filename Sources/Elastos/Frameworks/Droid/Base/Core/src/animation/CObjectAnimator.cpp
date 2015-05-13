
#include "animation/CObjectAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

IANIMATOR_METHOD_IMPL(CObjectAnimator, ObjectAnimator)
IVALUEANIMATOR_METHOD_IMPL(CObjectAnimator, ObjectAnimator)

ECode CObjectAnimator::constructor()
{
    return NOERROR;
}

PInterface CObjectAnimator::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_Animator)
    {
        return reinterpret_cast<PInterface>((Animator*)this);
    }else if(riid == EIID_ValueAnimator) {
        return reinterpret_cast<PInterface>((ValueAnimator*)this);
    }

    return _CObjectAnimator::Probe(riid);
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

AutoPtr<IObjectAnimator> CObjectAnimator::OfInt32Ex(
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

AutoPtr<IObjectAnimator> CObjectAnimator::OfFloatEx(
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

AutoPtr<IObjectAnimator> CObjectAnimator::OfObjectEx(
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

ECode CObjectAnimator::SetPropertyName(
    /* [in] */ const String& propertyName)
{
    return ObjectAnimator::SetPropertyName(propertyName);
}

ECode CObjectAnimator::SetProperty(
    /* [in] */ IProperty* property)
{
    return ObjectAnimator::SetProperty(property);
}

ECode CObjectAnimator::GetPropertyName(
    /* [out] */ String* propertyName)
{
    VALIDATE_NOT_NULL(propertyName);
    *propertyName = ObjectAnimator::GetPropertyName();
    return NOERROR;
}

ECode CObjectAnimator::GetTarget(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<IInterface> tmp = ObjectAnimator::GetTarget();
    *object = tmp;
    INTERFACE_ADDREF(*object);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
