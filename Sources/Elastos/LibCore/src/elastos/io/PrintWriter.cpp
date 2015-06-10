
#include "PrintWriter.h"
#ifdef ELASTOS_CORELIBRARY
#include "COutputStreamWriter.h"
#include "CFileOutputStream.h"
#include "CBufferedOutputStream.h"
#include "CStringWrapper.h"
#include "CLocaleHelper.h"
#include "CLocale.h"
#include "CFormatter.h"
#include <CSystem.h>
#endif
#include <elastos/core/Character.h>
#include <StringUtils.h>
#include <Slogger.h>

using Libcore::ICU::CLocale;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::Character;
using Elastos::Core::StringUtils;
using Elastos::Core::CStringWrapper;
using Elastos::Core::IAppendable;
using Elastos::Core::EIID_IAppendable;
using Elastos::IO::COutputStreamWriter;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::CBufferedOutputStream;
using Elastos::Utility::IFormatter;
using Elastos::Utility::CFormatter;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(PrintWriter, Writer, IPrintWriter)

PrintWriter::PrintWriter()
    : mIoError(FALSE)
    , mAutoflush(FALSE)
    , mLineSeparator("\n")
{}

PrintWriter::~PrintWriter()
{}

ECode PrintWriter::constructor(
    /* [in] */ IOutputStream* outs)
{
    AutoPtr<IOutputStreamWriter> opsw;
    FAIL_RETURN(COutputStreamWriter::New(outs, (IOutputStreamWriter**)&opsw));
    return constructor(IWriter::Probe(opsw), FALSE);
}

ECode PrintWriter::constructor(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Boolean autoflush)
{
    AutoPtr<IOutputStreamWriter> opsw;
    FAIL_RETURN(COutputStreamWriter::New(outs, (IOutputStreamWriter**)&opsw));
    return constructor(IWriter::Probe(opsw), autoflush);
}

ECode PrintWriter::constructor(
    /* [in] */ IWriter* wr)
{
    return constructor(wr, FALSE);
}

ECode PrintWriter::constructor(
    /* [in] */ IWriter* wr,
    /* [in] */ Boolean autoflush)
{
    Writer::constructor(IObject::Probe(wr));

    mAutoflush = autoflush;
    mOut = wr;
    return NOERROR;
}

ECode PrintWriter::constructor(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(file, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(IOutputStream::Probe(fops), (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, (IOutputStreamWriter**)&opsr));
    return constructor(IWriter::Probe(opsr), FALSE);
}

ECode PrintWriter::constructor(
    /* [in] */ IFile* file,
    /* [in] */ const String& csn)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(file, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(IOutputStream::Probe(fops), (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, csn, (IOutputStreamWriter**)&opsr));
    return constructor(IWriter::Probe(opsr), FALSE);
}

ECode PrintWriter::constructor(
    /* [in] */ const String& fileName)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(fileName, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(IOutputStream::Probe(fops), (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, (IOutputStreamWriter**)&opsr));
    return constructor(IWriter::Probe(opsr), FALSE);
}

ECode PrintWriter::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ const String& csn)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(fileName, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(IOutputStream::Probe(fops), (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, csn, (IOutputStreamWriter**)&opsr));
    return constructor(IWriter::Probe(opsr), FALSE);
}

ECode PrintWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    AutoPtr<IWriter> delegate = mOut;
    if (delegate == NULL) {
        *hasError = mIoError;
        return NOERROR;
    }

    Flush();
    Boolean tmp;
    FAIL_RETURN(delegate->CheckError(&tmp))
    *hasError = mIoError || tmp;
    return NOERROR;
}

ECode PrintWriter::ClearError()
{
    Object::Autolock lock(mLock);
    mIoError = FALSE;

    return NOERROR;
}

ECode PrintWriter::Close()
{
    Object::Autolock lock(mLock);
    if (mOut != NULL) {
        if (FAILED(ICloseable::Probe(mOut)->Close())) {
            SetError();
        }
        mOut = NULL;
    }

    return NOERROR;
}

ECode PrintWriter::Flush()
{
    Object::Autolock lock(mLock);
    if (mOut != NULL) {
        if (FAILED(IFlushable::Probe(mOut)->Flush())) {
            SetError();
        }
    }
    else {
        SetError();
    }

    return NOERROR;
}

ECode PrintWriter::Format(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    AutoPtr<ILocaleHelper> helper;
    FAIL_RETURN(CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper))
    AutoPtr<ILocale> locale;
    FAIL_RETURN(helper->GetDefault((ILocale**)&locale))
    return Format(locale, format, args);
}

ECode PrintWriter::Format(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    if (format.IsNull()) {
        //throw new NullPointerException("format == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<IFormatter> formatter;
    FAIL_RETURN(CFormatter::New((IAppendable*)this->Probe(EIID_IAppendable), l, (IFormatter**)&formatter))
    FAIL_RETURN(formatter->Format(format, args))
    if (mAutoflush) {
        Flush();
    }

    return NOERROR;
}

ECode PrintWriter::Printf(
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return Format(format, args);
}

ECode PrintWriter::Printf(
    /* [in] */ ILocale* l,
    /* [in] */ const String& format,
    /* [in] */ ArrayOf<IInterface*>* args)
{
    return Format(l, format, args);
}

ECode PrintWriter::Print(
    /* [in] */ ArrayOf<Char32>* charArray)
{
    return Print(String(charArray, 0));
}

ECode PrintWriter::PrintChar(
    /* [in] */ Char32 ch)
{
    String str;
    str.Append(ch);
    return Print(str);
}

ECode PrintWriter::Print(
    /* [in] */ Double dnum)
{
    return Print(StringUtils::DoubleToString(dnum));
}

ECode PrintWriter::Print(
    /* [in] */ Float fnum)
{
    //PrintString(String.valueOf(fnum));
    return PrintString(StringUtils::DoubleToString(fnum));
}

ECode PrintWriter::Print(
    /* [in] */ Int32 inum)
{
    return PrintString(StringUtils::Int32ToString(inum));
}

ECode PrintWriter::Print(
    /* [in] */ Int64 lnum)
{
    return PrintString(StringUtils::Int64ToString(lnum));
}

ECode PrintWriter::Print(
    /* [in] */ IInterface* obj)
{
    String str;
    IObject* object = IObject::Probe(obj);
    if (object) {
        object->ToString(&str);
    }
    else {
        str.AppendFormat("%p", obj);
    }

    return Print(str);
}

ECode PrintWriter::Print(
    /* [in] */ const String& str)
{
    return Write(!str.IsNull() ? str : String("NULL"));
}

ECode PrintWriter::Print(
    /* [in] */ Boolean result)
{
    return Print(StringUtils::BooleanToString(result));
}

ECode PrintWriter::Println()
{
    Object::Autolock lock(mLock);

    AutoPtr<ISystem> system;
#ifdef ELASTOS_CORELIBRARY
    AutoPtr<Elastos::Core::CSystem> cs;
    Elastos::Core::CSystem::AcquireSingletonByFriend((Elastos::Core::CSystem**)&cs);
    system = (ISystem*)cs.Get();
#else
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
#endif

    String separator;
    system->GetLineSeparator(&separator);
    FAIL_RETURN(Print(separator))
    if (mAutoflush) {
        Flush();
    }

    return NOERROR;
}

ECode PrintWriter::Println(
    /* [in] */ ArrayOf<Char32>* charArray)
{
    return Println(String(*charArray, 0));
}

ECode PrintWriter::PrintCharln(
    /* [in] */ Char32 ch)
{
    String str;
    str.Append(ch);
    return Println(str);
}

ECode PrintWriter::Println(
    /* [in] */ Double dnum)
{
    return Println(StringUtils::DoubleToString(dnum));
}

ECode PrintWriter::Println(
    /* [in] */ Float fnum)
{
    //PrintStringln(String.valueOf(fnum));
    return Println(StringUtils::DoubleToString(fnum));
}

ECode PrintWriter::Println(
    /* [in] */ Int32 inum)
{
    return Println(StringUtils::Int32ToString(inum));
}

ECode PrintWriter::Println(
    /* [in] */ Int64 lnum)
{
    return Println(StringUtils::Int64ToString(lnum));
}

ECode PrintWriter::Println(
    /* [in] */ IInterface* obj)
{
    String str;
    IObject* object = IObject::Probe(obj);
    if (object) {
        object->ToString(&str);
    }
    else {
        str.AppendFormat("%p", obj);
    }

    return Println(str);
}

ECode PrintWriter::Println(
    /* [in] */ const String& str)
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(Print(str))
    FAIL_RETURN(Println())

    return NOERROR;
}

ECode PrintWriter::Println(
    /* [in] */ Boolean result)
{
    return Println(StringUtils::BooleanToString(result));
}

void PrintWriter::SetError()
{
    Object::Autolock lock(mLock);
    mIoError = TRUE;
}

ECode PrintWriter::Write(
    /* [in] */ ArrayOf<Char32>* buffer)
{
    return Write(buffer, 0, buffer->GetLength());
}

ECode PrintWriter::Write(
    /* [in] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return DoWrite(offset, count, buffer);
}

ECode PrintWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    AutoPtr<ArrayOf<Char32> > tmp = ArrayOf<Char32>::Alloc(1);
    (*tmp)[0] = (Char32)oneChar32;
    return DoWrite(0, 1, *tmp);
}

ECode PrintWriter::DoWrite(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Char32>* buf)
{
    Object::Autolock lock(mLock);

    if (mOut != NULL) {
#ifdef DEBUG
        String info(buf);
        Slogger::D("PrintWriter", " >> %s", info.string());
#endif
        if (FAILED(mOut->Write(buf, offset, count))) {
            SetError();
        }
    }
    else {
        SetError();
    }

    return NOERROR;
}

ECode PrintWriter::Write(
    /* [in] */ const String& str)
{
    AutoPtr<ArrayOf<Char32> > tmp = str.GetChars();
    return Write(tmp);
}

ECode PrintWriter::Write(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    //write(str.substring(offset, offset + count).toCharArray());
    return Write(str.Substring(offset, offset + count));
}

ECode PrintWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return Write((Int32)c);
}

ECode PrintWriter::Append(
    /* [in] */ ICharSequence* csq)
{
    Int32 length;

    if (csq == NULL) {
        AutoPtr<ICharSequence> _csq;
        CStringWrapper::New(String("NULL"), (ICharSequence**)&_csq);
        FAIL_RETURN(_csq->GetLength(&length))
        return Append(_csq, 0, length);
    }

    FAIL_RETURN(csq->GetLength(&length))
    return Append(csq, 0, length);
}

ECode PrintWriter::Append(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String str("NULL");
    if (NULL != csq) {
        csq->ToString(&str);
    }

    return Write(str, start, end - start);
}

} // namespace IO
} // namespace Elastos
