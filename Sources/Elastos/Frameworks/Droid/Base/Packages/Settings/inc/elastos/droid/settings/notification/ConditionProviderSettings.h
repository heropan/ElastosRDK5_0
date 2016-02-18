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

package com.android.settings.notification;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Provider::ISettings;
using Elastos::Droid::Service::Notification::IConditionProviderService;

using Elastos::Droid::Settings::IR;

public class ConditionProviderSettings extends ManagedServiceSettings {
    private static const String TAG = ConditionProviderSettings.class->GetSimpleName();
    private static const Config CONFIG = GetConditionProviderConfig();

    private static Config GetConditionProviderConfig() {
        final Config c = new Config();
        c.tag = TAG;
        c.setting = Settings.Secure.ENABLED_CONDITION_PROVIDERS;
        c.intentAction = ConditionProviderService.SERVICE_INTERFACE;
        c.permission = Manifest::permission::BIND_CONDITION_PROVIDER_SERVICE;
        c.noun = "condition provider";
        c.warningDialogTitle = R::string::condition_provider_security_warning_title;
        c.warningDialogSummary = R::string::condition_provider_security_warning_summary;
        c.emptyText = R::string::no_condition_providers;
        return c;
    }

    //@Override
    protected Config GetConfig() {
        return CONFIG;
    }

    public static Int32 GetProviderCount(PackageManager pm) {
        return GetServicesCount(CONFIG, pm);
    }

    public static Int32 GetEnabledProviderCount(Context context) {
        return GetEnabledServicesCount(CONFIG, context);
    }
}
