/*
 * Copyright (C) 2014 The Android Open Source Project
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

package com.android.settings.dashboard;

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IAlertDialog;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::IDialogFragment;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Settings::IR;

public class NoHomeDialogFragment extends DialogFragment {
    public static void Show(Activity parent) {
        final NoHomeDialogFragment dialog = new NoHomeDialogFragment();
        dialog->Show(parent->GetFragmentManager(), NULL);
    }

    //@Override
    public Dialog OnCreateDialog(Bundle savedInstanceState) {
        return new AlertDialog->Builder(GetActivity())
                .SetMessage(R::string::only_one_home_message)
                .SetPositiveButton(android.R::string::ok, NULL)
                .Create();
    }
}
