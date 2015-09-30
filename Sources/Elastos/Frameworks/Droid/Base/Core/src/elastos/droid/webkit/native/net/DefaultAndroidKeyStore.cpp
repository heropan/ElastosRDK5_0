
#include "elastos/droid/webkit/native/net/DefaultAndroidKeyStore.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//           DefaultAndroidKeyStore::DefaultAndroidPrivateKey
//=====================================================================
DefaultAndroidKeyStore::DefaultAndroidPrivateKey::DefaultAndroidPrivateKey(
    /* [in] */ IPrivateKey* key,
    /* [in] */ DefaultAndroidKeyStore* store)
{
    // ==================before translated======================
    // mKey = key;
    // mStore = store;
}

AutoPtr<IPrivateKey> DefaultAndroidKeyStore::DefaultAndroidPrivateKey::GetJavaKey()
{
    // ==================before translated======================
    // return mKey;
    assert(0);
    AutoPtr<IPrivateKey> empty;
    return empty;
}

AutoPtr<AndroidKeyStore> DefaultAndroidKeyStore::DefaultAndroidPrivateKey::GetKeyStore()
{
    // ==================before translated======================
    // return mStore;
    assert(0);
    AutoPtr<AndroidKeyStore> empty;
    return empty;
}

//=====================================================================
//                        DefaultAndroidKeyStore
//=====================================================================
const String DefaultAndroidKeyStore::TAG("AndroidKeyStoreInProcessImpl");

AutoPtr<AndroidPrivateKey> DefaultAndroidKeyStore::CreateKey(
    /* [in] */ IPrivateKey* javaKey)
{
    // ==================before translated======================
    // return new DefaultAndroidPrivateKey(javaKey, this);
    assert(0);
    AutoPtr<AndroidPrivateKey> empty;
    return empty;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetRSAKeyModulus(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // if (javaKey instanceof RSAKey) {
    //     return ((RSAKey) javaKey).getModulus().toByteArray();
    // }
    // Log.w(TAG, "Not a RSAKey instance!");
    // return null;
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetDSAKeyParamQ(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // if (javaKey instanceof DSAKey) {
    //     DSAParams params = ((DSAKey) javaKey).getParams();
    //     return params.getQ().toByteArray();
    // }
    // Log.w(TAG, "Not a DSAKey instance!");
    // return null;
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetECKeyOrder(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // if (javaKey instanceof ECKey) {
    //     ECParameterSpec params = ((ECKey) javaKey).getParams();
    //     return params.getOrder().toByteArray();
    // }
    // Log.w(TAG, "Not an ECKey instance!");
    // return null;
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetPrivateKeyEncodedBytes(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // return javaKey.getEncoded();
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::RawSignDigestWithPrivateKey(
    /* [in] */ AndroidPrivateKey* key,
    /* [in] */ ArrayOf<Byte>* message)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // // Get the Signature for this key.
    // Signature signature = null;
    // // Hint: Algorithm names come from:
    // // http://docs.oracle.com/javase/6/docs/technotes/guides/security/StandardNames.html
    // try {
    //     if (javaKey instanceof RSAPrivateKey) {
    //         // IMPORTANT: Due to a platform bug, this will throw NoSuchAlgorithmException
    //         // on Android 4.0.x and 4.1.x. Fixed in 4.2 and higher.
    //         // See https://android-review.googlesource.com/#/c/40352/
    //         signature = Signature.getInstance("NONEwithRSA");
    //     } else if (javaKey instanceof DSAPrivateKey) {
    //         signature = Signature.getInstance("NONEwithDSA");
    //     } else if (javaKey instanceof ECPrivateKey) {
    //         signature = Signature.getInstance("NONEwithECDSA");
    //     }
    // } catch (NoSuchAlgorithmException e) {
    //     ;
    // }
    //
    // if (signature == null) {
    //     Log.e(TAG, "Unsupported private key algorithm: " + javaKey.getAlgorithm());
    //     return null;
    // }
    //
    // // Sign the message.
    // try {
    //     signature.initSign(javaKey);
    //     signature.update(message);
    //     return signature.sign();
    // } catch (Exception e) {
    //     Log.e(TAG, "Exception while signing message with " + javaKey.getAlgorithm() +
    //                 " private key: " + e);
    //     return null;
    // }
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

Int32 DefaultAndroidKeyStore::GetPrivateKeyType(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // if (javaKey instanceof RSAPrivateKey)
    //     return PrivateKeyType.RSA;
    // if (javaKey instanceof DSAPrivateKey)
    //     return PrivateKeyType.DSA;
    // if (javaKey instanceof ECPrivateKey)
    //     return PrivateKeyType.ECDSA;
    // else
    //     return PrivateKeyType.INVALID;
    assert(0);
    return 0;
}

Int64 DefaultAndroidKeyStore::GetOpenSSLHandleForPrivateKey(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // // Sanity checks
    // if (javaKey == null) {
    //     Log.e(TAG, "key == null");
    //     return 0;
    // }
    // if (!(javaKey instanceof RSAPrivateKey)) {
    //     Log.e(TAG, "does not implement RSAPrivateKey");
    //     return 0;
    // }
    // // First, check that this is a proper instance of OpenSSLRSAPrivateKey
    // // or one of its sub-classes.
    // Class<?> superClass;
    // try {
    //     superClass = Class.forName(
    //             "org.apache.harmony.xnet.provider.jsse.OpenSSLRSAPrivateKey");
    // } catch (Exception e) {
    //     // This may happen if the target device has a completely different
    //     // implementation of the java.security APIs, compared to vanilla
    //     // Android. Highly unlikely, but still possible.
    //     Log.e(TAG, "Cannot find system OpenSSLRSAPrivateKey class: " + e);
    //     return 0;
    // }
    // if (!superClass.isInstance(javaKey)) {
    //     // This may happen if the PrivateKey was not created by the "AndroidOpenSSL"
    //     // provider, which should be the default. That could happen if an OEM decided
    //     // to implement a different default provider. Also highly unlikely.
    //     Log.e(TAG, "Private key is not an OpenSSLRSAPrivateKey instance, its class name is:" +
    //                javaKey.getClass().getCanonicalName());
    //     return 0;
    // }
    //
    // try {
    //     // Use reflection to invoke the 'getOpenSSLKey()' method on
    //     // the private key. This returns another Java object that wraps
    //     // a native EVP_PKEY. Note that the method is final, so calling
    //     // the superclass implementation is ok.
    //     Method getKey = superClass.getDeclaredMethod("getOpenSSLKey");
    //     getKey.setAccessible(true);
    //     Object opensslKey = null;
    //     try {
    //         opensslKey = getKey.invoke(javaKey);
    //     } finally {
    //         getKey.setAccessible(false);
    //     }
    //     if (opensslKey == null) {
    //         // Bail when detecting OEM "enhancement".
    //         Log.e(TAG, "getOpenSSLKey() returned null");
    //         return 0;
    //     }
    //
    //     // Use reflection to invoke the 'getPkeyContext' method on the
    //     // result of the getOpenSSLKey(). This is an 32-bit integer
    //     // which is the address of an EVP_PKEY object. Note that this
    //     // method these days returns a 64-bit long, but since this code
    //     // path is used for older Android versions, it may still return
    //     // a 32-bit int here. To be on the safe side, we cast the return
    //     // value via Number rather than directly to Integer or Long.
    //     Method getPkeyContext;
    //     try {
    //         getPkeyContext = opensslKey.getClass().getDeclaredMethod("getPkeyContext");
    //     } catch (Exception e) {
    //         // Bail here too, something really not working as expected.
    //         Log.e(TAG, "No getPkeyContext() method on OpenSSLKey member:" + e);
    //         return 0;
    //     }
    //     getPkeyContext.setAccessible(true);
    //     long evp_pkey = 0;
    //     try {
    //         evp_pkey = ((Number) getPkeyContext.invoke(opensslKey)).longValue();
    //     } finally {
    //         getPkeyContext.setAccessible(false);
    //     }
    //     if (evp_pkey == 0) {
    //         // The PrivateKey is probably rotten for some reason.
    //         Log.e(TAG, "getPkeyContext() returned null");
    //     }
    //     return evp_pkey;
    //
    // } catch (Exception e) {
    //     Log.e(TAG, "Exception while trying to retrieve system EVP_PKEY handle: " + e);
    //     return 0;
    // }
    assert(0);
    return 0;
}

ECode DefaultAndroidKeyStore::ReleaseKey(
    /* [in] */ AndroidPrivateKey* key)
{
    VALIDATE_NOT_NULL(key);
    // ==================before translated======================
    // // no-op for in-process. GC will handle key collection
    assert(0);
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


