
#include <ext/frameworkdef.h>
#include "droid/test/suitebuilder/CTestMethod.h"

namespace Elastos {
namespace Droid {
namespace Test {
namespace SuiteBuilder {

ECode CTestMethod::constructor(
    /* [in] */ IMethodInfo* method,
    /* [in] */ IClassInfo* enclosingClass)
{
    StringBuf_<512> methodName;
    method->GetName(&methodName);
    return constructor(String((const char*)methodName), enclosingClass);
}

ECode CTestMethod::constructor(
    /* [in] */ const String& methodName,
    /* [in] */ IClassInfo* enclosingClass)
{
    mEnclosingClass = enclosingClass;
    StringBuf_<512> className;
    enclosingClass->GetName(&className);
    mEnclosingClassname = (const char*)className;
    mTestMethodName = methodName;
    return NOERROR;
}

ECode CTestMethod::constructor(
    /* [in] */ ITestCase* testCase)
{
    String name;
    testCase->GetName(&name);
    AutoPtr<IClassInfo> enclosingClass;
    _CObject_ReflectClassInfo(testCase, (IClassInfo**)&enclosingClass);
    return constructor(name, enclosingClass);
}

ECode CTestMethod::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mTestMethodName;
    return NOERROR;
}

ECode CTestMethod::GetEnclosingClassname(
    /* [out] */ String* classname)
{
    VALIDATE_NOT_NULL(classname);
    *classname = mEnclosingClassname;
    return NOERROR;
}

ECode CTestMethod::GetEnclosingClass(
    /* [out] */ IClassInfo** clazz)
{
    VALIDATE_NOT_NULL(clazz);
    *clazz = mEnclosingClass;
    INTERFACE_ADDREF(*clazz);
    return NOERROR;
}

ECode CTestMethod::CreateTest(
    /* [out] */ ITestCase** test)
{
    VALIDATE_NOT_NULL(test);
    return InstantiateTest(mEnclosingClass, mTestMethodName, test);
}

ECode CTestMethod::InstantiateTest(
    /* [in] */ IClassInfo* testCaseClass,
    /* [in] */ const String& testName,
    /* [out] */ ITestCase** test)
{
    Int32 N;
    testCaseClass->GetConstructorCount(&N);
    if (N == 0) {
        // return instantiateTest(testCaseClass.getSuperclass(), testName);
        assert(0);
        return NOERROR;
    }
    else {
        BufferOf<IConstructorInfo*>* constructors = BufferOf<IConstructorInfo*>::Alloc(N);
        testCaseClass->GetAllConstructorInfos(constructors);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IConstructorInfo> constructor = (*constructors)[i];
            Int32 count;
            constructor->GetParamCount(&count);
            BufferOf<IParamInfo*>* params = BufferOf<IParamInfo*>::Alloc(count);
            constructor->GetAllParamInfos(params);
            if (NoargsConstructor(params)) {
                AutoPtr<IInterface> obj;
                FAIL_RETURN(constructor->CreateObject(NULL, (IInterface**)&obj));
                AutoPtr<ITestCase> testCase =  ITestCase::Probe(obj);
                assert(testCase != NULL);
                testCase->SetName(testName);
                *test = testCase;
                INTERFACE_ADDREF(*test);
                return NOERROR;
            }
            else if (SingleStringConstructor(params)) {
                AutoPtr<IArgumentList> args;
                constructor->CreateArgumentList((IArgumentList**)&args);
                args->SetInputArgumentOfString(0, testName);
                AutoPtr<IInterface> obj;
                FAIL_RETURN(constructor->CreateObject(args, (IInterface**)&obj));
                AutoPtr<ITestCase> testCase =  ITestCase::Probe(obj);
                assert(testCase != NULL);
                *test = testCase;
                INTERFACE_ADDREF(*test);
                return NOERROR;
            }
            for (Int32 j = 0; j < count; j++) {
                (*params)[j]->Release();
                (*params)[j] = NULL;
            }
            BufferOf<IParamInfo*>::Free(params);
        }
        for (Int32 i = 0; i < N; i++) {
            (*constructors)[i]->Release();
            (*constructors)[i] = NULL;
        }
        BufferOf<IConstructorInfo*>::Free(constructors);
    }
    // throw new RuntimeException("Unable to locate a constructor for "
    //         + testCaseClass.getName());
    *test = NULL;
    return E_RUNTIME_EXCEPTION;
}

Boolean CTestMethod::SingleStringConstructor(
    /* [in] */ BufferOf<IParamInfo*>* params)
{
    if (params->GetUsed() != 1) return FALSE;
    AutoPtr<IParamInfo> param = (*params)[0];
    AutoPtr<IDataTypeInfo> typeInfo;
    param->GetTypeInfo((IDataTypeInfo**)&typeInfo);
    CarDataType type;
    typeInfo->GetDataType(&type);
    if (type == CarDataType_String) return TRUE;
    else return FALSE;
}

Boolean CTestMethod::NoargsConstructor(
    /* [in] */ BufferOf<IParamInfo*>* params)
{
    return (params->GetUsed() == 0);
}

ECode CTestMethod::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (this->Probe(EIID_IInterface) == obj->Probe(EIID_IInterface)) {
        *result = TRUE;
        return NOERROR;
    }
    if (obj == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    ClassID clsid;
    IObject::Probe(obj)->GetClassID(&clsid);
    if (clsid != ECLSID_CTestMethod) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<ITestMethod> that = ITestMethod::Probe(obj);

    String thatEnclosingClassname;
    that->GetEnclosingClassname(&thatEnclosingClassname);
    if (!mEnclosingClassname.IsNull()
            ? !mEnclosingClassname.Equals(thatEnclosingClassname)
            : !thatEnclosingClassname.IsNull()) {
        *result = FALSE;
        return NOERROR;
    }
    String thatTestMethodName;
    that->GetName(&thatTestMethodName);
    if (!mTestMethodName.IsNull()
            ? !mTestMethodName.Equals(thatTestMethodName)
            : !thatTestMethodName.IsNull()) {
        *result = FALSE;
        return NOERROR;
    }
    *result = TRUE;
    return NOERROR;
}

ECode CTestMethod::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    Int32 result;
    result = (!mEnclosingClassname.IsNull() ? mEnclosingClassname.GetHashCode() : 0);
    result = 31 * result + (!mTestMethodName.IsNull() ? mTestMethodName.GetHashCode() : 0);
    *hashCode = result;
    return NOERROR;
}

ECode CTestMethod::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = mEnclosingClassname + "." + mTestMethodName;
    return NOERROR;
}

} // namespace SuiteBuilder
} // namespace Test
} // namespace Droid
} // namespace Elastos
