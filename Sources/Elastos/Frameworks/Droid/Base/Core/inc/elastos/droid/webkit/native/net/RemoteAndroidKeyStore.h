// wuweizuo automatic build .h file from .java file.
// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_REMOTEANDROIDKEYSTORE_H_
#define _ELASTOS_DROID_WEBKIT_NET_REMOTEANDROIDKEYSTORE_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"

// package org.chromium.net;
// import android.os.RemoteException;
// import android.util.Log;

using Elastos::Droid::Os::IRemoteException;
using Elastos::Droid::Util::ILog;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
  * Provides a remoted implementation of AndroidKeyStore where all calls are forwarded via
  * binder to an external process.
  */
class RemoteAndroidKeyStore
    : public Object
    , public AndroidKeyStore
{
private:
    class RemotePrivateKey
        : public Object
        , public AndroidPrivateKey
    {
    public:
        RemotePrivateKey(
            /* [in] */ Int32 handle,
            /* [in] */ RemoteAndroidKeyStore* store);

        virtual CARAPI_(Int32) GetHandle();

        // @Override
        CARAPI_(AutoPtr<AndroidKeyStore>) GetKeyStore();

    public:
        // Reference to the key on a remote store.
        /*const*/ Int32 mHandle;
        // Key store handling this key.
        /*const*/ AutoPtr<RemoteAndroidKeyStore> mStore;
    };

public:
    RemoteAndroidKeyStore(
        /* [in] */ IRemoteAndroidKeyStore* manager);

    // @Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetRSAKeyModulus(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetDSAKeyParamQ(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetECKeyOrder(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) RawSignDigestWithPrivateKey(
        /* [in] */ AndroidPrivateKey* key,
        /* [in] */ ArrayOf<Byte>* message);

    // @Override
    CARAPI_(Int32) GetPrivateKeyType(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetPrivateKeyEncodedBytes(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI_(Int64) GetOpenSSLHandleForPrivateKey(
        /* [in] */ AndroidPrivateKey* privateKey);

    virtual CARAPI_(AutoPtr<AndroidPrivateKey>) CreateKey(
        /* [in] */ const String& alias);

    // @Override
    CARAPI ReleaseKey(
        /* [in] */ AndroidPrivateKey* key);

private:
    static const String TAG;
    /*const*/ AutoPtr<IRemoteAndroidKeyStore> mRemoteManager;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_REMOTEANDROIDKEYSTORE_H_

