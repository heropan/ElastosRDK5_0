
#ifndef __ELASTOS_IO_CPRINTWRITR_H__
#define __ELASTOS_IO_CPRINTWRITR_H__

#include "_Elastos_IO_CPrintWriter.h"
#include "elastos/io/PrintWriter.h"
#include <elastos.h>

using Libcore::ICU::ILocale;

namespace Elastos {
namespace IO {

CarClass(CPrintWriter) , public PrintWriter
{
public:
    CAR_OBJECT_DECL()
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CPRINTWRITR_H__
