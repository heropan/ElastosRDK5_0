// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_SURFACETEXTURELISTENER_H_
#define _ELASTOS_DROID_WEBKIT_UI_SURFACETEXTURELISTENER_H_

// package org.chromium.ui.gl;
// import android.graphics.SurfaceTexture;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * Listener to an android SurfaceTexture object for frame availability.
 */
// @JNINamespace("gfx")
class SurfaceTextureListener : public SurfaceTexture::OnFrameAvailableListener
{
public:
    SurfaceTextureListener(
        /* in */ Int64 nativeSurfaceTextureListener);

    // @Override
    CARAPI OnFrameAvailable(
    	/* in */ ISurfaceTexture* surfaceTexture);

protected:
    // @Override
    CARAPI Finalize();

private:
    CARAPI NativeFrameAvailable(
    	/* in */ Int64 nativeSurfaceTextureListener);

    CARAPI NativeDestroy(
    	/* in */ Int64 nativeSurfaceTextureListener);

private:
    // Used to determine the class instance to dispatch the native call to.
    Int64 mNativeSurfaceTextureListener;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_SURFACETEXTURELISTENER_H_

