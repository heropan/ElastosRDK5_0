
#include "elastos/droid/webkit/native/net/RemoteElastosKeyStore.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//               RemoteElastosKeyStore::RemotePrivateKey
//=====================================================================
RemoteElastosKeyStore::RemotePrivateKey::RemotePrivateKey(
    /* [in] */ Int32 handle,
    /* [in] */ RemoteElastosKeyStore* store)
    : mHandle(handle)
    , mStore(store)
{
    // ==================before translated======================
    // mHandle = handle;
    // mStore = store;
}

Int32 RemoteElastosKeyStore::RemotePrivateKey::GetHandle()
{
    // ==================before translated======================
    // return mHandle;

    return mHandle;
}

AutoPtr<ElastosKeyStore> RemoteElastosKeyStore::RemotePrivateKey::GetKeyStore()
{
    // ==================before translated======================
    // return mStore;

    return mStore;
}

//=====================================================================
//                        RemoteElastosKeyStore
//=====================================================================
const String RemoteElastosKeyStore::TAG("ElastosKeyStoreRemoteImpl");

RemoteElastosKeyStore::RemoteElastosKeyStore(
    /* [in] */ RemoteElastosKeyStore* manager)
    : mRemoteManager(manager)
{
    // ==================before translated======================
    // mRemoteManager = manager;
}

AutoPtr< ArrayOf<Byte> > RemoteElastosKeyStore::GetRSAKeyModulus(
    /* [in] */ ElastosPrivateKey* key)
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
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetRSAKeyModulus((ElastosPrivateKey*)remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteElastosKeyStore::GetDSAKeyParamQ(
    /* [in] */ ElastosPrivateKey* key)
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
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetDSAKeyParamQ((ElastosPrivateKey*)remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteElastosKeyStore::GetECKeyOrder(
    /* [in] */ ElastosPrivateKey* key)
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
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->GetECKeyOrder((ElastosPrivateKey*)remoteKey->GetHandle());
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteElastosKeyStore::RawSignDigestWithPrivateKey(
    /* [in] */ ElastosPrivateKey* key,
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
        AutoPtr< ArrayOf<Byte> > ret = mRemoteManager->RawSignDigestWithPrivateKey((ElastosPrivateKey*)remoteKey->GetHandle(), message);
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

Int32 RemoteElastosKeyStore::GetPrivateKeyType(
    /* [in] */ ElastosPrivateKey* key)
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
        return mRemoteManager->GetPrivateKeyType((ElastosPrivateKey*)remoteKey->GetHandle());
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return 0;
    //}
}

AutoPtr< ArrayOf<Byte> > RemoteElastosKeyStore::GetPrivateKeyEncodedBytes(
    /* [in] */ ElastosPrivateKey* key)
{
    // ==================before translated======================
    // // This should not be called as it's only for older versions of Android.
    // assert false;
    // return null;

    // This should not be called as it's only for older versions of Android.
    assert (FALSE);
    return NULL;
}

Int64 RemoteElastosKeyStore::GetOpenSSLHandleForPrivateKey(
    /* [in] */ ElastosPrivateKey* privateKey)
{
    // ==================before translated======================
    // // This should not be called as it's only for older versions of Android.
    // assert false;
    // return 0;

    // This should not be called as it's only for older versions of Android.
    assert (FALSE);
    return 0;
}

AutoPtr<ElastosPrivateKey> RemoteElastosKeyStore::CreateKey(
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
        AutoPtr<ElastosPrivateKey> ret = new RemotePrivateKey(handle, this);
        return ret;
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //    return null;
    //}
}

ECode RemoteElastosKeyStore::ReleaseKey(
    /* [in] */ ElastosPrivateKey* key)
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
        mRemoteManager->ReleaseKey((ElastosPrivateKey*)remoteKey->GetHandle());
    //} catch (RemoteException e) {
    //    e.printStackTrace();
    //}
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


