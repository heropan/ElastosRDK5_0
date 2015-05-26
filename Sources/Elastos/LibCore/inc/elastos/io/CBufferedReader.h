
#ifndef __CBUFFEREDREADER_H__
#define __CBUFFEREDREADER_H__

#include "_CBufferedReader.h"
#include "BufferedReader.h"

namespace Elastos {
namespace IO {

CarClass(CBufferedReader)
    , public BufferedReader
{
public:
    CAR_INTERFACE_DECL()
    CAR_OBJECT_DECL()
};

} // namespace IO
} // namespace Elastos

#endif // __CBUFFEREDREADER_H__
