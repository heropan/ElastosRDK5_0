
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

const String SelectFileDialog::IMAGE_TYPE("image/");
const String SelectFileDialog::VIDEO_TYPE("video/");
const String SelectFileDialog::AUDIO_TYPE("audio/");
const String SelectFileDialog::ALL_IMAGE_TYPES(IMAGE_TYPE + "*");
const String SelectFileDialog::ALL_VIDEO_TYPES(VIDEO_TYPE + "*");
const String SelectFileDialog::ALL_AUDIO_TYPES(AUDIO_TYPE + "*");
const String SelectFileDialog::ANY_TYPES("*/*");
const String SelectFileDialog::CAPTURE_IMAGE_DIRECTORY("browser-photos");

//===============================================================
//                 SelectFileDialog::GetDisplayNameTask
//===============================================================
SelectFileDialog::GetDisplayNameTask::GetDisplayNameTask(
    /* in */ SelectFileDialog* owner,
    /* in */ IContentResolver* contentResolver,
    /* in */ Boolean isMultiple)
    : mOwner(owner)
    , mContentResolver(contentResolver)
    , mIsMultiple(isMultiple)
{
}

//@Override
AutoPtr< ArrayOf<String> > SelectFileDialog::GetDisplayNameTask::DoInBackground(
    /* in */ Uri...uris)
{
    mFilePaths = ArrayOf::Alloc<String>(uris.length);
    AutoPtr< ArrayOf<String> > dispalyNames = ArrayOf::Alloc<String>(uris.length);
    for (Int32 i = 0; i < uris.length; i++) {
        (*mFilePaths)[i] = uris[i].ToString();
        CContentUriUtils::GetDisplayName(uris[i], mContentResolver, IMediaStore::MediaColumns::DISPLAY_NAME, &(*displayNames)[i]);
    }
    return displayNames;
}

//@Override
ECode SelectFileDialog::GetDisplayNameTask::OnPostExecute(
    /* in */ ArrayOf<String>* result)
{
    VALIDATE_NOT_NULL(result);
    if (!mIsMultiple) {
        mOwner->NativeOnFileSelected(mOwner->mNativeSelectFileDialog, (*mFilePaths)[0], (*result)[0]);
    }
    return NOERROR;
}

//===============================================================
//                         SelectFileDialog
//===============================================================
//@Override
ECode SelectFileDialog::OnIntentCompleted(
    /* in */ WindowAndroid* window,
    /* in */ Int32 resultCode,
    /* in */ IContentResolver* contentResolver,
    /* in */ IIntent* results)
{
    VALIDATE_NOT_NULL(window);
    VALIDATE_NOT_NULL(contentResolver);
    VALIDATE_NOT_NULL(results);
    if (resultCode != IActivity::RESULT_OK) {
        OnFileNotSelected();
        return NOERROR;
    }

    if (NULL == results) {
        // If we have a successful return but no data, then assume this is the camera returning
        // the photo that we requested.
        String path;
        mCameraOutputUri->GetPath(&path);
        NativeOnFileSelected(mNativeSelectFileDialog, path, String(""));

        // Broadcast to the media scanner that there's a new photo on the device so it will
        // show up right away in the gallery (rather than waiting until the next time the media
        // scanner runs).
        AutoPtr<IIntent> intent;
        CIntent::New(IIntent::ACTION_MEDIA_SCANNER_SCAN_FILE, mCameraOutputUri, (IIntent**)&intent);
        window->SendBroadcast(intent);
        return NOERROR;
    }

    AutoPtr<IUri> url;
    results->GetData((IUri**)&url);
    String scheme;
    url->GetScheme(&scheme);
    if (IContentResolver::SCHEME_FILE == scheme)) {
        String specifyPart;
        url->GetSchemeSpecificPart(&specifyPart);
        NativeOnFileSelected(mNativeSelectFileDialog, specifyPart, String(""));
        return NOERROR;
    }

    results->GetScheme(&scheme);
    if (IContentResolver::SCHEME_CONTENT == scheme)) {
        AutoPtr<GetDisplayNameTask> task = new GetDisplayNameTask(this, contentResolver, FALSE);
        task->Execute(url);
        return NOERROR;
    }

    OnFileNotSelected();
    window->ShowError(R::string::opening_file_error);
    return NOERROR;
}

SelectFileDialog::SelectFileDialog(
    /* in */ Int64 nativeSelectFileDialog)
    : mNativeSelectFileDialog(nativeSelectFileDialog)
    , mCapture(FALSE)
{
}

ECode SelectFileDialog::SelectFile(
    /* in */ ArrayOf<String>* fileTypes,
    /* in */ Boolean capture,
    /* in */ WindowAndroid* window)
{
    VALIDATE_NOT_NULL(fileTypes);
    VALIDATE_NOT_NULL(window);

    mFileTypes = ArrayList<String>::Alloc(fileTypes->GetLength());
    mCapture = capture;

    AutoPtr<IIntent> chooser;
    AutoPtr<IIntent> camera;
    CIntent::New(IIntent::ACTION_CHOOSER, (IIntent**)&chooser);
    CIntent::New(IMediaStore::ACTION_IMAGE_CAPTURE, (IIntent**)&camera);

    CUri::FromFile(GetFileForImageCapture(), (IUri**)&mCameraOutputUri);
    camera->PutExtra(IMediaStore::EXTRA_OUTPUT, mCameraOutputUri);

    AutoPtr<IIntent> camcorder;
    AutoPtr<IIntent> soundRecorder;
    CIntent::New(IMediaStore::ACTION_VIDEO_CAPTURE, (IIntent**)&camcorder);
    CIntent::New(IMediaStore::Media::RECORD_SOUND_ACTION, (IIntent**)&soundRecorder);

    // Quick check - if the |capture| parameter is set and |fileTypes| has the appropriate MIME
    // type, we should just launch the appropriate intent. Otherwise build up a chooser based on
    // the accept type and then display that to the user.
    if (CaptureCamera()) {
        if (window->ShowIntent(camera, this, R::string::low_memory_error))
            return NOERROR;
    }
    else if (CaptureCamcorder()) {
        if (window->ShowIntent(camcorder, this, R::string::low_memory_error))
            return NOERROR;
    }
    else if (CaptureMicrophone()) {
        if (window->ShowIntent(soundRecorder, this, R::string::low_memory_error))
            return NOERROR;
    }

    AutoPtr<IIntent> getContentIntent;
    CIntent::New(IIntent::ACTION_GET_CONTENT, (IIntent**)&getContentIntent);
    getContentIntent->AddCategory(IIntent::CATEGORY_OPENABLE);
    AutoPtr< ArrayOf< AutoPtr<IIntent> > > extraIntents = ArrayOf< AutoPtr<IIntent> >::Alloc(1);
    if (!NoSpecificType()) {
        // Create a chooser based on the accept type that was specified in the webpage. Note
        // that if the web page specified multiple accept types, we will have built a generic
        // chooser above.
        if (ShouldShowImageTypes()) {
            extraIntents->Add(camera);
            getContentIntent->SetType(ALL_IMAGE_TYPES);
        }
        else if (ShouldShowVideoTypes()) {
            extraIntents->Add(camcorder);
            getContentIntent->SetType(ALL_VIDEO_TYPES);
        }
        else if (ShouldShowAudioTypes()) {
            extraIntents->Add(soundRecorder);
            getContentIntent->SetType(ALL_AUDIO_TYPES);
        }
    }

    if (extraIntents->IsEmpty()) {
        // We couldn't resolve an accept type, so fallback to a generic chooser.
        getContentIntent->SetType(ANY_TYPES);
        extraIntents->Add(camera);
        extraIntents->Add(camcorder);
        extraIntents->Add(soundRecorder);
    }

    AutoPtr< ArrayOf< AutoPtr<IIntent> > > toArray;
    extraIntents->ToArray(&toArray)
    chooser->PutExtra(IIntent::EXTRA_INITIAL_INTENTS, &toArray);
    chooser->PutExtra(IIntent::EXTRA_INTENT, getContentIntent);

    if (!window->ShowIntent(chooser, this, R::string::low_memory_error)) {
        OnFileNotSelected();
    }

    return NOERROR;
}

AutoPtr<IFile> SelectFileDialog::GetFileForImageCapture()
{
    AutoPtr<IFile> externalDataDir;
    CEnvironment::GetExternalStoragePublicDirectory(IEnvironment::DIRECTORY_DCIM, (IFile**)&externalDataDir);

    String absolutePath;
    externalDataDir->GetAbsolutePath(&absolutePath);
    AutoPtr<IFile> cameraDataDir;
    CFile::New(absolutePath + IFile::separator + CAPTURE_IMAGE_DIRECTORY, (IFile**)&cameraDataDir);

    Boolean exist;
    cameraDataDir->Exists(&exist);

    Boolean mkdir;
    cameraDataDir->Mkdirs(&mkdir);
    if (!exist && !mkdir) {
        cameraDataDir = externalDataDir;
    }

    cameraDataDir->GetAbsolutePath(&absolutePath);
    Int32 currTimeMill = 0;
    System::CurrentTimeMillis(&currTimeMill);
    String strCurrTimeMill = String::ForNum(currTimeMill);
    AutoPtr<IFile> photoFile;
    CFile::New(absolutePath + IFile::separator + strCurrTimeMill + String(".jpg"), (IFile**)&photoFile);

    return photoFile;
}

ECode SelectFileDialog::OnFileNotSelected()
{
    NativeOnFileNotSelected(mNativeSelectFileDialog);
    return NOERROR;
}

Boolean SelectFileDialog::NoSpecificType()
{
    // We use a single Intent to decide the type of the file chooser we display to the user,
    // which means we can only give it a single type. If there are multiple accept types
    // specified, we will fallback to a generic chooser (unless a capture parameter has been
    // specified, in which case we'll try to satisfy that first.
    return mFileTypes->Size() != 1 || mFileTypes->Contains(ANY_TYPES);
}

Boolean SelectFileDialog::ShouldShowTypes(
    /* in */ String allTypes,
    /* in */ String specificType)
{
    if (NoSpecificType() || mFileTypes->Contains(allTypes))
        return TRUE;
    return AcceptSpecificType(specificType);
}

Boolean SelectFileDialog::ShouldShowImageTypes()
{
    return ShouldShowTypes(ALL_IMAGE_TYPES, IMAGE_TYPE);
}

Boolean SelectFileDialog::ShouldShowVideoTypes()
{
    return ShouldShowTypes(ALL_VIDEO_TYPES, VIDEO_TYPE);
}

Boolean SelectFileDialog::ShouldShowAudioTypes()
{
    return ShouldShowTypes(ALL_AUDIO_TYPES, AUDIO_TYPE);
}

Boolean SelectFileDialog::AcceptsSpecificType(
    /* in */ String type)
{
    return mFileTypes->Size() == 1 && CTextUtils::Equals(mFileTypes->Get(0), type);
}

Boolean SelectFileDialog::CaptureCamera()
{
    return mCapture && AcceptsSpecificType(ALL_IMAGE_TYPES);
}

Boolean SelectFileDialog::CaptureCamcorder()
{
    return mCapture && AcceptsSpecificType(ALL_VIDEO_TYPES);
}

Boolean SelectFileDialog::CaptureMicrophone()
{
    return mCapture && AcceptsSpecificType(ALL_AUDIO_TYPES);
}

Boolean SelectFileDialog::AcceptSpecificType(
    /* in */ String accept)
{
    String type;
    for (Int32 i=0; i<mFileTypes->Size(); ++i)
    {
        type = mFileTypes->Get(i);
        if (type.StartsWith(accept)) {
            return TRUE;
        }
    }

    return FALSE;
}

//@CalledByNative
AutoPtr<SelectFileDialog> SelectFileDialog::Create(
    /* in */ Int64 nativeSelectFileDialog)
{
    AutoPtr<SelectFileDialog> ret = new SelectFileDialog(nativeSelectFileDialog);
    return ret;
}

ECode SelectFileDialog::NativeOnFileSelected(
    /* in */ Int64 nativeSelectFileDialogImpl,
    /* in */ String filePath,
    /* in */ String displayName)
{
    return NOERROR;
}

ECode SelectFileDialog::NativeOnFileNotSelected(
    /* in */ Int64 nativeSelectFileDialogImpl)
{
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

