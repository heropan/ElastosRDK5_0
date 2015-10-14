
#include "elastos/droid/net/RssiCurve.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(RssiCurve, Object, IParcelable, IRssiCurve)

RssiCurve::RssiCurve()
{}

ECode RssiCurve::constructor(
    /* [in] */ Int32 start,
    /* [in] */ Int32 bucketWidth,
    /* [in] */ ArrayOf<Byte>* rssiBuckets)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.start = start;
        this.bucketWidth = bucketWidth;
        if (rssiBuckets == null || rssiBuckets.length == 0) {
            throw new IllegalArgumentException("rssiBuckets must be at least one element large.");
        }
        this.rssiBuckets = rssiBuckets;

#endif
}

RssiCurve::RssiCurve(
    /* [in] */ IParcel* in)
{
#if 0 // TODO: Translate codes below
        start = in.readInt();
        bucketWidth = in.readInt();
        int bucketCount = in.readInt();
        rssiBuckets = new byte[bucketCount];
        in.readByteArray(rssiBuckets);

#endif
}

ECode RssiCurve::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode RssiCurve::WriteToParcel(
    /* [in] */ IParcel* out,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        out.writeInt(start);
        out.writeInt(bucketWidth);
        out.writeInt(rssiBuckets.length);
        out.writeByteArray(rssiBuckets);

#endif
}

ECode RssiCurve::LookupScore(
    /* [in] */ Int32 rssi,
    /* [out] */ Byte* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int index = (rssi - start) / bucketWidth;
        // Snap the index to the closest bucket if it falls outside the curve.
        if (index < 0) {
            index = 0;
        } else if (index > rssiBuckets.length - 1) {
            index = rssiBuckets.length - 1;
        }
        return rssiBuckets[index];

#endif
}

ECode RssiCurve::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        RssiCurve rssiCurve = (RssiCurve) o;
        return start == rssiCurve.start &&
                bucketWidth == rssiCurve.bucketWidth &&
                Arrays.equals(rssiBuckets, rssiCurve.rssiBuckets);

#endif
}

ECode RssiCurve::GetHashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Objects.hash(start, bucketWidth, rssiBuckets);

#endif
}

ECode RssiCurve::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        StringBuilder sb = new StringBuilder();
        sb.append("RssiCurve[start=")
                .append(start)
                .append(",bucketWidth=")
                .append(bucketWidth);
        sb.append(",buckets=");
        for (int i = 0; i < rssiBuckets.length; i++) {
            sb.append(rssiBuckets[i]);
            if (i < rssiBuckets.length - 1) {
                sb.append(",");
            }
        }
        sb.append("]");
        return sb.toString();

#endif
}

ECode RssiCurve::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public RssiCurve createFromParcel(Parcel in) {
                    return new RssiCurve(in);
                }
                @Override
                public RssiCurve[] newArray(int size) {
                    return new RssiCurve[size];
                }

#endif
}

ECode RssiCurve::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                @Override
                public RssiCurve createFromParcel(Parcel in) {
                    return new RssiCurve(in);
                }
                @Override
                public RssiCurve[] newArray(int size) {
                    return new RssiCurve[size];
                }

#endif
}

ECode RssiCurve::GetStart(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mStart;
    return NOERROR;
}

ECode RssiCurve::GetBucketWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mBucketWidth;
    return NOERROR;
}

ECode RssiCurve::GetRssiBuckets(
    /* [out] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result)

    *result = mRssiBuckets;
    REFCOUNT_ADD(*result)
    return NOERROR;
}



} // namespace Net
} // namespace Droid
} // namespace Elastos
