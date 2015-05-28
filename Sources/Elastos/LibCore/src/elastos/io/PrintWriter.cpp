
#include "coredef.h"
#include "elastos/io/PrintWriter.h"
#ifdef ELASTOS_CORELIBRARY
#include "COutputStreamWriter.h"
#include "CFileOutputStream.h"
#include "CBufferedOutputStream.h"
#include "CStringWrapper.h"
#include "CLocaleHelper.h"
#include "CLocale.h"
#include "CFormatter.h"
#include <elastos/CSystem.h>
#endif
#include <elastos/Character.h>
#include <elastos/StringUtils.h>
#include <elastos/Slogger.h>

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

PrintWriter::PrintWriter()
    : mIoError(FALSE)
    , mAutoflush(FALSE)
    , mLineSeparator("\n")
{}

PrintWriter::~PrintWriter()
{}

ECode PrintWriter::Init(
    /* [in] */ IOutputStream* outs)
{
    AutoPtr<IOutputStreamWriter> opsw;
    FAIL_RETURN(COutputStreamWriter::New(outs, (IOutputStreamWriter**)&opsw));
    return Init(opsw, FALSE);
}

ECode PrintWriter::Init(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Boolean autoflush)
{
    AutoPtr<IOutputStreamWriter> opsw;
    FAIL_RETURN(COutputStreamWriter::New(outs, (IOutputStreamWriter**)&opsw));
    return Init(opsw, autoflush);
}

ECode PrintWriter::Init(
    /* [in] */ IWriter* wr)
{
    return Init(wr, FALSE);
}

ECode PrintWriter::Init(
    /* [in] */ IWriter* wr,
    /* [in] */ Boolean autoflush)
{
    AutoPtr<IInterface> obj;
    wr->GetLock((IInterface**)&obj);
    LockObject* lockobj = (LockObject*)obj.Get();
    Writer::Init(lockobj);

    mAutoflush = autoflush;
    mOut = wr;
    return NOERROR;
}

ECode PrintWriter::Init(
    /* [in] */ IFile* file)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(file, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, (IOutputStreamWriter**)&opsr));
    return Init(opsr, FALSE);
}

ECode PrintWriter::Init(
    /* [in] */ IFile* file,
    /* [in] */ const String& csn)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(file, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, csn, (IOutputStreamWriter**)&opsr));
    return Init(opsr, FALSE);
}

ECode PrintWriter::Init(
    /* [in] */ const String& fileName)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(fileName, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, (IOutputStreamWriter**)&opsr));
    return Init(opsr, FALSE);
}

ECode PrintWriter::Init(
    /* [in] */ const String& fileName,
    /* [in] */ const String& csn)
{
    AutoPtr<IFileOutputStream> fops;
    FAIL_RETURN(CFileOutputStream::New(fileName, (IFileOutputStream**)&fops));
    AutoPtr<IBufferedOutputStream> bops;
    FAIL_RETURN(CBufferedOutputStream::New(fops, (IBufferedOutputStream**)&bops));
    AutoPtr<IOutputStreamWriter> opsr;
    FAIL_RETURN(COutputStreamWriter::New(bops, csn, (IOutputStreamWriter**)&opsr));
    return Init(opsr, FALSE);
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

ECode PrintWriter::PrintChars(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    return PrintString(String(charArray, 0));
}

ECode PrintWriter::PrintChar(
    /* [in] */ Char32 ch)
{
    String str;
    str.Append(ch);
    return PrintString(str);
}

ECode PrintWriter::PrintDouble(
    /* [in] */ Double dnum)
{
    return PrintString(StringUtils::DoubleToString(dnum));
}

ECode PrintWriter::PrintFloat(
    /* [in] */ Float fnum)
{
    //PrintString(String.valueOf(fnum));
    return PrintString(StringUtils::DoubleToString(fnum));
}

ECode PrintWriter::PrintInt32(
    /* [in] */ Int32 inum)
{
    return PrintString(StringUtils::Int32ToString(inum));
}

ECode PrintWriter::PrintInt64(
    /* [in] */ Int64 lnum)
{
    return PrintString(StringUtils::Int64ToString(lnum));
}

ECode PrintWriter::PrintObject(
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

    return PrintString(str);
}

ECode PrintWriter::PrintString(
    /* [in] */ const String& str)
{
    return WriteString(!str.IsNull() ? str : String("NULL"));
}

ECode PrintWriter::PrintBoolean(
    /* [in] */ Boolean result)
{
    return PrintString(StringUtils::BooleanToString(result));
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
    FAIL_RETURN(PrintString(separator))
    if (mAutoflush) {
        Flush();
    }

    return NOERROR;
}

ECode PrintWriter::PrintCharsln(
    /* [in] */ const ArrayOf<Char32>& charArray)
{
    return PrintStringln(String(charArray, 0));
}

ECode PrintWriter::PrintCharln(
    /* [in] */ Char32 ch)
{
    String str;
    str.Append(ch);
    return PrintStringln(str);
}

ECode PrintWriter::PrintDoubleln(
    /* [in] */ Double dnum)
{
    return PrintStringln(StringUtils::DoubleToString(dnum));
}

ECode PrintWriter::PrintFloatln(
    /* [in] */ Float fnum)
{
    //PrintStringln(String.valueOf(fnum));
    return PrintStringln(StringUtils::DoubleToString(fnum));
}

ECode PrintWriter::PrintInt32ln(
    /* [in] */ Int32 inum)
{
    return PrintStringln(StringUtils::Int32ToString(inum));
}

ECode PrintWriter::PrintInt64ln(
    /* [in] */ Int64 lnum)
{
    return PrintStringln(StringUtils::Int64ToString(lnum));
}

ECode PrintWriter::PrintObjectln(
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

    return PrintStringln(str);
}

ECode PrintWriter::PrintStringln(
    /* [in] */ const String& str)
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(PrintString(str))
    FAIL_RETURN(Println())

    return NOERROR;
}

ECode PrintWriter::PrintBooleanln(
    /* [in] */ Boolean result)
{
    return PrintStringln(StringUtils::BooleanToString(result));
}

void PrintWriter::SetError()
{
    Object::Autolock lock(mLock);
    mIoError = TRUE;
}

ECode PrintWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer)
{
    return WriteChars(buffer, 0, buffer.GetLength());
}

ECode PrintWriter::WriteChars(
    /* [in] */ const ArrayOf<Char32>& buffer,
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
    /* [in] */ const ArrayOf<Char32>& buf)
{
    Object::Autolock lock(mLock);

    if (mOut != NULL) {
#ifdef DEBUG
        String info(buf);
        Slogger::D("PrintWriter", " >> %s", info.string());
#endif
        if (FAILED(mOut->WriteChars(buf, offset, count))) {
            SetError();
        }
    }
    else {
        SetError();
    }

    return NOERROR;
}

ECode PrintWriter::WriteString(
    /* [in] */ const String& str)
{
    AutoPtr<ArrayOf<Char32> > tmp = str.GetChars();
    return WriteChars(*tmp);
}

ECode PrintWriter::WriteString(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    //write(str.substring(offset, offset + count).toCharArray());
    return WriteString(str.Substring(offset, offset + count));
}

ECode PrintWriter::AppendChar(
    /* [in] */ Char32 c)
{
    return Write((Int32)c);
}

ECode PrintWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq)
{
    Int32 length;

    if (csq == NULL) {
        AutoPtr<ICharSequence> _csq;
        CStringWrapper::New(String("NULL"), (ICharSequence**)&_csq);
        FAIL_RETURN(_csq->GetLength(&length))
        return AppendCharSequence(_csq, 0, length);
    }

    FAIL_RETURN(csq->GetLength(&length))
    return AppendCharSequence(csq, 0, length);
}

ECode PrintWriter::AppendCharSequence(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    String str("NULL");
    if (NULL != csq) {
        csq->ToString(&str);
    }

    return WriteString(str, start, end - start);
}

} // namespace IO
} // namespace Elastos
