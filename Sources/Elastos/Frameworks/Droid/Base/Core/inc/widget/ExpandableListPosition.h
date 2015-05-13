
#ifndef __EXPANDABLELISTPOSITION_H__
#define __EXPANDABLELISTPOSITION_H__

#include "ext/frameworkext.h"
#include <elastos/Mutex.h>
#include "widget/ExpandableListView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * ExpandableListPosition can refer to either a group's position or a child's
 * position. Referring to a child's position requires both a group position (the
 * group containing the child) and a child position (the child's position within
 * that group). To create objects, use {@link #obtainChildPosition(Int32, Int32)} or
 * {@link #obtainGroupPosition(Int32)}.
 */
class ExpandableListPosition
{
private:
    CARAPI_(void) ResetState();


    static CARAPI_(AutoPtr<IExpandableListPosition>) GetRecycledOrCreate();

public:
    ExpandableListPosition();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI_(Int64) GetPackedPosition();

    static CARAPI_(AutoPtr<IExpandableListPosition>) ObtainGroupPosition(
        /* [in] */ Int32 groupPosition);

    static CARAPI_(AutoPtr<IExpandableListPosition>) ObtainChildPosition(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition);

    static CARAPI_(AutoPtr<IExpandableListPosition>) ObtainPosition(
        /* [in] */ Int64 packedPosition);

    static CARAPI_(AutoPtr<IExpandableListPosition>) Obtain(
        /* [in] */ Int32 type,
        /* [in] */ Int32 groupPos,
        /* [in] */ Int32 childPos,
        /* [in] */ Int32 flatListPos);

    virtual CARAPI Recycle();


private:
    static const Int32 MAX_POOL_SIZE = 5;
    static List<AutoPtr<IExpandableListPosition> > sPool;
   // static ArrayOf<IExpandableListPosition*> sPool;/* =
    //    new ArrayList<ExpandableListPosition>(MAX_POOL_SIZE);*/

public:
    /**
     * This data type represents a child position
     */
    static const Int32 CHILD = 1;

    /**
     * This data type represents a group position
     */
    static const Int32 GROUP = 2;

    static Mutex sLock;

    /**
     * The position of either the group being referred to, or the parent
     * group of the child being referred to
     */
    Int32 groupPos;

    /**
     * The position of the child within its parent group
     */
    Int32 childPos;

    /**
     * The position of the item in the flat list (optional, used internally when
     * the corresponding flat list position for the group or child is known)
     */
    Int32 flatListPos;

    /**
     * What type of position this ExpandableListPosition represents
     */
    Int32 type;

    Mutex mLock;

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos


#endif
