// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_MEDIADRMCREDENTIALMANAGER_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_MEDIADRMCREDENTIALMANAGER_H_

// package org.chromium.content.browser;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * A wrapper of the android MediaDrmCredentialManager
  */
// @JNINamespace("content")
class MediaDrmCredentialManager
{
public:
    /**
      * Callback interface for getting notified from credential reset.
      */
    class MediaDrmCredentialManagerCallback
    {
    public:
        /**
          * This method will be called when credential reset attempt is done.
          * @param succeeded Whether or not it succeeded.
          */
        // @CalledByNative("MediaDrmCredentialManagerCallback")
        virtual CARAPI OnCredentialResetFinished(
            /* [in] */ Boolean succeeded) = 0;
    };

public:
    /**
      * Attempts to reset the DRM credentials.
      * @param callback It notifies whether or not it succeeded.
      */
    static CARAPI ResetCredentials(
        /* [in] */ MediaDrmCredentialManagerCallback* callback);

private:
    static CARAPI NativeResetCredentials(
        /* [in] */ MediaDrmCredentialManagerCallback* callback);
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_MEDIADRMCREDENTIALMANAGER_H_

