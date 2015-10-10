// wuweizuo automatic build .h file from .java file.
// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_SCREENORIENTATIONPROVIDER_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_SCREENORIENTATIONPROVIDER_H_

#include "elastos/droid/ext/frameworkext.h"

// package org.chromium.content.browser;
// import android.app.Activity;
// import android.content.pm.ActivityInfo;
// import android.util.Log;
// import com.google.common.annotations.VisibleForTesting;
// import org.chromium.base.ApplicationStatus;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content.common.ScreenOrientationValues;

using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Util::ILog;
using Elastos::Droid::Webkit::Base::ApplicationStatus;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * This is the implementation of the C++ counterpart ScreenOrientationProvider.
  */
// @JNINamespace("content")
class ScreenOrientationProvider : public Object
{
public:
    // @VisibleForTesting
    // @CalledByNative
    static CARAPI_(AutoPtr<ScreenOrientationProvider>) Create();

    // @CalledByNative
    virtual CARAPI LockOrientation(
        /* [in] */ Byte orientations);

    // @CalledByNative
    virtual CARAPI UnlockOrientation();

private:
    ScreenOrientationProvider();

    CARAPI_(Int32) GetOrientationFromWebScreenOrientations(
        /* [in] */ Byte orientations);

private:
    static const String TAG;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_SCREENORIENTATIONPROVIDER_H_
