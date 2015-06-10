
#include "CProperties.h"

namespace Elastos{
namespace Utility{

CAR_OBJECT_IMPL(CProperties)

ECode CProperties::constructor()
{
    return HashTable::Init();
}

ECode CProperties::constructor(
    /* [in] */ IProperties* properties)
{
    FAIL_RETURN(HashTable::Init());
    return Properties::Init((CProperties*)properties);
}

Mutex* CProperties::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Utility
} // namespace Elastos
