
#ifndef __ELASTOS_IO_CPRINTSTREAM_H__
#define __ELASTOS_IO_CPRINTSTREAM_H__

#include "_Elastos_IO_CPrintStream.h"
#include "FilterOutputStream.h"

using Elastos::Core::ICharSequence;
using Libcore::ICU::ILocale;

namespace Elastos {
namespace IO {

CarClass(CPrintStream)
    , public FilterOutputStream
{
public:
    CPrintStream();

    CARAPI constructor(
        /* [in] */ IOutputStream* outs);

    CARAPI constructor(
        /* [in] */ IOutputStream* outs,
        /* [in] */ Boolean autoflush);

    CARAPI constructor(
        /* [in] */ IOutputStream* outs,
        /* [in] */ Boolean autoflush,
        /* [in] */ const String& enc);

    CARAPI constructor(
        /* [in] */ IFile* file);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ const String& csn);

    CARAPI constructor(
        /* [in] */ const String& fileName);

    CARAPI constructor(
        /* [in] */ const String& fileName,
        /* [in] */ const String& csn);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Close();

    CARAPI Flush();

    CARAPI Format(
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args,
        /* [out] */ IPrintStream** pw);

    CARAPI Format(
        /* [in] */ ILocale* l,
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args,
        /* [out] */ IPrintStream** pw);

    CARAPI Printf(
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args,
        /* [out] */ IPrintStream** pw);

    CARAPI Printf(
        /* [in] */ ILocale* l,
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args,
        /* [out] */ IPrintStream** ps);

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

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

protected:
    CARAPI ClearError();

    CARAPI SetError();

private:
    CARAPI Newline();

private:
    static const String sLineSeparator;

    /**
     * indicates whether or not this PrintStream has incurred an error.
     */
    Boolean mIoError;

    /**
     * indicates whether or not this PrintStream should flush its contents after
     * printing a new line.
     */
    Boolean mAutoFlush;

    String mEncoding;
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CPRINTSTREAM_H__
