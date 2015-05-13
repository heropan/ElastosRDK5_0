
#include "ext/frameworkext.h"
#include "net/CLegacyVpnInfo.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CLegacyVpnInfo::CLegacyVpnInfo()
    : mState(-1)
{}

ECode CLegacyVpnInfo::constructor()
{
    return NOERROR;
}

ECode CLegacyVpnInfo::GetKey(
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key);
    *key = mKey;
    return NOERROR;
}

ECode CLegacyVpnInfo::SetKey(
    /* [in] */ const String& key)
{
    mKey = key;
    return NOERROR;
}

ECode CLegacyVpnInfo::GetState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;
    return NOERROR;
}

ECode CLegacyVpnInfo::SetState(
    /* [in] */ Int32 state)
{
    mState = state;
    return NOERROR;
}

ECode CLegacyVpnInfo::GetIntent(
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mIntent;
    INTERFACE_ADDREF(*intent);
    return NOERROR;
}

ECode CLegacyVpnInfo::SetIntent(
    /* [in] */ IPendingIntent* intent)
{
    mIntent = intent;
    return NOERROR;
}

ECode CLegacyVpnInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mKey);
    source->ReadInt32(&mState);
    mIntent = NULL;
    source->ReadInterfacePtr((Handle32*)&mIntent);
    return NOERROR;
}

ECode CLegacyVpnInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mKey);
    dest->WriteInt32(mState);
    dest->WriteInterfacePtr(mIntent);
    return NOERROR;
}

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos
