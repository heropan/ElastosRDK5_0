// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_BASE_VIEWANDROID_H_
#define _ELASTOS_DROID_WEBKIT_UI_BASE_VIEWANDROID_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/native/ui/base/WindowAndroid.h"
#include "elastos/droid/webkit/native/ui/base/ViewAndroidDelegate.h"

// package org.chromium.ui.base;
// import android.view.View;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::View::IView;
using Elastos::Droid::Webkit::Ui::Base::WindowAndroid;
using Elastos::Droid::Webkit::Ui::Base::ViewAndroidDelegate;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

/**
  * From the Chromium architecture point of view, ViewAndroid and its native counterpart
  * serve purpose of representing Android view where Chrome expects to have a cross platform
  * handle to the system view type. As Views are Java object on Android, this ViewAndroid
  * and its native counterpart provide the expected abstractions on the C++ side and allow
  * it to be flexibly glued to an actual Android Java View at runtime.
  *
  * It should only be used where access to Android Views is needed from the C++ code.
  */
// @JNINamespace("ui")
class ViewAndroid
    : public Object
{
public:
    /**
      * Constructs a View object.
      */
    ViewAndroid(
        /* [in] */ WindowAndroid* nativeWindow,
        /* [in] */ ViewAndroidDelegate* viewAndroidDelegate);

    virtual CARAPI_(AutoPtr<ViewAndroidDelegate>) GetViewAndroidDelegate();

    /**
      * Destroys the c++ ViewAndroid object if one has been created.
      */
    virtual CARAPI Destroy();

    /**
      * Returns a pointer to the c++ AndroidWindow object.
      * @return A pointer to the c++ AndroidWindow.
      */
    virtual CARAPI_(Int64) GetNativePointer();

    /**
      * Set KeepScreenOn flag. If the flag already set, increase mKeepScreenOnCount.
      */
    virtual CARAPI IncrementKeepScreenOnCount();

    /**
      * Decrease mKeepScreenOnCount, if it is decreased to 0, remove the flag.
      */
    virtual CARAPI DecrementKeepScreenOnCount();

private:
    CARAPI_(Int64) NativeInit(
        /* [in] */ Int64 windowPtr);

    CARAPI NativeDestroy(
        /* [in] */ Int64 nativeViewAndroid);

private:
    // Native pointer to the c++ ViewAndroid object.
    Int64 mNativeViewAndroid;
    /*const*/ AutoPtr<ViewAndroidDelegate> mViewAndroidDelegate;
    /*const*/ AutoPtr<WindowAndroid> mWindowAndroid;
    Int32 mKeepScreenOnCount;
    AutoPtr<IView> mKeepScreenOnView;
};

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_BASE_VIEWANDROID_H_

