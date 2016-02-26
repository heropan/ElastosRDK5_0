// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_COMPONENTS_NAVIGATIONPARAMS_H_
#define _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_COMPONENTS_NAVIGATIONPARAMS_H_

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

// package org.chromium.components.navigation_interception;
// import org.chromium.base.CalledByNative;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
namespace Components {

class NavigationParams
    : public Object
{
public:
    NavigationParams(
        /* [in] */ const String& url,
        /* [in] */ Boolean isPost,
        /* [in] */ Boolean hasUserGesture,
        /* [in] */ Int32 pageTransitionType,
        /* [in] */ Boolean isRedirect);

    // @CalledByNative return NavigationParams
    static CARAPI_(AutoPtr<IInterface>) Create(
        /* [in] */ const String& url,
        /* [in] */ Boolean isPost,
        /* [in] */ Boolean hasUserGesture,
        /* [in] */ Int32 pageTransitionType,
        /* [in] */ Boolean isRedirect);

    static CARAPI_(void*) ElaNavigationParamsCallback_Init();
public:
    // Target url of the navigation.
    const String url;
    // True if the the navigation method is "POST".
    const Boolean isPost;
    // True if the navigation was initiated by the user.
    const Boolean hasUserGesture;
    // Page transition type (e.g. link / typed).
    const Int32 pageTransitionType;
    // Is the navigation a redirect (in which case url is the "target" address).
    const Boolean isRedirect;
};

} // namespace Components
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_COMPONENTS_NAVIGATIONPARAMS_H_

