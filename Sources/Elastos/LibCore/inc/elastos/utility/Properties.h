#ifndef __UTILITY_PROPERTIES_H__
#define __UTILITY_PROPERTIES_H__

#include "HashTable.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::IPrintStream;
using Elastos::IO::IPrintWriter;
using Elastos::IO::IReader;
using Elastos::IO::IWriter;
using Elastos::Core::IAppendable;
using Elastos::Core::IStringBuilder;

namespace Elastos{
namespace Utility{

class Properties : public HashTable
{
public:
    CARAPI GetProperty(
        /* [in] */ const String& name,
        /* [out] */ String* str);

    CARAPI GetProperty(
        /* [in] */ const String& name,
        /* [in] */ const String& defaultValue,
        /* [out] */ String* str);

    CARAPI List(
        /* [in] */ IPrintStream* outstream);

    CARAPI List(
        /* [in] */ IPrintWriter* outwriter);

    CARAPI Load(
        /* [in] */ IInputStream* instream);

    CARAPI Load(
        /* [in] */ IReader* inreader);

    CARAPI PropertyNames(
        /* [out] */ IEnumeration** names);

    CARAPI StringPropertyNames(
        /* [out] */ ISet** strNames);

    CARAPI Save(
        /* [in] */ IOutputStream* outstream,
        /* [in] */ const String& comment);

    CARAPI SetProperty(
        /* [in] */ const String& name,
        /* [in] */ const String& value ,
        /* [out] */ String* oldValue);

    CARAPI Store(
        /* [in] */ IOutputStream* outstream,
        /* [in] */ const String& comment);

    CARAPI Store(
        /* [in] */ IWriter* writer,
        /* [in] */ const String& comment);

    CARAPI LoadFromXML(
        /* [in] */ IInputStream* instream);

    CARAPI StoreToXML(
        /* [in] */ IOutputStream* os,
        /* [in] */ const String& comment);

    CARAPI StoreToXML(
        /* [in] */ IOutputStream* os,
        /* [in] */ const String& comment,
        /* [in] */ const String& encoding);

protected:
    Properties();

    CARAPI Init(
        /* [in] */ Properties* properties);

private:
    CARAPI_(void) DumpString(
        /* [in] */ IStringBuilder* buffer,
        /* [in] */ const String& string,
        /* [in] */ Boolean key);

    CARAPI ListToAppendable(
        /* [in] */ IAppendable* out);

    CARAPI SelectProperties(
        /* [in] */ IMap* selectProperties,
        /* [in] */ Boolean isStringOnly);

    CARAPI_(String) SubstitutePredefinedEntries(
        /* [in] */ const String& s);

protected:
    /**
    * The default values for keys not found in this {@code Properties}
    * instance.
     */
    AutoPtr<Properties> mDefaults;

private:
    const static String PROP_DTD_NAME;
    const static String PROP_DTD;

    const static Int32 NONE;
    const static Int32 SLASH;
    const static Int32 UNICODE;
    const static Int32 CONTINUE;
    const static Int32 KEY_DONE;
    const static Int32 IGNORE;

    // private transient DocumentBuilder builder = null;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_PROPERTIES_H__
