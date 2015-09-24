// wuweizuo automatic build .cpp file from .java file.

#include "AndroidNetworkLibrary.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//                        AndroidNetworkLibrary
//=====================================================================
const String AndroidNetworkLibrary::TAG("AndroidNetworkLibrary");

Boolean AndroidNetworkLibrary::StoreKeyPair(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<Byte>* publicKey,
    /* [in] */ ArrayOf<Byte>* privateKey)
{
    // ==================before translated======================
    // // TODO(digit): Use KeyChain official extra values to pass the public and private
    // // keys when they're available. The "KEY" and "PKEY" hard-coded constants were taken
    // // from the platform sources, since there are no official KeyChain.EXTRA_XXX definitions
    // // for them. b/5859651
    // try {
    //     Intent intent = KeyChain.createInstallIntent();
    //     intent.putExtra("PKEY", privateKey);
    //     intent.putExtra("KEY", publicKey);
    //     intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    //     context.startActivity(intent);
    //     return true;
    // } catch (ActivityNotFoundException e) {
    //     Log.w(TAG, "could not store key pair: " + e);
    // }
    // return false;
    assert(0);
    return FALSE;
}

Boolean AndroidNetworkLibrary::StoreCertificate(
    /* [in] */ IContext* context,
    /* [in] */ Int32 certType,
    /* [in] */ ArrayOf<Byte>* data)
{
    // ==================before translated======================
    // try {
    //     Intent intent = KeyChain.createInstallIntent();
    //     intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    //
    //     switch (certType) {
    //         case CertificateMimeType.X509_USER_CERT:
    //         case CertificateMimeType.X509_CA_CERT:
    //             intent.putExtra(KeyChain.EXTRA_CERTIFICATE, data);
    //             break;
    //
    //         case CertificateMimeType.PKCS12_ARCHIVE:
    //             intent.putExtra(KeyChain.EXTRA_PKCS12, data);
    //             break;
    //
    //         default:
    //             Log.w(TAG, "invalid certificate type: " + certType);
    //             return false;
    //     }
    //     context.startActivity(intent);
    //     return true;
    // } catch (ActivityNotFoundException e) {
    //     Log.w(TAG, "could not store crypto file: " + e);
    // }
    // return false;
    assert(0);
    return FALSE;
}

String AndroidNetworkLibrary::GetMimeTypeFromExtension(
    /* [in] */ const String& extension)
{
    // ==================before translated======================
    // return URLConnection.guessContentTypeFromName("foo." + extension);
    assert(0);
    return String("");
}

Boolean AndroidNetworkLibrary::HaveOnlyLoopbackAddresses()
{
    // ==================before translated======================
    // Enumeration<NetworkInterface> list = null;
    // try {
    //     list = NetworkInterface.getNetworkInterfaces();
    //     if (list == null) return false;
    // } catch (Exception e) {
    //     Log.w(TAG, "could not get network interfaces: " + e);
    //     return false;
    // }
    //
    // while (list.hasMoreElements()) {
    //     NetworkInterface netIf = list.nextElement();
    //     try {
    //         if (netIf.isUp() && !netIf.isLoopback()) return false;
    //     } catch (SocketException e) {
    //         continue;
    //     }
    // }
    // return true;
    assert(0);
    return FALSE;
}

String AndroidNetworkLibrary::GetNetworkList()
{
    // ==================before translated======================
    // Enumeration<NetworkInterface> list = null;
    // try {
    //     list = NetworkInterface.getNetworkInterfaces();
    //     if (list == null) return "";
    // } catch (SocketException e) {
    //     Log.w(TAG, "Unable to get network interfaces: " + e);
    //     return "";
    // }
    //
    // StringBuilder result = new StringBuilder();
    // while (list.hasMoreElements()) {
    //     NetworkInterface netIf = list.nextElement();
    //     try {
    //         // Skip loopback interfaces, and ones which are down.
    //         if (!netIf.isUp() || netIf.isLoopback())
    //             continue;
    //         for (InterfaceAddress interfaceAddress : netIf.getInterfaceAddresses()) {
    //             InetAddress address = interfaceAddress.getAddress();
    //             // Skip loopback addresses configured on non-loopback interfaces.
    //             if (address.isLoopbackAddress())
    //                 continue;
    //             StringBuilder addressString = new StringBuilder();
    //             addressString.append(netIf.getName());
    //             addressString.append("\t");
    //
    //             String ipAddress = address.getHostAddress();
    //             if (address instanceof Inet6Address && ipAddress.contains("%")) {
    //                 ipAddress = ipAddress.substring(0, ipAddress.lastIndexOf("%"));
    //             }
    //             addressString.append(ipAddress);
    //             addressString.append("/");
    //             addressString.append(interfaceAddress.getNetworkPrefixLength());
    //             addressString.append("\t");
    //
    //             // TODO(vitalybuka): use netIf.getIndex() when API level 19 is availible.
    //             addressString.append("0");
    //
    //             if (result.length() != 0)
    //                 result.append("\n");
    //             result.append(addressString.toString());
    //         }
    //     } catch (SocketException e) {
    //         continue;
    //     }
    // }
    // return result.toString();
    assert(0);
    return String("");
}

AutoPtr<AndroidCertVerifyResult> AndroidNetworkLibrary::VerifyServerCertificates(
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* certChain,
    /* [in] */ const String& authType,
    /* [in] */ const String& host)
{
    // ==================before translated======================
    // try {
    //     return X509Util.verifyServerCertificates(certChain, authType, host);
    // } catch (KeyStoreException e) {
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    // } catch (NoSuchAlgorithmException e) {
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    // }
    assert(0);
    AutoPtr<AndroidCertVerifyResult> empty;
    return empty;
}

ECode AndroidNetworkLibrary::AddTestRootCertificate(
    /* [in] */ ArrayOf<Byte>* rootCert)
{
    VALIDATE_NOT_NULL(rootCert);
    // ==================before translated======================
    // X509Util.addTestRootCertificate(rootCert);
    assert(0);
    return NOERROR;
}

ECode AndroidNetworkLibrary::ClearTestRootCertificates()
{
    // ==================before translated======================
    // X509Util.clearTestRootCertificates();
    assert(0);
    return NOERROR;
}

String AndroidNetworkLibrary::GetNetworkCountryIso(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // TelephonyManager telephonyManager =
    //     (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
    // if (telephonyManager != null) {
    //   return telephonyManager.getNetworkCountryIso();
    // }
    // return "";
    assert(0);
    return String("");
}

String AndroidNetworkLibrary::GetNetworkOperator(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // TelephonyManager telephonyManager =
    //     (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
    // if (telephonyManager != null) {
    //   return telephonyManager.getNetworkOperator();
    // }
    // return "";
    assert(0);
    return String("");
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


