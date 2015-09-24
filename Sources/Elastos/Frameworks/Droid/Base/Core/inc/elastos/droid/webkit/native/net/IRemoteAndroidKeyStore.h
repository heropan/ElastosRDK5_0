// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_IREMOTEANDROIDKEYSTORE_H_
#define _ELASTOS_DROID_WEBKIT_NET_IREMOTEANDROIDKEYSTORE_H_

//package org.chromium.net;

//import org.chromium.net.IRemoteAndroidKeyStoreCallbacks;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
 * Interface for communication with an Android KeyStore in another process.
 */
class RemoteAndroidKeyStore
{
public:
    // Remote calls for SSlClientCertificateRequest - these allow retrieving
    // the alias of the certificate to be used, its encoded chain and a handle
    // for identifying a private key in the remote process.
    virtual CARAPI_(String) GetClientCertificateAlias() = 0;

    virtual CARAPI_(AutoPtr< ArrayOf<Byte> >) GetEncodedCertificateChain(
        /* [in] */ const String& alias) = 0;

    virtual CARAPI_(Int32) GetPrivateKeyHandle(
        /* [in] */ const String& alias) = 0;

    // Registers callbacks for service->client communication.
    virtual CARAPI SetClientCallbacks(
        /* [in] */ IRemoteAndroidKeyStoreCallbacks* callbacks) = 0;

    // Remote calls for AndroidKeyStore - these functions are performing operations
    // with a PrivateKey in the remote process using the handle provided by
    // |getPrivateKeyHandle|.
    virtual CARAPI_(AutoPtr< ArrayOf<Byte> >) GetRSAKeyModulus(
        /* [in] */ Int32 handle) = 0;

    virtual CARAPI_(AutoPtr< ArrayOf<Byte> >) GetPrivateKeyEncodedBytes(
        /* [in] */ Int32 handle) = 0;

    virtual CARAPI_(AutoPtr< ArrayOf<Byte> >) GetDSAKeyParamQ(
        /* [in] */ Int32 handle) = 0;

    virtual CARAPI_(AutoPtr< ArrayOf<Byte> >) GetECKeyOrder(
        /* [in] */ Int32 handle) = 0;

    virtual CARAPI_(AutoPtr< ArrayOf<Byte> >) RawSignDigestWithPrivateKey(
        /* [in] */ Int32 handle,
        /* [in] */ ArrayOf<Byte>* message) = 0;

    virtual CARAPI_(Int32) GetPrivateKeyType(
        /* [in] */ Int32 handle) = 0;

    virtual CARAPI ReleaseKey(
        /* [in] */ Int32 handle) = 0;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_IREMOTEANDROIDKEYSTORE_H_

