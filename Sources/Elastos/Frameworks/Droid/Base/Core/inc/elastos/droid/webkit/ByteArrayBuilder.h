
#ifndef __ELASTOS_DROID_WEBKIT_BYTEARRAYBUILDER_H__
#define __ELASTOS_DROID_WEBKIT_BYTEARRAYBUILDER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/Core/Object.h>

using Elastos::Core::Object;
using Elastos::Utility::ILinkedList;

namespace Elastos {
namespace Droid {
namespace Webkit {

/** Utility class optimized for accumulating bytes, and then spitting
    them back out.  It does not optimize for returning the result in a
    single array, though this is supported in the API. It is fastest
    if the retrieval can be done via iterating through chunks.
*/
class ByteArrayBuilder : public Object
{
public:
    class Chunk
        : public Object
    {
    public:
        Chunk(
            /* [in] */ Int32 length);

        CAR_INTERFACE_DECL();

        CARAPI GetWeakReference(
            /* [out] */ IWeakReference** weakReference);

        /**
         * Release the chunk and make it available for reuse.
         */
        CARAPI_(void) ReleaseResources();

        CARAPI ToString(
            /* [out] */ String* info);

    public:
        AutoPtr< ArrayOf<Byte> > mArray;
        Int32 mLength;
    };

public:
    ByteArrayBuilder();

    //synchronized
    virtual CARAPI_(void) Append(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    /**
     * The fastest way to retrieve the data is to iterate through the
     * chunks.  This returns the first chunk.  Note: this pulls the
     * chunk out of the queue.  The caller must call Chunk.release() to
     * dispose of it.
     */
    //synchronized
    virtual CARAPI_(AutoPtr<Chunk>) GetFirstChunk();

    //synchronized
    virtual CARAPI_(Boolean) IsEmpty();

    //synchronized
    virtual CARAPI_(Int32) GetByteSize();

    //synchronized
    virtual CARAPI_(void) Clear();

    CARAPI ToString(
        /* [out] */ String* info);

private:
    // Must be called with lock held on sPool.
    CARAPI_(void) ProcessPoolLocked();

    CARAPI_(AutoPtr<Chunk>) ObtainChunk(
        /* [in] */ Int32 length);

private:
    static const Int32 DEFAULT_CAPACITY = 8192;

    // Global pool of chunks to be used by other ByteArrayBuilders.
    static const AutoPtr<ILinkedList> sPool;
    static Object sPoolObject;

    // Reference queue for processing gc'd entries.
//    static const ReferenceQueue<Chunk> sQueue =
//            new ReferenceQueue<Chunk>();

    AutoPtr<ILinkedList> mChunks;
    Object mLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_BYTEARRAYBUILDER_H__
