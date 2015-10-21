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

#ifndef _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_GEOLOCATIONPERMISSIONSADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_GEOLOCATIONPERMISSIONSADAPTER_H_

#include "elastos/droid/ext/frameworkext.h"
//#include "elastos/droid/webkit/GeolocationPermissions.h"
#include "elastos/droid/webkit/native/android_webview/AwGeolocationPermissions.h"

// package com.android.webview.chromium;
// import android.webkit.GeolocationPermissions;
// import android.webkit.ValueCallback;
// import org.chromium.android_webview.AwGeolocationPermissions;
// import java.util.Set;

//using Elastos::Droid::Webkit::IValueCallback;
//using Elastos::Droid::Webkit::GeolocationPermissions;
using Elastos::Droid::Webkit::AndroidWebview::AwGeolocationPermissions;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

/**
  * Chromium implementation of GeolocationPermissions -- forwards calls to the
  * chromium internal implementation.
  */
class GeolocationPermissionsAdapter
    : public Object
    //, public GeolocationPermissions
{
public:
    GeolocationPermissionsAdapter(
        /* [in] */ AwGeolocationPermissions* chromeGeolocationPermissions);

    // @Override
    CARAPI Allow(
        /* [in] */ const String& origin);

    // @Override
    CARAPI Clear(
        /* [in] */ const String& origin);

    // @Override
    CARAPI ClearAll();

    // @Override
    CARAPI GetAllowed(
        /* [in] */ const String& origin,
        /* [in] */ IInterface/*IValueCallback*/* callback);

    // @Override
    CARAPI GetOrigins(
        /* [in] */ IInterface/*IValueCallback*/* callback);

private:
    AutoPtr<AwGeolocationPermissions> mChromeGeolocationPermissions;
};

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_GEOLOCATIONPERMISSIONSADAPTER_H_

