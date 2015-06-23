
#include "ext/frameworkdef.h"
#include "os/CWorkSource.h"

namespace Elastos {
namespace Droid {
namespace Os {

static AutoPtr<IWorkSource> InitsTmpWorkSource()
{
    AutoPtr<CWorkSource> ws;
    CWorkSource::NewByFriend(0, (CWorkSource**)&ws);
    return (IWorkSource*)ws.Get();
}

const AutoPtr<IWorkSource> CWorkSource::sTmpWorkSource = InitsTmpWorkSource();
Mutex CWorkSource::sTmpWorkSourceLock;
AutoPtr<IWorkSource> CWorkSource::sNewbWork;
AutoPtr<IWorkSource> CWorkSource::sGoneWork;

CWorkSource::CWorkSource()
    : mNum(0)
{}

ECode CWorkSource::constructor()
{
    return NOERROR;
}

ECode CWorkSource::constructor(
    /* [in] */ IWorkSource* orig)
{
    if (orig == NULL) {
        return NOERROR;
    }
    CWorkSource* ws = (CWorkSource*)orig;
    mNum = ws->mNum;
    if (ws->mUids != NULL) {
        mUids = ws->mUids->Clone();
    }
    return NOERROR;
}

ECode CWorkSource::constructor(
    /* [in] */ Int32 uid)
{
    mNum = 1;
    mUids = ArrayOf<Int32>::Alloc(2);
    (*mUids)[0] = uid;
    (*mUids)[1] = 0;
    return NOERROR;
}

ECode CWorkSource::GetSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mNum;
    return NOERROR;
}

ECode CWorkSource::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = (*mUids)[index];
    return NOERROR;
}

ECode CWorkSource::Clear()
{
    mNum = 0;
    return NOERROR;
}

ECode CWorkSource::Equals(
    /* [in] */ IWorkSource* o,
    /* [out] */ Boolean* equal)
{
    VALIDATE_NOT_NULL(equal);
    Boolean difference;
    FAIL_RETURN(Diff(o, &difference));
    *equal = !difference;
    return NOERROR;
}

ECode CWorkSource::Equals(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* equal)
{
    VALIDATE_NOT_NULL(equal);
    return Equals(IWorkSource::Probe(o), equal);
}

ECode CWorkSource::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    Int32 result = 0;
    for (int i = 0; i < mNum; i++) {
        result = ((result << 4) | (((Int64)result) >> 28)) ^ (*mUids)[i];
    }
    *hash = result;
    return NOERROR;
}

ECode CWorkSource::Diff(
    /* [in] */ IWorkSource* other,
    /* [out] */ Boolean* difference)
{
    VALIDATE_NOT_NULL(difference);
    CWorkSource* otherObj = (CWorkSource*)other;
    Int32 N = mNum;
    if (N != otherObj->mNum) {
        *difference = TRUE;
        return NOERROR;
    }
    AutoPtr< ArrayOf<Int32> > uids1 = mUids;
    AutoPtr< ArrayOf<Int32> > uids2 = otherObj->mUids;
    for (Int32 i = 0; i < N; i++) {
        if ((*uids1)[i] != (*uids2)[i]) {
            *difference = TRUE;
            return NOERROR;
        }
    }
    *difference = FALSE;
    return NOERROR;
}

ECode CWorkSource::Set(
    /* [in] */ IWorkSource* other)
{
    if (other == NULL) {
        mNum = 0;
        return NOERROR;
    }
    CWorkSource* otherObj = (CWorkSource*)other;
    mNum = otherObj->mNum;
    if (otherObj->mUids != NULL) {
        if (mUids != NULL && mUids->GetLength() >= mNum) {
            mUids->Copy(otherObj->mUids, mNum);
        }
        else {
            mUids = otherObj->mUids->Clone();
        }
    }
    else {
        mUids = NULL;
    }
    return NOERROR;
}

ECode CWorkSource::Set(
    /* [in] */ Int32 uid)
{
    mNum = 1;
    if (mUids == NULL) mUids = ArrayOf<Int32>::Alloc(2);
    (*mUids)[0] = uid;
    return NOERROR;
}

ECode CWorkSource::SetReturningDiffs(
    /* [in] */ IWorkSource* other,
    /* [out, callee] */ ArrayOf<IWorkSource*>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    {
        Mutex::Autolock lock(sTmpWorkSourceLock);

        sNewbWork = NULL;
        sGoneWork = NULL;
        UpdateLocked(other, TRUE, TRUE);
        if (sNewbWork != NULL || sGoneWork != NULL) {
            AutoPtr< ArrayOf<IWorkSource*> > diffs = ArrayOf<IWorkSource*>::Alloc(2);
            diffs->Set(0, sNewbWork);
            diffs->Set(1, sGoneWork);
            *result = diffs;
            REFCOUNT_ADD(*result);
        }
    }

    return NOERROR;
}

ECode CWorkSource::Add(
    /* [in] */ IWorkSource* other,
    /* [out] */ Boolean* added)
{
    VALIDATE_NOT_NULL(added);
    {
        Mutex::Autolock lock(sTmpWorkSourceLock);
        *added = UpdateLocked(other, FALSE, FALSE);
        return NOERROR;
    }
}

ECode CWorkSource::AddReturningNewbs(
    /* [in] */ IWorkSource* other,
    /* [out] */ IWorkSource** result)
{
    VALIDATE_NOT_NULL(result);
    {
        Mutex::Autolock lock(sTmpWorkSourceLock);
        sNewbWork = NULL;
        UpdateLocked(other, FALSE, TRUE);
        *result = sNewbWork;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
}

ECode CWorkSource::Add(
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* added)
{
    VALIDATE_NOT_NULL(added);
    {
        Mutex::Autolock lock(sTmpWorkSourceLock);
        ((CWorkSource*)sTmpWorkSource.Get())->mUids->Set(0, uid);
        *added = UpdateLocked(sTmpWorkSource, FALSE, FALSE);
        return NOERROR;
    }
}

ECode CWorkSource::AddReturningNewbs(
    /* [in] */ Int32 uid,
    /* [out] */ IWorkSource** result)
{
    VALIDATE_NOT_NULL(result);
    {
        Mutex::Autolock lock(sTmpWorkSourceLock);
        sNewbWork = NULL;
        ((CWorkSource*)sTmpWorkSource.Get())->mUids->Set(0, uid);
        UpdateLocked(sTmpWorkSource, FALSE, TRUE);
        *result = sNewbWork;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
}

ECode CWorkSource::Remove(
    /* [in] */ IWorkSource* other,
    /* [out] */ Boolean* successed)
{
    VALIDATE_NOT_NULL(successed);
    CWorkSource* otherObj = (CWorkSource*)other;
    Int32 N1 = mNum;
    AutoPtr< ArrayOf<Int32> > uids1 = mUids;
    Int32 N2 = otherObj->mNum;
    AutoPtr< ArrayOf<Int32> > uids2 = otherObj->mUids;
    Boolean changed = FALSE;
    Int32 i1 = 0;
    for (Int32 i2 = 0; i2 < N2 && i1 < N1; i2++) {
        if ((*uids2)[i2] == (*uids1)[i1]) {
            N1--;
            if (i1 < N1) uids1->Copy(i1, uids1, i1 + 1, N1 - i1);
        }
        while (i1 < N1 && (*uids2)[i2] > (*uids1)[i1]) {
            i1++;
        }
    }

    mNum = N1;

    *successed = changed;
    return NOERROR;
}

Boolean CWorkSource::UpdateLocked(
    /* [in] */ IWorkSource* other,
    /* [in] */ Boolean set,
    /* [in] */ Boolean returnNewbs)
{
    CWorkSource* otherObj = (CWorkSource*)other;
    Int32 N1 = mNum;
    AutoPtr< ArrayOf<Int32> > uids1 = mUids;
    Int32 N2 = otherObj->mNum;
    AutoPtr< ArrayOf<Int32> > uids2 = otherObj->mUids;
    Boolean changed = FALSE;
    Int32 i1 = 0;
    for (Int32 i2 = 0; i2 < N2; i2++) {
        if (i1 >= N1 || (*uids2)[i2] < (*uids1)[i1]) {
            // Need to insert a new uid.
            changed = TRUE;
            if (uids1 == NULL) {
                uids1 = ArrayOf<Int32>::Alloc(4);
                (*uids1)[0] = (*uids2)[i2];
            }
            else if (i1 >= uids1->GetLength()) {
                AutoPtr< ArrayOf<Int32> > newuids = ArrayOf<Int32>::Alloc((uids1->GetLength() * 3) / 2);
                if (i1 > 0) newuids->Copy(uids1, i1);
                if (i1 < N1) newuids->Copy(i1 + 1, uids1, i1, N1 - i1);
                uids1 = newuids;
                (*uids1)[i1] = (*uids2)[i2];
            }
            else {
                if (i1 < N1) uids1->Copy(i1 + 1, uids1, i1, N1 - i1);
                (*uids1)[i1] = (*uids2)[i2];
            }
            if (returnNewbs) {
                if (sNewbWork == NULL) {
                    CWorkSource::New((*uids2)[i2], (IWorkSource**)&sNewbWork);
                }
                else {
                    ((CWorkSource*)sNewbWork.Get())->AddLocked((*uids2)[i2]);
                }
            }
            N1++;
            i1++;
        }
        else {
            if (!set) {
                // Skip uids that already exist or are not in 'other'.
                do {
                    i1++;
                } while (i1 < N1 && (*uids2)[i2] >= (*uids1)[i1]);
            }
            else {
                // Remove any uids that don't exist in 'other'.
                Int32 start = i1;
                while (i1 < N1 && (*uids2)[i2] > (*uids1)[i1]) {
                    if (sGoneWork == NULL) {
                        CWorkSource::New((*uids1)[i1], (IWorkSource**)&sGoneWork);
                    }
                    else {
                        ((CWorkSource*)sGoneWork.Get())->AddLocked((*uids1)[i1]);
                    }
                    i1++;
                }
                if (start < i1) {
                    uids1->Copy(start, uids1, i1, i1 - start);
                    N1 -= i1 - start;
                    i1 = start;
                }
                // If there is a matching uid, skip it.
                if (i1 < N1 && (*uids2)[i1] == (*uids1)[i1]) {
                    i1++;
                }
            }
        }
    }

    mNum = N1;
    mUids = uids1;

    return changed;
}

void CWorkSource::AddLocked(
    /* [in] */ Int32 uid)
{
    if (mUids == NULL) {
        mUids = ArrayOf<Int32>::Alloc(4);
        (*mUids)[0] = uid;
        mNum = 1;
        return;
    }
    if (mNum >= mUids->GetLength()) {
        AutoPtr< ArrayOf<Int32> > newuids = ArrayOf<Int32>::Alloc((mNum * 3) / 2);
        newuids->Copy(mUids, mNum);
        mUids = newuids;
    }

    (*mUids)[mNum] = uid;
    mNum++;
}

ECode CWorkSource::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mNum);
    source->ReadArrayOf((Handle32*)&mUids);
    return NOERROR;
}

ECode CWorkSource::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mNum);
    dest->WriteArrayOf((Handle32)mUids.Get());
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
