// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_BASE_ACTIVITYWINDOWANDROID_H_
#define _ELASTOS_DROID_WEBKIT_UI_BASE_ACTIVITYWINDOWANDROID_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/native/ui/base/WindowAndroid.h"

// package org.chromium.ui.base;
// import android.app.Activity;
// import android.app.PendingIntent;
// import android.content.ActivityNotFoundException;
// import android.content.Intent;
// import android.content.IntentSender.SendIntentException;
// import java.lang.ref.WeakReference;

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Webkit::Ui::Base::WindowAndroid;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

/**
  * The class provides the WindowAndroid's implementation which requires
  * Activity Instance.
  * Only instantiate this class when you need the implemented features.
  */
class ActivityWindowAndroid : public WindowAndroid
{
public:
    ActivityWindowAndroid(
        /* [in] */ IActivity* activity);

    // @Override
    CARAPI_(Int32) ShowCancelableIntent(
        /* [in] */ IPendingIntent* intent,
        /* [in] */ IntentCallback* callback,
        /* [in] */ Int32 errorId);

    // @Override
    CARAPI_(Int32) ShowCancelableIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ IntentCallback* callback,
        /* [in] */ Int32 errorId);

    // @Override
    CARAPI CancelIntent(
        /* [in] */ Int32 requestCode);

    // @Override
    CARAPI_(Boolean) OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);

    // @Override
    CARAPI_(AutoPtr<IWeakReference>) GetActivity();

private:
    CARAPI_(Int32) GenerateNextRequestCode();

    CARAPI StoreCallbackData(
        /* [in] */ Int32 requestCode,
        /* [in] */ IntentCallback* callback,
        /* [in] */ Int32 errorId);

private:
    // Constants used for intent request code bounding.
    static const Int32 REQUEST_CODE_PREFIX = 1000;
    static const Int32 REQUEST_CODE_RANGE_SIZE = 100;
    static const String TAG;
    /*const*/ AutoPtr<IWeakReference> mActivityRef;
    Int32 mNextRequestCode;
};

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_BASE_ACTIVITYWINDOWANDROID_H_

