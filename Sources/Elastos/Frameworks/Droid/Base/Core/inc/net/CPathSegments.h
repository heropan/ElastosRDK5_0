
#ifndef __CPATHSEGMENTS_H__
#define __CPATHSEGMENTS_H__

#include "_CPathSegments.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

/**
 * Wrapper for path segment array.
 */
class PathSegments
{
private:
    List<String> mSegments;
    Int32 mSize;

public:

    static AutoPtr<IPathSegments> EMPTY;

    PathSegments();

    CARAPI Init(
        /* [in] */ const ArrayOf<String>& segments,
        /* [in] */ Int32 size);

    virtual CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ String* value);

    virtual CARAPI Size(
        /* [out] */ Int32* value);
};

CarClass(CPathSegments), public PathSegments
{
public:
    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ String* value);

    CARAPI Size(
        /* [out] */ Int32* size);

    CARAPI constructor(
        /* [in] */ const ArrayOf<String>& segments,
        /* [in] */ Int32 size);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPATHSEGMENTS_H__
