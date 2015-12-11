
#include "elastos/droid/server/am/CPendingIntentRecordKey.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include "elastos/droid/server/am/ActivityRecord.h"
#include "elastos/droid/server/am/CActivityManagerService.h"

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {


const Int32 CPendingIntentRecordKey::ODD_PRIME_NUMBER;

CPendingIntentRecordKey::CPendingIntentRecordKey(
    /* [in] */ Int32 t,
    /* [in] */ const String& p,
    /* [in] */ ActivityRecord * a,
    /* [in] */ const String& w,
    /* [in] */ Int32 r,
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ ArrayOf<String>* strings,
    /* [in] */ Int32 f,
    /* [in] */ IBundle* o,
    /* [in] */ Int32 userId)
    : mType(t)
    , mPackageName(p)
    , mActivity(a)
    , mWho(w)
    , mRequestCode(r)
    , mOptions(o)
    , mFlags(f)
    , mUserId(0)
{
    if (intents != NULL && intents->GetLength() > 0) {
        mRequestIntent = (*intents)[intents->GetLength() - 1];
        mAllIntents = intents;
    }

    if (strings != NULL && strings->GetLength() > 0) {
        mRequestResolvedType = (*strings)[strings->GetLength() - 1];
        mAllResolvedTypes = strings;
    }

    Int32 hash = 23;
    hash = (ODD_PRIME_NUMBER*hash) + f;
    hash = (ODD_PRIME_NUMBER*hash) + r;
    hash = (ODD_PRIME_NUMBER*hash) + userId;
    if (!w.IsNull()) {
        hash = (ODD_PRIME_NUMBER*hash) + w.GetHashCode();
    }

    if (a != NULL) {
        hash = (ODD_PRIME_NUMBER*hash) + (Int32)a;
    }

    if (mRequestIntent != NULL) {
        Int32 riHashCode;
        mRequestIntent->FilterHashCode(&riHashCode);
        hash = (ODD_PRIME_NUMBER*hash) + riHashCode;
    }

    if (!mRequestResolvedType.IsNull()) {
        hash = (ODD_PRIME_NUMBER*hash) + mRequestResolvedType.GetHashCode();
    }

    if (!p.IsNull()) {
        hash = (ODD_PRIME_NUMBER*hash) + p.GetHashCode();
    }

    hash = (ODD_PRIME_NUMBER*hash) + t;
    mHashCode = hash;
}

CPendingIntentRecordKey::~CPendingIntentRecordKey()
{
    mRequestIntent = NULL;
    mOptions = NULL;
    mAllIntents = NULL;
    mAllResolvedTypes = NULL;
}

ECode CPendingIntentRecordKey::Equals(
    /* [in] */ const CPendingIntentRecordKey* other,
    /* [out] */ Boolean* result) const
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (other == NULL) {
        return NOERROR;
    }

    if (mType != other->mType) {
        return NOERROR;
    }
    if (mUserId != other->mUserId){
        return NOERROR;
    }

    if (!mPackageName.IsNull()) {
        if (!mPackageName.Equals(other->mPackageName)) {
            return NOERROR;
        }
    }
    else if (!other->mPackageName.IsNull()) {
        return NOERROR;
    }

    if (mActivity != other->mActivity) {
        return NOERROR;
    }

    if (!mWho.IsNull()) {
        if (!mWho.Equals(other->mWho)) {
            return NOERROR;
        }
    }
    else if (!other->mWho.IsNull()) {
        return NOERROR;
    }

    if (mRequestCode != other->mRequestCode) {
        return NOERROR;
    }

    if (mRequestIntent != other->mRequestIntent) {
        if (mRequestIntent != NULL) {
            Boolean equals = FALSE;
            mRequestIntent->FilterEquals(other->mRequestIntent, &equals);
            if (!equals) {
                return NOERROR;
            }
        } else if (other->mRequestIntent != NULL) {
            return NOERROR;
        }
    }

    if (!mRequestResolvedType.IsNull()) {
        if (!mRequestResolvedType.Equals(other->mRequestResolvedType)) {
            return NOERROR;
        }
    }
    else if (!other->mRequestResolvedType.IsNull()) {
        return NOERROR;
    }

    if (mFlags != other->mFlags) {
        return NOERROR;
    }
    *result = TRUE;
    return NOERROR;
}

ECode CPendingIntentRecordKey::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = mHashCode;
    return NOERROR;
}

String CPendingIntentRecordKey::ToString()
{
    StringBuilder sb("Key{");
    sb += TypeName();
    sb += " pkg=";
    sb += mPackageName;
    sb += " intent=";
    if (mRequestIntent) {
        String temp;
        mRequestIntent->ToShortString(FALSE, TRUE, FALSE, FALSE, &temp);
        sb += temp;
    }
    else {
        sb += "<null>";
    }
    sb += " flags=0x";
    sb += StringUtils::Int32ToHexString(mFlags);
    sb += " u=";
    sb += mUserId;
    sb += "}";

    return sb.ToString();
}

String CPendingIntentRecordKey::TypeName()
{
    switch (mType) {
        case IActivityManager::INTENT_SENDER_ACTIVITY:
            return String("startActivity");
        case IActivityManager::INTENT_SENDER_BROADCAST:
            return String("broadcastIntent");
        case IActivityManager::INTENT_SENDER_SERVICE:
            return String("startService");
        case IActivityManager::INTENT_SENDER_ACTIVITY_RESULT:
            return String("activityResult");
    }
    return StringUtils::Int32ToString(mType);
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
