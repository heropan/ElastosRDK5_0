
#ifndef __CSYNCSTORAGEENGINESYNCHISTORYITEM_H__
#define __CSYNCSTORAGEENGINESYNCHISTORYITEM_H__

#include "_CSyncStorageEngineSyncHistoryItem.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncStorageEngineSyncHistoryItem)
{
public:
    CSyncStorageEngineSyncHistoryItem();

    ~CSyncStorageEngineSyncHistoryItem();

    CARAPI GetAuthorityId(
        /* [out] */ Int32* authorityId);

    CARAPI SetAuthorityId(
        /* [in] */ Int32 authorityId);

    CARAPI GetHistoryId(
        /* [out] */ Int32* historyId);

    CARAPI SetHistoryId(
        /* [in] */ Int32 historyId);

    CARAPI GetEventTime(
        /* [out] */ Int64* eventTime);

    CARAPI SetEventTime(
        /* [in] */ Int64 eventTime);

    CARAPI GetElapsedTime(
        /* [out] */ Int64* elapsedTime);

    CARAPI SetElapsedTime(
        /* [in] */ Int64 elapsedTime);

    CARAPI GetSource(
        /* [out] */ Int32* source);

    CARAPI SetSource(
        /* [in] */ Int32 source);

    CARAPI GetEvent(
        /* [out] */ Int32* event);

    CARAPI SetEvent(
        /* [in] */ Int32 event);

    CARAPI GetUpstreamActivity(
        /* [out] */ Int64* upstreamActivity);

    CARAPI SetUpstreamActivity(
        /* [in] */ Int64 upstreamActivity);

    CARAPI GetDownstreamActivity(
        /* [out] */ Int64* downstreamActivity);

    CARAPI SetDownstreamActivity(
        /* [in] */ Int64 downstreamActivity);

    CARAPI GetMesg(
        /* [out] */ String* mesg);

    CARAPI SetMesg(
        /* [in] */ const String& mesg);

    CARAPI GetInitialization(
        /* [out] */ Boolean* initialization);

    CARAPI SetInitialization(
        /* [in] */ Boolean initialization);

private:
    Int32 mAuthorityId;
    Int32 mHistoryId;
    Int64 mEventTime;
    Int64 mElapsedTime;
    Int32 mSource;
    Int32 mEvent;
    Int64 mUpstreamActivity;
    Int64 mDownstreamActivity;
    String mMesg;
    Boolean mInitialization;

};

}
}
}

#endif // __CSYNCSTORAGEENGINESYNCHISTORYITEM_H__
