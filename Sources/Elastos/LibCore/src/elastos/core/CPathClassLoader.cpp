
#include "CPathClassLoader.h"

namespace Elastos {
namespace Core {

CAR_INTERFACE_IMPL(CPathClassLoader, Object, IClassLoader)

CAR_OBJECT_IMPL(CPathClassLoader)

ECode CPathClassLoader::constructor(
    /* [in] */ const String& path)
{
    if (path.IsNullOrEmpty()) {
        return E_INVALID_ARGUMENT;
    }

    mPath = path;
    return NOERROR;
}

ECode CPathClassLoader::LoadClass(
    /* [in] */ const String& className,
    /* [out] */ IClassInfo** klass)
{
    VALIDATE_NOT_NULL(klass);
    *klass = NULL;

    if (className.IsNullOrEmpty()) {
        return E_INVALID_ARGUMENT;
    }

    String sname;
    String name = className.Substring(className.LastIndexOf('.') + 1);
    if (name.GetChar(0) == 'C' && name.GetChar(1) <= 'Z') {
        sname = name;
    } else {
        sname = String("C") + name;
    }
    // ALOGD("CPathClassLoader::LoadClass: %s, className: %s mPath : %s\n",
    //     sname.string(), className.string(), mPath.string());
    AutoPtr<IModuleInfo> moduleInfo;
    if (sname == "CFragmentOne") {
        FAIL_RETURN(CReflector::AcquireModuleInfo(String("FragmentDemo.eco"), (IModuleInfo**)&moduleInfo));
    }
    else {
        FAIL_RETURN(CReflector::AcquireModuleInfo(mPath, (IModuleInfo**)&moduleInfo));
    }

    FAIL_RETURN(moduleInfo->GetClassInfo(sname, klass));
    (*klass)->SetClassLoader((IClassLoader*)this);
    return NOERROR;
}

} // namespace Core
} // namespace Elastos
