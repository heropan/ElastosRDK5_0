#ifndef __CSOCKETTAGGERhELPER_H__
#define __CSOCKETTAGGERhELPER_H__

#include "SocketTagger.h"
#include "_CSocketTaggerHelper.h"

namespace Libcore {
namespace IO {

CarClass(CSocketTaggerHelper)
{
public:
    /**
     * Sets this process' socket tagger to {@code tagger}.
     */
    CARAPI Set(
        /* [in] */ ISocketTagger* tagger);

    /**
     * Returns this process socket tagger.
     */
    CARAPI Get(
        /* [out] */ ISocketTagger** tagger);
};

} // namespace IO
} // namespace Libcore

#endif // __CSOCKETTAGGERhELPER_H__