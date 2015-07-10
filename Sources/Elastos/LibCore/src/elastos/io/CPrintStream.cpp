
#include "CPrintStream.h"
#include "CFile.h"
#include "CCharsetHelper.h"
#include "CFileOutputStream.h"
#include "CLocale.h"
//#include "CFormatter.h"
#include "Character.h"
#include "StringUtils.h"
#include "AutoLock.h"

using Elastos::Core::Character;
using Elastos::Core::StringUtils;
using Elastos::Core::EIID_IAppendable;
using Elastos::Utility::IFormatter;
//using Elastos::Utility::CFormatter;
using Elastos::Utility::CLocale;
using Elastos::IO::Charset::ICharsetHelper;
using Elastos::IO::Charset::CCharsetHelper;

namespace Elastos {
namespace IO {

CAR_OBJECT_IMPL(CPrintStream)

CAR_INTERFACE_IMPL_2(CPrintStream, FilterOutputStream, IPrintStream, IAppendable)

const String CPrintStream::sLineSeparator("\n");

CPrintStream::CPrintStream()
    : mIoError(FALSE)
    , mAutoFlush(FALSE)
{}

CPrintStream::~CPrintStream()
{}

ECode CPrintStream::constructor(
    /* [in] */ IOutputStream* outs)
{
    FAIL_RETURN(FilterOutputStream::constructor(outs));
    if (outs == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode CPrintStream::constructor(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Boolean autoflush)
{
    FAIL_RETURN(FilterOutputStream::constructor(outs));
    if (outs == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mAutoFlush = autoflush;
    return NOERROR;
}

ECode CPrintStream::constructor(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Boolean autoflush,
    /* [in] */ const String& enc)
{
    FAIL_RETURN(FilterOutputStream::constructor(outs));
    if (outs == NULL || enc.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mAutoFlush = autoflush;

    AutoPtr<ICharsetHelper> helper;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);

    Boolean tmp;
    ECode ec = helper->IsSupported(enc, &tmp);
    if(!tmp || NOERROR != ec) {
        return E_UNSUPPORTED_ENCODING_EXCEPTION;
    }
    mEncoding = enc;

    return NOERROR;
}

ECode CPrintStream::constructor(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileOutputStream> fos;
    FAIL_RETURN(CFileOutputStream::New(file, (IFileOutputStream**)&fos));
    return FilterOutputStream::constructor((IOutputStream*)fos.Get());
}

ECode CPrintStream::constructor(
    /* [in] */ IFile* file,
    /* [in] */ const String& csn)
{
    AutoPtr<IFileOutputStream> fos;
    FAIL_RETURN(CFileOutputStream::New(file, (IFileOutputStream**)&fos));
    FAIL_RETURN(FilterOutputStream::constructor((IOutputStream*)fos.Get()));

    if (csn.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<ICharsetHelper> helper;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);

    Boolean tmp;
    FAIL_RETURN(helper->IsSupported(csn, &tmp));
    if (!tmp) {
        return E_UNSUPPORTED_ENCODING_EXCEPTION;
    }
    mEncoding = csn;

    return NOERROR;
}

ECode CPrintStream::constructor(
    /* [in] */ const String& fileName)
{
    AutoPtr<IFile> file;
    FAIL_RETURN(CFile::New(fileName, (IFile**)&file));
    return constructor(file);
}

ECode CPrintStream::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ const String& csn)
{
    AutoPtr<IFile> file;
    FAIL_RETURN(CFile::New(fileName, (IFile**)&file));
    return constructor(file, csn);
}

ECode CPrintStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    AutoPtr<IOutputStream> delegate = mOut;
    if (delegate == NULL) {
        *hasError = mIoError;
        return NOERROR;
    }

    Flush();
    Boolean tmp;
    delegate->CheckError(&tmp);
    *hasError = mIoError || tmp;

    return NOERROR;
}

ECode CPrintStream::ClearError()
{
    mIoError = FALSE;
    return NOERROR;
}

ECode CPrintStream::Close()
{
    Flush();
    if (mOut != NULL) {
        if(NOERROR != ICloseable::Probe(mOut)->Close()) {
            SetError();
        }
        else {
            mOut = NULL;
        }
    }
    return NOERROR;
}

ECode CPrintStream::Flush()
{
    if (mOut != NULL) {
        IFlushable::Probe(mOut)->Flush();
    }

    return NOERROR;
}

ECode CPrintStream::Format(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args,
    /* [out] */ IPrintStream** pw)
{
    AutoPtr<ILocale> ouloc = CLocale::GetDefault();
    return Format(ouloc, format, args, pw);
}

ECode CPrintStream::Format(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args,
    /* [out] */ IPrintStream** pw)
{
    VALIDATE_NOT_NULL(pw)
    *pw = NULL;

    if (format.IsNull()) {
        return E_EOF_EXCEPTION;
    }
    AutoPtr<IFormatter> res;
    // FAIL_RETURN(CFormatter::New(THIS_PROBE(IAppendable), l, (IFormatter**)&res));
    res->Format(format, args);
    *pw = THIS_PROBE(IPrintStream);
    REFCOUNT_ADD(*pw)
    return NOERROR;
}

ECode CPrintStream::Printf(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args,
    /* [out] */ IPrintStream** pw)
{
    return Format(format, args, pw);
}

ECode CPrintStream::Printf(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args,
    /* [out] */ IPrintStream** ps)
{
    return Format(l, format, args, ps);
}

ECode CPrintStream::Newline()
{
    return Print(sLineSeparator);
}

ECode CPrintStream::Print(
    /* [in] */ ArrayOf<Char32>* charArray)
{
    AutoPtr<ArrayOf<Byte> > dst;
    Int32 dstOffset = 0;
    FAIL_RETURN(Character::ToChars(
        *charArray, 0, charArray->GetLength(), (ArrayOf<Byte>**)&dst, &dstOffset));

    return Print(String((const char*)dst->GetPayload()));
}

ECode CPrintStream::PrintChar(
    /* [in] */ Char32 ch)
{
    AutoPtr<ArrayOf<Byte> > charArray;
    FAIL_RETURN(Character::ToChars(ch, (ArrayOf<Byte>**)&charArray));
    return Print(String((const char*)charArray->GetPayload()));
}

ECode CPrintStream::Print(
    /* [in] */ Double dnum)
{
    return Print(StringUtils::ToString(dnum));
}

ECode CPrintStream::Print(
    /* [in] */ Float fnum)
{
    //PrintString(String.valueOf(fnum));
    return Print(StringUtils::ToString(fnum));
}

ECode CPrintStream::Print(
    /* [in] */ Int32 inum)
{
    return Print(StringUtils::ToString(inum));
}

ECode CPrintStream::Print(
    /* [in] */ Int64 lnum)
{
    return Print(StringUtils::ToString(lnum));
}

ECode CPrintStream::Print(
    /* [in] */ IInterface* obj)
{
    return Print(Object::ToString(obj));
}

ECode CPrintStream::Print(
    /* [in] */ const String& str)
{
    if (mOut == NULL) {
        SetError();
        return NOERROR;
    }
    if (str.IsNull()) {
        Print(String("NULL"));
        return NOERROR;
    }


    AutoPtr< ArrayOf<Byte> > tmp = str.GetBytes();
    if (mEncoding == NULL && NOERROR != Write(tmp, 0, tmp->GetLength())) {
        SetError();
    }
/*    if(mEncoding != NULL && NOERROR != WriteBuffer(str.getBytes(mEncoding)))
    {
        SetError();
    }*/

    return NOERROR;
}

ECode CPrintStream::Print(
    /* [in] */ Boolean result)
{
    return Print(StringUtils::BooleanToString(result));
}

ECode CPrintStream::Println()
{
    return Newline();
}

ECode CPrintStream::Println(
    /* [in] */ ArrayOf<Char32>* charArray)
{
    AutoPtr<ArrayOf<Byte> > dst;
    Int32 dstOffset = 0;
    FAIL_RETURN(Character::ToChars(
        *charArray, 0, charArray->GetLength(), (ArrayOf<Byte>**)&dst, &dstOffset));

    return Println(String((const char*)dst->GetPayload()));
}

ECode CPrintStream::PrintCharln(
    /* [in] */ Char32 ch)
{
    return Println(StringUtils::ToString((Int32)ch));
}

ECode CPrintStream::Println(
    /* [in] */ Double dnum)
{
    return Println(StringUtils::ToString(dnum));
}

ECode CPrintStream::Println(
    /* [in] */ Float fnum)
{
    //PrintStringln(String.valueOf(fnum));
    return Println(StringUtils::ToString(fnum));
}

ECode CPrintStream::Println(
    /* [in] */ Int32 inum)
{
    return Println(StringUtils::ToString(inum));
}

ECode CPrintStream::Println(
    /* [in] */ Int64 lnum)
{
    return Println(StringUtils::ToString(lnum));
}

ECode CPrintStream::Println(
    /* [in] */ IInterface* obj)
{
    assert(0);
    //PrintStringln(String.valueOf(obj));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintStream::Println(
    /* [in] */ const String& str)
{
    Print(str);
    return Newline();
}

ECode CPrintStream::Println(
    /* [in] */ Boolean result)
{
    return Println(StringUtils::BooleanToString(result));
}

ECode CPrintStream::SetError()
{
    mIoError = TRUE;
    return NOERROR;
}

ECode CPrintStream::Write(
    /* [in] */ Int32 oneChar32)
{
    AutoLock lock(this);

    if (mOut == NULL) {
        SetError();
        return NOERROR;
    }

    if(NOERROR != mOut->Write(oneChar32)) {
        SetError();
    }
    else {
        Int32 b = oneChar32 & 0xFF;
        // 0x0A is ASCII newline, 0x15 is EBCDIC newline.
        Boolean isNewline = (b == 0x0A || b == 0x15);
        if (mAutoFlush && isNewline) {
            Flush();
        }
    }

    return NOERROR;
}

ECode CPrintStream::Write(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    // Force buffer null check first!
    if (offset > buffer->GetLength() || offset < 0) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (count < 0 || count > buffer->GetLength() - offset) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoLock lock(this);

    if (mOut == NULL) {
        SetError();
        return NOERROR;
    }

    if(NOERROR != mOut->Write(buffer, offset, count)) {
        SetError();
    }
    else if (mAutoFlush) {
        Flush();
    }

    return NOERROR;
}

ECode CPrintStream::AppendChar(
    /* [in] */ Char32 c)
{
    return PrintChar(c);
}

ECode CPrintStream::Append(
    /* [in] */ ICharSequence* csq)
{
    String str("NULL");
    if (NULL != csq) {
        csq->ToString(&str);
    }
    Print(str);

    return NOERROR;
}

ECode CPrintStream::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String str("NULL");
    if (NULL != csq) {
        csq->ToString(&str);
    }
    return Print(str.Substring(start, end));
}

} // namespace IO
} // namespace Elastos
