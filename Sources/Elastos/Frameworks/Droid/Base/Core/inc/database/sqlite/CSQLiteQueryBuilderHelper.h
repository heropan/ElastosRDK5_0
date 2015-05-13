#ifndef __CSQLITEQUERYBUILDERHELPER_H__
#define __CSQLITEQUERYBUILDERHELPER_H__

#include "_CSQLiteQueryBuilderHelper.h"

using Elastos::Core::IStringBuilder;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

CarClass(CSQLiteQueryBuilderHelper)
{
    CARAPI BuildQueryString(
        /* [in] */ Boolean distinct,
        /* [in] */ const String& tables,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& where,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ String* queryStr);

    CARAPI AppendColumns(
        /* [in] */ IStringBuilder* s,
        /* [in] */ const ArrayOf<String>& columns);
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif //__CSQLITEQUERYBUILDERHELPER_H__