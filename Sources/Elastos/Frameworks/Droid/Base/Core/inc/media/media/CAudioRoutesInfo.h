
#ifndef __ELASTOS_DROID_MEDIA_CAUDIOROUTESINFO_H__
#define __ELASTOS_DROID_MEDIA_CAUDIOROUTESINFO_H__

#include "_CAudioRoutesInfo.h"
#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioRoutesInfo)
{
public:
    class ParcelableCreatorAudioRoutesInfo
        : public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL();

        CARAPI CreateFromParcel(
            /* [in] */ IParcel* in,
            /* [out] */ IAudioRoutesInfo** result);

        CARAPI NewArray(
            /* [in] */ Int32 size,
            /* [out] */ ArrayOf<IAudioRoutesInfo*>** result);
    };

public:
    CAudioRoutesInfo();

    ~CAudioRoutesInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IAudioRoutesInfo* o);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* dest);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetBluetoothName(
        /* [out] */ ICharSequence** result);

    CARAPI SetBluetoothName(
        /* [in] */ ICharSequence* bluetoothName);

    CARAPI GetMainType(
        /* [out] */ Int32* result);

    CARAPI SetMainType(
        /* [in] */ Int32 mainType);

public:
    static const Int32 MAIN_SPEAKER;
    static const Int32 MAIN_HEADSET;
    static const Int32 MAIN_HEADPHONES;
    static const Int32 MAIN_DOCK_SPEAKERS;
    static const Int32 MAIN_HDMI;

private:
    AutoPtr<ICharSequence> mBluetoothName;
    Int32 mMainType; // = MAIN_SPEAKER;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CAUDIOROUTESINFO_H__
