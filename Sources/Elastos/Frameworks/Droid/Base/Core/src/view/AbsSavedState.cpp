
#include "view/AbsSavedState.h"
#include "view/CEmptyAbsSavedState.h"

namespace Elastos {
namespace Droid {
namespace View {

static AutoPtr<IAbsSavedState> InitEmptySs()
{
    AutoPtr<IAbsSavedState> rst;
    CEmptyAbsSavedState::New((IAbsSavedState**)&rst);
    return rst;
}

const AutoPtr<IAbsSavedState> AbsSavedState::EMPTY_STATE = InitEmptySs();

/**
 * Constructor used to make the EMPTY_STATE singleton
 */
AbsSavedState::AbsSavedState()
{
}


AbsSavedState::~AbsSavedState()
{
}
/**
 * Constructor called by derived classes when creating their SavedState objects
 *
 * @param superState The state of the superclass of this view
 */
AbsSavedState::AbsSavedState(
    /* [in] */ IParcelable* superState)
{
    if (superState == NULL) {
        assert(0);
    //    throw new IllegalArgumentException("superState must not be null");
    }
    mSuperState = superState != IParcelable::Probe(EMPTY_STATE.Get()) ? superState : NULL;
}

/**
 * Constructor used when reading from a parcel. Reads the state of the superclass.
 *
 * @param source
 */
AbsSavedState::AbsSavedState(
    /* [in] */ IParcel* source)
{
    // FIXME need class loader
    //Parcelable superState = source->ReadParcelable(null);
    //
    //mSuperState = superState != null ? superState : EMPTY_STATE;
}

AutoPtr<IParcelable> AbsSavedState::GetSuperState()
{
    return mSuperState;
}

ECode AbsSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInterfacePtr(mSuperState);
    //dest.writeParcelable(mSuperState, flags);
    return NOERROR;
}

ECode AbsSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IParcelable> superState;
    source->ReadInterfacePtr((Handle32*)&superState);
    if(superState != NULL)
    {
        mSuperState = superState;
    }else{
        mSuperState = IParcelable::Probe(EMPTY_STATE.Get());
    }

    return NOERROR;
}

ECode AbsSavedState::Init()
{
    return NOERROR;
}

ECode AbsSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    if (superState == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    //    throw new IllegalArgumentException("superState must not be null");
    }
    mSuperState = superState != IParcelable::Probe(EMPTY_STATE.Get()) ? superState : NULL;

    return NOERROR;
}

ECode AbsSavedState::Init(
    /* [in] */ IParcel* source)
{
    // FIXME need class loader
    //Parcelable superState = source->ReadParcelable(null);
    //
    //mSuperState = superState != null ? superState : EMPTY_STATE;
    // You need call Init() and then invoke ReadFromParcel() to instead of;

    return E_NOT_IMPLEMENTED;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
