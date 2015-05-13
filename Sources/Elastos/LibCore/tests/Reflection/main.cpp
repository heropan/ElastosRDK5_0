
#include <elautoptr.h>
#include <Reflection.h>

using namespace Elastos;
using Elastos::Core::ISystem;

void testGetSet()
{
    PEL("======== testGetSet() ======")

    AutoPtr<IFooBar> test;
    CFooBar::New((IFooBar**)&test);
    test->SetValue0(10);
    test->SetString(String("Elastos"));

    Int32 value0;
    test->GetValue0(&value0);
    String strValue;
    test->GetString(&strValue);

    PFL_EX(" >> value0:%d, String:[%s]", value0, strValue.string());
}

ECode testReflection()
{
    printf("\n");
    PEL("======== testReflection() ======")

    AutoPtr<IModuleInfo> moduleInfo;
    // AutoPtr<IEnumInfo> enumInfo;
    // AutoPtr<IEnumItemInfo> emumItemInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    AutoPtr<IMethodInfo> methodInfo;
    AutoPtr<IArgumentList> argumentList;

    const char* moduleName = "/data/data/com.elastos.runtime/elastos/Reflection.eco";
    const char* klassName = "CFooBar";

    ECode ec = _CReflector_AcquireModuleInfo(moduleName, (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        printf("Acquire \"%s\" module info failed!\n", moduleName);
        return ec;
    }

    ec = moduleInfo->GetClassInfo(klassName, (IClassInfo**)&classInfo);
    if (FAILED(ec)) {
        printf("Acquire \"%s\" class info failed!\n", klassName);
        return ec;
    }

    // Create object
    //
    ec = classInfo->CreateObject((IInterface**)&object);
    if (FAILED(ec)) {
        printf("Create object failed!\n");
        return ec;
    }

    // SetString
    //
    ec = classInfo->GetMethodInfo("SetString", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        printf("Acquire \"SetString\" method info failed!\n");
        return ec;
    }

    ec = methodInfo->CreateArgumentList((IArgumentList**)&argumentList);
    if (FAILED(ec)) {
        printf("Acquire \"SetString\" method info failed!\n");
        return ec;
    }

    String strValue("Elastos");
    argumentList->SetInputArgumentOfString(0, strValue);

    ec = methodInfo->Invoke(object, argumentList);
    if (FAILED(ec)) {
        printf("Invoke \"SetString\" method failed!\n");
        return ec;
    }

    // GetString
    //
    methodInfo = NULL;
    ec = classInfo->GetMethodInfo("GetString", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        printf("Acquire \"GetString\" method info failed!\n");
        return ec;
    }

    argumentList = NULL;
    ec = methodInfo->CreateArgumentList((IArgumentList**)&argumentList);
    if (FAILED(ec)) {
        printf("Acquire \"GetString\" method info failed!\n");
        return ec;
    }

    String result;
    argumentList->SetOutputArgumentOfStringPtr(0, &result);

    ec = methodInfo->Invoke(object, argumentList);
    if (FAILED(ec)) {
        printf("Invoke \"GetString\" method failed!\n");
        return ec;
    }

    printf(" >>> GetString: [%s]\n", result.string());

    // Validate
    //
    IFooBar* foobar = IFooBar::Probe(object.Get());
    assert(foobar != NULL);
    String respect;
    foobar->GetString(&respect);

    assert(respect == result);

    // SetAllValues
    //
    methodInfo = NULL;
    ec = classInfo->GetMethodInfo("SetAllValues", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        printf("Acquire \"SetAllValues\" method info failed!\n");
        return ec;
    }

    argumentList = NULL;
    ec = methodInfo->CreateArgumentList((IArgumentList**)&argumentList);
    if (FAILED(ec)) {
        printf("Acquire \"SetAllValues\" method info failed!\n");
        return ec;
    }

    strValue = String("-Elastos-");
    argumentList->SetInputArgumentOfInt32(0, 11);
    argumentList->SetInputArgumentOfString(1, strValue);
    argumentList->SetInputArgumentOfInt64(2, 22);

    ec = methodInfo->Invoke(object, argumentList);
    if (FAILED(ec)) {
        printf("Invoke \"SetAllValues\" method failed!\n");
        return ec;
    }

    // GetAllValues
    //
    methodInfo = NULL;
    ec = classInfo->GetMethodInfo("GetAllValues", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        printf("Acquire \"GetAllValues\" method info failed!\n");
        return ec;
    }

    argumentList = NULL;
    ec = methodInfo->CreateArgumentList((IArgumentList**)&argumentList);
    if (FAILED(ec)) {
        printf("Acquire \"GetAllValues\" method info failed!\n");
        return ec;
    }

    String strResult;
    Int32 value0;
    Int64 value1;

    argumentList->SetOutputArgumentOfInt32Ptr(0, &value0);
    argumentList->SetOutputArgumentOfStringPtr(1, &strResult);
    argumentList->SetOutputArgumentOfInt64Ptr(2, &value1);

    ec = methodInfo->Invoke(object, argumentList);
    if (FAILED(ec)) {
        printf("Invoke \"GetAllValues\" method failed!\n");
        return ec;
    }

    printf(" >>> GetAllValues: [%s], value0: %d, value1: %lld\n",
        strResult.string(), value0, value1);
    return NOERROR;
}

int main(int argc, char *argv[])
{
    testGetSet();

    testReflection();
    return 0;
}
