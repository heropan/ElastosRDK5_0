
#ifndef __CMANIFEST_H__
#define __CMANIFEST_H__
#include "cmdef.h"
#include "_CManifest.h"
#include "elastos/Object.h"

using Elastos::IO::IOutputStream;
using Elastos::IO::IInputStream;
using Elastos::Core::Object;
using Elastos::IO::Charset::ICharsets;
using Elastos::IO::Charset::ICharset;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::Charset::ICharsetEncoder;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CManifest)
{
public:
    class Chunk
        : public IInterface
        , public ElLightRefBase {
    public:
        CAR_INTERFACE_DECL()

        Chunk(
            /* in */ Int32 start,
            /* in */ Int32 end)
            : mStart(start)
            , mEnd(end)
        {
        }

        Int32 mStart;
        Int32 mEnd;
    };

    static CARAPI Write(
        /* [in] */ IManifest* manifest,
        /* [in] */ IOutputStream* out);

    CManifest();

    ~CManifest();

    CARAPI Clear();

    CARAPI GetAttributes(
        /* [in] */ const String& name,
        /* [out] */ IAttributes** attrib);

    CARAPI GetEntries(
        /* [out] */ IMap** entries);

    CARAPI GetMainAttributes(
        /* [out] */ IAttributes** attrib);

    CARAPI Write(
        /* [in] */ IOutputStream* os);

    CARAPI Read(
        /* [in] */ IInputStream* is);

    CARAPI Clone(
        /* [out] */ IInterface** object);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IManifest* mani);

    //just for inner usage, not as a car interface.
    CManifest(
        /* [in] */ IInputStream* is,
        /* [in] */ Boolean verified);

    static CARAPI_(AutoPtr<IManifest>) Create(
        /* [in] */ IInputStream* is,
        /* [in] */ Boolean readChunks);

    static CARAPI_(const AutoPtr<ArrayOf<Byte> >) InitStatics();

    CARAPI GetHashCode(
        /* [out] */ Int32* hashcode);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetChunk(
        /* [in] */ const String& name,
        /* [out] */ Chunk** chunk);

    CARAPI RemoveChunks();

    CARAPI GetMainAttributesEnd(
    /* [out] */ Int32* mainAttrib);

private:
    /**
    * Returns a byte[] containing all the bytes from a ByteArrayInputStream.
    * Where possible, this returns the actual array rather than a copy.
    */
    static CARAPI ExposeByteArrayInputStreamBytes(
        /* [in] */ IByteArrayInputStream* bais,
        /* [out,callee] */ ArrayOf<Byte>** ret);

    static CARAPI WriteEntry(
        /* [in] */ IOutputStream* os,
        /* [in] */ IName* name,
        /* [in] */ const String& value,
        /* [in] */ ICharsetEncoder* encoder,
        /* [in] */ IByteBuffer* bBuf);

public:
    static const Int32 LINE_LENGTH_LIMIT = 72;
    //AutoPtr<HashMap<String, AutoPtr<IAttributes> > > mEntries;
    AutoPtr<IMap> mEntries;

private:
    static Object mLock;

    static const AutoPtr<ArrayOf<Byte> > LINE_SEPARATOR;

    static const AutoPtr<ArrayOf<Byte> > VALUE_SEPARATOR;

    static const AutoPtr<IName> NAME_ATTRIBUTE;// = new Attributes.Name("Name");

    //static const AutoPtr<IField> BAIS_BUF;// = getByteArrayInputStreamField("buf");

    //static const AutoPtr<IFieldInfo> BAIS_POS;// = getByteArrayInputStreamField("pos");

    AutoPtr<IAttributes> mMainAttributes;

    //AutoPtr<HashMap<String, AutoPtr<Chunk> > > mChunks;
    AutoPtr<IMap> mChunks;

    /**
     * The end of the main attributes section in the manifest is needed in
     * verification.
     */
    Int32 mMainEnd;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __CMANIFEST_H__
