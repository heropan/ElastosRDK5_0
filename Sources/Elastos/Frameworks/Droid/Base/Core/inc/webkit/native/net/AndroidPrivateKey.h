// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_ANDROIDPRIVATEKEY_H_
#define _ELASTOS_DROID_WEBKIT_NET_ANDROIDPRIVATEKEY_H_

//package org.chromium.net;

//import org.chromium.base.CalledByNative;
//import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
 * Abstract private key that bundles the PrivateKey and AndroidKeyStore that it belongs to.
 */
//@JNINamespace("net::android")
class AndroidPrivateKey
{
public:
    /** @return AndroidKeyStore that handles this key. */
    //@CalledByNative
    virtual CARAPI_(AutoPtr<AndroidKeyStore>) GetKeyStore() = 0;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_ANDROIDPRIVATEKEY_H_

