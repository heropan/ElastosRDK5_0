
#ifndef __ELASTOS_DROID_HARDWARE_SOUNDTRIGGER_CSOUNDTRIGGERRECOGNITIONCONFIG_H__
#define __ELASTOS_DROID_HARDWARE_SOUNDTRIGGER_CSOUNDTRIGGERRECOGNITIONCONFIG_H__

#include "_Elastos_Droid_Hardware_Soundtrigger_CSoundTriggerRecognitionConfig.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Soundtrigger {

CarClass(CSoundTriggerRecognitionConfig)
    , public Object
    , public ISoundTriggerRecognitionConfig
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CSoundTriggerRecognitionConfig();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean captureRequested,
        /* [in] */ Boolean allowMultipleTriggers,
        /* [in] */ ArrayOf<ISoundTriggerKeyphraseRecognitionExtra*>* keyphrases,
        /* [in] */ ArrayOf<Byte>* data);

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* val);

    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    //@Override
    CARAPI ToString(
        /* [out] */ String* str);

    // public static final Parcelable.Creator<RecognitionConfig> CREATOR
    //         = new Parcelable.Creator<RecognitionConfig>() {
    //     public RecognitionConfig createFromParcel(Parcel in) {
    //         return RecognitionConfig.fromParcel(in);
    //     }

    //     public RecognitionConfig[] newArray(int size) {
    //         return new RecognitionConfig[size];
    //     }
    // };

    CARAPI GetCaptureRequested(
        /* [out] */ Boolean* requested);

    CARAPI GetAllowMultipleTriggers(
        /* [out] */ Boolean* result);

    CARAPI GetKeyphrases(
        /* [out, callee] */ ArrayOf<ISoundTriggerKeyphraseRecognitionExtra*>** phrases);

    CARAPI GetData(
        /* [out, callee] */ ArrayOf<Byte>** data);


private:
    static CARAPI FromParcel(
        /* [in] */ IParcel* source,
        /* [out] */ ISoundTriggerRecognitionConfig** result);

public:
    /** True if the DSP should capture the trigger sound and make it available for further
     * capture. */
    Boolean mCaptureRequested;
    /**
     * True if the service should restart listening after the DSP triggers.
     * Note: This config flag is currently used at the service layer rather than by the DSP.
     */
    Boolean mAllowMultipleTriggers;
    /** List of all keyphrases in the sound model for which recognition should be performed with
     * options for each keyphrase. */
    AutoPtr<ArrayOf<ISoundTriggerKeyphraseRecognitionExtra*> > mKeyphrases;
    /** Opaque data for use by system applications who know about voice engine internals,
     * typically during enrollment. */
    AutoPtr<ArrayOf<Byte> > mData;
};

} //Soundtrigger
} //Hardware
} //Droid
} //Elastos

#endif  //__ELASTOS_DROID_HARDWARE_SOUNDTRIGGER_CSOUNDTRIGGERRECOGNITIONCONFIG_H__
