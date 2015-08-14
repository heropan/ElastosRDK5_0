
#ifndef __ELASTOS_DROID_LOCATION_CGPSSTATUS_H__
#define __ELASTOS_DROID_LOCATION_CGPSSTATUS_H__

#include "ext/frameworkext.h"
#include "_CGpsStatus.h"

namespace Elastos {
namespace Droid {
namespace Location {

CarClass(CGpsStatus)
{
private:
    static const Int32 NUM_SATELLITES = 255;

public:
    CGpsStatus();

    CARAPI SetStatus(
        /* [in] */ Int32 svCount,
        /* [in] */ ArrayOf<Int32>* prns,
        /* [in] */ ArrayOf<Float>* snrs,
        /* [in] */ ArrayOf<Float>* elevations,
        /* [in] */ ArrayOf<Float>* azimuths,
        /* [in] */ Int32 ephemerisMask,
        /* [in] */ Int32 almanacMask,
        /* [in] */ Int32 usedInFixMask);

    CARAPI SetStatus(
        /* [in] */ IGpsStatus* status);

    CARAPI SetTimeToFirstFix(
        /* [in] */ Int32 ttff);

    CARAPI GetTimeToFirstFix(
        /* [out] */ Int32* time);

    CARAPI GetSatellites(
        /* [out] */ IObjectContainer** satellites);

    CARAPI GetMaxSatellites(
        /* [out] */ Int32* maxNumber);

    CARAPI constructor();

private:
    /* These package private values are modified by the LocationManager class */
    Int32 mTimeToFirstFix;
    AutoPtr<ArrayOf<IGpsSatellite*> > mSatellites;//GpsSatellite mSatellites[] = new GpsSatellite[NUM_SATELLITES];
};

}//namespace Location
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_LOCATION_CGPSSTATUS_H__
