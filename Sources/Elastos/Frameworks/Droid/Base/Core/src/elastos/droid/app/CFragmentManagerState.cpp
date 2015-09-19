
#include "app/CFragmentManagerState.h"
#include "app/CBackStackState.h"

namespace Elastos {
namespace Droid {
namespace App {

CFragmentManagerState::CFragmentManagerState()
{}

ECode CFragmentManagerState::constructor()
{
    return NOERROR;
}

ECode CFragmentManagerState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 value;
    AutoPtr<IParcelable> parcelable;

    mActive = NULL;
    source->ReadInt32(&value);
    if (value >= 0) {
        Int32 size = value;
        mActive = ArrayOf<FragmentState*>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<FragmentState> nc;
            source->ReadInt32(&value);
            if (value != 0) {
                assert(0);
//                FragmentState::New((FragmentState**)&nc);
                parcelable = IParcelable::Probe(nc);
                parcelable->ReadFromParcel(source);
            }
            mActive->Set(i, nc);
        }
    }

    mAdded = NULL;
    source->ReadArrayOf((Handle32*)(&mAdded));

    mBackStack = NULL;
    source->ReadInt32(&value);
    if (value >= 0) {
        Int32 size = value;
        mBackStack = ArrayOf<IBackStackState*>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IBackStackState> nc;
            source->ReadInt32(&value);
            if (value != 0) {
                CBackStackState::New((IBackStackState**)&nc);
                parcelable = IParcelable::Probe(nc);
                parcelable->ReadFromParcel(source);
            }
            mBackStack->Set(i, nc);
        }
    }
    return NOERROR;
}

ECode CFragmentManagerState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    AutoPtr<IParcelable> parcelable;

    if (mActive == NULL) {
        dest->WriteInt32(-1);
    }
    else {
        Int32 size = mActive->GetLength();
        dest->WriteInt32(size);
        AutoPtr<FragmentState> nc;
        for (Int32 i = 0; i < size; ++i) {
            nc = (*mActive)[i];
            if (nc != NULL) {
                dest->WriteInt32(1);
                parcelable = IParcelable::Probe(nc);
                parcelable->WriteToParcel(dest);
            } else {
                dest->WriteInt32(0);
            }
        }
    }

    dest->WriteArrayOf((Handle32)mAdded.Get());

    if (mBackStack == NULL) {
        dest->WriteInt32(-1);
    }
    else {
        Int32 size = mBackStack->GetLength();
        dest->WriteInt32(size);
        AutoPtr<IBackStackState> nc;
        for (Int32 i = 0; i < size; ++i) {
            nc = (*mBackStack)[i];
            if (nc != NULL) {
                dest->WriteInt32(1);
                parcelable = IParcelable::Probe(nc);
                parcelable->WriteToParcel(dest);
            } else {
                dest->WriteInt32(0);
            }
        }
    }
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos