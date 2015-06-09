
#ifndef __ELASTOS_IO_FILEWRITER_H__
#define __ELASTOS_IO_FILEWRITER_H__

#include "OutputStreamWriter.h"

namespace Elastos {
namespace IO {

class FileWriter
    : public OutputStreamWriter
    , public IFileWriter
{
public:
	CAR_INTERFACE_DECL()
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_FILEWRITER_H__
