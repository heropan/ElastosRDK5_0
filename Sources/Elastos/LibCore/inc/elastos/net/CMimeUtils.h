#ifndef _CMIMEUTILS_H__
#define _CMIMEUTILS_H__

#include "_CMimeUtils.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Net {

CarClass(CMimeUtils)
{
public:

    CMimeUtils();

    ~CMimeUtils();

    CARAPI HasMineType(
        /* [in] */ const String& mimeType,
        /* [out] */ Boolean *result);

    CARAPI GuessMimeTypeFromExtension(
        /* [in] */ const String& extension,
        /* [out] */ String* result);

    CARAPI HasExtension(
        /* [in] */ const String& extension,
        /* [out] */ Boolean* result);

    CARAPI GuessExtensionFromMimeType(
        /* [in] */ const String& mimeType,
        /* [out] */ String* result);

private:
    void Add(
        /* [in] */ const String& mimeType,
        /* [in] */ const String& extension);

    CARAPI ApplyOverrides();

    void AddAllNeeded();

    CARAPI GetContentTypesPropertiesStream(
        /* [out] */IInputStream** result);

    CARAPI Load(
        /* [in] */ IInputStream* stream,
        /* [in] */ HashMap<String, String>* map);

private:

    AutoPtr< HashMap<String, String> > mMimeTypeToExtensionMap;
    AutoPtr< HashMap<String, String> > mExtensionToMimeTypeMap;
    typedef HashMap<String, String>::Iterator HMIterator;
};

} // namespace Net
} // namespace Elastos

#endif // _CMIMEUTILS_H__
