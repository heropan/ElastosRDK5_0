
#include "Preferences.h"

namespace Elastos {
namespace Utility {
namespace Prefs {

const Int32 Preferences::MAX_KEY_LENGTH = 80;
const Int32 Preferences::MAX_NAME_LENGTH = 80;
const Int32 Preferences::MAX_VALUE_LENGTH = 8192;

static AutoPtr<IPreferencesFactory> InitFactory()
{
    // // Try the system property first...
    // AutoPtr<IPreferencesFactory> result = ServiceLoader::LoadFromSystemProperty(PreferencesFactory.class);
    // if (result != NULL) {
    //     return result;
    // }
    // // Then use ServiceLoader for META-INF/services/...
    // for (PreferencesFactory impl : ServiceLoader.load(PreferencesFactory.class)) {
    //     return impl;
    // }
    // // Finally return a default...
    // return new FilePreferencesFactoryImpl();
}

volatile AutoPtr<IPreferencesFactory> Preferences::mFactory = InitFactory();

CAR_INTERFACE_IMPL(Preferences, Object, IPreferences);

AutoPtr<IPreferencesFactory> Preferences::SetPreferencesFactory(
    /* [in] */ IPreferencesFactory* pf)
{
    AutoPtr<IPreferencesFactory> previous = mFactory;
    mFactory = pf;
    return previous;
}

AutoPtr<IPreferencesFactory> Preferences::FindPreferencesFactory()
{
    return InitFactory();
}

Preferences::Preferences()
{
}

Preferences::~Preferences()
{
}

ECode Preferences::ImportPreferences(
    /* [in] */ IInputStream* istream)
{
    if (istream == NULL){
        // throw new MalformedURLException("Inputstream cannot be null");
        return E_MALFORMED_URL_EXCEPTION;
    }

    //TODO
    assert(0);
    // return XMLParser::ImportPrefs(istream);
    return NOERROR;
}

ECode Preferences::SystemNodeForPackage(
    /* [in] */ IInterface* c,
    /* [out] */ IPreferences** outpre)
{
    //TODO
    assert(0);
    // return mFactory.systemRoot().node(getNodeName(c));
    return NOERROR;
}

ECode Preferences::SystemRoot(
    /* [out] */ IPreferences** outpre)
{
    VALIDATE_NOT_NULL(outpre);
    return mFactory->SystemRoot(outpre);
}

ECode Preferences::UserNodeForPackage(
    /* [in] */ IInterface* c,
    /* [out] */ IPreferences** outpre)
{
    VALIDATE_NOT_NULL(outpre);
    //TODO
    assert(0);
    // return mFactory.userRoot().node(getNodeName(c));
    return NOERROR;
}

String Preferences::GetNodeName(
    /* [in] */ IInterface* c)
{
    // Package p = c.getPackage();
    // if (p == null){
    //     return "/<unnamed>";
    // }
    // return "/"+p.getName().replace('.', '/');
    //TODO
    assert(0);
    return String(NULL);
}

ECode Preferences::UserRoot(
    /* [out] */ IPreferences** outpre)
{
    VALIDATE_NOT_NULL(outpre);
    return mFactory->UserRoot(outpre);
}

} // namespace Prefs
} // namespace Utility
} // namespace Elastos
