#include "elastos/droid/speech/srec/Recognizer.h"
#include "Elastos.Droid.Core_server.h"
#include <elastos/utility/logging/Logger.h>
#include <Elastos.CoreLibrary.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

String Recognizer::TAG("Recognizer");

/******************
 * Recognizer::RecognizerGrammar
 *******************************************************************************************************/

CAR_INTERFACE_IMPL(Recognizer::RecognizerGrammar, Object, IRecognizerGrammar);

Recognizer::RecognizerGrammar::RecognizerGrammar()
{}

Recognizer::RecognizerGrammar::~RecognizerGrammar()
{}

ECode Recognizer::RecognizerGrammar::constructor()
{
    return  NOERROR;
}

Recognizer::RecognizerGrammar::constructor(
    /* [in] */ const String& g2gFileName,
    /* [in] */ Recognizer* r)
{
    mR = r;
    mGrammar = mR->SR_GrammarLoad(g2gFileName);
    mR -> SR_GrammarSetupVocabulary(mGrammar, r->mVocabulary);

    return NOERROR;
}

void Recognizer::RecognizerGrammar::ResetAllSlots()
{
    mR->SR_GrammarResetAllSlots(mGrammar);
}

void Recognizer::RecognizerGrammar::AddWordToSlot(
    /* [in] */ const String& slot,
    /* [in] */ const String& word,
    /* [in] */ const String& pron,
    /* [in] */ Int32 weight,
    /* [in] */ const String& tag)
{
    mR->SR_GrammarAddWordToSlot(mGrammar, slot, word, pron, weight, tag);
}

void Recognizer::RecognizerGrammar::Compile()
{
    mR->SR_GrammarCompile(mGrammar);
}

void Recognizer::RecognizerGrammar::SetupRecognizer()
{
    mR->SR_GrammarSetupRecognizer(mGrammar, mR->mRecognizer);
    mR->mActiveGrammar = this;
}

void Recognizer::RecognizerGrammar::Save(
    /* [in] */ const String& g2gFileName)// throws IOException
{
    mR->SR_GrammarSave(mGrammar, g2gFileName);
}

void Recognizer::RecognizerGrammar::Destroy()
{
    // TODO: need to do cleanup and disassociation with Recognizer
    if (mGrammar != 0) {
        mR->SR_GrammarDestroy(mGrammar);
        mGrammar = 0;
    }
}

void Recognizer::RecognizerGrammar::Finalize()
{
    if (mGrammar != 0) {
        Destroy();
        //Java:    throw new IllegalStateException("someone forgot to destroy Grammar");
        Logger::E(TAG, String("IllegalStateException:someone forgot to destroy Grammar\n"));
        return;// E_ILLEGAL_STATE_EXCEPTION;
    }
}

/******************
 * Recognizer
 *******************************************************************************************************/

CAR_INTERFACE_IMPL(Recognizer, Object, IRecognizer);

Recognizer::Recognizer()
{}

Recognizer::~Recognizer()
{}

ECode Recognizer::constructor()
{}

void Recognizer::constructor(
    /* [in] */ const String& configFile)
{
    PMemInit();
    SR_SessionCreate(configFile);
    mRecognizer = SR_RecognizerCreate();
    SR_RecognizerSetup(mRecognizer);
    mVocabulary = SR_VocabularyLoad();
}

String Recognizer::GetConfigDir(
    /* [in] */ ILocale* locale)
{
    if (locale == NULL){
        //locale = CLocale::US;
        AutoPtr<ILocaleHelper> lh;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&lh);
        lh->GetUS(&locale);
    }
    //String dir = "/system/usr/srec/config/" + locale.toString().replace('_', '.').toLowerCase();
    String localeString;
    locale->ToString(&localeString);
    localeString = localeString.Replace('_', '.');
    localeString = localeString.ToLowerCase();
    String dir = String("/system/usr/srec/config/") + localeString;

    Boolean bIsDirectory;
    AutoPtr<IFile> fT;
    CFile::New(dir, (IFile**)&fT);
    fT -> IsDirectory(&bIsDirectory);

    if (bIsDirectory) {
        return dir;
    }
    return String(NULL);
}

ECode Recognizer::Start()
{
    // TODO: shouldn't be here?
    SR_RecognizerActivateRule(mRecognizer, mActiveGrammar->mGrammar, String("trash"), 1);
    SR_RecognizerStart(mRecognizer);
    return NOERROR;
}

Int32 Recognizer::Advance()
{
    return SR_RecognizerAdvance(mRecognizer);
}

Int32 Recognizer::PutAudio(
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isLast)
{
    return SR_RecognizerPutAudio(mRecognizer, buf, offset, length, isLast);
}

void Recognizer::PutAudio(
    /* [in] */ IInputStream* audio)// throws IOException
{
    // make sure the audio buffer is allocated
    if (mPutAudioBuffer == NULL){
        mPutAudioBuffer = ArrayOf<Byte>::Alloc(512);
    }
    // read some data
    Int32 nbytes;
    audio->ReadBytes(mPutAudioBuffer, &nbytes);
    // eof, so signal Recognizer
    if (nbytes == -1) {
        SR_RecognizerPutAudio(mRecognizer, mPutAudioBuffer, 0, 0, TRUE);
    }
    // put it into the Recognizer
    else if (nbytes != SR_RecognizerPutAudio(mRecognizer, mPutAudioBuffer, 0, nbytes, FALSE)) {
        //Java:    throw new IOException("SR_RecognizerPutAudio failed nbytes=" + nbytes);
        Logger::E(TAG, String("IOException:SR_RecognizerPutAudio failed nbytes=") + StringUtils::Int32ToString(nbytes) + String("\n"));
    }
}

Int32 Recognizer::GetResultCount()
{
    return SR_RecognizerResultGetSize(mRecognizer);
}

AutoPtr< ArrayOf<String> > Recognizer::GetResultKeys(
    /* [in] */ Int32 index)
{
    return SR_RecognizerResultGetKeyList(mRecognizer, index);
}

String Recognizer::GetResult(
    /* [in] */ Int32 index,
    /* [in] */ const String& key)
{
    return SR_RecognizerResultGetValue(mRecognizer, index, key);
}

ECode Recognizer::Stop()
{
    SR_RecognizerStop(mRecognizer);
    SR_RecognizerDeactivateRule(mRecognizer, mActiveGrammar->mGrammar, String("trash"));
    return NOERROR;
}

ECode Recognizer::ResetAcousticState()
{
    SR_AcousticStateReset(mRecognizer);
    return NOERROR;
}

ECode Recognizer::SetAcousticState(
    /* [in] */ const String& state)
{
    SR_AcousticStateSet(mRecognizer, state);
    return NOERROR;
}

String Recognizer::GetAcousticState()
{
    return SR_AcousticStateGet(mRecognizer);
}

ECode Recognizer::Destroy()
{
    //try {
        if (mVocabulary != 0){
            SR_VocabularyDestroy(mVocabulary);
        }
    //} finally {
        mVocabulary = 0;
        //try {
            if (mRecognizer != 0){
                SR_RecognizerUnsetup(mRecognizer);
            }
        //} finally {
            //try {
                if (mRecognizer != 0){
                    SR_RecognizerDestroy(mRecognizer);
                }
            //} finally {
                mRecognizer = 0;
                //try {
                    SR_SessionDestroy();
                //} finally {
                    PMemShutdown();
                //}
            //}
        //}
    //}
    return NOERROR;
}

void Recognizer::Finalize()// throws Throwable
{
    if (mVocabulary != 0 || mRecognizer != 0) {
        Destroy();
        //Java:    throw new IllegalStateException("someone forgot to destroy Recognizer");
        Logger::E(TAG, String("IllegalStateException:someone forgot to destroy Recognizer\n"));
    }
}

void Recognizer::PMemInit()
{}

void Recognizer::PMemShutdown()
{}

void Recognizer::SR_SessionCreate(
    /* [in] */ const String& filename)
{}

void Recognizer::SR_SessionDestroy()
{}

String Recognizer::EventToString(
    /* [in] */ Int32 event)
{
    switch (event) {
        case IRecognizer::EVENT_INVALID:
            return String("EVENT_INVALID");
        case IRecognizer::EVENT_NO_MATCH:
            return String("EVENT_NO_MATCH");
        case IRecognizer::EVENT_INCOMPLETE:
            return String("EVENT_INCOMPLETE");
        case IRecognizer::EVENT_STARTED:
            return String("EVENT_STARTED");
        case IRecognizer::EVENT_STOPPED:
            return String("EVENT_STOPPED");
        case IRecognizer::EVENT_START_OF_VOICING:
            return String("EVENT_START_OF_VOICING");
        case IRecognizer::EVENT_END_OF_VOICING:
            return String("EVENT_END_OF_VOICING");
        case IRecognizer::EVENT_SPOKE_TOO_SOON:
            return String("EVENT_SPOKE_TOO_SOON");
        case IRecognizer::EVENT_RECOGNITION_RESULT:
            return String("EVENT_RECOGNITION_RESULT");
        case IRecognizer::EVENT_START_OF_UTTERANCE_TIMEOUT:
            return String("EVENT_START_OF_UTTERANCE_TIMEOUT");
        case IRecognizer::EVENT_RECOGNITION_TIMEOUT:
            return String("EVENT_RECOGNITION_TIMEOUT");
        case IRecognizer::EVENT_NEED_MORE_AUDIO:
            return String("EVENT_NEED_MORE_AUDIO");
        case IRecognizer::EVENT_MAX_SPEECH:
            return String("EVENT_MAX_SPEECH");
    }
    return String("EVENT_") + StringUtils::Int32ToString(event);
}

void Recognizer::SR_RecognizerStart(
    /* [in] */ Int64 recognizer)
{}

void Recognizer::SR_RecognizerStop(
    /* [in] */ Int64 recognizer)
{}

Int32 Recognizer::SR_RecognizerCreate()
{
    return 0;
}

void Recognizer::SR_RecognizerDestroy(
    /* [in] */ Int64 recognizer)
{}

void Recognizer::SR_RecognizerSetup(
    /* [in] */ Int64 recognizer)
{}

void Recognizer::SR_RecognizerUnsetup(
    /* [in] */ Int64 recognizer)
{}

Boolean Recognizer::SR_RecognizerIsSetup(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

String Recognizer::SR_RecognizerGetParameter(
    /* [in] */ Int64 recognizer,
    /* [in] */ const String& key)
{
    return String("");
}

Int32 Recognizer::SR_RecognizerGetSize_tParameter(
    /* [in] */ Int64 recognizer,
    /* [in] */ const String& key)
{
    return 0;
}

Boolean Recognizer::SR_RecognizerGetBoolParameter(
    /* [in] */ Int64 recognizer,
    /* [in] */ const String& key)
{
    return FALSE;
}

void Recognizer::SR_RecognizerSetParameter(
    /* [in] */ Int64 recognizer,
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{}

void Recognizer::SR_RecognizerSetSize_tParameter(
    /* [in] */ Int64 recognizer,
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{}

void Recognizer::SR_RecognizerSetBoolParameter(
    /* [in] */ Int64 recognizer,
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{}

void Recognizer::SR_RecognizerSetupRule(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 grammar,
    /* [in] */ const String& ruleName)
{}

Boolean Recognizer::SR_RecognizerHasSetupRules(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

void Recognizer::SR_RecognizerActivateRule(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 grammar,
    /* [in] */ const String& ruleName,
    /* [in] */ Int32 weight)
{}

void Recognizer::SR_RecognizerDeactivateRule(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 grammar,
    /* [in] */ const String& ruleName)
{}

void Recognizer::SR_RecognizerDeactivateAllRules(
    /* [in] */ Int64 recognizer)
{}

Boolean Recognizer::SR_RecognizerIsActiveRule(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 grammar,
    /* [in] */ const String& ruleName)
{
    return FALSE;
}

Boolean Recognizer::SR_RecognizerCheckGrammarConsistency(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 grammar)
{
    return FALSE;
}

Int32 Recognizer::SR_RecognizerPutAudio(
    /* [in] */ Int64 recognizer,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isLast)
{
    return 0;
}

Int32 Recognizer::SR_RecognizerAdvance(
    /* [in] */ Int64 recognizer)
{
    return 0;
}

Boolean Recognizer::SR_RecognizerIsSignalClipping(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

Boolean Recognizer::SR_RecognizerIsSignalDCOffset(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

Boolean Recognizer::SR_RecognizerIsSignalNoisy(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

Boolean Recognizer::SR_RecognizerIsSignalTooQuiet(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

Boolean Recognizer::SR_RecognizerIsSignalTooFewSamples(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

Boolean Recognizer::SR_RecognizerIsSignalTooManySamples(
    /* [in] */ Int64 recognizer)
{
    return FALSE;
}

void Recognizer::SR_AcousticStateReset(
    /* [in] */ Int64 recognizer)
{}

void Recognizer::SR_AcousticStateSet(
    /* [in] */ Int64 recognizer,
    /* [in] */ const String& state)
{}

String Recognizer::SR_AcousticStateGet(
    /* [in] */ Int64 recognizer)
{
    return String("");
}

void Recognizer::SR_GrammarCompile(
    /* [in] */ Int32 grammar)
{}

void Recognizer::SR_GrammarAddWordToSlot(
    /* [in] */ Int32 grammar,
    /* [in] */ const String& slot,
    /* [in] */ const String& word,
    /* [in] */ const String& pronunciation,
    /* [in] */ Int32 weight,
    /* [in] */ const String& tag)
{}

void Recognizer::SR_GrammarResetAllSlots(
    /* [in] */ Int32 grammar)
{}

void Recognizer::SR_GrammarSetupVocabulary(
    /* [in] */ Int32 grammar,
    /* [in] */ Int32 vocabulary)
{}

void Recognizer::SR_GrammarSetupRecognizer(
    /* [in] */ Int32 grammar,
    /* [in] */ Int64 recognizer)
{}

void Recognizer::SR_GrammarUnsetupRecognizer(
    /* [in] */ Int32 grammar)
{}

Int32 Recognizer::SR_GrammarCreate()
{
    return 0;
}

void Recognizer::SR_GrammarDestroy(
    /* [in] */ Int32 grammar)
{}

Int32 Recognizer::SR_GrammarLoad(
    /* [in] */ const String& filename)
{
    return 0;
}

void Recognizer::SR_GrammarSave(
    /* [in] */ Int32 grammar,
    /* [in] */ const String& filename)
{}

void Recognizer::SR_GrammarAllowOnly(
    /* [in] */ Int32 grammar,
    /* [in] */ const String& transcription)
{}

void Recognizer::SR_GrammarAllowAll(
    /* [in] */ Int32 grammar)
{}

Int32 Recognizer::SR_VocabularyLoad()
{
    return 0;
}

void Recognizer::SR_VocabularyDestroy(
    /* [in] */ Int32 vocabulary)
{}

String Recognizer::SR_VocabularyGetPronunciation(
    /* [in] */ Int32 vocabulary,
    /* [in] */ const String& word)
{
    return String("");
}

AutoPtr< ArrayOf<Byte> > Recognizer::SR_RecognizerResultGetWaveform(
    /* [in] */ Int64 recognizer)
{
    return NULL;
}

Int32 Recognizer::SR_RecognizerResultGetSize(
    /* [in] */ Int64 recognizer)
{
    return 0;
}

Int32 Recognizer::SR_RecognizerResultGetKeyCount(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 nbest)
{
    return 0;
}

AutoPtr< ArrayOf<String> > Recognizer::SR_RecognizerResultGetKeyList(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 nbest)
{
    return NULL;
}

String Recognizer::SR_RecognizerResultGetValue(
    /* [in] */ Int64 recognizer,
    /* [in] */ Int32 nbest,
    /* [in] */ const String& key)
{
    return String("");
}

} // namespace Srec
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
