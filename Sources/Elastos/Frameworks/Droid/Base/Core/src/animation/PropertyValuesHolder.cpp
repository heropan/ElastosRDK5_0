
#include "animation/PropertyValuesHolder.h"
#include "animation/CInt32Evaluator.h"
#include "animation/CFloatEvaluator.h"
#include "animation/Int32PropertyValuesHolder.h"
#include "animation/FloatPropertyValuesHolder.h"
#include "animation/PropertyValuesHolderImpl.h"
#include <elastos/core/Math.h>

using Elastos::Core::IDouble;
using Elastos::Core::EIID_IDouble;
using Elastos::Core::CDouble;
using Elastos::Core::EIID_IFloat;
using Elastos::Core::EIID_IInteger32;
using Elastos::Core::ECLSID_CFloat;
using Elastos::Core::ECLSID_CInteger32;
using Elastos::Core::ECLSID_CDouble;

namespace Elastos {
namespace Droid {
namespace Animation {

using Elastos::Core::Math;

// {13386499-5ce9-419f-84cb-52f8bb9442cc}
extern "C" const InterfaceID EIID_PropertyValuesHolder =
    { 0x13386499, 0x5ce9, 0x419f, { 0x84, 0xcb, 0x52, 0xf8, 0xbb, 0x94, 0x42, 0xcc } };

static AutoPtr<ITypeEvaluator> InitInt32Evaluator()
{
    AutoPtr<CInt32Evaluator> rst;
    CInt32Evaluator::NewByFriend((CInt32Evaluator**)&rst);
    return rst;
}

static AutoPtr<ITypeEvaluator> InitFloatEvaluator()
{
    AutoPtr<CFloatEvaluator> rst;
    CFloatEvaluator::NewByFriend((CFloatEvaluator**)&rst);
    return rst;
}

AutoPtr<ITypeEvaluator> PropertyValuesHolder::sIntEvaluator = InitInt32Evaluator();
AutoPtr<ITypeEvaluator> PropertyValuesHolder::sFloatEvaluator = InitFloatEvaluator();
AutoPtr< PropertyValuesHolder::ClassMethodMap > PropertyValuesHolder::sSetterPropertyMap = new ClassMethodMap();
AutoPtr< PropertyValuesHolder::ClassMethodMap > PropertyValuesHolder::sGetterPropertyMap = new ClassMethodMap();

PropertyValuesHolder::PropertyValuesHolder()
{
    mTmpValueArray = ArrayOf<IInterface*>::Alloc(1);
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfInt32(
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<IInt32PropertyValuesHolder> holder = new Int32PropertyValuesHolder(propertyName, values);
    return holder;
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfInt32(
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Int32>* values)
{
    AutoPtr<IInt32PropertyValuesHolder> holder = new Int32PropertyValuesHolder(property, values);
    return holder;
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfFloat(
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<Float>* values)
{
    AutoPtr<IFloatPropertyValuesHolder> holder = new FloatPropertyValuesHolder(propertyName, values);
    return holder;
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfFloat(
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<Float>* values)
{
    AutoPtr<IFloatPropertyValuesHolder> holder = new FloatPropertyValuesHolder(property, values);
    return holder;
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfObject(
    /* [in] */ const String& propertyName,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<IPropertyValuesHolder> holder = new PropertyValuesHolderImpl(propertyName);
    holder->SetObjectValues(values);
    holder->SetEvaluator(evaluator);
    return holder;
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfObject(
    /* [in] */ IProperty* property,
    /* [in] */ ITypeEvaluator* evaluator,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<IPropertyValuesHolder> holder = new PropertyValuesHolderImpl(property);
    holder->SetObjectValues(values);
    holder->SetEvaluator(evaluator);
    return holder;
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfKeyframe(
    /* [in] */ const String& propertyName,
    /* [in] */ ArrayOf<IKeyframe*>* values)
{
    AutoPtr<IKeyframeSet> keyframeSet = KeyframeSet::OfKeyframe(values);
    if (keyframeSet->Probe(EIID_IInt32KeyframeSet)) {
        AutoPtr<IInt32PropertyValuesHolder> holder = new Int32PropertyValuesHolder(propertyName, (IInt32KeyframeSet*)(keyframeSet.Get()));
        return holder;
    }
    else if (keyframeSet->Probe(EIID_IFloatKeyframeSet)) {
        AutoPtr<IFloatPropertyValuesHolder> holder = new FloatPropertyValuesHolder(propertyName, (IFloatKeyframeSet*)(keyframeSet.Get()));
        return holder;
    }
    else {
        AutoPtr<IPropertyValuesHolder> pvh = new PropertyValuesHolderImpl(propertyName);
        PropertyValuesHolder* temp= reinterpret_cast<PropertyValuesHolder*>(pvh->Probe(EIID_PropertyValuesHolder));
        temp->mKeyframeSet = keyframeSet;
        ClassID id;
        (*values)[0]->GetType(&id);
        temp->mValueType = id;
        return pvh;
    }
}

AutoPtr<IPropertyValuesHolder> PropertyValuesHolder::OfKeyframe(
    /* [in] */ IProperty* property,
    /* [in] */ ArrayOf<IKeyframe*>* values)
{
    AutoPtr<IKeyframeSet> keyframeSet = KeyframeSet::OfKeyframe(values);
    if (keyframeSet->Probe(EIID_IInt32KeyframeSet)) {
        AutoPtr<IInt32PropertyValuesHolder> holder = new Int32PropertyValuesHolder(property, (IInt32KeyframeSet*)(keyframeSet.Get()));
        return holder;
    }
    else if (keyframeSet->Probe(EIID_IFloatKeyframeSet)) {
        AutoPtr<IFloatPropertyValuesHolder> holder = new FloatPropertyValuesHolder(property, (IFloatKeyframeSet*)(keyframeSet.Get()));
        return holder;
    }
    else {
        AutoPtr<IPropertyValuesHolder> pvh = new PropertyValuesHolderImpl(property);
        PropertyValuesHolder* temp= reinterpret_cast<PropertyValuesHolder*>(pvh->Probe(EIID_PropertyValuesHolder));
        temp->mKeyframeSet = keyframeSet;
        ClassID id;
        (*values)[0]->GetType(&id);
        temp->mValueType = id;
        return pvh;
    }
}

ECode PropertyValuesHolder::SetInt32Values(
    /* [in] */ ArrayOf<Int32>* values)
{
    mValueType = ECLSID_CInteger32;
    mKeyframeSet = KeyframeSet::OfInt32(values);
    return NOERROR;
}

ECode PropertyValuesHolder::SetFloatValues(
    /* [in] */ ArrayOf<Float>* values)
{
    mValueType = ECLSID_CFloat;
    mKeyframeSet = KeyframeSet::OfFloat(values);
    return NOERROR;
}

ECode PropertyValuesHolder::SetKeyframes(
    /* [in] */ ArrayOf<IKeyframe*>* values)
{
    Int32 numKeyframes = values->GetLength();
    AutoPtr<ArrayOf<IKeyframe*> > keyframes = ArrayOf<IKeyframe*>::Alloc(Math::Max(numKeyframes, 2));
    (*values)[0]->GetType(&mValueType);
    for (Int32 i = 0; i < numKeyframes; ++i) {
        keyframes->Set(i, (*values)[i]);
    }
    mKeyframeSet = new KeyframeSet(keyframes);
    return NOERROR;
}

ECode PropertyValuesHolder::SetObjectValues(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    if (IObject::Probe((*values)[0]))
    {
        IObject::Probe((*values)[0])->GetClassID(&mValueType);
    }
    else {
        mValueType = ECLSID_CDummyObject;
    }
    mKeyframeSet = KeyframeSet::OfObject(values);
    return NOERROR;
}

ECode PropertyValuesHolder::SetupSetter(
    /* [in] */ IInterface* targetClass)
{
    AutoPtr<IClassInfo> info = TransformClassInfo(targetClass);
    if (info) {
        mSetter = SetupSetterOrGetter(info, sSetterPropertyMap.Get(), String("Set"));
        return NOERROR;
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode PropertyValuesHolder::SetupGetter(
     /* [in] */ IInterface* target)
{
    AutoPtr<IClassInfo> info = TransformClassInfo(target);
    if (info) {
        mGetter = SetupSetterOrGetter(info, sGetterPropertyMap.Get(), String("Get"));
        return NOERROR;
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode PropertyValuesHolder::SetupSetterAndGetter(
    /* [in] */ IInterface* target)
{
    if (mProperty)
    {
        Boolean writeOnly;
        if(mProperty->IsWriteOnly(&writeOnly), !writeOnly) {
            AutoPtr<IInterface> testValue;
            ASSERT_SUCCEEDED(mProperty->Get(target, (IInterface**)&testValue));
            AutoPtr<ArrayOf<IKeyframe*> > frames;
            mKeyframeSet->GetKeyframes((ArrayOf<IKeyframe*>**)&frames);
            for(Int32 i = 0; i < frames->GetLength(); i++)
            {
                IKeyframe* kf = (*frames)[i];
                Boolean has;
                kf->HasValue(&has);
                if (!has)
                {
                    ASSERT_SUCCEEDED(kf->SetValue(testValue));
                }
            }
        }
        return NOERROR;
    }

    if (mSetter == NULL)
    {
        SetupSetter(target);
    }
    AutoPtr<ArrayOf<IKeyframe*> > frames;
    mKeyframeSet->GetKeyframes((ArrayOf<IKeyframe*>**)&frames);
    for(Int32 i = 0; i < frames->GetLength(); i++)
    {
        IKeyframe* kf = (*frames)[i];
        Boolean has;
        kf->HasValue(&has);
        if (!has)
        {
            if (mGetter == NULL)
            {
                SetupGetter(target);
                if (mGetter == NULL)
                {
                    // Already logged the error - just return to avoid NPE
                    return NOERROR;
                }
            }
            AutoPtr<IArgumentList> arg;
            mGetter->CreateArgumentList((IArgumentList**)&arg);
            AutoPtr<IFunctionInfo> funcInfo;
            arg->GetFunctionInfo((IFunctionInfo**)&funcInfo);
            AutoPtr<IParamInfo> paramInfo;
            funcInfo->GetParamInfoByIndex(0, (IParamInfo**)&paramInfo);
            AutoPtr<IDataTypeInfo> dataTypeInfo;
            paramInfo->GetTypeInfo((IDataTypeInfo**)&dataTypeInfo);
            CarDataType carType;
            dataTypeInfo->GetDataType(&carType);
            if (mValueType == ECLSID_CInteger32) {
                if (carType == CarDataType_Int32) {
                    Int32 getRst;
                    arg->SetOutputArgumentOfInt32Ptr(0, &getRst);
                    FAIL_RETURN(mGetter->Invoke(target, arg));
                    AutoPtr<IInteger32> carRst;
                    CInteger32::New(getRst, (IInteger32**)&carRst);
                    kf->SetValue(carRst);
                }
                else if(carType == CarDataType_Interface) {
                    AutoPtr<IInteger32> getRst;
                    arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
                    FAIL_RETURN(mGetter->Invoke(target, arg));
                    kf->SetValue(getRst);
                }
                else {
                    assert(0);
                }
            }
            else if (mValueType == ECLSID_CFloat) {
                if (carType == CarDataType_Float) {
                    Float getRst;
                    arg->SetOutputArgumentOfFloatPtr(0, &getRst);
                    FAIL_RETURN(mGetter->Invoke(target, arg));
                    AutoPtr<IFloat> carRst;
                    CFloat::New(getRst, (IFloat**)&carRst);
                    kf->SetValue(carRst);
                }
                else if (carType == CarDataType_Interface) {
                    AutoPtr<IFloat> getRst;
                    arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
                    FAIL_RETURN(mGetter->Invoke(target, arg));
                    kf->SetValue(getRst);
                }
                else{
                    assert(0);
                }
            }
            else if (mValueType == ECLSID_CDouble) {
                if(carType == CarDataType_Double) {
                    Double getRst;
                    arg->SetOutputArgumentOfDoublePtr(0, &getRst);
                    FAIL_RETURN(mGetter->Invoke(target, arg));
                    AutoPtr<IDouble> carRst;
                    CDouble::New(getRst, (IDouble**)&carRst);
                    kf->SetValue(carRst);
                }
                else if(carType == CarDataType_Interface) {
                    AutoPtr<IDouble> getRst;
                    arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
                    FAIL_RETURN(mGetter->Invoke(target, arg));
                    kf->SetValue(getRst);
                }
                else{
                    assert(0);
                }
            }
            else {
               AutoPtr<IInterface> getRst;
               arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
               FAIL_RETURN(mGetter->Invoke(target, arg));
               kf->SetValue(getRst);
            }

        }
    }
    return NOERROR;
}

ECode PropertyValuesHolder::SetupValue(
     /* [in] */ IInterface* target,
     /* [in] */ IKeyframe* kf)
{
    if (mProperty)
    {
        AutoPtr<IInterface> value;
        mProperty->Get(target, (IInterface**)&value);
        kf->SetValue(value);
    }
    if (mGetter == NULL)
    {
        SetupGetter(target);
        if (mGetter == NULL)
        {
            // Already logged the error - just return to avoid NPE
            return NOERROR;
        }
    }

    AutoPtr<IArgumentList> arg;
    mGetter->CreateArgumentList((IArgumentList**)&arg);
    AutoPtr<IFunctionInfo> funcInfo;
    arg->GetFunctionInfo((IFunctionInfo**)&funcInfo);
    AutoPtr<IParamInfo> paramInfo;
    funcInfo->GetParamInfoByIndex(0, (IParamInfo**)&paramInfo);
    AutoPtr<IDataTypeInfo> dataTypeInfo;
    paramInfo->GetTypeInfo((IDataTypeInfo**)&dataTypeInfo);
    CarDataType carType;
    dataTypeInfo->GetDataType(&carType);
    if (mValueType == ECLSID_CInteger32)
    {
        if (carType == CarDataType_Int32)
        {
            Int32 getRst;
            arg->SetOutputArgumentOfInt32Ptr(0, &getRst);
            FAIL_RETURN(mGetter->Invoke(target, arg));
            AutoPtr<IInteger32> carRst;
            CInteger32::New(getRst, (IInteger32**)&carRst);
            kf->SetValue(carRst);
        }
        else if (carType == CarDataType_Interface) {
            AutoPtr<IInteger32> getRst;
            arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
            FAIL_RETURN(mGetter->Invoke(target, arg));
            kf->SetValue(getRst);
        }
        else{
            assert(0);
        }
    }
    else if (mValueType == ECLSID_CFloat) {
        if (carType == CarDataType_Float) {
            Float getRst;
            arg->SetOutputArgumentOfFloatPtr(0, &getRst);
            FAIL_RETURN(mGetter->Invoke(target, arg));
            AutoPtr<IFloat> carRst;
            CFloat::New(getRst, (IFloat**)&carRst);
            kf->SetValue(carRst);
        }
        else if (carType == CarDataType_Interface) {
            AutoPtr<IFloat> getRst;
            arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
            FAIL_RETURN(mGetter->Invoke(target, arg));
            kf->SetValue(getRst);
        }
        else{
            assert(0);
        }
    } else if(mValueType == ECLSID_CDouble) {
        if (carType == CarDataType_Double) {
            Double getRst;
            arg->SetOutputArgumentOfDoublePtr(0, &getRst);
            FAIL_RETURN(mGetter->Invoke(target, arg));
            AutoPtr<IDouble> carRst;
            CDouble::New(getRst, (IDouble**)&carRst);
            kf->SetValue(carRst);
        }
        else if (carType == CarDataType_Interface) {
            AutoPtr<IDouble> getRst;
            arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
            FAIL_RETURN(mGetter->Invoke(target, arg));
            kf->SetValue(getRst);
        }
        else{
            assert(0);
        }
    } else {
       AutoPtr<IInterface> getRst;
       arg->SetOutputArgumentOfObjectPtrPtr(0, (IInterface**)&getRst);
       FAIL_RETURN(mGetter->Invoke(target, arg));
       kf->SetValue(getRst);
    }
    return NOERROR;
}

ECode PropertyValuesHolder::SetupStartValue(
    /* [in] */ IInterface* target)
{
    AutoPtr<ArrayOf<IKeyframe*> > frames;
    mKeyframeSet->GetKeyframes((ArrayOf<IKeyframe*>**)&frames);

    return SetupValue(target, (*frames)[0]);
}
ECode PropertyValuesHolder::SetupEndValue(
    /* [in] */ IInterface* target)
{
    AutoPtr<ArrayOf<IKeyframe*> > frames;
    mKeyframeSet->GetKeyframes((ArrayOf<IKeyframe*>**)&frames);
    Int32 length = frames->GetLength();

    return SetupValue(target, (*frames)[length - 1]);
}

ECode PropertyValuesHolder::CloneSuperData(
    /* [in] */ PropertyValuesHolder* holder)
{
    holder->mPropertyName = mPropertyName;
    holder->mProperty = mProperty;
    holder->mSetter = mSetter;
    holder->mValueType = mValueType;
    mKeyframeSet->Clone((IKeyframeSet**)(&(holder->mKeyframeSet)));
    holder->mTmpValueArray = mTmpValueArray;
    holder->mGetter = mGetter;
    holder->mEvaluator = mEvaluator;
    holder->mAnimatedValue = mAnimatedValue;
    return NOERROR;
}

ECode PropertyValuesHolder::SetAnimatedValue(
    /* [in] */ IInterface* target)
{
    AutoPtr<IInterface> animatedValue = GetAnimatedValue();
    if(mProperty)
    {
        if(animatedValue)
            mProperty->Set(target, animatedValue);
    }

    if(mSetter)
    {
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

        if (mValueType == ECLSID_CInteger32)
        {
            if(carType == CarDataType_Int32) {
                Int32 setRst;
                AutoPtr<IInteger32> valTemp = IInteger32::Probe(animatedValue);
                if(!valTemp)
                    return NOERROR;
                valTemp->GetValue(&setRst);
                args->SetInputArgumentOfInt32(0, setRst);
                FAIL_RETURN(mSetter->Invoke(target, args));
            }
            else if(carType == CarDataType_Interface) {
                if(!animatedValue)
                    return NOERROR;
                args->SetInputArgumentOfObjectPtr(0, animatedValue);
                FAIL_RETURN(mSetter->Invoke(target, args));
            }
            else{
                return NOERROR;
            }
        }
        else if(mValueType == ECLSID_CFloat) {
            if (carType == CarDataType_Float)
            {
                Float setRst;
                AutoPtr<IFloat> valTemp = IFloat::Probe(animatedValue);
                if (!valTemp)
                    return NOERROR;
                valTemp->GetValue(&setRst);
                args->SetInputArgumentOfInt32(0, setRst);
                FAIL_RETURN(mSetter->Invoke(target, args));
            }
            else if (carType == CarDataType_Interface) {
                if(!animatedValue)
                    return NOERROR;
                args->SetInputArgumentOfObjectPtr(0, animatedValue);
                FAIL_RETURN(mSetter->Invoke(target, args));
            }
            else{
                return NOERROR;
            }
        }
        else if (mValueType == ECLSID_CDouble) {
            if (carType == CarDataType_Double) {
                Double setRst;
                AutoPtr<IDouble> valTemp = IDouble::Probe(animatedValue);
                if (!valTemp)
                    return NOERROR;
                valTemp->GetValue(&setRst);
                args->SetInputArgumentOfInt32(0, setRst);
                FAIL_RETURN(mSetter->Invoke(target, args));
            }
            else if (carType == CarDataType_Interface) {
                if(!animatedValue)
                    return NOERROR;
                args->SetInputArgumentOfObjectPtr(0, animatedValue);
                FAIL_RETURN(mSetter->Invoke(target, args));
            }
            else {
                return NOERROR;
            }
        }
        else {
            if (!animatedValue)
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            args->SetInputArgumentOfObjectPtr(0, animatedValue);
            FAIL_RETURN(mSetter->Invoke(target, args));
        }
    }
    return NOERROR;
}

ECode PropertyValuesHolder::Init()
{
    if (mEvaluator == NULL) {
        // We already handle int and float automatically, but not their Object
        // equivalents
        mEvaluator = (mValueType == ECLSID_CInteger32) ? sIntEvaluator :
                (mValueType == ECLSID_CFloat) ? sFloatEvaluator :
                NULL;
    }
    if (mEvaluator != NULL) {
        // KeyframeSet knows how to evaluate the common types - only give it a custom
        // evaluator if one has been set on this class
        mKeyframeSet->SetEvaluator(mEvaluator);
    }
    return NOERROR;
}

ECode PropertyValuesHolder::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    mEvaluator = evaluator;
    mKeyframeSet->SetEvaluator(evaluator);
    return NOERROR;
}

ECode PropertyValuesHolder::CalculateValue(
    /* [in] */ Float fraction)
{
    mAnimatedValue = NULL;
    mKeyframeSet->GetValue(fraction, (IInterface**)&mAnimatedValue);
    return NOERROR;
}

ECode PropertyValuesHolder::SetPropertyName(
    /* [in] */ const String& propertyName)
{
    mPropertyName = propertyName;
    return NOERROR;
}

ECode PropertyValuesHolder::SetProperty(
    /* [in] */ IProperty* property)
{
    mProperty = property;
    return NOERROR;
}

ECode PropertyValuesHolder::GetPropertyName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mPropertyName;
    return NOERROR;
}

AutoPtr<IInterface> PropertyValuesHolder::GetAnimatedValue()
{
    return mAnimatedValue;
}

String PropertyValuesHolder::GetMethodName(
    /* [in] */ const String& prefix,
    /* [in] */ const String& propertyName)
{
    if (propertyName.IsNullOrEmpty()) {
        // shouldn't get here
        return prefix;
    }

    String temp = propertyName.ToUpperCase(0, 1);
    return prefix + temp;
}

ECode PropertyValuesHolder::InitProperty(
    /* [in] */ const String& propertyName)
{
    mPropertyName = propertyName;
    return NOERROR;
}

ECode PropertyValuesHolder::InitProperty(
    /* [in] */ IProperty* property)
{
    mProperty = property;
    if (property != NULL) {
       property->GetName(&mPropertyName);
    }
    return NOERROR;
}

AutoPtr<IMethodInfo> PropertyValuesHolder::GetPropertyFunction(
     /* [in] */ IClassInfo* targetClass,
     /* [in] */ const String& prefix)
{
    String methodName = GetMethodName(prefix,mPropertyName);
    AutoPtr<IMethodInfo> returnVal;
    targetClass->GetMethodInfo(methodName, (IMethodInfo**)&returnVal);
    return returnVal;
}

AutoPtr<IMethodInfo> PropertyValuesHolder::SetupSetterOrGetter(
     /* [in] */ IClassInfo* targetClass,
     /* [in] */ PropertyValuesHolder::ClassMethodMap * propertyMapMap,
     /* [in] */ const String& prefix)
{
    AutoPtr<IMethodInfo> setterOrGetter;
    {
        AutoLock lock(mPropertyMapLock);
        AutoPtr<IClassInfo> key = targetClass;
        AutoPtr< MethodMap > propertyMap;
        typename ClassMethodMap::Iterator it = propertyMapMap->Find(key);
        if ((it != propertyMapMap->End()) && (it->mSecond != NULL)) {
            propertyMap = it->mSecond;
            typename MethodMap::Iterator it2 = propertyMap->Find(mPropertyName);
            if ((it2 != propertyMap->End()) && (it2->mSecond != NULL))
            {
                setterOrGetter = it2->mSecond;
            }
        }
        if (setterOrGetter == NULL) {
            setterOrGetter = GetPropertyFunction(targetClass, prefix);
            if (propertyMap == NULL) {
                propertyMap = new MethodMap();
                (*propertyMapMap)[key] = propertyMap;
            }

            (*propertyMap)[mPropertyName] = setterOrGetter;
        }
        return setterOrGetter;
    }
    return NULL;
}


AutoPtr<IClassInfo> PropertyValuesHolder::TransformClassInfo(
    /* [in] */ IInterface* o)
{
    AutoPtr<IObject> obj = IObject::Probe(o);
    assert(obj != NULL);
    ClassID objId;
    obj->GetClassID(&objId);
    AutoPtr<IModuleInfo> moduleInfo;
    String path(objId.pUunm);
    ASSERT_SUCCEEDED(_CReflector_AcquireModuleInfo(
           path, (IModuleInfo**)&moduleInfo));
    Int32 clsCount;
    moduleInfo->GetClassCount(&clsCount);
    BufferOf<IClassInfo*>* buf = BufferOf<IClassInfo*>::Alloc(clsCount);
    moduleInfo->GetAllClassInfos(buf);
    AutoPtr<IClassInfo> info;
    ClassID id;
    id.pUunm = (char*)malloc(80);
    for (Int32 i = 0; i < clsCount; i++) {
        (*buf)[i]->GetId(&id);
        if (id == objId) {
           info = (*buf)[i];
           break;
        }
    }

    free(id.pUunm);
    BufferOf<IClassInfo*>::Free(buf);
    return info;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
