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

package com.android.settings.location;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Preference::IPreference;
using Elastos::Droid::Utility::IAttributeSet;

/**
 * A preference item that can dim the icon when it's disabled, either directly or because its parent
 * is disabled.
 */
public class DimmableIconPreference extends Preference {
    private static const Int32 ICON_ALPHA_ENABLED = 255;
    private static const Int32 ICON_ALPHA_DISABLED = 102;

    public DimmableIconPreference(Context context, AttributeSet attrs, Int32 defStyle) {
        Super(context, attrs, defStyle);
    }

    public DimmableIconPreference(Context context, AttributeSet attrs) {
        Super(context, attrs);
    }

    public DimmableIconPreference(Context context) {
        Super(context);
    }

    private void DimIcon(Boolean dimmed) {
        Drawable icon = GetIcon();
        if (icon != NULL) {
            icon->Mutate()->SetAlpha(dimmed ? ICON_ALPHA_DISABLED : ICON_ALPHA_ENABLED);
            SetIcon(icon);
        }
    }

    //@Override
    CARAPI OnParentChanged(Preference parent, Boolean disableChild) {
        DimIcon(disableChild);
        super->OnParentChanged(parent, disableChild);
    }

    //@Override
    CARAPI SetEnabled(Boolean enabled) {
        DimIcon(!enabled);
        super->SetEnabled(enabled);
    }
}
