
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Crypto {

//==================================================================
//                  CipherFactory::CipherData
//==================================================================

CipherFactory::CipherData::CipherData(
    /* [in] */ IKey* _key,
    /* [in] */ ArrayOf<Byte>* _iv)
    : key(_key)
    , iv(_iv)
{
}

//==================================================================
//                  CipherFactory::LazyHolder
//==================================================================

AutoPtr<CipherFactory> CipherFactory::LazyHolder::sInstance = new CipherFactory();

//==================================================================
//                        CipherFactory
//==================================================================

const String CipherFactory::TAG("CipherFactory");
const Int32 CipherFactory::NUM_BYTES;

const String CipherFactory::BUNDLE_IV("org.chromium.content.browser.crypto.CipherFactory.IV");
const String CipherFactory::BUNDLE_KEY("org.chromium.content.browser.crypto.CipherFactory.KEY");

CipherFactory::CipherFactory()
{
    mRandomNumberProvider = new ByteArrayGenerator();
}

/** @return The Singleton instance. Creates it if it doesn't exist. */
AutoPtr<CipherFactory> CipherFactory::GetInstance()
{
    return LazyHolder::sInstance;
}

/**
 * Creates a secure Cipher for encrypting data.
 * This function blocks until data needed to generate a Cipher has been created by the
 * background thread.
 * @param opmode One of Cipher.{ENCRYPT,DECRYPT}_MODE.
 * @return A Cipher, or null if it is not possible to instantiate one.
 */
AutoPtr<ICipher> CipherFactory::GetCipher(
    /* [in] */ Int32 opmode)
{
    AutoPtr<CipherData> data = GetCipherData(TRUE);

    if (data != null) {
        try {
            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            cipher.init(opmode, data.key, new IvParameterSpec(data.iv));
            return cipher;
        } catch (GeneralSecurityException e) {
            // Can't do anything here.
        }
    }

    Log.e(TAG, "Error in creating cipher instance.");
    return null;
}

/**
 * Returns data required for generating the Cipher.
 * @param generateIfNeeded Generates data on the background thread, blocking until it is done.
 * @return Data to use for the Cipher, null if it couldn't be generated.
 */
AutoPtr<CipherData> CipherFactory::GetCipherData(
    /* [in] */ Boolean generateIfNeeded)
{
    if (mData == NULL && generateIfNeeded) {
        // Ideally, this task should have been started way before this.
        triggerKeyGeneration();

        // Grab the data from the task.
        CipherData data;
        try {
            data = mDataGenerator.get();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } catch (ExecutionException e) {
            throw new RuntimeException(e);
        }

        // Only the first thread is allowed to save the data.
        synchronized (mDataLock) {
            if (mData == null) mData = data;
        }
    }
    return mData;
}

/**
 * Creates a Callable that generates the data required to create a Cipher. This is done on a
 * background thread to prevent blocking on the I/O required for
 * {@link ByteArrayGenerator#getBytes(int)}.
 * @return Callable that generates the Cipher data.
 */
Callable<CipherData> CipherFactory::CreateGeneratorCallable()
{
    return new Callable<CipherData>() {
        @Override
        public CipherData call() {
            // Poll random data to generate initialization parameters for the Cipher.
            byte[] seed, iv;
            try {
                seed = mRandomNumberProvider.getBytes(NUM_BYTES);
                iv = mRandomNumberProvider.getBytes(NUM_BYTES);
            } catch (Exception e) {
                Log.e(TAG, "Couldn't get generator data.");
                return null;
            }

            try {
                // Old versions of SecureRandom do not seed themselves as securely as possible.
                // This workaround should suffice until the fixed version is deployed to all
                // users. The seed comes from RandomNumberProvider.getBytes(), which reads
                // from /dev/urandom, which is as good as the platform can get.
                //
                // TODO(palmer): Consider getting rid of this once the updated platform has
                // shipped to everyone. Alternately, leave this in as a defense against other
                // bugs in SecureRandom.
                SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
                random.setSeed(seed);

                KeyGenerator generator = KeyGenerator.getInstance("AES");
                generator.init(128, random);
                return new CipherData(generator.generateKey(), iv);
            } catch (GeneralSecurityException e) {
                Log.e(TAG, "Couldn't get generator instances.");
                return null;
            }
        }
    };
}

/**
 * Generates the encryption key and IV on a background thread (if necessary).
 * Should be explicitly called when the Activity determines that it will need a Cipher rather
 * than immediately calling {@link CipherFactory#getCipher(int)}.
 */
void CipherFactory::TriggerKeyGeneration()
{
    if (mData != null) return;

    synchronized (mDataLock) {
        if (mDataGenerator == null) {
            mDataGenerator = new FutureTask<CipherData>(createGeneratorCallable());
            AsyncTask.THREAD_POOL_EXECUTOR.execute(mDataGenerator);
        }
    }
}

/**
 * Saves the encryption data in a bundle. Expected to be called when an Activity saves its state
 * before being sent to the background.
 *
 * The IV *could* go into the first block of the payload. However, since the staleness of the
 * data is determined by whether or not it's able to be decrypted, the IV should not be read
 * from it.
 *
 * @param outState The data bundle to store data into.
 */
void CipherFactory::SaveToBundle(Bundle outState)
{
    CipherData data = getCipherData(false);
    if (data == null) return;

    byte[] wrappedKey = data.key.getEncoded();
    if (wrappedKey != null && data.iv != null) {
        outState.putByteArray(BUNDLE_KEY, wrappedKey);
        outState.putByteArray(BUNDLE_IV, data.iv);
    }
}

/**
 * Restores the encryption key from the given Bundle. Expected to be called when an Activity is
 * being restored after being killed in the background. If the Activity was explicitly killed by
 * the user, Android gives no Bundle (and therefore no key).
 *
 * @param savedInstanceState Bundle containing the Activity's previous state. Null if the user
 *                           explicitly killed the Activity.
 * @return                   True if the data was restored successfully from the Bundle, or if
 *                           the CipherData in use matches the Bundle contents.
 *
 */
Boolean CipherFactory::RestoreFromBundle(
    /* [in] */ IBundle* savedInstanceState)
{
    if (savedInstanceState == NULL) return FALSE;

    AutoPtr< ArrayOf<Byte> > wrappedKey = savedInstanceState->GetByteArray(BUNDLE_KEY);
    AutoPtr< ArrayOf<Byte> > iv = savedInstanceState->GetByteArray(BUNDLE_IV);
    if (wrappedKey == NULL || iv == NULL) return FALSE;

    // try {
        AutoPtr<IKey> bundledKey;
        CSecretKeySpec(wrappedKey, String("AES"), (IKey**)&bundledKey);

        {
            Object::Autolock lock(mDataLock);
            if (mData == NULL) {
                mData = new CipherData(bundledKey, iv);
                return TRUE;
            }
            else if (mData->key->Equals(bundledKey) && Arrays.equals(mData.iv, iv)) {
                return TRUE;
            }
            else {
//                Log.e(TAG, "Attempted to restore different cipher data.");
            }
        }
    // } catch (IllegalArgumentException e) {
    //     Log.e(TAG, "Error in restoring the key from the bundle.");
    // }

    return FALSE;
}

/**
 * Overrides the random number generated that is normally used by the class.
 * @param mockProvider Should be used to provide non-random data.
 */
void CipherFactory::SetRandomNumberProviderForTests(
    /* [in] */ ByteArrayGenerator* mockProvider)
{
    mRandomNumberProvider = mockProvider;
}

} // namespace Crypto
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
