
#include "speech/srec/CWaveHeader.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

ECode CWaveHeader::GetFormat(
    /* [out] */ Int16* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=WaveHeader::GetFormat();
    return NOERROR;
}

ECode CWaveHeader::SetFormat(
    /* [in] */ Int16 format,
    /* [out] */ IWaveHeader** ret)
{
    VALIDATE_NOT_NULL(ret);
    WaveHeader::SetFormat(format);
    AutoPtr<IWaveHeader> wh = (IWaveHeader*)this;
    *ret = wh;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CWaveHeader::GetNumChannels(
    /* [out] */ Int16* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = WaveHeader::GetNumChannels();
    return NOERROR;
}

ECode CWaveHeader::SetNumChannels(
    /* [in] */ Int16 numChannels,
    /* [out] */ IWaveHeader** ret)
{
    VALIDATE_NOT_NULL(ret);
    WaveHeader::SetNumChannels(numChannels);
    AutoPtr<IWaveHeader> wh = (IWaveHeader*)this;
    *ret = wh;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CWaveHeader::GetSampleRate(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = WaveHeader::GetSampleRate();
    return NOERROR;
}

ECode CWaveHeader::SetSampleRate(
    /* [in] */ Int32 sampleRate,
    /* [out] */ IWaveHeader** ret)
{
    VALIDATE_NOT_NULL(ret);
    WaveHeader::SetSampleRate(sampleRate);
    AutoPtr<IWaveHeader> wh = (IWaveHeader*)this;
    *ret = wh;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CWaveHeader::GetBitsPerSample(
    /* [out] */ Int16* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = WaveHeader::GetBitsPerSample();
    return NOERROR;
}

ECode CWaveHeader::SetBitsPerSample(
    /* [in] */ Int16 bitsPerSample,
    /* [out] */ IWaveHeader** ret)
{
    VALIDATE_NOT_NULL(ret);
    WaveHeader::SetBitsPerSample(bitsPerSample);
    AutoPtr<IWaveHeader> wh = (IWaveHeader*)this;
    *ret = wh;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CWaveHeader::GetNumBytes(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = WaveHeader::GetNumBytes();
    return NOERROR;
}

ECode CWaveHeader::SetNumBytes(
    /* [in] */ Int32 numBytes,
    /* [out] */ IWaveHeader** ret)
{
    VALIDATE_NOT_NULL(ret);
    WaveHeader::SetNumBytes(numBytes);
    AutoPtr<IWaveHeader> wh = (IWaveHeader*)this;
    *ret = wh;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CWaveHeader::Read(
    /* [in] */ IInputStream* in,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = WaveHeader::Read(in);
    return NOERROR;
}

ECode CWaveHeader::Write(
    /* [in] */ IOutputStream* out,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = WaveHeader::Write(out);
    return NOERROR;
}

ECode CWaveHeader::ToString(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = WaveHeader::ToString();
    return NOERROR;
}

ECode CWaveHeader::constructor()
{
    return NOERROR;
}

ECode CWaveHeader::constructor(
    /* [in] */ Int16 format,
    /* [in] */ Int16 numChannels,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int16 bitsPerSample,
    /* [in] */ Int32 numBytes)
{
    Init(format, numChannels, sampleRate, bitsPerSample, numBytes);
    return NOERROR;
}

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

