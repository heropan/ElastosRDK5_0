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

#ifndef _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_CONTENTSETTINGSADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_CONTENTSETTINGSADAPTER_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/native/android_webview/AwSettings.h"
//#include "elastos/droid/webkit/WebSettings.h"
//#include "elastos/droid/webkit/native/android_webview/AwSettings.h"

// package com.android.webview.chromium;
// import android.util.Log;
// import android.webkit.DebugFlags;
// import android.webkit.WebSettings.LayoutAlgorithm;
// import android.webkit.WebSettings.PluginState;
// import android.webkit.WebSettings.RenderPriority;
// import android.webkit.WebSettings.TextSize;
// import android.webkit.WebSettings.ZoomDensity;
// import org.chromium.android_webview.AwSettings;

//using Elastos::Droid::Webkit::WebSettings;
//using Elastos::Droid::Webkit::RenderPriority;
//using Elastos::Droid::Webkit::AndroidWebview::AwSettings;

using Elastos::Droid::Webkit::AndroidWebview::AwSettings;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {

class ContentSettingsAdapter
    : public Object
    //, public WebSettings
{
public:
    ContentSettingsAdapter(
        /* [in] */ AwSettings* awSettings);

    // virtual CARAPI_(AutoPtr<AwSettings>) GetAwSettings();

    // @Override
    // @Deprecated
    CARAPI SetNavDump(
        /* [in] */ Boolean enabled);

    // @Override
    // @Deprecated
    CARAPI_(Boolean) GetNavDump();

    // @Override
    CARAPI SetSupportZoom(
        /* [in] */ Boolean support);

    // @Override
    CARAPI_(Boolean) SupportZoom();

    // @Override
    CARAPI SetBuiltInZoomControls(
        /* [in] */ Boolean enabled);

    // @Override
    CARAPI_(Boolean) GetBuiltInZoomControls();

    // @Override
    CARAPI SetDisplayZoomControls(
        /* [in] */ Boolean enabled);

    // @Override
    CARAPI_(Boolean) GetDisplayZoomControls();

    // @Override
    CARAPI SetAllowFileAccess(
        /* [in] */ Boolean allow);

    // @Override
    CARAPI_(Boolean) GetAllowFileAccess();

    // @Override
    CARAPI SetAllowContentAccess(
        /* [in] */ Boolean allow);

    // @Override
    CARAPI_(Boolean) GetAllowContentAccess();

    // @Override
    CARAPI SetLoadWithOverviewMode(
        /* [in] */ Boolean overview);

    // @Override
    CARAPI_(Boolean) GetLoadWithOverviewMode();

    // @Override
    CARAPI SetAcceptThirdPartyCookies(
        /* [in] */ Boolean accept);

    // @Override
    CARAPI_(Boolean) GetAcceptThirdPartyCookies();

    // @Override
    CARAPI SetEnableSmoothTransition(
        /* [in] */ Boolean enable);

    // @Override
    CARAPI_(Boolean) EnableSmoothTransition();

    // @Override
    CARAPI SetUseWebViewBackgroundForOverscrollBackground(
        /* [in] */ Boolean view);

    // @Override
    CARAPI_(Boolean) GetUseWebViewBackgroundForOverscrollBackground();

    // @Override
    CARAPI SetSaveFormData(
        /* [in] */ Boolean save);

    // @Override
    CARAPI_(Boolean) GetSaveFormData();

    // @Override
    CARAPI SetSavePassword(
        /* [in] */ Boolean save);

    // @Override
    CARAPI_(Boolean) GetSavePassword();

    // @Override
    // synchronized
    CARAPI SetTextZoom(
        /* [in] */ Int32 textZoom);

    // @Override
    // synchronized
    CARAPI_(Int32) GetTextZoom();

    // @Override
    CARAPI SetDefaultZoom(
         /* [in] */ Int32/*WebSettings::ZoomDensity*/ zoom);

    // @Override
    CARAPI_(Int32/*WebSettings::ZoomDensity*/) GetDefaultZoom();

    // @Override
    CARAPI SetLightTouchEnabled(
        /* [in] */ Boolean enabled);

    // @Override
    CARAPI_(Boolean) GetLightTouchEnabled();

    // @Override
    // synchronized
    CARAPI SetUserAgent(
        /* [in] */ Int32 ua);

    // @Override
    // synchronized
    CARAPI_(Int32) GetUserAgent();

    // @Override
    // synchronized
    CARAPI SetUseWideViewPort(
        /* [in] */ Boolean use);

    // @Override
    // synchronized
    CARAPI_(Boolean) GetUseWideViewPort();

    // @Override
    // synchronized
    CARAPI SetSupportMultipleWindows(
        /* [in] */ Boolean support);

    // @Override
    // synchronized
    CARAPI_(Boolean) SupportMultipleWindows();

    // @Override
    // synchronized
    CARAPI SetLayoutAlgorithm(
        /* [in] */ Int32/*AwSettings::LayoutAlgorithm*/ la);

    // @Override
    CARAPI_(Int32/*AwSettings::LayoutAlgorithm*/) GetLayoutAlgorithm();

    // @Override
    // synchronized
    CARAPI SetStandardFontFamily(
        /* [in] */ const String& font);

    // @Override
    // synchronized
    CARAPI_(String) GetStandardFontFamily();

    // @Override
    // synchronized
    CARAPI SetFixedFontFamily(
        /* [in] */ const String& font);

    // @Override
    // synchronized
    CARAPI_(String) GetFixedFontFamily();

    // @Override
    // synchronized
    CARAPI SetSansSerifFontFamily(
        /* [in] */ const String& font);

    // @Override
    // synchronized
    CARAPI_(String) GetSansSerifFontFamily();

    // @Override
    // synchronized
    CARAPI SetSerifFontFamily(
        /* [in] */ const String& font);

    // @Override
    // synchronized
    CARAPI_(String) GetSerifFontFamily();

    // @Override
    // synchronized
    CARAPI SetCursiveFontFamily(
        /* [in] */ const String& font);

    // @Override
    // synchronized
    CARAPI_(String) GetCursiveFontFamily();

    // @Override
    // synchronized
    CARAPI SetFantasyFontFamily(
        /* [in] */ const String& font);

    // @Override
    // synchronized
    CARAPI_(String) GetFantasyFontFamily();

    // @Override
    // synchronized
    CARAPI SetMinimumFontSize(
        /* [in] */ Int32 size);

    // @Override
    // synchronized
    CARAPI_(Int32) GetMinimumFontSize();

    // @Override
    // synchronized
    CARAPI SetMinimumLogicalFontSize(
        /* [in] */ Int32 size);

    // @Override
    // synchronized
    CARAPI_(Int32) GetMinimumLogicalFontSize();

    // @Override
    // synchronized
    CARAPI SetDefaultFontSize(
        /* [in] */ Int32 size);

    // @Override
    // synchronized
    CARAPI_(Int32) GetDefaultFontSize();

    // @Override
    // synchronized
    CARAPI SetDefaultFixedFontSize(
        /* [in] */ Int32 size);

    // @Override
    // synchronized
    CARAPI_(Int32) GetDefaultFixedFontSize();

    // @Override
    // synchronized
    CARAPI SetLoadsImagesAutomatically(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI_(Boolean) GetLoadsImagesAutomatically();

    // @Override
    // synchronized
    CARAPI SetBlockNetworkImage(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI_(Boolean) GetBlockNetworkImage();

    // @Override
    // synchronized
    CARAPI SetBlockNetworkLoads(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI_(Boolean) GetBlockNetworkLoads();

    // @Override
    // synchronized
    CARAPI SetJavaScriptEnabled(
        /* [in] */ Boolean flag);

    // @Override
    CARAPI SetAllowUniversalAccessFromFileURLs(
        /* [in] */ Boolean flag);

    // @Override
    CARAPI SetAllowFileAccessFromFileURLs(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI SetPluginsEnabled(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI SetPluginState(
        /* [in] */ Int32/*WebSettings::PluginState*/ state);

    // @Override
    // synchronized
    CARAPI SetDatabasePath(
        /* [in] */ const String& databasePath);

    // @Override
    // synchronized
    CARAPI SetGeolocationDatabasePath(
        /* [in] */ const String& databasePath);

    // @Override
    // synchronized
    CARAPI SetAppCacheEnabled(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI SetAppCachePath(
        /* [in] */ const String& appCachePath);

    // @Override
    // synchronized
    CARAPI SetAppCacheMaxSize(
        /* [in] */ Int64 appCacheMaxSize);

    // @Override
    // synchronized
    CARAPI SetDatabaseEnabled(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI SetDomStorageEnabled(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI_(Boolean) GetDomStorageEnabled();

    // @Override
    // synchronized
    CARAPI_(String) GetDatabasePath();

    // @Override
    // synchronized
    CARAPI_(Boolean) GetDatabaseEnabled();

    // @Override
    // synchronized
    CARAPI SetGeolocationEnabled(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI_(Boolean) GetJavaScriptEnabled();

    // @Override
    CARAPI_(Boolean) GetAllowUniversalAccessFromFileURLs();

    // @Override
    CARAPI_(Boolean) GetAllowFileAccessFromFileURLs();

    // @Override
    // synchronized
    CARAPI_(Boolean) GetPluginsEnabled();

    // @Override
    // synchronized
    CARAPI_(Int32/*WebSettings::PluginState*/) GetPluginState();

    // @Override
    // synchronized
    CARAPI SetJavaScriptCanOpenWindowsAutomatically(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI_(Boolean) GetJavaScriptCanOpenWindowsAutomatically();

    // @Override
    // synchronized
    CARAPI SetDefaultTextEncodingName(
        /* [in] */ const String& encoding);

    // @Override
    // synchronized
    CARAPI_(String) GetDefaultTextEncodingName();

    // @Override
    // synchronized
    CARAPI SetUserAgentString(
        /* [in] */ const String& ua);

    // @Override
    // synchronized
    CARAPI_(String) GetUserAgentString();

    // @Override
    CARAPI SetNeedInitialFocus(
        /* [in] */ Boolean flag);

    // @Override
    // synchronized
    CARAPI SetRenderPriority(
        /* [in] */ Int32/*RenderPriority*/ priority);

    // @Override
    CARAPI SetCacheMode(
        /* [in] */ Int32 mode);

    // @Override
    CARAPI_(Int32) GetCacheMode();

    // @Override
    CARAPI SetMediaPlaybackRequiresUserGesture(
        /* [in] */ Boolean require);

    // @Override
    CARAPI_(Boolean) GetMediaPlaybackRequiresUserGesture();

    //    @Override
    virtual CARAPI SetMixedContentMode(
        /* [in] */ Int32 mode);

    //    @Override
    virtual CARAPI_(Int32) GetMixedContentMode();

    //    @Override
    virtual CARAPI SetVideoOverlayForEmbeddedEncryptedVideoEnabled(
        /* [in] */ Boolean flag);

    //    @Override
    virtual CARAPI_(Boolean) GetVideoOverlayForEmbeddedEncryptedVideoEnabled();

private:
    static const String LOGTAG;
    //AutoPtr<AwSettings> mAwSettings;
};

} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_CONTENTSETTINGSADAPTER_H_

