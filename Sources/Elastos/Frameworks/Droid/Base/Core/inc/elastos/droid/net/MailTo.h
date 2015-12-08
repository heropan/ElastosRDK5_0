
#ifndef __ELASTOS_DROID_NET_MAILTO_H__
#define __ELASTOS_DROID_NET_MAILTO_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Utility::IHashMap;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Net {

class MailTo
    : public Object
    , public IMailTo
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Test to see if the given string is a mailto URL
     * @param url string to be tested
     * @return true if the string is a mailto URL
     */
    static CARAPI IsMailTo(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    /**
     * Parse and decode a mailto scheme string. This parser implements
     * RFC 2368. The returned object can be queried for the parsed parameters.
     * @param url String containing a mailto URL
     * @return MailTo object
     * @exception ParseException if the scheme is not a mailto URL
     */
    static CARAPI Parse(
        /* [in] */ const String& url,
        /* [out] */ IMailTo** result);

    /**
     * Retrieve the To address line from the parsed mailto URL. This could be
     * several email address that are comma-space delimited.
     * If no To line was specified, then null is return
     * @return comma delimited email addresses or null
     */
    CARAPI GetTo(
        /* [out] */ String* result);

    /**
     * Retrieve the CC address line from the parsed mailto URL. This could be
     * several email address that are comma-space delimited.
     * If no CC line was specified, then null is return
     * @return comma delimited email addresses or null
     */
    CARAPI GetCc(
        /* [out] */ String* result);

    /**
     * Retrieve the subject line from the parsed mailto URL.
     * If no subject line was specified, then null is return
     * @return subject or null
     */
    CARAPI GetSubject(
        /* [out] */ String* result);

    /**
     * Retrieve the body line from the parsed mailto URL.
     * If no body line was specified, then null is return
     * @return body or null
     */
    CARAPI GetBody(
        /* [out] */ String* result);

    // @Override
    CARAPI ToString(
        /* [out] */ String* result);

    /**
     * Retrieve all the parsed email headers from the mailto URL
     * @return map containing all parsed values
     */
    CARAPI GetHeaders(
        /* [out] */ IMap** rethashmap);

private:
    CARAPI constructor();

    CARAPI GetField(
        /* [in] */ const String& name,
        /* [out] */ String* result);

private:
    AutoPtr <IHashMap> mHeaders;

    // Well known headers
    static const String TO;
    static const String BODY;
    static const String CC;
    static const String SUBJECT;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_MAILTO_H__
