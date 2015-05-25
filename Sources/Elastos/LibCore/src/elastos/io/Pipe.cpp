#include "Pipe.h"

namespace Elastos{
namespace IO{
namespace Channels{

Pipe::Pipe()
{

}

ECode Pipe::Open(IPipe** pipe)
{
    printf("ERROR: Need SelectorProvider.Provider()\n");
    return E_NOT_IMPLEMENTED;
}

} // namespace Channels
} // namespace IO
} // namespace Elastoss
