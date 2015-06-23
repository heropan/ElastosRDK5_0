#include "speech/tts/TtsEngines.h"
#include "content/CIntent.h"
#include <elastos/utility/logging/Logger.h>
#include "util/Xml.h"
#include "content/pm/CResolveInfo.h"
#include "content/pm/CServiceInfo.h"
#include "text/TextUtils.h"
//#include "provider/Settings.h"
#include "speech/tts/TextToSpeech.h"
#include "R.h"

using Elastos::Core::IComparator;
using Elastos::Core::EIID_IComparator;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::CServiceInfo;
using Elastos::Droid::Content::Pm::CResolveInfo;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Text::TextUtils;

using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocale;
using Libcore::ICU::CLocaleHelper;
using Elastos::Core::CStringWrapper;
using Elastos::Core::Mutex;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/******************************TtsEngines::EngineInfoComparator*************************/
AutoPtr<TtsEngines::EngineInfoComparator> TtsEngines::EngineInfoComparator::INSTANCE = new TtsEngines::EngineInfoComparator();

PInterface TtsEngines::EngineInfoComparator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IComparator*)this;
    }
    else if (riid == EIID_IComparator) {
        return (IComparator*)this;
    }
    return NULL;
}

UInt32 TtsEngines::EngineInfoComparator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TtsEngines::EngineInfoComparator::Release()
{
    return ElRefBase::Release();
}

ECode TtsEngines::EngineInfoComparator::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(IComparator*)this) {
        *iID = EIID_IComparator;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

TtsEngines::EngineInfoComparator::EngineInfoComparator()
{
}

ECode TtsEngines::EngineInfoComparator::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* isEqual)
{return NOERROR;}

ECode TtsEngines::EngineInfoComparator::Compare(
    /* [in] */ IInterface* object1,
    /* [in] */ IInterface* object2,
    /* [out] */ Int32* result)
{
    AutoPtr<TextToSpeech::TextToSpeechEngineInfo> lhs = (TextToSpeech::TextToSpeechEngineInfo*)object1;
    AutoPtr<TextToSpeech::TextToSpeechEngineInfo> rhs = (TextToSpeech::TextToSpeechEngineInfo*)object2;
    if (lhs->system && !rhs->system) {
        *result = -1;
        return NOERROR;
    } else if (rhs->system && !lhs->system) {
        *result = 1;
        return NOERROR;
    } else {
        // Either both system engines, or both non system
        // engines.
        //
        // Note, this isn't a typo. Higher priority numbers imply
        // higher priority, but are "lower" in the sort order.
        *result = rhs->priority - lhs->priority;
        return NOERROR;
    }
}

Int32 TtsEngines::EngineInfoComparator::Comparator(
    /* [in] */ AutoPtr<TextToSpeech::TextToSpeechEngineInfo> lhs,
    /* [in] */ AutoPtr<TextToSpeech::TextToSpeechEngineInfo> rhs)
{
    Int32 returnVal;
    if (lhs->system && !rhs->system) {
        returnVal = -1;
    } else if (rhs->system && !lhs->system) {
        returnVal = 1;
    } else {
        returnVal = rhs->priority - lhs->priority;
    }
    return returnVal;
}

/******************************TtsEngines*************************/
const CString TtsEngines::TAG = "TtsEngines";
const Boolean TtsEngines::DBG = FALSE;

const CString TtsEngines::LOCALE_DELIMITER = "-";
const CString TtsEngines::XML_TAG_NAME = "tts-engine";


TtsEngines::TtsEngines()
{}

TtsEngines::TtsEngines(
    /* [in] */ IContext* ctx)
{
    Init(ctx);
}

void TtsEngines::Init(
    /* [in] */ IContext* ctx)
{
    mContext = ctx;
}

String TtsEngines::GetDefaultEngine()
{
    //Java:    String engine = getString(mContext.getContentResolver(), Settings.Secure.TTS_DEFAULT_SYNTH);
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    String engine;// = GetString(cr.Get(), /*ISettingsSecure::TTS_DEFAULT_SYNTH*/String("tts_default_synth"));
    return IsEngineInstalled(engine) ? engine : GetHighestRankedEngineName();
}

String TtsEngines::GetHighestRankedEngineName()
{
    AutoPtr< List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> > > engines = GetEngines();

    if (engines != NULL && !engines->IsEmpty() && ((*engines)[0])->system) {
        return (*engines)[0]->name;
    }
    return String(NULL);
}

AutoPtr<TextToSpeech::TextToSpeechEngineInfo> TtsEngines::GetEngineInfo(
    /* [in] */ const String& packageName)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IIntent> intent;
    CIntent::New(ITextToSpeechEngine::INTENT_ACTION_TTS_SERVICE, (IIntent**)&intent);
    intent->SetPackage(packageName);
    List< AutoPtr<IResolveInfo> > resolveInfos;
    AutoPtr<IObjectContainer> oc;
    pm->QueryIntentServices(intent.Get(),IPackageManager::MATCH_DEFAULT_ONLY, (IObjectContainer**)&oc);
    if(oc != NULL) {
        AutoPtr<IObjectEnumerator> it;
        oc->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean succeeded = FALSE;
        while(it->MoveNext(&succeeded), succeeded) {
            AutoPtr<IResolveInfo> ttsEi;
            it->Current((IInterface**)&ttsEi);

            resolveInfos.PushBack(ttsEi);

        }
    }
    // Note that the current API allows only one engine per
    // package name. Since the "engine name" is the same as
    // the package name.
    if (resolveInfos.IsEmpty() && resolveInfos.GetSize() == 1) {
        return GetEngineInfo(resolveInfos[0], pm.Get());
    }
    return NULL;
}

AutoPtr< List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> > > TtsEngines::GetEngines()
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IIntent> intent;
    CIntent::New(ITextToSpeechEngine::INTENT_ACTION_TTS_SERVICE, (IIntent**)&intent);
    List< AutoPtr<IResolveInfo> > resolveInfos;
    AutoPtr<IObjectContainer> oc;
    pm->QueryIntentServices(intent.Get(),IPackageManager::MATCH_DEFAULT_ONLY, (IObjectContainer**)&oc);
    if(oc != NULL) {
        AutoPtr<IObjectEnumerator> it;
        oc->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean succeeded = FALSE;
        while(it->MoveNext(&succeeded), succeeded) {
            AutoPtr<IResolveInfo> ttsEi;
            it->Current((IInterface**)&ttsEi);

            resolveInfos.PushBack(ttsEi);

        }
    }
    if (resolveInfos.IsEmpty()){
        AutoPtr<List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> > > enginesNull;
        return enginesNull;    //Java:     return Collections.emptyList();
    }

    AutoPtr< List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> > > engines;// = new ArrayList<EngineInfo>(resolveInfos.size());
    engines = new List< AutoPtr<TextToSpeech::TextToSpeechEngineInfo> >(resolveInfos.GetSize());
    Int32 resolveInfosSize = resolveInfos.GetSize();
    for (Int32 i = 0; i < resolveInfosSize; ++i)
    {
        AutoPtr<TextToSpeech::TextToSpeechEngineInfo> engine = GetEngineInfo((resolveInfos[i]).Get(), pm.Get());
        if(engine != NULL){
            engines->PushBack(engine);
        }
    }
    engines->Sort(EngineInfoComparator::Comparator);

    return engines;
}

Boolean TtsEngines::IsSystemEngine(
    /* [in] */ IServiceInfo* info)
{
    AutoPtr<IApplicationInfo> appInfo;
    ((CServiceInfo*)info)->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 flags;
    appInfo->GetFlags(&flags);
    return appInfo != NULL && (flags & IApplicationInfo::FLAG_SYSTEM) != 0;
}

Boolean TtsEngines::IsEngineInstalled(
    /* [in] */ const String& engine)
{
    if (engine.IsNullOrEmpty()) {
        return FALSE;
    }

    return GetEngineInfo(engine) != NULL;
}

AutoPtr<IIntent> TtsEngines::GetSettingsIntent(
    /* [in] */ const String& engine)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IIntent> intent;
    CIntent::New(ITextToSpeechEngine::INTENT_ACTION_TTS_SERVICE, (IIntent**)&intent);
    intent->SetPackage(engine);
    List< AutoPtr<IResolveInfo> > resolveInfos;

    AutoPtr<IObjectContainer> oc;
    pm->QueryIntentServices(intent.Get(),
                IPackageManager::MATCH_DEFAULT_ONLY | IPackageManager::GET_META_DATA, (IObjectContainer**)&oc);
    if(oc != NULL) {
        AutoPtr<IObjectEnumerator> it;
        oc->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean succeeded = FALSE;
        while(it->MoveNext(&succeeded), succeeded) {
            AutoPtr<IResolveInfo> ttsEi;
            it->Current((IInterface**)&ttsEi);

            resolveInfos.PushBack(ttsEi);

        }
    }

    // Note that the current API allows only one engine per
    // package name. Since the "engine name" is the same as
    // the package name.
    if ((!resolveInfos.IsEmpty()) && resolveInfos.GetSize() == 1) {
        AutoPtr<IServiceInfo> service;
        (resolveInfos[0])->GetServiceInfo((IServiceInfo**)&service);
        if (service != NULL) {
            String settings = SettingsActivityFromServiceInfo(service.Get(), pm.Get());
            if (!settings.IsNull()) {
                AutoPtr<IIntent> i;
                CIntent::New((IIntent**)&i);
                i->SetClassName(engine, settings);
                return i;
            }
        }
    }

    return NULL;
}

String TtsEngines::SettingsActivityFromServiceInfo(
    /* [in] */ IServiceInfo* si,
    /* [in] */ IPackageManager* pm)
{
    AutoPtr<IXmlResourceParser> parser;
    //try {
        si->LoadXmlMetaData(pm, ITextToSpeechEngine::SERVICE_META_DATA, (IXmlResourceParser**)&parser);
        if (parser == NULL) {
            //Java:    Log.w(TAG, "No meta-data found for :" + si);
            Logger::W(TAG, String("No meta-data found for :")+String("\n"));
            return String(NULL);
        }

        AutoPtr<IResources> res;
        AutoPtr<IApplicationInfo> applicationInfo;
        ((CServiceInfo*)si)->GetApplicationInfo((IApplicationInfo**)&applicationInfo);
        pm->GetResourcesForApplication(applicationInfo.Get(), (IResources**)&res);

        Int32 type;
        parser->Next(&type);
        while ((type) != IXmlPullParser::END_DOCUMENT) {
            if (type == IXmlPullParser::START_TAG) {
                String strParserName;
                parser->GetName(&strParserName);
                if (!XML_TAG_NAME.Equals(strParserName)) {
                    //JavaL:    Log.w(TAG, "Package " + si + " uses unknown tag :"  + parser.getName());
                    Logger::W(TAG, String("Package ... uses unknown tag :")+strParserName+String("\n"));
                    return String(NULL);
                }

                AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
                AutoPtr< ArrayOf<Int32> >  aryTextToSpeechEngine = ArrayOf<Int32>::Alloc(1);
                for(Int32 i=0; i<1; i++){
                    (*aryTextToSpeechEngine)[i] = (R::styleable::TextToSpeechEngine)[i];
                }
                AutoPtr<ITypedArray> array;
                res->ObtainAttributes(attrs.Get(), aryTextToSpeechEngine.Get(), (ITypedArray**)&array);
                String settings;
                array->GetString(R::styleable::TextToSpeechEngine_settingsActivity, &settings);
                array->Recycle();
                parser->Close();
                return settings;
            }
        }

    //} catch (NameNotFoundException e) {
            //Java:    Log.w(TAG, "Could not load resources for : " + si);
//            Logger::W(TAG, String("Could not load resources for : ...")+String("\n"));
    //} catch (XmlPullParserException e) {
            //Java:    Log.w(TAG, "Error parsing metadata for " + si + ":" + e);
//            Logger::W(TAG, String("Error parsing metadata for... : ... ")+String("\n"));
    //} catch (IOException e) {
            //Java:    Log.w(TAG, "Error parsing metadata for " + si + ":" + e);
//            Logger::W(TAG, String("Error parsing metadata for ... : ... ")+String("\n"));
    //} finally {
            if (parser != NULL) {
                parser->Close();
            }
    //}
    return String(NULL);
}

AutoPtr<TextToSpeech::TextToSpeechEngineInfo> TtsEngines::GetEngineInfo(
    /* [in] */ IResolveInfo* resolve,
    /* [in] */ IPackageManager* pm)
{
    AutoPtr<IServiceInfo> service;
    resolve->GetServiceInfo((IServiceInfo**)&service);
    if (service != NULL) {
        AutoPtr<TextToSpeech::TextToSpeechEngineInfo> engine = new TextToSpeech::TextToSpeechEngineInfo();
        // Using just the package name isn't great, since it disallows having
        // multiple engines in the same package, but that's what the existing API does.
        ((CServiceInfo*)(service.Get()))->GetPackageName(&(engine->name));    //engine->name = ((CServiceInfo*)service)->packageName;
        AutoPtr<ICharSequence> label;
        service->LoadLabel(pm, (ICharSequence**)&label);
        if(TextUtils::IsEmpty(label)) {
            engine->label = engine->name;
        } else {
            label->ToString(&(engine->label));
        }
        service->GetIconResource(&(engine->icon));
        resolve->GetPriority(&(engine->priority));
        engine->system = IsSystemEngine(service.Get());
        return engine;
    }

    return NULL;
}

String TtsEngines::GetLocalePrefForEngine(
    /* [in] */ const String& engineName)
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);
    String locale;
    locale = ParseEnginePrefFromList( /*GetString(cr.Get(), ISettingsSecure::TTS_DEFAULT_LOCALE)*/String(NULL), engineName);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(locale, (ICharSequence**)&cs);
    if (TextUtils::IsEmpty(cs)) {
        // The new style setting is unset, attempt to return the old style setting.
        locale = GetV1Locale();
    }

    if (DBG){
        //Java:    Log.d(TAG, "getLocalePrefForEngine(" + engineName + ")= " + locale);
        Logger::D(TAG, String("getLocalePrefForEngine(")+ engineName + String(")= ") + locale+String("\n"));
    }

    return locale;
}

AutoPtr< ArrayOf<String> > TtsEngines::ParseLocalePref(
    /* [in] */ const String& pref)
{
    AutoPtr< ArrayOf<String> > returnVal = ArrayOf<String>::Alloc(3);
    (*returnVal)[0] = String("");
    (*returnVal)[1] = String("");
    (*returnVal)[2] = String("");

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(pref, (ICharSequence**)&cs);
    if (!TextUtils::IsEmpty(cs)) {
        Int32 localeDelimiterLen = pref.GetLength();
        Int32 splitCount = 1;
        Int32 splitStartPos = 0;
        while( splitStartPos>=0 && splitStartPos<localeDelimiterLen){
            splitStartPos = pref.IndexOf(LOCALE_DELIMITER, splitStartPos);
            if(splitStartPos>=0 && splitStartPos<=localeDelimiterLen-1) {
                splitCount++;
            }
            splitStartPos++;
        }
        AutoPtr< ArrayOf<String> > split = ArrayOf<String>::Alloc(splitCount);
        Int32 aryIndexRem = 0;
        Int32 curFindIndex=0;
        splitStartPos = 0;
        while( splitStartPos>=0 && splitStartPos<localeDelimiterLen){
            curFindIndex = splitStartPos;
            splitStartPos = pref.IndexOf(LOCALE_DELIMITER, splitStartPos);
            if(splitStartPos>=0 && splitStartPos<=localeDelimiterLen-1) {
                (*split)[aryIndexRem] = pref.Substring(curFindIndex, splitStartPos-curFindIndex);
                aryIndexRem++;
            }
            splitStartPos++;
        }
        for(Int32 i= 0; i<splitCount; i++){
            (*returnVal)[i] = (*split)[i];
        }
    }

    if (DBG) {
        //Java:    Log.d(TAG, "parseLocalePref(" + returnVal[0] + "," + returnVal[1] +"," + returnVal[2] +")");
        Logger::D(TAG, String("parseLocalePref(")+ (*returnVal)[0] + String(",")+ (*returnVal)[1] + String(",")+ (*returnVal)[2] + String(")\n"));
    }

    return returnVal;
}

String TtsEngines::GetV1Locale()
{
    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);

    String lang;
//    lang = Settings::Secure::GetString(cr.Get(), ISettingsSecure::TTS_DEFAULT_LANG);
    String country;
//    country = Settings::Secure::GetString(cr.Get(), ISettingsSecure::TTS_DEFAULT_COUNTRY);
    String variant;
//    variant = Settings::Secure::GetString(cr.Get(), ISettingsSecure::TTS_DEFAULT_VARIANT);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(lang, (ICharSequence**)&cs);
    if (TextUtils::IsEmpty(cs)) {
        return GetDefaultLocale();
    }

    String v1Locale = lang;
    cs = NULL;
    CStringWrapper::New(country, (ICharSequence**)&cs);
    if (!TextUtils::IsEmpty(cs)) {
            v1Locale += String(LOCALE_DELIMITER) + country;
    } else {
        return v1Locale;
    }

    cs = NULL;
    CStringWrapper::New(variant, (ICharSequence**)&cs);
    if (!TextUtils::IsEmpty(cs)) {
        v1Locale += String(LOCALE_DELIMITER) + variant;
    }

    return v1Locale;
}

String TtsEngines::GetDefaultLocale()
{
    AutoPtr<ILocaleHelper> lh;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&lh);
    AutoPtr<ILocale> locale;
    lh->GetDefault((ILocale**)&locale);

    // Note that the default locale might have an empty variant
    // or language, and we take care that the construction is
    // the same as {@link #getV1Locale} i.e no trailing delimiters
    // or spaces.
    String defaultLocale;
    locale->GetISO3Language(&defaultLocale);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(defaultLocale, (ICharSequence**)&cs);
    if (TextUtils::IsEmpty(cs)) {
        //Java:    Log.w(TAG, "Default locale is empty.");
        Logger::W(TAG, String("Default locale is empty.\n"));
        return String("");
    }

    String localeCountry;
    locale->GetISO3Country(&localeCountry);
    cs = NULL;
    CStringWrapper::New(localeCountry, (ICharSequence**)&cs);
    if (!TextUtils::IsEmpty(cs)) {
        defaultLocale += String(LOCALE_DELIMITER) + localeCountry;
    } else {
        // Do not allow locales of the form lang--variant with
        // an empty country.
        return defaultLocale;
    }

    String strVariant;
    locale->GetVariant(&strVariant);
    cs = NULL;
    CStringWrapper::New(strVariant, (ICharSequence**)&cs);
    if (!TextUtils::IsEmpty(cs)) {
        defaultLocale += String(LOCALE_DELIMITER) + strVariant;
    }

    return defaultLocale;
}

String TtsEngines::ParseEnginePrefFromList(
    /* [in] */ const String& prefValue,
    /* [in] */ const String& engineName)
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(prefValue, (ICharSequence**)&cs);
    if (TextUtils::IsEmpty(cs)) {
        return String(NULL);
    }

    Int32 localeDelimiterLen = prefValue.GetLength();
    Int32 splitCount = 1;
    Int32 splitStartPos = 0;
    while( splitStartPos>=0 && splitStartPos<localeDelimiterLen){
        splitStartPos = prefValue.IndexOf(String(","), splitStartPos);
        if(splitStartPos>=0 && splitStartPos<=localeDelimiterLen-1) {
            splitCount++;
        }
        splitStartPos++;
    }
    AutoPtr< ArrayOf<String> > prefValues = ArrayOf<String>::Alloc(splitCount);
    Int32 aryIndexRem = 0;
    Int32 curFindIndex=0;
    splitStartPos = 0;
    while( splitStartPos>=0 && splitStartPos<localeDelimiterLen){
        curFindIndex = splitStartPos;
        splitStartPos = prefValue.IndexOf(String(","), splitStartPos);
        if(splitStartPos>=0 && splitStartPos<=localeDelimiterLen-1) {
            (*prefValues)[aryIndexRem] = prefValue.Substring(curFindIndex, splitStartPos-curFindIndex);
            aryIndexRem++;
        }
        splitStartPos++;
    }

    for(Int32 i=0; i<splitCount; i++) {
        String value = (*prefValues)[i];
        Int32 delimiter = value.IndexOf(':');
        if (delimiter > 0) {
            if (engineName.Equals(value.Substring(0, delimiter))) {
                return value.Substring(delimiter + 1);
            }
        }
    }
    return String(NULL);
}

void TtsEngines::UpdateLocalePrefForEngine(
    /* [in] */ const String& name,
    /* [in] */ const String& newLocale)
{
    Mutex::Autolock lock(mLock);

    AutoPtr<IContentResolver> cr;
    mContext->GetContentResolver((IContentResolver**)&cr);

    String prefList;
//    prefList = Settings::Secure::GetString(cr.Get(), ISettingsSecure::TTS_DEFAULT_LOCALE);
    if (DBG) {
        //Java:    Log.d(TAG, "updateLocalePrefForEngine(" + name + ", " + newLocale + "), originally: " + prefList);
        Logger::D(TAG, String("updateLocalePrefForEngine(")+ name + String(", ") + newLocale + String("), originally: ") + prefList+String("\n"));
    }

    String newPrefList = UpdateValueInCommaSeparatedList(prefList, name, newLocale);

    if (DBG){
        //Java:    Log.d(TAG, "updateLocalePrefForEngine(), writing: " + newPrefList.toString());
        Logger::D(TAG, String("updateLocalePrefForEngine(), writing: ")+ newPrefList + String("\n"));

    }

//    Settings::Secure::PutString(cr.Get(), ISettingsSecure::TTS_DEFAULT_LOCALE, newPrefList);
}

String TtsEngines::UpdateValueInCommaSeparatedList(
    /* [in] */ const String& list,
    /* [in] */ const String& key,
    /* [in] */ const String&  newValue)
{
    //Java:    StringBuilder newPrefList = new StringBuilder();
    String newPrefList;
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(list, (ICharSequence**)&cs);
    if (TextUtils::IsEmpty(cs)) {
        // If empty, create a new list with a single entry.
        newPrefList.Append(key);
        newPrefList.Append(String(":"));
        newPrefList.Append(newValue);
    }
    else {
        //Java:    String[] prefValues = list.split(",");
        Int32 localeDelimiterLen = list.GetLength();
        Int32 splitCount = 1;
        Int32 splitStartPos = 0;
        while( splitStartPos>=0 && splitStartPos<localeDelimiterLen){
            splitStartPos = list.IndexOf(String(","), splitStartPos);
            if(splitStartPos>=0 && splitStartPos<=localeDelimiterLen-1) {
                splitCount++;
            }
            splitStartPos++;
        }
        AutoPtr< ArrayOf<String> > prefValues = ArrayOf<String>::Alloc(splitCount);
        Int32 aryIndexRem = 0;
        Int32 curFindIndex=0;
        splitStartPos = 0;
        while( splitStartPos>=0 && splitStartPos<localeDelimiterLen){
            curFindIndex = splitStartPos;
            splitStartPos = list.IndexOf(String(","), splitStartPos);
            if(splitStartPos>=0 && splitStartPos<=localeDelimiterLen-1) {
                (*prefValues)[aryIndexRem] = list.Substring(curFindIndex, splitStartPos-curFindIndex);
                aryIndexRem++;
            }
            splitStartPos++;
        }

        // Whether this is the first iteration in the loop.
        Boolean first = TRUE;
        // Whether we found the given key.
        Boolean found = FALSE;


        for(Int32 i=0; i<splitCount; i++) {
            String value = (*prefValues)[i];

            Int32 delimiter = value.IndexOf(':');
            if (delimiter > 0) {
                if (key.Equals(value.Substring(0, delimiter))) {
                    if (first) {
                        first = FALSE;
                    } else {
                        newPrefList.Append(String(","));
                    }
                    found = TRUE;
                    newPrefList.Append(key);
                    newPrefList.Append(String(":"));
                    newPrefList.Append(newValue);
                } else {
                    if (first) {
                        first = FALSE;
                    } else {
                        newPrefList.Append(String(","));
                    }
                    // Copy across the entire key + value as is.
                    newPrefList.Append(value);
                }
            }

        }

        if (!found) {
            // Not found, but the rest of the keys would have been copied
            // over already, so just append it to the end.
            newPrefList.Append(String(","));
            newPrefList.Append(key);
            newPrefList.Append(String(":"));
            newPrefList.Append(newValue);
        }
    }

    return newPrefList;
}


} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
