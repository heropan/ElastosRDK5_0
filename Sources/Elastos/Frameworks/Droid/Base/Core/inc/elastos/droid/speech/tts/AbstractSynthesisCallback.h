#ifndef __ELASTOS_DROID_SPEECH_TTS_AbstractSynthesisCallback_H__
#define __ELASTOS_DROID_SPEECH_TTS_AbstractSynthesisCallback_H__

#include "elastos/droid/speech/tts/SynthesisCallback.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/**
 * Defines additional methods the synthesis callback must implement that
 * are private to the TTS service implementation.
 */
//abstract
class AbstractSynthesisCallback
    : public ElRefBase
    , public ISynthesisCallback
    , public SynthesisCallback
{
public:
    /**
     * Checks whether the synthesis request completed successfully.
     */
    virtual CARAPI_(Boolean) IsDone() = 0;

    /**
     * Aborts the speech request.
     *
     * Can be called from multiple threads.
     */
    virtual CARAPI_(void) Stop() = 0;

public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* Object,
        /* [out] */ InterfaceID* iID);

public:
    CARAPI GetMaxBufferSize(
        /* [out] */ Int32* ret);

    CARAPI Start(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 channelCount,
        /* [out] */ Int32* ret);

    CARAPI AudioAvailable(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* ret);

    CARAPI Done(
        /* [out] */ Int32* ret);

    CARAPI Error();
};

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SPEECH_TTS_AbstractSynthesisCallback_H__
