
#include "elastos/droid/location/CGpsStatus.h"
#include "elastos/droid/location/CGpsSatellite.h"



namespace Elastos {
namespace Droid {
namespace Location {

const Int32 CGpsStatus::NUM_SATELLITES;

CGpsStatus::CGpsStatus()
    : mTimeToFirstFix(0)
{
    mSatellites = ArrayOf<IGpsSatellite*>::Alloc(NUM_SATELLITES);
    for(Int32 i = 0; i < mSatellites->GetLength(); i++)
    {
        AutoPtr<IGpsSatellite> satellite;
        CGpsSatellite::New(i+1, (IGpsSatellite**)&satellite);
        mSatellites->Set(i, satellite);
    }
}

ECode CGpsStatus::SetStatus(
    /* [in] */ Int32 svCount,
    /* [in] */ ArrayOf<Int32>* prns,
    /* [in] */ ArrayOf<Float>* snrs,
    /* [in] */ ArrayOf<Float>* elevations,
    /* [in] */ ArrayOf<Float>* azimuths,
    /* [in] */ Int32 ephemerisMask,
    /* [in] */ Int32 almanacMask,
    /* [in] */ Int32 usedInFixMask)
{
    Int32 i;
    for (i = 0; i < mSatellites->GetLength(); i++) {
        (*mSatellites)[i]->SetValid(FALSE);
    }

    for (i = 0; i < svCount; i++) {
        Int32 prn = (*prns)[i] - 1;
        Int32 prnShift = (1 << prn);
        if (prn >= 0 && prn < mSatellites->GetLength()) {
            AutoPtr<IGpsSatellite> satellite = (*mSatellites)[prn];

            satellite->SetValid(TRUE);
            satellite->SetSnr((*snrs)[i]);
            satellite->SetElevation((*elevations)[i]);
            satellite->SetAzimuth((*azimuths)[i]);
            satellite->SetHasEphemeris((ephemerisMask & prnShift) != 0);
            satellite->SetHasAlmanac((almanacMask & prnShift) != 0);
            satellite->SetUsedInFix((usedInFixMask & prnShift) != 0);
        }
    }

    return NOERROR;
}

ECode CGpsStatus::SetStatus(
    /* [in] */ IGpsStatus* status)
{
    status->GetTimeToFirstFix(&mTimeToFirstFix);

    AutoPtr<CGpsStatus> statusSrc = (CGpsStatus*)status;
    for(Int32 i = 0; i < mSatellites->GetLength(); i++) {
        AutoPtr<IGpsSatellite> temp = (*mSatellites)[i];
        AutoPtr<CGpsSatellite> pcTemp = (CGpsSatellite*)(temp.Get());
        pcTemp->SetStatus((*(statusSrc->mSatellites))[i]);
    }
    return NOERROR;
}

ECode CGpsStatus::SetTimeToFirstFix(
    /* [in] */ Int32 ttff)
{
    mTimeToFirstFix = ttff;
    return NOERROR;
}

ECode CGpsStatus::GetTimeToFirstFix(
    /* [out] */ Int32* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mTimeToFirstFix;
    return NOERROR;
}

ECode CGpsStatus::GetSatellites(
    /* [out] */ IObjectContainer** satellites)
{
    VALIDATE_NOT_NULL(satellites);
    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    for(Int32 i = 0; i < mSatellites->GetLength(); i++)
    {
        Boolean valid;
        (*mSatellites)[i]->IsValid(&valid);
        if(valid)
        {
            AutoPtr<IGpsSatellite> temp = (*mSatellites)[i];
            container->Add(temp);
        }
    }
    *satellites = container;
    REFCOUNT_ADD(*satellites);

    return NOERROR;
}

ECode CGpsStatus::GetMaxSatellites(
    /* [out] */ Int32* maxNumber)
{
    VALIDATE_NOT_NULL(maxNumber);
    *maxNumber = NUM_SATELLITES;

    return NOERROR;
}

ECode CGpsStatus::constructor()
{
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
