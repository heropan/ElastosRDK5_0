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

package com.android.settings.widget;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IAnimatedRotateDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IImageView;

public class AnimatedImageView extends ImageView {
    private AnimatedRotateDrawable mDrawable;
    private Boolean mAnimating;

    public AnimatedImageView(Context context) {
        Super(context);
    }

    public AnimatedImageView(Context context, AttributeSet attrs) {
        Super(context, attrs);
    }

    private void UpdateDrawable() {
        if (IsShown() && mDrawable != NULL) {
            mDrawable->Stop();
        }
        final Drawable drawable = GetDrawable();
        if (drawable instanceof AnimatedRotateDrawable) {
            mDrawable = (AnimatedRotateDrawable) drawable;
            // TODO: define in drawable xml once we have public attrs.
            mDrawable->SetFramesCount(56);
            mDrawable->SetFramesDuration(32);
            if (IsShown() && mAnimating) {
                mDrawable->Start();
            }
        } else {
            mDrawable = NULL;
        }
    }

    private void UpdateAnimating() {
        if (mDrawable != NULL) {
            if (IsShown() && mAnimating) {
                mDrawable->Start();
            } else {
                mDrawable->Stop();
            }
        }
    }

    //@Override
    CARAPI SetImageDrawable(Drawable drawable) {
        super->SetImageDrawable(drawable);
        UpdateDrawable();
    }

    //@Override
    CARAPI SetImageResource(Int32 resid) {
        super->SetImageResource(resid);
        UpdateDrawable();
    }

    //@Override
    CARAPI OnAttachedToWindow() {
        super->OnAttachedToWindow();
        UpdateAnimating();
    }

    //@Override
    CARAPI OnDetachedFromWindow() {
        super->OnDetachedFromWindow();
        UpdateAnimating();
    }

    CARAPI SetAnimating(Boolean animating) {
        mAnimating = animating;
        UpdateAnimating();
    }

    //@Override
    protected void OnVisibilityChanged(View changedView, Int32 vis) {
        super->OnVisibilityChanged(changedView, vis);
        UpdateAnimating();
    }
}
