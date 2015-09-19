// wuweizuo automatic build .cpp file from .java file.

#include "RemoteAndroidKeyStore.h"

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
{
    // ==================before translated======================
    // mHandle = handle;
    // mStore = store;
}

Int32 RemoteAndroidKeyStore::RemotePrivateKey::GetHandle()
{
    // ==================before translated======================
    // return mHandle;
    assert(0);
    return 0;
}

AutoPtr<AndroidKeyStore> RemoteAndroidKeyStore::RemotePrivateKey::GetKeyStore()
{
    // ==================before translated======================
    // return mStore;
    assert(0);
    AutoPtr<AndroidKeyStore> empty;
    return empty;
}

//=====================================================================
//                        RemoteAndroidKeyStore
//=====================================================================
const String RemoteAndroidKeyStore::TAG("AndroidKeyStoreRemoteImpl");

RemoteAndroidKeyStore::RemoteAndroidKeyStore(
    /* [in] */ IRemoteAndroidKeyStore* manager)
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
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
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
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
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
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
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
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
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
    assert(0);
    return 0;
}

AutoPtr< ArrayOf<Byte> > RemoteAndroidKeyStore::GetPrivateKeyEncodedBytes(
    /* [in] */ AndroidPrivateKey* key)
{
    // ==================before translated======================
    // // This should not be called as it's only for older versions of Android.
    // assert false;
    // return null;
    assert(0);
    AutoPtr< ArrayOf<Byte> > empty;
    return empty;
}

Int64 RemoteAndroidKeyStore::GetOpenSSLHandleForPrivateKey(
    /* [in] */ AndroidPrivateKey* privateKey)
{
    // ==================before translated======================
    // // This should not be called as it's only for older versions of Android.
    // assert false;
    // return 0;
    assert(0);
    return 0;
}

AutoPtr<AndroidPrivateKey> RemoteAndroidKeyStore::CreateKey(
    /* [in] */ String alias)
{
    // ==================before translated======================
    // try {
    //     int handle = mRemoteManager.getPrivateKeyHandle(alias);
    //     return new RemotePrivateKey(handle, this);
    // } catch (RemoteException e) {
    //     e.printStackTrace();
    //     return null;
    // }
    assert(0);
    AutoPtr<AndroidPrivateKey> empty;
    return empty;
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
    assert(0);
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


