#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#include "CManifest.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "CByteBufferHelper.h"
#include "CStringWrapper.h"
#include "CAttributes.h"
#include "CName.h"
#include "CCharsets.h"
#include "CStreams.h"
#include "InitManifest.h"
#include "CCharBufferHelper.h"
#include "CCoderResultHelper.h"
#include "CHashMap.h"
#include <elastos/HashMap.h>


using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::Charset::CCharsets;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::ICharBuffer;
using Elastos::IO::ICharBufferHelper;
using Elastos::IO::Charset::ICoderResult;
using Elastos::IO::Charset::ICoderResultHelper;
using Elastos::IO::CCharBufferHelper;
using Elastos::IO::Charset::CCoderResultHelper;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::CHashMap;

namespace Elastos {
namespace Utility {
namespace Jar {

Object CManifest::mLock;
const AutoPtr<IName> CManifest::NAME_ATTRIBUTE;
const AutoPtr<ArrayOf<Byte> > CManifest::LINE_SEPARATOR = CManifest::InitStatics();
const AutoPtr<ArrayOf<Byte> > CManifest::VALUE_SEPARATOR;

CAR_INTERFACE_IMPL_LIGHT(CManifest::Chunk, IInterface)

const AutoPtr<ArrayOf<Byte> > CManifest::InitStatics()
{
    Byte sep[] = {'\r', '\n'};
    const AutoPtr<ArrayOf<Byte> > tmp = ArrayOf<Byte>::Alloc(sizeof(sep));
    tmp->Copy(sep, sizeof(sep));
    VALUE_SEPARATOR = ArrayOf<Byte>::Alloc(2);
    (*VALUE_SEPARATOR)[0] = ':';
    (*VALUE_SEPARATOR)[1] = ' ';
    AutoPtr<CName> attrib;
    CName::NewByFriend(String("Name"), (CName**)&attrib);
    NAME_ATTRIBUTE = (IName*)attrib.Get();
    return tmp;
}

CManifest::CManifest()
    //: mEntries(new HashMap<String, AutoPtr<IAttributes> >())
    : mMainEnd(0)
{
    CHashMap::New((IMap**)&mEntries);
    CAttributes::New((IAttributes**)&mMainAttributes);
}

CManifest::~CManifest()
{
}

ECode CManifest::Clear()
{
    mEntries->Clear();
    ((IMap*)IMap::Probe(mMainAttributes.Get()))->Clear();
    return NOERROR;
}

ECode CManifest::GetAttributes(
    /* [in] */ const String& name,
    /* [out, callee] */ IAttributes** attrib)
{
    VALIDATE_NOT_NULL(attrib)
    AutoPtr<IMap> map;
    GetEntries((IMap**)&map);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(name, (ICharSequence**)&cs);
    map->Get(cs, attrib);
    return NOERROR;
}

ECode CManifest::GetEntries(
    /* [out, callee] */ IMap** entries)
{
    VALIDATE_NOT_NULL(entries)
    *entries = mEntries;
    INTERFACE_ADDREF(*entries)
    return NOERROR;
}

ECode CManifest::GetMainAttributes(
    /* [out] */ IAttributes** attrib)
{
    VALIDATE_NOT_NULL(attrib)
    *attrib = mMainAttributes;
    INTERFACE_ADDREF(*attrib)
    return NOERROR;
}

ECode CManifest::Write(
    /* [in] */ IOutputStream* os)
{
    return Write(this, os);
}

ECode CManifest::Read(
    /* [in] */ IInputStream* is)
{
    // TODO: Add your code here
    AutoPtr<ArrayOf<Byte> > buf;
    if (IByteArrayInputStream::Probe(is)) {
        ExposeByteArrayInputStreamBytes((IByteArrayInputStream*)is, (ArrayOf<Byte>**)&buf);
    }
    else {
        AutoPtr<IStreams> stream;
        FAIL_RETURN(CStreams::AcquireSingleton((IStreams**)&stream))
        FAIL_RETURN(stream->ReadFullyNoClose(is, (ArrayOf<Byte>**)&buf))
    }

    if (buf->GetLength() == 0) {
        return NOERROR;
    }

    // a workaround for HARMONY-5662
    // replace EOF and NUL with another new line
    // which does not trigger an error
    Byte b = (*buf)[buf->GetLength() - 1];
    if (b == 0 || b == 26) {
        (*buf)[buf->GetLength() - 1] = '\n';
    }

    // Attributes.Name.MANIFEST_VERSION is not used for
    // the second parameter for RI compatibility
    AutoPtr<InitManifest> im = new InitManifest(buf, mMainAttributes, NULL);
    mMainEnd = im->GetPos();

    return im->InitEntries(mEntries, mChunks);
}

ECode CManifest::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    CManifest::New(this, (IManifest**)&object);
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode CManifest::constructor()
{
    return NOERROR;
}

ECode CManifest::constructor(
    /* [in] */ IInputStream* is)
{
    return Read(is);
}

ECode CManifest::constructor(
    /* [in] */ IManifest* mani)
{
    // TODO: Add your code here
    AutoPtr<ICloneable> clone = ICloneable::Probe(((CManifest*)mani)->mMainAttributes);
    mMainAttributes = NULL;
    FAIL_RETURN(clone->Clone((PInterface*)&mMainAttributes));
    //here we need to do after porting hashmap.java
    //entries = (HashMap<String, Attributes>) ((HashMap<String, Attributes>) man
    //            .getEntries()).clone();
    return E_NOT_IMPLEMENTED;
}

CManifest::CManifest(
    /* [in] */ IInputStream* is,
    /* [in] */ Boolean readChunks)
    //: mEntries(new HashMap<String, AutoPtr<IAttributes> >())
    : mMainEnd(0)
{
    CHashMap::New((IMap**)&mEntries);
    CAttributes::New((IAttributes**)&mMainAttributes);
    if (readChunks) {
        //mChunks = new HashMap<String, AutoPtr<Chunk> >();
        CHashMap::New((IMap**)&mChunks);
    }
    Read(is);
}

AutoPtr<IManifest> CManifest::Create(
    /* [in] */ IInputStream* is,
    /* [in] */ Boolean readChunks)
{
    AutoPtr<CManifest> cobj = new CManifest(is, readChunks);
    return (IManifest*)cobj.Get();
}

ECode CManifest::ExposeByteArrayInputStreamBytes(
    /* [in] */ IByteArrayInputStream* bais,
    /* [out,callee] */ ArrayOf<Byte>** ret)
{
    AutoPtr<ArrayOf<Byte> > buffer;
    {
        Object::Autolock lock(mLock);
        AutoPtr<ArrayOf<Byte> > buf;
        Int32 pos;
        //later I will add two public function to wrapper the protected members
        // to emulate to get the reflection field
        /*
        try {
            buf = (byte[]) BAIS_BUF.get(bais);
            buf = bais->
            pos = BAIS_POS.getInt(bais);
        } catch (IllegalAccessException iae) {
            throw new AssertionError(iae);
        }
        */
        Int32 available;
        bais->Available(&available);
        if (pos == 0 && buf->GetLength() == available) {
            buffer = buf;
        } else {
            buffer = ArrayOf<Byte>::Alloc(available);
            buffer->Copy(buf, pos, available);
        }
        Int64 actual;
        FAIL_RETURN(bais->Skip(available, &actual))
    }
    *ret = buffer;
    ARRAYOF_ADDREF(*ret)
    return NOERROR;
}

/**
 * Returns the hash code for this instance.
 *
 * @return this {@code Manifest}'s hashCode.
*/
ECode CManifest::GetHashCode(
    /* [out] */ Int32* hashcode)
{
    //return mainAttributes.hashCode() ^ getEntries().hashCode();
    return E_NOT_IMPLEMENTED;
}

/**
 * Determines if the receiver is equal to the parameter object. Two {@code
 * Manifest}s are equal if they have identical main attributes as well as
 * identical entry attributes.
 *
 * @param o
 *            the object to compare against.
 * @return {@code true} if the manifests are equal, {@code false} otherwise
*/
ECode CManifest::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual)
    *isEqual = FALSE;
    if (obj == NULL) {
        return NOERROR;
    }

    IManifest* om = IManifest::Probe(obj);
    if (om == NULL) return NOERROR;

    Boolean equal;
    FAIL_RETURN(((CAttributes*)mMainAttributes.Get())->Equals(((CManifest*)om)->mMainAttributes, &equal))
    if (!equal) {
        *isEqual = equal;
        return NOERROR;
    }
    //just compare all items in the objectmap, or later implement hashmap in android?
    //return getEntries().equals(((Manifest) o).getEntries());
    return NOERROR;
}

ECode CManifest::GetChunk(
    /* [in] */ const String& name,
    /* [out] */ Chunk** chunk)
{
    VALIDATE_NOT_NULL(chunk)
    AutoPtr<ICharSequence> cs;
    AutoPtr<IInterface> value;
    CStringWrapper::New(name, (ICharSequence**)&cs);
    mChunks->Get(cs.Get(), (IInterface**)&value);
    *chunk = value.Get();
    INTERFACE_ADDREF(*chunk)
    return NOERROR;
}

ECode CManifest::RemoveChunks()
{
    mChunks = NULL;
    return NOERROR;
}

ECode CManifest::GetMainAttributesEnd(
    /* [out] */ Int32* mainAttrib)
{
    VALIDATE_NOT_NULL(mainAttrib)
    *mainAttrib = mMainEnd;
    return NOERROR;
}

/**
 * Writes out the attribute information of the specified manifest to the
 * specified {@code OutputStream}
 *
 * @param manifest
 *            the manifest to write out.
 * @param out
 *            The {@code OutputStream} to write to.
 * @throws IOException
 *             If an error occurs writing the {@code Manifest}.
 */
ECode CManifest::Write(
        /* [in] */ IManifest* manifest,
        /* [in] */ IOutputStream* out)
{
    AutoPtr<ICharsets> charsets;
    AutoPtr<ICharset> charset;
    CCharsets::AcquireSingleton((ICharsets**)&charsets);
    charsets->GetUTF_8((ICharset**)&charset);
    AutoPtr<ICharsetEncoder> encoder;
    charset->NewEncoder((ICharsetEncoder**)&encoder);

    AutoPtr<IByteBuffer> buffer;
    AutoPtr<IByteBufferHelper> helper;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
    helper->Allocate(LINE_LENGTH_LIMIT, (IByteBuffer**)&buffer);

    String version;
    ((CManifest*)manifest)->mMainAttributes->GetValue(CName::MANIFEST_VERSION, &version);

    if (version != NULL) {
        FAIL_RETURN(WriteEntry(out, CName::MANIFEST_VERSION, version, encoder, buffer))
        AutoPtr<ISet> keyset;
        ((CAttributes*)(((CManifest*)manifest)->mMainAttributes.Get()))->KeySet((ISet**)&keyset);
        AutoPtr<IIterator> entries;
        keyset->GetIterator((IIterator**)&entries);
        Boolean isflag = FALSE;
        String val;
        Boolean equal;
        while (entries->HasNext(&isflag), isflag) {
            AutoPtr<IInterface> outface;
            entries->Next((IInterface**)&outface);
            AutoPtr<IName> name = IName::Probe(outface);
            if((name->Equals(CName::MANIFEST_VERSION, &equal), !equal)) {
                ((CManifest*)manifest)->mMainAttributes->GetValue(name, &val);
                WriteEntry(out, name, val, encoder, buffer);
            }
        }
    }

    FAIL_RETURN(out->WriteBytes(*LINE_SEPARATOR))

    AutoPtr<IMap> entries;
    manifest->GetEntries((IMap**)&entries);
    AutoPtr<ISet> keySet;
    entries->KeySet((ISet**)&keySet);
    AutoPtr<IIterator> it;
    keySet->GetIterator((IIterator**)&it);
    Boolean hasNext;
    String key;
    String val;

    while(it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> elm;
        it->Next((IInterface**)&elm);
        ICharSequence::Probe(elm)->ToString(&key);
        WriteEntry(out, NAME_ATTRIBUTE, key, encoder, buffer);
        AutoPtr<IInterface> attrib;
        entries->Get(elm, (IInterface**)&attrib);
        AutoPtr<ISet> keySet;
        IMap::Probe(attrib)->KeySet((ISet**)&keySet);
        AutoPtr<IIterator> it;
        keySet->GetIterator((IIterator**)&it);
        while(it->HasNext(&hasNext), hasNext) {
            AutoPtr<IInterface> elm, value;
            it->Next((IInterface**)&elm);
            AutoPtr<IName> name = IName::Probe(elm);
            IMap::Probe(attrib)->Get(name.Get(), (IInterface**)&value);
            ICharSequence::Probe(value)->ToString(&val);
            FAIL_RETURN(WriteEntry(out, name, val, encoder, buffer))
        }
        FAIL_RETURN(out->WriteBytes(*LINE_SEPARATOR))
    }
    return NOERROR;
}

ECode CManifest::WriteEntry(
    /* [in] */ IOutputStream* os,
    /* [in] */ IName* name,
    /* [in] */ const String& value,
    /* [in] */ ICharsetEncoder* encoder,
    /* [in] */IByteBuffer* bBuf)
{
    String nameString;
    name->GetName(&nameString);
    const AutoPtr<ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc((Byte*)const_cast<char*>(nameString.string()),
        nameString.GetLength());
    FAIL_RETURN(os->WriteBytes(*bytes))
    FAIL_RETURN(os->WriteBytes(*VALUE_SEPARATOR))
    AutoPtr<ICharsetEncoder> charsetEncoder;
    FAIL_RETURN(encoder->Reset((ICharsetEncoder**)&charsetEncoder))
    bBuf->Clear();
    bBuf->SetLimit(LINE_LENGTH_LIMIT - nameString.GetLength() - 2);

    AutoPtr<ICharBuffer> cBuf;
    AutoPtr<ICharBufferHelper> helper;
    CCharBufferHelper::AcquireSingleton((ICharBufferHelper**)&helper);
    AutoPtr<ArrayOf<Char32> > chars = value.GetChars();
    helper->WrapArray(chars, (ICharBuffer**)&cBuf);

    while (TRUE) {
        AutoPtr<ICoderResult> r, underflow;
        AutoPtr<ICoderResultHelper> helper;
        encoder->EncodeEx(cBuf, bBuf, TRUE, (ICoderResult**)&r);
        CCoderResultHelper::AcquireSingleton((ICoderResultHelper**)&helper);
        helper->GetUNDERFLOW((ICoderResult**)&underflow);
        if (underflow == r) {
            r = NULL;
            encoder->Flush(bBuf, (ICoderResult**)&r);
        }
        AutoPtr<ArrayOf<Byte> > bBufArray;
        Int32 arrayOffset, position;
        bBuf->GetArray((ArrayOf<Byte>**)&bBufArray);
        bBuf->GetArrayOffset(&arrayOffset);
        bBuf->GetPosition(&position);
        os->WriteBytesEx(*bBufArray, arrayOffset, position);
        os->WriteBytes(*LINE_SEPARATOR);
        if (underflow == r) {
            break;
        }
        os->Write(' ');
        bBuf->Clear();
        bBuf->SetLimit(LINE_LENGTH_LIMIT - 1);
    }
    return NOERROR;
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos

