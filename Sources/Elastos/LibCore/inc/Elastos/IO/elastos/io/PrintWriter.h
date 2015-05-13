
#ifndef __PRINTWRITR_H__
#define __PRINTWRITR_H__

#include <elastos.h>
#ifdef ELASTOS_CORE
#include <Elastos.Core_server.h>
#else
#include <Elastos.Core.h>
#endif
#include "elastos/io/Writer.h"

using Libcore::ICU::ILocale;

namespace Elastos {
namespace IO {

class PrintWriter : public Writer
{
public:
    PrintWriter();

    ~PrintWriter();

    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Close();

    CARAPI Flush();

    CARAPI Format(
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args);

    CARAPI FormatEx(
        /* [in] */ ILocale* l,
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args);

    CARAPI Printf(
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args);

    CARAPI PrintfEx(
        /* [in] */ ILocale* l,
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args);

    CARAPI PrintChars(
        /* [in] */ const ArrayOf<Char32>& charArray);

    CARAPI PrintChar(
        /* [in] */ Char32 ch);

    CARAPI PrintDouble(
        /* [in] */ Double dnum);

    CARAPI PrintFloat(
        /* [in] */ Float fnum);

    CARAPI PrintInt32(
        /* [in] */ Int32 inum);

    CARAPI PrintInt64(
        /* [in] */ Int64 lnum);

    CARAPI PrintObject(
        /* [in] */ IInterface* obj);

    CARAPI PrintString(
        /* [in] */ const String& str);

    CARAPI PrintBoolean(
        /* [in] */ Boolean result);

    CARAPI Println();

    CARAPI PrintCharsln(
        /* [in] */ const ArrayOf<Char32>& charArray);

    CARAPI PrintCharln(
        /* [in] */ Char32 ch);

    CARAPI PrintDoubleln(
        /* [in] */ Double dnum);

    CARAPI PrintFloatln(
        /* [in] */ Float fnum);

    CARAPI PrintInt32ln(
        /* [in] */ Int32 inum);

    CARAPI PrintInt64ln(
        /* [in] */ Int64 lnum);

    CARAPI PrintObjectln(
        /* [in] */ IInterface* obj);

    CARAPI PrintStringln(
        /* [in] */ const String& str);

    CARAPI PrintBooleanln(
        /* [in] */ Boolean result);

    CARAPI WriteChars(
        /* [in] */ const ArrayOf<Char32>& buffer);

    CARAPI WriteCharsEx(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI WriteStringEx(
        /* [in] */ const String& str,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendCharSequenceEx(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

protected:
    CARAPI Init(
        /* [in] */ IOutputStream* outs);

    CARAPI Init(
        /* [in] */ IOutputStream* outs,
        /* [in] */ Boolean autoflush);

    CARAPI Init(
        /* [in] */ IWriter* wr);

    CARAPI Init(
        /* [in] */ IWriter* wr,
        /* [in] */ Boolean autoflush);

    CARAPI Init(
        /* [in] */ IFile* file);

    CARAPI Init(
        /* [in] */ IFile* file,
        /* [in] */ const String& csn);

    CARAPI Init(
        /* [in] */ const String& fileName);

    CARAPI Init(
        /* [in] */ const String& fileName,
        /* [in] */ const String& csn);

    CARAPI ClearError();

    CARAPI_(void) SetError();

private:
    CARAPI DoWrite(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Char32>& buf);

protected:
    /**
     * The writer to print data to.
     */
    AutoPtr<IWriter> mOut;

private:
    /**
     * Indicates whether this PrintWriter is in an error state.
     */
    Boolean mIoError;

    /**
     * Indicates whether or not this PrintWriter should flush its contents after
     * printing a new line.
     */
    Boolean mAutoflush;

    const String mLineSeparator;;

};

} // namespace IO
} // namespace Elastos

#endif //__PRINTWRITR_H__
