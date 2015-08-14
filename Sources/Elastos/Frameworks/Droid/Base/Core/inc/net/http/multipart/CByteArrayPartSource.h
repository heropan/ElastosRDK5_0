
#ifndef __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CBYTEARRAYPARTSOURCE_H__
#define __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CBYTEARRAYPARTSOURCE_H__

#include "_CByteArrayPartSource.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

/**
 * A PartSource that reads from a byte array.  This class should be used when
 * the data to post is already loaded into memory.
 * 
 * @author <a href="mailto:becke@u.washington.edu">Michael Becke</a>
 *   
 * @since 2.0 
 */
CarClass(CByteArrayPartSource)
{
public:
    /**
     * Constructor for ByteArrayPartSource.
     * 
     * @param fileName the name of the file these bytes represent
     * @param bytes the content of this part
     */
    CARAPI constructor(
        /* [in] */ const String& fileName,
        /* [in] */ ArrayOf<Byte>* bytes);

    /**
     * @see PartSource#getLength()
     */
    CARAPI GetLength(
        /* [out] */ Int64* length);

    /**
     * @see PartSource#getFileName()
     */
    CARAPI GetFileName(
        /* [out] */ String* fileName);

    /**
     * @see PartSource#createInputStream()
     */
    CARAPI CreateInputStream(
        /* [out] */ Elastos::IO::IInputStream** stream);

private:
    /** Name of the source file. */
    String mFileName;

    /** Byte array of the source file. */
    AutoPtr<ArrayOf<Byte> > mBytes;
};

}
}
}
}
}
}

#endif // __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CBYTEARRAYPARTSOURCE_H__
