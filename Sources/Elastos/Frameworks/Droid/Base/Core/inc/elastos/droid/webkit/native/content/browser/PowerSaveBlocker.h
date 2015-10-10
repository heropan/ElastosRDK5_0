// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_POWERSAVEBLOCKER_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_POWERSAVEBLOCKER_H_

#include "elastos/droid/ext/frameworkext.h"

// package org.chromium.content.browser;
// import org.chromium.base.CalledByNative;
// import org.chromium.ui.base.ViewAndroid;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

class PowerSaveBlocker
{
private:
    // @CalledByNative
    static CARAPI ApplyBlock(
        /* [in] */ ViewAndroid* view);

    // @CalledByNative
    static CARAPI RemoveBlock(
        /* [in] */ ViewAndroid* view);
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_POWERSAVEBLOCKER_H_
