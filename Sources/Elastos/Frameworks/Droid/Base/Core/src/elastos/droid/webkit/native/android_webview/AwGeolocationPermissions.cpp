
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//        AwGeolocationPermissions::GetAllowedRunnable
//===============================================================

AwGeolocationPermissions::GetAllowedRunnable::GetAllowedRunnable(
    /* [in] */ AwGeolocationPermissions* owner,
    /* [in] */ IValueCallback* callback,
    /* [in] */ Boolean finalAllowed)
    : mOwner(owner)
    , mCallback(callback)
    , mFinalAllowed(finalAllowed)
{
}

ECode AwGeolocationPermissions::GetAllowedRunnable::Run()
{
    return mCallback->OnReceiveValue(mFinalAllowed);
}

//===============================================================
//        AwGeolocationPermissions::GetOriginsRunnable
//===============================================================

AwGeolocationPermissions::GetOriginsRunnable::GetOriginsRunnable(
    /* [in] */ AwGeolocationPermissions* owner,
    /* [in] */ IValueCallback* callback,
    /* [in] */ Set<String>* origins)
    : mOwner(owner)
    , mCallback(callback)
    , mOrigins(origins)
{
}

ECode AwGeolocationPermissions::GetOriginsRunnable::Run()
{
    return mCallback->OnReceiveValue(mOrigins);
}

//===============================================================
//                    AwGeolocationPermissions
//===============================================================

static const String PREF_PREFIX("AwGeolocationPermissions%");

AwGeolocationPermissions::AwGeolocationPermissions(
    /* [in] */ ISharedPreferences* sharedPreferences)
    : mSharedPreferences(sharedPreferences)
{
}

/**
 * Set one origin to be allowed.
 */
void AwGeolocationPermissions::Allow(
    /* [in] */ String origin)
{
    String key = GetOriginKey(origin);
    if (key != NULL) {
        AutoPtr<ISharedPreferencesEditor> edit;
        mSharedPreferences->Edit((ISharedPreferencesEditor**)&edit);
        edit->PutBoolean(key, TRUE);
        edit->Apply();
    }
}

/**
 * Set one origin to be denied.
 */
void AwGeolocationPermissions::Deny(
    /* [in] */ String origin)
{
    String key = GetOriginKey(origin);
    if (key != NULL) {
        AutoPtr<ISharedPreferencesEditor> edit;
        mSharedPreferences->Edit((ISharedPreferencesEditor**)&edit);
        edit->PutBoolean(key, FALSE);
        edit->Apply();
    }
}

/**
 * Clear the stored permission for a particular origin.
 */
void AwGeolocationPermissions::Clear(
    /* [in] */ String origin)
{
    String key = GetOriginKey(origin);
    if (key != NULL) {
        AutoPtr<ISharedPreferencesEditor> edit;
        mSharedPreferences->Edit((ISharedPreferencesEditor**)&edit);
        edit->Remove(key);
        edit->Apply();
    }
}

/**
 * Clear stored permissions for all origins.
 */
void AwGeolocationPermissions::ClearAll()
{
    AutoPtr<ISharedPreferencesEditor> editor;
    for (String name : mSharedPreferences.getAll().keySet()) {
        if (name.StartsWith(PREF_PREFIX)) {
            if (editor == NULL) {
                mSharedPreferences->Edit((ISharedPreferencesEditor**)&editor);
            }
            editor->Remove(name);
        }
    }
    if (editor != NULL) {
        editor->Apply();
    }
}

/**
 * Synchronous method to get if an origin is set to be allowed.
 */
Boolean AwGeolocationPermissions::IsOriginAllowed(
    /* [in] */ String origin)
{
    Boolean result = FALSE;
    mSharedPreferences->GetBoolean(GetOriginKey(origin), FALSE, &result);
    return result;
}

/**
 * Returns true if the origin is either set to allowed or denied.
 */
Boolean AwGeolocationPermissions::HasOrigin(
    /* [in] */ String origin)
{
    Boolean result = FALSE;
    mSharedPreferences->Contains(GetOriginKey(origin), &result);
    return result;
}

/**
 * Asynchronous method to get if an origin set to be allowed.
 */
void AwGeolocationPermissions::GetAllowed(
    /* [in] */ String origin,
    /* [in] */ const IValueCallback* callback)
{
    Boolean finalAllowed = IsOriginAllowed(origin);
    AutoPtr<IRunnable> runnable = new GetAllowedRunnable(this, callback, finalAllowed);
    ThreadUtils::PostOnUiThread(runnable);
}

/**
 * Async method to get the domains currently allowed or denied.
 */
void AwGeolocationPermissions::GetOrigins(
    /* [in] */ const IValueCallback* callback)
{
    final Set<String> origins = new HashSet<String>();
    for (String name : mSharedPreferences.getAll().keySet()) {
        if (name.StartsWith(PREF_PREFIX)) {
            origins.Add(name.Substring(PREF_PREFIX.GetLength()));
        }
    }
    AutoPtr<IRunnable> runnable = new GetOriginsRunnable(this, callback, origins);
    ThreadUtils::PostOnUiThread(runnable);
}

/**
 * Get the domain of an URL using the GURL library.
 */
String AwGeolocationPermissions::GetOriginKey(
    /* [in] */ String url)
{
    String origin = GURLUtils::GetOrigin(url);
    if (origin.IsEmpty()) {
        return NULL;
    }

    return PREF_PREFIX + origin;
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos