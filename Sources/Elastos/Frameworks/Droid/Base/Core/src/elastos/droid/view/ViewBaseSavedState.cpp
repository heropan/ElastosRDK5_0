
#include "view/ViewBaseSavedState.h"

namespace Elastos{
namespace Droid{
namespace View{

ViewBaseSavedState::ViewBaseSavedState()
{
}

ViewBaseSavedState::ViewBaseSavedState(
    /* [in] */ IParcelable* superState)
    : AbsSavedState(superState)
{
}

ViewBaseSavedState::ViewBaseSavedState(
    /* [in] */ IParcel* source)
    : AbsSavedState(source)
{
}

ViewBaseSavedState::~ViewBaseSavedState()
{
}

}// namespace View
}// namespace Droid
}// namespace Elastos
