
#ifndef __INDENTINGPRINTWRITER_H__
#define __INDENTINGPRINTWRITER_H__

#include "_CIndentingPrintWriter.h"
#include "elastos/io/PrintWriter.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::ICharSequence;
using Elastos::IO::IWriter;
using Elastos::IO::PrintWriter;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

CarClass(CIndentingPrintWriter) , public PrintWriter
{
public:
    CIndentingPrintWriter();

    CARAPI constructor(
        /* [in] */ IWriter* writer,
        /* [in] */ const String& indent);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI IncreaseIndent();

    CARAPI DecreaseIndent();

    CARAPI PrintPair(
        /* [in] */ const String& key,
        /* [in] */ IInterface* value);

    CARAPI WriteChars(
        /* [in] */ const ArrayOf<Char32>& buffer,
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

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI WriteChars(
        /* [in] */ const ArrayOf<Char32>& buffer);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI WriteString(
        /* [in] */ const String& str,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Format(
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args);

    CARAPI Format(
        /* [in] */ ILocale* l,
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args);

    CARAPI Printf(
        /* [in] */ const String& format,
        /* [in] */ ArrayOf<IInterface*>* args);

    CARAPI Printf(
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

    CARAPI Close();

    CARAPI Flush();

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
private:
    CARAPI WriteIndent();

private:
    String mIndent;

    StringBuilder mBuilder;
    AutoPtr<ArrayOf<Char32> > mCurrent;
    Boolean mEmptyLine;
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__INDENTINGPRINTWRITER_H__
