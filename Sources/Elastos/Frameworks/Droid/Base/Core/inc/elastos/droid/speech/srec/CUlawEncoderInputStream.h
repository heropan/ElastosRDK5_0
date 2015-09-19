
#ifndef __ELASTOS_DROID_SPEECH_SREC_CULAWENCODERINPUTSTREAM_H__
#define __ELASTOS_DROID_SPEECH_SREC_CULAWENCODERINPUTSTREAM_H__

#include "_Elastos_Droid_Speech_Srec_CUlawEncoderInputStream.h"
#include "speech/srec/UlawEncoderInputStream.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

CarClass(CUlawEncoderInputStream)
    , public UlawEncoderInputStream
{
public:
    CARAPI Close();

    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IInputStream* in,
        /* [in] */ Int32 max);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

};

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_SREC_CULAWENCODERINPUTSTREAM_H__
