
#ifndef __CMICROPHONEINPUTSTREAM_H__
#define __CMICROPHONEINPUTSTREAM_H__

#include "_CMicrophoneInputStream.h"
#include "speech/srec/MicrophoneInputStream.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

CarClass(CMicrophoneInputStream)
    , public MicrophoneInputStream
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

    CARAPI ReadBytesEx(
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
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 fifoDepth);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CMICROPHONEINPUTSTREAM_H__
