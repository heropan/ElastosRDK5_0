
#ifndef __ELASTOS_DROID_INTERNAL_CONTENT_CPACKAGEHELPER_H__
#define __ELASTOS_DROID_INTERNAL_CONTENT_CPACKAGEHELPER_H__

#include "_CPackageHelper.h"
#include "content/PackageHelper.h"

using Elastos::Droid::Os::Storage::IMountService;
using Elastos::IO::IFile;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::Utility::Zip::IZipFile;
using Elastos::Utility::Zip::IZipOutputStream;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

CarClass(CPackageHelper), public PackageHelper
{
public:
    CARAPI GetMountService(
        /* [out] */ IMountService** mountService);

    CARAPI CreateSdDir(
        /* [in] */ Int32 sizeMb,
        /* [in] */ const String& cid,
        /* [in] */ const String& sdEncKey,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean isExternal,
        /* [out] */ String* dir);

    CARAPI MountSdDir(
        /* [in] */ const String& cid,
        /* [in] */ const String& key,
        /* [in] */ Int32 ownerUid,
        /* [out] */ String* dir);

    CARAPI UnMountSdDir(
        /* [in] */ const String& cid,
        /* [out] */ Boolean* result);

    CARAPI RenameSdDir(
        /* [in] */ const String& oldId,
        /* [in] */ const String& newId,
        /* [out] */ Boolean* result);

    CARAPI GetSdDir(
        /* [in] */ const String& cid,
        /* [out] */ String* dir);

    CARAPI GetSdFilesystem(
        /* [in] */ const String& cid,
        /* [out] */ String* str);

    CARAPI FinalizeSdDir(
        /* [in] */ const String& cid,
        /* [out] */ Boolean* result);

    CARAPI DestroySdDir(
        /* [in] */ const String& cid,
        /* [out] */ Boolean* result);

    CARAPI GetSecureContainerList(
        /* [out, callee] */ ArrayOf<String>** ids);

    CARAPI IsContainerMounted(
        /* [in] */ const String& cid,
        /* [out] */ Boolean* isMounted);

    CARAPI ExtractPublicFiles(
        /* [in] */ const String& packagePath,
        /* [in] */ IFile* publicZipFile,
        /* [out] */ Int32* size);

    CARAPI CopyZipEntry(
        /* [in] */ IZipEntry* zipEntry,
        /* [in] */ IZipFile* inZipFile,
        /* [in] */ IZipOutputStream* outZipStream);

    CARAPI FixSdPermissions(
        /* [in] */ const String& cid,
        /* [in] */ Int32 gid,
        /* [in] */ const String& filename,
        /* [out] */ Boolean* result);
};

} // namespace Content
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_CONTENT_CPACKAGEHELPER_H__
