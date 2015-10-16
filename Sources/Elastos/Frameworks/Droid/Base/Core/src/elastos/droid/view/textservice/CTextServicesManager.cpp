
namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

const String CTextServicesManager::TAG;// = TextServicesManager.class.getSimpleName();
const Boolean CTextServicesManager::DBG = FALSE;

AutoPtr<ITextServicesManager> CTextServicesManager::sInstance;
AutoPtr<ITextServicesManager> CTextServicesManager::sService;

Mutex CTextServicesManager::sLock;

/**
 * Retrieve the global TextServicesManager instance, creating it if it doesn't already exist.
 * @hide
 */
AutoPtr<ITextServicesManager> CTextServicesManager::GetInstance()
{
    //synchronized (TextServicesManager.class)
    {
        Object::AutoLock lock(sLock);
        if (sInstance != NULL) {
            return sInstance;
        }
        CTextServicesManager::New((ITextServicesManager**)&sInstance);
    }

    return sInstance;
}

ECode CTextServicesManager::constructor()
{
    if (sService == NULL) {
//        IBinder b = ServiceManager.getService(Context.TEXT_SERVICES_MANAGER_SERVICE);
//        sService = ITextServicesManager.Stub.asInterface(b);
    }
}

/**
 * Get a spell checker session for the specified spell checker
 * @param locale the locale for the spell checker. If {@code locale} is null and
 * referToSpellCheckerLanguageSettings is true, the locale specified in Settings will be
 * returned. If {@code locale} is not null and referToSpellCheckerLanguageSettings is true,
 * the locale specified in Settings will be returned only when it is same as {@code locale}.
 * Exceptionally, when referToSpellCheckerLanguageSettings is true and {@code locale} is
 * only language (e.g. "en"), the specified locale in Settings (e.g. "en_US") will be
 * selected.
 * @param listener a spell checker session lister for getting results from a spell checker.
 * @param referToSpellCheckerLanguageSettings if true, the session for one of enabled
 * languages in settings will be returned.
 * @return the spell checker session of the spell checker
 */
ECode CTextServicesManager::NewSpellCheckerSession(
    /* [in] */ IBundle* bundle,
    /* [in] */ ILocale* locale,
    /* [in] */ ISpellCheckerSessionListener* listener,
    /* [in] */ Boolean referToSpellCheckerLanguageSettings,
    /* [out] */ ISpellCheckerSession** session)
{
    VALIDATE_NOT_NULL(session);

    //if (listener == null) {
    //    throw new NullPointerException();
    //}
    assert(listener != NULL);

    //if (!referToSpellCheckerLanguageSettings && locale == null) {
    //    throw new IllegalArgumentException("Locale should not be null if you don't refer"
    //            + " settings.");
    //}
    assert(!(!referToSpellCheckerLanguageSettings && locale == NULL));

    Boolean enabled = FALSE;
    if (referToSpellCheckerLanguageSettings && !(IsSpellCheckerEnabled(&enabled), enabled)) {
        *session = NULL;
        return NOERROR;
    }

    AutoPtr<ISpellCheckerInfo> sci;
    //try {
        sService->GetCurrentSpellChecker(NULL, (ISpellCheckerInfo**)&sci);
    //} catch (RemoteException e) {
    //    return null;
    //}

    if (sci == NULL) {
        *session = NULL;
        return NOERROR;
    }

    AutoPtr<ISpellCheckerSubtype> subtypeInUse;
    if (referToSpellCheckerLanguageSettings) {
        GetCurrentSpellCheckerSubtype(TRUE, (ISpellCheckerSubtype**)&subtypeInUse);
        if (subtypeInUse == NULL) {
            *session = NULL;
            return NOERROR;
        }
        if (locale != NULL) {

            String subtypeLocale;
            String inputLocale;

            subtypeInUse->GetLocale(&subtypeLocale);
            locale->ToString(&inputLocale);
            if (subtypeLocale.GetLength() < 2 || inputLocale.GetLength() < 2
                    || !subtypeLocale.Substring(0, 2).Equals(inputLocale.Substring(0, 2))) {
                *session = NULL;
                return NOERROR;
            }
        }
    } else {

        String localeStr;
        locale->ToString(&localeStr);

        Int32 count;
        sci->GetSubtypeCount(&count);

        for (Int32 i = 0; i < count; ++i) {
            AutoPtr<ISpellCheckerSubtype> subtype;
            String tempSubtypeLocale;

            sci->GetSubtypeAt(i, (ISpellCheckerSubtype**)&subtype);
            subtype->GetLocale(&tempSubtypeLocale);

            if (tempSubtypeLocale.Equals(localeStr)) {
                subtypeInUse = subtype;
                break;
            } else if (localeStr.GetLength() >= 2 && tempSubtypeLocale.GetLength() >= 2
                    && localeStr.StartsWith(tempSubtypeLocale)) {
                subtypeInUse = subtype;
            }
        }
    }

    if (subtypeInUse == NULL) {
        *session = NULL;
        return NOERROR;
    }

    AutoPtr<ISpellCheckerSession> _session;
    CSpellCheckerSession::New(
            sci, sService, listener, subtypeInUse, (ISpellCheckerSession**)&_session);

    //try {
        String id, localeString;
        AutoPtr<IITextServicesSessionListener> textListener;
        AutoPtr<IISpellCheckerSessionListener> spellListener;

        sci->GetId(&id);
        subtypeInUse->GetLocale(&localeString);
        _session->GetTextServicesSessionListener((IITextServicesSessionListener**)&textListener);
        _session->GetSpellCheckerSessionListener((IISpellCheckerSessionListener**)&spellListener);

        sService->GetSpellCheckerService(id, localeString,
                textListener, spellListener, bundle);
    //} catch (RemoteException e) {
    //    return null;
    //}

    *session = _session;
    REFCOUNT_ADD(*session);
    return NOERROR;
}

/**
 * @hide
 */
ECode CTextServicesManager::GetEnabledSpellCheckers(
    /* [out] */ ArrayOf<ISpellCheckerInfo*>** infoArray)
{
    VALIDATE_NOT_NULL(infoArray);

    //try {
        return sService->GetEnabledSpellCheckers(infoArray);
    //    if (DBG) {
    //        Log.d(TAG, "getEnabledSpellCheckers: " + (retval != null ? retval.length : "null"));
    //    }
    //    return retval;
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Error in getEnabledSpellCheckers: " + e);
    //    return null;
    //}
}

/**
 * @hide
 */
ECode CTextServicesManager::GetCurrentSpellChecker(
    /* [out] */ ISpellCheckerInfo** info)
{
    VALIDATE_NOT_NULL(info);

    //try {
        // Passing null as a locale for ICS
        return sService->GetCurrentSpellChecker(NULL, info);
    //} catch (RemoteException e) {
    //    return null;
    //}
}

/**
 * @hide
 */
ECode CTextServicesManager::SetCurrentSpellChecker(
    /* [in] */ ISpellCheckerInfo* sci)
{
    //try {
    //    if (sci == null) {
    //        throw new NullPointerException("SpellCheckerInfo is null.");
    //    }
        assert(sci != NULL);

        String id;
        sci->GetId(&id);

        return sService->SetCurrentSpellChecker(NULL, id);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Error in setCurrentSpellChecker: " + e);
    //}
}

/**
 * @hide
 */
ECode CTextServicesManager::GetCurrentSpellCheckerSubtype(
    /* [in] */ Boolean allowImplicitlySelectedSubtype,
    /* [out] */ ISpellCheckerSubtype** subtype)
{
    VALIDATE_NOT_NULL(subtype);

    //try {
        if (sService == NULL) {
            // TODO: This is a workaround. Needs to investigate why sService could be null
            // here.
    //        Log.e(TAG, "sService is null.");
            *subtype = NULL;
            return NOERROR;
        }
        // Passing null as a locale until we support multiple enabled spell checker subtypes.
        return sService->GetCurrentSpellCheckerSubtype(NULL, allowImplicitlySelectedSubtype, subtype);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Error in getCurrentSpellCheckerSubtype: " + e);
    //    return null;
    //}
}

/**
 * @hide
 */
ECode CTextServicesManager::SetSpellCheckerSubtype(
    /* [in] */ ISpellCheckerSubtype* subtype)
{
    //try {
        Int32 hashCode;
        if (subtype == NULL) {
            hashCode = 0;
        } else {
            subtype->GetHashCode(&hashCode);
        }
        return sService->SetCurrentSpellCheckerSubtype(NULL, hashCode);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Error in setSpellCheckerSubtype:" + e);
    //}
}

/**
 * @hide
 */
ECode CTextServicesManager::SetSpellCheckerEnabled(
    /* [in] */ Boolean enabled)
{
    //try {
        return sService->SetSpellCheckerEnabled(enabled);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Error in setSpellCheckerEnabled:" + e);
    //}
}

/**
 * @hide
 */
ECode CTextServicesManager::IsSpellCheckerEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    //try {
        return sService->IsSpellCheckerEnabled(enabled);
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Error in isSpellCheckerEnabled:" + e);
    //    return false;
    //}
}

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
