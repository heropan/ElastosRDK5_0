#ifndef __ELASTOS_DROID_SPEECH_TTS_AudioPlaybackQueueItem_H__
#define __ELASTOS_DROID_SPEECH_TTS_AudioPlaybackQueueItem_H__

#include "speech/tts/PlaybackQueueItem.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IContext;
//using Elastos::Droid::media::IMediaPlayerOnErrorListener;
using Elastos::Droid::Net::IUri;
//using Elastos::Droid::Os::IConditionVariable;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

class AudioPlaybackQueueItem
    : public PlaybackQueueItem
    , public IRunnable
{
private:
    class MediaPlayerOnErrorListener
        : public ElRefBase
//        , public IMediaPlayerOnErrorListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* Object,
            /* [out] */ InterfaceID* iID);
    public:
        //@Override
        CARAPI OnError(
//            /* [in] */ IMediaPlayer* mp,
            /* [in] */ Int32 what,
            /* [in] */ Int32 extra,
            /* [out] */ Boolean* ret);
    public:
        MediaPlayerOnErrorListener(
            /* [in] */ AudioPlaybackQueueItem* apqi);
    private:
        AutoPtr<AudioPlaybackQueueItem> mApqi;
    };

    friend class MediaPlayerOnErrorListener;

private:
    class MediaPlayerOnCompletionListener
        : public ElRefBase
//        , public IMediaPlayerOnCompletionListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* Object,
            /* [out] */ InterfaceID* iID);
    public:
        //@Override
        CARAPI OnCompletion(
//            /* [in] */ IMediaPlayer* mp
            );

    public:
        MediaPlayerOnCompletionListener(
            /* [in] */ AudioPlaybackQueueItem* apqi);
    private:
        AutoPtr<AudioPlaybackQueueItem> mApqi;
    };

    friend class MediaPlayerOnCompletionListener;

public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* Object,
        /* [out] */ InterfaceID* iID);

public:
    AudioPlaybackQueueItem(
        /* [in] */ ITextToSpeechServiceUtteranceProgressDispatcher* dispatcher,
        /* [in] */ IInterface* callerIdentity,
        /* [in] */ IContext* context,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 streamType);

    //@Override
    CARAPI Run();

private:
    CARAPI_(void) Finish();

    //@Override
    CARAPI Stop(
        /* [in] */ Boolean isError);

private:
    static const CString TAG;// = "TTS.AudioQueueItem";

    AutoPtr<IContext> mContext;
    AutoPtr<IUri> mUri;
    Int32 mStreamType;

//    AutoPtr<IConditionVariable> mDone;
//    AutoPtr<IMediaPlayer> mPlayer;
    volatile Boolean mFinished;
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_AudioPlaybackQueueItem_H__
