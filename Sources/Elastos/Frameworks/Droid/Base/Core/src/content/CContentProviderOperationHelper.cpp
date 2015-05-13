
#include "content/CContentProviderOperationHelper.h"
#include "content/CContentProviderOperationBuilder.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CContentProviderOperationHelper::NewInsert(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder)
    return CContentProviderOperationBuilder::New(IContentProviderOperation::TYPE_INSERT, uri, operationBuilder);
}

ECode CContentProviderOperationHelper::NewUpdate(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder)
    return CContentProviderOperationBuilder::New(IContentProviderOperation::TYPE_UPDATE, uri, operationBuilder);
}

ECode CContentProviderOperationHelper::NewDelete(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder)
    return CContentProviderOperationBuilder::New(IContentProviderOperation::TYPE_DELETE, uri, operationBuilder);
}

ECode CContentProviderOperationHelper::NewAssertQuery(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder)
    return CContentProviderOperationBuilder::New(IContentProviderOperation::TYPE_ASSERT, uri, operationBuilder);
}

}
}
}

