// wuweizuo automatic build .h file from .java file.
// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_ANDROIDCERTVERIFYRESULT_H_
#define _ELASTOS_DROID_WEBKIT_NET_ANDROIDCERTVERIFYRESULT_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "elastos/security/cert/X509Certificate.h"
#include "elastos/utility/CArrayList.h"
#include "elastos/utility/Collections.h"

// package org.chromium.net;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import java.security.cert.CertificateEncodingException;
// import java.security.cert.X509Certificate;
// import java.util.ArrayList;
// import java.util.Collections;
// import java.util.List;

using Elastos::Security::Cert::IX509Certificate;
using Elastos::Utility::IArrayList;
using Elastos::Utility::ICollections;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
  * The result of a certification verification.
  */
// @JNINamespace("net::android")
class AndroidCertVerifyResult : public Object
{
public:
    AndroidCertVerifyResult(
        /* [in] */ Int32 status,
        /* [in] */ Boolean isIssuedByKnownRoot,
        /* [in] */ IList<IX509Certificate*>* certificateChain);

    AndroidCertVerifyResult(
        /* [in] */ Int32 status);

    // @CalledByNative
    virtual CARAPI_(Int32) GetStatus();

    // @CalledByNative
    virtual CARAPI_(Boolean) IsIssuedByKnownRoot();

    // @CalledByNative
    virtual CARAPI_(AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > >) GetCertificateChainEncoded();

private:
    /**
      * The verification status. One of the values in CertVerifyStatusAndroid.
      */
    /*const*/ Int32 mStatus;
    /**
      * True if the root CA in the chain is in the system store.
      */
    /*const*/ Boolean mIsIssuedByKnownRoot;
    /**
      * The properly ordered certificate chain used for verification.
      */
    /*const*/ AutoPtr< IList<IX509Certificate> > mCertificateChain;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_ANDROIDCERTVERIFYRESULT_H_

