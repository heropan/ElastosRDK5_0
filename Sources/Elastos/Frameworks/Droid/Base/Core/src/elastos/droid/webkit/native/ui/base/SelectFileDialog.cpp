// wuweizuo automatic build .cpp file from .java file.

#include "SelectFileDialog.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                 SelectFileDialog::GetDisplayNameTask
//=====================================================================
SelectFileDialog::GetDisplayNameTask::GetDisplayNameTask(
    /* [in] */ IContentResolver* contentResolver,
    /* [in] */ Boolean isMultiple)
{
    // ==================before translated======================
    // mContentResolver = contentResolver;
    // mIsMultiple = isMultiple;
}

AutoPtr< ArrayOf<String> > SelectFileDialog::GetDisplayNameTask::DoInBackground(
    /* [in] */  Uri)
{
    // ==================before translated======================
    // mFilePaths = new String[uris.length];
    // String[] displayNames = new String[uris.length];
    // for (int i = 0; i < uris.length; i++) {
    //     mFilePaths[i] = uris[i].toString();
    //     displayNames[i] = ContentUriUtils.getDisplayName(
    //             uris[i], mContentResolver, MediaStore.MediaColumns.DISPLAY_NAME);
    // }
    // return displayNames;
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

ECode SelectFileDialog::GetDisplayNameTask::OnPostExecute(
    /* [in] */ ArrayOf<String>* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (!mIsMultiple) {
    //     nativeOnFileSelected(mNativeSelectFileDialog, mFilePaths[0], result[0]);
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                           SelectFileDialog
//=====================================================================
const String SelectFileDialog::IMAGE_TYPE("image/");
const String SelectFileDialog::VIDEO_TYPE("video/");
const String SelectFileDialog::AUDIO_TYPE("audio/");
const String SelectFileDialog::ALL_IMAGE_TYPES(IMAGE_TYPE + "*");
const String SelectFileDialog::ALL_VIDEO_TYPES(VIDEO_TYPE + "*");
const String SelectFileDialog::ALL_AUDIO_TYPES(AUDIO_TYPE + "*");
const String SelectFileDialog::ANY_TYPES("*/*");
const String SelectFileDialog::CAPTURE_IMAGE_DIRECTORY("browser-photos");

ECode SelectFileDialog::OnIntentCompleted(
    /* [in] */ WindowAndroid* window,
    /* [in] */ Int32 resultCode,
    /* [in] */ IContentResolver* contentResolver,
    /* [in] */ IIntent* results)
{
    VALIDATE_NOT_NULL(window);
    VALIDATE_NOT_NULL(contentResolver);
    VALIDATE_NOT_NULL(results);
    // ==================before translated======================
    // if (resultCode != Activity.RESULT_OK) {
    //     onFileNotSelected();
    //     return;
    // }
    //
    // if (results == null) {
    //     // If we have a successful return but no data, then assume this is the camera returning
    //     // the photo that we requested.
    //     nativeOnFileSelected(mNativeSelectFileDialog, mCameraOutputUri.getPath(), "");
    //
    //     // Broadcast to the media scanner that there's a new photo on the device so it will
    //     // show up right away in the gallery (rather than waiting until the next time the media
    //     // scanner runs).
    //     window.sendBroadcast(new Intent(
    //             Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, mCameraOutputUri));
    //     return;
    // }
    //
    // if (ContentResolver.SCHEME_FILE.equals(results.getData().getScheme())) {
    //     nativeOnFileSelected(mNativeSelectFileDialog,
    //             results.getData().getSchemeSpecificPart(), "");
    //     return;
    // }
    //
    // if (ContentResolver.SCHEME_CONTENT.equals(results.getScheme())) {
    //     GetDisplayNameTask task = new GetDisplayNameTask(contentResolver, false);
    //     task.execute(results.getData());
    //     return;
    // }
    //
    // onFileNotSelected();
    // window.showError(R.string.opening_file_error);
    assert(0);
    return NOERROR;
}

SelectFileDialog::SelectFileDialog(
    /* [in] */ Int64 nativeSelectFileDialog)
{
    // ==================before translated======================
    // mNativeSelectFileDialog = nativeSelectFileDialog;
}

ECode SelectFileDialog::SelectFile(
    /* [in] */ ArrayOf<String>* fileTypes,
    /* [in] */ Boolean capture,
    /* [in] */ WindowAndroid* window)
{
    VALIDATE_NOT_NULL(fileTypes);
    VALIDATE_NOT_NULL(window);
    // ==================before translated======================
    // mFileTypes = new ArrayList<String>(Arrays.asList(fileTypes));
    // mCapture = capture;
    //
    // Intent chooser = new Intent(Intent.ACTION_CHOOSER);
    // Intent camera = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
    // mCameraOutputUri = Uri.fromFile(getFileForImageCapture());
    // camera.putExtra(MediaStore.EXTRA_OUTPUT, mCameraOutputUri);
    // Intent camcorder = new Intent(MediaStore.ACTION_VIDEO_CAPTURE);
    // Intent soundRecorder = new Intent(
    //         MediaStore.Audio.Media.RECORD_SOUND_ACTION);
    //
    // // Quick check - if the |capture| parameter is set and |fileTypes| has the appropriate MIME
    // // type, we should just launch the appropriate intent. Otherwise build up a chooser based on
    // // the accept type and then display that to the user.
    // if (captureCamera()) {
    //     if (window.showIntent(camera, this, R.string.low_memory_error)) return;
    // } else if (captureCamcorder()) {
    //     if (window.showIntent(camcorder, this, R.string.low_memory_error)) return;
    // } else if (captureMicrophone()) {
    //     if (window.showIntent(soundRecorder, this, R.string.low_memory_error)) return;
    // }
    //
    // Intent getContentIntent = new Intent(Intent.ACTION_GET_CONTENT);
    // getContentIntent.addCategory(Intent.CATEGORY_OPENABLE);
    // ArrayList<Intent> extraIntents = new ArrayList<Intent>();
    // if (!noSpecificType()) {
    //     // Create a chooser based on the accept type that was specified in the webpage. Note
    //     // that if the web page specified multiple accept types, we will have built a generic
    //     // chooser above.
    //     if (shouldShowImageTypes()) {
    //         extraIntents.add(camera);
    //         getContentIntent.setType(ALL_IMAGE_TYPES);
    //     } else if (shouldShowVideoTypes()) {
    //         extraIntents.add(camcorder);
    //         getContentIntent.setType(ALL_VIDEO_TYPES);
    //     } else if (shouldShowAudioTypes()) {
    //         extraIntents.add(soundRecorder);
    //         getContentIntent.setType(ALL_AUDIO_TYPES);
    //     }
    // }
    //
    // if (extraIntents.isEmpty()) {
    //     // We couldn't resolve an accept type, so fallback to a generic chooser.
    //     getContentIntent.setType(ANY_TYPES);
    //     extraIntents.add(camera);
    //     extraIntents.add(camcorder);
    //     extraIntents.add(soundRecorder);
    // }
    //
    // chooser.putExtra(Intent.EXTRA_INITIAL_INTENTS,
    //         extraIntents.toArray(new Intent[] { }));
    //
    // chooser.putExtra(Intent.EXTRA_INTENT, getContentIntent);
    //
    // if (!window.showIntent(chooser, this, R.string.low_memory_error)) {
    //     onFileNotSelected();
    // }
    assert(0);
    return NOERROR;
}

AutoPtr<IFile> SelectFileDialog::GetFileForImageCapture()
{
    // ==================before translated======================
    // File externalDataDir = Environment.getExternalStoragePublicDirectory(
    //         Environment.DIRECTORY_DCIM);
    // File cameraDataDir = new File(externalDataDir.getAbsolutePath() +
    //         File.separator + CAPTURE_IMAGE_DIRECTORY);
    // if (!cameraDataDir.exists() && !cameraDataDir.mkdirs()) {
    //     cameraDataDir = externalDataDir;
    // }
    // File photoFile = new File(cameraDataDir.getAbsolutePath() +
    //         File.separator + System.currentTimeMillis() + ".jpg");
    // return photoFile;
    assert(0);
    AutoPtr<IFile> empty;
    return empty;
}

ECode SelectFileDialog::OnFileNotSelected()
{
    // ==================before translated======================
    // nativeOnFileNotSelected(mNativeSelectFileDialog);
    assert(0);
    return NOERROR;
}

Boolean SelectFileDialog::NoSpecificType()
{
    // ==================before translated======================
    // // We use a single Intent to decide the type of the file chooser we display to the user,
    // // which means we can only give it a single type. If there are multiple accept types
    // // specified, we will fallback to a generic chooser (unless a capture parameter has been
    // // specified, in which case we'll try to satisfy that first.
    // return mFileTypes.size() != 1 || mFileTypes.contains(ANY_TYPES);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::ShouldShowTypes(
    /* [in] */ const String& allTypes,
    /* [in] */ const String& specificType)
{
    // ==================before translated======================
    // if (noSpecificType() || mFileTypes.contains(allTypes)) return true;
    // return acceptSpecificType(specificType);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::ShouldShowImageTypes()
{
    // ==================before translated======================
    // return shouldShowTypes(ALL_IMAGE_TYPES, IMAGE_TYPE);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::ShouldShowVideoTypes()
{
    // ==================before translated======================
    // return shouldShowTypes(ALL_VIDEO_TYPES, VIDEO_TYPE);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::ShouldShowAudioTypes()
{
    // ==================before translated======================
    // return shouldShowTypes(ALL_AUDIO_TYPES, AUDIO_TYPE);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::AcceptsSpecificType(
    /* [in] */ const String& type)
{
    // ==================before translated======================
    // return mFileTypes.size() == 1 && TextUtils.equals(mFileTypes.get(0), type);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::CaptureCamera()
{
    // ==================before translated======================
    // return mCapture && acceptsSpecificType(ALL_IMAGE_TYPES);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::CaptureCamcorder()
{
    // ==================before translated======================
    // return mCapture && acceptsSpecificType(ALL_VIDEO_TYPES);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::CaptureMicrophone()
{
    // ==================before translated======================
    // return mCapture && acceptsSpecificType(ALL_AUDIO_TYPES);
    assert(0);
    return FALSE;
}

Boolean SelectFileDialog::AcceptSpecificType(
    /* [in] */ const String& accept)
{
    // ==================before translated======================
    // for (String type : mFileTypes) {
    //     if (type.startsWith(accept)) {
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return FALSE;
}

AutoPtr<SelectFileDialog> SelectFileDialog::Create(
    /* [in] */ Int64 nativeSelectFileDialog)
{
    // ==================before translated======================
    // return new SelectFileDialog(nativeSelectFileDialog);
    assert(0);
    AutoPtr<SelectFileDialog> empty;
    return empty;
}

ECode SelectFileDialog::NativeOnFileSelected(
    /* [in] */ Int64 nativeSelectFileDialogImpl,
    /* [in] */ const String& filePath,
    /* [in] */ const String& displayName)
{
    assert(0);
    return NOERROR;
}

ECode SelectFileDialog::NativeOnFileNotSelected(
    /* [in] */ Int64 nativeSelectFileDialogImpl)
{
    assert(0);
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


