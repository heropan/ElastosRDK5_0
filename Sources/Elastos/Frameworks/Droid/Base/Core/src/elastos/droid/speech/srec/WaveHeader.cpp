#include "elastos/droid/speech/srec/WaveHeader.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

const String WaveHeader::TAG("WaveHeader");
const Int32 WaveHeader::HEADER_LENGTH = 44;

WaveHeader::WaveHeader()
{
    constructor(0, 0, 0, 0, 0);
}

WaveHeader::~WaveHeader()
{}

ECode WaveHeader::constructor()
{
    return NOERROR;
}

ECode WaveHeader::constructor(
    /* [in] */ Int16 format,
    /* [in] */ Int16 numChannels,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int16 bitsPerSample,
    /* [in] */ Int32 numBytes)
{
    mFormat = format;
    mSampleRate = sampleRate;
    mNumChannels = numChannels;
    mBitsPerSample = bitsPerSample;
    mNumBytes = numBytes;

    return NOERROR;
}

Int16 WaveHeader::GetFormat(
    /* [out] */ Int16* ret)
{
    *ret = mFormat;
    return NOERROR;
}

ECode WaveHeader::SetFormat(
    /* [in] */ Int16 format)
{
    mFormat = format;
    return NOERROR;
}

ECode WaveHeader::GetNumChannels(
    /* [out] */ Int16* ret)
{
    *ret  = mNumChannels;
    return NOERROR;
}

ECode WaveHeader::SetNumChannels(
    /* [in] */ Int16 numChannels)
{
    mNumChannels = numChannels;
    return NOERROR;
}

ECode WaveHeader::GetSampleRate(
    /* [out] */ Int32* ret)
{
    *ret = mSampleRate;
    return NOERROR;
}

ECode WaveHeader::SetSampleRate(
    /* [in] */ Int32 sampleRate)
{
    mSampleRate = sampleRate;
    return NOERROR;
}

ECode WaveHeader::GetBitsPerSample(
    /* [out] */ Int16* ret)
{
    *ret = mBitsPerSample;
    return NOERROR;
}

ECode WaveHeader::SetBitsPerSample(
    /* [in] */ Int16 bitsPerSample)
{
    mBitsPerSample = bitsPerSample;
    return NOERROR;
}

ECode WaveHeader::GetNumBytes(
   /* [out] */ Int32* ret);
{
    *ret = mNumBytes;
    return NOERROR;
}

ECode WaveHeader::SetNumBytes(
    /* [in] */ Int32 numBytes)
{
    mNumBytes = numBytes;
    return NOERROR;
}

ECode WaveHeader::Read(
    /* [in] */ IInputStream* in,
    /* [out] */ Int32* ret)
{
    *ret = 0;

    /* RIFF header */
    ReadId(in, String("RIFF"));
    Int32 numBytes = ReadInt(in) - 36;
    ReadId(in, String("WAVE"));

    /* fmt chunk */
    ReadId(in, String("fmt "));
    if (16 != ReadInt(in)){
        //Java:    throw new IOException("fmt chunk length not 16");
        Logger::E(TAG, "IOException:fmt chunk length not 16\n");
        return E_IO_EXCEPTION;
    }
    mFormat = ReadShort(in);
    mNumChannels = ReadShort(in);
    mSampleRate = ReadInt(in);
    Int32 byteRate = ReadInt(in);
    Int16 blockAlign = ReadShort(in);
    mBitsPerSample = ReadShort(in);
    if (byteRate != mNumChannels * mSampleRate * mBitsPerSample / 8) {
        //Java:    throw new IOException("fmt.ByteRate field inconsistent");
        Logger::E(TAG, "IOException:fmt.ByteRate field inconsistent\n");
        return E_IO_EXCEPTION;
    }
    if (blockAlign != mNumChannels * mBitsPerSample / 8) {
        //Java:    throw new IOException("fmt.BlockAlign field inconsistent");
        Logger::E(TAG, "IOException:fmt.BlockAlign field inconsistent\n");
        return E_IO_EXCEPTION;
    }

    /* data chunk */
    ReadId(in, String("data"));
    mNumBytes = ReadInt(in);

    *ret = HEADER_LENGTH;
    return NOERROR;
}

ECode WaveHeader::ReadId(
    /* [in] */ IInputStream* in,
    /* [in] */ const String& id)
{
    AutoPtr<ArrayOf<Char32> > chars = id.GetChars();
    Int32 inR;
    for (Int32 i = 0; i < chars->GetLength(); i++) {
        if ((*chars)[i] != (Char32)(in->Read(&inR), inR)){
            //Java:    throw new IOException( id + " tag not present");
            Logger::E(TAG, String("IOException:") + id + String(" tag not present\n"));
            return E_IO_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode WaveHeader::ReadInt(
    /* [in] */ IInputStream* in,
    /* [out] */ Int32* ret);
{
    Int32 val;
    in->Read(&val);
    *ret = val | (val << 8) | (val << 16) | (val << 24);

    return NOERROR;
}

ECode WaveHeader::ReadShort(
    /* [in] */ IInputStream* in,
    /* [out] */ Int16* ret);
{
    Int32 val;
    in->Read(&val);
    *ret = (Int16)(val | (val << 8));

    return NOERROR;
}

ECode WaveHeader::Write(
    /* [in] */ IOutputStream* out,
    /* [out] */ Int32* ret)
{
    /* RIFF header */
    WriteId(out, String("RIFF"));
    WriteInt(out, 36 + mNumBytes);
    WriteId(out, String("WAVE"));

    /* fmt chunk */
    WriteId(out, String("fmt "));
    WriteInt(out, 16);
    WriteShort(out, mFormat);
    WriteShort(out, mNumChannels);
    WriteInt(out, mSampleRate);
    WriteInt(out, mNumChannels * mSampleRate * mBitsPerSample / 8);
    WriteShort(out, (Int16)(mNumChannels * mBitsPerSample / 8));
    WriteShort(out, mBitsPerSample);

    /* data chunk */
    WriteId(out, String("data"));
    WriteInt(out, mNumBytes);

    *ret = HEADER_LENGTH;
    return NOERROR;
}

ECode WaveHeader::WriteId(
    /* [in] */ IOutputStream* out,
    /* [in] */ const String& id)
{
    AutoPtr<ArrayOf<Char32> > chars = id.GetChars();
    for (Int32 i = 0; i < chars->GetLength(); i++){
        out->Write((*chars)[i]);
    }

    return NOERROR;
}

ECode WaveHeader::WriteInt(
    /* [in] */ IOutputStream* out,
    /* [in] */ Int32 val)
{
    out->Write(val >> 0);
    out->Write(val >> 8);
    out->Write(val >> 16);
    out->Write(val >> 24);

    return NOERROR;
}

ECode WaveHeader::WriteShort(
    /* [in] */ IOutputStream* out,
    /* [in] */ Int16 val)
{
    out->Write(val >> 0);
    out->Write(val >> 8);

    return NOERROR;
}

ECode WaveHeader::ToString(
    /* [out] */ String* str)
{
    String strOut;
    /*
    strOut = String.fromat(
            "WaveHeader format=%d numChannels=%d sampleRate=%d bitsPerSample=%d numBytes=%d",
            mFormat, mNumChannels, mSampleRate, mBitsPerSample, mNumBytes);
            */
    *ret = strOut;
    return NOERROR;
}

} // namespace Srec
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
