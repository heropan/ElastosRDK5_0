/*
  * Copyright (C) 2008 The Android Open Source Project
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

#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_ICONUTILITIES_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_ICONUTILITIES_H__

#include "elastos/droid/ext/frameworkext.h"

// package com.android.internal.policy.impl;
// import android.graphics.drawable.BitmapDrawable;
// import android.graphics.drawable.Drawable;
// import android.graphics.drawable.PaintDrawable;
// import android.graphics.drawable.StateListDrawable;
// import android.graphics.Bitmap;
// import android.graphics.BlurMaskFilter;
// import android.graphics.Canvas;
// import android.graphics.ColorMatrix;
// import android.graphics.Paint;
// import android.graphics.PaintFlagsDrawFilter;
// import android.graphics.PorterDuff;
// import android.graphics.Rect;
// import android.graphics.TableMaskFilter;
// import android.util.DisplayMetrics;
// import android.util.TypedValue;
// import android.content.res.Resources;
// import android.content.Context;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

/**
  * Various utilities shared amongst the Launcher's classes.
  */
class IconUtilities
    : public Object
{
public:
    IconUtilities(
        /* [in] */ IContext* context);

    virtual CARAPI CreateIconDrawable(
        /* [in] */ IDrawable* src,
        /* [out] */ Drawable** result);

private:
    /**
      * Returns a bitmap suitable for the all apps view.  The bitmap will be a power
      * of two sized ARGB_8888 bitmap that can be used as a gl texture.
      */
    CARAPI_(AutoPtr<IBitmap>) CreateIconBitmap(
        /* [in] */ IDrawable* icon);

    CARAPI_(AutoPtr<IBitmap>) CreateSelectedBitmap(
        /* [in] */ IBitmap* src,
        /* [in] */ Boolean pressed);

private:
    static const String TAG;
    static const Int32 sColors[] = { 0xffff0000, 0xff00ff00, 0xff0000ff };
    Int32 mIconWidth;
    Int32 mIconHeight;
    Int32 mIconTextureWidth;
    Int32 mIconTextureHeight;
    /*const*/ AutoPtr<IPaint> mPaint;
    /*const*/ AutoPtr<IPaint> mBlurPaint;
    /*const*/ AutoPtr<IPaint> mGlowColorPressedPaint;
    /*const*/ AutoPtr<IPaint> mGlowColorFocusedPaint;
    /*const*/ AutoPtr<IRect> mOldBounds;
    /*const*/ AutoPtr<ICanvas> mCanvas;
    /*const*/ AutoPtr<IDisplayMetrics> mDisplayMetrics;
    Int32 mColorIndex;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_ICONUTILITIES_H__

