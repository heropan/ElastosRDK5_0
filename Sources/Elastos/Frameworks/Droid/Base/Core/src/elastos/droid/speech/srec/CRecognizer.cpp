
#include "elastos/droid/speech/srec/CRecognizer.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

ECode CRecognizer::Start()
{
    Recognizer::Start();
    return NOERROR;
}

ECode CRecognizer::Advance(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Recognizer::Advance();
    return NOERROR;
}

ECode CRecognizer::PutAudio(
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isLast,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Recognizer::PutAudio(buf, offset, length, isLast);
    return NOERROR;
}

ECode CRecognizer::PutAudio(
    /* [in] */ IInputStream* audio)
{
    Recognizer::PutAudio(audio);
    return NOERROR;
}

ECode CRecognizer::GetResultCount(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Recognizer::GetResultCount();
    return NOERROR;
}

ECode CRecognizer::GetResultKeys(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<String>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<String> > o = Recognizer::GetResultKeys(index);
    *ret = o;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CRecognizer::GetResult(
    /* [in] */ Int32 index,
    /* [in] */ const String& key,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Recognizer::GetResult(index, key);
    return NOERROR;
}

ECode CRecognizer::Stop()
{
    return Recognizer::Stop();
}

ECode CRecognizer::ResetAcousticState()
{
    return Recognizer::ResetAcousticState();
}

ECode CRecognizer::SetAcousticState(
    /* [in] */ const String& state)
{
    return Recognizer::SetAcousticState(state);
}

ECode CRecognizer::GetAcousticState(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Recognizer::GetAcousticState();
    return NOERROR;
}

ECode CRecognizer::Destroy()
{
    return Recognizer::Destroy();
}

ECode CRecognizer::constructor(
    /* [in] */ const String& configFile)
{
    Init(configFile);
    return NOERROR;
}

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos
