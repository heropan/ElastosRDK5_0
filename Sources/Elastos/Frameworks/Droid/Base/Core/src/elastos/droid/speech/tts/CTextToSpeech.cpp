
#include "elastos/droid/speech/tts/CTextToSpeech.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;


namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

ECode CTextToSpeech::Shutdown()
{
    TextToSpeech::Shutdown();
    return NOERROR;
}

ECode CTextToSpeech::AddSpeech(
    /* [in] */ const String& text,
    /* [in] */ const String& packagename,
    /* [in] */ Int32 resourceId,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::AddSpeech(text, packagename, resourceId);
    return NOERROR;
}

ECode CTextToSpeech::AddSpeech(
    /* [in] */ const String& text,
    /* [in] */ const String& filename,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::AddSpeech(text, filename);
    return NOERROR;
}

ECode CTextToSpeech::AddEarcon(
    /* [in] */ const String& earcon,
    /* [in] */ const String& packagename,
    /* [in] */ Int32 resourceId,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::AddEarcon(earcon, packagename, resourceId);
    return NOERROR;
}

ECode CTextToSpeech::AddEarcon(
    /* [in] */ const String& earcon,
    /* [in] */ const String& filename,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::AddEarcon(earcon, filename);
    return NOERROR;
}

ECode CTextToSpeech::Speak(
    /* [in] */ const String& text,
    /* [in] */ Int32 queueMode,
    /* [in] */ IObjectStringMap* params,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::Speak(text, queueMode, params);
    return NOERROR;
}

ECode CTextToSpeech::PlayEarcon(
    /* [in] */ const String& earcon,
    /* [in] */ Int32 queueMode,
    /* [in] */ IObjectStringMap* params,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::PlayEarcon(earcon, queueMode, params);
    return NOERROR;
}

ECode CTextToSpeech::PlaySilence(
    /* [in] */ Int64 durationInMs,
    /* [in] */ Int32 queueMode,
    /* [in] */ IObjectStringMap* params,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::PlaySilence(durationInMs, queueMode, params);
    return NOERROR;
}

ECode CTextToSpeech::GetFeatures(
    /* [in] */ ILocale* locale,
    /* [out] */ IObjectContainer** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<Set<String> > sR = TextToSpeech::GetFeatures(locale);
    AutoPtr<IObjectContainer> oc;
    CObjectContainer::New((IObjectContainer**)&oc);
    String strTemp;
    Set<String>::Iterator iterCur = sR->Begin();
    for( ; iterCur != sR->End(); ++iterCur){
        strTemp = *iterCur;
        AutoPtr<ICharSequence> cs;
        CString::New(strTemp, (ICharSequence**)&cs);
        oc->Add(cs.Get());
    }
    *ret = oc;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTextToSpeech::IsSpeaking(
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::IsSpeaking();
    return NOERROR;
}

ECode CTextToSpeech::Stop(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::Stop();
    return NOERROR;
}

ECode CTextToSpeech::SetSpeechRate(
    /* [in] */ Float speechRate,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::SetSpeechRate(speechRate);
    return NOERROR;
}

ECode CTextToSpeech::SetPitch(
    /* [in] */ Float pitch,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::SetPitch(pitch);
    return NOERROR;
}

ECode CTextToSpeech::GetCurrentEngine(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::GetCurrentEngine();
    return NOERROR;
}

ECode CTextToSpeech::SetLanguage(
    /* [in] */ ILocale* loc,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::SetLanguage(loc);
    return NOERROR;
}

ECode CTextToSpeech::GetLanguage(
    /* [out] */ ILocale** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ILocale> l = TextToSpeech::GetLanguage();
    *ret = l;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTextToSpeech::IsLanguageAvailable(
    /* [in] */ ILocale* loc,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::IsLanguageAvailable(loc);
    return NOERROR;
}

ECode CTextToSpeech::SynthesizeToFile(
    /* [in] */ const String& text,
    /* [in] */ IObjectStringMap* params,
    /* [in] */ const String& filename,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::SynthesizeToFile(text, params, filename);
    return NOERROR;
}

ECode CTextToSpeech::SetOnUtteranceCompletedListener(
    /* [in] */ ITextToSpeechOnUtteranceCompletedListener* listener,
    /* [out] */ Int32* ret)
{
    *ret = TextToSpeech::SetOnUtteranceCompletedListener(listener);
    return NOERROR;
}

ECode CTextToSpeech::SetOnUtteranceProgressListener(
    /* [in] */ IUtteranceProgressListener* listener,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::SetOnUtteranceProgressListener(listener);
    return NOERROR;
}

ECode CTextToSpeech::SetEngineByPackageName(
    /* [in] */ const String& enginePackageName,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::SetEngineByPackageName(enginePackageName);
    return NOERROR;
}

ECode CTextToSpeech::GetDefaultEngine(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::GetDefaultEngine();
    return NOERROR;
}

ECode CTextToSpeech::AreDefaultsEnforced(
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextToSpeech::AreDefaultsEnforced();
    return NOERROR;
}

ECode CTextToSpeech::GetEngines(
    /* [out] */ IObjectContainer** ret)
{
    VALIDATE_NOT_NULL(ret);

    AutoPtr<List< AutoPtr<ITextToSpeechEngineInfo> > > lR = TextToSpeech::GetEngines();
    AutoPtr<IObjectContainer> oc;
    CObjectContainer::New((IObjectContainer**)&oc);
    AutoPtr<ITextToSpeechEngineInfo> temp;
    List< AutoPtr<ITextToSpeechEngineInfo> >::Iterator iterCur = lR->Begin();
    List< AutoPtr<ITextToSpeechEngineInfo> >::Iterator iterEnd = lR->End();
    for( ; iterCur != iterEnd; ++iterCur){
        temp = *iterCur;
        oc->Add((IInterface*)(temp.Get()));
    }
    *ret = oc;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTextToSpeech::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener)
{
    Init(context, listener);
    return NOERROR;
}

ECode CTextToSpeech::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener,
    /* [in] */ const String& engine)
{
    Init(context, listener, engine);
    return NOERROR;
}

ECode CTextToSpeech::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ITextToSpeechOnInitListener* listener,
    /* [in] */ const String& engine,
    /* [in] */ const String& packageName,
    /* [in] */ Boolean useFallback)
{
    Init(context, listener, engine, packageName, useFallback);
    return NOERROR;
}

}//namespace Tts
}//namespace Speech
}//namespace Droid
}//namespace Elastos

