
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
// 				RemoteAndroidKeyStore::RemotePrivateKey
//===============================================================
Int32 RemoteAndroidKeyStore::RemotePrivateKey::GetHandle()
{
    return mHandle;
}

AutoPtr<AndroidKeyStore> RemoteAndroidKeyStore::RemotePrivateKey::GetKeyStore()
{
    return mStore;
}

RemoteAndroidKeyStore::RemotePrivateKey::RemotePrivateKey(
    /* [in] */ Int32 handle,
    /* [in] */ RemoteAndroidKeyStore* store)
    : mHandle(handle)
    , mStore(store)
{
}

//===============================================================
// 					RemoteAndroidKeyStore
//===============================================================
const RemoteAndroidKeyStore::String TAG("AndroidKeyStoreRemoteImpl");

RemoteAndroidKeyStore::RemoteAndroidKeyStore(
    /* [in] */ RemoteAndroidKeyStore* manager)
    : mRemoteManager(manager)
{
}

//@Override
AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetRSAKeyModulus(
    /* [in] */ AndroidPrivateKey* key)
{
	RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getRSAKeyModulus");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetRSAKeyModulus(remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

//@Override
AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetDSAKeyParamQ(
    /* [in] */ AndroidPrivateKey* key)
{
    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getDSAKeyParamQ");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetDSAKeyParamQ(remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

//@Override
AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetECKeyOrder(
    /* [in] */ AndroidPrivateKey* key)
{
    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getECKeyOrder");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetECKeyOrder(remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

//@Override
AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::RawSignDigestWithPrivateKey(
    /* [in] */ AndroidPrivateKey* key,
    /* [in] */ ArrayOf<Byte>* messages)
{
    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "rawSignDigestWithPrivateKey");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->RawSignDigestWithPrivateKey(remoteKey->GetHandle(), message);
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

//@Override
Int32 RemoteAndroidKeyStore::GetPrivateKeyType(
    /* [in] */ AndroidPrivateKey* key)
{
    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getPrivateKeyType");
        return mRemoteManager->GetPrivateKeyType(remoteKey->GetHandle());
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return 0;
    //}
}

//@Override
AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetPrivateKeyEncodedBytes(
    /* [in] */ AndroidPrivateKey* key)
{
    // This should not be called as it's only for older versions of Android.
    assert (FALSE);
    return NULL;
}

//@Override
Int64 RemoteAndroidKeyStore::GetOpenSSLHandleForPrivateKey(
    /* [in] */ AndroidPrivateKey* privateKey)
{
    // This should not be called as it's only for older versions of Android.
    assert (FALSE);
    return 0;
}

AutoPtr<AndroidPrivateKey> RemoteAndroidKeyStore::CreateKey(
    /* [in] */ String alias)
{
    //try {
        Int32 handle = mRemoteManager->GetPrivateKeyHandle(alias);
        AutoPtr<AndroidPrivateKey> ret = new RemotePrivateKey(handle, this);
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

//@Override
ECode RemoteAndroidKeyStore::ReleaseKey(
    /* [in] */ AndroidPrivateKey* key)
{
	VALIDATE_NOT_NULL(key);
    RemotePrivateKey* remoteKey = (RemotePrivateKey*) key;
    //try {
        //Log.d(TAG, "releaseKey");
        mRemoteManager->ReleaseKey(remoteKey->GetHandle());
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //}
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
