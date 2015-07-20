#ifndef __ELASTOS_DROID_SERVER_PM_INSTALLER_H__
#define __ELASTOS_DROID_SERVER_PM_INSTALLER_H__

#include "Elastos.Droid.Server_server.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

using Elastos::Droid::Content::Pm::IPackageStats;
using Elastos::Droid::Net::ILocalSocket;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class Installer : public ElRefBase
{
public:
    Installer();

    CARAPI_(Int32) Install(
        /* [in] */ const String& name,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI_(Int32) Dexopt(
        /* [in] */ const String& apkPath,
        /* [in] */ Int32 uid,
        /* [in] */ Boolean isPublic);

    CARAPI_(Int32) Movedex(
        /* [in] */ const String& srcPath,
        /* [in] */ const String& dstPath);

    CARAPI_(Int32) Rmdex(
        /* [in] */ const String& codePath);

    CARAPI_(Int32) Remove(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) Rename(
        /* [in] */ const String& oldname,
        /* [in] */ const String& newname);

    CARAPI_(Int32) FixUid(
        /* [in] */ const String& name,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI_(Int32) DeleteCacheFiles(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) CreateUserData(
        /* [in] */ const String& name,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) RemoveUserDataDirs(
        /* [in] */ Int32 userId);

    CARAPI_(Int32) ClearUserData(
        /* [in] */ const String& name,
        /* [in] */ Int32 userId);

    CARAPI_(Int32) CloneUserData(
        /* [in] */ Int32 srcUserId,
        /* [in] */ Int32 targetUserId,
        /* [in] */ Boolean copyData);

    CARAPI_(Boolean) Ping();

    CARAPI_(Int32) FreeCache(
        /* [in] */ Int64 freeStorageSize);

    CARAPI_(Int32) GetSizeInfo(
        /* [in] */ const String& pkgName,
        /* [in] */ Int32 persona,
        /* [in] */ const String& apkPath,
        /* [in] */ const String& fwdLockApkPath,
        /* [in] */ const String& asecPath,
        /* [in] */ IPackageStats* pStats);

    CARAPI_(Int32) MoveFiles();

    CARAPI_(Int32) LinkNativeLibraryDirectory(
        /* [in] */ const String& dataPath,
        /* [in] */ const String& nativeLibPath,
        /* [in] */ Int32 userId);

private:
    CARAPI_(Boolean) Connect();

    CARAPI_(void) Disconnect();

    CARAPI_(Boolean) ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 len);

    CARAPI_(Boolean) ReadReply();

    CARAPI_(Boolean) WriteCommand(
        /* [in] */ const String& cmd);

    CARAPI_(String) Transaction(
        /* [in] */ const String& cmd);

    CARAPI_(Int32) Execute(
        /* [in] */ const String& cmd);

public:
    AutoPtr<IInputStream> mIn;

    AutoPtr<IOutputStream> mOut;

    AutoPtr<ILocalSocket> mSocket;

    AutoPtr< ArrayOf<Byte> > mBuf;

    Int32 mBuflen;

private:
    static const String TAG;

    static const Boolean LOCAL_DEBUG = FALSE;

    Object mLock;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_PM_INSTALLER_H__
