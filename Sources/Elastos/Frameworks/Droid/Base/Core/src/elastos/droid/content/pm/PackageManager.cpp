
#include "elastos/droid/ext/frameworkext.h"
#include "content/pm/PackageManager.h"
#include "content/pm/PackageParser.h"
#include <elastos/utility/logging/Slogger.h>
#ifdef DROID_CORE
#include "os/CUserHandleHelper.h"
#include "os/CEnvironment.h"
#include "utility/CDisplayMetrics.h"
#endif

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::CEnvironment;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::IO::IFile;
using Elastos::IO::CFile;


static const String TAG("PackageManager");

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode PackageManager::GetPackageArchiveInfo(
    /* [in] */ const String& archiveFilePath,
    /* [in] */ Int32 flags,
    /* [out] */ IPackageInfo** pkgInfo)
{
    VALIDATE_NOT_NULL(pkgInfo)
    AutoPtr<PackageParser> packageParser = new PackageParser(archiveFilePath);
    AutoPtr<IDisplayMetrics> metrics;
    CDisplayMetrics::New((IDisplayMetrics**)&metrics);
    metrics->SetToDefaults();
    AutoPtr<IFile> sourceFile;
    CFile::New(archiveFilePath, (IFile**)&sourceFile);
    AutoPtr<PackageParser::Package> pkg = packageParser->ParsePackage(
             sourceFile, archiveFilePath, metrics, 0);
    if (pkg == NULL) {
        *pkgInfo = NULL;
        return NOERROR;
    }
    if ((flags & IPackageManager::GET_SIGNATURES) != 0) {
        AutoPtr<ArrayOf<Byte> > readBuffer = ArrayOf<Byte>::Alloc(PackageParser::CERTIFICATE_BUFFER_SIZE);
        packageParser->CollectCertificates(pkg, 0, readBuffer);
    }
    AutoPtr<PackageUserState> state = new PackageUserState();
    AutoPtr<IPackageInfo> temp = PackageParser::GeneratePackageInfo(pkg, NULL, flags, 0LL, 0LL, NULL, state);
    *pkgInfo = temp;
    REFCOUNT_ADD(*pkgInfo)
    return NOERROR;
}

ECode PackageManager::GetPackageSizeInfo(
    /* [in] */ const String&  packageName,
    /* [in] */ IPackageStatsObserver* observer)
{
    AutoPtr<IUserHandleHelper> helper;
    Int32 id;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&id);
    return GetPackageSizeInfo(packageName, id, observer);
}

ECode PackageManager::AddPreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ ArrayOf<IComponentName*>* set,
    /* [in] */ IComponentName* activity,
    /* [in] */ Int32 userId)
{
    Slogger::E(TAG, "Not implemented. Must override in a subclass.");
    return E_RUNTIME_EXCEPTION;
}

String PackageManager::GetDataDirForUser(
    /* [in] */ Int32 userId,
    /* [in] */ const String& packageName)
{
    // TODO: This should be shared with Installer's knowledge of user directory
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> file;
    env->GetDataDirectory((IFile**)&file);
    String strFile;
    file->ToString(&strFile);
    strFile.AppendFormat("/user/%d/%s", userId, packageName.string());
    return strFile;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
