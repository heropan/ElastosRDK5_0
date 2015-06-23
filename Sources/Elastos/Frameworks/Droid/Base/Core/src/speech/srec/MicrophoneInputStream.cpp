#include "speech/srec/MicrophoneInputStream.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

const CString MicrophoneInputStream::TAG = "MicrophoneInputStream";

MicrophoneInputStream::MicrophoneInputStream()
{}

MicrophoneInputStream::MicrophoneInputStream(
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 fifoDepth) //Java:    throws IOException
{
    Init(sampleRate, fifoDepth);
}

ECode MicrophoneInputStream::Init(
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 fifoDepth) //Java:    throws IOException
{
    mOneByte = ArrayOf<Byte>::Alloc(1);
    mAudioRecord = AudioRecordNew(sampleRate, fifoDepth);
    if (mAudioRecord == 0){
        //Java:    throw new IOException("AudioRecord constructor failed - busy?");
        Logger::E(TAG, String("IOException:AudioRecord constructor failed - busy?\n"));
        return E_IO_EXCEPTION;
    }
    Int32 status = AudioRecordStart(mAudioRecord);
    if (status != 0) {
        Close();
        //Java:    throw new IOException("AudioRecord start failed: " + status);
        Logger::E(TAG, String("IOException:AudioRecord start failed: ") + StringUtils::Int32ToString(status) + String("\n") );
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode MicrophoneInputStream::Read(
    /* [out] */ Int32* value) //Java:    throws IOException
{
    if (mAudioRecord == 0){
        //Java:    throw new IllegalStateException("not open");
        Logger::E(TAG, String("IllegalStateException:not open\n"));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Int32 rtn = AudioRecordRead(mAudioRecord, mOneByte, 0, 1);
    *value = rtn == 1 ? ( (Int32)(*mOneByte)[0] ) & 0xff : -1;
    return NOERROR;
}

ECode MicrophoneInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* number) //Java:    throws IOException
{
    if (mAudioRecord == 0){
        //Java:    throw new IllegalStateException("not open");
        Logger::E(TAG, String("IllegalStateException:not open\n"));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *number = AudioRecordRead(mAudioRecord, b, 0, b->GetLength());
    return NOERROR;
}

ECode MicrophoneInputStream::ReadBytes(
        /* [out] */ ArrayOf<Byte>* b,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number) //Java:    throws IOException
{
    if (mAudioRecord == 0){
        //Java:    throw new IllegalStateException("not open");
        Logger::E(TAG, String("IllegalStateException:not open\n"));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // TODO: should we force all reads to be a multiple of the sample size?
    *number = AudioRecordRead(mAudioRecord, b, offset, length);
    return NOERROR;
}

ECode MicrophoneInputStream::Close()
{
    if (mAudioRecord != 0) {
        //try {
            AudioRecordStop(mAudioRecord);
        //} finally {
            //try {
                AudioRecordDelete(mAudioRecord);
            //} finally {
                mAudioRecord = 0;
            //}
        //}
    }
    return NOERROR;
}

void MicrophoneInputStream::Finalize()// throws Throwable
{
    if (mAudioRecord != 0) {
        Close();
        //throw new IOException("someone forgot to close MicrophoneInputStream");
        Logger::E(TAG, String("someone forgot to close MicrophoneInputStream\n") );
        return;//E_IO_EXCEPTION
    }
}

Int32 MicrophoneInputStream::AudioRecordNew(
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 fifoDepth)
{return 0;}

Int32 MicrophoneInputStream::AudioRecordStart(
    /* [in] */ Int32 audioRecord)
{return 0;}

Int32 MicrophoneInputStream::AudioRecordRead(
    /* [in] */ Int32 audioRecord,
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)// throws IOException
{return 0;}

void MicrophoneInputStream::AudioRecordStop(
    /* [in] */ Int32 audioRecord)// throws IOException
{}

void MicrophoneInputStream::AudioRecordDelete(
    /* [in] */ Int32 audioRecord)// throws IOException
{}

} // namespace Srec
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
