
#ifndef __BYTEARRAYGENERATOR_H__
#define __BYTEARRAYGENERATOR_H__

// import java.io.FileInputStream;
// import java.io.IOException;
// import java.security.GeneralSecurityException;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Crypto {

/**
 * Generates byte arrays for use in crypto algorithms. Defaults to pulling random data
 * from /dev/urandom, but can be overwritten for other generation methods.
 */
class ByteArrayGenerator {
    /**
     * Polls random data to generate the array.
     * @param numBytes Length of the array to generate.
     * @return byte[] containing randomly generated data.
     */
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetBytes(
        /* [in] */ Int32 numBytes);
};

} // namespace Crypto
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__BYTEARRAYGENERATOR_H__
