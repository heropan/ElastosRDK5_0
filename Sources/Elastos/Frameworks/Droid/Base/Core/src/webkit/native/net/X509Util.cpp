
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
// 				X509Util::TrustStorageListener
//===============================================================
ECode X509Util::TrustStorageListener::OnReceive(
	/* [in] */ IContext* context,
	/* [in] */ IIntent* intent)
{
	VALIDATE_NOT_NULL(context);
	VALIDATE_NOT_NULL(intent);

	String action;
	intent->GetAction(&action);
	if (action.Equals(IKeyChain::ACTION_STORAGE_CHANGED)) {
	    //try {
	        ReloadDefaultTrustManager();
	    //}
	    //catch (CertificateException e) {
	    //    Log.e(TAG, "Unable to reload the default TrustManager", e);
	    //}
	    //catch (KeyStoreException e) {
	    //    Log.e(TAG, "Unable to reload the default TrustManager", e);
	    //}
	    //catch (NoSuchAlgorithmException e) {
	    //    Log.e(TAG, "Unable to reload the default TrustManager", e);
	    //}
	}
	return NOERROR;
}


//===============================================================
// 			X509Util::X509TrustManagerIceCreamSandwich
//===============================================================
X509Util::X509TrustManagerIceCreamSandwich::X509TrustManagerIceCreamSandwich(
    /* [in] */ IX509TrustManager* trustManager)
    : mTrustManager(trustManager)
{
}

//@Override
AutoPtr< List< AutoPtr<IX509Certificate> > > X509Util::X509TrustManagerIceCreamSandwich::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate*>* chain,
    /* [in] */ String authType,
    /* [in] */ String host)
{
    mTrustManager->CheckServerTrusted(chain, authType);
	//return ICollections.<X509Certificate>emptyList();
	AutoPtr< List< AutoPtr<IX509Certificate> > > ret = new List<AutoPtr<IX509Certificate> >();
	return ret;
}

//===============================================================
// 				X509Util::X509TrustManagerJellyBean
//===============================================================
//@SuppressLint("NewApi")
X509Util::X509TrustManagerJellyBean::X509TrustManagerJellyBean(
    /* [in] */ IX509TrustManager* trustManager)
{
	// question: init
	//CX509TrustManagerExtensions::New((IX509TrustManagerExtensions**)&mTrustManagerExtensions);
    mTrustManagerExtensions = new X509TrustManagerExtensions(trustManager);
}

//@Override
AutoPtr< List< AutoPtr<IX509Certificate> > > X509Util::X509TrustManagerJellyBean::CheckServerTrusted(
    /* [in] */ ArrayOf<IX509Certificate*>* chain,
    /* [in] */ String authType,
    /* [in] */ String host)
{
	AutoPtr<List<AutoPtr<IX509Certificate> > > lst = new List<X509Certificate>();
    mTrustManagerExtensions->CheckServerTrusted(chain, authType, host, &lst);
    return lst;
}

//===============================================================
// 							X509Util
//===============================================================
const String X509Util::TAG("X509Util");

AutoPtr<ICertificateFactory> X509Util::sCertificateFactory;

const String X509Util::OID_TLS_SERVER_AUTH("1.3.6.1.5.5.7.3.1");

const String X509Util::OID_ANY_EKU("2.5.29.37.0");

const String X509Util::OID_SERVER_GATED_NETSCAPE("2.16.840.1.113730.4.1");

const String X509Util::OID_SERVER_GATED_MICROSOFT("1.3.6.1.4.1.311.10.3.3");

AutoPtr<X509TrustManagerImplementation> X509Util::sDefaultTrustManager;

AutoPtr<TrustStorageListener> X509Util::sTrustStorageListener;

AutoPtr<X509TrustManagerImplementation> X509Util::sTestTrustManager;

AutoPtr<IKeyStore> X509Util::sTestKeyStore;

AutoPtr<IKeyStore> X509Util::sSystemKeyStore;

AutoPtr<IFile> X509Util::sSystemCertificateDirectory;

AutoPtr<ISet<Pair<AutoPtr<IX500Principal>, AutoPtr<IPublicKey*> > > > X509Util::sSystemTrustAnchorCache;

Boolean X509Util::sLoadedSystemKeyStore;

const Object X509Util::sLock;

Boolean X509Util::sDisableNativeCodeForTest = FALSE;

const AutoPtr< ArrayOf<Char16> > X509Util::HEX_DIGITS = X509Util::InitHexDigits();

/**
 * Convert a DER encoded certificate to an X509Certificate.
 */
AutoPtr<IX509Certificate> X509Util::CreateCertificateFromBytes(
    /* [in] */ ArrayOf<Byte>* derBytes)
{
	EnsureInitialized();
	AutoPtr<IX509Certificate> cert;
    AutoPtr<IX509CertificateHelper> helper;
    CX509CertificateHelper::AcquireSingleton((IX509CertificateHelper**)&helper);
	hepler->CreateInstallIntent((IX509Certificate**)&cert);
    (IX509Certificate*) sCertificateFactory->GenerateCertificate(new ByteArrayInputStream(derBytes), &cert);
    return cert;
}

ECode X509Util::AddTestRootCertificate(
    /* [in] */ ArrayOf<Byte>* rootCertBytes)
{
	VALIDATE_NOT_NULL(rootCertBytes);
    EnsureInitialized();
    AutoPtr<IX509Certificate> rootCert = CreateCertificateFromBytes(rootCertBytes);

    {
    	Object::AutoLock lock(sLock);
    	sTestKeyStore->SetCertificateEntry(String("root_cert_") + IInteger32::ToString(sTestKeyStore->GetLenght()), rootCert);
        ReloadTestTrustManager();
    }

    return NOERROR;
}

ECode X509Util::ClearTestRootCertificates()
{
    EnsureInitialized();

    {
    	Object::AutoLock lock(sLock);
        //try {
            sTestKeyStore->Load(NULL);
            ReloadTestTrustManager();
        //} catch (IOException e) {
            // No IO operation is attempted.
        //}
    }

    return NOERROR;
}

AutoPtr<AndroidCertVerifyResult> X509Util::VerifyServerCertificates(
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* certChain,
    /* [in] */ String authType,
    /* [in] */ String host)
{
    if (certChain == NULL || certChain->GetLenght() == 0 || certChain[0] == NULL) {
        //throw new IllegalArgumentException("Expected non-null and non-empty certificate " +
        //        "chain passed as |certChain|. |certChain|=" + Arrays.deepToString(certChain));
		AutoPtr<AndroidCertVerifyResult> ret = NULL;
        return ret;
    }

    //try {
        EnsureInitialized();
    //} catch (CertificateException e) {
    //    return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    //}

    AutoPtr< ArrayOf< AutoPtr<IX509Certificate> > > serverCertificates = new X509Certificate[certChain->GetLenght()];
    //try {
        for (Int32 i = 0; i < certChain->GetLenght; ++i) {
            (*serverCertificates)[i] = CreateCertificateFromBytes(certChain[i]);
        }
    //} catch (CertificateException e) {
    //    return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_UNABLE_TO_PARSE);
    //}

    // Expired and not yet valid certificates would be rejected by the trust managers, but the
    // trust managers report all certificate errors using the general CertificateException. In
    // order to get more granular error information, cert validity time range is being checked
    // separately.
    //try {
        serverCertificates[0]->CheckValidity();
        if (!VerifyKeyUsage(serverCertificates[0])) {
        	AutoPtr<AndroidCertVerifyResult> ret = new AndroidCertVerifyResult(ICertVerifyStatusAndroid::VERIFY_INCORRECT_KEY_USAGE);
            return ret;
        }
    //} catch (CertificateExpiredException e) {
    //    return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_EXPIRED);
    //} catch (CertificateNotYetValidException e) {
    //    return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_NOT_YET_VALID);
    //} catch (CertificateException e) {
    //    return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    //}

    {
    	Object::AutoLock lock(sLock);
        // If no trust manager was found, fail without crashing on the null pointer.
        if (sDefaultTrustManager == NULL) {
        	AutoPtr<AndroidCertVerifyResult> ret = new AndroidCertVerifyResult(ICertVerifyStatusAndroid::VERIFY_FAILED);
            return ret;
        }

        List< AutoPtr<IX509Certificate> > verifiedChain;
        //try {
            sDefaultTrustManager->CheckServerTrusted(serverCertificates, authType, host, &verifiedChain);
        //} catch (CertificateException eDefaultManager) {
        //    try {
                sTestTrustManager->CheckServerTrusted(serverCertificates, authType, host, &verifiedChain);
        //    } catch (CertificateException eTestManager) {
                // Neither of the trust managers confirms the validity of the certificate chain,
                // log the error message returned by the system trust manager.
        //        Log.i(TAG, "Failed to validate the certificate chain, error: " +
        //                  eDefaultManager.getMessage());
        //        return new AndroidCertVerifyResult(
        //                CertVerifyStatusAndroid.VERIFY_NO_TRUSTED_ROOT);
        //    }
        //}

        Boolean isIssuedByKnownRoot = FALSE;
        if (verifiedChain->GetLength() > 0) {
            AutoPtr<IX509Certificate> root = verifiedChain->Get(verifiedChain->GetLenght() - 1);
            isIssuedByKnownRoot = IsKnownRoot(root);
        }

		AutoPtr<AndroidCertVerifyResult> ret = new AndroidCertVerifyResult(ICertVerifyStatusAndroid::VERIFY_OK,
                                           isIssuedByKnownRoot, verifiedChain);
        return ret;
    }
}

ECode X509Util::SetDisableNativeCodeForTest(
    /* [in] */ Boolean disabled)
{
    sDisableNativeCodeForTest = disabled;
    return NOERROR;
}

ECode X509Util::EnsureInitialized()
{
    {
    	Object::AutoLock lock(sLock);

        if (sCertificateFactory == NULL) {
            CCertificateFactory::New(String("X.509"), (ICertificateFactory**)&sCertificateFactory);
        }
        if (sDefaultTrustManager == NULL) {
            sDefaultTrustManager = X509Util::CreateTrustManager(NULL);
        }
        if (!sLoadedSystemKeyStore) {
            //try {
                CKeyStore::New(String("AndroidCAStore"), (ICertificateFactory**)&sSystemKeyStore);
                //try {
                    sSystemKeyStore->Load(NULL);
                //} catch (IOException e) {
                    // No IO operation is attempted.
                //}
                String sysEnv;
                AutoPtr<ISystem> sys;
				CSystem::New((ISystem**)&sys);
				sys->GetEnv(String("ANDROID_ROOT") + String("/etc/security/cacerts"), &sysEnv);
				CFile::New(sysEnv, (IFile**)&sSystemCertificateDirectory);
            //} catch (KeyStoreException e) {
                // Could not load AndroidCAStore. Continue anyway; isKnownRoot will always
                // return false.
            //}
            if (NULL != sDisableNativeCodeForTest)
                NativeRecordCertVerifyCapabilitiesHistogram(sSystemKeyStore != NULL);
            sLoadedSystemKeyStore = TRUE;
        }
        if (NULL == sSystemTrustAnchorCache) {
            sSystemTrustAnchorCache = new HashSet< Pair< AutoPtr<IX500Principal>, AutoPtr<IPublicKey> > >();
        }
        if (NULL == sTestKeyStore) {
        	AutoPtr<IKeyStoreHelper> keyStore;
        	CKeyStoreHelper::AcquireSingleton((IKeyStoreHelper**)&keyStore);
        	Int32 type;
        	keyStore->GetDefaultType(&type);
            keyStore->GetInstance(type, (IKeyStore**)&sTestKeyStore);

            //try {
                sTestKeyStore->Load(NULL);
            //} catch (IOException e) {
                // No IO operation is attempted.
            //}
        }
        if (sTestTrustManager == NULL) {
            sTestTrustManager = X509Util::CreateTrustManager(sTestKeyStore);
        }
        if (!sDisableNativeCodeForTest && sTrustStorageListener == NULL) {
            sTrustStorageListener = new TrustStorageListener();
            NativeGetApplicationContext()->RegisterReceiver(sTrustStorageListener,
                    new IntentFilter(IKeyChain::ACTION_STORAGE_CHANGED));
        }
    }

    return NOERROR;
}

/**
 * Creates a X509TrustManagerImplementation backed up by the given key
 * store. When null is passed as a key store, system default trust store is
 * used. Returns null if no created TrustManager was suitable.
 * @throws KeyStoreException, NoSuchAlgorithmException on error initializing the TrustManager.
 */
AutoPtr<X509TrustManagerImplementation> X509Util::CreateTrustManager(
    /* [in] */ IKeyStore* keyStore)
{

	AutoPtr<IKeyManagerFactoryHelper> helper;
	CKeyManagerFactoryHelper::AcquireSingleton((IKeyManagerFactoryHelper**)&helper);
	String algorithm;
	helper->GetDefaultAlgorithm(&algorithm);

    AutoPtr<ITrustManagerFactory> tmf;
    helper->GetInstance(algorithm, (ITrustManagerFactory**)&&tmf);
    tmf->Init(keyStore);

    AutoPtr< ArrayOf< AutoPtr<ITrustManager> > > managers;
    tmf->GetTrustManagers(&managers);

    AutoPtr<X509TrustManagerImplementation> ret = NULL;
    AutoPtr<ITrustManager> tm = NULL;
    AutoPtr<IX509TrustManager> x509tm = NULL;
    for (Int32 i=0; i<managers->GetLenght(); ++i) {
    	tm = (*managers)[i];
    	x509tm = (IX509TrustManager**)tm->Probe(EIID_IX509TrustManager);
    	if (NULL != x509tm) {
            //try {
                if (IBuild::VERSION::SDK_INT >= IBuild::VERSION_CODES::JELLY_BEAN_MR1) {
                	ret = new X509TrustManagerJellyBean((X509TrustManager*)x509tm);
                    return ret;
                }
                else {
                	ret = new X509TrustManagerIceCreamSandwich((X509TrustManager*)x509tm);
                    return ret;
                }
            //} catch (IllegalArgumentException e) {
            //    String className = tm.getClass().getName();
            //    Log.e(TAG, "Error creating trust manager (" + className + "): " + e);
            //}
        }
    }

    //Log.e(TAG, "Could not find suitable trust manager");
    return ret;
}

/**
 * After each modification of test key store, trust manager has to be generated again.
 */
ECode X509Util::ReloadTestTrustManager()
{
    sTestTrustManager = X509Util::CreateTrustManager(sTestKeyStore);
    return NOERROR;
}

/**
 * After each modification by the system of the key store, trust manager has to be regenerated.
 */
ECode X509Util::ReloadDefaultTrustManager()
{
    sDefaultTrustManager = NULL;
    sSystemTrustAnchorCache = NULL;
    NativeNotifyKeyChainChanged();
    EnsureInitialized();
    return NOERROR;
}

String X509Util::HashPrincipal(
    /* [in] */ IX500Principal* principal)
{
    // Android hashes a principal as the first four bytes of its MD5 digest, encoded in
    // lowercase hex and reversed. Verified in 4.2, 4.3, and 4.4.
    AutoPtr<IMessageDigestHelper> helper;
	CMessageDigestHelper::AcquireSingleton((IMessageDigestHelper**)&helper);
    AutoPtr<IMessageDigest> msgDigest;
    helper->GetInstance(String("MD5"), (IMessageDigest**)&&msgDigest);
    String encoded;
	principal->GetEncoded(&encoded);
	AutoPtr< ArrayOf<Byte> > digest;
    msgDigest->Digest(encoded, &digest);

    Char16 dig[8];
    for (Int32 i = 0; i < 4; ++i) {
        dig[2 * i] = HEX_DIGITS[(digest[3 - i] >> 4) & 0xf];
        dig[2 * i + 1] = HEX_DIGITS[digest[3 - i] & 0xf];
    }
    return String(dig);
}

Boolean X509Util::IsKnownRoot(
    /* [in] */ IX509Certificate* root)
{
    // Could not find the system key store. Conservatively report false.
    if (sSystemKeyStore == NULL)
        return FALSE;

    // Check the in-memory cache first; avoid decoding the anchor from disk
    // if it has been seen before.
	AutoPtr<IX500Principal> principal;
	CX500Principal::New((IX500Principal**)&principal);
	root->GetSubjectX500Principal((IX500Principal**)&principal);

	AutoPtr<IPublicKey> publicKey;
	CPublicKey::New((IPublicKey**)&publicKey);
	root->GetPublicKey((IPublicKey**)&publicKey);

    Pair< AutoPtr<IX500Principal>, AutoPtr<IPublicKey> > key =
        new Pair< AutoPtr<IX500Principal>, AutoPtr<IPublicKey> >(principal, publicKey);
    if (sSystemTrustAnchorCache->Contains(key))
        return TRUE;

    // Note: It is not sufficient to call sSystemKeyStore.getCertificiateAlias. If the server
    // supplies a copy of a trust anchor, X509TrustManagerExtensions returns the server's
    // version rather than the system one. getCertificiateAlias will then fail to find an anchor
    // name. This is fixed upstream in https://android-review.googlesource.com/#/c/91605/
    //
    // TODO(davidben): When the change trickles into an Android release, query sSystemKeyStore
    // directly.

    // System trust anchors are stored under a hash of the principal. In case of collisions,
    // a number is appended.
    String hash = HashPrincipal(principal);
    AutoPtr<IX509Certificate> x509Cert = NULL;
    for (Int32 i = 0; TRUE; ++i) {
        String alias = hash + String('.') + Char32(i);

		AutoPtr<IFileHelper> helper;
		CFileHelper::AcquireSingleton((IFileHelper**)&helper);

		AutoPtr<IFile> temp;
		helper->CreateTempFile(sSystemCertificateDirectory, alias, (IFile**)&temp);
        if (!temp)
            break;

		AutoPtr<ICertificate> anchor;
		sSystemKeyStore->GetCertificate(String("system:") + alias, (ICertificate**)&anchor);
		// It is possible for this to return null if the user deleted a trust anchor. In
        // that case, the certificate remains in the system directory but is also added to
        // another file. Continue iterating as there may be further collisions after the
        // deleted anchor.
        if (anchor == NULL)
            continue;

		x509Cert = (IX509Certificate*)anchor->Probe(EIID_IX509Certificate);
        if (NULL != x509Cert) {
            // This should never happen.
            String className;
            anchor->GetClassName(&className);
            //Log.e(TAG, "Anchor " + alias + " not an X509Certificate: " + className);
            continue;
        }

        // If the subject and public key match, this is a system root.
        AutoPtr<IX500Principal> anchorx509Principal;
		x509Cert->GetSubjectX500Principal((IX500Principal**)&anchorx509Principal);

		AutoPtr<IPublicKey> anchorx509PublicKey;
		x509Cert->GetPublicKey((IPublicKey**)&anchorx509PublicKey);

	    if (principal->Equal(anchorx509Principal) && publicKey->Equals(anchorx509PublicKey)) {
            sSystemTrustAnchorCache->Add(key);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * If an EKU extension is present in the end-entity certificate, it MUST contain either the
 * anyEKU or serverAuth or netscapeSGC or Microsoft SGC EKUs.
 *
 * @return true if there is no EKU extension or if any of the EKU extensions is one of the valid
 * OIDs for web server certificates.
 *
 * TODO(palmer): This can be removed after the equivalent change is made to the Android default
 * TrustManager and that change is shipped to a large majority of Android users.
 */
Boolean X509Util::VerifyKeyUsage(
    /* [in] */ IX509Certificate* certificate)
{
    List<String> ekuOids;
    //try {
        certificate->GetExtendedKeyUsage(&ekuOids);
    //} catch (NullPointerException e) {
        // getExtendedKeyUsage() can crash due to an Android platform bug. This probably
        // happens when the EKU extension data is malformed so return false here.
        // See http://crbug.com/233610
    //    return false;
    //}
    if (ekuOids->IsEmpty())
        return TRUE;

	String ekuOid;
	for (Int32 i=0; i<ekuOids->GetLength(); ++i) {
		ekuOid = ekuOids[i];
		if (ekuOid.Equals(OID_TLS_SERVER_AUTH) ||
            ekuOid.Equals(OID_ANY_EKU) ||
            ekuOid.Equals(OID_SERVER_GATED_NETSCAPE) ||
            ekuOid.Equals(OID_SERVER_GATED_MICROSOFT)) {
            return TRUE;
        }
	}

    return FALSE;
}

/**
 * Notify the native net::CertDatabase instance that the system database has been updated.
 */
//private static native void nativeNotifyKeyChainChanged();
ECode X509Util::NativeNotifyKeyChainChanged()
{
	return NOERROR;
}

/**
 * Record histograms on the platform's certificate verification capabilities.
 */
//private static native void nativeRecordCertVerifyCapabilitiesHistogram(boolean foundSystemTrustRoots);
ECode X509Util::NativeRecordCertVerifyCapabilitiesHistogram(
	/* [in] */ Boolean foundSystemTrustRoots)
{
	return NOERROR;
}

/**
 * Returns the application context.
 */
//private static native Context nativeGetApplicationContext();
AutoPtr<IContext> X509Util::NativeGetApplicationContext()
{
	AutoPtr<IContext> context;
	return context;
}

AutoPtr< ArrayOf<Char16> > X509Util::InitHexDigits()
{
	Char16 tmps[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    	'a', 'b', 'c', 'd', 'e', 'f'
    };

    Int32 count = sizeof(tmps) / sizeof(tmps[0]);
    AutoPtr< ArrayOf<Char16> > result = ArrayOf<Char16>::Alloc(count);
    for (Int32 i=0; i<count; ++i) {
    	result->Set(i, tmps[i]);
    }

    return result;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
