
#ifndef __CBUFFEREDWRITER_H__
#define __CBUFFEREDWRITER_H__

#include "_Elastos_IO_CBufferedWriter.h"
#include "BufferedWriter.h"

namespace Elastos {
namespace IO {

CarClass(CBufferedWriter)
    ,public BufferedWriter
{
public:
    CAR_INTERFACE_DECL()
    CAR_OBJECT_DECL()
};

} // namespace IO
} // namespace Elastos

#endif // __CBUFFEREDWRITER_H__
