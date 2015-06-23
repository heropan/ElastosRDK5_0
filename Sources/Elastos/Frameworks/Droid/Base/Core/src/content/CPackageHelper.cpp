
#include "content/CPackageHelper.h"
#include <ext/frameworkext.h>
#include <elastos/utility/logging/Logger.h>
#include "os/CServiceManager.h"

using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::IServiceManager;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::CIoUtils;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::IIoUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Zip::CZipEntry;
using Elastos::Utility::Zip::CZipFile;
using Elastos::Utility::Zip::CZipOutputStream;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

ECode CPackageHelper::GetMountService(
    /* [out] */ IMountService** mountService)
{
    return PackageHelper::GetMountService(mountService);
}

ECode CPackageHelper::CreateSdDir(
    /* [in] */ Int32 sizeMb,
    /* [in] */ const String& cid,
    /* [in] */ const String& sdEncKey,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isExternal,
    /* [out] */ String* dir)
{
    VALIDATE_NOT_NULL(dir)
    *dir = PackageHelper::CreateSdDir(sizeMb, cid, sdEncKey, uid, isExternal);
    return NOERROR;
}

ECode CPackageHelper::MountSdDir(
    /* [in] */ const String& cid,
    /* [in] */ const String& key,
    /* [in] */ Int32 ownerUid,
    /* [out] */ String* dir)
{
    VALIDATE_NOT_NULL(dir)
    *dir = PackageHelper::MountSdDir(cid, key, ownerUid);
    return NOERROR;
}

ECode CPackageHelper::UnMountSdDir(
    /* [in] */ const String& cid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = PackageHelper::UnMountSdDir(cid);
    return NOERROR;
}

ECode CPackageHelper::RenameSdDir(
    /* [in] */ const String& oldId,
    /* [in] */ const String& newId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = PackageHelper::RenameSdDir(oldId, newId);
    return NOERROR;
}

ECode CPackageHelper::GetSdDir(
    /* [in] */ const String& cid,
    /* [out] */ String* dir)
{
    VALIDATE_NOT_NULL(dir)
    *dir = PackageHelper::GetSdDir(cid);
    return NOERROR;
}

ECode CPackageHelper::GetSdFilesystem(
    /* [in] */ const String& cid,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = PackageHelper::GetSdFilesystem(cid);
    return NOERROR;
}

ECode CPackageHelper::FinalizeSdDir(
    /* [in] */ const String& cid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = PackageHelper::FinalizeSdDir(cid);
    return NOERROR;
}

ECode CPackageHelper::DestroySdDir(
    /* [in] */ const String& cid,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = PackageHelper::DestroySdDir(cid);
    return NOERROR;
}

ECode CPackageHelper::GetSecureContainerList(
    /* [out, callee] */ ArrayOf<String>** ids)
{
    VALIDATE_NOT_NULL(ids)
    AutoPtr< ArrayOf<String> > list = PackageHelper::GetSecureContainerList();
    *ids = list;
    REFCOUNT_ADD(*ids);
    return NOERROR;
}

ECode CPackageHelper::IsContainerMounted(
    /* [in] */ const String& cid,
    /* [out] */ Boolean* isMounted)
{
    VALIDATE_NOT_NULL(isMounted)
    *isMounted = PackageHelper::IsContainerMounted(cid);
    return NOERROR;
}

ECode CPackageHelper::ExtractPublicFiles(
    /* [in] */ const String& packagePath,
    /* [in] */ IFile* publicZipFile,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = PackageHelper::ExtractPublicFiles(packagePath, publicZipFile);
    return NOERROR;
}

ECode CPackageHelper::CopyZipEntry(
    /* [in] */ IZipEntry* zipEntry,
    /* [in] */ IZipFile* inZipFile,
    /* [in] */ IZipOutputStream* outZipStream)
{
    return PackageHelper::CopyZipEntry(zipEntry, inZipFile, outZipStream);
}

ECode CPackageHelper::FixSdPermissions(
    /* [in] */ const String& cid,
    /* [in] */ Int32 gid,
    /* [in] */ const String& filename,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = PackageHelper::FixSdPermissions(cid, gid, filename);
    return NOERROR;
}

} // namespace Content
} // namespace Internal
} // namespace Droid
} // namespace Elastos
