// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_DROPDOWNDIVIDERDRAWABLE_H_
#define _ELASTOS_DROID_WEBKIT_UI_DROPDOWNDIVIDERDRAWABLE_H_

//package org.chromium.ui;

//import android.graphics.Canvas;
//import android.graphics.ColorFilter;
//import android.graphics.Paint;
//import android.graphics.PixelFormat;
//import android.graphics.Rect;
//import android.graphics.drawable.Drawable;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

class DropdownDividerDrawable
    : public Object
    : public IDrawable
{
public:
    DropdownDividerDrawable();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    //@Override
    CARAPI OnBoundsChange(
        /* [in] */ IRect* bounds);

    virtual CARAPI SetHeight(
        /* [in] */ Int32 height);

    virtual CARAPI SetColor(
        /* [in] */ Int32 color);

    //@Override
    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    //@Override
    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    //@Override
    CARAPI_(Int32) GetOpacity();

private:
    AutoPtr<IPaint> mPaint;
    AutoPtr<IRect> mDividerRect;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_DROPDOWNDIVIDERDRAWABLE_H_

