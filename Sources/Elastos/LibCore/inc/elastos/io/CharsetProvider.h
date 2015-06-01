#ifndef __ELASTOS_IO_CHARSETPROVIDER_H__
#define __ELASTOS_IO_CHARSETPROVIDER_H__

#include <coredef.h>
#include "Elastos.CoreLibrary_server.h"

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

#endif //__ELASTOS_IO_CHARSETPROVIDER_H__
