
#ifndef __CBUFFEREDOUTPUTSTREAM_H__
#define __CBUFFEREDOUTPUTSTREAM_H__

#include "_CBufferedOutputStream.h"
#include "BufferedOutputStream.h"

namespace Elastos {
namespace IO {

CarClass(CBufferedOutputStream)
    , public BufferedOutputStream
{
public:
    CAR_OBJECT_DECL()

};

} // namespace IO
} // namespace Elastos

#endif // __CBUFFEREDOUTPUTSTREAM_H__
