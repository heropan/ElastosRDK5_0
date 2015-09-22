
#ifndef __CPATHSEGMENTSBUILDER_H__
#define __CPATHSEGMENTSBUILDER_H__

#include "_CPathSegmentsBuilder.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

/**
 * Builds PathSegments.
 */
class PathSegmentsBuilder
{
private:
    List<String> mSegments;
    Int32 mSize;

public:
    PathSegmentsBuilder();

    virtual CARAPI Add(
        /* [in] */ const String& segment);

    virtual CARAPI Build(
        /* [out] */ IPathSegments** pathSegments);
};

CarClass(CPathSegmentsBuilder), public PathSegmentsBuilder
{
public:
    CARAPI Add(
        /* [in] */ const String& segment);

    CARAPI Build(
        /* [out] */ IPathSegments** pathSegments);

private:
};

#endif // __CPATHSEGMENTSBUILDER_H__
