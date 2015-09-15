// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_PEPPERPLUGINMANAGER_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_PEPPERPLUGINMANAGER_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/Context.h"
#include "content/Intent.h"
#include "content/pm/CApplicationInfo.h"
#include "content/pm/CPackageInfo.h"
#include "content/pm/PackageManager.h"
#include "content/pm/CResolveInfo.h"
#include "content/pm/CServiceInfo.h"
#include "os/CBundle.h"

// package org.chromium.content.browser;
// import android.content.Context;
// import android.content.Intent;
// import android.content.pm.ApplicationInfo;
// import android.content.pm.PackageInfo;
// import android.content.pm.PackageManager;
// import android.content.pm.PackageManager.NameNotFoundException;
// import android.content.pm.ResolveInfo;
// import android.content.pm.ServiceInfo;
// import android.os.Bundle;
// import android.util.Log;
// import java.util.List;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Util::ILog;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * {@link PepperPluginManager} collects meta data about plugins from preloaded android apps
  * that reply to PEPPERPLUGIN intent query.
  */
class PepperPluginManager
{
public:
    /**
      * Collects information about installed plugins and returns a plugin description
      * string, which will be appended to for command line to load plugins.
      *
      * @param context Android context
      * @return        Description string for plugins
      */
    static CARAPI_(String) GetPlugins(
        /* [in] */ const IContext* context);

private:
    static CARAPI_(String) GetPluginDescription(
        /* [in] */ IBundle* metaData);

public:
    /**
      * Service Action: A plugin wishes to be loaded in the ContentView must
      * provide {@link android.content.IntentFilter IntentFilter} that accepts
      * this action in its AndroidManifest.xml.
      */
    static const String PEPPER_PLUGIN_ACTION;
    static const String PEPPER_PLUGIN_ROOT;

private:
    static const String LOGTAG;
    // A plugin will specify the following fields in its AndroidManifest.xml.
    static const String FILENAME;
    static const String MIMETYPE;
    static const String NAME;
    static const String DESCRIPTION;
    static const String VERSION;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_PEPPERPLUGINMANAGER_H_

