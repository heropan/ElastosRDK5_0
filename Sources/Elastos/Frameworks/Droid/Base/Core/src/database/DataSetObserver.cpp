
#include "database/DataSetObserver.h"

namespace Elastos {
namespace Droid {
namespace Database {

ECode DataSetObserver::OnChanged()
{
    // Do nothing
    return NOERROR;
}

ECode DataSetObserver::OnInvalidated()
{
    // Do nothing
    return NOERROR;
}

} //Database
} //Droid
} //Elastos
