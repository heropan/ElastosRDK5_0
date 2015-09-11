
#include "pm/CUserManagerService.h"
#include "pm/CUserStopUserCallback.h"
#include "pm/CPackageManagerService.h"
#include "app/ActivityManagerNative.h"
#include "os/Binder.h"
#include "os/FileUtils.h"
#include "os/UserHandle.h"
#include "util/Xml.h"
#include "R.h"
#include "Manifest.h"
#include <elastos/utility/etl/List.h>
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::EIID_IRunnable;
using Elastos::Utility::Etl::List;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::CBufferedOutputStream;
using Elastos::Utility::Logging::Slogger;
using Org::Xmlpull::V1::IXmlSerializer;

using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::App::IActivityManagerHelper;
using Elastos::Droid::R;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::CUserInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::CEnvironment;
using Elastos::Droid::Os::FileUtils;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::IUserManagerHelper;
using Elastos::Droid::Os::CUserManagerHelper;
using Elastos::Droid::Text::Format::IDateUtils;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Utility::CFastXmlSerializer;
using Elastos::Droid::Utility::IAtomicFile;
using Elastos::Droid::Utility::IFastXmlSerializer;
using Elastos::Droid::Utility::CAtomicFile;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

ECode CUserManagerService::FinishThread::Run()
{
    AutoLock lock(mHost->mInstallLock);
    {
        AutoLock lock(mHost->mPackagesLock);
        mHost->RemoveUserStateLocked(mUserHandle);
    }
    return NOERROR;
}

ECode CUserManagerService::FinishBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    if (CUserManagerService::DBG) {
        Slogger::I(CUserManagerService::TAG,
                "USER_REMOVED broadcast sent, cleaning up user data %d",
                mUserHandle);
    }
    AutoPtr<FinishThread> thread = new FinishThread(mHost, mUserHandle);
    thread->Start();
    return NOERROR;
}

ECode CUserManagerService::RemoveRunnable::Run()
{
    AutoLock lock(mHost->mPackagesLock);
    mHost->mRemovingUserIds.Erase(mUserHandle);
    return NOERROR;
}

CAR_INTERFACE_IMPL(CUserManagerService::RemoveRunnable, IRunnable)

const String CUserManagerService::TAG("CUserManagerService");

const Boolean CUserManagerService::DBG;

const String CUserManagerService::TAG_NAME = String("name");
const String CUserManagerService::ATTR_FLAGS = String("flags");
const String CUserManagerService::ATTR_ICON_PATH = String("icon");
const String CUserManagerService::ATTR_ID = String("id");
const String CUserManagerService::ATTR_CREATION_TIME = String("created");
const String CUserManagerService::ATTR_LAST_LOGGED_IN_TIME = String("lastLoggedIn");
const String CUserManagerService::ATTR_SERIAL_NO = String("serialNumber");
const String CUserManagerService::ATTR_NEXT_SERIAL_NO = String("nextSerialNumber");
const String CUserManagerService::ATTR_PARTIAL = String("partial");
const String CUserManagerService::ATTR_USER_VERSION = String("version");
const String CUserManagerService::TAG_USERS = String("users");
const String CUserManagerService::TAG_USER = String("user");

const String CUserManagerService::USER_INFO_DIR = String("system/users"); //"system" + File.separator + "users";
const String CUserManagerService::USER_LIST_FILENAME = String("userlist.xml");
const String CUserManagerService::USER_PHOTO_FILENAME = String("photo.png");

const Int32 CUserManagerService::MIN_USER_ID;

const Int32 CUserManagerService::USER_VERSION;

const Int64 CUserManagerService::EPOCH_PLUS_30_YEARS; // ms

AutoPtr<CUserManagerService> CUserManagerService::sInstance;
Mutex CUserManagerService::sLock;

CUserManagerService::CUserManagerService()
    : mPm(NULL)
    , mInstallLock(NULL)
    , mPackagesLock(NULL)
    , mDeleteLock(FALSE)
    , mUsers(7)
    , mRemovingUserIds(7)
    , mGuestEnabled(FALSE)
    , mNextSerialNumber(0)
    , mUserVersion(0)
{}

CUserManagerService::~CUserManagerService()
{
    mPm = NULL;
    if (mDeleteLock) {
        if (mInstallLock != NULL) delete mInstallLock;
        if (mPackagesLock != NULL) delete mPackagesLock;
    }
    mInstallLock = NULL;
    mPackagesLock = NULL;
}

ECode CUserManagerService::constructor(
    /* [in] */ IFile* dataDir,
    /* [in] */ IFile* baseUserPath)
{
    Object* installLock = new Object();
    Object* packagesLock = new Object();
    mDeleteLock = TRUE;
    return Init(NULL, NULL, installLock, packagesLock, dataDir, baseUserPath);
}

ECode CUserManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Handle32 pm,
    /* [in] */ Handle32 installLock,
    /* [in] */ Handle32 packagesLock)
{
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> dataDir;
    env->GetDataDirectory((IFile**)&dataDir);
    AutoPtr<IFile> baseUserPath;
    CFile::New(dataDir, String("user"), (IFile**)&baseUserPath);
    return Init(context, (CPackageManagerService*)pm,
            (Object*)installLock, (Object*)packagesLock, dataDir, baseUserPath);
}

ECode CUserManagerService::Init(
    /* [in] */ IContext* context,
    /* [in] */ CPackageManagerService* pm,
    /* [in] */ Object* installLock,
    /* [in] */ Object* packagesLock,
    /* [in] */ IFile* dataDir,
    /* [in] */ IFile* baseUserPath)
{
    mContext = context;
    mPm = pm;
    mInstallLock = installLock;
    mPackagesLock = packagesLock;
    CHandler::New((IHandler**)&mHandler);
    {
        AutoLock lock(mInstallLock);

        {
            AutoLock lock(mPackagesLock);

            Boolean result;
            CFile::New(dataDir, USER_INFO_DIR, (IFile**)&mUsersDir);
            mUsersDir->Mkdirs(&result);
            // Make zeroth user directory, for services to migrate their files to that location
            AutoPtr<IFile> userZeroDir;
            CFile::New(mUsersDir, String("0"), (IFile**)&userZeroDir);
            userZeroDir->Mkdirs(&result);
            mBaseUserPath = baseUserPath;
            String usersDirStr;
            mUsersDir->ToString(&usersDirStr);

            FileUtils::SetPermissions(usersDirStr,
                    FileUtils::sS_IRWXU | FileUtils::sS_IRWXG
                    | FileUtils::sS_IROTH | FileUtils::sS_IXOTH,
                    -1, -1);

            CFile::New(mUsersDir, USER_LIST_FILENAME, (IFile**)&mUserListFile);
            ReadUserListLocked();
            // Prune out any partially created/partially removed users.
            List< AutoPtr<IUserInfo> > partials;
            HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it;
            for (it = mUsers.Begin(); it != mUsers.End(); ++it) {
                AutoPtr<IUserInfo> ui = it->mSecond;
                Boolean partial;
                ui->GetPartial(&partial);
                if (partial && it != mUsers.Begin()) {
                    partials.PushBack(ui);
                }
            }
            List< AutoPtr<IUserInfo> >::Iterator pit;
            for (pit = partials.Begin(); pit != partials.End(); ++pit) {
                AutoPtr<IUserInfo> ui = *pit;
//                Slog.w(TAG, "Removing partially created user #" + i
//                        + " (name=" + ui.name + ")");
                Int32 id;
                ui->GetId(&id);
                RemoveUserStateLocked(id);
            }
            sInstance = this;
        }
    }
    return NOERROR;
}

AutoPtr<CUserManagerService> CUserManagerService::GetInstance()
{
    AutoLock lock(sLock);

    return sInstance;
}

ECode CUserManagerService::GetUsers(
    /* [in] */ Boolean excludeDying,
    /* [out] */ IObjectContainer** users)
{
    VALIDATE_NOT_NULL(users);
    *users = NULL;

    FAIL_RETURN(CheckManageUsersPermission(String("query users")));
    {
        AutoLock lock(mPackagesLock);

        CParcelableObjectContainer::New(users);
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it;
        for (it = mUsers.Begin(); it != mUsers.End(); ++it) {
            AutoPtr<IUserInfo> ui = it->mSecond;
            Boolean partial;
            ui->GetPartial(&partial);
            if (partial) {
                continue;
            }
            Int32 id;
            ui->GetId(&id);
            if (!excludeDying || mRemovingUserIds.Find(id) == mRemovingUserIds.End()) {
                (*users)->Add(ui);
            }
        }
        return NOERROR;
    }
}

ECode CUserManagerService::GetUserInfo(
    /* [in] */ Int32 userId,
    /* [out] */ IUserInfo** userInfo)
{
    VALIDATE_NOT_NULL(userInfo);
    *userInfo = NULL;

    FAIL_RETURN(CheckManageUsersPermission(String("query user")));
    {
        AutoLock lock(mPackagesLock);

        AutoPtr<IUserInfo> user = GetUserInfoLocked(userId);
        *userInfo = user;
        REFCOUNT_ADD(*userInfo);
        return NOERROR;
    }
}

AutoPtr<IUserInfo> CUserManagerService::GetUserInfoLocked(
    /* [in] */ Int32 userId)
{
    AutoPtr<IUserInfo> ui;
    HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(userId);
    if (it != mUsers.End()) {
        ui = it->mSecond;
    }
    // If it is partial and not in the process of being removed, return as unknown user.
    Boolean partial;
    if (ui != NULL && (ui->GetPartial(&partial), partial) &&
        mRemovingUserIds.Find(userId) == mRemovingUserIds.End()) {
        Slogger::W(TAG, "getUserInfo: unknown user #%d", userId);
        return NULL;
    }
    return ui;
}

Boolean CUserManagerService::Exists(
    /* [in] */ Int32 userId)
{
    AutoLock lock(mPackagesLock);

    for (Int32 i = 0; i < mUserIds->GetLength(); ++i) {
        if ((*mUserIds)[i] == userId) return TRUE;
    }
    return FALSE;
}

ECode CUserManagerService::SetUserName(
    /* [in] */ Int32 userId,
    /* [in] */ const String& name)
{
    FAIL_RETURN(CheckManageUsersPermission(String("rename users")));
    Boolean changed = FALSE;
    {
        AutoLock lock(mPackagesLock);

        AutoPtr<IUserInfo> info;
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(userId);
        if (it != mUsers.End()) {
            info = it->mSecond;
        }
        Boolean partial;
        if (info == NULL || (info->GetPartial(&partial), partial)) {
            Slogger::W(TAG, "setUserName: unknown user #%d", userId);
            return NOERROR;
        }
        String infoName;
        if (!name.IsNull() && (info->GetName(&infoName), !name.Equals(infoName))) {
            info->SetName(name);
            WriteUserLocked(info);
            changed = TRUE;
        }
    }
    if (changed) {
        SendUserInfoChangedBroadcast(userId);
    }
    return NOERROR;
}

ECode CUserManagerService::SetUserIcon(
    /* [in] */ Int32 userId,
    /* [in] */ IBitmap* bitmap)
{
    FAIL_RETURN(CheckManageUsersPermission(String("update users")));
    {
        AutoLock lock(mPackagesLock);

        AutoPtr<IUserInfo> info;
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(userId);
        if (it != mUsers.End()) {
            info = it->mSecond;
        }
        Boolean partial;
        if (info == NULL || (info->GetPartial(&partial), partial)) {
            Slogger::W(TAG, "setUserIcon: unknown user #%d", userId);
            return NOERROR;
        }
        WriteBitmapLocked(info, bitmap);
        WriteUserLocked(info);
    }
    SendUserInfoChangedBroadcast(userId);
    return NOERROR;
}

void CUserManagerService::SendUserInfoChangedBroadcast(
    /* [in] */ Int32 userId)
{
    AutoPtr<IIntent> changedIntent;
    CIntent::New(IIntent::ACTION_USER_INFO_CHANGED, (IIntent**)&changedIntent);
    changedIntent->PutExtra(IIntent::EXTRA_USER_HANDLE, userId);
    changedIntent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY);
    AutoPtr<IUserHandle> uHandle;
    CUserHandle::New(userId, (IUserHandle**)&uHandle);
    mContext->SendBroadcastAsUser(changedIntent, uHandle);
}

ECode CUserManagerService::GetUserIcon(
    /* [in] */ Int32 userId,
    /* [out] */ IBitmap** userIcon)
{
    VALIDATE_NOT_NULL(userIcon);
    *userIcon = NULL;

    FAIL_RETURN(CheckManageUsersPermission(String("read users")));
    {
        AutoLock lock(mPackagesLock);

        AutoPtr<IUserInfo> info;
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(userId);
        if (it != mUsers.End()) {
            info = it->mSecond;
        }
        Boolean partial;
        if (info == NULL || (info->GetPartial(&partial), partial)) {
            Slogger::W(TAG, "getUserIcon: unknown user #%d", userId);
            return NOERROR;
        }
        String iconPath;
        info->GetIconPath(&iconPath);
        if (iconPath.IsNull()) {
            return NOERROR;
        }
        AutoPtr<IBitmapFactory> factory;
        CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
        return factory->DecodeFile(iconPath, userIcon);
    }
}

ECode CUserManagerService::SetGuestEnabled(
    /* [in] */ Boolean enable)
{
    FAIL_RETURN(CheckManageUsersPermission(String("enable guest users")));
    {
        AutoLock lock(mPackagesLock);

        if (mGuestEnabled != enable) {
            mGuestEnabled = enable;
            // Erase any guest user that currently exists
            HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it;
            for (it = mUsers.Begin(); it != mUsers.End(); ++it) {
                AutoPtr<IUserInfo> user = it->mSecond;
                Boolean partial, isGuest;
                user->GetPartial(&partial);
                user->IsGuest(&isGuest);
                if (!partial && isGuest) {
                    if (!enable) {
                        Int32 id;
                        user->GetId(&id);
                        Boolean succeeded;
                        RemoveUser(id, &succeeded);
                    }
                    return NOERROR;
                }
            }
            // No guest was found
            if (enable) {
                AutoPtr<IUserInfo> u;
                CreateUser(String("Guest"), IUserInfo::FLAG_GUEST, (IUserInfo**)&u);
            }
        }
    }
    return NOERROR;
}

ECode CUserManagerService::IsGuestEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    {
        AutoLock lock(mPackagesLock);

        *result = mGuestEnabled;
        return NOERROR;
    }
}

ECode CUserManagerService::WipeUser(
    /* [in] */ Int32 userHandle)
{
    FAIL_RETURN(CheckManageUsersPermission(String("wipe user")));
    // TODO:
    return NOERROR;
}

ECode CUserManagerService::MakeInitialized(
    /* [in] */ Int32 userId)
{
    FAIL_RETURN(CheckManageUsersPermission(String("makeInitialized")));
    {
        AutoLock lock(mPackagesLock);

        AutoPtr<IUserInfo> info;
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(userId);
        if (it != mUsers.End()) {
            info = it->mSecond;
        }
        Boolean partial;
        if (info == NULL || (info->GetPartial(&partial), partial)) {
            Slogger::W(TAG, "makeInitialized: unknown user #%d", userId);
        }
        Int32 flags;
        info->GetFlags(&flags);
        if ((flags& IUserInfo::FLAG_INITIALIZED) == 0) {
            flags |= IUserInfo::FLAG_INITIALIZED;
            info->SetFlags(flags);
            WriteUserLocked(info);
        }
    }
    return NOERROR;
}

Boolean CUserManagerService::IsUserLimitReachedLocked()
{
    Int32 nUsers = mUsers.GetSize();
    AutoPtr<IUserManagerHelper> helper;
    CUserManagerHelper::AcquireSingleton((IUserManagerHelper**)&helper);
    Int32 mUsers;
    helper->GetMaxSupportedUsers(&mUsers);
    return nUsers >= mUsers;
}

ECode CUserManagerService::CheckManageUsersPermission(
    /* [in] */ const String& message)
{
    const Int32 uid = Binder::GetCallingUid();
    AutoPtr<IActivityManagerHelper> amHelper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
    Int32 val;
    amHelper->CheckComponentPermission(
                    Elastos::Droid::Manifest::Permission::MANAGE_USERS,
                    uid, -1, TRUE, &val);
    if (uid != IProcess::SYSTEM_UID && uid != 0
            && val != IPackageManager::PERMISSION_GRANTED) {
        Slogger::D(TAG, "You need MANAGE_USERS permission to: ");
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
}

void CUserManagerService::WriteBitmapLocked(
    /* [in] */ IUserInfo* info,
    /* [in] */ IBitmap* bitmap)
{
    // try {
    Int32 id;
    info->GetId(&id);
    AutoPtr<IFile> dir;
    CFile::New(mUsersDir, StringUtils::Int32ToString(id), (IFile**)&dir);
    AutoPtr<IFile> file;
    CFile::New(dir, USER_PHOTO_FILENAME, (IFile**)&file);
    Boolean exists = FALSE;
    dir->Exists(&exists);
    if (!exists) {
        Boolean mked = FALSE;
        dir->Mkdir(&mked);
        String dirPath;
        dir->GetPath(&dirPath);
        FileUtils::SetPermissions(
                dirPath,
                FileUtils::sS_IRWXU|FileUtils::sS_IRWXG|FileUtils::sS_IXOTH,
                -1, -1);
    }
    AutoPtr<IFileOutputStream> os;
    CFileOutputStream::New(file, (IFileOutputStream**)&os);
    Boolean result = FALSE;
    if (bitmap->Compress(Elastos::Droid::Graphics::BitmapCompressFormat_PNG, 100, os, &result), result) {
        String aPath;
        file->GetAbsolutePath(&aPath);
        info->SetIconPath(aPath);
    }
    // try {
    os->Close();
    // } catch (IOException ioe) {
    //     // What the ... !
    // }
    // } catch (FileNotFoundException e) {
    //     Slog.w(TAG, "Error setting photo for user ", e);
    // }
}

AutoPtr< ArrayOf<Int32> > CUserManagerService::GetUserIds()
{
    {
        AutoLock lock(mPackagesLock);

        return mUserIds;
    }
}

AutoPtr< ArrayOf<Int32> > CUserManagerService::GetUserIdsLPr()
{
    return mUserIds;
}

void CUserManagerService::ReadUserList()
{
    {
        AutoLock lock(mPackagesLock);

        ReadUserListLocked();
    }
}

void CUserManagerService::ReadUserListLocked()
{
    String tag;
    mGuestEnabled = FALSE;
    Boolean isExist;
    if (mUserListFile->Exists(&isExist), !isExist) {
        FallbackToSingleUserLocked();
        return;
    }
    AutoPtr<IFileInputStream> fis;
    AutoPtr<IAtomicFile> userListFile;
    CAtomicFile::New(mUserListFile, (IAtomicFile**)&userListFile);
//     try {
    userListFile->OpenRead((IFileInputStream**)&fis);
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInput(fis, String(NULL));
    Int32 type;
    while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
            && type != IXmlPullParser::END_DOCUMENT) {
        ;
    }

    if (type != IXmlPullParser::START_TAG) {
        Slogger::E(TAG, "Unable to read user list");
        FallbackToSingleUserLocked();
        goto exit;
    }

    mNextSerialNumber = -1;
    if (parser->GetName(&tag), tag.Equals(TAG_USERS)) {
        String lastSerialNumber;
        parser->GetAttributeValue(String(NULL), ATTR_NEXT_SERIAL_NO, &lastSerialNumber);
        if (!lastSerialNumber.IsNull()) {
            mNextSerialNumber = StringUtils::ParseInt32(lastSerialNumber);
        }
        String versionNumber;
        parser->GetAttributeValue(String(NULL), ATTR_USER_VERSION, &versionNumber);
        if (!versionNumber.IsNull()) {
            mUserVersion = StringUtils::ParseInt32(versionNumber);
        }
    }

    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT) {
        if (type == IXmlPullParser::START_TAG && (parser->GetName(&tag), tag.Equals(TAG_USER))) {
            String id;
            parser->GetAttributeValue(String(NULL), ATTR_ID, &id);
            AutoPtr<IUserInfo> user = ReadUser(StringUtils::ParseInt32(id));

            if (user != NULL) {
                Int32 userId;
                user->GetId(&userId);
                mUsers[userId] = user;
                Boolean isGuest;
                if (user->IsGuest(&isGuest), isGuest) {
                    mGuestEnabled = TRUE;
                }
                if (mNextSerialNumber < 0 || mNextSerialNumber <= userId) {
                    mNextSerialNumber = userId + 1;
                }
            }
        }
    }

    if (mUsers.IsEmpty()) {
        FallbackToSingleUserLocked();
    }

    UpdateUserIdsLocked();
    UpgradeIfNecessary();
//     } catch (IOException ioe) {
//         fallbackToSingleUserLocked();
//     } catch (XmlPullParserException pe) {
//         fallbackToSingleUserLocked();
//     } catch (Exception e) {
//         Slog.e(LOG_TAG, "Error readUserListLocked , restore default" + "\n" + e);
//         fallbackToSingleUserLocked();
//     } finally {
exit:
    if (fis != NULL) {
        // try {
        fis->Close();
        // } catch (IOException e) {
        // }
    }
//     }
}

void CUserManagerService::UpgradeIfNecessary()
{
    Int32 userVersion = mUserVersion;
    if (userVersion < 1) {
        // Assign a proper name for the owner, if not initialized correctly before
        AutoPtr<IUserInfo> user;
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(IUserHandle::USER_OWNER);
        if (it != mUsers.End()) {
            user = it->mSecond;
        }
        assert(user != NULL);
        String temp;
        user->GetName(&temp);
        if (String("Primary").Equals(temp)) {
            AutoPtr<IResources> resources;
            mContext->GetResources((IResources**)&resources);
            resources->GetString(R::string::owner_name, &temp);
            user->SetName(temp);
            WriteUserLocked(user);
        }
        userVersion = 1;
    }

    if (userVersion < 2) {
        // Owner should be marked as initialized
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(IUserHandle::USER_OWNER);
        AutoPtr<IUserInfo> user;
        if (it != mUsers.End()) {
            user = it->mSecond;
            Int32 uFlags;
            user->GetFlags(&uFlags);
            if ((uFlags & IUserInfo::FLAG_INITIALIZED) == 0) {
                uFlags |= IUserInfo::FLAG_INITIALIZED;
                WriteUserLocked(user);
            }
            userVersion = 2;
        }
    }

    if (userVersion < USER_VERSION) {
        Slogger::W(TAG, String("User version ") + StringUtils::Int32ToString(mUserVersion) + String(" didn't upgrade as expected to ")
                + StringUtils::Int32ToString(USER_VERSION));
    } else {
        mUserVersion = userVersion;
        WriteUserListLocked();
    }
}

void CUserManagerService::FallbackToSingleUserLocked()
{
    // Create the primary user
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    String valueString;
    resources->GetString(R::string::owner_name, &valueString);
    AutoPtr<IUserInfo> primary;
    CUserInfo::New(0, valueString, String(NULL),
            IUserInfo::FLAG_ADMIN | IUserInfo::FLAG_PRIMARY | IUserInfo::FLAG_INITIALIZED, (IUserInfo**)&primary);
    mUsers[0] = primary;
    mNextSerialNumber = MIN_USER_ID;
    UpdateUserIdsLocked();

    WriteUserListLocked();
    WriteUserLocked(primary);
}

void CUserManagerService::WriteUserLocked(
    /* [in] */ IUserInfo* userInfo)
{
    AutoPtr<IFileOutputStream> fos;
    Int32 id;
    userInfo->GetId(&id);
    AutoPtr<IFile> baseFile;
    CFile::New(mUsersDir, StringUtils::Int32ToString(id) + ".xml", (IFile**)&baseFile);
    AutoPtr<IAtomicFile> userFile;
    CAtomicFile::New(baseFile, (IAtomicFile**)&userFile);
//     try {
    userFile->StartWrite((IFileOutputStream**)&fos);
    AutoPtr<IBufferedOutputStream> bos;
    CBufferedOutputStream::New(fos, (IBufferedOutputStream**)&bos);

    // XmlSerializer serializer = XmlUtils.serializerInstance();
    AutoPtr<IXmlSerializer> serializer;
    CFastXmlSerializer::New((IFastXmlSerializer**)&serializer);
    serializer->SetOutput(bos, String("utf-8"));
    AutoPtr<IBoolean> bv;
    CBoolean::New(TRUE, (IBoolean**)&bv);
    serializer->StartDocument(String(NULL), bv);
    serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE);

    serializer->WriteStartTag(String(NULL), TAG_USER);
    userInfo->GetId(&id);
    serializer->WriteAttribute(String(NULL), ATTR_ID, StringUtils::Int32ToString(id));
    Int32 serialNumber;
    userInfo->GetSerialNumber(&serialNumber);
    serializer->WriteAttribute(String(NULL), ATTR_SERIAL_NO, StringUtils::Int32ToString(serialNumber));
    Int32 flags;
    userInfo->GetFlags(&flags);
    serializer->WriteAttribute(String(NULL), ATTR_FLAGS, StringUtils::Int32ToString(flags));
    Int64 creationTime;
    userInfo->GetCreationTime(&creationTime);
    serializer->WriteAttribute(String(NULL), ATTR_CREATION_TIME, StringUtils::Int64ToString(creationTime));
    Int64 lastLoggedInTime;
    userInfo->GetLastLoggedInTime(&lastLoggedInTime);
    serializer->WriteAttribute(String(NULL), ATTR_LAST_LOGGED_IN_TIME,
            StringUtils::Int64ToString(lastLoggedInTime));
    String iconPath;
    userInfo->GetIconPath(&iconPath);
    if (!iconPath.IsNull()) {
        serializer->WriteAttribute(String(NULL),  ATTR_ICON_PATH, iconPath);
    }
    Boolean partial;
    userInfo->GetPartial(&partial);
    if (partial) {
        serializer->WriteAttribute(String(NULL), ATTR_PARTIAL, String("true"));
    }

    serializer->WriteStartTag(String(NULL), TAG_NAME);
    String name;
    userInfo->GetName(&name);
    serializer->WriteText(name);
    serializer->WriteEndTag(String(NULL), TAG_NAME);

    serializer->WriteEndTag(String(NULL), TAG_USER);

    serializer->EndDocument();
    userFile->FinishWrite(fos);
//     } catch (Exception ioe) {
//         Slog.e(TAG, "Error writing user info " + userInfo.id + "\n" + ioe);
//         userFile.failWrite(fos);
//     }
}

void CUserManagerService::WriteUserListLocked()
{
    AutoPtr<IFileOutputStream> fos;
    AutoPtr<IAtomicFile> userListFile;
    CAtomicFile::New(mUserListFile, (IAtomicFile**)&userListFile);
    // try {
    userListFile->StartWrite((IFileOutputStream**)&fos);
    AutoPtr<IBufferedOutputStream> bos;
    CBufferedOutputStream::New(fos, (IBufferedOutputStream**)&bos);

    // XmlSerializer serializer = XmlUtils.serializerInstance();
    AutoPtr<IFastXmlSerializer> serializer;
    CFastXmlSerializer::New((IFastXmlSerializer**)&serializer);
    serializer->SetOutput(bos, String("utf-8"));
    AutoPtr<IBoolean> bv;
    CBoolean::New(TRUE, (IBoolean**)&bv);
    serializer->StartDocument(String(NULL), bv);
    serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE);

    serializer->WriteStartTag(String(NULL), TAG_USERS);
    serializer->WriteAttribute(String(NULL), ATTR_NEXT_SERIAL_NO, StringUtils::Int32ToString(mNextSerialNumber));
    serializer->WriteAttribute(String(NULL), ATTR_USER_VERSION, StringUtils::Int32ToString(mUserVersion));

    HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it;
    for (it = mUsers.Begin(); it != mUsers.End(); ++it) {
        AutoPtr<IUserInfo> user = it->mSecond;
        Int32 id;
        user->GetId(&id);
        serializer->WriteStartTag(String(NULL), TAG_USER);
        serializer->WriteAttribute(String(NULL), ATTR_ID, StringUtils::Int32ToString(id));
        serializer->WriteEndTag(String(NULL), TAG_USER);
    }

    serializer->WriteEndTag(String(NULL), TAG_USERS);

    serializer->EndDocument();
    userListFile->FinishWrite(fos);
    // } catch (Exception e) {
    //     userListFile.failWrite(fos);
    //     Slog.e(TAG, "Error writing user list");
    // }
}

AutoPtr<IUserInfo> CUserManagerService::ReadUser(
    /* [in] */ Int32 id)
{
    Int32 flags = 0;
    Int32 serialNumber = id;
    String tag;
    String name;
    String iconPath;
    Int64 creationTime = 0ll;
    Int64 lastLoggedInTime = 0ll;
    Boolean partial = FALSE;

    AutoPtr<IUserInfo> userInfo;
    AutoPtr<IFileInputStream> fis;
//     try {
    AutoPtr<IFile> baseFile;
    CFile::New(mUsersDir, StringUtils::Int32ToString(id) + ".xml", (IFile**)&baseFile);
    AutoPtr<IAtomicFile> userFile;
    CAtomicFile::New(baseFile, (IAtomicFile**)&userFile);
    userFile->OpenRead((IFileInputStream**)&fis);
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInput(fis, String(NULL));
    Int32 type;
    while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
            && type != IXmlPullParser::END_DOCUMENT) {
        ;
    }

    if (type != IXmlPullParser::START_TAG) {
        Slogger::E(TAG, "Unable to read user %d", id);
        userInfo = NULL;
        goto exit;
    }

    if (type == IXmlPullParser::START_TAG && (parser->GetName(&tag), tag.Equals(TAG_USER))) {
        Int32 storedId = ReadInt32Attribute(parser, ATTR_ID, -1);
        if (storedId != id) {
            Slogger::E(TAG, "User id does not match the file name");
            userInfo = NULL;
            goto exit;
        }
        serialNumber = ReadInt32Attribute(parser, ATTR_SERIAL_NO, id);
        flags = ReadInt32Attribute(parser, ATTR_FLAGS, 0);
        parser->GetAttributeValue(String(NULL), ATTR_ICON_PATH, &iconPath);
        creationTime = ReadInt64Attribute(parser, ATTR_CREATION_TIME, 0);
        lastLoggedInTime = ReadInt64Attribute(parser, ATTR_LAST_LOGGED_IN_TIME, 0);
        String valueString;
        parser->GetAttributeValue(String(NULL), ATTR_PARTIAL, &valueString);
        if (valueString.Equals("true")) {
            partial = TRUE;
        }

        while ((parser->Next(&type), type) != IXmlPullParser::START_TAG
                && type != IXmlPullParser::END_DOCUMENT) {
        }
        if (type == IXmlPullParser::START_TAG && (parser->GetName(&tag), tag.Equals(TAG_NAME))) {
            parser->Next(&type);
            if (type == IXmlPullParser::TEXT) {
                parser->GetText(&name);
            }
        }
    }

    CUserInfo::New(id, name, iconPath, flags, (IUserInfo**)&userInfo);
    userInfo->SetSerialNumber(serialNumber);
    userInfo->SetCreationTime(creationTime);
    userInfo->SetLastLoggedInTime(lastLoggedInTime);
    userInfo->SetPartial(partial);

//     } catch (IOException ioe) {
//     } catch (XmlPullParserException pe) {
//     } finally {
exit:
    if (fis != NULL) {
        // try {
        fis->Close();
        // } catch (IOException e) {
        // }
    }
//     }
    return userInfo;
}

Int32 CUserManagerService::ReadInt32Attribute(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& attr,
    /* [in] */ Int32 defaultValue)
{
    String valueString;
    parser->GetAttributeValue(String(NULL), attr, &valueString);
    if (valueString.IsNull()) return defaultValue;
    // try {
    return StringUtils::ParseInt32(valueString);
    // } catch (NumberFormatException nfe) {
    //     return defaultValue;
    // }
}

Int64 CUserManagerService::ReadInt64Attribute(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& attr,
    /* [in] */ Int64 defaultValue)
{
    String valueString;
    parser->GetAttributeValue(String(NULL), attr, &valueString);
    if (valueString.IsNull()) return defaultValue;
    // try {
    return StringUtils::ParseInt64(valueString);
    // } catch (NumberFormatException nfe) {
    //     return defaultValue;
    // }
}

ECode CUserManagerService::CreateUser(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IUserInfo** uInfo)
{
    VALIDATE_NOT_NULL(uInfo);
    *uInfo = NULL;

    FAIL_RETURN(CheckManageUsersPermission(String("Only the system can create users")));

    const Int64 ident = Binder::ClearCallingIdentity();
    AutoPtr<IUserInfo> userInfo;
    // try {
        {
            AutoLock lock(mInstallLock);
            {
                AutoLock lock(mPackagesLock);
                if (IsUserLimitReachedLocked()) return NOERROR;
                Int32 userId = GetNextAvailableIdLocked();
                CUserInfo::New(userId, name, String(NULL), flags, (IUserInfo**)&userInfo);
                AutoPtr<IFile> userPath;
                CFile::New(mBaseUserPath, StringUtils::Int32ToString(userId), (IFile**)&userPath);
                userInfo->SetSerialNumber(mNextSerialNumber++);
                AutoPtr<ISystem> system;
                Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
                Int64 now;
                system->GetCurrentTimeMillis(&now);
                userInfo->SetCreationTime((now > EPOCH_PLUS_30_YEARS) ? now : 0);
                userInfo->SetPartial(TRUE);
                Int32 id;
                userInfo->GetId(&id);
                AutoPtr<IEnvironment> env;
                CEnvironment::AcquireSingleton((IEnvironment**)&env);
                AutoPtr<IFile> usDir;
                env->GetUserSystemDirectory(id, (IFile**)&usDir);
                Boolean mked;
                usDir->Mkdirs(&mked);
                mUsers[userId] = userInfo;
                WriteUserListLocked();
                WriteUserLocked(userInfo);
                mPm->CreateNewUserLILPw(userId, userPath);
                userInfo->SetPartial(FALSE);
                WriteUserLocked(userInfo);
                UpdateUserIdsLocked();
            }
        }
        if (userInfo != NULL) {
            AutoPtr<IIntent> addedIntent;
            CIntent::New(IIntent::ACTION_USER_ADDED, (IIntent**)&addedIntent);
            Int32 id;
            userInfo->GetId(&id);
            addedIntent->PutExtra(IIntent::EXTRA_USER_HANDLE, id);
            mContext->SendBroadcastAsUser(addedIntent, UserHandle::ALL,
                    Elastos::Droid::Manifest::Permission::MANAGE_USERS);
        }
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
    *uInfo = userInfo;
    REFCOUNT_ADD(*uInfo);
    return NOERROR;
}

ECode CUserManagerService::RemoveUser(
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    FAIL_RETURN(CheckManageUsersPermission(String("Only the system can remove users")));
    AutoPtr<IUserInfo> user;
    {
        AutoLock lock(mPackagesLock);
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(userHandle);
        if (it != mUsers.End()) user = it->mSecond;
        if (userHandle == 0 || user == NULL) {
            *succeeded = FALSE;
            return NOERROR;
        }
        mRemovingUserIds[userHandle] = TRUE;
        // Set this to a partially created user, so that the user will be purged
        // on next startup, in case the runtime stops now before stopping and
        // removing the user completely.
        user->SetPartial(TRUE);
        WriteUserLocked(user);
    }
    if (DBG) Slogger::I(TAG, "Stopping user %d", userHandle);
    Int32 res;
    // try {
    AutoPtr<IStopUserCallback> callback;
    CUserStopUserCallback::New(this, (IStopUserCallback**)&callback);
    if (FAILED(ActivityManagerNative::GetDefault()->StopUser(userHandle, callback, &res))) {
        *succeeded = FALSE;
        return NOERROR;
    }
    // } catch (RemoteException e) {
    //     return false;
    // }

    *succeeded = res == IActivityManager::USER_OP_SUCCESS;
    return NOERROR;
}

void CUserManagerService::FinishRemoveUser(
    /* [in] */ Int32 userHandle)
{
    if (DBG) Slogger::I(TAG, "finishRemoveUser %d", userHandle);
    // Let other services shutdown any activity and clean up their state before completely
    // wiping the user's system directory and removing from the user list
    Int64 ident = Binder::ClearCallingIdentity();
    // try {
    AutoPtr<IIntent> addedIntent;
    CIntent::New(IIntent::ACTION_USER_REMOVED, (IIntent**)&addedIntent);
    addedIntent->PutExtra(IIntent::EXTRA_USER_HANDLE, userHandle);
    AutoPtr<IBroadcastReceiver> receiver = new FinishBroadcastReceiver(this, userHandle);
    mContext->SendOrderedBroadcastAsUser(addedIntent, UserHandle::ALL,
            Elastos::Droid::Manifest::Permission::MANAGE_USERS, receiver,
            NULL, IActivity::RESULT_OK, String(NULL), NULL);
    // } finally {
    Binder::RestoreCallingIdentity(ident);
    // }
}

void CUserManagerService::RemoveUserStateLocked(
    /* [in] */ Int32 userHandle)
{
    // Cleanup package manager settings
    mPm->CleanUpUserLILPw(userHandle);

    // Remove this user from the list
    mUsers.Erase(userHandle);

    // Have user ID linger for several seconds to let external storage VFS
    // cache entries expire. This must be greater than the 'entry_valid'
    // timeout used by the FUSE daemon.
    AutoPtr<IRunnable> runnable = new RemoveRunnable(this, userHandle);
    Boolean result;
    mHandler->PostDelayed(runnable, IDateUtils::MINUTE_IN_MILLIS, &result);

    mRemovingUserIds.Erase(userHandle);
    // Remove user file
    AutoPtr<IFile> file;
    StringBuilder sb("");
    sb += userHandle;
    sb += ".xml";
    CFile::New(mUsersDir, sb.ToString(), (IFile**)&file);
    AutoPtr<IAtomicFile> userFile;
    CAtomicFile::New(file, (IAtomicFile**)&userFile);
    userFile->Delete();
    // Update the user list
    WriteUserListLocked();
    UpdateUserIdsLocked();
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> usDir;
    env->GetUserSystemDirectory(userHandle, (IFile**)&usDir);
    RemoveDirectoryRecursive(usDir);
}

void CUserManagerService::RemoveDirectoryRecursive(
    /* [in] */ IFile* parent)
{
    Boolean result;
    if (parent->IsDirectory(&result), result) {
        AutoPtr< ArrayOf<String> > files;
        parent->List((ArrayOf<String>**)&files);
        for (Int32 i = 0; i < files->GetLength(); ++i) {
            String filename = (*files)[i];
            AutoPtr<IFile> child;
            CFile::New(parent, filename, (IFile**)&child);
            RemoveDirectoryRecursive(child);
        }
    }
    parent->Delete(&result);
}

ECode CUserManagerService::GetUserSerialNumber(
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* serialNo)
{
    VALIDATE_NOT_NULL(serialNo);

    {
        AutoLock lock(mPackagesLock);

        if (!Exists(userHandle)) {
            *serialNo = -1;
            return NOERROR;
        }
        return GetUserInfoLocked(userHandle)->GetSerialNumber(serialNo);
    }
}

ECode CUserManagerService::GetUserHandle(
    /* [in] */ Int32 userSerialNumber,
    /* [out] */ Int32* userHandle)
{
    VALIDATE_NOT_NULL(userHandle);

    {
        AutoLock lock(mPackagesLock);

        for (Int32 i = 0; i < mUserIds->GetLength(); ++i) {
            Int32 userId = (*mUserIds)[i];
            AutoPtr<IUserInfo> user = GetUserInfoLocked(userId);
            Int32 serialNumber;
            user->GetSerialNumber(&serialNumber);
            if (serialNumber == userSerialNumber) {
                *userHandle = userId;
                return NOERROR;
            }
        }
        // Not found
        *userHandle = -1;
        return NOERROR;
    }
}

void CUserManagerService::UpdateUserIdsLocked()
{
    Int32 num = 0;
    HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it;
    for (it = mUsers.Begin(); it != mUsers.End(); ++it) {
        Boolean partial;
        it->mSecond->GetPartial(&partial);
        if (!partial) {
            num++;
        }
    }
    AutoPtr< ArrayOf<Int32> > newUsers = ArrayOf<Int32>::Alloc(num);
    Int32 n = 0;
    for (it = mUsers.Begin(); it != mUsers.End(); ++it) {
        Boolean partial;
        it->mSecond->GetPartial(&partial);
        if (!partial) {
            (*newUsers)[n++] = it->mFirst;
        }
    }
    mUserIds = newUsers;
}

void CUserManagerService::UserForeground(
    /* [in] */ Int32 userId)
{
    {
        AutoLock lock(mPackagesLock);

        AutoPtr<IUserInfo> user;
        HashMap<Int32, AutoPtr<IUserInfo> >::Iterator it = mUsers.Find(userId);
        if (it != mUsers.End()) {
            user = it->mSecond;
        }

        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 now;
        system->GetCurrentTimeMillis(&now);
        Boolean partial;
        if (user == NULL || (user->GetPartial(&partial), partial)) {
            Slogger::W(TAG, "userForeground: unknown user #%d", userId);
            return;
        }
        if (now > EPOCH_PLUS_30_YEARS) {
            user->SetLastLoggedInTime(now);
            WriteUserLocked(user);
        }
    }
}

Int32 CUserManagerService::GetNextAvailableIdLocked()
{
    {
        AutoLock lock(mPackagesLock);

        Int32 i = MIN_USER_ID;
        while (i < Elastos::Core::Math::INT32_MAX_VALUE) {
            if (mUsers.Find(i) == mUsers.End() &&
                    mRemovingUserIds.Find(i) == mRemovingUserIds.End()) {
                break;
            }
            i++;
        }
        return i;
    }
}

// @Override
// protected void dump(FileDescriptor fd, PrintWriter pw, String[] args) {
//     if (mContext.checkCallingOrSelfPermission(Elastos::Droid::Manifest::Permission::DUMP)
//             != PackageManager.PERMISSION_GRANTED) {
//         pw.println("Permission Denial: can't dump UserManager from from pid="
//                 + Binder.getCallingPid()
//                 + ", uid=" + Binder.getCallingUid()
//                 + " without permission "
//                 + Elastos::Droid::Manifest::Permission::DUMP);
//         return;
//     }

//     long now = System.currentTimeMillis();
//     StringBuilder sb = new StringBuilder();
//     synchronized (mPackagesLock) {
//         pw.println("Users:");
//         for (int i = 0; i < mUsers.size(); i++) {
//             UserInfo user = mUsers.valueAt(i);
//             if (user == null) continue;
//             pw.print("  "); pw.print(user); pw.print(" serialNo="); pw.print(user.serialNumber);
//             if (mRemovingUserIds.contains(mUsers.keyAt(i))) pw.print(" <removing> ");
//             if (user.partial) pw.print(" <partial>");
//             pw.println();
//             pw.print("    Created: ");
//             if (user.creationTime == 0) {
//                 pw.println("<unknown>");
//             } else {
//                 sb.setLength(0);
//                 TimeUtils.formatDuration(now - user.creationTime, sb);
//                 sb.append(" ago");
//                 pw.println(sb);
//             }
//             pw.print("    Last logged in: ");
//             if (user.lastLoggedInTime == 0) {
//                 pw.println("<unknown>");
//             } else {
//                 sb.setLength(0);
//                 TimeUtils.formatDuration(now - user.lastLoggedInTime, sb);
//                 sb.append(" ago");
//                 pw.println(sb);
//             }
//         }
//     }
// }

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
