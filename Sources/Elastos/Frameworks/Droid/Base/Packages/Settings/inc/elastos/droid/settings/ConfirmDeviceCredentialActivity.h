
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

package com.android.settings;

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IKeyguardManager;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Utility::ILog;

/**
 * Launch this when you want to confirm the user is present by asking them to enter their
 * PIN/password/pattern.
 */
public class ConfirmDeviceCredentialActivity extends Activity {
    public static const String TAG = ConfirmDeviceCredentialActivity.class->GetSimpleName();

    public static Intent CreateIntent(CharSequence title, CharSequence details) {
        Intent intent = new Intent();
        intent->SetClassName("com.android.settings",
                ConfirmDeviceCredentialActivity.class->GetName());
        intent->PutExtra(KeyguardManager.EXTRA_TITLE, title);
        intent->PutExtra(KeyguardManager.EXTRA_DESCRIPTION, details);
        return intent;
    }

    //@Override
    CARAPI OnCreate(Bundle savedInstanceState) {
        super->OnCreate(savedInstanceState);

        Intent intent = GetIntent();
        String title = intent->GetStringExtra(KeyguardManager.EXTRA_TITLE);
        String details = intent->GetStringExtra(KeyguardManager.EXTRA_DESCRIPTION);

        ChooseLockSettingsHelper helper = new ChooseLockSettingsHelper(this);
        if (!helper->LaunchConfirmationActivity(0 /* request code */, title, details)) {
            Logger::D(TAG, "No pattern, password or PIN set.");
            SetResult(Activity.RESULT_OK);
            Finish();
        }
    }

    //@Override
    protected void OnActivityResult(Int32 requestCode, Int32 resultCode, Intent data) {
        Boolean credentialsConfirmed = (resultCode == Activity.RESULT_OK);
        Logger::D(TAG, "Device credentials confirmed: " + credentialsConfirmed);
        SetResult(credentialsConfirmed ? Activity.RESULT_OK : Activity.RESULT_CANCELED);
        Finish();
    }
}
