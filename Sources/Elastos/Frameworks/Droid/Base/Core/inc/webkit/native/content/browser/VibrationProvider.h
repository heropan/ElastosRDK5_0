// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_VIBRATIONPROVIDER_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_VIBRATIONPROVIDER_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/Context.h"
#include "content/pm/PackageManager.h"
#include "os/Vibrator.h"

// package org.chromium.content.browser;
// import android.content.Context;
// import android.content.pm.PackageManager;
// import android.media.AudioManager;
// import android.os.Vibrator;
// import android.util.Log;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Os::IVibrator;
using Elastos::Droid::Util::ILog;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * This is the implementation of the C++ counterpart VibrationProvider.
  */
// @JNINamespace("content")
class VibrationProvider : public Object
{
private:
    VibrationProvider(
        /* [in] */ IContext* context);

    // @CalledByNative
    static CARAPI_(AutoPtr<VibrationProvider>) Create(
        /* [in] */ IContext* context);

    // @CalledByNative
    CARAPI Vibrate(
        /* [in] */ Int64 milliseconds);

    // @CalledByNative
    CARAPI CancelVibration();

private:
    static const String TAG;
    /*const*/ AutoPtr<IAudioManager> mAudioManager;
    /*const*/ AutoPtr<IVibrator> mVibrator;
    /*const*/ Boolean mHasVibratePermission;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_VIBRATIONPROVIDER_H_

