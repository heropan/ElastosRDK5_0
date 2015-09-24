
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

const String AndroidProtocolHandler::TAG("AndroidProtocolHandler");
const String AndroidProtocolHandler::FILE_SCHEME("file");
const String AndroidProtocolHandler::CONTENT_SCHEME("content");

/**
 * Open an InputStream for an Android resource.
 * @param context The context manager.
 * @param url The url to load.
 * @return An InputStream to the Android resource.
 */
//@CalledByNative
AutoPtr<IInputStream> AndroidProtocolHandler::Open(
    /* [in] */ IContext* context,
    /* [in] */ const String& url)
{
    AutoPtr<IUri> uri = VerifyUrl(url);
    if (uri == NULL) {
        return NULL;
    }
    //try {
        String path;
        uri->GetPath(&path);
        String scheme;
        uri->GetScheme(&scheme);
        if (scheme.Equals(FILE_SCHEME)) {
            if (path.StartsWith(NativeGetAndroidAssetPath())) {
                return OpenAsset(context, uri);
            }
            else if (path.StartsWith(NativeGetAndroidResourcePath())) {
                return OpenResource(context, uri);
            }
        }
        else if (scheme.Equals(CONTENT_SCHEME)) {
            return OpenContent(context, uri);
        }
    //} catch (Exception ex) {
    //    Log.e(TAG, "Error opening inputstream: " + url);
    //}
    return NULL;
}

Int32 AndroidProtocolHandler::GetFieldId(
    /* [in] */ IContext* context,
    /* [in] */ const String& assetType,
    /* [in] */ const String& assetName)
{
    Class<?> d = context.getClassLoader()
        .loadClass(context.getPackageName() + ".R$" + assetType);
    java.lang.reflect.Field field = d.getField(assetName);
    int id = field.getInt(null);
    return id;
}

Int32 AndroidProtocolHandler::GetValueType(
    /* [in] */ IContext* context,
    /* [in] */ Int32 fieldId)
{
    AutoPtr<ITypedValue> value;
    CTypedValue((ITypedValue**)&value);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetValue(fieldId, value, TRUE);
    Int32 type;
    value->GetType(&type);
    return type;
}

AutoPtr<IInputStream> AndroidProtocolHandler::OpenResource(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    String scheme;
    uri->GetScheme(&scheme);
    String path;
    uri->GetPath(&path);
    assert(scheme.equals(FILE_SCHEME));
    assert(path != NULL);
    assert(path.StartsWith(NativeGetAndroidResourcePath()));
    // The path must be of the form "/android_res/asset_type/asset_name.ext".
    AutoPtr< ArrayOf<String> > pathSegments;
    uri->GetPathSegments((ArrayOf<String>**)&pathSegments);
    if (pathSegments->Size() != 3) {
//        Log.e(TAG, "Incorrect resource path: " + uri);
        return NULL;
    }
    String assetPath = (*pathSegments)[0];
    String assetType = (*pathSegments)[1];
    String assetName = (*pathSegments)[2];
    if (!("/" + assetPath + "/").equals(nativeGetAndroidResourcePath())) {
        Log.e(TAG, "Resource path does not start with " + nativeGetAndroidResourcePath() +
              ": " + uri);
        return null;
    }
    // Drop the file extension.
    assetName = assetName.split("\\.")[0];
    //try {
        // Use the application context for resolving the resource package name so that we do
        // not use the browser's own resources. Note that if 'context' here belongs to the
        // test suite, it does not have a separate application context. In that case we use
        // the original context object directly.
        AutoPtr<IContext> appContext;
        context->GetApplicationContext((IContext**)&appContext);
        if (appContext != NULL) {
            context = appContext;
        }
        Int32 fieldId = GetFieldId(context, assetType, assetName);
        Int32 valueType = GetValueType(context, fieldId);
        if (valueType == ITypedValue::TYPE_STRING) {
            AutoPtr<IResources> res;
            context->GetResources((IResources**)&res);
            AutoPtr<IInputStream> inputStream;
            res->OpenRawResource(fieldId, (IInputStream**)inputstream);
            return inputstream;
        }
        else {
//            Log.e(TAG, "Asset not of type string: " + uri);
            return NULL;
        }
    //} catch (ClassNotFoundException e) {
    //    Log.e(TAG, "Unable to open resource URL: " + uri, e);
    //    return null;
    //} catch (NoSuchFieldException e) {
    //    Log.e(TAG, "Unable to open resource URL: " + uri, e);
    //    return null;
    //} catch (IllegalAccessException e) {
    //    Log.e(TAG, "Unable to open resource URL: " + uri, e);
    //    return null;
    //}
}

AutoPtr<IInputStream> AndroidProtocolHandler::OpenAsset(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    String scheme, pathStr;
    uri->GetScheme(&scheme);
    uri->GetPath(&pathStr);
    assert(scheme.Equals(FILE_SCHEME));
    assert(pathStr != NULL);
    assert(pathStr.StartsWith(NativeGetAndroidAssetPath()));
    String path = pathStr.ReplaceFirst(NativeGetAndroidAssetPath(), "");
    //try {
        AutoPtr<IAssetManager> assets;
        context->GetAssets((IAssetManager**)&assets);
        AutoPtr<IInputStream> inputstream;
        assets->Open(path, IAssetManager::ACCESS_STREAMING, (IInputStream**)&inputstream);
        return inputstream;
    //} catch (IOException e) {
    //    Log.e(TAG, "Unable to open asset URL: " + uri);
    //    return null;
    //}
}

AutoPtr<IInputStream> AndroidProtocolHandler::OpenContent(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    String scheme;
    uri->GetScheme(&scheme);
    assert(scheme.Equals(CONTENT_SCHEME));
    //try {
        AutoPtr<IContentResolver> resolver;
        context->GetContentResolver((IContentResolver**)&resolver);
        AutoPtr<IInputStream> inputstream;
        resolver->OpenInputStream(uri, (IInputStream**)&inputstream);
        return inputstream;
    //} catch (Exception e) {
    //    Log.e(TAG, "Unable to open content URL: " + uri);
    //    return null;
    //}
}

/**
 * Determine the mime type for an Android resource.
 * @param context The context manager.
 * @param stream The opened input stream which to examine.
 * @param url The url from which the stream was opened.
 * @return The mime type or null if the type is unknown.
 */
//@CalledByNative
String AndroidProtocolHandler::GetMimeType(
    /* [in] */ IContext* context,
    /* [in] */ IInputStream* stream,
    /* [in] */ const String& url)
{
    AutoPtr<IUri> uri = VerifyUrl(url);
    if (uri == NULL) {
        return NULL;
    }
    //try {
        String path;
        uri->GetPath(&path);
        // The content URL type can be queried directly.
        String scheme;
        uri->GetScheme(&scheme);
        if (scheme.Equals(CONTENT_SCHEME)) {
            AutoPtr<IContentResolver> resolver;
            context->GetContentResolver((IContentResolver**)&resolver);
            String type;
            resolver->GetType(uri, &type);
            return type;
            // Asset files may have a known extension.
        }
        else if (scheme.Equals(FILE_SCHEME) &&
                   path.StartsWith(NativeGetAndroidAssetPath())) {
            AutoPtr<IURLConnectionHelper> helper;
            CURLConnectionHelper::AcquireSingleton((IURLConnectionHelper**)&helper);
            String mimeType;
            helper->GuessContentTypeFromName(path, &mimeType);
            if (mimeType != NULL) {
                return mimeType;
            }
        }
    //} catch (Exception ex) {
//        Log.e(TAG, "Unable to get mime type" + url);
        return NULL;
    }
    // Fall back to sniffing the type from the stream.
    //try {
        AutoPtr<IURLConnectionHelper> helper;
        CURLConnectionHelper::AcquireSingleton((IURLConnectionHelper**)&helper);

        String type;
        helper->GuessContentTypeFromStream(stream, &type);
        return type;
    //} catch (IOException e) {
    //    return null;
    //}
}

/**
 * Make sure the given string URL is correctly formed and parse it into a Uri.
 * @return a Uri instance, or null if the URL was invalid.
 */
AutoPtr<IUri> AndroidProtocolHandler::VerifyUrl(
    /* [in] */ const String& url)
{
    if (url == NULL) {
        return NULL;
    }

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> uri;
    helper->Parse(url, (IUri**)&uri);
    if (uri == NULL) {
//        Log.e(TAG, "Malformed URL: " + url);
        return NULL;
    }
    String path;
    uri->GetPath(&path);
    if (path == NULL || path.GetLength() == 0) {
//        Log.e(TAG, "URL does not have a path: " + url);
        return NULL;
    }
    return uri;
}

/**
 * Set the context to be used for resolving resource queries.
 * @param context Context to be used, or null for the default application
 *                context.
 */
void AndroidProtocolHandler::SetResourceContextForTesting(
    /* [in] */ IContext* context)
{
    NativeSetResourceContextForTesting(context);
}

void AndroidProtocolHandler::NativeSetResourceContextForTesting(
    /* [in] */ IContext* context);

String AndroidProtocolHandler::NativeGetAndroidAssetPath()
{
}

String AndroidProtocolHandler::NativeGetAndroidResourcePath()
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
