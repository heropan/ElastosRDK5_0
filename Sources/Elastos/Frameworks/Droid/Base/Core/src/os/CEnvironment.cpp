
#include "ext/frameworkdef.h"
#include "os/CEnvironment.h"
#include "os/Environment.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CEnvironment::GetRootDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetRootDirectory();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetDataDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetDataDirectory();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetSystemSecureDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetSystemSecureDirectory();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetSecureDataDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetSecureDataDirectory();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetUserSystemDirectory(
    /* [in] */ Int32 userId,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetUserSystemDirectory(userId);
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetExternalStorageDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetExternalStorageDirectory();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetLegacyExternalStorageDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetLegacyExternalStorageDirectory();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetEmulatedStorageSource(
    /* [in] */ Int32 userId,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetEmulatedStorageSource(userId);
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetEmulatedStorageObbSource(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetEmulatedStorageObbSource();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetExternalStoragePublicDirectory(
    /* [in] */ const String& type,
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetExternalStoragePublicDirectory(type);
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetDownloadCacheDirectory(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir)
    AutoPtr<IFile> f = Environment::GetDownloadCacheDirectory();
    *dir = f;
    REFCOUNT_ADD(*dir)
    return NOERROR;
}

ECode CEnvironment::GetExternalStorageState(
    /* [out] */ String* state)
{
    VALIDATE_NOT_NULL(state)
    *state = Environment::GetExternalStorageState();
    return NOERROR;
}

ECode CEnvironment::IsExternalStorageRemovable(
    /* [out] */ Boolean* isRemovable)
{
    VALIDATE_NOT_NULL(isRemovable)
    *isRemovable = Environment::IsExternalStorageRemovable();
    return NOERROR;
}

ECode CEnvironment::IsExternalStorageEmulated(
    /* [out] */ Boolean* isEmulated)
{
    VALIDATE_NOT_NULL(isEmulated)
    *isEmulated = Environment::IsExternalStorageEmulated();
    return NOERROR;
}


} // namespace Os
} // namespace Droid
} // namespace Elastos
