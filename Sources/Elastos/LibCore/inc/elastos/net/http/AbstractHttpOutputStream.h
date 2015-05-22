
#ifndef __ABSTRACTHTTPOUTPUTSTREAM_H__
#define __ABSTRACTHTTPOUTPUTSTREAM_H__

#include "Elastos.CoreLibrary_server.h"
#include "OutputStream.h"

using Elastos::IO::OutputStream;

namespace Elastos {
namespace Net {
namespace Http {

class AbstractHttpOutputStream : public OutputStream
{
public:
    AbstractHttpOutputStream();
    // @Override
    CARAPI Write(
        /* [in] */ Int32 value);

protected:
    CARAPI CheckNotClosed();

protected:
    Boolean mClosed;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__ABSTRACTHTTPOUTPUTSTREAM_H__
