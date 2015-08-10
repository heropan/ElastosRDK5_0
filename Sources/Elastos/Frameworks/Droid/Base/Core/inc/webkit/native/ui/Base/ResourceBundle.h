// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_RESOURCEBUNDLE_H_
#define _ELASTOS_DROID_WEBKIT_UI_RESOURCEBUNDLE_H_

// package org.chromium.ui.base;
// import android.content.Context;
// import android.content.res.AssetFileDescriptor;
// import android.content.res.AssetManager;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import java.io.IOException;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * This class provides the resource bundle related methods for the native library.
 */
class ResourceBundle
{
public:
    //@CalledByNative
    static CARAPI_(Boolean) AssetContainedInApk(
        /* in */ IContext* ctx,
        /* in */ String filename);
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_RESOURCEBUNDLE_H_

