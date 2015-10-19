
#include "elastos/droid/net/UidRange.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(UidRange, Object, IParcelable, IUidRange)

ECode UidRange::constructor(
    /* [in] */ Int32 startUid,
    /* [in] */ Int32 stopUid)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (startUid < 0) throw new IllegalArgumentException("Invalid start UID.");
        if (stopUid < 0) throw new IllegalArgumentException("Invalid stop UID.");
        if (startUid > stopUid) throw new IllegalArgumentException("Invalid UID range.");
        start = startUid;
        stop  = stopUid;

#endif
}

ECode UidRange::CreateForUser(
    /* [in] */ Int32 userId,
    /* [out] */ IUidRange** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return new UidRange(userId * PER_USER_RANGE, (userId + 1) * PER_USER_RANGE - 1);

#endif
}

ECode UidRange::GetStartUser(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return start / PER_USER_RANGE;

#endif
}

ECode UidRange::HashCode(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        int result = 17;
        result = 31 * result + start;
        result = 31 * result + stop;
        return result;

#endif
}

ECode UidRange::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this == o) {
            return true;
        }
        if (o instanceof UidRange) {
            UidRange other = (UidRange) o;
            return start == other.start && stop == other.stop;
        }
        return false;

#endif
}

ECode UidRange::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return start + "-" + stop;

#endif
}

ECode UidRange::DescribeContents(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return 0;

#endif
}

ECode UidRange::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        dest.writeInt(start);
        dest.writeInt(stop);

#endif
}

ECode UidRange::ReadFromParcel(
        /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            @Override
            public UidRange createFromParcel(Parcel in) {
                int start = in.readInt();
                int stop = in.readInt();
                return new UidRange(start, stop);
            }
            @Override
            public UidRange[] newArray(int size) {
                return new UidRange[size];
            }

#endif
}

ECode UidRange::WriteToParcel(
        /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            @Override
            public UidRange createFromParcel(Parcel in) {
                int start = in.readInt();
                int stop = in.readInt();
                return new UidRange(start, stop);
            }
            @Override
            public UidRange[] newArray(int size) {
                return new UidRange[size];
            }

#endif
}

ECode UidRange::GetStart(
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mStart;
    return NOERROR;
}

ECode UidRange::GetStop(
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mStop;
    return NOERROR;
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
