
#ifndef __ELASTOS_DROID_OS_CUSERENVIRONMENT_H__
#define __ELASTOS_DROID_OS_CUSERENVIRONMENT_H__

#include "_CUserEnvironment.h"
#include "os/Environment.h"

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Provides access to environment variables.
 */
CarClass(CUserEnvironment) , public Environment::UserEnvironment
{
public:
    CARAPI GetExternalStorageDirectory(
        /* [out] */ IFile** dir);

    CARAPI GetExternalStorageObbDirectory(
        /* [out] */ IFile** dir);

    CARAPI GetExternalStoragePublicDirectory(
        /* [in] */ const String& type,
        /* [out] */ IFile** dir);

    CARAPI GetExternalStorageAndroidDataDir(
        /* [out] */ IFile** dir);

    CARAPI GetExternalStorageAppDataDirectory(
        /* [in] */ const String& packageName,
        /* [out] */ IFile** dir);

    CARAPI GetExternalStorageAppMediaDirectory(
        /* [in] */ const String& packageName,
        /* [out] */ IFile** dir);

    CARAPI GetExternalStorageAppObbDirectory(
        /* [in] */ const String& packageName,
        /* [out] */ IFile** dir);

    CARAPI GetExternalStorageAppFilesDirectory(
        /* [in] */ const String& packageName,
        /* [out] */ IFile** dir);

    CARAPI GetExternalStorageAppCacheDirectory(
        /* [in] */ const String& packageName,
        /* [out] */ IFile** dir);

    CARAPI GetMediaStorageDirectory(
        /* [out] */ IFile** dir);

    CARAPI constructor(
        /* [in] */ Int32 userId);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CUSERENVIRONMENT_H__
