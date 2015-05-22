#include "cmdef.h"
#include "elastos/HashMap.h"
#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "CManifest.h"
#include "InitManifest.h"
#include "CUnsafeByteSequence.h"
#include "CStringWrapper.h"
#include "CName.h"
#include "CAttributes.h"
#include <cutils/log.h>

using Elastos::Utility::HashMap;
using Elastos::IO::CUnsafeByteSequence;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Utility {
namespace Jar {

static const String TAG("InitManifest");

InitManifest::InitManifest(
    /* in */ ArrayOf<Byte>* buf,
    /* in */ IAttributes* main,
    /* in */ IName* ver)
    : mPos(0)
    , mConsecutiveLineBreaks(0)
{
    CUnsafeByteSequence::New(128, (IUnsafeByteSequence**)&mValueBuffer);
    mBuf = buf;

    IMap* map = NULL;
    // check a version attribute
    Boolean isEqual, result;
    ECode ec = ReadHeader(&result);
    FAIL_GOTO(ec, EXCEPTION_PROCESS)

    if (!result || (ver != NULL && !(mName->Equals(ver, &isEqual), isEqual))) {
        //String verByString;
        //Slogger::E(TAG, "Missing version attribute: %s", (ver->ToString(&verByString), verByString).string());
        goto EXCEPTION_PROCESS;
    }

    {
        map = IMap::Probe(main);
        map->Put(mName.Get(), mValue.Get(), NULL);
        ec = ReadHeader(&result);
        FAIL_GOTO(ec, EXCEPTION_PROCESS)
        while (result) {
            map->Put(mName, mValue, NULL);
            ec = ReadHeader(&result);
            FAIL_GOTO(ec, EXCEPTION_PROCESS)
        }
    }

EXCEPTION_PROCESS:
    ;
}

ECode InitManifest::InitEntries(
    /* [in, out] */ IMap* entries,
    /* [in, out] */ IMap* chunks)
{
    Int32 mark = mPos;
    String tempValue;
    Boolean isEqual;
    AutoPtr<IInterface> tmp, chunk;
    AutoPtr<IAttributes> entry;
    AutoPtr<ICharSequence> entryNameValue, cs;
    IMap* map;

    Int32 loopCount = 0;
    String nameStr;
    CName::NAME->GetName(&nameStr);

    Boolean result;
    FAIL_RETURN(ReadHeader(&result))
    while (result) {
        if (!nameStr.EqualsIgnoreCase(mNameStr)) {
            ALOGE("Entry is not named");
            return E_IO_EXCEPTION;
        }

        entryNameValue = mValue;
        entry = NULL;
        entries->Get(entryNameValue.Get(), (IInterface**)&entry);
        if (entry == NULL) {
            FAIL_RETURN(CAttributes::New(12, (IAttributes**)&entry))
        }

        map = IMap::Probe(entry);
        FAIL_RETURN(ReadHeader(&result))
        while (result) {
            FAIL_RETURN(map->Put(mName.Get(), mValue.Get(), NULL))
            FAIL_RETURN(ReadHeader(&result))
        }

        if (chunks != NULL) {
            tmp = NULL;
            chunks->Get(entryNameValue.Get(), (IInterface**)&tmp);
            if (tmp != NULL) {
                // TODO A bug: there might be several verification chunks for
                // the same name. I believe they should be used to update
                // signature in order of appearance; there are two ways to fix
                // this: either use a list of chunks, or decide on used
                // signature algorithm in advance and reread the chunks while
                // updating the signature; for now a defensive error is thrown
                ALOGE("A jar verifier does not support more than one entry with the same name");
                return E_IO_EXCEPTION;
            }

            chunk = new CManifest::Chunk(mark, mPos);
            chunks->Put(entryNameValue.Get(), chunk, NULL);
            mark = mPos;
        }

        entries->Put(entryNameValue.Get(), entry.Get(), NULL);
        FAIL_RETURN(ReadHeader(&result))
    }

    return NOERROR;
}

Int32 InitManifest::GetPos()
{
    return mPos;
}

ECode InitManifest::ReadHeader(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    if (mConsecutiveLineBreaks > 1) {
        // break a section on an empty line
        mConsecutiveLineBreaks = 0;
        *result = FALSE;
        return NOERROR;
    }

    FAIL_RETURN(ReadName());
    mConsecutiveLineBreaks = 0;
    FAIL_RETURN(ReadValue());

    // if the last line break is missed, the line
    // is ignored by the reference implementation
    *result = mConsecutiveLineBreaks > 0;
    return NOERROR;
}

ECode InitManifest::ReadName()
{
    Int32 mark = mPos;
    Int32 length = mBuf->GetLength();
    Int32 len = 0;
    Byte* p = mBuf->GetPayload();
    Byte ch;
    while (mPos < length) {
        ch = *(p + mPos++);
        if (ch != ':') {
            if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
                    || ch == '_' || ch == '-' || (ch >= '0' && ch <= '9'))) {
                // validation from CName
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            continue;
        }

        len = mPos - mark -1;
        if (*(p + mPos++) != ' ') {
            String name = String(p + mark, len);
            ALOGE("Invalid value for attribute '%s'", name.string());
            return E_IO_EXCEPTION;
        }

        mNameStr = String(p + mark, len);
        mName = NULL;
        return CName::New(mNameStr, FALSE, (IName**)&mName);
    }
    return NOERROR;
}

ECode InitManifest::ReadValue()
{
    Boolean lastCr = FALSE;
    Int32 mark = mPos;
    Int32 last = mPos;
    Int32 length = mBuf->GetLength();
    mValueBuffer->Rewind();

    Byte* p = mBuf->GetPayload();
    while (mPos < length) {
        switch (*(p + mPos++)) {
        case 0:
            ALOGE("NUL character in a manifest");
            return E_IO_EXCEPTION;
        case '\n':
            if (lastCr) {
                lastCr = FALSE;
            } else {
                mConsecutiveLineBreaks++;
            }
            continue;
        case '\r':
            lastCr = TRUE;
            mConsecutiveLineBreaks++;
            continue;
        case ' ':
            if (mConsecutiveLineBreaks == 1) {
                FAIL_RETURN(mValueBuffer->Write(mBuf, mark, last - mark))
                mark = mPos;
                mConsecutiveLineBreaks = 0;
                continue;
            }
        }

        if (mConsecutiveLineBreaks >= 1) {
            mPos--;
            break;
        }
        last = mPos;
    }

    FAIL_RETURN(mValueBuffer->Write(mBuf, mark, last - mark))
    String str;
    mValueBuffer->ToString(&str);
    mValue = NULL;
    return CStringWrapper::New(str, (ICharSequence**)&mValue);
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos
