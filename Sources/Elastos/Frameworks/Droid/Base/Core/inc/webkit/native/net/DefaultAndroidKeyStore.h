// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_DEFAULTANDROIDKEYSTORE_H_
#define _ELASTOS_DROID_WEBKIT_NET_DEFAULTANDROIDKEYSTORE_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "elastos/security/Signature.h"
#include "elastos/security/spec/CECParameterSpec.h"

// package org.chromium.net;
// import android.util.Log;
// import java.lang.reflect.Method;
// import java.security.NoSuchAlgorithmException;
// import java.security.PrivateKey;
// import java.security.Signature;
// import java.security.interfaces.DSAKey;
// import java.security.interfaces.DSAParams;
// import java.security.interfaces.DSAPrivateKey;
// import java.security.interfaces.ECKey;
// import java.security.interfaces.ECPrivateKey;
// import java.security.interfaces.RSAKey;
// import java.security.interfaces.RSAPrivateKey;
// import java.security.spec.ECParameterSpec;

using Elastos::Droid::Util::ILog;
using Elastos::Security::ISignature;
using Elastos::Security::Spec::IECParameterSpec;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
  * Simple implementation of the AndroidKeyStore for use with an in-process Java KeyStore.
  */
class DefaultAndroidKeyStore
    : public Object
    , public AndroidKeyStore
{
private:
    class DefaultAndroidPrivateKey
        : public Object
        , public AndroidPrivateKey
    {
    public:
        DefaultAndroidPrivateKey(
            /* [in] */ IPrivateKey* key,
            /* [in] */ DefaultAndroidKeyStore* store);

        virtual CARAPI_(AutoPtr<IPrivateKey>) GetJavaKey();

        // @Override
        CARAPI_(AutoPtr<AndroidKeyStore>) GetKeyStore();

    public:
        // The actual Java key being wrapped.
        /*const*/ AutoPtr<IPrivateKey> mKey;
        // Key store handling this key.
        /*const*/ AutoPtr<DefaultAndroidKeyStore> mStore;
    };

public:
    virtual CARAPI_(AutoPtr<AndroidPrivateKey>) CreateKey(
        /* [in] */ IPrivateKey* javaKey);

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
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetPrivateKeyEncodedBytes(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) RawSignDigestWithPrivateKey(
        /* [in] */ AndroidPrivateKey* key,
        /* [in] */ ArrayOf<Byte>* message);

    // @Override
    CARAPI_(Int32) GetPrivateKeyType(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI_(Int64) GetOpenSSLHandleForPrivateKey(
        /* [in] */ AndroidPrivateKey* key);

    // @Override
    CARAPI ReleaseKey(
        /* [in] */ AndroidPrivateKey* key);

private:
    static const String TAG;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_DEFAULTANDROIDKEYSTORE_H_

