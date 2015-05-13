#include "speech/srec/WaveHeader.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

const CString WaveHeader::TAG = "WaveHeader";
const Int32 WaveHeader::HEADER_LENGTH = 44;

WaveHeader::WaveHeader()
{
    Init(0, 0, 0, 0, 0);
}

WaveHeader::WaveHeader(
    /* [in] */ Int16 format,
    /* [in] */ Int16 numChannels,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int16 bitsPerSample,
    /* [in] */ Int32 numBytes)
{
    Init(format, numChannels, sampleRate, bitsPerSample, numBytes);
}

void WaveHeader::Init(
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
}

Int16 WaveHeader::GetFormat()
{
    return mFormat;
}

ECode WaveHeader::SetFormat(
    /* [in] */ Int16 format)
{
    mFormat = format;
    return NOERROR;
}

Int16 WaveHeader::GetNumChannels()
{
    return mNumChannels;
}

ECode WaveHeader::SetNumChannels(
    /* [in] */ Int16 numChannels)
{
    mNumChannels = numChannels;
    return NOERROR;
}

Int32 WaveHeader::GetSampleRate()
{
    return mSampleRate;
}

ECode WaveHeader::SetSampleRate(
    /* [in] */ Int32 sampleRate)
{
    mSampleRate = sampleRate;
    return NOERROR;
}

Int16 WaveHeader::GetBitsPerSample()
{
    return mBitsPerSample;
}

ECode WaveHeader::SetBitsPerSample(
    /* [in] */ Int16 bitsPerSample)
{
    mBitsPerSample = bitsPerSample;
    return NOERROR;
}

Int32 WaveHeader::GetNumBytes()
{
    return mNumBytes;
}

ECode WaveHeader::SetNumBytes(
    /* [in] */ Int32 numBytes)
{
    mNumBytes = numBytes;
    return NOERROR;
}

Int32 WaveHeader::Read(
    /* [in] */ IInputStream* in)// throws IOException
{
    /* RIFF header */
    ReadId(in, String("RIFF"));
    Int32 numBytes = ReadInt(in) - 36;
    ReadId(in, String("WAVE"));

    /* fmt chunk */
    ReadId(in, String("fmt "));
    if (16 != ReadInt(in)){
        //Java:    throw new IOException("fmt chunk length not 16");
        Logger::E(TAG, String("IOException:fmt chunk length not 16\n"));
        return 0;// E_IO_EXCEPTION;
    }
    mFormat = ReadShort(in);
    mNumChannels = ReadShort(in);
    mSampleRate = ReadInt(in);
    Int32 byteRate = ReadInt(in);
    Int16 blockAlign = ReadShort(in);
    mBitsPerSample = ReadShort(in);
    if (byteRate != mNumChannels * mSampleRate * mBitsPerSample / 8) {
        //Java:    throw new IOException("fmt.ByteRate field inconsistent");
        Logger::E(TAG, String("IOException:fmt.ByteRate field inconsistent\n"));
        return 0;// E_IO_EXCEPTION;
    }
    if (blockAlign != mNumChannels * mBitsPerSample / 8) {
        //Java:    throw new IOException("fmt.BlockAlign field inconsistent");
        Logger::E(TAG, String("IOException:fmt.BlockAlign field inconsistent\n"));
        return 0;// E_IO_EXCEPTION;
    }

    /* data chunk */
    ReadId(in, String("data"));
    mNumBytes = ReadInt(in);

    return HEADER_LENGTH;
}

void WaveHeader::ReadId(
    /* [in] */ IInputStream* in,
    /* [in] */ const String& id)// throws IOException
{
    AutoPtr<ArrayOf<Char32> > chars = id.GetChars();
    Int32 inR;
    for (Int32 i = 0; i < chars->GetLength(); i++) {
        if ((*chars)[i] != (Char32)(in->Read(&inR), inR)){
            //Java:    throw new IOException( id + " tag not present");
            Logger::E(TAG, String("IOException:") + id + String(" tag not present\n"));
            return;// E_IO_EXCEPTION;
        }
    }
}

Int32 WaveHeader::ReadInt(
    /* [in] */ IInputStream* in)// throws IOException
{
    Int32 val;
    in->Read(&val);
    return val | (val << 8) | (val << 16) | (val << 24);
}

Int16 WaveHeader::ReadShort(
    /* [in] */ IInputStream* in)// throws IOException
{
    Int32 val;
    in->Read(&val);
    return (Int16)(val | (val << 8));
}

Int32 WaveHeader::Write(
    /* [in] */ IOutputStream* out)// throws IOException
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

    return HEADER_LENGTH;
}

void WaveHeader::WriteId(
    /* [in] */ IOutputStream* out,
    /* [in] */ const String& id)// throws IOException
{
    AutoPtr<ArrayOf<Char32> > chars = id.GetChars();
    for (Int32 i = 0; i < chars->GetLength(); i++){
        out->Write((*chars)[i]);
    }
}

void WaveHeader::WriteInt(
    /* [in] */ IOutputStream* out,
    /* [in] */ Int32 val)// throws IOException
{
    out->Write(val >> 0);
    out->Write(val >> 8);
    out->Write(val >> 16);
    out->Write(val >> 24);
}

void WaveHeader::WriteShort(
    /* [in] */ IOutputStream* out,
    /* [in] */ Int16 val)// throws IOException
{
    out->Write(val >> 0);
    out->Write(val >> 8);
}

String WaveHeader::ToString()
{
    String strOut;
    /*
    strOut = String.fromat(
            "WaveHeader format=%d numChannels=%d sampleRate=%d bitsPerSample=%d numBytes=%d",
            mFormat, mNumChannels, mSampleRate, mBitsPerSample, mNumBytes);
            */
    return strOut;
}


} // namespace Srec
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
