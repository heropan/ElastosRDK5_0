// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_COMPONENTS_WEBCONTENTSDELEGATEANDROID_H_
#define _ELASTOS_DROID_WEBKIT_COMPONENTS_WEBCONTENTSDELEGATEANDROID_H_

#include "elastos/droid/ext/frameworkext.h"

// package org.chromium.components.web_contents_delegate_android;
// import android.view.KeyEvent;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content.browser.ContentViewCore;

using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

class ContentViewCore;

/**
  * Java peer of the native class of the same name.
  */
// @JNINamespace("web_contents_delegate_android")
class WebContentsDelegateAndroid : public Object
{
public:
    virtual CARAPI_(Int32) GetMostRecentProgress();

    /**
      * @param disposition         The new tab disposition as per the constants in
      *                            org.chromium.ui.WindowOpenDisposition (See
      *                            window_open_disposition_list.h for the enumeration definitions).
      * @param isRendererInitiated Whether or not the renderer initiated this action.
      */
    // @CalledByNative
    virtual CARAPI OpenNewTab(
        /* [in] */ const String& url,
        /* [in] */ const String& extraHeaders,
        /* [in] */ ArrayOf<Byte>* postData,
        /* [in] */ Int32 disposition,
        /* [in] */ Boolean isRendererInitiated);

    // @CalledByNative
    virtual CARAPI ActivateContents();

    // @CalledByNative
    virtual CARAPI CloseContents();

    // @CalledByNative
    virtual CARAPI OnLoadStarted();

    // @CalledByNative
    virtual CARAPI OnLoadStopped();

    // @CalledByNative
    virtual CARAPI NavigationStateChanged(
        /* [in] */ Int32 flags);

    // @CalledByNative
    virtual CARAPI VisibleSSLStateChanged();

    /**
      * @param progress The load progress [0, 100] for the current web contents.
      */
    virtual CARAPI OnLoadProgressChanged(
        /* [in] */ Int32 progress);

    /**
      * Signaled when the renderer has been deemed to be unresponsive.
      */
    // @CalledByNative
    virtual CARAPI RendererUnresponsive();

    /**
      * Signaled when the render has been deemed to be responsive.
      */
    // @CalledByNative
    virtual CARAPI RendererResponsive();

    // @CalledByNative
    virtual CARAPI OnUpdateUrl(
        /* [in] */ const String& url);

    // @CalledByNative
    virtual CARAPI_(Boolean) TakeFocus(
        /* [in] */ Boolean reverse);

    // @CalledByNative
    virtual CARAPI HandleKeyboardEvent(
        /* [in] */ IKeyEvent* event);

    /**
      * Report a JavaScript console message.
      *
      * @param level message level. One of WebContentsDelegateAndroid.LOG_LEVEL*.
      * @param message the error message.
      * @param lineNumber the line number int the source file at which the error is reported.
      * @param sourceId the name of the source file that caused the error.
      * @return true if the client will handle logging the message.
      */
    // @CalledByNative
    virtual CARAPI_(Boolean) AddMessageToConsole(
        /* [in] */ Int32 level,
        /* [in] */ const String& message,
        /* [in] */ Int32 lineNumber,
        /* [in] */ const String& sourceId);

    /**
      * Report a form resubmission. The overwriter of this function should eventually call
      * either of ContentViewCore.ContinuePendingReload or ContentViewCore.CancelPendingReload.
      */
    // @CalledByNative
    virtual CARAPI ShowRepostFormWarningDialog(
        /* [in] */ ContentViewCore* contentViewCore);

    // @CalledByNative
    virtual CARAPI ToggleFullscreenModeForTab(
        /* [in] */ Boolean enterFullscreen);

    // @CalledByNative
    virtual CARAPI_(Boolean) IsFullscreenForTabOrPending();

private:
    // @SuppressWarnings("unused")
    // @CalledByNative
    const CARAPI NotifyLoadProgressChanged(
        /* [in] */ Double progress);

public:
    // Equivalent of WebCore::WebConsoleMessage::LevelTip.
    static const Int32 LOG_LEVEL_TIP = 0;
    // Equivalent of WebCore::WebConsoleMessage::LevelLog.
    static const Int32 LOG_LEVEL_LOG = 1;
    // Equivalent of WebCore::WebConsoleMessage::LevelWarning.
    static const Int32 LOG_LEVEL_WARNING = 2;
    // Equivalent of WebCore::WebConsoleMessage::LevelError.
    static const Int32 LOG_LEVEL_ERROR = 3;
    // Flags passed to the WebContentsDelegateAndroid.navigationStateChanged to tell it
    // what has changed. Should match the values in invalidate_type.h.
    // Equivalent of InvalidateTypes::INVALIDATE_TYPE_URL.
    static const Int32 INVALIDATE_TYPE_URL = 1 << 0;
    // Equivalent of InvalidateTypes::INVALIDATE_TYPE_TAB.
    static const Int32 INVALIDATE_TYPE_TAB = 1 << 1;
    // Equivalent of InvalidateTypes::INVALIDATE_TYPE_LOAD.
    static const Int32 INVALIDATE_TYPE_LOAD = 1 << 2;
    // Equivalent of InvalidateTypes::INVALIDATE_TYPE_PAGE_ACTIONS.
    static const Int32 INVALIDATE_TYPE_PAGE_ACTIONS = 1 << 3;
    // Equivalent of InvalidateTypes::INVALIDATE_TYPE_TITLE.
    static const Int32 INVALIDATE_TYPE_TITLE = 1 << 4;

private:
    // The most recent load progress callback received from WebContents, as a percentage.
    // Initialize to 100 to indicate that we're not in a loading state.
    Int32 mMostRecentProgress;
};

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_COMPONENTS_WEBCONTENTSDELEGATEANDROID_H_

