
#ifndef __ELASTOS_DROID_LOCATION_FUSEDBATCHOPTIONS_H__
#define __ELASTOS_DROID_LOCATION_FUSEDBATCHOPTIONS_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Location {

class FusedBatchOptions
    : public Object
    , public IFusedBatchOptions
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    FusedBatchOptions();

    CARAPI constructor();

    CARAPI SetMaxPowerAllocationInMW(
        /* [in] */ Double value);

    CARAPI GetMaxPowerAllocationInMW(
        /* [out] */ Double* result);

    CARAPI SetPeriodInNS(
        /* [in] */ Int64 value);

    CARAPI GetPeriodInNS(
        /* [out] */ Int64* result);

    CARAPI SetSourceToUse(
        /* [in] */ Int32 source);

    CARAPI ResetSourceToUse(
        /* [in] */ Int32 source);

    CARAPI IsSourceToUseSet(
        /* [in] */ Int32 source,
        /* [out] */ Boolean* result);

    CARAPI GetSourcesToUse(
        /* [out] */ Int32* result);

    CARAPI SetFlag(
        /* [in] */ Int32 flag);

    CARAPI ResetFlag(
        /* [in] */ Int32 flag);

    CARAPI IsFlagSet(
        /* [in] */ Int32 flag,
        /* [out] */ Boolean* result);

    CARAPI GetFlags(
        /* [out] */ Int32* result);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    Int64 mPeriodInNS;
    Int32 mSourcesToUse;
    Int32 mFlags;

    // the default value is set to request fixes at no cost
    Double mMaxPowerAllocationInMW;

public:
    class SourceTechnologies {
    public:
        static Int32 GNSS;// = 1<<0;
        static Int32 WIFI;// = 1<<1;
        static Int32 SENSORS;// = 1<<2;
        static Int32 CELL;// = 1<<3;
        static Int32 BLUETOOTH;// = 1<<4;
    };

    class BatchFlags {
    public:
        // follow the definitions to the letter in fused_location.h
        static Int32 WAKEUP_ON_FIFO_FULL;// = 0x0000001;
        static Int32 CALLBACK_ON_LOCATION_FIX;// =0x0000002;
    };
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_LOCATION_FUSEDBATCHOPTIONS_H__
