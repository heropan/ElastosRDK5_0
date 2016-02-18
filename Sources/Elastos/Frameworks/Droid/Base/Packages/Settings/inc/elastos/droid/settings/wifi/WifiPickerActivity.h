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
package com.android.settings.wifi;

using Elastos::Droid::Settings::IButtonBarHandler;
using Elastos::Droid::Settings::ISettingsActivity;
using Elastos::Droid::Settings::Wifi::P2p::IWifiP2pSettings;
using Elastos::Droid::Settings::IR;

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Preference::IPreferenceFragment;

using Elastos::Lang::IClass;

public class WifiPickerActivity extends SettingsActivity implements ButtonBarHandler {

    //@Override
    public Intent GetIntent() {
        Intent modIntent = new Intent(super->GetIntent());
        if (!modIntent->HasExtra(EXTRA_SHOW_FRAGMENT)) {
            modIntent->PutExtra(EXTRA_SHOW_FRAGMENT, GetWifiSettingsClass()->GetName());
            modIntent->PutExtra(EXTRA_SHOW_FRAGMENT_TITLE_RESID, R::string::wifi_select_network);
        }
        return modIntent;
    }

    //@Override
    protected Boolean IsValidFragment(String fragmentName) {
        if (WifiSettings.class->GetName()->Equals(fragmentName)
                || WifiP2pSettings.class->GetName()->Equals(fragmentName)
                || SavedAccessPointsWifiSettings.class->GetName()->Equals(fragmentName)
                || AdvancedWifiSettings.class->GetName()->Equals(fragmentName)) return TRUE;
        return FALSE;
    }

    /* package */ Class<? extends PreferenceFragment> GetWifiSettingsClass() {
        return WifiSettings.class;
    }
}
