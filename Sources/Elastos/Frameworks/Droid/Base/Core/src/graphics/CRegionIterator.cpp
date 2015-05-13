
#include "ext/frameworkext.h"
#include "graphics/CRegionIterator.h"
#include "graphics/CRegion.h"
#include "graphics/CRect.h"
#include "graphics/GraphicsNative.h"
#include <skia/core/SkRegion.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

struct RgnIterPair
{
    RgnIterPair(const SkRegion& rgn) : fRgn(rgn)
    {
        // have our iterator reference our copy (fRgn), so we know it will be
        // unchanged for the lifetime of the iterator
        fIter.reset(fRgn);
    }

    SkRegion fRgn;   // a copy of the caller's region
    SkRegion::Iterator fIter;  // an iterator acting upon the copy (fRgn)
};

CRegionIterator::~CRegionIterator()
{
    NativeDestructor(mNativeIter);
}

ECode CRegionIterator::constructor(
    /* [in] */ IRegion* region)
{
    mNativeIter = NativeConstructor(((CRegion*)region)->Ni());
    return NOERROR;
}

ECode CRegionIterator::Next(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (r == NULL) {
        // throw new NullPointerException("The Rect must be provided");
        return E_NULL_POINTER_EXCEPTION;
    }
    *result = NativeNext(mNativeIter, r);
    return NOERROR;
}

Int32 CRegionIterator::NativeConstructor(
    /* [in] */ Int32 nativeRegion)
{
    SkASSERT(nativeRegion);
    return (Int32)new RgnIterPair(*(SkRegion*)nativeRegion);
}

void CRegionIterator::NativeDestructor(
    /* [in] */ Int32 nativeIter)
{
    SkASSERT(nativeIter);
    delete (RgnIterPair*)nativeIter;
}

Boolean CRegionIterator::NativeNext(
    /* [in] */ Int32 nativeIter,
    /* [in] */ IRect* r)
{
    // the caller has checked that rectObject is not nul
    SkASSERT(nativeIter);
    SkASSERT(r);

    RgnIterPair* pair = (RgnIterPair*)nativeIter;
    if (!pair->fIter.done()) {
        GraphicsNative::SkIRect2IRect(pair->fIter.rect(), r);
        pair->fIter.next();
        return TRUE;
    }
    return FALSE;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
