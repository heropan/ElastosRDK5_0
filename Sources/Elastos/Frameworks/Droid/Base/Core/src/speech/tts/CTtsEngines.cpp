
#include "speech/tts/CTtsEngines.h"
#include "ext/frameworkext.h"

using Elastos::Core::CObjectContainer;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

ECode CTtsEngines::GetDefaultEngine(
    /* [out] */ String * pRet)
{
    VALIDATE_NOT_NULL(pRet);
    *pRet=TtsEngines::GetDefaultEngine();
    return NOERROR;
}

ECode CTtsEngines::GetHighestRankedEngineName(
    /* [out] */ String * pRet)
{
    VALIDATE_NOT_NULL(pRet);
    *pRet=TtsEngines::GetHighestRankedEngineName();
    return NOERROR;
}

ECode CTtsEngines::GetEngineInfo(
    /* [in] */ const String& packageName,
    /* [out] */ ITextToSpeechEngineInfo ** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);

    AutoPtr<ITextToSpeechEngineInfo> temp = TtsEngines::GetEngineInfo(packageName);
    *ppRet = temp;
    REFCOUNT_ADD(*ppRet);
    return NOERROR;
}

ECode CTtsEngines::GetEngines(
    /* [out] */ IObjectContainer ** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);

    AutoPtr<List<  AutoPtr<TextToSpeech::TextToSpeechEngineInfo> > > lR = TtsEngines::GetEngines();
    AutoPtr<IObjectContainer> oc;
    CObjectContainer::New((IObjectContainer**)&oc);
    AutoPtr<TextToSpeech::TextToSpeechEngineInfo> temp;
    List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> >::Iterator iterCur = lR->Begin();
    List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> >::Iterator iterEnd = lR->End();
    for( ; iterCur != iterEnd; ++iterCur){
        temp = *iterCur;
        oc->Add((IInterface*)(temp.Get()));
    }

    *ppRet = oc;
    REFCOUNT_ADD(*ppRet);
    return NOERROR;
}

ECode CTtsEngines::IsEngineInstalled(
    /* [in] */ const String& engine,
    /* [out] */ Boolean * pRet)
{
    VALIDATE_NOT_NULL(pRet);
    *pRet=TtsEngines::IsEngineInstalled(engine);
    return NOERROR;
}

ECode CTtsEngines::GetSettingsIntent(
    /* [in] */ const String& engine,
    /* [out] */ IIntent ** ppRet)
{
    VALIDATE_NOT_NULL(ppRet);
    AutoPtr<IIntent> temp = TtsEngines::GetSettingsIntent(engine);
    *ppRet = temp;
    REFCOUNT_ADD(*ppRet);
    return NOERROR;
}

ECode CTtsEngines::GetLocalePrefForEngine(
    /* [in] */ const String& engineName,
    /* [out] */ String * pRet)
{
    VALIDATE_NOT_NULL(pRet);
    *pRet=TtsEngines::GetLocalePrefForEngine(engineName);
    return NOERROR;
}

ECode CTtsEngines::UpdateLocalePrefForEngine(
    /* [in] */ const String& name,
    /* [in] */ const String& newLocale)
{
    TtsEngines::UpdateLocalePrefForEngine(name, newLocale);
    return NOERROR;
}

ECode CTtsEngines::constructor(
    /* [in] */ IContext * pCtx)
{
    Init(pCtx);
    _m_syncLock.mMutex = (TtsEngines::mLock).mMutex;
    return NOERROR;
}

}//namespace Tts
}//namespace Speech
}//namespace Droid
}//namespace Elastos

