
#ifndef __ELASTOS_IO_CPRINTWRITR_H__
#define __ELASTOS_IO_CPRINTWRITR_H__

#include "_Elastos_IO_CPrintWriter.h"
#include "elastos/io/PrintWriter.h"
#include <elastos.h>

using Libcore::ICU::ILocale;

namespace Elastos {
namespace IO {

CarClass(CPrintWriter) , public PrintWriter
{
public:
    CARAPI constructor(
        /* [in] */ IOutputStream* outs);

    CARAPI constructor(
        /* [in] */ IOutputStream* outs,
        /* [in] */ Boolean autoflush);

    CARAPI constructor(
        /* [in] */ IWriter* wr);

    CARAPI constructor(
        /* [in] */ IWriter* wr,
        /* [in] */ Boolean autoflush);

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

    CARAPI_(PInterface) Probe(
    /* [in]  */ REIID riid);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Close();

    CARAPI Flush();

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

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI Write(
        /* [in] */ const ArrayOf<Char32>& buffer);

    CARAPI Write(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI WriteString(
        /* [in] */ const String& str,
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
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CPRINTWRITR_H__
