
#ifndef __PACKAGEHELPER_H__
#define __PACKAGEHELPER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::IO::IFile;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::Utility::Zip::IZipFile;
using Elastos::Utility::Zip::IZipOutputStream;

using Elastos::Droid::Os::Storage::IMountService;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

class PackageHelper
{
public:
    static CARAPI GetMountService(
        /* [out] */ IMountService** mountService);

    static CARAPI_(String) CreateSdDir(
        /* [in] */ Int32 sizeMb,
        /* [in] */ const String& cid,
        /* [in] */ const String& sdEncKey,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean isExternal);

    static CARAPI_(String) MountSdDir(
        /* [in] */ const String& cid,
        /* [in] */ const String& key,
        /* [in] */ Int32 ownerUid);

    static CARAPI_(Boolean) UnMountSdDir(
        /* [in] */ const String& cid);

    static CARAPI_(Boolean) RenameSdDir(
        /* [in] */ const String& oldId,
        /* [in] */ const String& newId);

    static CARAPI_(String) GetSdDir(
        /* [in] */ const String& cid);

    static CARAPI_(String) GetSdFilesystem(
        /* [in] */ const String& cid);

    static CARAPI_(Boolean) FinalizeSdDir(
        /* [in] */ const String& cid);

    static CARAPI_(Boolean) DestroySdDir(
        /* [in] */ const String& cid);

    static CARAPI_(AutoPtr< ArrayOf<String> >) GetSecureContainerList();

    static CARAPI_(Boolean) IsContainerMounted(
        /* [in] */ const String& cid);

    static CARAPI_(Int32) ExtractPublicFiles(
        /* [in] */ const String& packagePath,
        /* [in] */ IFile* publicZipFile);

    static CARAPI CopyZipEntry(
        /* [in] */ IZipEntry* zipEntry,
        /* [in] */ IZipFile* inZipFile,
        /* [in] */ IZipOutputStream* outZipStream);

    static CARAPI_(Boolean) FixSdPermissions(
        /* [in] */ const String& cid,
        /* [in] */ Int32 gid,
        /* [in] */ const String& filename);

private:
    static const Boolean localLOGV;
    static const String TAG;
};

} // namespace Content
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__PACKAGEHELPER_H__
