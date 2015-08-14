
#include "animation/ObjectAnimator.h"
#include "animation/CObjectAnimator.h"
#include "animation/PropertyValuesHolder.h"

namespace Elastos {
namespace Droid {
namespace Animation {

String ObjectAnimator::LOG_TAG = String("ObjectAnimator");
CAR_INTERFACE_IMPL(ObjectAnimator, ValueAnimator, IObjectAnimator);

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
    String propertyName;
    if (mPropertyName != NULL) {
        propertyName = mPropertyName;
    } else if (mProperty != NULL) {
        mProperty->GetName(&propertyName);
    } else if (mValues != NULL && mValues->GetLength() > 0) {
        for (Int32 i = 0; i < mValues->GetLength(); ++i) {
            if (i == 0) {
                propertyName = String("");
            } else {
                propertyName += String(",");
            }

            String tmp;
            (*mValues)[i]->GetPropertyName(&tmp);
            propertyName += tmp;
        }
    }
    return propertyName;
}

String ObjectAnimator::GetNameForTrace() {
    return String("animator:") + GetPropertyName();
}

ECode ObjectAnimator::constructor(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName)
{
    SetTarget(target);
    return SetPropertyName(propertyName);
}

ECode ObjectAnimator::constructor(
    /* [in] */ IInterface* target,
    /* [in] */ AutoPtr<IProperty> property)
{
    SetTarget(target);
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
    // See if any of the current active/pending animators need to be canceled
    AnimationHandler handler = sAnimationHandler.get();
    if (handler != null) {
        int numAnims = handler.mAnimations.size();
        for (int i = numAnims - 1; i >= 0; i--) {
            if (handler.mAnimations.get(i) instanceof ObjectAnimator) {
                ObjectAnimator anim = (ObjectAnimator) handler.mAnimations.get(i);
                if (anim.mAutoCancel && hasSameTargetAndProperties(anim)) {
                    anim.cancel();
                }
            }
        }
        numAnims = handler.mPendingAnimations.size();
        for (int i = numAnims - 1; i >= 0; i--) {
            if (handler.mPendingAnimations.get(i) instanceof ObjectAnimator) {
                ObjectAnimator anim = (ObjectAnimator) handler.mPendingAnimations.get(i);
                if (anim.mAutoCancel && hasSameTargetAndProperties(anim)) {
                    anim.cancel();
                }
            }
        }
        numAnims = handler.mDelayedAnims.size();
        for (int i = numAnims - 1; i >= 0; i--) {
            if (handler.mDelayedAnims.get(i) instanceof ObjectAnimator) {
                ObjectAnimator anim = (ObjectAnimator) handler.mDelayedAnims.get(i);
                if (anim.mAutoCancel && hasSameTargetAndProperties(anim)) {
                    anim.cancel();
                }
            }
        }
    }
    // if (DBG) {
    //     Log.d(LOG_TAG, "Anim target, duration: " + getTarget() + ", " + getDuration());
    //     for (int i = 0; i < mValues.length; ++i) {
    //         PropertyValuesHolder pvh = mValues[i];
    //         Log.d(LOG_TAG, "   Values[" + i + "]: " +
    //             pvh.getPropertyName() + ", " + pvh.mKeyframes.getValue(0) + ", " +
    //             pvh.mKeyframes.getValue(1));
    //     }
    // }
    ValueAnimator::Start();
    return NOERROR;
}

ECode ObjectAnimator::InitAnimation()
{
    if (!mInitialized) {
        // mValueType may change due to setter/getter setup; do this before calling super.init(),
        // which uses mValueType to set up the default type evaluator.
        AutoPtr<IInterface> target;
        GetTarget((IInterface**)&target);
        if (target != NULL) {
            const Int32 numValues = mValues->GetLength();
            for (Int32 i = 0; i < numValues; ++i) {
                (reinterpret_cast<PropertyValuesHolder*>((*mValues)[i]->Probe(EIID_PropertyValuesHolder)))->SetupSetterAndGetter(target);
            }
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

ECode ObjectAnimator::GetTarget(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = mTarget;
    REFCOUNT_ADD(*object);
    return NOERROR;
}

ECode ObjectAnimator::SetTarget(
    /* [in] */ IInterface* target)
{
    if (target != mTarget) {
        AutoPtr<IInterface> oldTarget = mTarget;
        mTarget = IWeakReference::Probe(target);
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
    AutoPtr<IInterface> target;
    GetTarget((IInterface**)&target);
    if (target != NULL) {
        Int32 numValues = mValues->GetLength();
        for (Int32 i = 0; i < numValues; ++i) {
            AutoPtr<IPropertyValuesHolder> holder = (*mValues)[i];
            reinterpret_cast<PropertyValuesHolder*>(holder->Probe(EIID_PropertyValuesHolder))->SetupStartValue(target);
        }
    }

    return NOERROR;
}

ECode ObjectAnimator::SetupEndValues()
{
    InitAnimation();

    AutoPtr<IInterface> target;
    GetTarget((IInterface**)&target);
    if (target != NULL) {
        Int32 numValues = mValues->GetLength();
        for (Int32 i = 0; i < numValues; ++i) {
            AutoPtr<IPropertyValuesHolder> holder = (*mValues)[i];
            reinterpret_cast<PropertyValuesHolder*>(holder->Probe(EIID_PropertyValuesHolder))->SetupEndValue(target);
        }
    }

    return NOERROR;
}

ECode ObjectAnimator::AnimateValue(
    /* [in] */ Float fraction)
{
    AutoPtr<IInterface> target;
    GetTarget((IInterface**)&target);
    if (mTarget != NULL && target == NULL) {
        // We lost the target reference, cancel and clean up.
        Cancel();
        return NOERROR;
    }

    ValueAnimator::AnimateValue(fraction);
    Int32 numValues = mValues->GetLength();
    for (Int32 i = 0; i < numValues; ++i) {
        AutoPtr<IPropertyValuesHolder> holder = (*mValues)[i];
        PropertyValuesHolder* ptr = reinterpret_cast<PropertyValuesHolder*>(holder->Probe(EIID_PropertyValuesHolder));
        ptr->SetAnimatedValue(target);
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
    REFCOUNT_ADD(*object);
    return NOERROR;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfInt32(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->constructor(target, propertyName);
    anim->SetInt32Values(values);
    return anim;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfInt32(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->constructor(target, property);
    anim->SetInt32Values(values);
    return anim;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfInt32(
    /* [in] */ IObject* target,
    /* [in] */ const String& xPropertyName,
    /* [in] */ const String& yPropertyName,
    /* [in] */ IPath* path)
{
    AutoPtr<IPathKeyframes> keyframes = KeyframeSet::OfPath(path);
    AutoPtr<IPropertyValuesHolder> x = PropertyValuesHolder::OfKeyframes(xPropertyName,
            keyframes->CreateXInt32Keyframes());
    AutoPtr<IPropertyValuesHolder> y = PropertyValuesHolder::OfKeyframes(yPropertyName,
            keyframes->CreateYInt32Keyframes());
    return OfPropertyValuesHolder(target, x, y);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfInt32(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* xProperty,
    /* [in] */ IProperty yProperty,
    /* [in] */ IPath* path)
{
    PathKeyframes keyframes = KeyframeSet.ofPath(path);
    PropertyValuesHolder x = PropertyValuesHolder.ofKeyframes(xProperty,
            keyframes->CreateXInt32Keyframes());
    PropertyValuesHolder y = PropertyValuesHolder.ofKeyframes(yProperty,
            keyframes->CreateYInt32Keyframes());
    return ofPropertyValuesHolder(target, x, y);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfMultiInt32(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<ArrayOf<Int32>* > values)
{
    AutoPtr<IPropertyValuesHolder> pvh = PropertyValuesHolder::OfMultiInt32(propertyName, values);
    return OfPropertyValuesHolder(target, pvh);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfMultiInt32(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ IPath* path)
{
    AutoPtr<IPropertyValuesHolder> pvh = PropertyValuesHolder::OfMultiInt32(propertyName, path);
    return OfPropertyValuesHolder(target, pvh);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfMultiInt32(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ITypeConverter* converter,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<IPropertyValuesHolder> pvh = PropertyValuesHolder::OfMultiInt32(propertyName, converter,
            evaluator, values);
    return ObjectAnimator::OfPropertyValuesHolder(target, pvh);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfArgb(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<IObjectAnimator> animator = OfInt32(target, propertyName, values);
    animator->SetEvaluator(CArgbEvaluator::GetInstance());
    return animator;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfArgb(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<IObjectAnimator> animator = OfInt32(target, property, values);
    animator->SetEvaluator(CArgbEvaluator::GetInstance());
    return animator;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ const String& xPropertyName,
    /* [in] */ const String& yPropertyName,
    /* [in] */ IPath* path)
{
    PathKeyframes keyframes = KeyframeSet.ofPath(path);
    PropertyValuesHolder x = PropertyValuesHolder.ofKeyframes(xPropertyName,
            keyframes.createXFloatKeyframes());
    PropertyValuesHolder y = PropertyValuesHolder.ofKeyframes(yPropertyName,
            keyframes.createYFloatKeyframes());
    return OfPropertyValuesHolder(target, x, y);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* xProperty,
    /* [in] */ IProperty* yProperty,
    /* [in] */ IPath* path)
{
    PathKeyframes keyframes = KeyframeSet.ofPath(path);
    PropertyValuesHolder x = PropertyValuesHolder.ofKeyframes(xProperty,
            keyframes.createXFloatKeyframes());
    PropertyValuesHolder y = PropertyValuesHolder.ofKeyframes(yProperty,
            keyframes.createYFloatKeyframes());
    return OfPropertyValuesHolder(target, x, y);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfMultiFloat(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<ArrayOf<Float>*>* values)
{
    AutoPtr<IPropertyValuesHolder> pvh = PropertyValuesHolder::OfMultiFloat(propertyName, values);
    return OfPropertyValuesHolder(target, pvh);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfMultiFloat(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ IPath* path)
{
    AutoPtr<IPropertyValuesHolder> pvh = PropertyValuesHolder::OfMultiFloat(propertyName, path);
    return OfPropertyValuesHolder(target, pvh);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfMultiFloat(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ITypeConverter* converter,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<IPropertyValuesHolder> pvh = PropertyValuesHolder::OfMultiFloat(propertyName, converter,
            evaluator, values);
    return ObjectAnimator::OfPropertyValuesHolder(target, pvh);
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Float>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->constructor(target, propertyName);
    anim->SetFloatValues(values);
    return anim;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfFloat(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Float>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->constructor(target, property);
    anim->SetFloatValues(values);
    return anim;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfObject(
    /* [in] */ IInterface* target,
    /* [in] */ const String& propertyName,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->constructor(target, propertyName);
    anim->SetObjectValues(values);
    anim->SetEvaluator(evaluator);
    return anim;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfObject(
    /* [in] */ IInterface* target,
    /* [in] */ IProperty* property,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<CObjectAnimator> anim;
    CObjectAnimator::NewByFriend((CObjectAnimator**)&anim);
    anim->constructor(target, property);
    anim->SetObjectValues(values);
    anim->SetEvaluator(evaluator);
    return anim;
}

AutoPtr<IObjectAnimator> ObjectAnimator::OfPropertyValuesHolder(
    /* [in] */ IInterface* target,
    /* [in] */ ArrayOf<IPropertyValuesHolder*>* values)
{
    AutoPtr<IObjectAnimator> anim;
    CObjectAnimator::New((IObjectAnimator**)&anim);
    anim->SetTarget(target);
    anim->SetValues(values);
    return anim;
}

ECode ObjectAnimator::SetAutoCancel(
    /* [in] */ Boolean cancel)
{
    mAutoCancel = cancel;
    return NOERROR;
}

Boolean ObjectAnimator::HasSameTargetAndProperties(
    /* [in] */ /*@Nullable*/ IAnimator* anim)
{
    if (IObjectAnimator::Probe(anim)) {
        PropertyValuesHolder[] theirValues = ((ObjectAnimator) anim).getValues();
        if (((ObjectAnimator) anim).getTarget() == getTarget() &&
                mValues.length == theirValues.length) {
            for (int i = 0; i < mValues.length; ++i) {
                PropertyValuesHolder pvhMine = mValues[i];
                PropertyValuesHolder pvhTheirs = theirValues[i];
                if (pvhMine.getPropertyName() == null ||
                        !pvhMine.getPropertyName().equals(pvhTheirs.getPropertyName())) {
                    return FALSE;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

ECode ObjectAnimator::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    Int32 hashCode = 0;
    GetHashCode(&hashCode);
    AutoPtr<IInterface> t;
    GetTarget((IInterface**)&t);
    String returnVal = String("ObjectAnimator@") + StringUtils::ToHexString(hashCode) + String(", target ") +
        StringUtils::ToHexString(t.Get());
    if (mValues != NULL) {
        for (Int32 i = 0; i < mValues->GetLength(); ++i) {
            String v;
            (*mValues)[i]->ToString(&v);
            returnVal += String("\n    ") + v;
        }
    }
    *str = returnVal;
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
