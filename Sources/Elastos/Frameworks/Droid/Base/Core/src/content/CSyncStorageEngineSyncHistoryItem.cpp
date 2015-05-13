
#include "content/CSyncStorageEngineSyncHistoryItem.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Content {

CSyncStorageEngineSyncHistoryItem::CSyncStorageEngineSyncHistoryItem()
    : mAuthorityId(0)
    , mHistoryId(0)
    , mEventTime(0)
    , mElapsedTime(0)
    , mSource(0)
    , mEvent(0)
    , mUpstreamActivity(0)
    , mDownstreamActivity(0)
    , mInitialization(FALSE)
{}

CSyncStorageEngineSyncHistoryItem::~CSyncStorageEngineSyncHistoryItem()
{}

ECode CSyncStorageEngineSyncHistoryItem::GetAuthorityId(
    /* [out] */ Int32* authorityId)
{
    VALIDATE_NOT_NULL(authorityId)
    *authorityId = mAuthorityId;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetAuthorityId(
    /* [in] */ Int32 authorityId)
{
    mAuthorityId = authorityId;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetHistoryId(
    /* [out] */ Int32* historyId)
{
    VALIDATE_NOT_NULL(historyId)
    *historyId = mHistoryId;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetHistoryId(
    /* [in] */ Int32 historyId)
{
    mHistoryId = historyId;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetEventTime(
    /* [out] */ Int64* eventTime)
{
    VALIDATE_NOT_NULL(eventTime)
    *eventTime = mEventTime;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetEventTime(
    /* [in] */ Int64 eventTime)
{
    mEventTime = eventTime;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetElapsedTime(
    /* [out] */ Int64* elapsedTime)
{
    VALIDATE_NOT_NULL(elapsedTime)
    *elapsedTime = mElapsedTime;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetElapsedTime(
    /* [in] */ Int64 elapsedTime)
{
    mElapsedTime = elapsedTime;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetSource(
    /* [out] */ Int32* source)
{
    VALIDATE_NOT_NULL(source)
    *source = mSource;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetSource(
    /* [in] */ Int32 source)
{
    mSource = source;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetEvent(
    /* [out] */ Int32* event)
{
    VALIDATE_NOT_NULL(event)
    *event = mEvent;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetEvent(
    /* [in] */ Int32 event)
{
    mEvent = event;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetUpstreamActivity(
    /* [out] */ Int64* upstreamActivity)
{
    VALIDATE_NOT_NULL(upstreamActivity)
    *upstreamActivity = mUpstreamActivity;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetUpstreamActivity(
    /* [in] */ Int64 upstreamActivity)
{
    mUpstreamActivity = upstreamActivity;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetDownstreamActivity(
    /* [out] */ Int64* downstreamActivity)
{
    VALIDATE_NOT_NULL(downstreamActivity)
    *downstreamActivity = mDownstreamActivity;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetDownstreamActivity(
    /* [in] */ Int64 downstreamActivity)
{
    mDownstreamActivity = downstreamActivity;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetMesg(
    /* [out] */ String* mesg)
{
    VALIDATE_NOT_NULL(mesg)
    *mesg = mMesg;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetMesg(
    /* [in] */ const String& mesg)
{
    mMesg = mesg;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::GetInitialization(
    /* [out] */ Boolean* initialization)
{
    VALIDATE_NOT_NULL(initialization)
    *initialization = mInitialization;
    return NOERROR;
}

ECode CSyncStorageEngineSyncHistoryItem::SetInitialization(
    /* [in] */ Boolean initialization)
{
    mInitialization = initialization;
    return NOERROR;
}

}
}
}

