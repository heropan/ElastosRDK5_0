
#include "os/storage/CExternalStorageFormatter.h"

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {


ECode CExternalStorageFormatter::OnCreate()
{
    Service::OnCreate();

     mExternalStoragebeSD = Environment->IsExternalStorageBeSdcard();

    if (mStorageManager == NULL) {
        Context::GetSystemService(IContext::STORAGE_SERVICE, (IInterface**)(IStorageManager**)&mStorageManager);
        mStorageManager->RegisterListener(mStorageListener);
    }

//    mWakeLock = ((PowerManager)getSystemService(Context.POWER_SERVICE))
//            .newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "ExternalStorageFormatter");
//    mWakeLock.acquire();
    return NOERROR;
}

ECode CExternalStorageFormatter::OnStartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId
    /* [out] */ Int32 pCmd)
{
    if (FORMAT_AND_FACTORY_RESET.Equals(intent->GetAction())) {
        mFactoryReset = TRUE;
    }
    if (intent->GetBooleanExtra(EXTRA_ALWAYS_RESET, FALSE)) {
        mAlwaysReset = TRUE;
    }

    mStorageVolume = intent->GetParcelableExtra(StorageVolume::EXTRA_STORAGE_VOLUME);

    if (mProgressDialog == NULL) {
        mProgressDialog = new ProgressDialog(this);
        mProgressDialog->SetIndeterminate(TRUE);
        mProgressDialog->SetCancelable(TRUE);
        mProgressDialog->GetWindow()->SetType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        if (!mAlwaysReset) {
            mProgressDialog->SetOnCancelListener(this);
        }
        TryUnmountExternalStorage2();
        UpdateProgressState(NULL, NULL);
        mProgressDialog->Show();
    }

    return Service::START_REDELIVER_INTENT;
}

ECode CExternalStorageFormatter::OnDestroy()
{
    if (mStorageManager != NULL) {
        mStorageManager->UnregisterListener(mStorageListener);
    }
    if (mProgressDialog != NULL) {
        mProgressDialog->Dismiss();
    }
//    mWakeLock.release();
    Service::OnDestroy();
    return NOERROR;
}

ECode CExternalStorageFormatter::OnBind(
    /* [in] */ IIntent* intent
    /* [out] */ IBinder** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    return NOERROR;
}

ECode CExternalStorageFormatter::OnCancel(
    /* [in] */ DialogInterface dialog)
{
    AutoPtr<IMountService> mountService;
    GetMountService(&mountService);
    String extStoragePath = mStorageVolume == NULL ?
            Environment->GetLegacyExternalStorageDirectory()->ToString() :
            mStorageVolume->GetPath();
//    try {
        mountService->MountVolume(extStoragePath);
//    } catch (RemoteException e) {
//        Log.w(TAG, "Failed talking with mount service", e);
//    }
    if ((!mExternalStoragebeSD) &&
        (extStoragePath.Equals(Environment->GetExternalStorageDirectory().toString()))) {
//     Log.i(TAG, "cancel format on a internal sdcard volume,try mount external sdcard");
//     try {
         mountService->MountVolume( Environment->GetExternalStorage2Directory().toString());
//     } catch (RemoteException e) {
//         Log.w(TAG, "Failed talking with mount service when mount extsd2", e);
//     }
 }
    stopSelf();
    return NOERROR;
}

ECode CExternalStorageFormatter::Fail(
    /* [in] */ Int32 msg)
{
    Toast.makeText(this, msg, Toast.LENGTH_LONG).show();
    if (mAlwaysReset) {
        SendBroadcast(new Intent("android.intent.action.MASTER_CLEAR"));
    }
    stopSelf();
    return NOERROR;
}

ECode CExternalStorageFormatter::TryUnmountExternalStorage2()
{
    String extStoragePath = (mStorageVolume == NULL) ?
            Environment->GetExternalStorageDirectory().toString() :
            mStorageVolume->GetPath();
    mTryMountExtStorage2 = FALSE;
    if ((!mExternalStoragebeSD)&&
        (extStoragePath.Equals(Environment->GetExternalStorageDirectory().toString()))) {
//        Log.i(TAG, "unmount external storage2 first!");
        String status = Environment->GetExternalStorage2State();
        if (Environment::MEDIA_MOUNTED.Equals(status) ||
            Environment::MEDIA_MOUNTED_READ_ONLY.Equals(status)) {
            UpdateProgressDialog(R.string.progress_unmounting);
            AutoPtr<IMountService> mountService = GetMountService();
        String ext2StoragePath = Environment->GetExternalStorage2Directory().toString();
//            try {
            mountService.unmountVolume(ext2StoragePath, TRUE,FALSE);
//            } catch (RemoteException e) {
//                  Log.w(TAG, "Failed talking with mount service", e);
//            }
            mTryMountExtStorage2 = TRUE;
         }
     }
    return NOERROR;
 }

ECode CExternalStorageFormatter::UpdateProgressState(
    /* [in] */ const String& path,
    /* [in] */ const String& newState)
{
    if (mTryMountExtStorage2) {
        if (path==NULL)//wait for Storage2 status notify
            return;
        if (path.Equals(Environment->GetExternalStorage2Directory().toString())) {
            if ((!Environment::MEDIA_UNMOUNTED.Equals(newState) ) &&
                   (!Environment::MEDIA_REMOVED.Equals(newState) )) {
                   return;//bypass all the state change of Storage2 until it unmonted
           }
        }
    }
    String status = mStorageVolume == NULL ?
            Environment->GetExternalStorageState() :
            mStorageManager->GetVolumeState(mStorageVolume->GetPath());
    if (Environment::MEDIA_MOUNTED.Equals(status) ||
        Environment::MEDIA_MOUNTED_READ_ONLY.Equals(status)) {
        UpdateProgressDialog(R.string.progress_unmounting);
        AutoPtr<IMountService> mountService = g=GetMountService();
        String extStoragePath = mStorageVolume == NULL ?
                Environment->GetLegacyExternalStorageDirectory().toString() :
                mStorageVolume->GetPath();
//        try {
            // Remove encryption mapping if this is an unmount for a factory reset.
            mountService->UnmountVolume(extStoragePath, TRUE, mFactoryReset);
//        } catch (RemoteException e) {
//            Log.w(TAG, "Failed talking with mount service", e);
//        }
    } else if (Environment::MEDIA_NOFS.Equals(status) ||
                    Environment::MEDIA_UNMOUNTED.Equals(status) ||
                    Environment::MEDIA_UNMOUNTABLE.Equals(status)) {
        UpdateProgressDialog(R.string.progress_erasing);
        AutoPtr<IMountService> mountService = GetMountService();
        String extStoragePath = mStorageVolume == NULL ?
                Environment->GetLegacyExternalStorageDirectory().toString() :
                mStorageVolume->GetPath();
        if (mountService != NULL) {
            new Thread() {
                @Override
                public void run() {
                    Boolean success = FALSE;
//                    try {
                        mountService->FormatVolume(extStoragePath);
                        String ext2StoragePath = Environment->GetExternalStorage2Directory().toString();
                mountService->FormatVolume(ext2StoragePath);
                        success = TRUE;
//                    } catch (Exception e) {
//                        Toast.makeText(ExternalStorageFormatter.this,
//                                R.string.format_error, Toast.LENGTH_LONG).show();
//                    }
                    if (success) {
                        if (mFactoryReset) {
                            SendBroadcast(new Intent("android.intent.action.MASTER_CLEAR"));
                            // Intent handling is asynchronous -- assume it will happen soon.
                            stopSelf();
                            return;
                        }
                    }
                    // If we didn't succeed, or aren't doing a full factory
                    // reset, then it is time to remount the storage.
                    if (!success && mAlwaysReset) {
                        SendBroadcast(new Intent("android.intent.action.MASTER_CLEAR"));
                    } else {
//                        try {
                            mountService->MountVolume(extStoragePath);
                            if (mTryMountExtStorage2==TRUE) {
                                    String extStoragePath2= Environment->GetExternalStorage2Directory().toString();
                                    mountService->MountVolume(extStoragePath2);
                            }
//                        } catch (RemoteException e) {
//                            Log.w(TAG, "Failed talking with mount service", e);
//                        }
                    }
                    stopSelf();
                    return;
                }
            }.start();
        } else {
//            Log.w(TAG, "Unable to locate IMountService");
        }
    } else if (Environment::MEDIA_BAD_REMOVAL.Equals(status)) {
        Fail(R.string.media_bad_removal);
    } else if (Environment::MEDIA_CHECKING.Equals(status)) {
        Fail(R.string.media_checking);
    } else if (Environment::MEDIA_REMOVED.Equals(status)) {
        Fail(R.string.media_removed);
    } else if (Environment::MEDIA_SHARED.Equals(status)) {
        Fail(R.string.media_shared);
    } else {
        Fail(R.string.media_unknown_state);
//        Log.w(TAG, "Unknown storage state: " + status);
        stopSelf();
    }
    return NOERROR;
}

ECode CExternalStorageFormatter::UpdateProgressDialog(
    /* [in] */ Int32 msg)
{
    if (mProgressDialog == NULL) {
        mProgressDialog = new ProgressDialog(this);
        mProgressDialog.setIndeterminate(TRUE);
        mProgressDialog.setCancelable(FALSE);
        mProgressDialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        mProgressDialog.show();
    }

    mProgressDialog.setMessage(getText(msg));
    return NOERROR;
}

ECode CExternalStorageFormatter::GetMountService(
    /* [out] */ IMountService** result)
{
    VALIDATE_NOT_NULL(result);
    if (mMountService == NULL) {
        IBinder service = ServiceManager.GetService("mount");
        if (service != NULL) {
            mMountService = IMountService.Stub.asInterface(service);
        } else {
//            Log.e(TAG, "Can't get mount service");
        }
    }

    *result = mMountService;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos


