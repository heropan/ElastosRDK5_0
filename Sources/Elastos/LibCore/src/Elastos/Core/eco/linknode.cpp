
#include <linknode.h>

ELAPI_(DoubleLinkNode*) DoubleLinkNodeInsertPrev(
    DoubleLinkNode *currNode, DoubleLinkNode *prevNode)
{
    assert(prevNode);

    prevNode->mPrev = currNode->mPrev;
    prevNode->mNext = currNode;
    currNode->mPrev->mNext = prevNode;
    currNode->mPrev = prevNode;

    return prevNode;
}

ELAPI_(DoubleLinkNode*) DoubleLinkNodeInsertNext(
    DoubleLinkNode *currNode, DoubleLinkNode *nextNode)
{
    assert(nextNode);

    nextNode->mPrev = currNode;
    nextNode->mNext = currNode->mNext;
    currNode->mNext->mPrev = nextNode;
    currNode->mNext = nextNode;

    return nextNode;
}

ELAPI_(DoubleLinkNode*) DoubleLinkNodeDetach(
    DoubleLinkNode *currNode)
{
    currNode->mPrev->mNext = currNode->mNext;
    currNode->mNext->mPrev = currNode->mPrev;
#ifdef _DEBUG
    currNode->mPrev = currNode->mNext = (DoubleLinkNode *)INVALID_ADDRVALUE;
#endif

    return currNode;
}


ELAPI_(SingleLinkNode*) SingleLinkNodeInsertNext(
    SingleLinkNode *currNode, SingleLinkNode *nextNode)
{
    assert(nextNode);

    nextNode->mNext = currNode->mNext;
    currNode->mNext = nextNode;

    return nextNode;
}

ELAPI_(SingleLinkNode*) SingleLinkNodeDetach(
    SingleLinkNode *currNode, SingleLinkNode *prevNode)
{
    prevNode->mNext = currNode->mNext;
    currNode->mNext = NULL;

    return currNode;
}
