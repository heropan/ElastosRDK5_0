
#include "CTableResult.h"
#include <elastos/core/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CTableResult);

ECode CTableResult::constructor()
{
    return TableResult::Init();
}

ECode CTableResult::constructor(
    /* [in] */ Int32 maxrows)
{
    return TableResult::Init(maxrows);
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
