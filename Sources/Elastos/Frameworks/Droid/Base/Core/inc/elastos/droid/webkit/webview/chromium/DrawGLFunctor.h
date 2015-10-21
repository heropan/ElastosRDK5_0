/*
  * Copyright (C) 2012 The Android Open Source Project
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

#ifndef _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_DRAWGLFUNCTOR_H_
#define _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_DRAWGLFUNCTOR_H_

#include "elastos/droid/ext/frameworkext.h"
//#include "elastos/droid/view/ViewRootImpl.h"
// has no now: #include "elastos/droid/webkit/native/content/common/CleanupReference.h"

// package com.android.webview.chromium;
// import android.view.HardwareCanvas;
// import android.view.ViewRootImpl;
// import android.util.Log;
// import org.chromium.content.common.CleanupReference;

using Elastos::Core::IRunnable;
using Elastos::Droid::View::IHardwareCanvas;
//using Elastos::Droid::View::ViewRootImpl;
//using Elastos::Droid::Webkit::Content::Common::CleanupReference;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

// Simple Java abstraction and wrapper for the native DrawGLFunctor flow.
// An instance of this class can be constructed, bound to a single view context (i.e. AwContennts)
// and then drawn and detached from the view tree any number of times (using requestDrawGL and
// detach respectively). Then when finished with, it can be explicitly released by calling
// destroy() or will clean itself up as required via finalizer / CleanupReference.
class DrawGLFunctor : public Object
{
public:
    // Holds the core resources of the class, everything required to correctly cleanup.
    // IMPORTANT: this class must not hold any reference back to the outer DrawGLFunctor
    // instance, as that will defeat GC of that object.
    class DestroyRunnable
        : public Object
        , public IRunnable
    {
    public:
        CAR_INTERFACE_DECL()

        DestroyRunnable(
            /* [in] */ Int64 nativeDrawGLFunctor);

        // Called when the outer DrawGLFunctor instance has been GC'ed, i.e this is its finalizer.
        // @Override
        CARAPI Run();

        virtual CARAPI DetachNativeFunctor();

    public:
        AutoPtr<IInterface/*ViewRootImpl*/> mViewRootImpl;
        Int64 mNativeDrawGLFunctor;
    };

public:
    DrawGLFunctor(
        /* [in] */ Int64 viewContext);

    virtual CARAPI Destroy();

    virtual CARAPI Detach();

    virtual CARAPI_(Boolean) RequestDrawGL(
        /* [in] */ IHardwareCanvas* canvas,
        /* [in] */ IInterface/*ViewRootImpl*/* viewRootImpl,
        /* [in] */ Boolean waitForCompletion);

    static CARAPI SetChromiumAwDrawGLFunction(
        /* [in] */ Int64 functionPointer);

private:
    static CARAPI_(Int64) NativeCreateGLFunctor(
        /* [in] */ Int64 viewContext);

    static CARAPI NativeDestroyGLFunctor(
        /* [in] */ Int64 functor);

    static CARAPI NativeSetChromiumAwDrawGLFunction(
        /* [in] */ Int64 functionPointer);

private:
    static const String TAG;
    // Pointer to native side instance
    AutoPtr<IInterface/*CleanupReference*/> mCleanupReference;
    AutoPtr<DestroyRunnable> mDestroyRunnable;
};

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_DRAWGLFUNCTOR_H_

