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

#ifndef _ELASTOS_DROID_WEBKIT_CHROMIUM_COOKIEMANAGERADAPTER_H_
#define _ELASTOS_DROID_WEBKIT_CHROMIUM_COOKIEMANAGERADAPTER_H_

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/WebAddress.h"
//#include "elastos/droid/webkit/CookieManager.h"
#include "elastos/droid/webkit/native/android_webview/AwCookieManager.h"

// package com.android.webview.chromium;
// import android.net.ParseException;
// import android.net.WebAddress;
// import android.util.Log;
// import android.webkit.CookieManager;
// import android.webkit.ValueCallback;
// import android.webkit.WebView;
// import org.chromium.android_webview.AwCookieManager;

using Elastos::Droid::Net::WebAddress;
//using Elastos::Droid::Webkit::CookieManager;
using Elastos::Droid::Webkit::AndroidWebview::AwCookieManager;

class WebView;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Chromium {

class CookieManagerAdapter //: public CookieManager
{
public:
    CookieManagerAdapter(
        /* [in] */ AwCookieManager* chromeCookieManager);

    // @Override
    // synchronized
    CARAPI SetAcceptCookie(
        /* [in] */ Boolean accept);

    // @Override
    // synchronized
    CARAPI_(Boolean) AcceptCookie();

    // @Override
    // synchronized
    CARAPI SetAcceptThirdPartyCookies(
        /* [in] */ WebView* webView,
        /* [in] */ Boolean accept);

    // @Override
    // synchronized
    CARAPI_(Boolean) AcceptThirdPartyCookies(
        /* [in] */ WebView* webView);

    // @Override
    CARAPI SetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value);

    // @Override
    CARAPI SetCookie(
        /* [in] */ const String& url,
        /* [in] */ const String& value,
        /* [in] */ IInterface/*IValueCallback*/* callback);

    // @Override
    CARAPI_(String) GetCookie(
        /* [in] */ const String& url);

    // @Override
    CARAPI_(String) GetCookie(
        /* [in] */ const String& url,
        /* [in] */ Boolean privateBrowsing);

    // @Override
    // synchronized
    CARAPI_(String) GetCookie(
        /* [in] */ WebAddress* uri);

    // @Override
    CARAPI RemoveSessionCookie();

    // @Override
    CARAPI RemoveSessionCookies(
        /* [in] */ IInterface/*IValueCallback*/* callback);

    // @Override
    CARAPI RemoveAllCookie();

    // @Override
    CARAPI RemoveAllCookies(
        /* [in] */ IInterface/*IValueCallback*/* callback);

    // @Override
    // synchronized
    CARAPI_(Boolean) HasCookies();

    // @Override
    // synchronized
    CARAPI_(Boolean) HasCookies(
        /* [in] */ Boolean privateBrowsing);

    // @Override
    CARAPI RemoveExpiredCookie();

protected:
    // @Override
    CARAPI FlushCookieStore();

    // @Override
    CARAPI_(Boolean) AllowFileSchemeCookiesImpl();

    // @Override
    CARAPI SetAcceptFileSchemeCookiesImpl(
        /* [in] */ Boolean accept);

private:
    static CARAPI_(String) FixupUrl(
        /* [in] */ const String& url);

public:
    AutoPtr<AwCookieManager> mChromeCookieManager;

private:
    static const String LOGTAG;
};

} // namespace Chromium
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CHROMIUM_COOKIEMANAGERADAPTER_H_

