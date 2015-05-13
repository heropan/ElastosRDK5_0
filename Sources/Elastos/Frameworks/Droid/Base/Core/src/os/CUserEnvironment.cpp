
#include "ext/frameworkdef.h"
#include "os/CUserEnvironment.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CUserEnvironment::GetExternalStorageDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageDirectory();
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStorageObbDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageObbDirectory();
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStoragePublicDirectory(
    /* [in] */ const String& type,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStoragePublicDirectory(type);
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStorageAndroidDataDir(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageAndroidDataDir();
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStorageAppDataDirectory(
    /* [in] */ const String& packageName,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageAppDataDirectory(packageName);
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStorageAppMediaDirectory(
    /* [in] */ const String& packageName,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageAppMediaDirectory(packageName);
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStorageAppObbDirectory(
    /* [in] */ const String& packageName,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageAppObbDirectory(packageName);
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStorageAppFilesDirectory(
    /* [in] */ const String& packageName,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageAppFilesDirectory(packageName);
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetExternalStorageAppCacheDirectory(
    /* [in] */ const String& packageName,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetExternalStorageAppCacheDirectory(packageName);
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::GetMediaStorageDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::UserEnvironment::GetMediaStorageDirectory();
    *dir = f;
    INTERFACE_ADDREF(*dir)
    return NOERROR;
}

ECode CUserEnvironment::constructor(
    /* [in] */ Int32 userId)
{
    return Init(userId);
}


} // namespace Os
} // namespace Droid
} // namespace Elastos
