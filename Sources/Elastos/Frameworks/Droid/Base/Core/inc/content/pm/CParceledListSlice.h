
#ifndef __ELASTOS_DROID_CONTENT_PM_CPARCELEDLISTSLICE_H__
#define __ELASTOS_DROID_CONTENT_PM_CPARCELEDLISTSLICE_H__

#include "_CParceledListSlice.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {


/**
 * Builds up a parcel that is discarded when written to another parcel or
 * written to a list. This is useful for API that sends huge lists across a
 * Binder that may be larger than the IPC limit.
 *
 * @hide
 */
CarClass(CParceledListSlice)
{
public:
    CParceledListSlice();

    ~CParceledListSlice();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* p,
        /* [in] */ Int32 numItems,
        /* [in] */ Boolean lastSlice);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Write this to another Parcel. Note that this discards the internal Parcel
     * and should not be used anymore. This is so we can pass this to a Binder
     * where we won't have a chance to call recycle on this.
     */
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI Append(
        /* [in] */ IParcelable* item,
        /* [out] */ Boolean* result);

    CARAPI PopulateList(
        /* [in, out] */ IObjectContainer* list,
        /* [in] */ const ClassID& clsid,
        /* [out] */ IInterface** obj);

    CARAPI SetLastSlice(
        /* [in] */ Boolean lastSlice);

    CARAPI IsLastSlice(
        /* [out] */ Boolean* result);

private:
    /*
     * TODO get this number from somewhere else. For now set it to a quarter of
     * the 1MB limit.
     */
    static const Int32 MAX_IPC_SIZE = 256 * 1024;

    AutoPtr<IParcel> mParcel;

    Int32 mNumItems;

    Boolean mIsLastSlice;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CPARCELEDLISTSLICE_H__
