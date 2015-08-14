#ifndef __ELASTOS_DROID_SPEECH_TTS_FileSynthesisCallback_H__
#define __ELASTOS_DROID_SPEECH_TTS_FileSynthesisCallback_H__

#include "speech/tts/AbstractSynthesisCallback.h"
#include <Elastos.CoreLibrary.h>
//
using Elastos::IO::IFile;
using Elastos::IO::IRandomAccessFile;
namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Speech synthesis request that writes the audio to a WAV file.
 */
class FileSynthesisCallback
    : public AbstractSynthesisCallback
{
public:
    FileSynthesisCallback(
        /* [in] */ IFile* fileName);

    //@Override
    CARAPI_(void) Stop();

    //@Override
    CARAPI_(Int32) GetMaxBufferSize();

    //@Override
    CARAPI_(Boolean) IsDone();

    //@Override
    CARAPI_(Int32) Start(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 channelCount);

    //@Override
    CARAPI_(Int32) AudioAvailable(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    //@Override
    CARAPI_(Int32) Done();

    //@Override
    CARAPI Error();

private:
    /**
     * Must be called while holding the monitor on {@link #mStateLock}.
     */
    CARAPI_(void) CleanUp();

    /**
     * Must be called while holding the monitor on {@link #mStateLock}.
     */
    CARAPI_(void) CloseFileAndWidenPermissions();

    /**
     * Checks whether a given file exists, and deletes it if it does.
     */
    CARAPI_(Boolean) MaybeCleanupExistingFile(
        /* [in] */ IFile* file);

    CARAPI_(AutoPtr< ArrayOf<Byte> >) MakeWavHeader(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 channelCount,
        /* [in] */ Int32 dataLength);

private:
    static const CString TAG;// = "FileSynthesisRequest";
    static const Boolean DBG;// = false;

    static const Int32 MAX_AUDIO_BUFFER_SIZE;// = 8192;

    static const Int32 WAV_HEADER_LENGTH;// = 44;
    static const Int16 WAV_FORMAT_PCM;// = 0x0001;

    Object mStateLock;
    AutoPtr<IFile> mFileName;// = NULL;
    Int32 mSampleRateInHz;// = 0;
    Int32 mAudioFormat;// = 0;
    Int32 mChannelCount;// = 0;
    AutoPtr<IRandomAccessFile> mFile;// = NULL;
    Boolean mStopped;// = FALSE;
    Boolean mDone;// = FALSE;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_FileSynthesisCallback_H__
