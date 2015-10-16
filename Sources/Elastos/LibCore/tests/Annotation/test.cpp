
#include <elastos.h>
#include <stdio.h>

using namespace Elastos;

int main(int argc, char** argv)
{
    printf("======== testAnnotation() ======\n");

    AutoPtr<IModuleInfo> moduleInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    AutoPtr<IMethodInfo> methodInfo;

    const char* moduleName = "/data/data/com.elastos.runtime/elastos/Annotation.eco";
    const char* klassName = "CAnnotation";

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

    // M4
    //
    ec = classInfo->GetMethodInfo("M4", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        printf("Acquire \"M4\" method info failed!\n");
        return ec;
    }

    String annotation;
    ec = methodInfo->GetAnnotation(&annotation);
    if (FAILED(ec)) {
        printf("Get \"M4\" method annotation failed!\n");
        return ec;
    }

    printf("M4 annotation: %s\n", annotation.string());

    methodInfo = NULL;

    // M5
    //
    ec = classInfo->GetMethodInfo("M5", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        printf("Acquire \"M5\" method info failed!\n");
        return ec;
    }

    ec = methodInfo->GetAnnotation(&annotation);
    if (FAILED(ec)) {
        printf("Get \"M5\" method annotation failed!\n");
        return ec;
    }

    printf("M5 annotation: %s\n", annotation.string());

    return 0;
}