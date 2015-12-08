
#include "elastos/droid/speech/tts/CTextToSpeech.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

CAR_OBJECT_IMPL(CTextToSpeech);

CTextToSpeech::CTextToSpeech()
{}

CTextToSpeech::~CTextToSpeech()
{}

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

} // namespace Tts
} // namespace Speech
} // namespace Droid
} // namespace Elastos

