// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_IREMOTEANDROIDKEYSTORECALLBACKS_H_
#define _ELASTOS_DROID_WEBKIT_NET_IREMOTEANDROIDKEYSTORECALLBACKS_H_

//package org.chromium.net;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

class RemoteAndroidKeyStoreCallbacks
{
public:
    /**
      * A critical failure has occurred and the service won't be able to recover.
      * The client should unbind and optionally rebind at a later time.
      */
    virtual CARAPI OnDisabled() = 0;

    /**
     * The service has started up and is fully initialized. This allows for the
     * service to take some time to initialize. Remote calls shouldn't be invoked
     * until this call has fired.
     */
    virtual CARAPI OnInitComplete() = 0;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_IREMOTEANDROIDKEYSTORECALLBACKS_H_

