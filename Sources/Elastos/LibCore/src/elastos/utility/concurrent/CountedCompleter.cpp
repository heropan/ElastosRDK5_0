
#include "CountedCompleter.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CountedCompleter::
//====================================================================
CountedCompleter::CountedCompleter(
    /* [in] */ CountedCompleter* completer,
    /* [in] */ Int32 initialPendingCount)
{
    mCompleter = completer;
    mPending = initialPendingCount;
}

CountedCompleter::CountedCompleter(
    /* [in] */ CountedCompleter* completer)
{
    mCompleter = completer;
}

CountedCompleter::CountedCompleter()
{
    mCompleter = NULL;
}

void CountedCompleter::OnCompletion(
    /* [in] */ CountedCompleter* caller)
{
}

Boolean CountedCompleter::OnExceptionalCompletion(
    /* [in] */ IThrowable* ex,
    /* [in] */ CountedCompleter* caller)
{
    return TRUE;
}

AutoPtr<CountedCompleter> CountedCompleter::GetCompleter()
{
    return mCompleter;
}

Int32 CountedCompleter::GetPendingCount()
{
    return mPending;
}

void CountedCompleter::SetPendingCount(
    /* [in] */ Int32 count)
{
    mPending = count;
}

void CountedCompleter::AddToPendingCount(
    /* [in] */ Int32 delta)
{
//    Int32 c;
//    do {} while (!U.compareAndSwapInt(this, PENDING, c = pending, c+delta));
}

Boolean CountedCompleter::CompareAndSetPendingCount(
    /* [in] */ Int32 expected,
    /* [in] */ Int32 count)
{
    return FALSE;
//    return U.compareAndSwapInt(this, PENDING, expected, count);
}

Int32 CountedCompleter::DecrementPendingCountUnlessZero()
{
    Int32 c = 0;
    // do {} while ((c = pending) != 0 &&
    //              !U.compareAndSwapInt(this, PENDING, c, c - 1));
    return c;
}

AutoPtr<CountedCompleter> CountedCompleter::GetRoot()
{
    AutoPtr<CountedCompleter> a = this, p;
    while ((p = a->mCompleter) != NULL)
        a = p;
    return a;
}

void CountedCompleter::TryComplete()
{
    AutoPtr<CountedCompleter> a = this, s = a;
    for (Int32 c;;) {
        if ((c = a->mPending) == 0) {
            a->OnCompletion(s);
            if ((a = (s = a)->mCompleter) == NULL) {
                s->QuietlyComplete();
                return;
            }
        }
        // else if (U.compareAndSwapInt(a, PENDING, c, c - 1))
        //     return;
    }
}

void CountedCompleter::PropagateCompletion()
{
    AutoPtr<CountedCompleter> a = this, s = a;
    for (Int32 c;;) {
        if ((c = a->mPending) == 0) {
            if ((a = (s = a)->mCompleter) == NULL) {
                s->QuietlyComplete();
                return;
            }
        }
        // else if (U.compareAndSwapInt(a, PENDING, c, c - 1))
        //     return;
    }
}

ECode CountedCompleter::Complete(
    /* [in] */ IInterface* value)
{
    AutoPtr<CountedCompleter> p;
    SetRawResult(value);
    OnCompletion(this);
    QuietlyComplete();
    if ((p = mCompleter) != NULL)
        p->TryComplete();
    return NOERROR;
}

AutoPtr<CountedCompleter> CountedCompleter::FirstComplete()
{
    for (Int32 c;;) {
        if ((c = mPending) == 0)
            return this;
        // else if (U.compareAndSwapInt(this, PENDING, c, c - 1))
        //     return NULL;
    }
}

AutoPtr<CountedCompleter> CountedCompleter::NextComplete()
{
    AutoPtr<CountedCompleter> p;
    if ((p = mCompleter) != NULL)
        return p->FirstComplete();
    else {
        QuietlyComplete();
        return NULL;
    }
}

void CountedCompleter::QuietlyCompleteRoot()
{
    for (AutoPtr<CountedCompleter> a = this, p;;) {
        if ((p = a->mCompleter) == NULL) {
            a->QuietlyComplete();
            return;
        }
        a = p;
    }
}

void CountedCompleter::InternalPropagateException(
    /* [in] */ IThrowable* ex)
{
    AutoPtr<CountedCompleter> a = this, s = a;
    while (a->OnExceptionalCompletion(ex, s) &&
           (a = (s = a)->mCompleter) != NULL && a->mStatus >= 0 &&
           a->RecordExceptionalCompletion(ex) == EXCEPTIONAL)
        ;
}

ECode CountedCompleter::Exec(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)

    Compute();
    *res = FALSE;
    return NOERROR;
}

ECode CountedCompleter::GetRawResult(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    *outface = NULL;
    return NOERROR;
}

ECode CountedCompleter::SetRawResult(
    /* [in] */ IInterface* value)
{
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
