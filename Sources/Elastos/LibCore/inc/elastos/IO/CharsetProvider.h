#ifndef __CHARSETPROVIDER_H__
#define __CHARSETPROVIDER_H__

#include <cmdef.h>
#include "Elastos.Core_server.h"

namespace Elastos {
namespace IO {
namespace Charset {
namespace Spi {

class CharsetProvider
    : public ElRefBase
    , public ICharsetProvider
{
public:
    /**
     * Constructor for subclassing with concrete types.
     */
    CharsetProvider();

    virtual ~CharsetProvider();

    CAR_INTERFACE_DECL()

};

} // namespace Spi
} // namespace CharSet
} // namespace IO
} // namespace Elastos

#endif //__CHARSETPROVIDER_H__
