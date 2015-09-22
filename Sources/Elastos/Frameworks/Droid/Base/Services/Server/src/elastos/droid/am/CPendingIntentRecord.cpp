#include "ext/frameworkext.h"
#include "am/CPendingIntentRecord.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Slogger.h>
#include "os/Binder.h"
#include "am/CActivityManagerService.h"
#include "am/ActivityStack.h"

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::App::IPendingIntent;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

// 67BF1DDF-D715-4C06-9D48-3C3ACC9D16F6
extern "C" const InterfaceID EIID_PendingIntentRecord =
        { 0x67bf1ddf, 0xd715, 0x4c06, { 0x9d, 0x48, 0x3c, 0x3a, 0xcc, 0x9d, 0x16, 0xf6 } };

CPendingIntentRecord::CPendingIntentRecord() :
    mOwner(0),
    mUid(0),
    mSent(FALSE),
    mCanceled(FALSE)
{
}

PInterface CPendingIntentRecord::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_PendingIntentRecord) {
        return reinterpret_cast<PInterface>(this);
    }
    return _CPendingIntentRecord::Probe(riid);
}

ECode CPendingIntentRecord::constructor(
    /* [in] */ Handle32 owner,
    /* [in] */ Handle32 k,
    /* [in] */ Int32 u)
{
    return Init(owner, k, u);
}

ECode CPendingIntentRecord::Init(
    /* [in] */ Handle32 owner,
    /* [in] */ Handle32 k,
    /* [in] */ Int32 u)
{
    mOwner = (CActivityManagerService*)owner;
    mKey = (CPendingIntentRecordKey*)k;
    mUid = u;
    GetWeakReference((IWeakReference**)&mRef);
    return NOERROR;
}

ECode CPendingIntentRecord::Send(
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IIntentReceiver* finishedReceiver,
    /* [in] */ const String& requiredPermission,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = SendInner(code, intent, resolvedType, finishedReceiver,
                requiredPermission, NULL, String(NULL), 0, 0, 0, NULL);
    return NOERROR;
}

Int32 CPendingIntentRecord::SendInner(
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& aResolvedType,
    /* [in] */ IIntentReceiver* finishedReceiver,
    /* [in] */ const String& requiredPermission,
    /* [in] */ IBinder* resultTo,
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ IBundle* options)
{
    String resolvedType(aResolvedType);
    AutoLock lock(mOwner->mLock);

    if (!mCanceled) {
        mSent = TRUE;
        if ((mKey->mFlags & IPendingIntent::FLAG_ONE_SHOT) != 0) {
            mOwner->CancelIntentSenderLocked(this, TRUE);
            mCanceled = TRUE;
        }

        AutoPtr<IIntent> finalIntent;
        if (mKey->mRequestIntent != NULL) {
            CIntent::New(mKey->mRequestIntent, (IIntent**)&finalIntent);
        }
        else {
            CIntent::New((IIntent**)&finalIntent);
        }

        if (intent != NULL) {
            Int32 changes;
            finalIntent->FillIn(intent, mKey->mFlags, &changes);
            if ((changes & IIntent::FILL_IN_DATA) == 0) {
                resolvedType = mKey->mRequestResolvedType;
            }
        }
        else {
            resolvedType = mKey->mRequestResolvedType;
        }

        flagsMask &= ~IIntent::IMMUTABLE_FLAGS;
        flagsValues &= flagsMask;
        Int32 flags;
        finalIntent->GetFlags(&flags);
        finalIntent->SetFlags((flags&~flagsMask) | flagsValues);

        Int64 origId = Binder::ClearCallingIdentity();

        Boolean sendFinish = finishedReceiver != NULL;
        Int32 userId = mKey->mUserId;
        if (userId == IUserHandle::USER_CURRENT) {
            userId = mOwner->GetCurrentUserIdLocked();
        }

        Int32 status;
        ECode ec = NOERROR;
        String nullStr;

        switch (mKey->mType) {
        case IActivityManager::INTENT_SENDER_ACTIVITY:
            if (options == NULL) {
                options = mKey->mOptions;
            }
            else if (mKey->mOptions != NULL) {
                AutoPtr<IBundle> opts;
                CBundle::New(mKey->mOptions, (IBundle**)&opts);
                opts->PutAll(options);
                options = opts;
            }

            if (mKey->mAllIntents != NULL && mKey->mAllIntents->GetLength() > 1) {
                Int32 length = mKey->mAllIntents->GetLength();
                AutoPtr< ArrayOf<IIntent*> > allIntents = ArrayOf<IIntent*>::Alloc(length);
                AutoPtr< ArrayOf<String> > allResolvedTypes = ArrayOf<String>::Alloc(length);
                allIntents->Copy(mKey->mAllIntents);
                if (mKey->mAllResolvedTypes != NULL)
                    allResolvedTypes->Copy(mKey->mAllResolvedTypes);

                allIntents->Set(length - 1, finalIntent);
                allResolvedTypes->Set(length -1, resolvedType);
                ec = mOwner->StartActivitiesInPackage(mUid, allIntents,
                        allResolvedTypes, resultTo, options, userId, &status);
            }
            else {
                ec = mOwner->StartActivityInPackage(mUid, finalIntent, resolvedType,
                        resultTo, resultWho, requestCode, 0, options, userId, &status);
            }

            if (ec == (ECode)E_RUNTIME_EXCEPTION) {
                Slogger::W(CActivityManagerService::TAG,
                        "Unable to send startActivity intent %08x", ec);
            }
            break;

        case IActivityManager::INTENT_SENDER_ACTIVITY_RESULT:
            mKey->mActivity->mStack->SendActivityResultLocked(-1, mKey->mActivity,
                    mKey->mWho, mKey->mRequestCode, code, finalIntent);
            break;

        case IActivityManager::INTENT_SENDER_BROADCAST:
            // If a completion callback has been requested, require
            // that the broadcast be delivered synchronously
            ec = mOwner->BroadcastIntentInPackage(mKey->mPackageName, mUid,
                    finalIntent, resolvedType, finishedReceiver, code, nullStr, NULL,
                    requiredPermission, (finishedReceiver != NULL), FALSE, userId, &status);
            sendFinish = FALSE;
            if (ec == (ECode)E_RUNTIME_EXCEPTION) {
                Slogger::W(CActivityManagerService::TAG,
                        "Unable to send startActivity intent %08x", ec);
            }
            break;

        case IActivityManager::INTENT_SENDER_SERVICE:
            AutoPtr<IComponentName> cn;
            ec = mOwner->StartServiceInPackage(mUid, finalIntent, resolvedType, userId, (IComponentName**)&cn);
            if (ec == (ECode)E_RUNTIME_EXCEPTION) {
                Slogger::W(CActivityManagerService::TAG,
                        "Unable to send startService intent %08x", ec);
            }
            break;
        }

        if (sendFinish) {
//            try {
            String nullStr;
            AutoPtr<IIntent> newIntent;
            CIntent::New(finalIntent, (IIntent**)&newIntent);
            finishedReceiver->PerformReceive(newIntent, 0,
                    nullStr, NULL, FALSE, FALSE, mKey->mUserId);
//            } catch (RemoteException e) {
//            }
        }

        Binder::RestoreCallingIdentity(origId);
        return 0;
    }

    return IActivityManager::START_CANCELED;
}

ECode CPendingIntentRecord::CompleteFinalize()
{
    AutoLock lock(mOwner->mLock);
    typename CActivityManagerService::PendingIntentRecordHashMap::Iterator it;
    it = mOwner->mIntentSenderRecords.Find(mKey);
    if (it != mOwner->mIntentSenderRecords.End()) {
        if (it->mSecond == mRef) {
            mOwner->mIntentSenderRecords.Erase(it);
        }
    }
    return NOERROR;
}

ECode CPendingIntentRecord::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    if (!mStringName.IsNull()) {
        *str = mStringName;
        return NOERROR;
    }

    StringBuilder sb("PendingIntentRecord{");
    sb += (StringUtils::Int32ToHexString(Int32(this)));
    sb += (' ');
    sb += (mKey->mPackageName);
    sb += (' ');
    sb += (mKey->TypeName());
    sb += ('}');
    mStringName = sb.ToString();
    *str = mStringName;
    return NOERROR;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
