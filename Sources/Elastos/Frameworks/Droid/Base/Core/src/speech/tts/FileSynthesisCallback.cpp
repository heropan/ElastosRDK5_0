#include "speech/tts/FileSynthesisCallback.h"
#include "os/FileUtils.h"
#include "Elastos.Droid.Core_server.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::IO::CRandomAccessFile;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::FileUtils;


namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

const CString FileSynthesisCallback::TAG = "FileSynthesisRequest";
const Boolean FileSynthesisCallback::DBG = FALSE;

const Int32 FileSynthesisCallback::MAX_AUDIO_BUFFER_SIZE = 8192;

const Int32 FileSynthesisCallback::WAV_HEADER_LENGTH = 44;
const Int16 FileSynthesisCallback::WAV_FORMAT_PCM = 0x0001;


FileSynthesisCallback::FileSynthesisCallback(
    /* [in] */ IFile* fileName)
{
    mFileName = fileName;

    mSampleRateInHz = 0;
    mAudioFormat = 0;
    mChannelCount = 0;
    mStopped = FALSE;
    mDone = FALSE;
}

//@Override
void FileSynthesisCallback::Stop()
{
    AutoLock lock(mStateLock);
    mStopped = TRUE;
    CleanUp();
}

void FileSynthesisCallback::CleanUp()
{
    CloseFileAndWidenPermissions();
    if (mFile != NULL) {
        Boolean bDelete;
        mFileName->Delete(&bDelete);
    }
}

void FileSynthesisCallback::CloseFileAndWidenPermissions()
{
    //try {
        if (mFile != NULL) {
            mFile->Close();
            mFile = NULL;
        }
    //} catch (IOException ex) {
        //Java:    Log.e(TAG, "Failed to close " + mFileName + ": " + ex);
        /*
        String strPath;
        mFileName->GetAbsolutePath(&strPath);
        Logger::E(TAG, String("Failed to close ") + strPath + String("\n"));
        */
    //}

    //try {
        // Make the written file readable and writeable by everyone.
        // This allows the app that requested synthesis to read the file.
        //
        // Note that the directory this file was written must have already
        // been world writeable in order it to have been
        // written to in the first place.
        String absolutePath;
        FileUtils::SetPermissions((mFileName->GetAbsolutePath(&absolutePath), absolutePath), 0666, -1, -1); //-rw-rw-rw
    //} catch (SecurityException se) {
        //Java:    Log.e(TAG, "Security exception setting rw permissions on : " + mFileName);
        /*
        String strPath;
        mFileName->GetAbsolutePath(&strPath);
        Logger::E(TAG, String("Security exception setting rw permissions on : ") + strPath + String("\n"));
        */
    //}
}

Boolean FileSynthesisCallback::MaybeCleanupExistingFile(
    /* [in] */ IFile* file)
{
    Boolean bExists;
    if ((file->Exists(&bExists), bExists)) {
        //Java:    Log.v(TAG, "File " + file + " exists, deleting.");
        String strPath;
        Logger::V(TAG, String("File ") + (file->GetAbsolutePath(&strPath), strPath) + String(" exists, deleting.\n"));

        Boolean bDelete;
        if (!(file->Delete(&bDelete), bDelete)) {
            //Java:    Log.e(TAG, "Failed to delete " + file);
            Logger::E(TAG, String("Failed to delete ") + strPath + String("\n"));
            return FALSE;
        }
    }
    return TRUE;
}

Int32 FileSynthesisCallback::GetMaxBufferSize()
{
    return MAX_AUDIO_BUFFER_SIZE;
}

Boolean FileSynthesisCallback::IsDone()
{
    return mDone;
}

Int32 FileSynthesisCallback::Start(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 channelCount)
{
    if (DBG) {
        //Java:    Log.d(TAG, "FileSynthesisRequest.start(" + sampleRateInHz + "," + audioFormat + "," + channelCount + ")");
        String strOut = String("FileSynthesisRequest.start(")+StringUtils::Int32ToString(sampleRateInHz)+String(",")+StringUtils::Int32ToString(audioFormat)+String(",")+StringUtils::Int32ToString(channelCount)+String(")\n");
        Logger::D(TAG, strOut);
    }
    AutoLock lock(mStateLock);
    if (mStopped) {
        if (DBG){
            //Java:    Log.d(TAG, "Request has been aborted.");
            Logger::D(TAG, String("Request has been aborted.\n"));
        }
        return ITextToSpeech::TTS_ERROR;
    }
    if (mFile != NULL) {
        CleanUp();
        //throw new IllegalArgumentException("FileSynthesisRequest.start() called twice");
        Logger::E(TAG, String("FileSynthesisRequest.start() called twice\n"));
        return 0;// E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!MaybeCleanupExistingFile(mFileName)) {
        return ITextToSpeech::TTS_ERROR;
    }

    mSampleRateInHz = sampleRateInHz;
    mAudioFormat = audioFormat;
    mChannelCount = channelCount;
    //try {
        mFile = NULL;
        CRandomAccessFile::New(mFileName.Get(), String("rw"), (IRandomAccessFile**)&mFile);
        // Reserve space for WAV header
        AutoPtr< ArrayOf<Byte> > aryB = ArrayOf<Byte>::Alloc(WAV_HEADER_LENGTH);
        mFile->WriteBytes(*aryB);
        return ITextToSpeech::TTS_SUCCESS;
    //} catch (IOException ex) {
        //Java:    Log.e(TAG, "Failed to open " + mFileName + ": " + ex);
        String strPath;
        mFileName->GetAbsolutePath(&strPath);
        Logger::E(TAG, String("Failed to open ")+strPath+String(": ")+String("\n"));
        CleanUp();
        return ITextToSpeech::TTS_ERROR;
    //}
}

Int32 FileSynthesisCallback::AudioAvailable(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (DBG) {
        //Java:    Log.d(TAG, "FileSynthesisRequest.audioAvailable(" + buffer + "," + offset + "," + length + ")");
        Logger::D(TAG, String("FileSynthesisRequest.audioAvailable\n"));
    }
    AutoLock lock(mStateLock);
    if (mStopped) {
        if (DBG){
            //Java:    Log.d(TAG, "Request has been aborted.");
            Logger::D(TAG, String("Request has been aborted.\n"));
        }
        return ITextToSpeech::TTS_ERROR;
    }
    if (mFile == NULL) {
        //Java:    Log.e(TAG, "File not open");
        Logger::E(TAG, String("File not open\n"));
        return ITextToSpeech::TTS_ERROR;
    }
    //try {
        mFile->WriteBytes(*buffer, offset, length);
        return ITextToSpeech::TTS_SUCCESS;
    //} catch (IOException ex) {
        //Java:    Log.e(TAG, "Failed to write to " + mFileName + ": " + ex);
        String strPath;
        Logger::E(TAG, String("Failed to write to ")+(mFileName->GetAbsolutePath(&strPath), strPath)+String(": ")+String("\n"));
        CleanUp();
        return ITextToSpeech::TTS_ERROR;
    //}
}

Int32 FileSynthesisCallback::Done()
{
    if (DBG){
        //Java:    Log.d(TAG, "FileSynthesisRequest.done()");
        Logger::D(TAG, String("FileSynthesisRequest.done()\n"));
    }
    AutoLock lock(mStateLock);
    if (mDone) {
        if (DBG){
            //Java:    Log.d(TAG, "Duplicate call to done()");
            Logger::D(TAG, String("Duplicate call to done()\n"));
        }
        // This preserves existing behaviour. Earlier, if done was called twice
        // we'd return ERROR because mFile == null and we'd add to logspam.
        return ITextToSpeech::TTS_ERROR;
    }
    if (mStopped) {
        if (DBG){
            //Java:    Log.d(TAG, "Request has been aborted.");
            Logger::D(TAG, String("Request has been aborted.\n"));
        }
        return ITextToSpeech::TTS_ERROR;
    }
    if (mFile == NULL) {
        //Java:    Log.e(TAG, "File not open");
        Logger::E(TAG, String("File not open\n"));
        return ITextToSpeech::TTS_ERROR;
    }
    //try {
        // Write WAV header at start of file
        mFile->Seek(0);
        Int64 fileLen;
        Int32 dataLength = (Int32) ((mFile->GetLength(&fileLen), fileLen) - WAV_HEADER_LENGTH);
        mFile->WriteBytes( *(MakeWavHeader(mSampleRateInHz, mAudioFormat, mChannelCount, dataLength).Get()));
        CloseFileAndWidenPermissions();
        mDone = TRUE;
        return ITextToSpeech::TTS_SUCCESS;
    //} catch (IOException ex) {
        //Java:    Log.e(TAG, "Failed to write to " + mFileName + ": " + ex);
        String strPath;
        Logger::E(TAG, String("Failed to write to ")+(mFileName->GetAbsolutePath(&strPath), strPath)+String(": \n"));
        CleanUp();
        return ITextToSpeech::TTS_ERROR;
    //}
}

//@Override
//public
ECode FileSynthesisCallback::Error()
{
    //Java:    if (DBG) Log.d(TAG, "FileSynthesisRequest.error()");
    Logger::D(TAG, String("FileSynthesisRequest.error()\n"));
    AutoLock lock(mStateLock);
    CleanUp();
    return NOERROR;
}

AutoPtr< ArrayOf<Byte> > FileSynthesisCallback::MakeWavHeader(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 channelCount,
    /* [in] */ Int32 dataLength)
{
    // TODO: is AudioFormat.ENCODING_DEFAULT always the same as ENCODING_PCM_16BIT?
    Int32 sampleSizeInBytes = (audioFormat == /*IAudioFormat::ENCODING_PCM_8BIT*/3 ? 1 : 2);
    Int32 byteRate = sampleRateInHz * sampleSizeInBytes * channelCount;
    Int16 blockAlign = (Int16) (sampleSizeInBytes * channelCount);
    Int16 bitsPerSample = (Int16) (sampleSizeInBytes * 8);

    //AutoPtr< ArrayOf<Byte> > headerBuf = ArrayOf<Byte>::Alloc(WAV_HEADER_LENGTH);

    AutoPtr<IByteBuffer> header;
    AutoPtr<IByteBufferHelper> bbh;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&bbh);
    bbh->Allocate(WAV_HEADER_LENGTH, (IByteBuffer**)&header);
    header->SetOrder(Elastos::IO::ByteOrder_LITTLE_ENDIAN);

    //Java:    header.put(new byte[]{ 'R', 'I', 'F', 'F' });
    AutoPtr< ArrayOf<Byte> > riff = ArrayOf<Byte>::Alloc(4);
    (*riff)[0] = 'R';    (*riff)[1] = 'I';    (*riff)[2] = 'F';    (*riff)[3] = 'F';
    header->PutBytes(*riff);

    //Java:    header.putInt(dataLength + WAV_HEADER_LENGTH - 8);  // RIFF chunk size
    header->PutInt32(dataLength + WAV_HEADER_LENGTH - 8);

    //Java:    header.put(new byte[]{ 'W', 'A', 'V', 'E' });
    AutoPtr< ArrayOf<Byte> > wave = ArrayOf<Byte>::Alloc(4);
    (*wave)[0] = 'W';    (*wave)[1] = 'A';    (*wave)[2] = 'V';    (*wave)[3] = 'E';
    header->PutBytes(*wave);

    //Java:    header.put(new byte[]{ 'f', 'm', 't', ' ' });
    AutoPtr< ArrayOf<Byte> > fmt = ArrayOf<Byte>::Alloc(4);
    (*fmt)[0] = 'f';    (*fmt)[1] = 'm';    (*fmt)[2] = 't';    (*fmt)[3] = ' ';
    header->PutBytes(*fmt);

    //Java:    header.putInt(16);  // size of fmt chunk
    header->PutInt32(16);

    //Java:    header.putShort(WAV_FORMAT_PCM);    header.putShort((short) channelCount);
    header->PutInt16(WAV_FORMAT_PCM);
    header->PutInt16((Int16) channelCount);

    //Java:    header.putInt(sampleRateInHz);    header.putInt(byteRate);
    header->PutInt32(sampleRateInHz);
    header->PutInt32(byteRate);

    //Java:    header.putShort(blockAlign);    header.putShort(bitsPerSample);
    header->PutInt16(blockAlign);
    header->PutInt16(bitsPerSample);

    //Java:    header.put(new byte[]{ 'd', 'a', 't', 'a' });
    AutoPtr< ArrayOf<Byte> > data = ArrayOf<Byte>::Alloc(4);
    (*data)[0] = 'd';    (*data)[1] = 'a';    (*data)[2] = 't';    (*data)[3] = 'a';
    header->PutBytes(*data);

    //Java:    header.putInt(dataLength);
    header->PutInt32(dataLength);

    AutoPtr< ArrayOf<Byte> > headerBuf;// = ArrayOf<Byte>::Alloc(WAV_HEADER_LENGTH);
    header->GetArray((ArrayOf<Byte>**)&headerBuf);

    return headerBuf;
}



} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
