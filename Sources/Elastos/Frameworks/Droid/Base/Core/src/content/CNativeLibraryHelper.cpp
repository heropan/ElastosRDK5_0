
#include "content/CNativeLibraryHelper.h"
#include "os/Build.h"
#include <ext/frameworkext.h>
#include <elastos/Slogger.h>
#include <utils/Log.h>
#include <utils/ZipFileRO.h>
#include <zlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define APK_LIB "lib/"
#define APK_LIB_LEN (sizeof(APK_LIB) - 1)

#define LIB_PREFIX "/lib"
#define LIB_PREFIX_LEN (sizeof(LIB_PREFIX) - 1)

#define LIB_SUFFIX ".so"
#define LIB_SUFFIX_LEN (sizeof(LIB_SUFFIX) - 1)

#define GDBSERVER "gdbserver"
#define GDBSERVER_LEN (sizeof(GDBSERVER) - 1)

#define TMP_FILE_PATTERN "/tmp.XXXXXX"
#define TMP_FILE_PATTERN_LEN (sizeof(TMP_FILE_PATTERN) - 1)

using namespace android;
using Elastos::Utility::Logging::Slogger;
using Elastos::IO::CFile;
using Elastos::Droid::Os::Build;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

// These match PackageManager.java install codes
typedef enum {
    INSTALL_SUCCEEDED = 1,
    INSTALL_FAILED_INVALID_APK = -2,
    INSTALL_FAILED_INSUFFICIENT_STORAGE = -4,
    INSTALL_FAILED_CONTAINER_ERROR = -18,
    INSTALL_FAILED_INTERNAL_ERROR = -110,
} install_status_t;

typedef install_status_t (*iterFunc)(void*, ZipFileRO*, ZipEntryRO, const char*);
static bool isFilenameSafe(const char* filename);
static bool isFileDifferent(const char* filePath, size_t fileSize, time_t modifiedTime,
        long zipCrc, struct stat64* st);
static install_status_t
        sumFiles(void* arg, ZipFileRO* zipFile, ZipEntryRO zipEntry, const char* fileName);
static install_status_t
        copyFileIfChanged(void* arg, ZipFileRO* zipFile, ZipEntryRO zipEntry, const char* fileName);
static install_status_t
        iterateOverNativeFiles(const String& filePath, const String& cpuAbi, const String& cpuAbi2,
        iterFunc callFunc, void* callArg);

const String CNativeLibraryHelper::TAG = String("NativeHelper");
const Boolean CNativeLibraryHelper::DEBUG_NATIVE = FALSE;

ECode CNativeLibraryHelper::SumNativeBinariesLI(
    /* [in] */ IFile* apkFile,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(apkFile)
    VALIDATE_NOT_NULL(result)
    String cpuAbi = Build::CPU_ABI;
    String cpuAbi2 = Build::CPU_ABI2;
    String path;
    FAIL_RETURN(apkFile->GetPath(&path))
    *result = NativeSumNativeBinaries(path, cpuAbi, cpuAbi2);
    return NOERROR;
}

ECode CNativeLibraryHelper::CopyNativeBinariesIfNeededLI(
    /* [in] */ IFile* apkFile,
    /* [in] */ IFile* sharedLibraryDir,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(apkFile)
    VALIDATE_NOT_NULL(sharedLibraryDir)
    VALIDATE_NOT_NULL(result)
    String cpuAbi = Build::CPU_ABI;
    String cpuAbi2 = Build::CPU_ABI2;
    String filePath;
    String libPath;
    FAIL_RETURN(apkFile->GetPath(&filePath))
    FAIL_RETURN(sharedLibraryDir->GetPath(&libPath))
    *result = NativeCopyNativeBinaries(filePath, libPath, cpuAbi, cpuAbi2);
    return NOERROR;
}

ECode CNativeLibraryHelper::RemoveNativeBinariesLI(
    /* [in] */ const String& nativeLibraryPath,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IFile> newFile;
    FAIL_RETURN(CFile::New(nativeLibraryPath, (IFile**)&newFile))
    return RemoveNativeBinariesFromDirLI(newFile, result);
}

ECode CNativeLibraryHelper::RemoveNativeBinariesFromDirLI(
    /* [in] */ IFile* nativeLibraryDir,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(nativeLibraryDir)
    VALIDATE_NOT_NULL(result)
    if (DEBUG_NATIVE) {
        String str("Deleting native binaries from: ");
        String tmp;
        FAIL_RETURN(nativeLibraryDir->GetPath(&tmp))
        str += tmp;
        Slogger::W(TAG, str);
    }

    *result = FALSE;
    Boolean ret = FALSE;

    /*
     * Just remove any file in the directory. Since the directory is owned
     * by the 'system' UID, the application is not supposed to have written
     * anything there.
     */
    if ((nativeLibraryDir->Exists(&ret), ret)) {
        AutoPtr<ArrayOf<IFile*> > binaries;
        FAIL_RETURN(nativeLibraryDir->ListFiles((ArrayOf<IFile*>**)&binaries))
        if (NULL != binaries) {
            for (Int32 nn = 0; nn < binaries->GetLength(); nn++) {
                if (DEBUG_NATIVE) {
                    String str("    Deleting ");
                    String name;
                    FAIL_RETURN((*binaries)[nn]->GetName(&name))
                    str += name;
                    Slogger::D(TAG, str);
                }

                Boolean isDeleted = FALSE;
                FAIL_RETURN((*binaries)[nn]->Delete(&isDeleted))
                if (!isDeleted) {
                    String str("Could not delete native binary: ");
                    String tmp;
                    FAIL_RETURN((*binaries)[nn]->GetPath(&tmp))
                    str += tmp;
                    Slogger::W(TAG, str);
                }
                else {
                    *result = TRUE;
                }
            }
        }
        // Do not delete 'lib' directory itself, or this will prevent
        // installation of future updates.
    }

    return NOERROR;
}

// for epk
ECode CNativeLibraryHelper::CopyEcoLI(
    /* [in] */ const String& epkPath,
    /* [in] */ const String& ecoName,
    /* [in] */ const String& ecoDir,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    ZipFileRO zipFile;

    if (zipFile.open(epkPath.string()) != NO_ERROR) {
        ALOGI("Couldn't open EPK %s\n", epkPath.string());
        *result = INSTALL_FAILED_INVALID_APK;
        return NOERROR;
    }

    const ZipEntryRO entry = zipFile.findEntryByName(ecoName.string());
    if (entry == NULL) {
        ALOGE("%s isn't found in %s", ecoName.string(), epkPath.string());
        *result = INSTALL_FAILED_INVALID_APK;
        return NOERROR;
    }

    install_status_t ret = copyFileIfChanged(
        const_cast<char*>(ecoDir.string()), &zipFile, entry, ecoName.string());

    if (ret != INSTALL_SUCCEEDED) {
        ALOGV("Failure for entry %s", ecoName.string());
    }
    *result = ret;

    return NOERROR;
}

Int64 CNativeLibraryHelper::NativeSumNativeBinaries(
    /* [in] */ const String& filePath,
    /* [in] */ const String& cpuAbi,
    /* [in] */ const String& cpuAbi2)
{
    size_t totalSize = 0;

    iterateOverNativeFiles(filePath, cpuAbi, cpuAbi2, sumFiles, &totalSize);

    return (Int64)totalSize;
}

Int32 CNativeLibraryHelper::NativeCopyNativeBinaries(
    /* [in] */ const String& filePath,
    /* [in] */ const String& sharedLibraryPath,
    /* [in] */ const String& cpuAbi,
    /* [in] */ const String& cpuAbi2)
{
    return (Int32)iterateOverNativeFiles(filePath, cpuAbi, cpuAbi2,
        copyFileIfChanged, const_cast<char*>(sharedLibraryPath.string()));
}

// Equivalent to isFilenameSafe
static bool
isFilenameSafe(const char* filename)
{
    off_t offset = 0;
    for (;;) {
        switch (*(filename + offset)) {
        case 0:
            // Null.
            // If we've reached the end, all the other characters are good.
            return true;

        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '0' ... '9':
        case '+':
        case ',':
        case '-':
        case '.':
        case '/':
        case '=':
        case '_':
            offset++;
            break;

        default:
            // We found something that is not good.
            return false;
        }
    }
    // Should not reach here.
}

static bool
isFileDifferent(const char* filePath, size_t fileSize, time_t modifiedTime,
        long zipCrc, struct stat64* st)
{
    if (lstat64(filePath, st) < 0) {
        // File is not found or cannot be read.
        ALOGV("Couldn't stat %s, copying: %s\n", filePath, strerror(errno));
        return true;
    }

    if (!S_ISREG(st->st_mode)) {
        return true;
    }

    if (st->st_size != fileSize) {
        return true;
    }

    // For some reason, bionic doesn't define st_mtime as time_t
    if (time_t(st->st_mtime) != modifiedTime) {
        ALOGV("mod time doesn't match: %ld vs. %ld\n", st->st_mtime, modifiedTime);
        return true;
    }

    int fd = TEMP_FAILURE_RETRY(open(filePath, O_RDONLY));
    if (fd < 0) {
        ALOGV("Couldn't open file %s: %s", filePath, strerror(errno));
        return true;
    }

    long crc = crc32(0L, Z_NULL, 0);
    unsigned char crcBuffer[16384];
    ssize_t numBytes;
    while ((numBytes = TEMP_FAILURE_RETRY(read(fd, crcBuffer, sizeof(crcBuffer)))) > 0) {
        crc = crc32(crc, crcBuffer, numBytes);
    }
    close(fd);

    // ALOGV("%s: crc = %lx, zipCrc = %lx\n", filePath, crc, zipCrc);

    if (crc != zipCrc) {
        return true;
    }

    return false;
}

static install_status_t
sumFiles(void* arg, ZipFileRO* zipFile, ZipEntryRO zipEntry, const char* fileName)
{
    size_t* total = (size_t*) arg;
    size_t uncompLen;

    if (!zipFile->getEntryInfo(zipEntry, NULL, &uncompLen, NULL, NULL, NULL, NULL)) {
        return INSTALL_FAILED_INVALID_APK;
    }

    *total += uncompLen;

    return INSTALL_SUCCEEDED;
}

/*
 * Copy the native library if needed.
 *
 * This function assumes the library and path names passed in are considered safe.
 */
static install_status_t
copyFileIfChanged(void* arg, ZipFileRO* zipFile, ZipEntryRO zipEntry, const char* fileName)
{
    // jstring* javaNativeLibPath = (jstring*) arg;
    // ScopedUtfChars nativeLibPath(env, *javaNativeLibPath);
    String nativeLibPath((char*) arg);

    size_t uncompLen;
    long when;
    long crc;
    time_t modTime;

    if (!zipFile->getEntryInfo(zipEntry, NULL, &uncompLen, NULL, NULL, &when, &crc)) {
        ALOGD("Couldn't read zip entry info\n");
        return INSTALL_FAILED_INVALID_APK;
    } else {
        struct tm t;
        ZipFileRO::zipTimeToTimespec(when, &t);
        modTime = mktime(&t);
    }

    // Build local file path
    const size_t fileNameLen = strlen(fileName);
    char localFileName[nativeLibPath.GetByteLength() + fileNameLen + 2];

    if (strlcpy(localFileName, nativeLibPath.string(), sizeof(localFileName)) != nativeLibPath.GetByteLength()) {
        ALOGD("Couldn't allocate local file name for library");
        return INSTALL_FAILED_INTERNAL_ERROR;
    }

    *(localFileName + nativeLibPath.GetByteLength()) = '/';

    if (strlcpy(localFileName + nativeLibPath.GetByteLength() + 1, fileName, sizeof(localFileName)
                    - nativeLibPath.GetByteLength() - 1) != fileNameLen) {
        ALOGD("Couldn't allocate local file name for library");
        return INSTALL_FAILED_INTERNAL_ERROR;
    }

    // Only copy out the native file if it's different.
    struct stat st;
    if (!isFileDifferent(localFileName, uncompLen, modTime, crc, &st)) {
        return INSTALL_SUCCEEDED;
    }

    char localTmpFileName[nativeLibPath.GetByteLength() + TMP_FILE_PATTERN_LEN + 2];
    if (strlcpy(localTmpFileName, nativeLibPath.string(), sizeof(localTmpFileName))
            != nativeLibPath.GetByteLength()) {
        ALOGD("Couldn't allocate local file name for library");
        return INSTALL_FAILED_INTERNAL_ERROR;
    }

    *(localFileName + nativeLibPath.GetByteLength()) = '/';

    if (strlcpy(localTmpFileName + nativeLibPath.GetByteLength(), TMP_FILE_PATTERN,
                    TMP_FILE_PATTERN_LEN - nativeLibPath.GetByteLength()) != TMP_FILE_PATTERN_LEN) {
        ALOGI("Couldn't allocate temporary file name for library");
        return INSTALL_FAILED_INTERNAL_ERROR;
    }

    int fd = mkstemp(localTmpFileName);
    if (fd < 0) {
        ALOGI("Couldn't open temporary file name: %s: %s\n", localTmpFileName, strerror(errno));
        return INSTALL_FAILED_CONTAINER_ERROR;
    }

    if (!zipFile->uncompressEntry(zipEntry, fd)) {
        ALOGI("Failed uncompressing %s to %s\n", fileName, localTmpFileName);
        close(fd);
        unlink(localTmpFileName);
        return INSTALL_FAILED_CONTAINER_ERROR;
    }

    close(fd);

    // Set the modification time for this file to the ZIP's mod time.
    struct timeval times[2];
    times[0].tv_sec = st.st_atime;
    times[1].tv_sec = modTime;
    times[0].tv_usec = times[1].tv_usec = 0;
    if (utimes(localTmpFileName, times) < 0) {
        ALOGI("Couldn't change modification time on %s: %s\n", localTmpFileName, strerror(errno));
        unlink(localTmpFileName);
        return INSTALL_FAILED_CONTAINER_ERROR;
    }

    // Set the mode to 755
    static const mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP |  S_IXGRP | S_IROTH | S_IXOTH;
    if (chmod(localTmpFileName, mode) < 0) {
        ALOGI("Couldn't change permissions on %s: %s\n", localTmpFileName, strerror(errno));
        unlink(localTmpFileName);
        return INSTALL_FAILED_CONTAINER_ERROR;
    }

    // Finally, rename it to the final name.
    if (rename(localTmpFileName, localFileName) < 0) {
        ALOGI("Couldn't rename %s to %s: %s\n", localTmpFileName, localFileName, strerror(errno));
        unlink(localTmpFileName);
        return INSTALL_FAILED_CONTAINER_ERROR;
    }

    ALOGV("Successfully moved %s to %s\n", localTmpFileName, localFileName);

    return INSTALL_SUCCEEDED;
}

static install_status_t
iterateOverNativeFiles(const String& filePath, const String& cpuAbi, const String& cpuAbi2,
        iterFunc callFunc, void* callArg) {
    // ScopedUtfChars filePath(env, javaFilePath);
    // ScopedUtfChars cpuAbi(env, javaCpuAbi);
    // ScopedUtfChars cpuAbi2(env, javaCpuAbi2);

    ZipFileRO zipFile;

    if (zipFile.open(filePath.string()) != NO_ERROR) {
        ALOGI("Couldn't open APK %s\n", filePath.string());
        return INSTALL_FAILED_INVALID_APK;
    }

    const int N = zipFile.getNumEntries();

    char fileName[PATH_MAX];
    bool hasPrimaryAbi = false;

    for (int i = 0; i < N; i++) {
        const ZipEntryRO entry = zipFile.findEntryByIndex(i);
        if (entry == NULL) {
            continue;
        }

        // Make sure this entry has a filename.
        if (zipFile.getEntryFileName(entry, fileName, sizeof(fileName))) {
            continue;
        }

        // Make sure we're in the lib directory of the ZIP.
        if (strncmp(fileName, APK_LIB, APK_LIB_LEN)) {
            continue;
        }

        // Make sure the filename is at least to the minimum library name size.
        const size_t fileNameLen = strlen(fileName);
        static const size_t minLength = APK_LIB_LEN + 2 + LIB_PREFIX_LEN + 1 + LIB_SUFFIX_LEN;
        if (fileNameLen < minLength) {
            continue;
        }

        const char* lastSlash = strrchr(fileName, '/');
        ALOG_ASSERT(lastSlash != NULL, "last slash was null somehow for %s\n", fileName);

        // Check to make sure the CPU ABI of this file is one we support.
        const char* cpuAbiOffset = fileName + APK_LIB_LEN;
        const size_t cpuAbiRegionSize = lastSlash - cpuAbiOffset;

        // ALOGV("Comparing ABIs %s and %s versus %s\n", cpuAbi.string(), cpuAbi2.string(), cpuAbiOffset);

        if (cpuAbi.GetByteLength() == cpuAbiRegionSize
                && *(cpuAbiOffset + cpuAbi.GetByteLength()) == '/'
                && !strncmp(cpuAbiOffset, cpuAbi.string(), cpuAbiRegionSize)) {
            // ALOGV("Using primary ABI %s\n", cpuAbi.string());
            hasPrimaryAbi = true;
        }
        else if (cpuAbi2.GetByteLength() == cpuAbiRegionSize
                && *(cpuAbiOffset + cpuAbi2.GetByteLength()) == '/'
                && !strncmp(cpuAbiOffset, cpuAbi2.string(), cpuAbiRegionSize)) {

            /*
             * If this library matches both the primary and secondary ABIs,
             * only use the primary ABI.
             */
            if (hasPrimaryAbi) {
                ALOGV("Already saw primary ABI, skipping secondary ABI %s\n", cpuAbi2.string());
                continue;
            }
            else {
                ALOGV("Using secondary ABI %s\n", cpuAbi2.string());
            }
        }
        else {
            ALOGV("abi didn't match anything: %s (end at %zd)\n", cpuAbiOffset, cpuAbiRegionSize);
            continue;
        }

        // If this is a .so file, check to see if we need to copy it.
        if ((!strncmp(fileName + fileNameLen - LIB_SUFFIX_LEN, LIB_SUFFIX, LIB_SUFFIX_LEN)
                    && !strncmp(lastSlash, LIB_PREFIX, LIB_PREFIX_LEN)
                    && isFilenameSafe(lastSlash + 1))
                || !strncmp(lastSlash + 1, GDBSERVER, GDBSERVER_LEN)) {

            install_status_t ret = callFunc(callArg, &zipFile, entry, lastSlash + 1);

            if (ret != INSTALL_SUCCEEDED) {
                ALOGV("Failure for entry %s", lastSlash + 1);
                return ret;
            }
        }
    }

    return INSTALL_SUCCEEDED;
}

}
}
}
}

