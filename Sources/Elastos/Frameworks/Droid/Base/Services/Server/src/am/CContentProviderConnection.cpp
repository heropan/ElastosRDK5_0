
#include "am/CContentProviderConnection.h"
#include "am/ProcessRecord.h"
#include "am/ContentProviderRecord.h"
#include "os/SystemClock.h"

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::EIID_IBinder;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

CContentProviderConnection::CContentProviderConnection()
    : mProvider(NULL)
    , mClient(NULL)
    , mCreateTime(0)
    , mStableCount(0)
    , mUnstableCount(0)
    , mWaiting(FALSE)
    , mDead(FALSE)
    , mNumStableIncs(0)
    , mNumUnstableIncs(0)
{}

CContentProviderConnection::~CContentProviderConnection()
{
    mProvider = NULL;
    mClient = NULL;
}

ECode CContentProviderConnection::constructor(
    /* [in] */ Handle32 provider,
    /* [in] */ Handle32 client)
{
    mProvider = (ContentProviderRecord*)provider;
    mClient = (ProcessRecord*)client;
    mCreateTime = SystemClock::GetElapsedRealtime();
    return NOERROR;
}

ECode CContentProviderConnection::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb(128);
    sb += ("CContentProviderConnection{");
    ToShortString(&sb);
    sb += ('}');
    *str = sb.ToString();
    return NOERROR;
}

String CContentProviderConnection::ToShortString()
{
    StringBuilder sb(128);
    ToShortString(&sb);
    return sb.ToString();
}

String CContentProviderConnection::ToClientString()
{
    StringBuilder sb(128);
    ToClientString(&sb);
    return sb.ToString();
}

void CContentProviderConnection::ToShortString(
    /* [in] */ StringBuilder* sb)
{
    if (sb == NULL)
        return;

    *sb += mProvider->ToShortString();
    *sb += ("->");
    ToClientString(sb);
}

void CContentProviderConnection::ToClientString(
    /* [in] */ StringBuilder* sb)
{
    if (sb == NULL)
        return;

    if (mClient != NULL) {
        *sb += (mClient->ToShortString());
    }
    *sb += (" s");
    *sb += (mStableCount);
    *sb += ("/");
    *sb += (mNumStableIncs);
    *sb += (" u");
    *sb += (mUnstableCount);
    *sb += ("/");
    *sb += (mNumUnstableIncs);
    if (mWaiting) {
        *sb += (" WAITING");
    }
    if (mDead) {
        *sb += (" DEAD");
    }

    Int64 nowReal = SystemClock::GetElapsedRealtime();
    *sb += (" ");
    // TODO TimeUtils.formatDuration(nowReal - mCreateTime, sb);
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
