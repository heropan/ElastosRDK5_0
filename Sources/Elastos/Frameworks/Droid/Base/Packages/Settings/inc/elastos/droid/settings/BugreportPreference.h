/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.settings;

using Elastos::Droid::App::AlertDialog::IBuilder;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Preference::IDialogPreference;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IView;

public class BugreportPreference extends DialogPreference {
    public BugreportPreference(Context context, AttributeSet attrs) {
        Super(context, attrs);
    }

    //@Override
    protected void OnPrepareDialogBuilder(Builder builder) {
        super->OnPrepareDialogBuilder(builder);
        builder->SetPositiveButton(R::string::report, this);
        builder->SetMessage(R::string::bugreport_message);
    }

    //@Override
    protected void ShowDialog(Bundle state) {
        super->ShowDialog(state);
    }

    //@Override
    protected void OnBindDialogView(View view) {
        super->OnBindDialogView(view);
    }

    //@Override
    protected void OnDialogClosed(Boolean positiveResult) {
        super->OnDialogClosed(positiveResult);
    }

    //@Override
    CARAPI OnClick(DialogInterface dialog, Int32 which) {
        if (which == DialogInterface.BUTTON_POSITIVE) {
            SystemProperties->Set("ctl.start", "bugreport");
        }
    }
}
