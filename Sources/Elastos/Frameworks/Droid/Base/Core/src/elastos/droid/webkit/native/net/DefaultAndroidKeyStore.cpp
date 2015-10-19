
#include "elastos/droid/webkit/native/net/DefaultAndroidKeyStore.h"
#include "elastos/droid/webkit/native/net/PrivateKeyType.h"
#include "elastos/droid/content/pm/CSignature.h"

using Elastos::Security::IPrivateKey;
using Elastos::Security::ISignature;
using Elastos::Security::Interfaces::IRSAKey;
using Elastos::Security::Interfaces::EIID_IRSAKey;
using Elastos::Security::Interfaces::IDSAKey;
using Elastos::Security::Interfaces::EIID_IDSAKey;
using Elastos::Security::Interfaces::IDSAParams;
using Elastos::Security::Interfaces::IECKey;
using Elastos::Security::Interfaces::EIID_IECKey;
using Elastos::Security::Interfaces::IRSAPrivateKey;
using Elastos::Security::Interfaces::IDSAPrivateKey;
using Elastos::Security::Interfaces::IECPrivateKey;
using Elastos::Security::Interfaces::EIID_IRSAPrivateKey;
using Elastos::Security::Interfaces::EIID_IDSAPrivateKey;
using Elastos::Security::Interfaces::EIID_IECPrivateKey;
using Elastos::Security::Spec::IECParameterSpec;
using Elastos::Math::IBigInteger;
using Elastos::Math::CBigInteger;
using Elastos::Droid::Content::Pm::CSignature;
using Elastos::Droid::Webkit::Net::PrivateKeyType;

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
    : mKey(key)
    , mStore(store)
{
    // ==================before translated======================
    // mKey = key;
    // mStore = store;
}

AutoPtr<IPrivateKey> DefaultAndroidKeyStore::DefaultAndroidPrivateKey::GetJavaKey()
{
    // ==================before translated======================
    // return mKey;

    return mKey;
}

AutoPtr<AndroidKeyStore> DefaultAndroidKeyStore::DefaultAndroidPrivateKey::GetKeyStore()
{
    // ==================before translated======================
    // return mStore;

    AutoPtr<AndroidKeyStore> result = (AndroidKeyStore*)&mStore;
    return result;
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


    AutoPtr<AndroidPrivateKey> result = new DefaultAndroidPrivateKey(javaKey, this);;
    return result;
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

    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IRSAKey> rsakey = (IRSAKey*)javaKey->Probe(EIID_IRSAKey);
    if (rsakey != NULL) {
        AutoPtr<IBigInteger> bigInteger;
        CBigInteger::New((IBigInteger**)&bigInteger);
        rsakey->GetModulus((IBigInteger**)&bigInteger);

        AutoPtr< ArrayOf<Byte> > result;
        bigInteger->ToByteArray((ArrayOf<Byte>**)&result);
        return result;
    }
    //Log.w(TAG, "Not a RSAKey instance!");
    return NULL;
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
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IDSAKey> dsaKey = (IDSAKey*)javaKey->Probe(EIID_IDSAKey);
    if (dsaKey != NULL) {
        AutoPtr<IDSAParams> params;
        dsaKey->GetParams((IDSAParams**)&params);

        AutoPtr<IBigInteger> bigInteger;
        CBigInteger::New((IBigInteger**)&bigInteger);
        params->GetQ((IBigInteger**)&bigInteger);

        AutoPtr< ArrayOf<Byte> > result;
        bigInteger->ToByteArray((ArrayOf<Byte>**)&result);
        return result;
    }
    //Log.w(TAG, "Not a DSAKey instance!");
    return NULL;
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
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IECKey> eckkey = (IECKey*)javaKey->Probe(EIID_IECKey);
    if (eckkey != NULL) {
        AutoPtr<IECParameterSpec> params;
        eckkey->GetParams((IECParameterSpec**)&params);

        AutoPtr<IBigInteger> bigInteger;
        CBigInteger::New((IBigInteger**)&bigInteger);
        params->GetOrder((IBigInteger**)&bigInteger);

        AutoPtr< ArrayOf<Byte> > result;
        bigInteger->ToByteArray((ArrayOf<Byte>**)&result);
        return result;
    }
    //Log.w(TAG, "Not an ECKey instance!");
    return NULL;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetPrivateKeyEncodedBytes(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // PrivateKey javaKey = ((DefaultAndroidPrivateKey) key).getJavaKey();
    // return javaKey.getEncoded();

    assert(0);
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr< ArrayOf<Byte> > result;
    //javaKey->GetEncoded((ArrayOf<Byte>**)&result); // no matching function
    return result;
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
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    // Get the Signature for this key.
    AutoPtr<ISignature> signature;
    // Hint: Algorithm names come from:
    // http://docs.oracle.com/javase/6/docs/technotes/guides/security/StandardNames.html
    //try {
        AutoPtr<IRSAPrivateKey> rsakey = (IRSAPrivateKey*)javaKey->Probe(EIID_IRSAPrivateKey);
        AutoPtr<IDSAPrivateKey> dsakey = (IDSAPrivateKey*)javaKey->Probe(EIID_IDSAPrivateKey);
        AutoPtr<IECPrivateKey> eckey = (IECPrivateKey*)javaKey->Probe(EIID_IECPrivateKey);
        if (rsakey != NULL) {
            // IMPORTANT: Due to a platform bug, this will throw NoSuchAlgorithmException
            // on Android 4.0.x and 4.1.x. Fixed in 4.2 and higher.
            // See https://android-review.googlesource.com/#/c/40352/
            //CSignature::New(String("NONEwithRSA"), (ISignature**)&signature);
        }
        else if (dsakey != NULL) {
            //CSignature::New(String("NONEwithDSA"), (ISignature**)&signature);
        }
        else if (eckey != NULL) {
            //CSignature::New(String("NONEwithECDSA"), (ISignature**)&signature);
        }
    //} catch (NoSuchAlgorithmException e) {
    //    ;
    //}

    if (signature == NULL) {
        //Log.e(TAG, "Unsupported private key algorithm: " + javaKey.getAlgorithm());
        return NULL;
    }

    // Sign the message.
    //try {
        signature->InitSign((IPrivateKey*)&javaKey);
        signature->Update(message);
        AutoPtr< ArrayOf<Byte> > res;
        signature->Sign((ArrayOf<Byte>**)&res);
        return res;
    //} catch (Exception e) {
    //    Log.e(TAG, "Exception while signing message with " + javaKey.getAlgorithm() +
    //                " private key: " + e);
    //    return null;
    //}
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
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IRSAPrivateKey> rsakey = (IRSAPrivateKey*)javaKey->Probe(EIID_IRSAPrivateKey);
    AutoPtr<IDSAPrivateKey> dsakey = (IDSAPrivateKey*)javaKey->Probe(EIID_IDSAPrivateKey);
    AutoPtr<IECPrivateKey> eckey = (IECPrivateKey*)javaKey->Probe(EIID_IECPrivateKey);

    if (rsakey != NULL)
        return PrivateKeyType::RSA;
    if (dsakey != NULL)
        return PrivateKeyType::DSA;
    if (eckey != NULL)
        return PrivateKeyType::ECDSA;
    else
        return PrivateKeyType::INVALID;
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
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    // Sanity checks
    if (NULL == javaKey) {
        //Log.e(TAG, "key == null");
        return 0;
    }

    AutoPtr<IRSAPrivateKey> rsaKey = (IRSAPrivateKey*)javaKey->Probe(EIID_IRSAPrivateKey);
    if (NULL == rsaKey) {
        //Log.e(TAG, "does not implement RSAPrivateKey");
        return 0;
    }
    // First, check that this is a proper instance of OpenSSLRSAPrivateKey
    // or one of its sub-classes.

    // question: how to using reflection mechanism to initialize a class
    //Class<?> superClass;
    //--AutoPtr<IOpenSSLRSAPrivateKey> superClass;
    //--COpenSSLRSAPrivateKey::New((IOpenSSLRSAPrivateKey**)&superClass);
    //try {
        //superClass = Class.forName(
        //        "org.apache.harmony.xnet.provider.jsse.OpenSSLRSAPrivateKey");
    //} catch (Exception e) {
        // This may happen if the target device has a completely different
        // implementation of the java.security APIs, compared to vanilla
        // Android. Highly unlikely, but still possible.
    //    Log.e(TAG, "Cannot find system OpenSSLRSAPrivateKey class: " + e);
    //    return 0;
    //}
    //--AutoPtr<IOpenSSLRSAPrivateKey> sslKey = (IOpenSSLRSAPrivateKey*)javaKey->Probe(EIID_IOpenSSLRSAPrivateKey);
    //--if (NULL == sslKey) {
        // This may happen if the PrivateKey was not created by the "AndroidOpenSSL"
        // provider, which should be the default. That could happen if an OEM decided
        // to implement a different default provider. Also highly unlikely.
        //Log.e(TAG, "Private key is not an OpenSSLRSAPrivateKey instance, its class name is:" + javaKey.getClass().getCanonicalName());
        return 0;
    //--}

    //try {
        // Use reflection to invoke the 'getOpenSSLKey()' method on
        // the private key. This returns another Java object that wraps
        // a native EVP_PKEY. Note that the method is final, so calling
        // the superclass implementation is ok.
        //--AutoPtr<IMethod> getKey;
        //--superClass->GetDeclaredMethod(String("getOpenSSLKey"), (IMethod**)&getKey);
        //--getKey->SetAccessible(TRUE);
        //--AutoPtr<Object> opensslKey;
        //try {
            //--getKey->Invoke(javaKey, &opensslKey);
        //} finally {
            //--getKey->SetAccessible(FALSE);
        //}
        //--if (opensslKey == NULL) {
            // Bail when detecting OEM "enhancement".
            //Log.e(TAG, "getOpenSSLKey() returned null");
            return 0;
        //--}

        // Use reflection to invoke the 'getPkeyContext' method on the
        // result of the getOpenSSLKey(). This is an 32-bit integer
        // which is the address of an EVP_PKEY object. Note that this
        // method these days returns a 64-bit long, but since this code
        // path is used for older Android versions, it may still return
        // a 32-bit int here. To be on the safe side, we cast the return
        // value via Number rather than directly to Integer or Long.
        //--AutoPtr<IMethod> getPkeyContext;
        //try {
            //--superClass->GetDeclaredMethod(String("getPkeyContext"), (IMethod**)&getPkeyContext);
        //} catch (Exception e) {
            // Bail here too, something really not working as expected.
        //    Log.e(TAG, "No getPkeyContext() method on OpenSSLKey member:" + e);
        //    return 0;
        //}
        //--getPkeyContext->SetAccessible(TRUE);
        //--Int64 evp_pkey = 0;
        //try {
            //--AutoPtr<INumber> number;
            //--getPkeyContext->Invoke(&opensslKey, (INumber**)&number);
            //--number->Int64Value(&evp_pkey);
        //} finally {
            //--getPkeyContext->SetAccessible(FALSE);
        //}
        //--if (evp_pkey == 0) {
            // The PrivateKey is probably rotten for some reason.
            //Log.e(TAG, "getPkeyContext() returned null");
        //--}
        //--return evp_pkey;

    //} catch (Exception e) {
    //    Log.e(TAG, "Exception while trying to retrieve system EVP_PKEY handle: " + e);
    //    return 0;
    //}
}

ECode DefaultAndroidKeyStore::ReleaseKey(
    /* [in] */ AndroidPrivateKey* key)
{
    VALIDATE_NOT_NULL(key);
    // ==================before translated======================
    // // no-op for in-process. GC will handle key collection

    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


