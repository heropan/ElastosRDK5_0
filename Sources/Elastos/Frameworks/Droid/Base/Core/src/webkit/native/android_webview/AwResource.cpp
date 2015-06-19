
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

Int32 AwResource::RAW_LOAD_ERROR = 0;
Int32 AwResource::RAW_NO_DOMAIN = 0;
Int32 AwResource::STRING_ARRAY_CONFIG_KEY_SYSTEM_UUID_MAPPING = 0;
AutoPtr<IResources> AwResource::sResources;
SparseArray<SoftReference<String>> AwResource::sResourceCache;
const Int32 AwResource::TYPE_STRING;
const Int32 AwResource::TYPE_RAW;

void AwResource::SetResources(
    /* [in] */ IResources* resources)
{
    sResources = resources;
    sResourceCache = new SparseArray<SoftReference<String>>();
}

void AwResource::SetErrorPageResources(
    /* [in] */ Int32 loaderror,
    /* [in] */ Int32 nodomain)
{
    RAW_LOAD_ERROR = loaderror;
    RAW_NO_DOMAIN = nodomain;
}

void AwResource::SetDefaultTextEncoding(
    /* [in] */ Int32 encoding)
{
}

void AwResource::SetConfigKeySystemUuidMapping(
    /* [in] */ Int32 config)
{
    STRING_ARRAY_CONFIG_KEY_SYSTEM_UUID_MAPPING = config;
}

//@CalledByNative
String AwResource::GetDefaultTextEncoding()
{
    return String("UTF-8");
}

//@CalledByNative
String AwResource::GetNoDomainPageContent()
{
    return GetResource(RAW_NO_DOMAIN, TYPE_RAW);
}

//@CalledByNative
String AwResource::GetLoadErrorPageContent()
{
    return GetResource(RAW_LOAD_ERROR, TYPE_RAW);
}

AutoPtr<ArrayOf< <String> > AwResource::GetConfigKeySystemUuidMapping()
{
    // No need to cache, since this should be called only once.
    return sResources.getStringArray(STRING_ARRAY_CONFIG_KEY_SYSTEM_UUID_MAPPING);
}

String AwResource::GetResource(
    /* [in] */ Int32 resid,
    /* [in] */ Int32 type)
{
    assert(resid != 0);
    assert(sResources != NULL);
    assert(sResourceCache != NULL);

    SoftReference<String> stringRef = sResourceCache.get(resid);
    String result = stringRef == NULL ? NULL : stringRef.get();
    if (result == NULL) {
        switch (type) {
            case TYPE_STRING:
                sResources->GetString(resid, &result);
                break;
            case TYPE_RAW:
                result = GetRawFileResourceContent(resid);
                break;
            default:
//                throw new IllegalArgumentException("Unknown resource type");
                assert(0);
        }

        sResourceCache.put(resid, new SoftReference<String>(result));
    }
    return result;
}

String AwResource::GetRawFileResourceContent(
    /* [in] */ Int32 resid)
{
    assert(resid != 0);
    assert(sResources != NULL);

    AutoPtr<IInputStreamReader> isr;
    String result;

    // try {
        AutoPtr<IInputStream> res;
        sResources->OpenRawResource(resid, (IInputStream**)&res);
        CInputStreamReader(res, (IInputStreamReader**)&isr);
        // \A tells the scanner to use the beginning of the input
        // as the delimiter, hence causes it to read the entire text.
        AutoPtr<IScanner> scanner;
        CScanner::New(isr, (IScanner**)&scanner);
        AutoPtr<IScanner> _scanner;
        scanner->UseDelimiter(String("\\A"), (IScanner**)&_scanner);
        _scanner->Next(&result);
    // } catch (Resources.NotFoundException e) {
    //     return "";
    // } catch (NoSuchElementException e) {
    //     return "";
    // } finally {
    //     try {
    //         if (isr != null) {
    //             isr.close();
    //         }
    //     } catch (IOException e) {
    //         // Nothing to do if close() fails.
    //     }
    // }
    return result;
}
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
