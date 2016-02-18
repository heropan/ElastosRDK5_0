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

package com.android.settings.applications;

using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IResources;

class InterestingConfigChanges {
    final Configuration mLastConfiguration = new Configuration();
    Int32 mLastDensity;

    Boolean ApplyNewConfig(Resources res) {
        Int32 configChanges = mLastConfiguration->UpdateFrom(res->GetConfiguration());
        Boolean densityChanged = mLastDensity != res->GetDisplayMetrics().densityDpi;
        if (densityChanged || (configChanges&(ActivityInfo.CONFIG_LOCALE
                |ActivityInfo.CONFIG_UI_MODE|ActivityInfo.CONFIG_SCREEN_LAYOUT)) != 0) {
            mLastDensity = res->GetDisplayMetrics().densityDpi;
            return TRUE;
        }
        return FALSE;
    }
}
