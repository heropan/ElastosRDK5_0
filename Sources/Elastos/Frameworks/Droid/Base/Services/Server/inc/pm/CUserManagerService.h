#ifndef __CUSERMANAGERSERVICE_H__
#define __CUSERMANAGERSERVICE_H__

#include "ext/frameworkext.h"
#include "_CUserManagerService.h"
#include "content/BroadcastReceiver.h"
#include <elastos/ThreadBase.h>
#include <elastos/HashMap.h>

using Elastos::Utility::HashMap;
using Elastos::IO::IFile;
using Org::Xmlpull::V1::IXmlPullParser;

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class CPackageManagerService;

CarClass(CUserManagerService)
{
public:
    class FinishThread
        : public ThreadBase
    {
    public:
        FinishThread(
            /* [in] */ CUserManagerService* host,
            /* [in] */ Int32 userHandle)
            : mHost(host)
            , mUserHandle(userHandle)
        {}

        CARAPI Run();

    private:
        CUserManagerService* mHost;
        Int32 mUserHandle;
    };

    class FinishBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        FinishBroadcastReceiver(
            /* [in] */ CUserManagerService* host,
            /* [in] */ Int32 userHandle)
            : mHost(host)
            , mUserHandle(userHandle)
        {}

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CUserManagerService::FinishBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CUserManagerService* mHost;
        Int32 mUserHandle;
    };

    class RemoveRunnable
        : public ElRefBase
        , public IRunnable
    {
    public:
        RemoveRunnable(
            /* [in] */ CUserManagerService* host,
            /* [in] */ Int32 userHandle)
            : mHost(host)
            , mUserHandle(userHandle)
        {}

        CAR_INTERFACE_DECL()

        CARAPI Run();

    private:
        CUserManagerService* mHost;
        Int32 mUserHandle;
    };

public:
    CUserManagerService();

    ~CUserManagerService();

    CARAPI constructor(
        /* [in] */ IFile* dataDir,
        /* [in] */ IFile* baseUserPath);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Handle32 pm,
        /* [in] */ Handle32 installLock,
        /* [in] */ Handle32 packagesLock);

    static CARAPI_(AutoPtr<CUserManagerService>) GetInstance();

    // @Override
    CARAPI GetUsers(
        /* [in] */ Boolean excludeDying,
        /* [out] */ IObjectContainer** users);

    // @Override
    CARAPI GetUserInfo(
        /* [in] */ Int32 userId,
        /* [out] */ IUserInfo** userInfo);

    CARAPI_(Boolean) Exists(
        /* [in] */ Int32 userId);

    // @Override
    CARAPI SetUserName(
        /* [in] */ Int32 userId,
        /* [in] */ const String& name);

    // @Override
    CARAPI SetUserIcon(
        /* [in] */ Int32 userId,
        /* [in] */ IBitmap* bitmap);

    // @Override
    CARAPI GetUserIcon(
        /* [in] */ Int32 userId,
        /* [out] */ IBitmap** userIcon);

    // @Override
    CARAPI SetGuestEnabled(
        /* [in] */ Boolean enable);

    // @Override
    CARAPI IsGuestEnabled(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI WipeUser(
        /* [in] */ Int32 userHandle);

    CARAPI MakeInitialized(
        /* [in] */ Int32 userId);

    CARAPI_(AutoPtr< ArrayOf<Int32> >) GetUserIds();

    CARAPI_(AutoPtr< ArrayOf<Int32> >) GetUserIdsLPr();

    // @Override
    CARAPI CreateUser(
        /* [in] */ const String& name,
        /* [in] */ Int32 flags,
        /* [out] */ IUserInfo** userInfo);

    CARAPI RemoveUser(
        /* [in] */ Int32 userHandle,
        /* [out] */ Boolean* succeeded);

    CARAPI_(void) FinishRemoveUser(
        /* [in] */ Int32 userHandle);

    // @Override
    CARAPI GetUserSerialNumber(
        /* [in] */ Int32 userHandle,
        /* [out] */ Int32* serialNo);

    // @Override
    CARAPI GetUserHandle(
        /* [in] */ Int32 userSerialNumber,
        /* [out] */ Int32* userHandle);

    CARAPI_(void) UserForeground(
        /* [in] */ Int32 userId);

protected:
    // @Override
    // protected void dump(FileDescriptor fd, PrintWriter pw, String[] args);

private:
    // CUserManagerService(Context context, PackageManagerService pm,
    //         Object installLock, Object packagesLock,
    //         File dataDir, File baseUserPath);
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ CPackageManagerService* pm,
        /* [in] */ Mutex* installLock,
        /* [in] */ Mutex* packagesLock,
        /* [in] */ IFile* dataDir,
        /* [in] */ IFile* baseUserPath);

    CARAPI_(AutoPtr<IUserInfo>) GetUserInfoLocked(
        /* [in] */ Int32 userId);

    CARAPI_(void) SendUserInfoChangedBroadcast(
        /* [in] */ Int32 userId);

    CARAPI_(Boolean) IsUserLimitReachedLocked();

    static CARAPI CheckManageUsersPermission(
        /* [in] */ const String& message);

    CARAPI_(void) WriteBitmapLocked(
        /* [in] */ IUserInfo* info,
        /* [in] */ IBitmap* bitmap);

    CARAPI_(void) ReadUserList();

    CARAPI_(void) ReadUserListLocked();

    CARAPI_(void) UpgradeIfNecessary();

    CARAPI_(void) FallbackToSingleUserLocked();

    CARAPI_(void) WriteUserLocked(
        /* [in] */ IUserInfo* userInfo);

    CARAPI_(void) WriteUserListLocked();

    CARAPI_(AutoPtr<IUserInfo>) ReadUser(
        /* [in] */ Int32 id);

    CARAPI_(Int32) ReadInt32Attribute(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& attr,
        /* [in] */ Int32 defaultValue);

    CARAPI_(Int64) ReadInt64Attribute(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ const String& attr,
        /* [in] */ Int64 defaultValue);

    CARAPI_(void) RemoveUserStateLocked(
        /* [in] */ Int32 userHandle);

    CARAPI_(void) RemoveDirectoryRecursive(
        /* [in] */ IFile* parent);

    CARAPI_(void) UpdateUserIdsLocked();

    CARAPI_(Int32) GetNextAvailableIdLocked();

private:
    static const String TAG;

    static const Boolean DBG = FALSE;

    static const String TAG_NAME;
    static const String ATTR_FLAGS;
    static const String ATTR_ICON_PATH;
    static const String ATTR_ID;
    static const String ATTR_CREATION_TIME;
    static const String ATTR_LAST_LOGGED_IN_TIME;
    static const String ATTR_SERIAL_NO;
    static const String ATTR_NEXT_SERIAL_NO;
    static const String ATTR_PARTIAL;
    static const String ATTR_USER_VERSION;
    static const String TAG_USERS;
    static const String TAG_USER;

    static const String USER_INFO_DIR;
    static const String USER_LIST_FILENAME;
    static const String USER_PHOTO_FILENAME;

    static const Int32 MIN_USER_ID = 10;

    static const Int32 USER_VERSION = 2;

    static const Int64 EPOCH_PLUS_30_YEARS = 30LL * 365 * 24 * 60 * 60 * 1000LL; // ms

    AutoPtr<IContext> mContext;
    CPackageManagerService* mPm;
    Mutex* mInstallLock;
    Mutex* mPackagesLock;
    Boolean mDeleteLock;

    AutoPtr<IHandler> mHandler;

    AutoPtr<IFile> mUsersDir;
    AutoPtr<IFile> mUserListFile;
    AutoPtr<IFile> mBaseUserPath;

    HashMap<Int32, AutoPtr<IUserInfo> > mUsers;

    /**
     * Set of user IDs being actively removed. Removed IDs linger in this set
     * for several seconds to work around a VFS caching issue.
     */
    // @GuardedBy("mPackagesLock")
    HashMap<Int32, Boolean> mRemovingUserIds;// = new SparseBooleanArray();

    AutoPtr< ArrayOf<Int32> > mUserIds;
    Boolean mGuestEnabled;
    Int32 mNextSerialNumber;

    Int32 mUserVersion;

    static AutoPtr<CUserManagerService> sInstance;
    static Mutex sLock;
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CUSERMANAGERSERVICE_H__
