
#include "cmdef.h"
#include "CPathClassLoader.h"

namespace Elastos {
namespace Core {

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

    String sname("C");
    sname += className.Substring(className.LastIndexOf('.') + 1);
    // ALOGD("CPathClassLoader::LoadClass: %s, className: %s mPath : %s\n",
    //     sname.string(), className.string(), mPath.string());
    AutoPtr<IModuleInfo> moduleInfo;
    if (sname == "CFragmentOne") {
        FAIL_RETURN(CReflector::AcquireModuleInfo("FragmentDemo.eco", (IModuleInfo**)&moduleInfo));
    }
    else {
        FAIL_RETURN(CReflector::AcquireModuleInfo(mPath, (IModuleInfo**)&moduleInfo));
    }

    return moduleInfo->GetClassInfo(sname, klass);
}

} // namespace Core
} // namespace Elastos
