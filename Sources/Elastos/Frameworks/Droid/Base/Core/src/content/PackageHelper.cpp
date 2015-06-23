
#include "ext/frameworkdef.h"
#include "content/PackageHelper.h"
#include "os/ServiceManager.h"
#include "os/storage/StorageResultCode.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::IO::IInputStream;
using Elastos::IO::IFlushable;
using Elastos::Utility::Zip::CZipEntry;
using Elastos::Utility::Zip::CZipFile;
using Elastos::Utility::Zip::CZipOutputStream;
using Elastos::Utility::Logging::Slogger;

using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::Storage::StorageResultCode;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

const Boolean PackageHelper::localLOGV = FALSE;
const String PackageHelper::TAG("PackageHelper");

ECode PackageHelper::GetMountService(
    /* [out] */ IMountService** mountService)
{
    VALIDATE_NOT_NULL(mountService)

    AutoPtr<IInterface> obj = ServiceManager::GetService(String("mount"));
    if (obj != NULL) {
        *mountService = IMountService::Probe(obj);
        REFCOUNT_ADD(*mountService);
        return NOERROR;
    }
    else {
        Slogger::E(TAG, "Can't get mount service");
        //throw new RemoteException("Could not contact mount service");
        return E_RUNTIME_EXCEPTION;
    }
}

String PackageHelper::CreateSdDir(
    /* [in] */ Int32 sizeMb,
    /* [in] */ const String& cid,
    /* [in] */ const String& sdEncKey,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isExternal)
{
    // Create mount point via MountService
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "MountService running?");
        return String(NULL);
    }

    // if (localLOGV)
    //             Log.i(TAG, "Size of container " + sizeMb + " MB");

    Int32 rc = 0;
    if (FAILED(mountService->CreateSecureContainer(cid, sizeMb, String("ext4"), sdEncKey, uid, isExternal, &rc))) {
        Slogger::E(TAG, "MountService running?");
        return String(NULL);
    }
    if (rc != StorageResultCode::OperationSucceeded) {
        Slogger::E(TAG, "Failed to create secure container %s", cid.string());
        return String(NULL);
    }

    String cachePath;
    if (FAILED(mountService->GetSecureContainerPath(cid, &cachePath))){
        Slogger::E(TAG, "MountService running?");
        return String(NULL);
    }
    if (localLOGV) {
        Slogger::I(TAG, "Created secure container %s at %s", cid.string(), cachePath.string());
    }
    return cachePath;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "MountService running?");
    // }
}

String PackageHelper::MountSdDir(
    /* [in] */ const String& cid,
    /* [in] */ const String& key,
    /* [in] */ Int32 ownerUid)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "MountService running?");
        return String(NULL);
    }
    Int32 rc = 0;
    if (FAILED(mountService->MountSecureContainer(cid, key, ownerUid, &rc))) {
        Slogger::E(TAG, "MountService running?");
        return String(NULL);
    }
    if (rc != StorageResultCode::OperationSucceeded) {
        Slogger::I(TAG, "Failed to mount container %s rc: %s", cid.string(), StringUtils::Int32ToString(rc).string());
        return String(NULL);
    }

    String dir;
    if (FAILED(mountService->GetSecureContainerPath(cid, &dir))) {
        Slogger::E(TAG, "MountService running?");
        return String(NULL);
    }
    return dir;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "MountService running?");
    // }
}

Boolean PackageHelper::UnMountSdDir(
    /* [in] */ const String& cid)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "MountService running?");
        return FALSE;
    }
    Int32 rc = 0;
    if (FAILED(mountService->UnmountSecureContainer(cid, TRUE, &rc))) {
        Slogger::E(TAG, "MountService running?");
        return FALSE;
    }
    if (rc != StorageResultCode::OperationSucceeded) {
        Slogger::E(TAG, "Failed to unmount %s with rc %s", cid.string(), StringUtils::Int32ToString(rc).string());
        return NOERROR;
    }
    return TRUE;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "MountService running?");
    // }
}

Boolean PackageHelper::RenameSdDir(
    /* [in] */ const String& oldId,
    /* [in] */ const String& newId)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to rename %s to %s", oldId.string(), newId.string());
        return FALSE;
    }
    Int32 rc = 0;
    if (FAILED(mountService->RenameSecureContainer(oldId, newId, &rc))) {
        Slogger::E(TAG, "Failed to rename %s to %s", oldId.string(), newId.string());
        return FALSE;
    }
    if (rc != StorageResultCode::OperationSucceeded) {
        Slogger::E(TAG, "Failed to rename %s to %s with rc %s", oldId.string(),
                newId.string(), StringUtils::Int32ToString(rc).string());
        return NOERROR;
    }
    return TRUE;
    // } catch (RemoteException e) {
    //     Log.i(TAG, "Failed ot rename  " + oldId + " to " + newId +
    //             " with exception : " + e);
    // }
}

String PackageHelper::GetSdDir(
    /* [in] */ const String& cid)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to get container path for %s", cid.string());
        return String(NULL);
    }
    String dir;
    if (FAILED(mountService->GetSecureContainerPath(cid, &dir))) {
        Slogger::E(TAG, "Failed to get container path for %s", cid.string());
        return String(NULL);
    }
    return dir;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Failed to get container path for " + cid +
    //         " with exception " + e);
    // }
}

String PackageHelper::GetSdFilesystem(
    /* [in] */ const String& cid)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to get container path for %s", cid.string());
        return String(NULL);
    }
    String fs;
    if (FAILED(mountService->GetSecureContainerFilesystemPath(cid, &fs))) {
        Slogger::E(TAG, "Failed to get container path for %s", cid.string());
        return String(NULL);
    }
    return fs;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Failed to get container path for " + cid +
    //         " with exception " + e);
    // }
}

Boolean PackageHelper::FinalizeSdDir(
    /* [in] */ const String& cid)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to finalize container %s", cid.string());
        return FALSE;
    }
    Int32 rc = 0;
    if (FAILED(mountService->FinalizeSecureContainer(cid, &rc))) {
        Slogger::E(TAG, "Failed to finalize container %s", cid.string());
        return FALSE;
    }
    if (rc != StorageResultCode::OperationSucceeded) {
        Slogger::E(TAG, "Failed to finalize container %s", cid.string());
        return FALSE;
    }
    return TRUE;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Failed to finalize container " + cid +
    //             " with exception " + e);
    // }
}

Boolean PackageHelper::DestroySdDir(
    /* [in] */ const String& cid)
{
    // try {
    if (localLOGV) {
        Slogger::I(TAG, "Forcibly destroying container %s", cid.string());
    }

    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to destroy container %s", cid.string());
        return FALSE;
    }
    Int32 rc = 0;
    if (FAILED(mountService->DestroySecureContainer(cid, TRUE, &rc))) {
        Slogger::E(TAG, "Failed to destroy container %s", cid.string());
        return FALSE;
    }
    if (rc != StorageResultCode::OperationSucceeded) {
        Slogger::I(TAG, "Failed to destroy container %s", cid.string());
        return FALSE;
    }
    return TRUE;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Failed to destroy container " + cid +
    //             " with exception " + e);
    // }
}

AutoPtr< ArrayOf<String> > PackageHelper::GetSecureContainerList()
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to get secure container list");
        return NULL;
    }
    AutoPtr< ArrayOf<String> > list;
    if (FAILED(mountService->GetSecureContainerList((ArrayOf<String>**)&list))) {
        Slogger::E(TAG, "Failed to get secure container list");
        return NULL;
    }
    return list;
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Failed to get secure container list with exception" +
    //             e);
    // }
}

Boolean PackageHelper::IsContainerMounted(
    /* [in] */ const String& cid)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to find out if container %s mounted", cid.string());
        return FALSE;
    }
    Boolean isMounted;
    if (FAILED(mountService->IsSecureContainerMounted(cid, &isMounted))) {
        Slogger::E(TAG, "Failed to find out if container %s mounted", cid.string());
        return FALSE;
    }
    return isMounted;
    // } catch (RemoteException e) {
    //        Log.e(TAG, "Failed to find out if container " + cid + " mounted");
    // }
}

Int32 PackageHelper::ExtractPublicFiles(
    /* [in] */ const String& packagePath,
    /* [in] */ IFile* publicZipFile)
{
    AutoPtr<IFileOutputStream> fstr;
    AutoPtr<IZipOutputStream> publicZipOutStream;
    AutoPtr<IIoUtils> ioUtils;

    if (publicZipFile != NULL) {
        ASSERT_SUCCEEDED(CFileOutputStream::New(publicZipFile, (IFileOutputStream**)&fstr))
        ASSERT_SUCCEEDED(CZipOutputStream::New(fstr, (IZipOutputStream**)&publicZipOutStream))
    }

    Int32 size = 0;

    // try {
    AutoPtr<IZipFile> privateZip;
    AutoPtr<IObjectContainer> entries;
    AutoPtr<IObjectEnumerator> it;
    if (FAILED(CZipFile::New(packagePath, (IZipFile**)&privateZip))) goto Exit2;
    // try {
    // Copy manifest, resources.arsc and res directory to public zip
    if (FAILED(privateZip->GetEntries((IObjectContainer**)&entries))) goto Exit1;
    if (entries != NULL) {
        entries->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean hasNext = FALSE;
        AutoPtr<IZipEntry> zipEntry;
        String zipEntryName;
        while ((it->MoveNext(&hasNext), hasNext)) {
            zipEntry = NULL;
            it->Current((IInterface**)&zipEntry);
            zipEntry->GetName(&zipEntryName);
            if (String("AndroidManifest.xml").Equals(zipEntryName)
                    || String("resources.arsc").Equals(zipEntryName)
                    || zipEntryName.StartWith("res/")) {
                Int64 entrySize = 0;
                zipEntry->GetSize(&entrySize);
                size += entrySize;
                if (publicZipFile != NULL) {
                    if (FAILED(CopyZipEntry(zipEntry, privateZip, publicZipOutStream))) goto Exit1;
                }
            }
        }
    }
    // } finally {
    //     try { privateZip.close(); } catch (IOException e) {}
    // }
Exit1:
    privateZip->Close();
    if (publicZipFile != NULL) {
//        publicZipOutStream.finish();
//        publicZipOutStream.flush();
//        FileUtils.sync(fstr);
//        publicZipOutStream.close();
//        FileUtils.setPermissions(publicZipFile.getAbsolutePath(), FileUtils.S_IRUSR
//                | FileUtils.S_IWUSR | FileUtils.S_IRGRP | FileUtils.S_IROTH, -1, -1);
        assert(0);
    }
    // } finally {
    //     IoUtils.closeQuietly(publicZipOutStream);
    // }
Exit2:
    ASSERT_SUCCEEDED(CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils));
    ioUtils->CloseQuietly(publicZipOutStream);
    return size;
}

ECode PackageHelper::CopyZipEntry(
    /* [in] */ IZipEntry* zipEntry,
    /* [in] */ IZipFile* inZipFile,
    /* [in] */ IZipOutputStream* outZipStream)
{
    AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(4096);
    Int32 num = 0;

    AutoPtr<IZipEntry> newEntry;
    AutoPtr<IIoUtils> ioUtils;

    Int32 ret = 0;
    if (zipEntry->GetMethod(&ret), ret == IZipEntry::STORED) {
        // Preserve the STORED method of the input entry.
        FAIL_RETURN(CZipEntry::New(zipEntry, (IZipEntry**)&newEntry))
    }
    else {
        // Create a new entry so that the compressed len is recomputed.
        String name;
        FAIL_RETURN(zipEntry->GetName(&name))
        FAIL_RETURN(CZipEntry::New(name, (IZipEntry**)&newEntry))
    }
    FAIL_RETURN(outZipStream->PutNextEntry(newEntry))

    AutoPtr<IInputStream> data;
    FAIL_RETURN(inZipFile->GetInputStream(zipEntry, (IInputStream**)&data))
    // try {
    while ((data->ReadBytes(buffer, &num), num) > 0) {
        if (FAILED(outZipStream->WriteBytes(*buffer, 0, num))) goto Exit1;
    }
    if (FAILED(IFlushable::Probe(outZipStream)->Flush())) goto Exit1;
    // } finally {
    //     IoUtils.closeQuietly(data);
    // }
Exit1:
    ASSERT_SUCCEEDED(CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils))
    FAIL_RETURN(ioUtils->CloseQuietly(data))
    return NOERROR;
}

Boolean PackageHelper::FixSdPermissions(
    /* [in] */ const String& cid,
    /* [in] */ Int32 gid,
    /* [in] */ const String& filename)
{
    // try {
    AutoPtr<IMountService> mountService;
    if (FAILED(GetMountService((IMountService**)&mountService))) {
        Slogger::E(TAG, "Failed to fixperms container %s", cid.string());
        return FALSE;
    }
    Int32 rc = 0;
    if (FAILED(mountService->FixPermissionsSecureContainer(cid, gid, filename, &rc))) {
        Slogger::E(TAG, "Failed to fixperms container %s", cid.string());
        return FALSE;
    }
    if (rc != StorageResultCode::OperationSucceeded) {
        Slogger::E(TAG, "Failed to fixperms container %s", cid.string());
        return FALSE;
    }
    return TRUE;
    // } catch (RemoteException e) {
    //     Logger::E(TAG, "Failed to fixperms container " + cid + " with exception " + e);
    // }
}

} // namespace Content
} // namespace Internal
} // namespace Droid
} // namespace Elastos
