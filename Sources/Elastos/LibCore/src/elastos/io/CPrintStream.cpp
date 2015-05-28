
#include "coredef.h"
#include "CPrintStream.h"
#include "CFile.h"
#include "CCharsetHelper.h"
#include "CFileOutputStream.h"
#include "CLocale.h"
#include "CFormatter.h"
#include <elastos/Character.h>
#include <elastos/StringUtils.h>

using Elastos::Core::Character;
using Elastos::Core::StringUtils;
using Elastos::Core::EIID_IAppendable;
using Elastos::Utility::IFormatter;
using Elastos::Utility::CFormatter;
using Elastos::IO::Charset::ICharsetHelper;
using Elastos::IO::Charset::CCharsetHelper;
using Libcore::ICU::CLocale;

namespace Elastos {
namespace IO {

const String CPrintStream::sLineSeparator = String("\n");

CPrintStream::CPrintStream()
    : mIoError(FALSE)
    , mAutoFlush(FALSE)
{}

ECode CPrintStream::constructor(
    /* [in] */ IOutputStream* outs)
{
    FAIL_RETURN(FilterOutputStream::Init(outs));
    if (outs == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode CPrintStream::constructor(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Boolean autoflush)
{
    FAIL_RETURN(FilterOutputStream::Init(outs));
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
    FAIL_RETURN(FilterOutputStream::Init(outs));
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
    return FilterOutputStream::Init((IOutputStream*)fos.Get());
}

ECode CPrintStream::constructor(
    /* [in] */ IFile* file,
    /* [in] */ const String& csn)
{
    AutoPtr<IFileOutputStream> fos;
    FAIL_RETURN(CFileOutputStream::New(file, (IFileOutputStream**)&fos));
    FAIL_RETURN(FilterOutputStream::Init((IOutputStream*)fos.Get()));

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
        if(NOERROR != mOut->Close()) {
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
    FAIL_RETURN(CFormatter::New(THIS_PROBE(IAppendable), l, (IFormatter**)&res));
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
    return PrintString(sLineSeparator);
}

ECode CPrintStream::PrintChars(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    AutoPtr<ArrayOf<Char8> > dst;
    Int32 dstOffset = 0;
    FAIL_RETURN(Character::ToChars(
        charArray, 0, charArray.GetLength(), (ArrayOf<Char8>**)&dst, &dstOffset));

    return PrintString(String(dst->GetPayload()));
}

ECode CPrintStream::PrintChar(
    /* [in] */ Char32 ch)
{
    AutoPtr<ArrayOf<Char8> > charArray;
    FAIL_RETURN(Character::ToChars(ch, (ArrayOf<Char8>**)&charArray));
    return PrintString(String(charArray->GetPayload()));
}

ECode CPrintStream::PrintDouble(
    /* [in] */ Double dnum)
{
    return PrintString(StringUtils::DoubleToString(dnum));
}

ECode CPrintStream::PrintFloat(
    /* [in] */ Float fnum)
{
    //PrintString(String.valueOf(fnum));
    return PrintString(StringUtils::DoubleToString(fnum));
}

ECode CPrintStream::PrintInt32(
    /* [in] */ Int32 inum)
{
    return PrintString(StringUtils::Int32ToString(inum));
}

ECode CPrintStream::PrintInt64(
    /* [in] */ Int64 lnum)
{
    return PrintString(StringUtils::Int64ToString(lnum));
}

ECode CPrintStream::PrintObject(
    /* [in] */ IInterface* obj)
{
    assert(0);
    //PrintString(String.valueOf(obj));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintStream::PrintString(
    /* [in] */ const String& str)
{
    if (mOut == NULL) {
        SetError();
        return NOERROR;
    }
    if (str.IsNull()) {
        PrintString(String("NULL"));
        return NOERROR;
    }

    Int32 size = str.GetByteLength();
    AutoPtr< ArrayOf<Byte> > tmp = ArrayOf<Byte>::Alloc(size);
    tmp->Copy((Byte*)str.string(), size);

    if (mEncoding == NULL && NOERROR != WriteBytes(*tmp)) {
        SetError();
    }
/*    if(mEncoding != NULL && NOERROR != WriteBuffer(str.getBytes(mEncoding)))
    {
        SetError();
    }*/

    return NOERROR;
}

ECode CPrintStream::PrintBoolean(
    /* [in] */ Boolean result)
{
    return PrintString(StringUtils::BooleanToString(result));
}

ECode CPrintStream::Println()
{
    return Newline();
}

ECode CPrintStream::PrintCharsln(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    AutoPtr<ArrayOf<Char8> > dst;
    Int32 dstOffset = 0;
    FAIL_RETURN(Character::ToChars(
        charArray, 0, charArray.GetLength(), (ArrayOf<Char8>**)&dst, &dstOffset));

    return PrintStringln(String(dst->GetPayload()));
}

ECode CPrintStream::PrintCharln(
    /* [in] */ Char32 ch)
{
    return PrintStringln(StringUtils::Int32ToString((Int32)ch));
}

ECode CPrintStream::PrintDoubleln(
    /* [in] */ Double dnum)
{
    return PrintStringln(StringUtils::DoubleToString(dnum));
}

ECode CPrintStream::PrintFloatln(
    /* [in] */ Float fnum)
{
    //PrintStringln(String.valueOf(fnum));
    return PrintStringln(StringUtils::DoubleToString(fnum));
}

ECode CPrintStream::PrintInt32ln(
    /* [in] */ Int32 inum)
{
    return PrintStringln(StringUtils::Int32ToString(inum));
}

ECode CPrintStream::PrintInt64ln(
    /* [in] */ Int64 lnum)
{
    return PrintStringln(StringUtils::Int64ToString(lnum));
}

ECode CPrintStream::PrintObjectln(
    /* [in] */ IInterface* obj)
{
    assert(0);
    //PrintStringln(String.valueOf(obj));
    return E_NOT_IMPLEMENTED;
}

ECode CPrintStream::PrintStringln(
    /* [in] */ const String& str)
{
    PrintString(str);
    return Newline();
}

ECode CPrintStream::PrintBooleanln(
    /* [in] */ Boolean result)
{
    return PrintStringln(StringUtils::BooleanToString(result));
}

ECode CPrintStream::SetError()
{
    mIoError = TRUE;
    return NOERROR;
}

ECode CPrintStream::Write(
    /* [in] */ Int32 oneChar32)
{
    Object::Autolock lock(mLock);

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

ECode CPrintStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return FilterOutputStream::WriteBytes(buffer);
}

ECode CPrintStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    // Force buffer null check first!
    if (offset > buffer.GetLength() || offset < 0) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (count < 0 || count > buffer.GetLength() - offset) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Object::Autolock lock(mLock);

    if (mOut == NULL) {
        SetError();
        return NOERROR;
    }

    if(NOERROR != mOut->WriteBytes(buffer, offset, count)) {
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

ECode CPrintStream::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    String str("NULL");
    if (NULL != csq) {
        csq->ToString(&str);
    }
    PrintString(str);

    return NOERROR;
}

ECode CPrintStream::AppendCharSequence(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String str("NULL");
    if (NULL != csq) {
        csq->ToString(&str);
    }
    return PrintString(str.Substring(start, end));
}

ECode CPrintStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FilterOutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

PInterface CPrintStream::Probe(
    /* [in] */ REIID riid)
{
    return _CPrintStream::Probe(riid);
}

} // namespace IO
} // namespace Elastos
