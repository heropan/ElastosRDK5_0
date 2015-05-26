
#ifndef __CONTENTTYPE_H__
#define __CONTENTTYPE_H__

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

/**
 * This class incapsulates the constants determining the
 * types of SSL/TLS record's content data.
 * Constant values are taken according to the TLS v1 specification
 * (http://www.ietf.org/rfc/rfc2246.txt).
 */

class ContentType {
    /**
     * Identifies change cipher spec message
     */
protected:
    static const Byte CHANGE_CIPHER_SPEC = 20;

    /**
     * Identifies alert message
     */
    static const Byte ALERT = 21;

    /**
     * Identifies handshake message
     */
    static const Byte HANDSHAKE = 22;

    /**
     * Identifies application data message
     */
    static const Byte APPLICATION_DATA = 23;
};

} // namespace Jsse
} // namespace Provider
} // namespace Xnet
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CONTENTTYPE_H__
