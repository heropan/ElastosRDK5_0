// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_SELECTFILEDIALOG_H_
#define _ELASTOS_DROID_WEBKIT_UI_SELECTFILEDIALOG_H_

// package org.chromium.ui.base;
// import android.app.Activity;
// import android.content.ContentResolver;
// import android.content.Intent;
// import android.net.Uri;
// import android.os.AsyncTask;
// import android.os.Environment;
// import android.provider.MediaStore;
// import android.text.TextUtils;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.ContentUriUtils;
// import org.chromium.base.JNINamespace;
// import org.chromium.ui.R;
// import java.io.File;
// import java.util.ArrayList;
// import java.util.Arrays;
// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * A dialog that is triggered from a file input field that allows a user to select a file based on
 * a set of accepted file types. The path of the selected file is passed to the native dialog.
 */
//@JNINamespace("ui")
class SelectFileDialog : public WindowAndroid::IntentCallback
{
private:
    class GetDisplayNameTask : public AsyncTask<Uri, Void, ArrayOf<String> >
    {
    public:
        GetDisplayNameTask(
            /* in */ SelectFileDialog* owner,
            /* in */ IContentResolver* contentResolver,
            /* in */ Boolean isMultiple);

    protected:
        //@Override
        CARAPI_(AutoPtr< ArrayOf<String> >) DoInBackground(
            /* in */ Uri...uris);

        //@Override
        CARAPI OnPostExecute(
            /* in */ ArrayOf<String>* result);

    public:
        AutoPtr< ArrayOf<String> > mFilePaths;
        const AutoPtr<IContentResolver> mContentResolver;
        const Boolean mIsMultiple;

    private:
        SelectFileDialog* mOwner;
    };

public:
    /**
     * Callback method to handle the intent results and pass on the path to the native
     * SelectFileDialog.
     * @param window The window that has access to the application activity.
     * @param resultCode The result code whether the intent returned successfully.
     * @param contentResolver The content resolver used to extract the path of the selected file.
     * @param results The results of the requested intent.
     */
    //@Override
    CARAPI OnIntentCompleted(
        /* in */ WindowAndroid* window,
        /* in */ Int32 resultCode,
        /* in */ IContentResolver* contentResolver,
        /* in */ IIntent* results);

private:
    SelectFileDialog(
        /* in */ Int64 nativeSelectFileDialog);

    /**
     * Creates and starts an intent based on the passed fileTypes and capture value.
     * @param fileTypes MIME types requested (i.e. "image/*")
     * @param capture The capture value as described in http://www.w3.org/TR/html-media-capture/
     * @param window The WindowAndroid that can show intents
     */
    //@CalledByNative
    CARAPI SelectFile(
        /* in */ ArrayOf<String>* fileTypes,
        /* in */ Boolean capture,
        /* in */ WindowAndroid* window);

    /**
     * Get a file for the image capture in the CAPTURE_IMAGE_DIRECTORY directory.
     */
    CARAPI_(AutoPtr<IFile> GetFileForImageCapture();

    CARAPI OnFileNotSelected();

    CARAPI_(Boolean) NoSpecificType();

    CARAPI_(Boolean) ShouldShowTypes(
        /* in */ String allTypes,
        /* in */ String specificType);

    CARAPI_(Boolean) ShouldShowImageTypes();

    CARAPI_(Boolean) ShouldShowVideoTypes();

    CARAPI_(Boolean) ShouldShowAudioTypes();

    CARAPI_(Boolean) AcceptsSpecificType(
        /* in */ String type);

    CARAPI_(Boolean) CaptureCamera();

    CARAPI_(Boolean) CaptureCamcorder();

    CARAPI_(Boolean) CaptureMicrophone();

    CARAPI_(Boolean) AcceptSpecificType(
        /* in */ String accept);

    //@CalledByNative
    static CARAPI_(AutoPtr<SelectFileDialog>) Create(
        /* in */ Int64 nativeSelectFileDialog);

    CARAPI NativeOnFileSelected(
        /* in */ Int64 nativeSelectFileDialogImpl,
        /* in */ String filePath,
        /* in */ String displayName);
    CARAPI NativeOnFileNotSelected(
        /* in */ Int64 nativeSelectFileDialogImpl);

private:
    static const String IMAGE_TYPE;
    static const String VIDEO_TYPE;
    static const String AUDIO_TYPE;
    static const String ALL_IMAGE_TYPES;
    static const String ALL_VIDEO_TYPES;
    static const String ALL_AUDIO_TYPES;
    static const String ANY_TYPES;
    static const String CAPTURE_IMAGE_DIRECTORY;

    const Int64 mNativeSelectFileDialog;
    AutoPtr< List<String> > mFileTypes;
    Boolean mCapture;
    AutoPtr<IUri> mCameraOutputUri;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_SELECTFILEDIALOG_H_

