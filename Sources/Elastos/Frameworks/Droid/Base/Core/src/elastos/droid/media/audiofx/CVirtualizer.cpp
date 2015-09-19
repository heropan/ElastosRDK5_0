#include "media/audiofx/CVirtualizer.h"
#include "media/media/audiofx/CVirtualizerSettings.h"

using Elastos::Utility::IUUIDHelper;
using Elastos::Utility::CUUIDHelper;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

const String CVirtualizer::TAG("Virtualizer");

IAUDIOEFFECT_METHODS_IMPL(CVirtualizer, AudioEffect)

CAR_INTERFACE_IMPL(CVirtualizer::BaseParameterListener, IAudioEffectOnParameterChangeListener)

ECode CVirtualizer::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value)
{
    AutoPtr<IVirtualizerOnParameterChangeListener> l;
    {
        AutoLock lock(&mHost->mParamListenerLock);
        if (mHost->mParamListener != NULL) {
            l = mHost->mParamListener;
        }
    }
    if (l != NULL) {
        Int32 p = -1;
        Int16 v = -1;

        if (param->GetLength() == 4) {
            mHost->ByteArrayToInt32(param, 0, &p);
        }
        if (value->GetLength() == 2) {
            mHost->ByteArrayToInt16(value, 0, &v);
        }
        if (p != -1 && v != -1) {
            l->OnParameterChange((IVirtualizer*)mHost, status, p, v);
        }
    }
    return NOERROR;
}

CVirtualizer::CVirtualizer()
    : mStrengthSupported(FALSE)
{
}

ECode CVirtualizer::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    // super(EFFECT_TYPE_VIRTUALIZER, EFFECT_TYPE_NULL, priority, audioSession);
    AutoPtr<IUUIDHelper> helper;
    CUUIDHelper::AcquireSingleton((IUUIDHelper**)&helper);
    AutoPtr<IUUID> typeVIRTUALIZER;
    AutoPtr<IUUID> typeNULL;
    helper->FromString(IAudioEffect::EFFECT_TYPE_VIRTUALIZER, (IUUID**)&typeVIRTUALIZER);
    helper->FromString(IAudioEffect::EFFECT_TYPE_NULL, (IUUID**)&typeNULL);
    Init(typeVIRTUALIZER, typeNULL, priority, audioSession);

    if (audioSession == 0) {
        // Log.w(TAG, "WARNING: attaching a Virtualizer to global output mix is deprecated!");
    }

    AutoPtr<ArrayOf<Int32> > value = ArrayOf<Int32>::Alloc(1);
    Int32 status;
    GetParameter(IVirtualizer::PARAM_STRENGTH_SUPPORTED, value, &status);
    CheckStatus(status);
    mStrengthSupported = (value[0] != 0);
    return NOERROR;
}

PInterface CVirtualizer::Probe(
    /* [in] */ REIID riid)
{
    return _CVirtualizer::Probe(riid);
}

ECode CVirtualizer::GetStrengthSupported(
    /* [out] */ Boolean* strengthSupported)
{
    VALIDATE_NOT_NULL(strengthSupported);
    *strengthSupported = mStrengthSupported;
    return NOERROR;
}

ECode CVirtualizer::SetStrength(
    /* [in] */ Int16 strength)
{
    Int32 status;
    SetParameter(IVirtualizer::PARAM_STRENGTH, strength, &status);
    return CheckStatus(status);
}

ECode CVirtualizer::GetRoundedStrength(
    /* [out] */ Boolean* strength)
{
    VALIDATE_NOT_NULL(strength);

    AutoPtr<ArrayOf<Int16> > value = ArrayOf<Int16>::Alloc(1);
    Int32 status;
    GetParameter(IVirtualizer::PARAM_STRENGTH, value, &status);
    CheckStatus(status);
    *strength = (*value)[0];
    return NOERROR;
}

ECode CVirtualizer::SetParameterListener(
    /* [in] */ IVirtualizerOnParameterChangeListener* listener)
{
    AutoLock lock(&mParamListenerLock);
    if (mParamListener != NULL) {
        mParamListener = listener;
        mBaseParamListener = new BaseParameterListener(this);
        SetParameterListener(mBaseParamListener);
    }
    return NOERROR;
}

ECode CVirtualizer::GetProperties(
    /* [out] */ IVirtualizerSettings** properties)
{
    VALIDATE_NOT_NULL(properties);

    AutoPtr<IVirtualizerSettings> settings;
    CVirtualizerSettings::New((IVirtualizerSettings**)&settings);
    AutoPtr<ArrayOf<Int16> > value = ArrayOf<Int16>::Alloc(1);
    Int32 status;
    GetParameter(IVirtualizer::PARAM_STRENGTH, value, &status);
    CheckStatus(status);
    settings->SetStrength((*value)[0]);
    *properties = settings;
    REFCOUNT_ADD(*properties);
    return NOERROR;
}

ECode CVirtualizer::SetProperties(
    /* [in] */ IVirtualizerSettings* settings)
{
    Int16 strength;
    settings->GetStrength(&strength);
    Int32 status;
    SetParameter(IVirtualizer::PARAM_STRENGTH, strength, &status);
    return CheckStatus(status);
}

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
