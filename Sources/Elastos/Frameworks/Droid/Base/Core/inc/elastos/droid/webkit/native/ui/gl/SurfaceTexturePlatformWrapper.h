// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_GL_SURFACETEXTUREPLATFORMWRAPPER_H_
#define _ELASTOS_DROID_WEBKIT_UI_GL_SURFACETEXTUREPLATFORMWRAPPER_H_

#include "elastos/droid/ext/frameworkext.h"

// package org.chromium.ui.gl;
// import android.graphics.SurfaceTexture;
// import android.os.Build;
// import android.util.Log;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::Graphics::ISurfaceTexture;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gl {

/**
  * Wrapper class for the underlying platform's SurfaceTexture in order to
  * provide a stable JNI API.
  */
// @JNINamespace("gfx")
class SurfaceTexturePlatformWrapper
    : public Object
{
private:
    // @CalledByNative
    static CARAPI_(AutoPtr<ISurfaceTexture>) Create(
        /* [in] */ Int32 textureId);

    // @CalledByNative
    static CARAPI_(AutoPtr<ISurfaceTexture>) CreateSingleBuffered(
        /* [in] */ Int32 textureId);

    // @CalledByNative
    static CARAPI Destroy(
        /* [in] */ ISurfaceTexture* surfaceTexture);

    // @CalledByNative
    static CARAPI SetFrameAvailableCallback(
        /* [in] */ ISurfaceTexture* surfaceTexture,
        /* [in] */ Int64 nativeSurfaceTextureListener);

    // @CalledByNative
    static CARAPI UpdateTexImage(
        /* [in] */ ISurfaceTexture* surfaceTexture);

    // @CalledByNative
    static CARAPI ReleaseTexImage(
        /* [in] */ ISurfaceTexture* surfaceTexture);

    // @CalledByNative
    static CARAPI SetDefaultBufferSize(
        /* [in] */ ISurfaceTexture* surfaceTexture,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    // @CalledByNative
    static CARAPI GetTransformMatrix(
        /* [in] */ ISurfaceTexture* surfaceTexture,
        /* [in] */ ArrayOf<Float>* matrix);

    // @CalledByNative
    static CARAPI AttachToGLContext(
        /* [in] */ ISurfaceTexture* surfaceTexture,
        /* [in] */ Int32 texName);

    // @CalledByNative
    static CARAPI DetachFromGLContext(
        /* [in] */ ISurfaceTexture* surfaceTexture);

private:
    static const String TAG;
};

} // namespace Gl
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_GL_SURFACETEXTUREPLATFORMWRAPPER_H_

