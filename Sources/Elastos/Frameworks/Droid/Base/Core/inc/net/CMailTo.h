
#ifndef __ELASTOS_DROID_NET_CCMailTo_H__
#define __ELASTOS_DROID_NET_CCMailTo_H__

#include "_Elastos_Droid_Net_CCMailTo.h"
#include "elautptr.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CCMailTo)
{
public:
    /**
     * Test to see if the given string is a mailto URL
     * @param url string to be tested
     * @return true if the string is a mailto URL
     */
    CARAPI IsMailTo(
        /* [in] */ const String& url,
        /* [out] */ Boolean* result);

    /**
     * Parse and decode a mailto scheme string. This parser implements
     * RFC 2368. The returned object can be queried for the parsed parameters.
     * @param url String containing a mailto URL
     * @return MailTo object
     * @exception ParseException if the scheme is not a mailto URL
     */
    CARAPI Parse(
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

    CARAPI ToString(
        /* [out] */ String* result);

    /**
     * Retrieve all the parsed email headers from the mailto URL
     * @return map containing all parsed values
     */
    CARAPI GetHeaders(
        /* [out, callee] */ IObjectStringMap** rethashmap);


private:
    AutoPtr <HashMap<String, String>> mHeaders;
    static const String TO = "to";
    static const String BODY = "body";
    static const String CC = "cc";
    static const String SUBJECT = "subject";

    CCMailTo();
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos


#endif // __CMailTo_H__
