
#ifndef __ELASTOS_IO_FILEREADER_H__
#define __ELASTOS_IO_FILEREADER_H__

#include "InputStreamReader.h"

namespace Elastos {
namespace IO {

class FileReader
    : public InputStreamReader
    , public IFileReader
{
public:
	CAR_INTERFACE_DECL()
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_FILEREADER_H__
