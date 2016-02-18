// Copyright 2011 Google Inc. All Rights Reserved.

package com.android.settings.deviceinfo;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::Environment::IUserEnvironment;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IViewDebug;
using Elastos::Droid::Widget::ICheckBox;
using Elastos::Droid::Widget::ICheckable;
using Elastos::Droid::Widget::IRelativeLayout;
using Elastos::Droid::Widget::ITextView;

using Elastos::Droid::Settings::IR;

/**
 * Handles display of a single row entry on Settings --> Storage --> Misc Files screen
 */
public class FileItemInfoLayout extends RelativeLayout implements Checkable {
    private TextView mFileNameView;
    private TextView mFileSizeView;
    private CheckBox mCheckbox;

    private static const Int32 sLengthExternalStorageDirPrefix = new UserEnvironment(
            UserHandle->MyUserId()).GetExternalStorageDirectory()->GetAbsolutePath().Length() + 1;

    public FileItemInfoLayout(Context context) {
        This(context, NULL);
    }

    public FileItemInfoLayout(Context context, AttributeSet attrs) {
        This(context, attrs, 0);
    }

    public FileItemInfoLayout(Context context, AttributeSet attrs, Int32 defStyle) {
        Super(context, attrs, defStyle);
    }

    CARAPI Toggle() {
        SetChecked(!mCheckbox->IsChecked());
    }

    /* (non-Javadoc)
     * @see android.view.View#OnFinishInflate()
     */
    //@Override
    protected void OnFinishInflate() {
        super->OnFinishInflate();
        mFileNameView = (TextView) FindViewById(R.id.misc_filename);
        mFileSizeView = (TextView) FindViewById(R.id.misc_filesize);
        mCheckbox = (CheckBox) FindViewById(R.id.misc_checkbox);
    }

    CARAPI SetFileName(String fileName) {
        mFileNameView->SetText(fileName->Substring(sLengthExternalStorageDirPrefix));
    }

    CARAPI SetFileSize(String filesize) {
        mFileSizeView->SetText(filesize);
    }

    @ViewDebug.ExportedProperty
    public Boolean IsChecked() {
        return mCheckbox->IsChecked();
    }

    public CheckBox GetCheckBox() {
        return mCheckbox;
    }

    /**
     * <p>Changes the checked state of this text view.</p>
     *
     * @param checked TRUE to check the text, FALSE to uncheck it
     */
    CARAPI SetChecked(Boolean checked) {
        mCheckbox->SetChecked(checked);
    }
}