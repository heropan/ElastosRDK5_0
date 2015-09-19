
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_SSLUTIL_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_SSLUTIL_H__

// import android.net.http.SslCertificate;
// import android.net.http.SslError;
// import android.util.Log;

// import org.chromium.net.NetError;
// import org.chromium.net.X509Util;

// import java.security.KeyStoreException;
// import java.security.NoSuchAlgorithmException;
// import java.security.cert.CertificateException;
// import java.security.cert.X509Certificate;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

class SslUtil
{
public:
    /**
     * Creates an SslError object from a chromium net error code.
     */
    static CARAPI_(AutoPtr<ISslError>) SslErrorFromNetErrorCode(
        /* [in] */ Int32 error,
        /* [in] */ ISslCertificate* cert,
        /* [in] */ String url);

    static CARAPI_(AutoPtr<ISslCertificate>) GetCertificateFromDerBytes(
        /* [in] */ ArrayOf<Byte>* derBytes);

private:
    static const String TAG;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_SSLUTIL_H__
