
#include <Elastos.CoreLibrary.IO.h>
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Os.h"
#include "elastos/droid/webkit/native/base/PathUtils.h"
#include "elastos/droid/webkit/native/base/api/PathUtils_dec.h"
#include "elastos/droid/os/Environment.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Os::Environment;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

String PathUtils::sDataDirectorySuffix;

// Prevent instantiation.
PathUtils::PathUtils()
{
}

/**
 * Sets the suffix that should be used for the directory where private data is to be stored
 * by the application.
 * @param suffix The private data directory suffix.
 * @see Context#getDir(String, int)
 */
void PathUtils::SetPrivateDataDirectorySuffix(
    /* [in] */ const String& suffix)
{
    sDataDirectorySuffix = suffix;
}

/**
 * @return the private directory that is used to store application data.
 */
//@CalledByNative
String PathUtils::GetDataDirectory(
    /* [in] */ IContext* appContext)
{
    if (sDataDirectorySuffix == NULL) {
        Logger::E("PathUtils::GetDataDirectory", "sDataDirectorySuffix is null");
        assert(0);
//        throw new IllegalStateException(
//                "setDataDirectorySuffix must be called before getDataDirectory");
    }

    AutoPtr<IFile> file;
    appContext->GetDir(sDataDirectorySuffix, IContext::MODE_PRIVATE, (IFile**)&file);
    String path;
    file->GetPath(&path);

    return path;
}

/**
 * @return the private directory that is used to store application database.
 */
//@CalledByNative
String PathUtils::GetDatabaseDirectory(
    /* [in] */ IContext* appContext)
{
    // Context.getDatabasePath() returns path for the provided filename.
    AutoPtr<IFile> file;
    appContext->GetDatabasePath(String("foo"), (IFile**)&file);
    String parent;
    file->GetParent(&parent);
    return parent;
}

/**
 * @return the cache directory.
 */
//@SuppressWarnings("unused")
//@CalledByNative
String PathUtils::GetCacheDirectory(
    /* [in] */ IContext* appContext)
{
    AutoPtr<IFile> file;
    appContext->GetCacheDir((IFile**)&file);
    String path;
    file->GetPath(&path);
    return path;
}

/**
 * @return the public downloads directory.
 */
//@SuppressWarnings("unused")
//@CalledByNative
String PathUtils::GetDownloadsDirectory(
    /* [in] */ IContext* appContext)
{
    AutoPtr<IFile> file = Environment::GetExternalStoragePublicDirectory(
            Environment::DIRECTORY_DOWNLOADS);
    String path;
    file->GetPath(&path);
    return path;
}

/**
 * @return the path to native libraries.
 */
//@SuppressWarnings("unused")
//@CalledByNative
String PathUtils::GetNativeLibraryDirectory(
    /* [in] */ IContext* appContext)
{
    AutoPtr<IApplicationInfo> ai;
    appContext->GetApplicationInfo((IApplicationInfo**)&ai);
    Int32 flags;
    ai->GetFlags(&flags);
    if ((flags & IApplicationInfo::FLAG_UPDATED_SYSTEM_APP) != 0 ||
        (flags & IApplicationInfo::FLAG_SYSTEM) == 0)
    {
        String dir;
        ai->GetNativeLibraryDir(&dir);
        return dir;
    }

    return String("/system/lib/");
}

/**
 * @return the external storage directory.
 */
//@SuppressWarnings("unused")
//@CalledByNative
String PathUtils::GetExternalStorageDirectory()
{
    AutoPtr<IFile> file = Environment::GetExternalStorageDirectory();
    String path;
    file->GetAbsolutePath(&path);
    return path;
}

String PathUtils::GetDataDirectory(
    /* [in] */ IInterface* appContext)
{
    AutoPtr<IContext> c = IContext::Probe(appContext);
    return GetDataDirectory(c);
}

String PathUtils::GetDatabaseDirectory(
    /* [in] */ IInterface* appContext)
{
    AutoPtr<IContext> c = IContext::Probe(appContext);
    return GetDatabaseDirectory(c);
}

String PathUtils::GetCacheDirectory(
    /* [in] */ IInterface* appContext)
{
    AutoPtr<IContext> c = IContext::Probe(appContext);
    return GetCacheDirectory(c);
}

String PathUtils::GetDownloadsDirectory(
    /* [in] */ IInterface* appContext)
{
    AutoPtr<IContext> c = IContext::Probe(appContext);
    return GetDownloadsDirectory(c);
}

String PathUtils::GetNativeLibraryDirectory(
    /* [in] */ IInterface* appContext)
{
    AutoPtr<IContext> c = IContext::Probe(appContext);
    return GetNativeLibraryDirectory(c);
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
