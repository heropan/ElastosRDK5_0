#include "animation/Int32PropertyValuesHolder.h"

using Elastos::Core::ECLSID_CInteger32;

namespace Elastos {
namespace Droid {
namespace Animation {

Int32PropertyValuesHolder::ClassMethodMap Int32PropertyValuesHolder::sJNISetterPropertyMap;

Int32PropertyValuesHolder::Int32PropertyValuesHolder(
    /* [in] */ const String& propertyName,
    /* [in] */ IInt32KeyframeSet* keyframeSet)
    : PropertyValuesHolder(propertyName)
{
    mValueType = ECLSID_CInteger32;
    mKeyframeSet = keyframeSet;
    mInt32KeyframeSet = keyframeSet;
}

Int32PropertyValuesHolder::Int32PropertyValuesHolder(
    /* [in] */ IProperty* property,
    /* [in] */ IInt32KeyframeSet* keyframeSet)
    : PropertyValuesHolder(property)
{
    mValueType = ECLSID_CInteger32;
    mKeyframeSet = keyframeSet;
    mInt32KeyframeSet = keyframeSet;
}

Int32PropertyValuesHolder::Int32PropertyValuesHolder(
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Int32>* values)
    : PropertyValuesHolder(propertyName)
{
    SetInt32Values(values);
}

Int32PropertyValuesHolder::Int32PropertyValuesHolder(
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Int32>* values)
    : PropertyValuesHolder(property)
{
    SetInt32Values(values);
   if(property->Probe(EIID_IInt32Property))
   {
       mInt32Property = IInt32Property::Probe(mProperty);
   }
}

UInt32 Int32PropertyValuesHolder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Int32PropertyValuesHolder::Release()
{
    return ElRefBase::Release();
}

ECode Int32PropertyValuesHolder::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IInterface*)(IInt32PropertyValuesHolder *)this) {
        *pIID = EIID_IInt32PropertyValuesHolder;
    } else if (object == reinterpret_cast<PInterface>((PropertyValuesHolder*)this)) {
        *pIID = EIID_PropertyValuesHolder;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

PInterface Int32PropertyValuesHolder::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_PropertyValuesHolder) {
        return reinterpret_cast<PInterface>((PropertyValuesHolder*)this);
    } else if (riid == EIID_IInterface) {
        return (IInterface*)this;
    } else if (riid == EIID_IPropertyValuesHolder) {
        return (IPropertyValuesHolder*)this;
    } else if (riid == EIID_IInt32PropertyValuesHolder) {
        return (IInt32PropertyValuesHolder*)this;
    }  else {
        return NULL;
    }
}

ECode Int32PropertyValuesHolder::SetInt32Values(
    /* [in] */ ArrayOf<Int32>* values)
{
    PropertyValuesHolder::SetInt32Values(values);
    mInt32KeyframeSet = (IInt32KeyframeSet*)(mKeyframeSet->Probe(EIID_IInt32KeyframeSet));
    return NOERROR;
}

ECode Int32PropertyValuesHolder::CalculateValue(
    /* [in] */ Float fraction)
{
    mInt32KeyframeSet->GetIntValue(fraction, &mInt32AnimatedValue);
    return NOERROR;
}

ECode Int32PropertyValuesHolder::GetAnimatedValue(
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IInteger32> rst;
    CInteger32::New(mInt32AnimatedValue, (IInteger32**)&rst);
    *value = rst;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode Int32PropertyValuesHolder::Clone(
    /* [out] */ IPropertyValuesHolder** holder)
{
    AutoPtr<Int32PropertyValuesHolder> v = new Int32PropertyValuesHolder(mPropertyName, mInt32KeyframeSet);
    CloneSuperData(v);
    v->mJniSetter = mJniSetter;
    v->mInt32KeyframeSet = mInt32KeyframeSet;
    v->mInt32AnimatedValue = mInt32AnimatedValue;
    *holder = v;
    REFCOUNT_ADD(*holder);

    return NOERROR;
}

ECode Int32PropertyValuesHolder::SetAnimatedValue(
    /* [in] */ IInterface* target)
{
    if (mInt32Property != NULL) {
        return mInt32Property->SetValue(target, mInt32AnimatedValue);
    }

    AutoPtr<IInterface> animatedValue;
    GetAnimatedValue((IInterface**)&animatedValue);
    if (mProperty != NULL) {
        mProperty->Set(target, animatedValue);
        return NOERROR;
    }

    if (mJniSetter != NULL) {
        AutoPtr<IArgumentList> args;
        mJniSetter->CreateArgumentList((IArgumentList**)&args);
        AutoPtr<IFunctionInfo> funcInfo;
        args->GetFunctionInfo((IFunctionInfo**)&funcInfo);
        AutoPtr<IParamInfo> paramInfo;
        funcInfo->GetParamInfoByIndex(0, (IParamInfo**)&paramInfo);
        AutoPtr<IDataTypeInfo> dataTypeInfo;
        paramInfo->GetTypeInfo((IDataTypeInfo**)&dataTypeInfo);
        CarDataType carType;
        dataTypeInfo->GetDataType(&carType);
        if(carType == CarDataType_Int32)
        {
            args->SetInputArgumentOfInt32(0, mInt32AnimatedValue);
        } else if(carType == CarDataType_Interface){
            args->SetInputArgumentOfObjectPtr(0, animatedValue);
        } else {
            assert(0);
        }
        mJniSetter->Invoke(target, args);
        return NOERROR;
    }
    if (mSetter != NULL) {
        AutoPtr<IArgumentList> args;
        mSetter->CreateArgumentList((IArgumentList**)&args);
        AutoPtr<IFunctionInfo> funcInfo;
        args->GetFunctionInfo((IFunctionInfo**)&funcInfo);
        AutoPtr<IParamInfo> paramInfo;
        funcInfo->GetParamInfoByIndex(0, (IParamInfo**)&paramInfo);
        AutoPtr<IDataTypeInfo> dataTypeInfo;
        paramInfo->GetTypeInfo((IDataTypeInfo**)&dataTypeInfo);
        CarDataType carType;
        dataTypeInfo->GetDataType(&carType);
        if(carType == CarDataType_Int32)
        {
            args->SetInputArgumentOfInt32(0, mInt32AnimatedValue);
        } else if(carType == CarDataType_Interface){
            args->SetInputArgumentOfObjectPtr(0, animatedValue);
        } else {
            assert(0);
        }
        mSetter->Invoke(target, args);

        return NOERROR;
    }
    return NOERROR;
}

ECode Int32PropertyValuesHolder::SetupSetter(
        /* [in] */ IInterface* target)
{
    if (mProperty != NULL) {
        return NOERROR;
    }
    // Check new static hashmap<propName, int> for setter method
    {
        AutoLock lock(mPropertyMapLock);
        AutoPtr<IClassInfo> clInfo = TransformClassInfo(target);
        ClassMethodMapIterator exit = sJNISetterPropertyMap.Find(clInfo);
        AutoPtr<MethodMap> propertyMap = NULL;
        if(exit != sJNISetterPropertyMap.End())
        {
            propertyMap = exit->mSecond;
            if(propertyMap != NULL)
            {
                MethodMapIterator it = propertyMap->Find(mPropertyName);
                if(it != propertyMap->End())
                {
                    AutoPtr<IMethodInfo> mtInfo = it->mSecond;
                    if(mtInfo != NULL)
                    {
                        mJniSetter = mtInfo;
                    }
                }
            }
        }
        if(mJniSetter == NULL)
        {
            String methodName = GetMethodName(String("Set"), mPropertyName);
            clInfo->GetMethodInfo(methodName, (IMethodInfo**)&mJniSetter);
            if(mJniSetter != NULL) {
                if (propertyMap == NULL) {
                    propertyMap = new MethodMap();
                    sJNISetterPropertyMap[clInfo] = propertyMap;
                }
                (*propertyMap)[mPropertyName] = mJniSetter;
            }
        }

    }
    if (mJniSetter == NULL) {
        // Couldn't find method through fast JNI approach - just use reflection
        return PropertyValuesHolder::SetupSetter(target);
    }
    return NOERROR;
}

ECode Int32PropertyValuesHolder::SetFloatValues(
    /* [in] */ ArrayOf<Float>* values)
{
    return PropertyValuesHolder::SetFloatValues(values);
}

ECode Int32PropertyValuesHolder::SetKeyframes(
    /* [in] */ ArrayOf<IKeyframe*>* values)
{
    return PropertyValuesHolder::SetKeyframes(values);
}

ECode Int32PropertyValuesHolder::SetObjectValues(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    return PropertyValuesHolder::SetObjectValues(values);
}

ECode Int32PropertyValuesHolder::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    return PropertyValuesHolder::SetEvaluator(evaluator);
}

ECode Int32PropertyValuesHolder::SetPropertyName(
    /* [in] */ const String& propertyName)
{
    return PropertyValuesHolder::SetPropertyName(propertyName);
}

ECode Int32PropertyValuesHolder::SetProperty(
    /* [in] */ IProperty* property)
{
    return PropertyValuesHolder::SetProperty(property);
}

ECode Int32PropertyValuesHolder::GetPropertyName(
    /* [out] */ String* name)
{
    return PropertyValuesHolder::GetPropertyName(name);
}

} // namespace Animation
} // namepsace Droid
} // namespace Elastos
