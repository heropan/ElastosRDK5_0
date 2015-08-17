
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
//       DefaultAndroidKeyStore::DefaultAndroidPrivateKey
//===============================================================
DefaultAndroidKeyStore::DefaultAndroidPrivateKey::DefaultAndroidPrivateKey(
    /* [in] */ IPrivateKey* key,
    /* [in] */ DefaultAndroidKeyStore* store)
    : mKey(key)
    , mStore(store)
{
}

AutoPtr<AndroidKeyStore> DefaultAndroidKeyStore::DefaultAndroidPrivateKey::GetKeyStore()
{
    AutoPtr<AndroidKeyStore> ret(mStore);
    return ret;
}

AutoPtr<IPrivateKey> DefaultAndroidKeyStore::DefaultAndroidPrivateKey::GetJavaKey()
{
    return mKey;
}

//===============================================================
//                   DefaultAndroidKeyStore
//===============================================================
const String DefaultAndroidKeyStore::TAG("AndroidKeyStoreInProcessImpl");

AutoPtr<AndroidPrivateKey> DefaultAndroidKeyStore::CreateKey(
    /* [in] */ IPrivateKey* javaKey)
{
    AutoPtr<AndroidPrivateKey> privateKey = new DefaultAndroidPrivateKey(javaKey, this);
    return privateKey;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetRSAKeyModulus(
    /* [in] */ AndroidPrivateKey* key)
{
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IRSAKey> rsakey = (IRSAKey*)javaKey->Probe(EIID_IRSAKey);
    if (rsakey != NULL) {
           AutoPtr<IBigInteger> bigInteger;
           CBigInteger::New((IBigInteger**)&bigInteger);
           rsakey->GetModulus(&bigInteger);

           AutoPtr< ArrayOf<Byte> > result;
        bigInteger->ToByteArray(&result);
        return result;
    }
    //Log.w(TAG, "Not a RSAKey instance!");
    return NULL;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetDSAKeyParamQ(
    /* [in] */ AndroidPrivateKey* key)
{
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IDSAKey> dsaKey = (IDSAKey*)javaKey->Probe(EIID_IDSAKey);
    if (dsakey != NULL) {
        AutoPtr<IDSAParams> params;
        CDSAParams::New((IDSAParams**)&params);
        dsakey->GetParams(&params);

        AutoPtr<IBigInteger> bigInteger;
           CBigInteger::New((IBigInteger**)&bigInteger);
           params->GetQ(&bigInteger);

        AutoPtr< ArrayOf<Byte> > result;
        bigInteger->ToByteArray(&result);
        return result;
    }
    //Log.w(TAG, "Not a DSAKey instance!");
    return NULL;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetECKeyOrder(
    /* [in] */ AndroidPrivateKey* key)
{
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IECKey> eckkey = (IECKey*)javaKey->Probe(EIID_IECKey);
    if (eckkey != NULL) {
        AutoPtr<IECParameterSpec> params;
        CECParameterSpec::New((IECParameterSpec**)&params);
        eckkey->GetParams(&params);

        AutoPtr<IBigInteger> bigInteger;
           CBigInteger::New((IBigInteger**)&bigInteger);
        params->GetOrder(&bigInteger);

        AutoPtr< ArrayOf<Byte> > result;
        bigInteger->ToByteArray(&result);
        return result;
    }
    //Log.w(TAG, "Not an ECKey instance!");
    return NULL;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::GetPrivateKeyEncodedBytes(
    /* [in] */ AndroidPrivateKey* key)
{
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr< ArrayOf<Byte> > result;
    javaKey->GetEncoded(&result);
    return result;
}

AutoPtr< ArrayOf<Byte> > DefaultAndroidKeyStore::RawSignDigestWithPrivateKey(
    /* [in] */ AndroidPrivateKey* key,
    /* [in] */ ArrayOf<Byte>* message)
{
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
               CSignature::New(String("NONEwithRSA"), (ISignature**)&signature);
        }
        else if (dsakey != NULL) {
              CSignature::New(String("NONEwithDSA"), (ISignature**)&signature);
        }
        else if (eckey != NULL) {
               CSignature::New(String("NONEwithECDSA"), (ISignature**)&signature);
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
        signature->InitSign((IPrivateKey**)&javaKey);
        signature->UpdateEx(message);
        AutoPtr< ArrayOf<Byte> > res;
        signature->Sign(&res);
        return res;
    //} catch (Exception e) {
    //    Log.e(TAG, "Exception while signing message with " + javaKey.getAlgorithm() +
                    " private key: " + e);
    //    return null;
    //}
}

Int32 DefaultAndroidKeyStore::GetPrivateKeyType(
    /* [in] */ AndroidPrivateKey* key)
{
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    AutoPtr<IRSAPrivateKey> rsakey = (IRSAPrivateKey*)javaKey->Probe(EIID_IRSAPrivateKey);
    AutoPtr<IDSAPrivateKey> dsakey = (IDSAPrivateKey*)javaKey->Probe(EIID_IDSAPrivateKey);
    AutoPtr<IECPrivateKey> eckey = (IECPrivateKey*)javaKey->Probe(EIID_IECPrivateKey);

    if (rsakey != NULL)
        return IPrivateKeyType::RSA;
    if (dsakey != NULL)
        return IPrivateKeyType::DSA;
    if (eckey != NULL)
        return IPrivateKeyType::ECDSA;
    else
        return IPrivateKeyType::INVALID;
}

Int64 DefaultAndroidKeyStore::GetOpenSSLHandleForPrivateKey(
    /* [in] */ AndroidPrivateKey* key)
{
    AutoPtr<IPrivateKey> javaKey = ((DefaultAndroidPrivateKey*)key)->GetJavaKey();
    // Sanity checks
    if (javaKey == NULL) {
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
    AutoPtr<IOpenSSLRSAPrivateKey> superClass;
    COpenSSLRSAPrivateKey::New((IOpenSSLRSAPrivateKey**)&superClass);
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
    AutoPtr<IOpenSSLRSAPrivateKey> sslKey = (IOpenSSLRSAPrivateKey*)javaKey->Probe(EIID_IOpenSSLRSAPrivateKey);
    if (NULL == sslKey) {
        // This may happen if the PrivateKey was not created by the "AndroidOpenSSL"
        // provider, which should be the default. That could happen if an OEM decided
        // to implement a different default provider. Also highly unlikely.
        //Log.e(TAG, "Private key is not an OpenSSLRSAPrivateKey instance, its class name is:" + javaKey.getClass().getCanonicalName());
        return 0;
    }

    //try {
        // Use reflection to invoke the 'getOpenSSLKey()' method on
        // the private key. This returns another Java object that wraps
        // a native EVP_PKEY. Note that the method is final, so calling
        // the superclass implementation is ok.
        AutoPtr<IMethod> getKey;
        superClass->GetDeclaredMethod(String("getOpenSSLKey"), (IMethod**)&getKey);
        getKey->SetAccessible(TRUE);
        AutoPtr<Object> opensslKey;
        //try {
            getKey->Invoke(javaKey, &opensslKey);
        //} finally {
            getKey->SetAccessible(FALSE);
        //}
        if (opensslKey == NULL) {
            // Bail when detecting OEM "enhancement".
            //Log.e(TAG, "getOpenSSLKey() returned null");
            return 0;
        }

        // Use reflection to invoke the 'getPkeyContext' method on the
        // result of the getOpenSSLKey(). This is an 32-bit integer
        // which is the address of an EVP_PKEY object. Note that this
        // method these days returns a 64-bit long, but since this code
        // path is used for older Android versions, it may still return
        // a 32-bit int here. To be on the safe side, we cast the return
        // value via Number rather than directly to Integer or Long.
        AutoPtr<IMethod> getPkeyContext;
        //try {
            superClass->GetDeclaredMethod(String("getPkeyContext"), (IMethod**)&getPkeyContext);
        //} catch (Exception e) {
            // Bail here too, something really not working as expected.
        //    Log.e(TAG, "No getPkeyContext() method on OpenSSLKey member:" + e);
        //    return 0;
        //}
        getPkeyContext->SetAccessible(TRUE);
        Int64 evp_pkey = 0;
        //try {
            AutoPtr<INumber> number;
            getPkeyContext->Invoke(&opensslKey, (INumber**)&number);
            number->Int64Value(&evp_pkey);
        //} finally {
            getPkeyContext->SetAccessible(FALSE);
        //}
        if (evp_pkey == 0) {
            // The PrivateKey is probably rotten for some reason.
            //Log.e(TAG, "getPkeyContext() returned null");
        }
        return evp_pkey;

    //} catch (Exception e) {
    //    Log.e(TAG, "Exception while trying to retrieve system EVP_PKEY handle: " + e);
    //    return 0;
    //}
}

ECode DefaultAndroidKeyStore::ReleaseKey(
    /* [in] */ AndroidPrivateKey* key)
{
    VALIDATE_NOT_NULL(key);
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
