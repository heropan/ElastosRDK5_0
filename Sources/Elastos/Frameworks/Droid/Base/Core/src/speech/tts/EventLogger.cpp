#include "speech/tts/EventLogger.h"
#include "text/TextUtils.h"
#include "os/SystemClock.h"
#include <elastos/Logger.h>
#include <elastos/StringBuilder.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

EventLogger::EventLogger(
    /* [in] */ ISynthesisRequest* request,
    /* [in] */ Int32 callerUid,
    /* [in] */ Int32 callerPid,
    /* [in] */ const String& serviceApp)
{
    mRequest = request;
    mCallerUid = callerUid;
    mCallerPid = callerPid;
    mServiceApp = serviceApp;
    mReceivedTime = SystemClock::GetElapsedRealtime();

    mPlaybackStartTime = -1;
    mRequestProcessingStartTime = -1;
    mEngineStartTime = -1;
    mEngineCompleteTime = -1;
    mError = FALSE;
    mStopped = FALSE;
    mLogWritten = FALSE;
}

void EventLogger::OnRequestProcessingStart()
{
    mRequestProcessingStartTime = SystemClock::GetElapsedRealtime();
    return;
}

void EventLogger::OnEngineDataReceived()
{
    if (mEngineStartTime == -1) {
        mEngineStartTime = SystemClock::GetElapsedRealtime();
    }
    return;
}

void EventLogger::OnEngineComplete()
{
    mEngineCompleteTime = SystemClock::GetElapsedRealtime();
    return;
}

void EventLogger::OnAudioDataWritten()
{
    // For now, keep track of only the first chunk of audio
    // that was played.
    if (mPlaybackStartTime == -1) {
        mPlaybackStartTime = SystemClock::GetElapsedRealtime();
    }
    return;
}

void EventLogger::OnStopped()
{
    mStopped = FALSE;
    return;
}

void EventLogger::OnError()
{
    mError = TRUE;
    return;
}

void EventLogger::OnWriteData()
{
    if (mLogWritten) {
            return;
    } else {
        mLogWritten = TRUE;
    }

    Int64 completionTime = SystemClock::GetElapsedRealtime();
    // onAudioDataWritten() should normally always be called if an
    // error does not occur.
    if (mError || mPlaybackStartTime == -1 || mEngineCompleteTime == -1) {
        //Java:    EventLogTags.writeTtsSpeakFailure(mServiceApp, mCallerUid, mCallerPid, getUtteranceLength(), getLocaleString(), mRequest.getSpeechRate(), mRequest.getPitch());
        Logger::W("EventLogger", String("writeTtsSpeakFailure\n"));
        return;
    }

    // We don't report stopped syntheses because their overall
    // total time spent will be innacurate (will not correlate with
    // the length of the utterance).
    if (mStopped) {
        return;
    }

    Int64 audioLatency = mPlaybackStartTime - mReceivedTime;
    Int64 engineLatency = mEngineStartTime - mRequestProcessingStartTime;
    Int64 engineTotal = mEngineCompleteTime - mRequestProcessingStartTime;

    //Java:    EventLogTags.writeTtsSpeakSuccess(mServiceApp, mCallerUid, mCallerPid, getUtteranceLength(), getLocaleString(), mRequest.getSpeechRate(), mRequest.getPitch(), engineLatency, engineTotal, audioLatency);
    Logger::W("EventLogger", String("writeTtsSpeakSuccess\n"));
    return;
}

Int32 EventLogger::GetUtteranceLength()
{
    String utterance;
    mRequest->GetText(&utterance);
    return utterance.IsNull() ? 0 : utterance.GetLength();
}

String EventLogger::GetLocaleString()
{
    //Java:    StringBuilder sb = new StringBuilder(mRequest.getLanguage());
    String rLanguage;
    StringBuilder sb;
    sb.AppendString((mRequest->GetLanguage(&rLanguage), rLanguage));

    String rCountry;
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New((mRequest->GetCountry(&rCountry), rCountry), (ICharSequence**)&cs);

    if (!TextUtils::IsEmpty(cs)) {
        sb.AppendChar((Char32)"-");
        sb.AppendString(rCountry);

        String rVariant;
        mRequest->GetVariant(&rVariant);
        cs = NULL;
        CStringWrapper::New(rVariant, (ICharSequence**)&cs);
        if (!TextUtils::IsEmpty(cs)) {
            sb.AppendChar((Char32)"-");
            sb.AppendString(rVariant);
        }
    }
    return sb.ToString();
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
