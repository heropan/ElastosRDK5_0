
#include "os/Environment.h"
#include "os/SystemProperties.h"
#include "os/ServiceManager.h"
#include "os/storage/CStorageManagerHelper.h"
#include "content/res/CResourcesHelper.h"
#include "R.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Utility::Logging::Logger;
using Elastos::IO::CFile;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Os::Storage::IStorageManager;
using Elastos::Droid::Os::Storage::IStorageManagerHelper;
using Elastos::Droid::Os::Storage::CStorageManagerHelper;

namespace Elastos {
namespace Droid {
namespace Os {

static const String TAG("Environment");
const String Environment::ENV_EXTERNAL_STORAGE("EXTERNAL_STORAGE");
const String Environment::ENV_EMULATED_STORAGE_SOURCE("EMULATED_STORAGE_SOURCE");
const String Environment::SYSTEM_PROPERTY_EFS_ENABLED("persist.security.efs.enabled");
AutoPtr<IFile> Environment::ROOT_DIRECTORY;
AutoPtr<IMountService> Environment::mMntSvc;
AutoPtr<IStorageVolume> Environment::sPrimaryVolume;
volatile Boolean Environment::sIsStorageVolumeInited;
Mutex Environment::sLock;
AutoPtr<IFile> Environment::DATA_DIRECTORY;
AutoPtr<IFile> Environment::SECURE_DATA_DIRECTORY;
AutoPtr<IFile> Environment::EXTERNAL_STORAGE_DIRECTORY;
AutoPtr<IFile> Environment::EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY;
AutoPtr<IFile> Environment::EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY;
AutoPtr<IFile> Environment::EXTERNAL_STORAGE_ELASTOS_OBB_DIRECTORY;
AutoPtr<IFile> Environment::DOWNLOAD_CACHE_DIRECTORY;

Boolean Environment::sIsInitilized = FALSE;

AutoPtr<IFile> Environment::GetRootDirectory()
{
    Init();
    return ROOT_DIRECTORY;
}

AutoPtr<IFile> Environment::GetSystemSecureDirectory()
{
    Init();
    AutoPtr<IFile> file;
    if (IsEncryptedFilesystemEnabled()) {
        CFile::New(SECURE_DATA_DIRECTORY, String("system"), (IFile**)&file);
    }
    else {
        CFile::New(DATA_DIRECTORY, String("system"), (IFile**)&file);
    }
    return file;
}

AutoPtr<IFile> Environment::GetSecureDataDirectory()
{
    Init();
    if (IsEncryptedFilesystemEnabled()) {
        return SECURE_DATA_DIRECTORY;
    }
    else {
        return DATA_DIRECTORY;
    }
}

AutoPtr<IFile> Environment::GetUserSystemDirectory(
    /* [in] */ Int32 userId)
{
    AutoPtr<IFile> usersFile;
    CFile::New(GetSystemSecureDirectory(), String("users"), (IFile**)&usersFile);
    AutoPtr<IFile> file;
    CFile::New(usersFile, StringUtils::Int32ToString(userId), (IFile**)&file);
    return file;
}

Boolean Environment::IsEncryptedFilesystemEnabled()
{
    return SystemProperties::GetBoolean(SYSTEM_PROPERTY_EFS_ENABLED, FALSE);
}

AutoPtr<IFile> Environment::GetDataDirectory()
{
    Init();
    return DATA_DIRECTORY;
}

AutoPtr<IFile> Environment::GetExternalStorageDirectory()
{
    Init();
    return EXTERNAL_STORAGE_DIRECTORY;
}

AutoPtr<IFile> Environment::GetLegacyExternalStorageDirectory()
{
    Init();

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String dir;
    system->GetEnv(ENV_EXTERNAL_STORAGE, &dir);

    AutoPtr<IFile> file;
    CFile::New(dir, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetEmulatedStorageSource(
    /* [in] */ Int32 userId)
{
    // /mnt/shell/emulated/0
    Init();

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String dir;
    system->GetEnv(ENV_EMULATED_STORAGE_SOURCE, &dir);

    AutoPtr<IFile> file;
    CFile::New(dir, StringUtils::Int32ToString(userId), (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetEmulatedStorageObbSource()
{
    // /mnt/shell/emulated/obb
    Init();

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String dir;
    system->GetEnv(ENV_EMULATED_STORAGE_SOURCE, &dir);

    AutoPtr<IFile> file;
    CFile::New(dir, String("obb"), (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStoragePublicDirectory(
    /* [in] */ const String& type)
{
    Init();
    AutoPtr<IFile> file;
    CFile::New(GetExternalStorageDirectory(), type, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStorageElastosDataDir()
{
    Init();
    return EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY;
}

AutoPtr<IFile> Environment::GetExternalStorageAppDataDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY, capsuleName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStorageAppMediaDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY, capsuleName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStorageAppObbDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_OBB_DIRECTORY, capsuleName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStorageAppFilesDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file1, file2;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY, capsuleName, (IFile**)&file1);
    CFile::New(file1, String("files"), (IFile**)&file2);
    return file2;
}

AutoPtr<IFile> Environment::GetExternalStorageAppCacheDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file1, file2;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY, capsuleName, (IFile**)&file1);
    CFile::New(file1, String("cache"), (IFile**)&file2);
    return file2;
}

AutoPtr<IFile> Environment::GetDownloadCacheDirectory()
{
    Init();
    return DOWNLOAD_CACHE_DIRECTORY;
}

String Environment::GetExternalStorageState()
{
    Init();

    if (mMntSvc == NULL) {
        mMntSvc = IMountService::Probe(ServiceManager::GetService(String("mount")));
    }

    String str, val;
    GetExternalStorageDirectory()->ToString(&str);
    ECode ec = mMntSvc->GetVolumeState(str, &val);
    if (FAILED(ec)) {
        return IEnvironment::MEDIA_REMOVED;
    }

    return val;
}

AutoPtr<IStorageVolume> Environment::GetPrimaryVolume()
{
    if (!sIsStorageVolumeInited) {
        AutoLock lock(sLock);
        if (!sIsStorageVolumeInited) {
            assert(Environment::sPrimaryVolume.Get() == NULL);
            // try {
            AutoPtr<IMountService> mountService = IMountService::Probe(ServiceManager::GetService(String("mount")));
            if (mountService != NULL) {
                AutoPtr<ArrayOf<IStorageVolume*> > volumes;
                mountService->GetVolumeList((ArrayOf<IStorageVolume*>**)&volumes);
                AutoPtr<IStorageManagerHelper> helper;
                CStorageManagerHelper::AcquireSingleton((IStorageManagerHelper**)&helper);
                helper->GetPrimaryVolume(volumes, (IStorageVolume**)&Environment::sPrimaryVolume);
                sIsStorageVolumeInited = TRUE;
            }
            else {
                Logger::E(TAG, "couldn't talk to MountService");
            }
            // } catch (Exception e) {
            //     Log.e(TAG, "couldn't talk to MountService", e);
            // }
        }
    }
    return Environment::sPrimaryVolume;
}

Boolean Environment::IsExternalStorageRemovable()
{
    Boolean result = FALSE;
    AutoPtr<IStorageVolume> primary = GetPrimaryVolume();
    if (primary != NULL) {
        primary->IsRemovable(&result);
    }
    return result;
}

Boolean Environment::IsExternalStorageEmulated()
{
    Boolean result = FALSE;
    AutoPtr<IStorageVolume> primary = GetPrimaryVolume();
    if (primary != NULL) {
        primary->IsEmulated(&result);
    }
    return result;
}

AutoPtr<IFile> Environment::GetDirectory(
    /* [in] */ const String& variableName,
    /* [in] */ const String& defaultPath)
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String path;
    system->GetEnv(variableName, &path);

    AutoPtr<IFile> file;
    if (path.IsNull()) {
        CFile::New(defaultPath, (IFile**)&file);
    }
    else {
        CFile::New(path, (IFile**)&file);
    }
    return file;
}

void Environment::Init()
{
    if (!sIsInitilized) {
        ROOT_DIRECTORY = GetDirectory(String("ELASTOS_ROOT"), String("/system"));
        DATA_DIRECTORY = GetDirectory(String("ELASTOS_DATA"), String("/data"));
        SECURE_DATA_DIRECTORY = GetDirectory(String("ELASTOS_SECURE_DATA"), String("/data/secure"));
        EXTERNAL_STORAGE_DIRECTORY = GetDirectory(String("EXTERNAL_STORAGE"), String("/sdcard"));

        AutoPtr<IFile> file1, file2, file3;
        file1 = GetDirectory(String("EXTERNAL_STORAGE"), String("/sdcard"));
        CFile::New(file1, String("Elastos"), (IFile**)&file2);
        CFile::New(file2, String("data"), (IFile**)&EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY);

        file1 = GetDirectory(String("EXTERNAL_STORAGE"), String("/sdcard"));
        CFile::New(file1, String("Elastos"), (IFile**)&file3);
        CFile::New(file3, String("media"), (IFile**)&EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY);
        CFile::New(file3, String("obb"), (IFile**)&EXTERNAL_STORAGE_ELASTOS_OBB_DIRECTORY);

        DOWNLOAD_CACHE_DIRECTORY = GetDirectory(String("DOWNLOAD_CACHE"), String("/cache"));

        sIsInitilized = TRUE;
    }
}

ECode Environment::UserEnvironment::Init(
    /* [in] */ Int32 userId)
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    // See storage config details at http://source.android.com/tech/storage/
    String rawExternalStorage, rawEmulatedStorageTarget, rawMediaStorage;
    system->GetEnv(String("EXTERNAL_STORAGE"), &rawExternalStorage);
    system->GetEnv(String("EMULATED_STORAGE_TARGET"), &rawEmulatedStorageTarget);
    system->GetEnv(String("MEDIA_STORAGE"), &rawMediaStorage);

    if (rawMediaStorage.IsNullOrEmpty()) {
        rawMediaStorage = String("/data/media");
    }

    if (!rawEmulatedStorageTarget.IsNullOrEmpty()) {
        // Device has emulated storage; external storage paths should have
        // userId burned into them.
        String rawUserId = StringUtils::Int32ToString(userId);
        AutoPtr<IFile> emulatedBase;
        CFile::New(rawEmulatedStorageTarget, (IFile**)&emulatedBase);
        AutoPtr<IFile> mediaBase;
        CFile::New(rawMediaStorage, (IFile**)&mediaBase);

        // /storage/emulated/0
        mExternalStorage = BuildPath(emulatedBase, rawUserId);
        // /data/media/0
        mMediaStorage = BuildPath(mediaBase, rawUserId);

    } else {
        // Device has physical external storage; use plain paths.
        if (rawExternalStorage.IsNullOrEmpty()) {
            //Logger::W(TAG, String("EXTERNAL_STORAGE undefined; falling back to default"));
            rawExternalStorage = String("/storage/sdcard0");
        }

        // /storage/sdcard0
        CFile::New(rawExternalStorage, (IFile**)&mExternalStorage);
        // /data/media
        CFile::New(rawMediaStorage, (IFile**)&mMediaStorage);
    }

    ArrayOf_<String, 2> array;
    array.Set(0, String("Android")/*DIRECTORY_ANDROID*/);
    array.Set(1, String("obb"));
    mExternalStorageAndroidObb = BuildPath(mExternalStorage, array);
    array.Set(1, String("data"));
    mExternalStorageAndroidData = BuildPath(mExternalStorage, array);
    array.Set(1, String("media"));
    mExternalStorageAndroidMedia = BuildPath(mExternalStorage, array);
    return NOERROR;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageDirectory()
{
    return mExternalStorage;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageObbDirectory()
{
    return mExternalStorageAndroidObb;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStoragePublicDirectory(
    /* [in] */ const String& type)
{
    AutoPtr<IFile> file;
    CFile::New(mExternalStorage, type, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageAndroidDataDir()
{
    return mExternalStorageAndroidData;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageAppDataDirectory(
    /* [in] */ const String& packageName)
{
    AutoPtr<IFile> file;
    CFile::New(mExternalStorageAndroidData, packageName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageAppMediaDirectory(
    /* [in] */ const String& packageName)
{
    AutoPtr<IFile> file;
    CFile::New(mExternalStorageAndroidMedia, packageName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageAppObbDirectory(
    /* [in] */ const String& packageName)
{
    AutoPtr<IFile> file;
    CFile::New(mExternalStorageAndroidObb, packageName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageAppFilesDirectory(
    /* [in] */ const String& packageName)
{
    AutoPtr<IFile> fileTemp;
    CFile::New(mExternalStorageAndroidData, packageName, (IFile**)&fileTemp);
    AutoPtr<IFile> file;
    CFile::New(fileTemp, String("files"), (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageAppCacheDirectory(
    /* [in] */ const String& packageName)
{
    AutoPtr<IFile> fileTemp;
    CFile::New(mExternalStorageAndroidData, packageName, (IFile**)&fileTemp);
    AutoPtr<IFile> file;
    CFile::New(fileTemp, String("cache"), (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::UserEnvironment::GetMediaStorageDirectory()
{
    return mMediaStorage;
}

AutoPtr<IFile> Environment::UserEnvironment::BuildPath(
    /* [in] */ IFile* base,
    /* [in] */ const ArrayOf<String>& segments)
{
    AutoPtr<IFile> cur = base, temp;
    Int32 i;
    String segment;
    for(i = 0; i < segments.GetLength(); i++) {
        segment = segments[i];
        if (cur == NULL) {
            CFile::New(segment, (IFile**)&cur);
        }
        else {
            temp = NULL;
            CFile::New(cur, segment, (IFile**)&temp);
            cur = temp;
        }
    }
    return cur;
}

AutoPtr<IFile> Environment::UserEnvironment::BuildPath(
    /* [in] */ IFile* base,
    /* [in] */ const String& segment)
{
    AutoPtr<IFile> cur;
    if (base == NULL) {
        CFile::New(segment, (IFile**)&cur);
    }
    else {
        CFile::New(base, segment, (IFile**)&cur);
    }
    return cur;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
