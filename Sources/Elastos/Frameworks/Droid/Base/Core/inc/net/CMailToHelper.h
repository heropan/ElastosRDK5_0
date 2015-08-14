
#ifndef __ELASTOS_DROID_NET_CCMailToHelper_H__
#define __ELASTOS_DROID_NET_CCMailToHelper_H__

#include "_CCMailTo.h"
#include "net/CMailTo.h"
#include "elautptr.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CCMailToHelper)
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

};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CCMailToHelper_H__
