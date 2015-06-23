#ifndef __CVIRTUALIZER_H__
#define __CVIRTUALIZER_H__

#include "_CVirtualizer.h"
#include "ext/frameworkext.h"
#include "media/media/audiofx/AudioEffect.h"


using Elastos::Core::Mutex;
using Elastos::Droid::Media::Audiofx::AudioEffect;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

/**
 * An audio virtualizer is a general name for an effect to spatialize audio channels. The exact
 * behavior of this effect is dependent on the number of audio input channels and the types and
 * number of audio output channels of the device. For example, in the case of a stereo input and
 * stereo headphone output, a stereo widening effect is used when this effect is turned on.
 * <p>An application creates a Virtualizer object to instantiate and control a virtualizer engine
 * in the audio framework.
 * <p>The methods, parameter types and units exposed by the Virtualizer implementation are directly
 * mapping those defined by the OpenSL ES 1.0.1 Specification (http://www.khronos.org/opensles/)
 * for the SLVirtualizerItf interface. Please refer to this specification for more details.
 * <p>To attach the Virtualizer to a particular AudioTrack or MediaPlayer, specify the audio session
 * ID of this AudioTrack or MediaPlayer when constructing the Virtualizer. If the audio session ID 0
 * is specified, the Virtualizer applies to the main audio output mix.
 * <p>Creating a Virtualizer on the output mix (audio session 0) requires permission
 * {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}
 * <p>See {@link android.media.MediaPlayer#getAudioSessionId()} for details on audio sessions.
 * <p>See {@link android.media.audiofx.AudioEffect} class for more details on controlling
 * audio effects.
 */

CarClass(CVirtualizer), public AudioEffect
{

private:
    /**
     * Listener used internally to receive unformatted parameter change events from AudioEffect
     * super class.
     */
    class BaseParameterListener
        : public ElRefBase
        , public IAudioEffectOnParameterChangeListener
    {
    public:
        BaseParameterListener(
           /* [in] */  CVirtualizer* host) : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnParameterChange(
            /* [in] */ IAudioEffect* effect,
            /* [in] */ Int32 status,
            /* [in] */ ArrayOf<Byte>* param,
            /* [in] */ ArrayOf<Byte>* value);

    private:
        CVirtualizer* mHost;
    };

public:
    IAUDIOEFFECT_METHODS_DECL()

    CVirtualizer();

    /**
     * Class constructor.
     * @param priority the priority level requested by the application for controlling the Virtualizer
     * engine. As the same engine can be shared by several applications, this parameter indicates
     * how much the requesting application needs control of effect parameters. The normal priority
     * is 0, above normal is a positive number, below normal a negative number.
     * @param audioSession  system wide unique audio session identifier. If audioSession
     *  is not 0, the Virtualizer will be attached to the MediaPlayer or AudioTrack in the
     *  same audio session. Otherwise, the Virtualizer will apply to the output mix.
     *
     * @throws java.lang.IllegalStateException
     * @throws java.lang.IllegalArgumentException
     * @throws java.lang.UnsupportedOperationException
     * @throws java.lang.RuntimeException
     */
    CARAPI constructor(
        /* [in] */ Int32 priority,
        /* [in] */ Int32 audioSession);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Indicates whether setting strength is supported. If this method returns false, only one
     * strength is supported and the setStrength() method always rounds to that value.
     * @return true is strength parameter is supported, false otherwise
     */
    CARAPI GetStrengthSupported(
        /* [out] */ Boolean* strengthSupported);

    /**
     * Sets the strength of the virtualizer effect. If the implementation does not support per mille
     * accuracy for setting the strength, it is allowed to round the given strength to the nearest
     * supported value. You can use the {@link #getRoundedStrength()} method to query the
     * (possibly rounded) value that was actually set.
     * @param strength strength of the effect. The valid range for strength strength is [0, 1000],
     * where 0 per mille designates the mildest effect and 1000 per mille designates the strongest.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetStrength(
        /* [in] */ Int16 strength);

    /**
     * Gets the current strength of the effect.
     * @return the strength of the effect. The valid range for strength is [0, 1000], where 0 per
     * mille designates the mildest effect and 1000 per mille the strongest
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetRoundedStrength(
        /* [out] */ Boolean* result);

    /**
     * Registers an OnParameterChangeListener interface.
     * @param listener OnParameterChangeListener interface registered
     */
    CARAPI SetParameterListener(
        /* [in] */ IVirtualizerOnParameterChangeListener* listener);

    /**
     * Gets the virtualizer properties. This method is useful when a snapshot of current
     * virtualizer settings must be saved by the application.
     * @return a Virtualizer.Settings object containing all current parameters values
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetProperties(
        /* [out] */ IVirtualizerSettings** properties);

    /**
     * Sets the virtualizer properties. This method is useful when virtualizer settings have to
     * be applied from a previous backup.
     * @param settings a Virtualizer.Settings object containing the properties to apply
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetProperties(
        /* [in] */ IVirtualizerSettings* settings);

private:
    static const String TAG;
    /**
     * Indicates if strength parameter is supported by the virtualizer engine
     */
    Boolean mStrengthSupported;

    /**
     * Registered listener for parameter changes.
     */
    AutoPtr<IVirtualizerOnParameterChangeListener> mParamListener;

    /**
     * Listener used internally to to receive raw parameter change event from AudioEffect super class
     */
    AutoPtr<BaseParameterListener> mBaseParamListener;

    /**
     * Lock for access to mParamListener
     */
    Mutex mParamListenerLock;
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__CVIRTUALIZER_H__
