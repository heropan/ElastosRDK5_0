// wuweizuo automatic build .h file from .java file.
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_COMPONENTS_INTERCEPTNAVIGATIONDELEGATE_H_
#define _ELASTOS_DROID_WEBKIT_COMPONENTS_INTERCEPTNAVIGATIONDELEGATE_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"

// package org.chromium.components.navigation_interception;
// import org.chromium.base.CalledByNative;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

class InterceptNavigationDelegate
{
public:
    /**
      * This method is called for every top-level navigation within the associated WebContents.
      * The method allows the embedder to ignore navigations. This is used on Android to 'convert'
      * certain navigations to Intents to 3rd party applications.
      *
      * @param navigationParams parameters describing the navigation.
      * @return true if the navigation should be ignored.
      */
    // @CalledByNative
    virtual CARAPI_(Boolean) ShouldIgnoreNavigation(
        /* [in] */ NavigationParams* navigationParams) = 0;
};

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_COMPONENTS_INTERCEPTNAVIGATIONDELEGATE_H_

