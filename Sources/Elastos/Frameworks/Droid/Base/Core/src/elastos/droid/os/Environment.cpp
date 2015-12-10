
#include "Elastos.CoreLibrary.IO.h"
#include "_Elastos.Droid.Core.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Os.h"
#include "elastos/droid/os/Environment.h"
#include "elastos/droid/os/SystemProperties.h"
#include "elastos/droid/os/UserHandle.h"
//#include "elastos/droid/os/FileUtils.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>

//#include "elastos/droid/text/TextUtils.h"

using Elastos::IO::CFile;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;

//using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Os {

const String Environment::MEDIA_UNKNOWN("unknown");
const String Environment::MEDIA_REMOVED("removed");
const String Environment::MEDIA_UNMOUNTED("unmounted");
const String Environment::MEDIA_CHECKING("checking");
const String Environment::MEDIA_NOFS("nofs");
const String Environment::MEDIA_MOUNTED("mounted");
const String Environment::MEDIA_MOUNTED_READ_ONLY("mounted_ro");
const String Environment::MEDIA_SHARED("shared");
const String Environment::MEDIA_BAD_REMOVAL("bad_removal");
const String Environment::MEDIA_UNMOUNTABLE("unmountable");

String Environment::DIRECTORY_MUSIC("Music");
String Environment::DIRECTORY_PODCASTS("Podcasts");
String Environment::DIRECTORY_RINGTONES("Ringtones");
String Environment::DIRECTORY_ALARMS("Alarms");
String Environment::DIRECTORY_NOTIFICATIONS("Notifications");
String Environment::DIRECTORY_PICTURES("Pictures");
String Environment::DIRECTORY_MOVIES("Movies");
String Environment::DIRECTORY_DOWNLOADS("Download");
String Environment::DIRECTORY_DCIM("DCIM");
String Environment::DIRECTORY_DOCUMENTS("Documents");

const String Environment::DIR_ANDROID("Android");
const String Environment::DIRECTORY_ANDROID("Android"); // = DIR_ANDROID;

const String Environment::ENV_EXTERNAL_STORAGE("EXTERNAL_STORAGE");
const String Environment::ENV_EMULATED_STORAGE_SOURCE("EMULATED_STORAGE_SOURCE");
const String Environment::ENV_EMULATED_STORAGE_TARGET("EMULATED_STORAGE_TARGET");
const String Environment::ENV_MEDIA_STORAGE("MEDIA_STORAGE");
const String Environment::ENV_SECONDARY_STORAGE("SECONDARY_STORAGE");
const String Environment::ENV_ANDROID_ROOT("ANDROID_ROOT");
const String Environment::ENV_OEM_ROOT("OEM_ROOT");
const String Environment::ENV_VENDOR_ROOT("VENDOR_ROOT");

const String Environment::DIR_DATA("data");
const String Environment::DIR_MEDIA("media");
const String Environment::DIR_OBB("obb");
const String Environment::DIR_FILES("files");
const String Environment::DIR_CACHE("cache");

AutoPtr<IFile> GetDirectoryImpl(
    /* [in] */ const String& variableName,
    /* [in] */ const String& defaultPath)
{
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);

    String path;
    system->GetEnv(variableName, &path);
    if (path.IsNull()) path = defaultPath;
    AutoPtr<IFile> file;
    CFile::New(path, (IFile**)&file);
    return file;
}

String GetCanonicalPathOrNull(
    /* [in] */ const String& variableName)
{
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);

    String path;
    system->GetEnv(variableName, &path);

    if (path.IsNull()) {
        return path;
    }

    AutoPtr<IFile> file;
    CFile::New(path, (IFile**)&file);
    String cp;
    ECode ec = file->GetCanonicalPath(&cp);
    if (FAILED(ec)) {
        Logger::W("Environment", "Unable to resolve canonical path for %s", path.string());
        return String(NULL);
    }
    return cp;
}

AutoPtr<Environment::UserEnvironment> InitForCurrentUser()
{
    Int32 userId = UserHandle::GetMyUserId();
    AutoPtr<Environment::UserEnvironment> ue = new Environment::UserEnvironment(userId);
    return ue;
}

const AutoPtr<IFile> Environment::DIR_ANDROID_ROOT = GetDirectoryImpl(String("ANDROID_ROOT")/*ENV_ANDROID_ROOT*/, String("/system"));
const AutoPtr<IFile> Environment::DIR_OEM_ROOT = GetDirectoryImpl(String("OEM_ROOT")/*ENV_OEM_ROOT*/, String("/oem"));
const AutoPtr<IFile> Environment::DIR_VENDOR_ROOT = GetDirectoryImpl(String("VENDOR_ROOT")/*ENV_VENDOR_ROOT*/, String("/vendor"));
const AutoPtr<IFile> Environment::DIR_MEDIA_STORAGE = GetDirectoryImpl(String("MEDIA_STORAGE")/*ENV_MEDIA_STORAGE*/, String("/data/media"));

const AutoPtr<IFile> Environment::DATA_DIRECTORY = GetDirectoryImpl(String("ANDROID_DATA"), String("/data"));

const AutoPtr<IFile> Environment::SECURE_DATA_DIRECTORY = GetDirectoryImpl(String("ANDROID_SECURE_DATA"), String("/data/secure"));
const AutoPtr<IFile> Environment::DOWNLOAD_CACHE_DIRECTORY = GetDirectoryImpl(String("DOWNLOAD_CACHE"), String("/cache"));

const String Environment::CANONCIAL_EMULATED_STORAGE_TARGET =
    GetCanonicalPathOrNull(String("EMULATED_STORAGE_TARGET")/*ENV_EMULATED_STORAGE_TARGET*/);

const String Environment::SYSTEM_PROPERTY_EFS_ENABLED("persist.security.efs.enabled");

AutoPtr<Environment::UserEnvironment> Environment::sCurrentUser = InitForCurrentUser();
Boolean Environment::sUserRequired;

//===========================================================================================
// Environment::UserEnvironment
//===========================================================================================

Environment::UserEnvironment::UserEnvironment(
    /* [in] */ Int32 userId)
{
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);

    // See storage config details at http://source.android.com/tech/storage/
    String rawExternalStorage, rawEmulatedSource, rawEmulatedTarget;

    system->GetEnv(Environment::ENV_EXTERNAL_STORAGE, &rawExternalStorage);
    system->GetEnv(Environment::ENV_EMULATED_STORAGE_SOURCE, &rawEmulatedSource);
    system->GetEnv(Environment::ENV_EMULATED_STORAGE_TARGET, &rawEmulatedTarget);

    String rawMediaStorage;
    system->GetEnv(Environment::ENV_MEDIA_STORAGE, &rawMediaStorage);
    assert(0 && "TODO");
    // if (TextUtils::IsEmpty(rawMediaStorage)) {
    //     rawMediaStorage = String("/data/media");
    // }

    List<AutoPtr<IFile> > externalForVold, externalForApp;

    Boolean isEmpty = FALSE;
    //TODO Boolean isEmpty = TextUtils::IsEmpty(rawEmulatedTarget)
    if (!isEmpty) {
        // Device has emulated storage; external storage paths should have
        // userId burned into them.
        String rawUserId = StringUtils::ToString(userId);
        AutoPtr<IFile> emulatedSourceBase, emulatedTargetBase, mediaBase;
        CFile::New(rawEmulatedSource, (IFile**)&emulatedSourceBase);
        CFile::New(rawEmulatedTarget, (IFile**)&emulatedTargetBase);
        CFile::New(rawMediaStorage, (IFile**)&mediaBase);

        // /storage/emulated/0
        AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(1);
        params->Set(0, rawUserId);
        externalForVold.PushBack(Environment::BuildPath(emulatedSourceBase, params));
        externalForApp.PushBack(Environment::BuildPath(emulatedTargetBase, params));
        // /data/media/0
        mEmulatedDirForDirect = Environment::BuildPath(mediaBase, params);
    }
    else {
        // Device has physical external storage; use plain paths.
        //TODO isEmpty = TextUtils::IsEmpty(rawExternalStorage)
        if (isEmpty) {
            Logger::W("Environment", "EXTERNAL_STORAGE undefined; falling back to default");
            rawExternalStorage = String("/storage/sdcard0");
        }

        // /storage/sdcard0
        AutoPtr<IFile> f1, f2;
        CFile::New(rawExternalStorage, (IFile**)&f1);
        CFile::New(rawExternalStorage, (IFile**)&f2);
        externalForVold.PushBack(f1);
        externalForApp.PushBack(f2);
        // /data/media
        CFile::New(rawMediaStorage, (IFile**)&mEmulatedDirForDirect);
    }

    // Splice in any secondary storage paths, but only for owner
    String rawSecondaryStorage;
    system->GetEnv(ENV_SECONDARY_STORAGE, &rawSecondaryStorage);

    assert(0);
    //isEmpty = TextUtils::IsEmpty(rawSecondaryStorage);
    if (!isEmpty && userId == UserHandle::USER_OWNER) {
        AutoPtr<ArrayOf<String> > arrays;
        StringUtils::Split(rawSecondaryStorage, String(":"), (ArrayOf<String>**)&arrays);

        for (Int32 i = 0; i < arrays->GetLength(); ++i) {
            String secondaryPath = (*arrays)[i];
            AutoPtr<IFile> f1, f2;
            CFile::New(secondaryPath, (IFile**)&f1);
            CFile::New(secondaryPath, (IFile**)&f2);
            externalForVold.PushBack(f1);
            externalForApp.PushBack(f2);
        }
    }

    mExternalDirsForVold = ArrayOf<IFile*>::Alloc(externalForVold.GetSize());
    mExternalDirsForApp = ArrayOf<IFile*>::Alloc(externalForApp.GetSize());

    List<AutoPtr<IFile> >::Iterator it;

    Int32 i = 0;
    for (it = externalForVold.Begin(); it != externalForVold.End(); ++it) {
        mExternalDirsForVold->Set(i++, *it);
    }

    i = 0;
    for (it = externalForApp.Begin(); it != externalForApp.End(); ++it) {
        mExternalDirsForApp->Set(i++, *it);
    }
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStorageDirectory()
{
    return (*mExternalDirsForApp)[0];
}

AutoPtr<IFile> Environment::UserEnvironment::GetExternalStoragePublicDirectory(
    /* [in] */ const String& type)
{
    AutoPtr<ArrayOf<IFile*> > dirs = BuildExternalStoragePublicDirs(type);
    return (*dirs)[0];
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::GetExternalDirsForVold()
{
    return mExternalDirsForVold;
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::GetExternalDirsForApp()
{
    return mExternalDirsForApp;
}

AutoPtr<IFile> Environment::UserEnvironment::GetMediaDir()
{
    return mEmulatedDirForDirect;
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStoragePublicDirs(
    /* [in] */ const String& type)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(1);
    params->Set(0, type);
    return BuildPaths(mExternalDirsForApp, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAndroidDataDirs()
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(2);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_DATA);
    return BuildPaths(mExternalDirsForApp, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAndroidObbDirs()
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(3);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_OBB);
    params->Set(2, Environment::DIR_DATA);
    return BuildPaths(mExternalDirsForApp, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppDataDirs(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(3);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_DATA);
    params->Set(2, packageName);
    return BuildPaths(mExternalDirsForApp, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppDataDirsForVold(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(3);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_DATA);
    params->Set(2, packageName);
    return BuildPaths(mExternalDirsForVold, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppMediaDirs(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(3);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_MEDIA);
    params->Set(2, packageName);
    return BuildPaths(mExternalDirsForApp, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppMediaDirsForVold(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(3);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_MEDIA);
    params->Set(2, packageName);
    return BuildPaths(mExternalDirsForVold, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppObbDirs(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(3);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_OBB);
    params->Set(2, packageName);
    return BuildPaths(mExternalDirsForApp, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppObbDirsForVold(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(3);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_OBB);
    params->Set(2, packageName);
    return BuildPaths(mExternalDirsForVold, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppFilesDirs(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(4);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_DATA);
    params->Set(2, packageName);
    params->Set(3, Environment::DIR_FILES);
    return BuildPaths(mExternalDirsForApp, params);
}

AutoPtr<ArrayOf<IFile*> > Environment::UserEnvironment::BuildExternalStorageAppCacheDirs(
    /* [in] */ const String& packageName)
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(4);
    params->Set(0, Environment::DIR_ANDROID);
    params->Set(1, Environment::DIR_DATA);
    params->Set(2, packageName);
    params->Set(3, Environment::DIR_CACHE);

    return BuildPaths(mExternalDirsForApp, params);
}

//===========================================================================================
// Environment::UserEnvironment
//===========================================================================================

AutoPtr<IFile> Environment::GetRootDirectory()
{
    return DIR_ANDROID_ROOT;
}

AutoPtr<IFile> Environment::GetOemDirectory()
{
    return DIR_OEM_ROOT;
}

AutoPtr<IFile> Environment::GetVendorDirectory()
{
    return DIR_VENDOR_ROOT;
}

AutoPtr<IFile> Environment::GetSystemSecureDirectory()
{
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
    if (IsEncryptedFilesystemEnabled()) {
        return SECURE_DATA_DIRECTORY;
    }
    else {
        return DATA_DIRECTORY;
    }
}

AutoPtr<IFile> Environment::GetMediaStorageDirectory()
{
    if (FAILED(ThrowIfUserRequired())) return NULL;
    return sCurrentUser->GetMediaDir();
}

AutoPtr<IFile> Environment::GetUserSystemDirectory(
    /* [in] */ Int32 userId)
{
    AutoPtr<IFile> sd = GetSystemSecureDirectory();
    AutoPtr<IFile> users;
    CFile::New(sd, String("users"), (IFile**)&users);
    AutoPtr<IFile> result;
    CFile::New(users, StringUtils::ToString(userId), (IFile**)&result);
    return result;
}

AutoPtr<IFile> Environment::GetUserConfigDirectory(
    /* [in] */ Int32 userId)
{
    AutoPtr<IFile> sd = GetDataDirectory();
    AutoPtr<IFile> misc;
    CFile::New(sd, String("misc"), (IFile**)&misc);
    AutoPtr<IFile> user;
    CFile::New(misc, String("user"), (IFile**)&user);
    AutoPtr<IFile> result;
    CFile::New(user, StringUtils::ToString(userId), (IFile**)&result);
    return result;
}

Boolean Environment::IsEncryptedFilesystemEnabled()
{
    Boolean result;
    SystemProperties::GetBoolean(SYSTEM_PROPERTY_EFS_ENABLED, FALSE, &result);
    return result;
}

AutoPtr<IFile> Environment::GetDataDirectory()
{
    return DATA_DIRECTORY;
}

AutoPtr<IFile> Environment::GetExternalStorageDirectory()
{
    if (FAILED(ThrowIfUserRequired())) return NULL;
    AutoPtr<ArrayOf<IFile*> > files = sCurrentUser->GetExternalDirsForApp();
    return (*files)[0];
}

AutoPtr<IFile> Environment::GetLegacyExternalStorageDirectory()
{
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    String path;
    system->GetEnv(ENV_EXTERNAL_STORAGE, &path);
    AutoPtr<IFile> result;
    CFile::New(path, (IFile**)&result);
    return result;
}

/** {@hide} */
AutoPtr<IFile> Environment::GetLegacyExternalStorageObbDirectory()
{
    AutoPtr<ArrayOf<String> > params = ArrayOf<String>::Alloc(2);
    params->Set(0, DIR_ANDROID);
    params->Set(1, DIR_OBB);
    return BuildPath(GetLegacyExternalStorageDirectory(), params);
}

AutoPtr<IFile> Environment::GetEmulatedStorageSource(
    /* [in] */ Int32 userId)
{
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    String value;
    system->GetEnv(ENV_EMULATED_STORAGE_SOURCE, &value);
    // /mnt/shell/emulated/0
    AutoPtr<IFile> file;
    CFile::New(value, StringUtils::ToString(userId), (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetEmulatedStorageObbSource()
{
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    String value;
    system->GetEnv(ENV_EMULATED_STORAGE_SOURCE, &value);
    // /mnt/shell/emulated/obb
    AutoPtr<IFile> file;
    CFile::New(value, DIR_OBB, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStoragePublicDirectory(
    /* [in] */ const String& type)
{
    if (FAILED(ThrowIfUserRequired())) return NULL;
    AutoPtr<ArrayOf<IFile*> > files = sCurrentUser->BuildExternalStoragePublicDirs(type);
    return (*files)[0];
}

AutoPtr<ArrayOf<IFile*> > Environment::BuildExternalStorageAndroidDataDirs()
{
    if (FAILED(ThrowIfUserRequired())) return NULL;
    return sCurrentUser->BuildExternalStorageAndroidDataDirs();
}

AutoPtr<ArrayOf<IFile*> > Environment::BuildExternalStorageAppDataDirs(
    /* [in] */ const String& packageName)
 {
    if (FAILED(ThrowIfUserRequired())) return NULL;
    return sCurrentUser->BuildExternalStorageAppDataDirs(packageName);
}

AutoPtr<ArrayOf<IFile*> > Environment::BuildExternalStorageAppMediaDirs(
    /* [in] */ const String& packageName)
 {
    if (FAILED(ThrowIfUserRequired())) return NULL;
    return sCurrentUser->BuildExternalStorageAppMediaDirs(packageName);
}

AutoPtr<ArrayOf<IFile*> > Environment::BuildExternalStorageAppObbDirs(
    /* [in] */ const String& packageName)
{
    if (FAILED(ThrowIfUserRequired())) return NULL;
    return sCurrentUser->BuildExternalStorageAppObbDirs(packageName);
}

AutoPtr<ArrayOf<IFile*> > Environment::BuildExternalStorageAppFilesDirs(
    /* [in] */ const String& packageName)
{
    if (FAILED(ThrowIfUserRequired())) return NULL;
    return sCurrentUser->BuildExternalStorageAppFilesDirs(packageName);
}

AutoPtr<ArrayOf<IFile*> > Environment::BuildExternalStorageAppCacheDirs(
    /* [in] */ const String& packageName)
{
    if (FAILED(ThrowIfUserRequired())) return NULL;
    return sCurrentUser->BuildExternalStorageAppCacheDirs(packageName);
}

AutoPtr<IFile> Environment::GetDownloadCacheDirectory()
{
    return DOWNLOAD_CACHE_DIRECTORY;
}

String Environment::GetExternalStorageState()
{
    AutoPtr<ArrayOf<IFile*> > files = sCurrentUser->GetExternalDirsForApp();
    AutoPtr<IFile> externalDir = (*files)[0];
    return GetExternalStorageState(externalDir);
}

String Environment::GetStorageState(
    /* [in] */ IFile* path)
{
    return GetExternalStorageState(path);
}

String Environment::GetExternalStorageState(
    /* [in] */ IFile* path)
{
    AutoPtr<IStorageVolume> volume = GetStorageVolume(path);
    if (volume != NULL) {
        assert(0);

        // AutoPtr<IInterface> obj = ServiceManager::GetService(String("mount"));
        // IMountService* mountService = IMountService::Probe(mountService);
        // // try {
        // String result, path;
        // volume->GetPath(&path);
        // ECode ec = mountService->GetVolumeState(path, &result);
        // if (!FAILED(ec)) {
        //     return result;
        // }
        // } catch (RemoteException e) {
        // }
    }

    return MEDIA_UNKNOWN;
}

Boolean Environment::IsExternalStorageRemovable()
{
    if (IsStorageDisabled()) return FALSE;
    AutoPtr<ArrayOf<IFile*> > files = sCurrentUser->GetExternalDirsForApp();
    AutoPtr<IFile> externalDir = (*files)[0];
    return IsExternalStorageRemovable(externalDir);
}

Boolean Environment::IsExternalStorageRemovable(
    /* [in] */ IFile* path)
{
    AutoPtr<IStorageVolume> volume = GetStorageVolume(path);
    Boolean result = FALSE;
    if (volume != NULL) {
        volume->IsRemovable(&result);
    }
    // else {
    //     throw new IllegalArgumentException("Failed to find storage device at " + path);
    // }
    return result;
}

Boolean Environment::IsExternalStorageEmulated()
{
    if (IsStorageDisabled()) return FALSE;
    AutoPtr<ArrayOf<IFile*> > files = sCurrentUser->GetExternalDirsForApp();
    AutoPtr<IFile> externalDir = (*files)[0];
    return IsExternalStorageEmulated(externalDir);
}

Boolean Environment::IsExternalStorageEmulated(
    /* [in] */ IFile* path)
{
    AutoPtr<IStorageVolume> volume = GetStorageVolume(path);
    Boolean result = FALSE;
    if (volume != NULL) {
        volume->IsEmulated(&result);
    }
    // else {
    //     throw new IllegalArgumentException("Failed to find storage device at " + path);
    // }
    return result;
}

AutoPtr<IFile> Environment::GetDirectory(
    /* [in] */ const String& variableName,
    /* [in] */ const String& defaultPath)
{
    return GetDirectoryImpl(variableName, defaultPath);
}

void Environment::SetUserRequired(
    /* [in] */ Boolean userRequired)
{
    sUserRequired = userRequired;
}

AutoPtr<ArrayOf<IFile*> > Environment::BuildPaths(
    /* [in] */ ArrayOf<IFile*>* base,
    /* [in] */ ArrayOf<String>* segments)
{
    AutoPtr<ArrayOf<IFile*> > result = ArrayOf<IFile*>::Alloc(base->GetLength());

    for (Int32 i = 0; i < base->GetLength(); i++) {
        result->Set(i, BuildPath((*base)[i], segments));
    }
    return result;
}

AutoPtr<IFile> Environment::BuildPath(
    /* [in] */ IFile* base,
    /* [in] */ ArrayOf<String>* segments)
{
    AutoPtr<IFile> cur = base;
    for (Int32 i = 0; i < segments->GetLength(); ++i) {
        String segment = (*segments)[i];
        if (cur == NULL) {
            CFile::New(segment, (IFile**)&cur);
        }
        else {
            AutoPtr<IFile> temp;
            CFile::New(cur, segment, (IFile**)&temp);
            cur = temp;
        }
    }
    return cur;
}

AutoPtr<IFile> Environment::MaybeTranslateEmulatedPathToInternal(
    /* [in] */ IFile* path)
{
    // Fast return if not emulated, or missing variables
    if (!Environment::IsExternalStorageEmulated()
            || CANONCIAL_EMULATED_STORAGE_TARGET == NULL) {
        return path;
    }

    // try {
    String rawPath;
    path->GetCanonicalPath(&rawPath);
    if (rawPath.StartWith(CANONCIAL_EMULATED_STORAGE_TARGET)) {
        String path = rawPath.Substring(CANONCIAL_EMULATED_STORAGE_TARGET.GetLength());
        AutoPtr<IFile> internalPath;
        CFile::New(DIR_MEDIA_STORAGE, path, (IFile**)&internalPath);
        Boolean exists;
        internalPath->Exists(&exists);
        if (exists) {
            return internalPath;
        }
    }
    // } catch (IOException e) {
    //     Logger::W(TAG, "Failed to resolve canonical path for " + path);
    // }

    // Unable to translate to internal path; use original
    return path;
}

ECode Environment::ThrowIfUserRequired()
{
    if (sUserRequired) {
        // Log.wtf(TAG, "Path requests must specify a user by using UserEnvironment",
        //         new Throwable());
        return E_FAIL;
    }

    return NOERROR;
}


Boolean Environment::IsStorageDisabled()
{
    Boolean result;
    SystemProperties::GetBoolean(String("config.disable_storage"), FALSE, &result);
    return result;
}

AutoPtr<IStorageVolume> Environment::GetStorageVolume(
    /* [in] */ IFile* inPath)
{
    AutoPtr<IFile> path;
    ECode ec = inPath->GetCanonicalFile((IFile**)&path);
    if (ec == (ECode)E_IO_EXCEPTION) {
        return NULL;
    }

    assert(0);
    // try {
    // AutoPtr<IInterface> obj = ServiceManager::GetService(String("mount"));
    // IMountService* mountService = IMountService::Probe(mountService);
    // AutoPtr<ArrayOf<IStorageVolume*> > volumes;
    // mountService->GetVolumeList((ArrayOf<IStorageVolume*>**)&volumes);
    // for (Int32 i = 0; i < volumes; ++i) {
    //     AutoPtr<IStorageVolume> volume = (*volumes)[i];
    //     AutoPtr<IFile> file;
    //     volume->GetPathFile((IFile**)&file);
    //     if (FileUtils::Contains(file, path)) {
    //         return volume;
    //     }
    // }
    // } catch (RemoteException e) {
    // }

    return NULL;
}



} // namespace Os
} // namespace Droid
} // namespace Elastos
