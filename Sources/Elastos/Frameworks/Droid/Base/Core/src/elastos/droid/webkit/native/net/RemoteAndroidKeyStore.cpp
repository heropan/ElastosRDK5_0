
#include "elastos/droid/webkit/native/net/RemoteAndroidKeyStore.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//               RemoteAndroidKeyStore::RemotePrivateKey
//=====================================================================
RemoteAndroidKeyStore::RemotePrivateKey::RemotePrivateKey(
    /* [in] */ Int32 handle,
    /* [in] */ RemoteAndroidKeyStore* store)
    : mHandle(handle)
    , mStore(store)
{
    // ==================before translated======================
    // mHandle = handle;
    // mStore = store;
}

Int32 RemoteAndroidKeyStore::RemotePrivateKey::GetHandle()
{
    // ==================before translated======================
    // return mHandle;

    return mHandle;
}

AutoPtr<AndroidKeyStore> RemoteAndroidKeyStore::RemotePrivateKey::GetKeyStore()
{
    // ==================before translated======================
    // return mStore;

    return mStore;
}

//=====================================================================
//                        RemoteAndroidKeyStore
//=====================================================================
const String RemoteAndroidKeyStore::TAG("AndroidKeyStoreRemoteImpl");

RemoteAndroidKeyStore::RemoteAndroidKeyStore(
    /* [in] */ RemoteAndroidKeyStore* manager)
    : mRemoteManager(manager)
{
    // ==================before translated======================
    // mRemoteManager = manager;
}

AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetRSAKeyModulus(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // RemotePrivateKey remoteKey = (RemotePrivateKey) key;
    // try {
    //     Log.d(TAG, "getRSAKeyModulus");
    //     return mRemoteManager.getRSAKeyModulus(remoteKey.getHandle());
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return null;
    // }

    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getRSAKeyModulus");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetRSAKeyModulus((AndroidPrivateKey*)remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetDSAKeyParamQ(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // RemotePrivateKey remoteKey = (RemotePrivateKey) key;
    // try {
    //     Log.d(TAG, "getDSAKeyParamQ");
    //     return mRemoteManager.getDSAKeyParamQ(remoteKey.getHandle());
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return null;
    // }

    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getDSAKeyParamQ");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetDSAKeyParamQ((AndroidPrivateKey*)remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetECKeyOrder(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // RemotePrivateKey remoteKey = (RemotePrivateKey) key;
    // try {
    //     Log.d(TAG, "getECKeyOrder");
    //     return mRemoteManager.getECKeyOrder(remoteKey.getHandle());
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return null;
    // }

    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getECKeyOrder");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetECKeyOrder((AndroidPrivateKey*)remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::RawSignDigestWithPrivateKey(
    /* [in] */ AndroidPrivateKey* key,
    /* [in] */ ArrayOf<Byte>* message)
{
    // ==================before translated======================
    // RemotePrivateKey remoteKey = (RemotePrivateKey) key;
    // try {
    //     Log.d(TAG, "rawSignDigestWithPrivateKey");
    //     return mRemoteManager.rawSignDigestWithPrivateKey(remoteKey.getHandle(), message);
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return null;
    // }

    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "rawSignDigestWithPrivateKey");
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->RawSignDigestWithPrivateKey((AndroidPrivateKey*)remoteKey->GetHandle(), message);
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

Int32 RemoteAndroidKeyStore::GetPrivateKeyType(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // RemotePrivateKey remoteKey = (RemotePrivateKey) key;
    // try {
    //     Log.d(TAG, "getPrivateKeyType");
    //     return mRemoteManager.getPrivateKeyType(remoteKey.getHandle());
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return 0;
    // }

    RemotePrivateKey* remoteKey = (RemotePrivateKey*)key;
    //try {
        //Log.d(TAG, "getPrivateKeyType");
        return mRemoteManager->GetPrivateKeyType((AndroidPrivateKey*)remoteKey->GetHandle());
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return 0;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetPrivateKeyEncodedBytes(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // // This should not be called as it's only for older versions of Android.
    // assert false;
    // return null;

    // This should not be called as it's only for older versions of Android.
    assert (FALSE);
    return NULL;
}

Int64 RemoteAndroidKeyStore::GetOpenSSLHandleForPrivateKey(
    /* [in] */ AndroidPrivateKey* privateKey)
{
    // ==================before translated======================
    // // This should not be called as it's only for older versions of Android.
    // assert false;
    // return 0;

    // This should not be called as it's only for older versions of Android.
    assert (FALSE);
    return 0;
}

AutoPtr<AndroidPrivateKey> RemoteAndroidKeyStore::CreateKey(
    /* [in] */ const String& alias)
{
    // ==================before translated======================
    // try {
    //     int handle = mRemoteManager.getPrivateKeyHandle(alias);
    //     return new RemotePrivateKey(handle, this);
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return null;
    // }

    assert (0);
    //try {
        /* java file mRemoteManager is belong type IRemoteAndroidKeyStore that suffix is .aidl file */
        Int32 handle;// = mRemoteManager->GetPrivateKeyHandle(alias);
        AutoPtr<AndroidPrivateKey> ret = new RemotePrivateKey(handle, this);
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

ECode RemoteAndroidKeyStore::ReleaseKey(
    /* [in] */ AndroidPrivateKey* key)
{
    VALIDATE_NOT_NULL(key);
    // ==================before translated======================
    // RemotePrivateKey remoteKey = (RemotePrivateKey) key;
    // try {
    //     Log.d(TAG, "releaseKey");
    //     mRemoteManager.releaseKey(remoteKey.getHandle());
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    // }

    RemotePrivateKey* remoteKey = (RemotePrivateKey*) key;
    //try {
        //Log.d(TAG, "releaseKey");
        mRemoteManager->ReleaseKey((AndroidPrivateKey*)remoteKey->GetHandle());
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //}
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


