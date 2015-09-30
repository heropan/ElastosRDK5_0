
#include "elastos/droid/webkit/native/net/X509Util.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//              X509Util::TrustStorageListener
//=====================================================================
ECode X509Util::TrustStorageListener::OnReceive(
	/* [in] */ IContext* context,
	/* [in] */ IIntent* intent)
{
	// ==================before translated======================
	// if (intent.getAction().equals(KeyChain.ACTION_STORAGE_CHANGED)) {
	//     try {
	//         reloadDefaultTrustManager();
	//     }
	//     catch (CertificateException e) {
	//         Log.e(TAG, "Unable to reload the default TrustManager", e);
	//     }
	//     catch (KeyStoreException e) {
	//         Log.e(TAG, "Unable to reload the default TrustManager", e);
	//     }
	//     catch (NoSuchAlgorithmException e) {
	//         Log.e(TAG, "Unable to reload the default TrustManager", e);
	//     }
	// }
	assert(0);
    return NOERROR;
}

//=====================================================================
//              X509Util::X509TrustManagerIceCreamSandwich
//=====================================================================
X509Util::X509TrustManagerIceCreamSandwich::X509TrustManagerIceCreamSandwich(
    /* [in] */ IX509TrustManager* trustManager)
{
    // ==================before translated======================
    // mTrustManager = trustManager;
}

AutoPtr<IList> X509Util::X509TrustManagerIceCreamSandwich::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate>* chain,
    /* [in] */ const String& authType,
    /* [in] */ const String& host)
{
    // ==================before translated======================
    // mTrustManager.checkServerTrusted(chain, authType);
    // return Collections.<X509Certificate>emptyList();
    assert(0);
    AutoPtr<IList> empty;
    return empty;
}

//=====================================================================
//                 X509Util::X509TrustManagerJellyBean
//=====================================================================
X509Util::X509TrustManagerJellyBean::X509TrustManagerJellyBean(
    /* [in] */ IX509TrustManager* trustManager)
{
    // ==================before translated======================
    // mTrustManagerExtensions = new X509TrustManagerExtensions(trustManager);
}

AutoPtr<IList> X509Util::X509TrustManagerJellyBean::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate>* chain,
    /* [in] */ const String& authType,
    /* [in] */ const String& host)
{
    // ==================before translated======================
    // return mTrustManagerExtensions.checkServerTrusted(chain, authType, host);
    assert(0);
    AutoPtr<IList> empty;
    return empty;
}

//=====================================================================
//                               X509Util
//=====================================================================
const String X509Util::TAG("X509Util");
AutoPtr<ICertificateFactory> X509Util::sCertificateFactory;
const String X509Util::OID_TLS_SERVER_AUTH("1.3.6.1.5.5.7.3.1");
const String X509Util::OID_ANY_EKU("2.5.29.37.0");
const String X509Util::OID_SERVER_GATED_NETSCAPE("2.16.840.1.113730.4.1");
const String X509Util::OID_SERVER_GATED_MICROSOFT("1.3.6.1.4.1.311.10.3.3");
AutoPtr<X509Util::X509TrustManagerImplementation> X509Util::sDefaultTrustManager;
AutoPtr<X509Util::TrustStorageListener> X509Util::sTrustStorageListener;
AutoPtr<X509Util::X509TrustManagerImplementation> X509Util::sTestTrustManager;
AutoPtr<IKeyStore> X509Util::sTestKeyStore;
AutoPtr<IKeyStore> X509Util::sSystemKeyStore;
AutoPtr<IFile> X509Util::sSystemCertificateDirectory;
AutoPtr<ISet> X509Util::sSystemTrustAnchorCache;
Boolean X509Util::sLoadedSystemKeyStore;
const AutoPtr<Object> X509Util::sLock = new Object();
Boolean X509Util::sDisableNativeCodeForTest = false;
AutoPtr< ArrayOf<Byte> > X509Util::HEX_DIGITS = X509Util::MiddleInitHexDigits();

AutoPtr<IX509Certificate> X509Util::CreateCertificateFromBytes(
    /* [in] */ ArrayOf<Byte>* derBytes)
{
    // ==================before translated======================
    // ensureInitialized();
    // return (X509Certificate) sCertificateFactory.generateCertificate(
    //         new ByteArrayInputStream(derBytes));
    assert(0);
    AutoPtr<IX509Certificate> empty;
    return empty;
}

ECode X509Util::AddTestRootCertificate(
    /* [in] */ ArrayOf<Byte>* rootCertBytes)
{
    VALIDATE_NOT_NULL(rootCertBytes);
    // ==================before translated======================
    // ensureInitialized();
    // X509Certificate rootCert = createCertificateFromBytes(rootCertBytes);
    // synchronized (sLock) {
    //     sTestKeyStore.setCertificateEntry(
    //             "root_cert_" + Integer.toString(sTestKeyStore.size()), rootCert);
    //     reloadTestTrustManager();
    // }
    assert(0);
    return NOERROR;
}

ECode X509Util::ClearTestRootCertificates()
{
    // ==================before translated======================
    // ensureInitialized();
    // synchronized (sLock) {
    //     try {
    //         sTestKeyStore.load(null);
    //         reloadTestTrustManager();
    //     } catch (IOException e) {
    //         // No IO operation is attempted.
    //     }
    // }
    assert(0);
    return NOERROR;
}

Boolean X509Util::VerifyKeyUsage(
    /* [in] */ IX509Certificate* certificate)
{
    // ==================before translated======================
    // List<String> ekuOids;
    // try {
    //     ekuOids = certificate.getExtendedKeyUsage();
    // } catch (NullPointerException e) {
    //     // getExtendedKeyUsage() can crash due to an Android platform bug. This probably
    //     // happens when the EKU extension data is malformed so return false here.
    //     // See http://crbug.com/233610
    //     return false;
    // }
    // if (ekuOids == null)
    //     return true;
    //
    // for (String ekuOid : ekuOids) {
    //     if (ekuOid.equals(OID_TLS_SERVER_AUTH) ||
    //         ekuOid.equals(OID_ANY_EKU) ||
    //         ekuOid.equals(OID_SERVER_GATED_NETSCAPE) ||
    //         ekuOid.equals(OID_SERVER_GATED_MICROSOFT)) {
    //         return true;
    //     }
    // }
    //
    // return false;
    assert(0);
    return FALSE;
}

AutoPtr<AndroidCertVerifyResult> X509Util::VerifyServerCertificates(
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* certChain,
    /* [in] */ const String& authType,
    /* [in] */ const String& host)
{
    // ==================before translated======================
    // if (certChain == null || certChain.length == 0 || certChain[0] == null) {
    //     throw new IllegalArgumentException("Expected non-null and non-empty certificate " +
    //             "chain passed as |certChain|. |certChain|=" + Arrays.deepToString(certChain));
    // }
    //
    //
    // try {
    //     ensureInitialized();
    // } catch (CertificateException e) {
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    // }
    //
    // X509Certificate[] serverCertificates = new X509Certificate[certChain.length];
    // try {
    //     for (int i = 0; i < certChain.length; ++i) {
    //         serverCertificates[i] = createCertificateFromBytes(certChain[i]);
    //     }
    // } catch (CertificateException e) {
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_UNABLE_TO_PARSE);
    // }
    //
    // // Expired and not yet valid certificates would be rejected by the trust managers, but the
    // // trust managers report all certificate errors using the general CertificateException. In
    // // order to get more granular error information, cert validity time range is being checked
    // // separately.
    // try {
    //     serverCertificates[0].checkValidity();
    //     if (!verifyKeyUsage(serverCertificates[0])) {
    //         return new AndroidCertVerifyResult(
    //                 CertVerifyStatusAndroid.VERIFY_INCORRECT_KEY_USAGE);
    //     }
    // } catch (CertificateExpiredException e) {
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_EXPIRED);
    // } catch (CertificateNotYetValidException e) {
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_NOT_YET_VALID);
    // } catch (CertificateException e) {
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    // }
    //
    // synchronized (sLock) {
    //     // If no trust manager was found, fail without crashing on the null pointer.
    //     if (sDefaultTrustManager == null)
    //         return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    //
    //     List<X509Certificate> verifiedChain;
    //     try {
    //         verifiedChain = sDefaultTrustManager.checkServerTrusted(serverCertificates,
    //                                                                 authType, host);
    //     } catch (CertificateException eDefaultManager) {
    //         try {
    //             verifiedChain = sTestTrustManager.checkServerTrusted(serverCertificates,
    //                                                                  authType, host);
    //         } catch (CertificateException eTestManager) {
    //             // Neither of the trust managers confirms the validity of the certificate chain,
    //             // log the error message returned by the system trust manager.
    //             Log.i(TAG, "Failed to validate the certificate chain, error: " +
    //                       eDefaultManager.getMessage());
    //             return new AndroidCertVerifyResult(
    //                     CertVerifyStatusAndroid.VERIFY_NO_TRUSTED_ROOT);
    //         }
    //     }
    //
    //     boolean isIssuedByKnownRoot = false;
    //     if (verifiedChain.size() > 0) {
    //         X509Certificate root = verifiedChain.get(verifiedChain.size() - 1);
    //         isIssuedByKnownRoot = isKnownRoot(root);
    //     }
    //
    //     return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_OK,
    //                                        isIssuedByKnownRoot, verifiedChain);
    // }
    assert(0);
    AutoPtr<AndroidCertVerifyResult> empty;
    return empty;
}

ECode X509Util::SetDisableNativeCodeForTest(
    /* [in] */ Boolean disabled)
{
    // ==================before translated======================
    // sDisableNativeCodeForTest = disabled;
    assert(0);
    return NOERROR;
}

ECode X509Util::EnsureInitialized()
{
    // ==================before translated======================
    // synchronized (sLock) {
    //     if (sCertificateFactory == null) {
    //         sCertificateFactory = CertificateFactory.getInstance("X.509");
    //     }
    //     if (sDefaultTrustManager == null) {
    //         sDefaultTrustManager = X509Util.createTrustManager(null);
    //     }
    //     if (!sLoadedSystemKeyStore) {
    //         try {
    //             sSystemKeyStore = KeyStore.getInstance("AndroidCAStore");
    //             try {
    //                 sSystemKeyStore.load(null);
    //             } catch (IOException e) {
    //                 // No IO operation is attempted.
    //             }
    //             sSystemCertificateDirectory =
    //                     new File(System.getenv("ANDROID_ROOT") + "/etc/security/cacerts");
    //         } catch (KeyStoreException e) {
    //             // Could not load AndroidCAStore. Continue anyway; isKnownRoot will always
    //             // return false.
    //         }
    //         if (!sDisableNativeCodeForTest)
    //             nativeRecordCertVerifyCapabilitiesHistogram(sSystemKeyStore != null);
    //         sLoadedSystemKeyStore = true;
    //     }
    //     if (sSystemTrustAnchorCache == null) {
    //         sSystemTrustAnchorCache = new HashSet<Pair<X500Principal, PublicKey>>();
    //     }
    //     if (sTestKeyStore == null) {
    //         sTestKeyStore = KeyStore.getInstance(KeyStore.getDefaultType());
    //         try {
    //             sTestKeyStore.load(null);
    //         } catch (IOException e) {
    //             // No IO operation is attempted.
    //         }
    //     }
    //     if (sTestTrustManager == null) {
    //         sTestTrustManager = X509Util.createTrustManager(sTestKeyStore);
    //     }
    //     if (!sDisableNativeCodeForTest && sTrustStorageListener == null) {
    //         sTrustStorageListener = new TrustStorageListener();
    //         nativeGetApplicationContext().registerReceiver(sTrustStorageListener,
    //                 new IntentFilter(KeyChain.ACTION_STORAGE_CHANGED));
    //     }
    // }
    assert(0);
    return NOERROR;
}

AutoPtr<X509Util::X509TrustManagerImplementation> X509Util::CreateTrustManager(
    /* [in] */ IKeyStore* keyStore)
{
    // ==================before translated======================
    // String algorithm = TrustManagerFactory.getDefaultAlgorithm();
    // TrustManagerFactory tmf = TrustManagerFactory.getInstance(algorithm);
    // tmf.init(keyStore);
    //
    // for (TrustManager tm : tmf.getTrustManagers()) {
    //     if (tm instanceof X509TrustManager) {
    //         try {
    //             if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
    //                 return new X509TrustManagerJellyBean((X509TrustManager) tm);
    //             } else {
    //                 return new X509TrustManagerIceCreamSandwich((X509TrustManager) tm);
    //             }
    //         } catch (IllegalArgumentException e) {
    //             String className = tm.getClass().getName();
    //             Log.e(TAG, "Error creating trust manager (" + className + "): " + e);
    //         }
    //     }
    // }
    // Log.e(TAG, "Could not find suitable trust manager");
    // return null;
    assert(0);
    AutoPtr<X509TrustManagerImplementation> empty;
    return empty;
}

ECode X509Util::ReloadTestTrustManager()
{
    // ==================before translated======================
    // sTestTrustManager = X509Util.createTrustManager(sTestKeyStore);
    assert(0);
    return NOERROR;
}

ECode X509Util::ReloadDefaultTrustManager()
{
    // ==================before translated======================
    // sDefaultTrustManager = null;
    // sSystemTrustAnchorCache = null;
    // nativeNotifyKeyChainChanged();
    // ensureInitialized();
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<Byte> > X509Util::MiddleInitHexDigits()
{
    // ==================before translated======================
    // ->WWZ_SIGN: ARRAY_INIT_START {
    // '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    //          'a', 'b', 'c', 'd', 'e', 'f',
    // ->WWZ_SIGN: ARRAY_INIT_END }
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

String X509Util::HashPrincipal(
    /* [in] */ IX500Principal* principal)
{
    // ==================before translated======================
    // // Android hashes a principal as the first four bytes of its MD5 digest, encoded in
    // // lowercase hex and reversed. Verified in 4.2, 4.3, and 4.4.
    // byte[] digest = MessageDigest.getInstance("MD5").digest(principal.getEncoded());
    // char[] hexChars = new char[8];
    // for (int i = 0; i < 4; i++) {
    //     hexChars[2 * i] = HEX_DIGITS[(digest[3 - i] >> 4) & 0xf];
    //     hexChars[2 * i + 1] = HEX_DIGITS[digest[3 - i] & 0xf];
    // }
    // return new String(hexChars);
    assert(0);
    return String("");
}

Boolean X509Util::IsKnownRoot(
    /* [in] */ IX509Certificate* root)
{
    // ==================before translated======================
    // // Could not find the system key store. Conservatively report false.
    // if (sSystemKeyStore == null)
    //     return false;
    //
    // // Check the in-memory cache first; avoid decoding the anchor from disk
    // // if it has been seen before.
    // Pair<X500Principal, PublicKey> key =
    //     new Pair<X500Principal, PublicKey>(root.getSubjectX500Principal(), root.getPublicKey());
    // if (sSystemTrustAnchorCache.contains(key))
    //     return true;
    //
    // // Note: It is not sufficient to call sSystemKeyStore.getCertificiateAlias. If the server
    // // supplies a copy of a trust anchor, X509TrustManagerExtensions returns the server's
    // // version rather than the system one. getCertificiateAlias will then fail to find an anchor
    // // name. This is fixed upstream in https://android-review.googlesource.com/#/c/91605/
    // //
    // // TODO(davidben): When the change trickles into an Android release, query sSystemKeyStore
    // // directly.
    //
    // // System trust anchors are stored under a hash of the principal. In case of collisions,
    // // a number is appended.
    // String hash = hashPrincipal(root.getSubjectX500Principal());
    // for (int i = 0; true; i++) {
    //     String alias = hash + '.' + i;
    //     if (!new File(sSystemCertificateDirectory, alias).exists())
    //         break;
    //
    //     Certificate anchor = sSystemKeyStore.getCertificate("system:" + alias);
    //     // It is possible for this to return null if the user deleted a trust anchor. In
    //     // that case, the certificate remains in the system directory but is also added to
    //     // another file. Continue iterating as there may be further collisions after the
    //     // deleted anchor.
    //     if (anchor == null)
    //         continue;
    //
    //     if (!(anchor instanceof X509Certificate)) {
    //         // This should never happen.
    //         String className = anchor.getClass().getName();
    //         Log.e(TAG, "Anchor " + alias + " not an X509Certificate: " + className);
    //         continue;
    //     }
    //
    //     // If the subject and public key match, this is a system root.
    //     X509Certificate anchorX509 = (X509Certificate)anchor;
    //     if (root.getSubjectX500Principal().equals(anchorX509.getSubjectX500Principal()) &&
    //         root.getPublicKey().equals(anchorX509.getPublicKey())) {
    //         sSystemTrustAnchorCache.add(key);
    //         return true;
    //     }
    // }
    //
    // return false;
    assert(0);
    return FALSE;
}

ECode X509Util::NativeNotifyKeyChainChanged()
{
    assert(0);
    return NOERROR;
}

ECode X509Util::NativeRecordCertVerifyCapabilitiesHistogram(
    /* [in] */ Boolean foundSystemTrustRoots)
{
    assert(0);
    return NOERROR;
}

AutoPtr<IContext> X509Util::NativeGetApplicationContext()
{
    assert(0);
    AutoPtr<IContext> empty;
    return empty;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


