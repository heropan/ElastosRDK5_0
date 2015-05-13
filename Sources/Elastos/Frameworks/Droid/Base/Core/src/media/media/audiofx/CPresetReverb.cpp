#include "media/audiofx/CPresetReverb.h"
#include "media/media/audiofx/CPresetReverbSettings.h"

using Elastos::Utility::IUUIDHelper;
using Elastos::Utility::CUUIDHelper;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

const String CPresetReverb::TAG("PresetReverb");

IAUDIOEFFECT_METHODS_IMPL(CPresetReverb, AudioEffect)

CAR_INTERFACE_IMPL(CPresetReverb::BaseParameterListener, IAudioEffectOnParameterChangeListener)

ECode CPresetReverb::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value)
{
    AutoPtr<IPresetReverbOnParameterChangeListener> l;
    {
        Mutex::Autolock lock(&mHost->mParamListenerLock);
        if (mHost->mParamListener != NULL) {
            l = mHost->mParamListener;
        }
    }
    if (l != NULL) {
        Int32 p = -1;
        Int16 v = -1;

        if (param->GetLength() == 4) {
            mHost->ByteArrayToInt32Ex(param, 0, &p);
        }
        if (value->GetLength() == 2) {
            mHost->ByteArrayToInt16Ex(value, 0, &v);
        }
        if (p != -1 && v != -1) {
            l->OnParameterChange((IPresetReverb*)mHost, status, p, v);
        }
    }
    return NOERROR;
}

CPresetReverb::CPresetReverb()
{
}

ECode CPresetReverb::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    // super(EFFECT_TYPE_PRESET_REVERB, EFFECT_TYPE_NULL, priority, audioSession);
    AutoPtr<IUUIDHelper> helper;
    CUUIDHelper::AcquireSingleton((IUUIDHelper**)&helper);
    AutoPtr<IUUID> typeREVERB;
    AutoPtr<IUUID> typeNULL;
    helper->FromString(IAudioEffect::EFFECT_TYPE_PRESET_REVERB, (IUUID**)&typeREVERB);
    helper->FromString(IAudioEffect::EFFECT_TYPE_NULL, (IUUID**)&typeNULL);
    Init(typeREVERB, typeNULL, priority, audioSession);
}

PInterface CPresetReverb::Probe(
    /* [in] */ REIID riid)
{
    return _CPresetReverb::Probe(riid);
}

ECode CPresetReverb::SetPreset(
    /* [in] */ Int16 preset)
{
    Int32 status;
    SetParameterEx2(IPresetReverb::PARAM_PRESET, preset, &status);
    return CheckStatus(status);
}

ECode CPresetReverb::GetPreset(
    /* [out] */ Int16* preset)
{
    VALIDATE_NOT_NULL(preset);

    AutoPtr<ArrayOf<Int16> > value = ArrayOf<Int16>::Alloc(1);
    Int32 status;
    GetParameterEx3(IPresetReverb::PARAM_PRESET, value, &status);
    CheckStatus(status);
    *preset = (*value)[0];
    return NOERROR;
}

ECode CPresetReverb::SetParameterListenerEx(
        /* [in] */ IPresetReverbOnParameterChangeListener* listener)
{
    Mutex::Autolock lock(&mParamListenerLock);
    if (mParamListener != NULL) {
        mParamListener = listener;
        mBaseParamListener = new BaseParameterListener(this);
        SetParameterListener(mBaseParamListener);
    }
    return NOERROR;
}

ECode CPresetReverb::GetProperties(
    /* [out] */ IPresetReverbSettings** properties)
{
    VALIDATE_NOT_NULL(properties);

    AutoPtr<IPresetReverbSettings> settings;
    CPresetReverbSettings::New((IPresetReverbSettings**)&settings);
    AutoPtr<ArrayOf<Int16> > value = ArrayOf<Int16>::Alloc(1);
    Int32 status;
    GetParameterEx3(IPresetReverb::PARAM_PRESET, value, &status);
    CheckStatus(status);
    settings->SetPreset((*value)[0]);
    *properties = settings;
    INTERFACE_ADDREF(*properties);
    return NOERROR;
}

ECode CPresetReverb::SetProperties(
    /* [in] */ IPresetReverbSettings* settings)
{
    Int16 preset;
    settings->GetPreset(&preset);
    Int32 status;
    SetParameterEx2(IPresetReverb::PARAM_PRESET, preset, &status);
    return CheckStatus(status);
}

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
