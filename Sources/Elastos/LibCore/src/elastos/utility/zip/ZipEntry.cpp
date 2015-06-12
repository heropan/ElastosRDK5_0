
#include "ZipEntry.h"
// #include "CGregorianCalendar.h"
//#include "CDate.h"
#include "CStreams.h"
// #include "CHeapBufferIterator.h"

using Libcore::IO::IStreams;
using Libcore::IO::CStreams;
using Libcore::IO::IHeapBufferIterator;
// using Libcore::IO::CHeapBufferIterator;
using Elastos::IO::ByteOrder_LITTLE_ENDIAN;
using Elastos::Core::EIID_ICloneable;

namespace Elastos {
namespace Utility {
namespace Zip {

//{2BFF7AA8-6021-6956-0EB4-0AACADF7368B}
extern "C" const InterfaceID EIID_ZipEntry =
        { 0x2BFF7AA8, 0x6021, 0x6956, { 0x0E, 0xB4, 0x0A, 0xAC, 0xAD, 0xF7, 0x36, 0x8B } };

CAR_INTERFACE_IMPL_2(ZipEntry, Object, IZipEntry, ICloneable)

ZipEntry::ZipEntry()
    : mCompressedSize(-1)
    , mCrc(-1)
    , mSize(-1)
    , mCompressionMethod(-1)
    , mTime(-1)
    , mModDate(-1)
    , mExtra(NULL)
    , mNameLength(-1)
    , mLocalHeaderRelOffset(-1)
{}

ZipEntry::~ZipEntry()
{
}

ECode ZipEntry::Clone(
    /* [out] */ IInterface** obj)
{
    return NOERROR;
}

ECode ZipEntry::GetHashCode(
    /* [out] */ Int32* hash)
{
    // return name.hashCode();
    return NOERROR;
}

ECode ZipEntry::GetComment(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mComment;
    return NOERROR;
}

ECode ZipEntry::GetCompressedSize(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mCompressedSize;
    return NOERROR;
}

ECode ZipEntry::GetCrc(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mCrc;
    return NOERROR;
}

ECode ZipEntry::GetExtra(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mExtra;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode ZipEntry::GetMethod(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mCompressionMethod;
    return NOERROR;
}

ECode ZipEntry::GetName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mName;
    return NOERROR;
}

ECode ZipEntry::GetSize(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mSize;
    return NOERROR;
}

ECode ZipEntry::GetTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    Int64 time = -1;
    if (mTime != -1) {
        AutoPtr<ICalendar> cal;
        // CGregorianCalendar::New((ICalendar**)&cal);
        cal->Set(ICalendar::MILLISECOND, 0);
        cal->Set(1980 + ((mModDate >> 9) & 0x7f), ((mModDate >> 5) & 0xf) - 1,
                mModDate & 0x1f, (mTime >> 11) & 0x1f, (mTime >> 5) & 0x3f,
                (mTime & 0x1f) << 1);
        AutoPtr<IDate> d;
        cal->GetTime((IDate**)&d);
        d->GetTime(&time);
    }
    *result = time;
    return NOERROR;
}

ECode ZipEntry::IsDirectory(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 length = mName.GetLength();
    assert(length > 0);
    *result = mName.GetChar(length - 1) == '/';
    return NOERROR;
}

ECode ZipEntry::SetComment(
    /* [in] */ const String& comment)
{
    if (comment.IsNull() || comment.GetLength() <= 0xFFFF) {
        mComment = comment;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

ECode ZipEntry::SetCompressedSize(
    /* [in] */ Int64 value)
{
    mCompressedSize = value;
    return NOERROR;
}

ECode ZipEntry::SetCrc(
    /* [in] */ Int64 value)
{
    if (value >= 0 && value <= 0xFFFFFFFFll) {
        mCrc = value;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

ECode ZipEntry::SetExtra(
    /* [in] */ ArrayOf<Byte>* data)
{
    if (data == NULL || data->GetLength() <= 0xFFFF) {
        mExtra = data;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

ECode ZipEntry::SetMethod(
    /* [in] */ Int32 value)
{
    if (value != IZipEntry::STORED && value != IZipEntry::DEFLATED) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressionMethod = value;
    return NOERROR;
}

ECode ZipEntry::SetSize(
    /* [in] */ Int64 value)
{
    if (value >= 0 && value <= 0xFFFFFFFFll) {
        mSize = value;
        return NOERROR;
    }
    else {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

ECode ZipEntry::SetTime(
    /* [in] */ Int64 value)
{
    AutoPtr<ICalendar> cal;
    // CGregorianCalendar::New((ICalendar**)&cal);
    AutoPtr<IDate> date;
    // CDate::New(value, (IDate**)&date);
    cal->SetTime(date);
    Int32 year;
    cal->Get(ICalendar::YEAR, &year);
    if (year < 1980) {
        mModDate = 0x21;
        mTime = 0;
    }
    else {
        mModDate = cal->Get(ICalendar::DATE, &mModDate);
        Int32 month;
        cal->Get(ICalendar::MONTH, &month);
        mModDate = ((month + 1) << 5) | mModDate;
        mModDate = ((year - 1980) << 9) | mModDate;
        Int32 time;
        cal->Get(ICalendar::SECOND, &time);
        mTime = time >> 1;
        cal->Get(ICalendar::MINUTE, &time);
        mTime = (time << 5) | mTime;
        cal->Get(ICalendar::HOUR_OF_DAY, &time);
        mTime = (time << 11) | mTime;
    }

    return NOERROR;
}

ECode ZipEntry::constructor(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (name.GetLength() > 0xFFFF) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mName = name;
    return NOERROR;
}

ECode ZipEntry::constructor(
    /* [in] */ ZipEntry* ze)
{
    mName = ze->mName;
    mComment = ze->mComment;
    mTime = ze->mTime;
    mSize = ze->mSize;
    mCompressedSize = ze->mCompressedSize;
    mCrc = ze->mCrc;
    mCompressionMethod = ze->mCompressionMethod;
    mModDate = ze->mModDate;
    mExtra = ze->mExtra == NULL ? NULL : ze->mExtra->Clone();
    mNameLength = ze->mNameLength;
    mLocalHeaderRelOffset = ze->mLocalHeaderRelOffset;
    return NOERROR;
}

ECode ZipEntry::constructor(
    /* in */ ArrayOf<Byte>* buf,
    /* in */ IInputStream* in)
{
    VALIDATE_NOT_NULL(buf)
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    FAIL_RETURN(streams->ReadFully(in, 0, buf->GetLength(), buf));

//     AutoPtr<IHeapBufferIterator> it;
//     CHeapBufferIterator::New(buf, 0, buf->GetLength(), ByteOrder_LITTLE_ENDIAN,
//             (IHeapBufferIterator**)&it);

//     Int32 sig;
//     it->ReadInt32(&sig);
//     if (sig != IZipConstants::CENSIG) {
//         return E_ZIP_EXCEPTION;
// //         throw new ZipException("Central Directory Entry not found");
//     }

//     Int16 temp16;
//     it->Seek(10);
//     it->ReadInt16(&temp16);
//     mCompressionMethod = temp16;
//     it->ReadInt16(&temp16);
//     mTime = temp16;
//     it->ReadInt16(&temp16);
//     mModDate = temp16;

//     // These are 32-bit values in the file, but 64-bit fields in this object.
//     Int32 temp;
//     it->ReadInt32(&temp);
//     mCrc = ((Int64)temp) & 0xffffffffll;

//     it->ReadInt32(&temp);
//     mCompressedSize = ((Int64)temp) & 0xffffffffll;

//     it->ReadInt32(&temp);
//     mSize = ((Int64)temp) & 0xffffffffll;

//     it->ReadInt16(&temp16);
//     mNameLength = temp16;
//     Int16 extraLength, commentLength;
//     it->ReadInt16(&extraLength);
//     it->ReadInt16(&commentLength);

//     // This is a 32-bit value in the file, but a 64-bit field in this object.
//     it->Seek(42);
//     it->ReadInt32(&temp);
//     mLocalHeaderRelOffset = ((Int64) temp) & 0xffffffffll;

//     AutoPtr< ArrayOf<Byte> > nameBytes = ArrayOf<Byte>::Alloc(mNameLength);
//     FAIL_RETURN(streams->ReadFully(in, 0, mNameLength, nameBytes));
//     mName = String((const char *)nameBytes->GetPayload(), mNameLength);

//     // The RI has always assumed UTF-8. (If GPBF_UTF8_FLAG isn't set, the encoding is
//     // actually IBM-437.)
//     if (commentLength > 0) {
//         AutoPtr< ArrayOf<Byte> > commentBytes = ArrayOf<Byte>::Alloc(commentLength);
//         FAIL_RETURN(streams->ReadFully(in, 0, commentLength, commentBytes));
//         mComment = String((const char *)commentBytes->GetPayload(),
//                 commentBytes->GetLength());
//     }

//     if (extraLength > 0) {
//         mExtra = ArrayOf<Byte>::Alloc(extraLength);
//         streams->ReadFully(in, 0, extraLength, mExtra);
//     }

    return NOERROR;
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
