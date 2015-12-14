
#include "Elastos.Droid.Os.h"
#include "elastos/droid/service/notification/NotificationRankingUpdate.h"

namespace Elastos {
namespace Droid {
namespace Service {
namespace Notification {

CAR_INTERFACE_IMPL_2(NotificationRankingUpdate, Object, INotificationRankingUpdate, IParcelable)

NotificationRankingUpdate::NotificationRankingUpdate()
    : mFirstAmbientIndex(0)
{
}

ECode NotificationRankingUpdate::constructor(
    /* [in] */ ArrayOf<String>* keys,
    /* [in] */ ArrayOf<String>* interceptedKeys,
    /* [in] */ IBundle* visibilityOverrides,
    /* [in] */ Int32 firstAmbientIndex)
{
    mKeys = keys;
    mFirstAmbientIndex = firstAmbientIndex;
    mInterceptedKeys = interceptedKeys;
    mVisibilityOverrides = visibilityOverrides;
    return NOERROR;
}

ECode NotificationRankingUpdate::constructor(
    /* [in] */ IParcelable* in)
{
    // mKeys = in.readStringArray();
    // mFirstAmbientIndex = in.readInt();
    // mInterceptedKeys = in.readStringArray();
    // mVisibilityOverrides = in.readBundle();
    //TODO
    return NOERROR;
}

ECode NotificationRankingUpdate::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    //TODO
    return NOERROR;
}

ECode NotificationRankingUpdate::WriteToParcel(
    /* [in] */ IParcel* out)
{
    // out.writeStringArray(mKeys);
    out->WriteInt32(mFirstAmbientIndex);
    // out.writeStringArray(mInterceptedKeys);
    // out.writeBundle(mVisibilityOverrides);
    //TODO
    return NOERROR;
}


ECode NotificationRankingUpdate::GetOrderedKeys(
    /* [out, callee] */ ArrayOf<String>** orderedKeys)
{
    VALIDATE_NOT_NULL(orderedKeys)
    *orderedKeys = mKeys;
    REFCOUNT_ADD(*orderedKeys)
    return NOERROR;
}

ECode NotificationRankingUpdate::GetFirstAmbientIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = mFirstAmbientIndex;
    return NOERROR;
}

ECode NotificationRankingUpdate::GetInterceptedKeys(
     /* [out, callee] */ ArrayOf<String>** interceptedKeys)
{
    VALIDATE_NOT_NULL(interceptedKeys)
    *interceptedKeys = mInterceptedKeys;
    REFCOUNT_ADD(*interceptedKeys)
    return NOERROR;
}

ECode NotificationRankingUpdate::GetVisibilityOverrides(
    /* [out] */ IBundle** visibilityOverrides)
{
    VALIDATE_NOT_NULL(visibilityOverrides)
    *visibilityOverrides = mVisibilityOverrides;
    REFCOUNT_ADD(*visibilityOverrides)
    return NOERROR;
}

} // namespace Notification
} // namespace Service
} // namepsace Droid
} // namespace Elastos