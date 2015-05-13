
#ifndef __CNATIVELIBRARYHELPER_H__
#define __CNATIVELIBRARYHELPER_H__

#include "_CNativeLibraryHelper.h"

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

CarClass(CNativeLibraryHelper)
{
public:
    /**
     * Sums the size of native binaries in an APK.
     *
     * @param apkFile APK file to scan for native libraries
     * @return size of all native binary files in bytes
     */
    CARAPI SumNativeBinariesLI(
        /* [in] */ IFile* apkFile,
        /* [out] */ Int64* result);

    /**
     * Copies native binaries to a shared library directory.
     *
     * @param apkFile APK file to scan for native libraries
     * @param sharedLibraryDir directory for libraries to be copied to
     * @return {@link PackageManager#INSTALL_SUCCEEDED} if successful or another
     *         error code from that class if not
     */
    CARAPI CopyNativeBinariesIfNeededLI(
        /* [in] */ IFile* apkFile,
        /* [in] */ IFile* sharedLibraryDir,
        /* [out] */ Int32* result);

    // Convenience method to call removeNativeBinariesFromDirLI(File)
    CARAPI RemoveNativeBinariesLI(
        /* [in] */ const String& nativeLibraryPath,
        /* [out] */ Boolean* result);

    // Remove the native binaries of a given package. This simply
    // gets rid of the files in the 'lib' sub-directory.
    CARAPI RemoveNativeBinariesFromDirLI(
        /* [in] */ IFile* nativeLibraryDir,
        /* [out] */ Boolean* result);

    // for epk
    // copy 'ecoName'.eco in epkFile to ecoDir
    CARAPI CopyEcoLI(
        /* [in] */ const String& epkPath,
        /* [in] */ const String& ecoName,
        /* [in] */ const String& ecoDir,
        /* [out] */ Int32* result);

private:
    static CARAPI_(Int64) NativeSumNativeBinaries(
        /* [in] */ const String& filePath,
        /* [in] */ const String& cpuAbi,
        /* [in] */ const String& cpuAbi2);

    static CARAPI_(Int32) NativeCopyNativeBinaries(
        /* [in] */ const String& filePath,
        /* [in] */ const String& sharedLibraryPath,
        /* [in] */ const String& cpuAbi,
        /* [in] */ const String& cpuAbi2);

private:
    static const String TAG;
    static const Boolean DEBUG_NATIVE;

};

}
}
}
}

#endif // __CNATIVELIBRARYHELPER_H__
