
#ifndef __ELASTOS_DROID_MEDIA_CCRYPTOINFO_H__
#define __ELASTOS_DROID_MEDIA_CCRYPTOINFO_H__

#include "_CMediaCodecBufferInfo.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaCodecBufferInfo)
{
public:
    CMediaCodecBufferInfo();

    CARAPI Set(
        /* [in] */ Int32 newOffset,
        /* [in] */ Int32 newSize,
        /* [in] */ Int64 newTimeUs,
        /* [in] */ Int32 newFlags);

    CARAPI SetOffset(
        /* [in] */ Int32 offset);

    CARAPI SetNewSize(
        /* [in] */ Int32 newSize);

    CARAPI SetPresentationTimeUs(
        /* [in] */ Int64 newTimeUs);

    CARAPI SetFlags(
        /* [in] */ Int32 flags);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI GetNewSize(
        /* [out] */ Int32* newSize);

    CARAPI GetPresentationTimeUs(
        /* [out] */ Int64* newTimeUs);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

private:
    Int32 mOffset;
    Int32 mSize;
    Int64 mPresentationTimeUs;
    Int32 mFlags;
};


} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIACODEC_H__
