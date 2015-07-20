#include "animation/FloatPropertyValuesHolder.h"

using Elastos::Core::ECLSID_CFloat;
using Elastos::Droid::Utility::EIID_IFloatProperty;
using Elastos::Droid::Widget::ECLSID_CButton;

namespace Elastos {
namespace Droid {
namespace Animation {

FloatPropertyValuesHolder::ClassMethodMap FloatPropertyValuesHolder::sJNISetterPropertyMap;

FloatPropertyValuesHolder::FloatPropertyValuesHolder(
    /* [in] */ const String& propertyName,
    /* [in] */ IFloatKeyframeSet* keyframeSet)
{
    PropertyValuesHolder::InitProperty(propertyName);
    mValueType = ECLSID_CFloat;
    mKeyframeSet = keyframeSet;
    mFloatKeyframeSet = keyframeSet;
}

FloatPropertyValuesHolder::FloatPropertyValuesHolder(
    /* [in] */ IProperty* property,
    /* [in] */ IFloatKeyframeSet* keyframeSet)
{
    PropertyValuesHolder::InitProperty(property);
    mValueType = ECLSID_CFloat;
    mKeyframeSet = keyframeSet;
    mFloatKeyframeSet = keyframeSet;
}

FloatPropertyValuesHolder::FloatPropertyValuesHolder(
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Float>* values)
{
    PropertyValuesHolder::InitProperty(propertyName);
    SetFloatValues(values);
}

FloatPropertyValuesHolder::FloatPropertyValuesHolder(
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Float>* values)
{
    PropertyValuesHolder::InitProperty(property);
    SetFloatValues(values);
    if(property->Probe(EIID_IFloatProperty))
    {
        mFloatProperty = IFloatProperty::Probe(mProperty);
    }
}

UInt32 FloatPropertyValuesHolder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FloatPropertyValuesHolder::Release()
{
    return ElRefBase::Release();
}

ECode FloatPropertyValuesHolder::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IInterface*)(IFloatPropertyValuesHolder *)this) {
        *pIID = EIID_IFloatPropertyValuesHolder;
    } else if (object == reinterpret_cast<PInterface>((PropertyValuesHolder*)this)) {
        *pIID = EIID_PropertyValuesHolder;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

PInterface FloatPropertyValuesHolder::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_PropertyValuesHolder) {
        return reinterpret_cast<PInterface>((PropertyValuesHolder*)this);
    } else if (riid == EIID_IInterface) {
        return (IInterface*)this;
    } else if (riid == EIID_IPropertyValuesHolder) {
        return (IPropertyValuesHolder*)this;
    } else if (riid == EIID_IFloatPropertyValuesHolder) {
        return (IFloatPropertyValuesHolder*)this;
    }  else {
        return NULL;
    }
}

ECode FloatPropertyValuesHolder::SetFloatValues(
    /* [in] */ ArrayOf<Float>* values)
{
    PropertyValuesHolder::SetFloatValues(values);
    mFloatKeyframeSet = (IFloatKeyframeSet*)(mKeyframeSet->Probe(EIID_IFloatKeyframeSet));
    return NOERROR;
}

ECode FloatPropertyValuesHolder::CalculateValue(
    /* [in] */ Float fraction)
{
    mFloatKeyframeSet->GetFloatValue(fraction, &mFloatAnimatedValue);
    return NOERROR;
}

AutoPtr<IInterface> FloatPropertyValuesHolder::GetAnimatedValue()
{
    AutoPtr<IFloat> rst;
    CFloat::New(mFloatAnimatedValue, (IFloat**)&rst);
    return rst;
}

ECode FloatPropertyValuesHolder::Clone(
    /* [out] */ IPropertyValuesHolder** holder)
{
    AutoPtr<FloatPropertyValuesHolder> v = new FloatPropertyValuesHolder(mPropertyName, mFloatKeyframeSet);
    CloneSuperData(v);
    v->mJniSetter = mJniSetter;
    v->mFloatKeyframeSet = mFloatKeyframeSet;
    v->mFloatAnimatedValue = mFloatAnimatedValue;
    *holder = v;
    REFCOUNT_ADD(*holder);

    return NOERROR;
}

ECode FloatPropertyValuesHolder::SetAnimatedValue(
    /* [in] */ IInterface* target)
{
    if (mFloatProperty != NULL) {
        return mFloatProperty->SetValue(target, mFloatAnimatedValue);
    }

    AutoPtr<IInterface> animatedValue = GetAnimatedValue();
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
        if(carType == CarDataType_Float)
        {
            args->SetInputArgumentOfFloat(0, mFloatAnimatedValue);
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
        if(carType == CarDataType_Float)
        {
            args->SetInputArgumentOfFloat(0, mFloatAnimatedValue);
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

ECode FloatPropertyValuesHolder::SetupSetter(
        /* [in] */ IInterface* target)
{
    if (mProperty != NULL) {
        return NOERROR;
    }
    // Check new static hashmap<propName, int> for setter method
    {
        AutoLock lock(mPropertyMapLock);
        AutoPtr<IClassInfo> clInfo = TransformClassInfo(target);
        ClassID id;
        id.pUunm = (char*)malloc(80);
        clInfo->GetId(&id);
        free(id.pUunm);
        ClassMethodMapIterator exit = sJNISetterPropertyMap.Find(clInfo);
        AutoPtr<MethodMap> propertyMap;
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
            if(mJniSetter != NULL)
            {
                if(propertyMap == NULL)
                {
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

ECode FloatPropertyValuesHolder::SetInt32Values(
    /* [in] */ ArrayOf<Int32>* values)
{
    return PropertyValuesHolder::SetInt32Values(values);
}

ECode FloatPropertyValuesHolder::SetKeyframes(
    /* [in] */ ArrayOf<IKeyframe*>* values)
{
    return PropertyValuesHolder::SetKeyframes(values);
}

ECode FloatPropertyValuesHolder::SetObjectValues(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    return PropertyValuesHolder::SetObjectValues(values);
}

ECode FloatPropertyValuesHolder::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    return PropertyValuesHolder::SetEvaluator(evaluator);
}

ECode FloatPropertyValuesHolder::SetPropertyName(
    /* [in] */ const String& propertyName)
{
    return PropertyValuesHolder::SetPropertyName(propertyName);
}

ECode FloatPropertyValuesHolder::SetProperty(
    /* [in] */ IProperty* property)
{
    return PropertyValuesHolder::SetProperty(property);
}

ECode FloatPropertyValuesHolder::GetPropertyName(
    /* [out] */ String* name)
{
    return PropertyValuesHolder::GetPropertyName(name);
}

} // namespace Animation
} // namepsace Droid
} // namespace Elastos
