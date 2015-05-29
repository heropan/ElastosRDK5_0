
#include "CSplitter.h"
#include "CMatcher.h"
#include "CStringWrapper.h"
#include <elastos/core/Math.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::Math;

namespace Elastos {
namespace Utility {
namespace Regex {

CAR_INTERFACE_IMPL(CSplitter, Singleton, ISplitter)

CAR_SINGLETON_IMPL(CSplitter)

const String CSplitter::METACHARACTERS("\\?*+[](){}^$.|");

ECode CSplitter::FastSplit(
    /* [in] */ const String& regularExpression,
    /* [in] */ const String& input,
    /* [in] */ Int32 limit,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    // Can we do it cheaply?
    Int32 len = regularExpression.GetLength();
    if (len == 0) {
        return NOERROR;
    }
    Char32 ch = regularExpression.GetChar(0);
    if (len == 1 && METACHARACTERS.IndexOf(ch) == -1) {
        // We're looking for a single non-metacharacter. Easy.
    }
    else if (len == 2 && ch == '\\') {
        // We're looking for a quoted character.
        // Quoted metacharacters are effectively single non-metacharacters.
        ch = regularExpression.GetChar(1);
        if (METACHARACTERS.IndexOf(ch) == -1) {
            return NOERROR;
        }
    }
    else {
        return NOERROR;
    }

    // We can do this cheaply...

    // Unlike Perl, which considers the result of splitting the empty string to be the empty
    // array, Java returns an array containing the empty string.
    if (input.IsEmpty()) {
        *array = ArrayOf<String>::Alloc(1);
        REFCOUNT_ADD(*array);
        (**array)[0] = String("");
        return NOERROR;
    }

    // Collect text preceding each occurrence of the separator, while there's enough space.
    List<String> list;

    UInt32 maxSize = limit <= 0 ? Elastos::Core::Math::INT32_MAX_VALUE : limit;
    Int32 begin = 0, end = 0;
    while ((end = input.IndexOf(ch, begin)) != -1 && list.GetSize() + 1 < maxSize) {
        String subStr = input.Substring(begin, end);
        list.PushBack(subStr);
        begin = end + 1;
    }
    return FinishSplit(list, input, begin, maxSize, limit, array);
}

ECode CSplitter::Split(
    /* [in] */ IPattern* pattern,
    /* [in] */ const String& regularExpression,
    /* [in] */ const String& input,
    /* [in] */ Int32 limit,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    AutoPtr<ArrayOf<String> > splitResult;
    FAIL_RETURN(FastSplit(regularExpression, input, limit, (ArrayOf<String>**)&splitResult));
    if (splitResult) {
        *array = splitResult;
        REFCOUNT_ADD(*array);
        return NOERROR;
    }

    // Unlike Perl, which considers the result of splitting the empty string to be the empty
    // array, Java returns an array containing the empty string.
    if (input.IsEmpty()) {
        *array = ArrayOf<String>::Alloc(1);
        REFCOUNT_ADD(*array);
        (**array)[0] = String("");
        return NOERROR;
    }

    // Collect text preceding each occurrence of the separator, while there's enough space.
    List<String> list;

    UInt32 maxSize = limit <= 0 ? Elastos::Core::Math::INT32_MAX_VALUE : limit;
    AutoPtr<CMatcher> matcher;
    AutoPtr<ICharSequence> inputSeq;
   FAIL_RETURN(CStringWrapper::New(input, (ICharSequence**)&inputSeq));
    FAIL_RETURN(CMatcher::NewByFriend(pattern, inputSeq, (CMatcher**)&matcher));

    Int32 begin = 0, end, size = 0;
    Boolean result;
    String subStr;
    while ((matcher->Find(&result), result) && size + 1 < maxSize) {
        matcher->Start(&end);
        subStr = input.Substring(begin, end);
        list.PushBack(subStr);
        matcher->End(&begin);
        ++size;
    }

   return FinishSplit(list, input, begin, maxSize, limit, array);
}

ECode CSplitter::FinishSplit(
    /* [in] */ List<String>& list,
    /* [in] */ const String& input,
    /* [in] */ Int32 begin,
    /* [in] */ Int32 maxSize,
    /* [in] */ Int32 limit,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    // Add trailing text.
    if ((UInt32)begin < input.GetLength()) {
        list.PushBack(input.Substring(begin));
    }
    else if (limit != 0) { // No point adding the empty string if limit == 0, just to remove it below.
        list.PushBack(String(""));
    }

    // Remove all trailing empty matches in the limit == 0 case.
    if (limit == 0) {
        List<String>::ReverseIterator rit = list.RBegin();
        while (rit != list.REnd()) {
            if ((*rit).IsNullOrEmpty()) {
                rit = List<String>::ReverseIterator(list.Erase(--(rit.GetBase())));
            }
            else {
                ++rit;
            }
        }
    }

    if (list.IsEmpty() == FALSE) {
        // Convert to an array.
        *array = ArrayOf<String>::Alloc(list.GetSize());
        List<String>::Iterator it = list.Begin();
        for (Int32 i = 0; it != list.End(); ++i, ++it) {
            (**array)[i] = *it;
        }
        list.Clear();

    }
    REFCOUNT_ADD(*array);
    return NOERROR;
}


} // namespace Regex
} // namespace Utility
} // namespace Elastos
