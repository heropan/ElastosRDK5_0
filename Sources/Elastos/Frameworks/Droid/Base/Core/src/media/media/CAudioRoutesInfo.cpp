
#include "media/CAudioRoutesInfo.h"
#include "text/TextUtils.h"

using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Media {

const Int32 CAudioRoutesInfo::MAIN_SPEAKER = 0;
const Int32 CAudioRoutesInfo::MAIN_HEADSET = 1<<0;
const Int32 CAudioRoutesInfo::MAIN_HEADPHONES = 1<<1;
const Int32 CAudioRoutesInfo::MAIN_DOCK_SPEAKERS = 1<<2;
const Int32 CAudioRoutesInfo::MAIN_HDMI = 1<<3;

CAudioRoutesInfo::CAudioRoutesInfo()
    : mMainType(MAIN_SPEAKER)
{}

CAudioRoutesInfo::~CAudioRoutesInfo()
{}

ECode CAudioRoutesInfo::constructor()
{
    return NOERROR;
}

ECode CAudioRoutesInfo::constructor(
    /* [in] */ IAudioRoutesInfo* o)
{
    VALIDATE_NOT_NULL(o);
    o->GetBluetoothName((ICharSequence**)&mBluetoothName);
    o->GetMainType(&mMainType);
    return NOERROR;
}

ECode CAudioRoutesInfo::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    mBluetoothName = NULL;
    FAIL_RETURN(TextUtils::CHAR_SEQUENCE_CREATOR::CreateFromParcel(src, (ICharSequence**)&mBluetoothName));
    src->ReadInt32(&mMainType);
    return NOERROR;
}

ECode CAudioRoutesInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    TextUtils::WriteToParcel(mBluetoothName, dest);
    dest->WriteInt32(mMainType);
    return NOERROR;
}

ECode CAudioRoutesInfo::GetBluetoothName(
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mBluetoothName;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CAudioRoutesInfo::SetBluetoothName(
    /* [in] */ ICharSequence* bluetoothName)
{
    mBluetoothName = bluetoothName;
    return NOERROR;
}

ECode CAudioRoutesInfo::GetMainType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mMainType;
    return NOERROR;
}

ECode CAudioRoutesInfo::SetMainType(
    /* [in] */ Int32 mainType)
{
    mMainType = mainType;
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
