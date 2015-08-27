// wuweizuo automatic build .h file from .java file.
// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INTERSTITIALPAGEDELEGATEANDROID_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INTERSTITIALPAGEDELEGATEANDROID_H_

// package org.chromium.content.browser;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * Allows the specification and handling of Interstitial pages in java.
  */
// @JNINamespace("content")
class InterstitialPageDelegateAndroid
{
public:
    /**
      * Constructs an interstitial with the given HTML content.
      *
      * @param htmlContent The HTML content for the interstitial.
      */
    InterstitialPageDelegateAndroid(
        /* [in] */ String htmlContent);

    /**
      * @return The pointer to the underlying native counterpart.
      */
    virtual CARAPI_(Int64) GetNative();

protected:
    /**
      * Called when "proceed" is triggered on the interstitial.
      */
    // @CalledByNative
    virtual CARAPI OnProceed();

    /**
      * Called when "dont' proceed" is triggered on the interstitial.
      */
    // @CalledByNative
    virtual CARAPI OnDontProceed();

    /**
      * Called when a command has been received from the interstitial.
      *
      * @param command The command that was received.
      */
    // @CalledByNative
    virtual CARAPI CommandReceived(
        /* [in] */ String command);

    /**
      * Notifies the native interstitial to proceed.
      */
    virtual CARAPI Proceed();

    /**
      * Notifies the native interstitial to not proceed.
      */
    virtual CARAPI DontProceed();

private:
    // @CalledByNative
    CARAPI OnNativeDestroyed();

    CARAPI_(Int64) NativeInit(
        /* [in] */ String htmlContent);

    CARAPI NativeProceed(
        /* [in] */ Int64 nativeInterstitialPageDelegateAndroid);

    CARAPI NativeDontProceed(
        /* [in] */ Int64 nativeInterstitialPageDelegateAndroid);

private:
    Int64 mNativePtr;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INTERSTITIALPAGEDELEGATEANDROID_H_

