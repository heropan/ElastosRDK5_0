
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
public class ByteArrayGenerator {
/**
 * Polls random data to generate the array.
 * @param numBytes Length of the array to generate.
 * @return byte[] containing randomly generated data.
 */
AutoPtr< ArrayOf<Byte> > ByteArrayGenerator::GetBytes(
    /* [in] */ Int32 numBytes)
{
    AutoPtr<IFileInputStream> fis;
    // try {
        CFileInputStream::New(String("/dev/urandom"), (IFileInputStream**)&fis);
        AutoPtr< ArrayOf<Byte> > bytes = new ArrayOf<Byte>::Alloc(numBytes);
        Int32 readLen;
        fis->Read(bytes, &readLen);
        if (bytes->GetLength() != readLen) {
            //throw new GeneralSecurityException("Not enough random data available");
            assert(0);
        }
    //    return bytes;
    // } finally {
        if (fis != NULL) {
            fis->Close();
        }
    // }

    return bytes;
}

} // namespace Crypto
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
