
#ifndef __CWAVEHEADER_H__
#define __CWAVEHEADER_H__

#include "_CWaveHeader.h"
#include "speech/srec/WaveHeader.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

CarClass(CWaveHeader) , public WaveHeader
{
public:
    CARAPI GetFormat(
        /* [out] */ Int16* ret);

    CARAPI SetFormat(
        /* [in] */ Int16 format,
        /* [out] */ IWaveHeader** ret);

    CARAPI GetNumChannels(
        /* [out] */ Int16* ret);

    CARAPI SetNumChannels(
        /* [in] */ Int16 numChannels,
        /* [out] */ IWaveHeader** ret);

    CARAPI GetSampleRate(
        /* [out] */ Int32* ret);

    CARAPI SetSampleRate(
        /* [in] */ Int32 sampleRate,
        /* [out] */ IWaveHeader** ret);

    CARAPI GetBitsPerSample(
        /* [out] */ Int16* ret);

    CARAPI SetBitsPerSample(
        /* [in] */ Int16 bitsPerSample,
        /* [out] */ IWaveHeader** ret);

    CARAPI GetNumBytes(
        /* [out] */ Int32* ret);

    CARAPI SetNumBytes(
        /* [in] */ Int32 numBytes,
        /* [out] */ IWaveHeader** ret);

    CARAPI Read(
        /* [in] */ IInputStream* in,
        /* [out] */ Int32* ret);

    CARAPI Write(
        /* [in] */ IOutputStream* out,
        /* [out] */ Int32* ret);

    CARAPI ToString(
        /* [out] */ String* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int16 format,
        /* [in] */ Int16 numChannels,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int16 bitsPerSample,
        /* [in] */ Int32 numBytes);

};

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CWAVEHEADER_H__
