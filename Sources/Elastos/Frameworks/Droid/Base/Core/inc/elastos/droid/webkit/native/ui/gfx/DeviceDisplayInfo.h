// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_GFX_DEVICEDISPLAYINFO_H_
#define _ELASTOS_DROID_WEBKIT_UI_GFX_DEVICEDISPLAYINFO_H_

#include "ext/frameworkext.h"

// package org.chromium.ui.gfx;
// import android.content.Context;
// import android.graphics.PixelFormat;
// import android.graphics.Point;
// import android.os.Build;
// import android.util.DisplayMetrics;
// import android.view.Display;
// import android.view.Surface;
// import android.view.WindowManager;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gfx {

/**
  * This class facilitates access to android information typically only
  * available using the Java SDK, including {@link Display} properties.
  *
  * Currently the information consists of very raw display information (height, width, DPI scale)
  * regarding the main display.
  */
// @JNINamespace("gfx")
class DeviceDisplayInfo : public Object
{
public:
    /**
      * @return Display height in physical pixels.
      */
    // @CalledByNative
    virtual CARAPI_(Int32) GetDisplayHeight();

    /**
      * @return Display width in physical pixels.
      */
    // @CalledByNative
    virtual CARAPI_(Int32) GetDisplayWidth();

    /**
      * @return Real physical display height in physical pixels.
      */
    // @CalledByNative
    virtual CARAPI_(Int32) GetPhysicalDisplayHeight();

    /**
      * @return Real physical display width in physical pixels.
      */
    // @CalledByNative
    virtual CARAPI_(Int32) GetPhysicalDisplayWidth();

    /**
      * @return Bits per pixel.
      */
    // @CalledByNative
    virtual CARAPI_(Int32) GetBitsPerPixel();

    /**
      * @return Bits per component.
      */
    // @SuppressWarnings("deprecation")
    // @CalledByNative
    virtual CARAPI_(Int32) GetBitsPerComponent();

    /**
      * @return A scaling factor for the Density Independent Pixel unit. 1.0 is
      *         160dpi, 0.75 is 120dpi, 2.0 is 320dpi.
      */
    // @CalledByNative
    virtual CARAPI_(Double) GetDIPScale();

    /**
      * Inform the native implementation to update its cached representation of
      * the DeviceDisplayInfo values.
      */
    virtual CARAPI UpdateNativeSharedDisplayInfo();

    /**
      * Creates DeviceDisplayInfo for a given Context.
      *
      * @param context A context to use.
      * @return DeviceDisplayInfo associated with a given Context.
      */
    // @CalledByNative
    static CARAPI_(AutoPtr<DeviceDisplayInfo>) Create(
        /* [in] */ IContext* context);

private:
    DeviceDisplayInfo(
        /* [in] */ IContext* context);

    // @SuppressWarnings("deprecation")
    CARAPI_(Int32) GetPixelFormat();

    /**
      * @return Smallest screen size in density-independent pixels that the
      *         application will see, regardless of orientation.
      */
    // @CalledByNative
    CARAPI_(Int32) GetSmallestDIPWidth();

    /**
      * @return the screen's rotation angle from its 'natural' orientation.
      * Expected values are one of { 0, 90, 180, 270 }.
      * See http://developer.android.com/reference/android/view/Display.html#getRotation()
      * for more information about Display.getRotation() behavior.
      */
    // @CalledByNative
    CARAPI_(Int32) GetRotationDegrees();

    CARAPI_(AutoPtr<IDisplay>) GetDisplay();

    CARAPI NativeUpdateSharedDeviceDisplayInfo(
        /* [in] */ Int32 displayHeight,
        /* [in] */ Int32 displayWidth,
        /* [in] */ Int32 physicalDisplayHeight,
        /* [in] */ Int32 physicalDisplayWidth,
        /* [in] */ Int32 bitsPerPixel,
        /* [in] */ Int32 bitsPerComponent,
        /* [in] */ Double dipScale,
        /* [in] */ Int32 smallestDIPWidth,
        /* [in] */ Int32 rotationDegrees);

private:
    /*const*/ AutoPtr<IContext> mAppContext;
    /*const*/ AutoPtr<IWindowManager> mWinManager;
    AutoPtr<IPoint> mTempPoint;
    AutoPtr<IDisplayMetrics> mTempMetrics;
};

} // namespace Gfx
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_GFX_DEVICEDISPLAYINFO_H_

