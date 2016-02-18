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

package com.android.settings.deviceinfo;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::Drawable::IShapeDrawable;
using Elastos::Droid::Graphics::Drawable::Shapes::IRectShape;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Preference::IPreference;

using Elastos::Droid::Settings::IR;

public class StorageItemPreference extends Preference {
    public final Int32 color;
    public final Int32 userHandle;

    public StorageItemPreference(Context context, Int32 titleRes, Int32 colorRes) {
        This(context, context->GetText(titleRes), colorRes, UserHandle.USER_NULL);
    }

    public StorageItemPreference(
            Context context, CharSequence title, Int32 colorRes, Int32 userHandle) {
        Super(context);

        if (colorRes != 0) {
            this.color = context->GetResources()->GetColor(colorRes);

            final Resources res = context->GetResources();
            final Int32 width = res->GetDimensionPixelSize(R.dimen.device_memory_usage_button_width);
            final Int32 height = res->GetDimensionPixelSize(R.dimen.device_memory_usage_button_height);
            SetIcon(CreateRectShape(width, height, this.color));
        } else {
            this.color = Color.MAGENTA;
        }

        SetTitle(title);
        SetSummary(R::string::memory_calculating_size);

        this.userHandle = userHandle;
    }

    private static ShapeDrawable CreateRectShape(Int32 width, Int32 height, Int32 color) {
        ShapeDrawable shape = new ShapeDrawable(new RectShape());
        shape->SetIntrinsicHeight(height);
        shape->SetIntrinsicWidth(width);
        shape->GetPaint()->SetColor(color);
        return shape;
    }
}
