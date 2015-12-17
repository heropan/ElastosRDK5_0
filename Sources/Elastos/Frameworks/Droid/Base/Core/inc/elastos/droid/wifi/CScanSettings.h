
#ifndef __ELASTOS_DROID_NET_WIFI_CSCANSETTINGS_H__
#define __ELASTOS_DROID_NET_WIFI_CSCANSETTINGS_H__

#include <Elastos.CoreLibrary.Utility.h>
#include "_Elastos_Droid_Wifi_CScanSettings.h"
#include <elastos/core/Object.h>

using Elastos::Utility::ICollection;

namespace Elastos {
namespace Droid {
namespace Wifi {

CarClass(CScanSettings)
    , public Object
    , public IParcelable
    , public IScanSettings
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IScanSettings* source);

    CARAPI GetChannelSet(
        /* [out] */ ICollection** result);

    CARAPI SetChannelSet(
        /* [in] */ ICollection* channelSet);

    /** check for validity */
    CARAPI IsValid(
        /* [out] */ Boolean* result);

    /** implement Parcelable interface */
    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    /** implement Parcelable interface */
    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* out,
        /* [in] */ Int32 flags);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    /** implement Parcelable interface */
    // public static final Parcelable.Creator<ScanSettings> CREATOR =
    //         new Parcelable.Creator<ScanSettings>() {
    //     @Override
    //     public ScanSettings createFromParcel(Parcel in) {
    //         ScanSettings settings = new ScanSettings();
    //         int size = in.readInt();
    //         if (size > 0) {
    //             settings.channelSet = new ArrayList<WifiChannel>(size);
    //             while (size-- > 0)
    //                 settings.channelSet.add(WifiChannel.CREATOR.createFromParcel(in));
    //         }
    //         return settings;
    //     }

    //     @Override
    //     public ScanSettings[] newArray(int size) {
    //         return new ScanSettings[size];
    //     }
    // };

private:
    /** channel set to scan. this can be null or empty, indicating a full scan */
    AutoPtr<ICollection> mChannelSet;
};

} // namespace Wifi
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_NET_WIFI_CSCANSETTINGS_H__
