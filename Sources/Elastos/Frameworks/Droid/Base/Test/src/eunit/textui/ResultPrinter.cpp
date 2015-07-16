
#include "eunit/textui/ResultPrinter.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Eunit::Framework::EIID_ITestListener;

namespace Eunit {
namespace Textui {

CAR_INTERFACE_IMPL_2(ResultPrinter, ITestListener, IResultPrinter)

void ResultPrinter::Print(
    /* [in] */ ITestResult* result,
    /* [in] */ Int64 runTime)
{
    Mutex::Autolock lock(&mLock);
    PrintHeader(runTime);
    PrintErrors(result);
    PrintFailures(result);
    PrintFooter(result);
}

void ResultPrinter::PrintWaitPrompt()
{
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    writer->Println();
    writer->PrintStringln(String("<RETURN> to continue"));
}

void ResultPrinter::PrintHeader(
    /* [in] */ Int64 runTime)
{
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    writer->Println();
    writer->PrintStringln(String("Time: ") + ElapsedTimeAsString(runTime));
}

void ResultPrinter::PrintErrors(
    /* [in] */ ITestResult* result)
{
    AutoPtr<IEnumeration> errors;
    result->Errors((IEnumeration**)&errors);
    Int32 number;
    result->ErrorCount(&number);
    PrintDefects(errors, number, String("error"));
}

void ResultPrinter::PrintFailures(
    /* [in] */ ITestResult* result)
{
    AutoPtr<IEnumeration> failures;
    result->Failures((IEnumeration**)&failures);
    Int32 number;
    result->FailureCount(&number);
    PrintDefects(failures, number, String("failure"));
}

void ResultPrinter::PrintDefects(
    /* [in] */ IEnumeration* booBoos,
    /* [in] */ Int32 count,
    /* [in] */ const String& type)
{
    if (count == 0) return;
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    if (count == 1) {
        StringBuilder sb;
        sb.AppendCStr("There was ");
        sb.AppendInt32(count);
        sb.AppendCStr(" ");
        sb.AppendString(type);
        sb.AppendCStr(":");
        writer->PrintStringln(sb.ToString());
    }
    else {
        StringBuilder sb;
        sb.AppendCStr("There were ");
        sb.AppendInt32(count);
        sb.AppendCStr(" ");
        sb.AppendString(type);
        sb.AppendCStr("s:");
        writer->PrintStringln(sb.ToString());
    }
    Boolean hasMore;
    for (Int32 i = 1; (booBoos->HasMoreElements(&hasMore), hasMore); i++) {
        AutoPtr<IInterface> e;
        booBoos->NextElement((IInterface**)&e);
        assert(ITestFailure::Probe(e) != NULL);
        PrintDefect(ITestFailure::Probe(e), i);
    }
}

ECode ResultPrinter::PrintDefect(
    /* [in] */ ITestFailure* booBoo,
    /* [in] */ Int32 count)
{
    PrintDefectHeader(booBoo, count);
    PrintDefectTrace(booBoo);
    return NOERROR;
}

void ResultPrinter::PrintDefectHeader(
    /* [in] */ ITestFailure* booBoo,
    /* [in] */ Int32 count)
{
    // I feel like making this a println, then adding a line giving the throwable a chance to print something
    // before we get to the stack trace.
    AutoPtr<ITest> test;
    booBoo->FailedTest((ITest**)&test);
    String testStr;
    IObject::Probe(test)->ToString(&testStr);
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    writer->PrintString(StringUtils::Int32ToString(count) + ") " + testStr);
}

void ResultPrinter::PrintDefectTrace(
    /* [in] */ ITestFailure* booBoo)
{
    assert(0);
    // getWriter().print(BaseTestRunner.getFilteredTrace(booBoo.trace()));
}

void ResultPrinter::PrintFooter(
    /* [in] */ ITestResult* result)
{
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    Boolean successful;
    if (result->WasSuccessful(&successful), successful) {
        writer->Println();
        writer->PrintString(String("OK"));
        Int32 number;
        result->RunCount(&number);
        StringBuilder sb;
        sb.AppendCStr(" (");
        sb.AppendInt32(number);
        sb.AppendCStr(" test");
        if (number == 1) sb.AppendCStr("");
        else sb.AppendCStr("s");
        sb.AppendCStr(")");
        writer->PrintStringln(sb.ToString());
    }
    else{
        writer->Println();
        writer->PrintStringln(String("FAILURES!!!"));
        StringBuilder sb;
        sb.AppendCStr("Tests run: ");
        Int32 number;
        result->RunCount(&number);
        sb.AppendInt32(number);
        sb.AppendCStr(",  Failures: ");
        result->FailureCount(&number);
        sb.AppendInt32(number);
        sb.AppendCStr(",  Errors: ");
        result->ErrorCount(&number);
        sb.AppendInt32(number);
        writer->PrintStringln(sb.ToString());
    }
    writer->Println();
}

String ResultPrinter::ElapsedTimeAsString(
    /* [in] */ Int64 runTime)
{
    // android-changed
    // The following line was altered for compatibility with
    // Android libraries.
    return StringUtils::DoubleToString((Double)runTime / 1000);
}

ECode ResultPrinter::GetWriter(
    /* [out] */ IPrintStream** writer)
{
    VALIDATE_NOT_NULL(writer);
    *writer = mWriter;
    REFCOUNT_ADD(*writer);
    return NOERROR;
}

ECode ResultPrinter::AddError(
    /* [in] */ ITest* test,
    /* [in] */ ECode ec)
{
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    writer->PrintString(String("E"));
    return NOERROR;
}

ECode ResultPrinter::AddFailure(
    /* [in] */ ITest* test,
    /* [in] */ ITestFailure* failure)
{
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    writer->PrintString(String("F"));
    return NOERROR;
}

ECode ResultPrinter::EndTest(
    /* [in] */ ITest* test)
{
    return NOERROR;
}

ECode ResultPrinter::StartTest(
    /* [in] */ ITest* test)
{
    AutoPtr<IPrintStream> writer;
    GetWriter((IPrintStream**)&writer);
    writer->PrintString(String("."));
    if (mColumn++ >= 40) {
        writer->Println();
        mColumn = 0;
    }
    return NOERROR;
}

} // namespace Textui
} // namespace Eunit
