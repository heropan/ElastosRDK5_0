#ifndef __ELASTOS_DROID_UTILITY_CARRAYMAP_H__
#define __ELASTOS_DROID_UTILITY_CARRAYMAP_H__

#include "_Elastos_Droid_Utility_CArrayMap.h"
#include <elastos/core/Object.h>
#include "util/MapCollections.h"

using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CArrayMap)
    , public Object
    , public IArrayMap
    , public IMap
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI_(Int32) IndexOf(
        /* [in] */ IInterface* key,
        /* [in] */ Int32 hash);

    CARAPI_(Int32) IndexOfNull();

private:
    CARAPI AllocArrays(
        /* [in] */ Int32 size);

    static CARAPI FreeArrays(
        /* [in] */ ArrayOf<Int32>* hashes,
        /* [in] */ ArrayOf<IInterface*> array,
        /* [in] */ Int32 size);


public:
    /**
     * @hide Special immutable empty ArrayMap.
     */
    static const AutoPtr<IArrayMap> EMPTY;// = new ArrayMap(true);

private:
    static const Boolean DEBUG;// = false;
    static const String TAG;// = "ArrayMap";

    /**
     * The minimum amount by which the capacity of a ArrayMap will increase.
     * This is tuned to be relatively space-efficient.
     */
    static const Int32 BASE_SIZE;// = 4;

    /**
     * Maximum number of entries to have in array caches.
     */
    static const Int32 CACHE_SIZE;// = 10;

    /**
     * Caches of small array objects to avoid spamming garbage.  The cache
     * Object[] variable is a pointer to a linked list of array objects.
     * The first entry in the array is a pointer to the next array in the
     * list; the second entry is a pointer to the Int32[] hash code array for it.
     */
    static AutoPtr<ArrayOf<IInterface*> > mBaseCache;
    static Int32 mBaseCacheSize;
    static AutoPtr<ArrayOf<IInterface*> > mTwiceBaseCache;
    static Int32 mTwiceBaseCacheSize;

    /**
     * Special hash array value that indicates the container is immutable.
     */
    static const AutoPtr<ArrayOf<Int32> > EMPTY_IMMUTABLE_INTS;// = new Int32[0];

    AutoPtr<ArrayOf<Int32> > mHashes;
    AutoPtr<ArrayOf<IInterface*> > mArray;
    Int32 mSize;
    AutoPtr<MapCollections> mCollections;
};

} // Utility
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_UTILITY_CARRAYMAP_H__
