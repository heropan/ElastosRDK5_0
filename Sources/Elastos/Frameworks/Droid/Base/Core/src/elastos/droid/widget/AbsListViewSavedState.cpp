
#include "elastos/droid/widget/AbsListViewSavedState.h"

namespace Elastos{
namespace Droid{
namespace Widget{

AbsListViewSavedState::AbsListViewSavedState()
    : mSelectedId(-1)
    , mFirstId(-1)
    , mViewTop(0)
    , mPosition(0)
    , mHeight(0)
    , mInActionMode(FALSE)
    , mCheckedItemCount(0)
{
}

AbsListViewSavedState::AbsListViewSavedState(
    /* [in] */ IParcelable* superState)
    : ViewBaseSavedState(superState)
    , mSelectedId(-1)
    , mFirstId(-1)
    , mViewTop(0)
    , mPosition(0)
    , mHeight(0)
    , mInActionMode(FALSE)
    , mCheckedItemCount(0)
{
}


AbsListViewSavedState::~AbsListViewSavedState()
{
    mCheckState = NULL;
    mCheckIdState = NULL;
}

ECode AbsListViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(ViewBaseSavedState::WriteToParcel(dest));

    dest->WriteInt64(mSelectedId);
    dest->WriteInt64(mFirstId);
    dest->WriteInt32(mViewTop);
    dest->WriteInt32(mPosition);
    dest->WriteInt32(mHeight);
    dest->WriteString(mFilter);

    dest->WriteByte((Byte) (mInActionMode ? 1 : 0));
    dest->WriteInt32(mCheckedItemCount);

    Int32 M = mCheckState != NULL ? mCheckState->GetSize() : 0;
    dest->WriteInt32(M);
    HashMap<Int32, Boolean>::Iterator it = mCheckState->Begin();
    for(Int32 i=0; i<M || it != mCheckState->End(); i++, it++)
    {
        dest->WriteInt32(it->mFirst);
        dest->WriteBoolean(it->mSecond);
    }

    Int32 N = mCheckIdState != NULL ? mCheckIdState->GetSize() : 0;
    dest->WriteInt32(N);
    HashMap<Int64, Int32>::Iterator it2 = mCheckIdState->Begin();
    for(Int32 i=0; i<N || it2 != mCheckIdState->End(); i++, it2++)
    {
        dest->WriteInt64(it2->mFirst);
        dest->WriteInt32(it2->mSecond);
    }

    return NOERROR;
}

ECode AbsListViewSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(ViewBaseSavedState::ReadFromParcel(source));
    source->ReadInt64(&mSelectedId);
    source->ReadInt64(&mFirstId);
    source->ReadInt32(&mViewTop);
    source->ReadInt32(&mPosition);
    source->ReadInt32(&mHeight);
    source->ReadString(&mFilter);
    Char8 value;
    source->ReadChar8(&value);
    mInActionMode = value != 0;
    source->ReadInt32(&mCheckedItemCount);

    Int32 M;
    source->ReadInt32(&M);
    if(M>0)
    {
        if(!mCheckState)
            mCheckState = new HashMap<Int32, Boolean>();
        for(Int32 i=0; i<M; i++)
        {
            Int32 key ;
            source->ReadInt32(&key);
            Boolean value;
            source->ReadBoolean(&value);
            (*mCheckState)[key] = value;
        }
    }

    Int32 N;
    source->ReadInt32(&N);
    if(N>0)
    {
        if(!mCheckIdState)
        mCheckIdState = new HashMap<Int64, Int32>();
        for(Int32 i=0; i<N; i++)
        {
            Int64 key ;
            source->ReadInt64(&key);
            Int32 value;
            source->ReadInt32(&value);
            (*mCheckIdState)[key] = value;
        }
    }
    return NOERROR;
}

ECode AbsListViewSavedState::Init()
{
    return ViewBaseSavedState::Init();
}

ECode AbsListViewSavedState::Init(
    /* [in] */ IParcelable* superState)
{
    return ViewBaseSavedState::Init(superState);
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
