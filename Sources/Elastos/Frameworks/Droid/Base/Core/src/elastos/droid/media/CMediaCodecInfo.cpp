#include "elastos/droid/media/CMediaCodecInfo.h"
#include "elastos/droid/utility/CRational.h"
#include "elastos/droid/utility/Range.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Utility::CRational;
using Elastos::Droid::Utility::Range;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::Core::Math;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CHashMap;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IHashMap;
using Elastos::Utility::ICollection;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Media {

const Int32 ERROR_UNRECOGNIZED = (1 << 0);
const Int32 ERROR_UNSUPPORTED = (1 << 1);
const Int32 ERROR_NONE_SUPPORTED = (1 << 2);

CAR_INTERFACE_IMPL(CMediaCodecInfo, Object, IMediaCodecInfo)

CAR_OBJECT_IMPL(CMediaCodecInfo)

CMediaCodecInfo::CMediaCodecInfo()
    : mIsEncoder(FALSE)
{
}

CMediaCodecInfo::~CMediaCodecInfo()
{
}

ECode CMediaCodecInfo::constructor(
    /* [in] */ const String& name,
    /* [in] */ Boolean isEncoder,
    /* [in] */ ArrayOf<IMediaCodecInfoCodecCapabilities*>* caps)
{
    mName = name;
    mIsEncoder = isEncoder;
    CHashMap::New((IHashMap**)&mCaps);

    for (Int32 i = 0; i < caps->GetLength(); i++) {
        AutoPtr<IMediaCodecInfoCodecCapabilities> c = (*caps)[i];
        String str;
        c->GetMimeType(&str);
        AutoPtr<ICharSequence> csq;
        CString::New(str, (ICharSequence**)&csq);
        mCaps->Put(csq, c);
    }
    return NOERROR;
}

ECode CMediaCodecInfo::GetName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mName;
    return NOERROR;
}

ECode CMediaCodecInfo::IsEncoder(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mIsEncoder;
    return NOERROR;
}

ECode CMediaCodecInfo::GetSupportedTypes(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ISet> typeSet;
    mCaps->GetKeySet((ISet**)&typeSet);
    AutoPtr<ArrayOf<IInterface*> > types;
    typeSet->ToArray((ArrayOf<IInterface*>**)&types);
    Arrays::Sort(types);
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(types->GetLength());
    for (Int32 i = 0; i < types->GetLength(); i++) {
        String s;
        (*types)[i]->ToString(&s);
        array->Set(i, s);
    }
    *result = array;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CMediaCodecInfo::GetCapabilitiesForType(
    /* [in] */ const String& type,
    /* [out] */ ICodecCapabilities** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    AutoPtr<ICharSequence> csq;
    CString::New(type, (ICharSequence**)&csq);

    AutoPtr<IInterface> obj;
    mCaps->Get(csq, (IInterface**)&obj);
    AutoPtr<IMediaCodecInfoCodecCapabilities> caps =
        IMediaCodecInfoCodecCapabilities::Probe(obj);

    if (caps == NULL) {
        // throw new IllegalArgumentException("codec does not support type");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // clone writable object
    return caps->Dup(result);
}

ECode CMediaCodecInfo::MakeRegular(
    /* [out] */ IMediaCodecInfo** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IArrayList> caps;
    CArrayList::New((IArrayList**)&caps);

    AutoPtr<ICollection> values;
    mCaps->GetValues((ICollection**)&values);
    AutoPtr<ArrayOf<IInterface*> > array;
    values->ToArray((ArrayOf<IInterface*>**)&array);

    for (Int32 i = 0; i < array->GetLength(); i++) {
        AutoPtr<IMediaCodecInfoCodecCapabilities> c =
                IMediaCodecInfoCodecCapabilities::Probe((*array)[i]);
        Boolean b;
        if (c->IsRegular(&b), b) {
            caps->Add(c);
        }
    }

    Int32 size;
    caps->GetSize(&size);
    Int32 capSize;
    mCaps->GetSize(&capSize);

    if (size == 0) {
        *result = NULL;
        return NOERROR;
    }
    else if (size == capSize) {
        *result = THIS_PROBE(IMediaCodecInfo);
        return NOERROR;
    }

    AutoPtr<ArrayOf<IInterface*> > a;
    caps->ToArray((ArrayOf<IInterface*>**)&a);
    AutoPtr<ArrayOf<IMediaCodecInfoCodecCapabilities*> > capArray
            = ArrayOf<IMediaCodecInfoCodecCapabilities*>::Alloc(a->GetLength());
    for (Int32 i = 0; i < a->GetLength(); i++) {
        capArray->Set(i, IMediaCodecInfoCodecCapabilities::Probe((*a)[i]));
    }
    return CMediaCodecInfo::New(mName, mIsEncoder, capArray, result);
}

ECode CMediaCodecInfo::CheckPowerOfTwo(
    /* [in] */ Int32 value,
    /* [in] */ const String& message,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;

    if ((value & (value - 1)) != 0) {
        // throw new IllegalArgumentException(message);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = value;
    return NOERROR;
}

void ECode CMediaCodecInfo::Init()
{
    POSITIVE_INTEGERS = Range::Create(1, Elastos::Core::Math::INT32_MAX_VALUE);
    POSITIVE_LONGS = Range::Create(1l, Elastos::Core::Math::INT64_MAX_VALUE);
    AutoPtr<IRational> start;
    CRational::New(1, Elastos::Core::Math::INT32_MAX_VALUE, (IRational**)&start);
    AutoPtr<IRational> end;
    CRational::New(Elastos::Core::Math::INT32_MAX_VALUE, 1, (IRational**)&start);
    POSITIVE_RATIONALS = Range::Create(start, end);
    SIZE_RANGE = Range::Create(1, 32768);
    FRAME_RATE_RANGE = Range::Create(0, 960);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
