
#include "CDataTruncation.h"

using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Sql {

const String CDataTruncation::THE_REASON = String("Data truncation");

const String CDataTruncation::THE_SQLSTATE_READ = String("01004");

const String CDataTruncation::THE_SQLSTATE_WRITE = String("22001");

CAR_OBJECT_IMPL(CDataTruncation);

CAR_INTERFACE_IMPL_3(CDataTruncation, Object, IDataTruncation, ISQLWarning, ISerializable);

ECode CDataTruncation::constructor(
    /* [in] */ Int32 index,
    /* [in] */ Boolean parameter,
    /* [in] */ Boolean read,
    /* [in] */ Int32 dataSize,
    /* [in] */ Int32 transferSize)
{
    assert(0 && "TODO");
    //super(THE_REASON, THE_SQLSTATE_READ, THE_ERROR_CODE);
    this->index = index;
    this->parameter = parameter;
    this->read = read;
    this->dataSize = dataSize;
    this->transferSize = transferSize;
    return NOERROR;
}

// ECode CDataTruncation::constructor(
//     /* [in] */ Int32 index,
//     /* [in] */ Boolean parameter,
//     /* [in] */ Boolean read,
//     /* [in] */ Int32 dataSize,
//     /* [in] */ Int32 transferSize,
//     /* [in] */ IThrowable * cause)
// {
//     //super(THE_REASON, read ? THE_SQLSTATE_READ : THE_SQLSTATE_WRITE, THE_ERROR_CODE, cause);
//     this->index = index;
//     this->parameter = parameter;
//     this->read = read;
//     this->dataSize = dataSize;
//     this->transferSize = transferSize;
//     return NOERROR;
// }

ECode CDataTruncation::GetDataSize(
    /* [out] */ Int32* size)
{
    *size = dataSize;
    return NOERROR;
}

ECode CDataTruncation::GetIndex(
    /* [out] */ Int32* index)
{
    *index = this->index;
    return NOERROR;
}

ECode CDataTruncation::GetParameter(
    /* [out] */ Boolean* isPara)
{
    *isPara = parameter;
    return NOERROR;
}

ECode CDataTruncation::GetRead(
    /* [out] */ Boolean* isRead)
{
    *isRead = read;
    return NOERROR;
}

ECode CDataTruncation::GetTransferSize(
    /* [out] */ Int32* size)
{
    *size = transferSize;
    return NOERROR;
}

ECode CDataTruncation::GetNextWarning(
    /* [out] */ ISQLWarning ** sqlwar)
{
    // ISQLException next = super.getNextException();
    // if (next == null) {
    //     return null;
    // }
    // if (next instanceof SQLWarning) {
    //     return (SQLWarning) next;
    // }
    // throw new Error("SQLWarning chain holds value that is not a SQLWarning");
    return NOERROR;
}

ECode CDataTruncation::SetNextWarning(
    /* [in] */ ISQLWarning * w)
{
    // super.setNextException(w);
    return NOERROR;
}

} // namespace Sql
} // namespace Elastos
