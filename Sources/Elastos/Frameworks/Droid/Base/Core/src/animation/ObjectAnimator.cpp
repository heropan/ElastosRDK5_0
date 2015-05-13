#include "animation/ObjectAnimator.h"
#include "animation/CObjectAnimator.h"
#include "animation/PropertyValuesHolder.h"

namespace Elastos {
namespace Droid {
namespace Animation {

ObjectAnimator::ObjectAnimator()
{}


ECode ObjectAnimator::SetPropertyName(
    /* [in] */ const String& propertyName)
{
    // mValues could be NULL if this is being constructed piecemeal. Just record the
    // propertyName to be used later when setValues() is called if so.
    if (mValues != NULL) {
        AutoPtr<IPropertyValuesHolder> valuesHolder = (*mValues)[0];
        String oldName;
        valuesHolder->GetPropertyName(&oldName);

        valuesHolder->SetPropertyName(propertyName);
        mValuesMap.Erase(oldName);
        mValuesMap[propertyName] = valuesHolder;
    }

    mPropertyName = propertyName;
    // New property/values/target should cause re-initialization prior to starting
    mInitialized = FALSE;

    return NOERROR;
}

ECode ObjectAnimator::SetProperty(
    /* [in] */ IProperty* property)
{
    // mValues could be NULL if this is being constructed piecemeal. Just record the
    // propertyName to be used later when setValues() is called if so.
    if (mValues != NULL) {
        AutoPtr<IPropertyValuesHolder> valuesHolder = (*mValues)[0];
        String oldName;
        valuesHolder->GetPropertyName(&oldName);
        valuesHolder->SetProperty(property);
        mValuesMap.Erase(oldName);
        mValuesMap[mPropertyName] = valuesHolder;
    }

    if (mProperty != NULL) {
        property->GetName(&mPropertyName);
    }

    mProperty = property;
    // New property/values/target should cause re-initialization prior to starting
    mInitialized = FALSE;

    return NOERROR;
}

String ObjectAnimator::GetPropertyName()
{
    return mPropertyName;
}

ECode ObjectAnimator::Init(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName)
{
    mTarget = target;
    return SetPropertyName(propertyName);
}

ECode ObjectAnimator::Init(
    /* [in] */ IInterface* target,
    /* [in] */ AutoPtr<IProperty> property)
{
    mTarget = target;
    return SetProperty(property);
}

ECode ObjectAnimator::SetInt32Values(
    /* [in] */ ArrayOf<Int32>* values)
{
    if (mValues == NULL || mValues->GetLength() == 0) {
        // No values yet - this animator is being constructed piecemeal. Init the values with
        // whatever the current propertyName is
        AutoPtr<ArrayOf<IPropertyValuesHolder*> > params =ArrayOf<IPropertyValuesHolder*>::Alloc(1);
        if (mProperty != NULL) {
            params->Set(0, PropertyValuesHolder::OfInt32(mProperty, values));
            SetValues(params);
        } else {
            params->Set(0, PropertyValuesHolder::OfInt32(mPropertyName, values));
            SetValues(params);
        }
    } else {
        ValueAnimator::SetInt32Values(values);
    }

    return NOERROR;
}

ECode ObjectAnimator::SetFloatValues(
    /* [in] */ ArrayOf<Float>* values)
{
    if (mValues == NULL || mValues->GetLength() == 0) {
        // No values yet - this animator is being constructed piecemeal. Init the values with
        // whatever the current propertyName is
        AutoPtr<ArrayOf<IPropertyValuesHolder*> > params =ArrayOf<IPropertyValuesHolder*>::Alloc(1);
        if (mProperty != NULL) {
            AutoPtr<IPropertyValuesHolder> holder = PropertyValuesHolder::OfFloat(mProperty, values);
            params->Set(0, holder);
            SetValues(params);
        } else {
            AutoPtr<IPropertyValuesHolder> holder = PropertyValuesHolder::OfFloat(mPropertyName, values);
            params->Set(0, holder);
            SetValues(params);
        }
    } else {
        ValueAnimator::SetFloatValues(values);
    }

    return NOERROR;
}

ECode ObjectAnimator::SetObjectValues(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    if (mValues == NULL || mValues->GetLength() == 0) {
        // No values yet - this animator is being constructed piecemeal. Init the values with
        // whatever the current propertyName is
        AutoPtr<ArrayOf<IPropertyValuesHolder*> > params =ArrayOf<IPropertyValuesHolder*>::Alloc(1);
        if (mProperty != NULL) {
            AutoPtr<IPropertyValuesHolder> holder = PropertyValuesHolder::OfObject(mProperty, NULL, values);
            params->Set(0, holder);
            SetValues(params);
        } else {
            AutoPtr<IPropertyValuesHolder> holder = PropertyValuesHolder::OfObject(mPropertyName, NULL, values);
            params->Set(0, holder);
            SetValues(params);
        }
    } else {
        ValueAnimator::SetObjectValues(values);
    }

    return NOERROR;
}

ECode ObjectAnimator::Start()
{
//     if (DBG) {
//         for (Int32 i = 0; i < mValues->GetLength(); ++i) {
//             AutoPtr<IPropertyValuesHolder> pvh = (*mValues)[i];
//             ArrayList<Keyframe> keyframes = pvh.mKeyframeSet.mKeyframes;
//             Log.d("ObjectAnimator", "   Values[" + i + "]: " +
//                 pvh.getPropertyName() + ", " + keyframes.get(0).getValue() + ", " +
//                 keyframes.get(pvh.mKeyframeSet.mNumKeyframes - 1).getValue());
//         }
//     }
    ValueAnimator::Start();
    return NOERROR;
}

ECode ObjectAnimator::InitAnimation()
{
    if (!mInitialized) {
        // mValueType may change due to setter/getter setup; do this before calling super.init(),
        // which uses mValueType to set up the default type evaluator.
        Int32 numValues = mValues->GetLength();
        for (Int32 i = 0; i < numValues; ++i) {
           (reinterpret_cast<PropertyValuesHolder*>((*mValues)[i]->Probe(EIID_PropertyValuesHolder)))->SetupSetterAndGetter(mTarget);
        }

        ValueAnimator::InitAnimation();
    }
    return NOERROR;
}

ECode ObjectAnimator::SetDuration(
    /* [in] */ Int64 duration)
{
    ValueAnimator::SetDuration(duration);
    return NOERROR;
}

AutoPtr<IInterface> ObjectAnimator::GetTarget()
{
    return mTarget;
}

ECode ObjectAnimator::SetTarget(
    /* [in] */ IInterface* target)
{
    if (target != mTarget) {
        AutoPtr<IInterface> oldTarget = mTarget;
        mTarget = target;
        AutoPtr<IObject> old = IObject::Probe(oldTarget);
        AutoPtr<IObject> now = IObject::Probe(target);
        assert(now != NULL);
        EGuid oldId = ECLSID_CDummyObject;
        EGuid newId;
        if(old != NULL) {
            old->GetClassID(&oldId);
        }
        now->GetClassID(&newId);
         if (oldTarget != NULL && target != NULL && oldId == newId) {
             return NOERROR;
         }

        // New target type should cause re-initialization prior to starting
        mInitialized = FALSE;
    }

    return NOERROR;
}

ECode ObjectAnimator::SetupStartValues()
{
    InitAnimation();
    Int32 numValues = mValues->GetLength();
    for (Int32 i = 0; i < numValues; ++i) {
        AutoPtr<IPropertyValuesHolder> holder = (*mValues)[i];
        reinterpret_cast<PropertyValuesHolder*>(holder->Probe(EIID_PropertyValuesHolder))->SetupStartValue(mTarget);
    }

    return NOERROR;
}

ECode ObjectAnimator::SetupEndValues()
{
    InitAnimation();
    Int32 numValues = mValues->GetLength();
    for (Int32 i = 0; i < numValues; ++i) {
        AutoPtr<IPropertyValuesHolder> holder = (*mValues)[i];
        reinterpret_cast<PropertyValuesHolder*>(holder->Probe(EIID_PropertyValuesHolder))->SetupEndValue(mTarget);
    }

    return NOERROR;
}

ECode ObjectAnimator::AnimateValue(
    /* [in] */ Float fraction)
{
    ValueAnimator::AnimateValue(fraction);
    Int32 numValues = mValues->GetLength();
    for (Int32 i = 0; i < numValues; ++i) {
        AutoPtr<IPropertyValuesHolder> holder = (*mValues)[i];
        PropertyValuesHolder* ptr = reinterpret_cast<PropertyValuesHolder*>(holder->Probe(EIID_PropertyValuesHolder));
        ptr->SetAnimatedValue(mTarget);
    }
    return NOERROR;
}

ECode ObjectAnimator::Clone(
    /* [out] */ IAnimator** object)
{
    AutoPtr<CObjectAnimator> newObject;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&newObject);
    CloneSuperData(newObject.Get());
    CloneInternal(newObject);
    ObjectAnimator* anim = newObject;
    anim->mTarget = mTarget;
    anim->mPropertyName = mPropertyName;
    anim->mProperty = mProperty;
    *object = (IAnimator*)newObject->Probe(EIID_IAnimator);
    INTERFACE_ADDREF(*object);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
