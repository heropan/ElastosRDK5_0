
#include "cmdef.h"
#include "CSocketTaggerHelper.h"

namespace Libcore {
namespace IO {

ECode CSocketTaggerHelper::Set(
    /* [in] */ ISocketTagger* tagger)
{
    return SocketTagger::Set(tagger);
}

ECode CSocketTaggerHelper::Get(
    /* [out] */ ISocketTagger** tagger)
{
    return SocketTagger::Get(tagger);
}

} // namespace IO
} // namespace Libcore