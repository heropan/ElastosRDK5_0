#include "util/PathParser.h"

namespace Elastos {
namespace Droid {
namespace Utility {

const String PathParser::LOGTAG("PathParser");

//===========================================================================
// PathDataNode
//===========================================================================

PathDataNode::PathDataNode(
    /* [in] */ Char32 type,
    /* [in] */ ArrayOf<Float>* params)
{
}

PathDataNode::PathDataNode(
    /* [in] */ PathDataNode* n)
{
}

void PathDataNode::NodesToPath(
    /* [in] */ ArrayOf<PathDataNode*>* node,
    /* [in] */ IPath* path)
{
}

void PathDataNode::InterpolatePathDataNode(
    /* [in] */ PathDataNode* nodeFrom,
    /* [in] */ PathDataNode* nodeTo,
    /* [in] */ Float fraction)
{
}

void PathDataNode::AddCommand(
    /* [in] */ IPath* path,
    /* [in] */ ArrayOf<Float>* current,
    /* [in] */ Char32 previousCmd,
    /* [in] */ Char32 cmd,
    /* [in] */ ArrayOf<Float>* val)
{
}

void PathDataNode::DrawArc(
    /* [in] */ IPath* p,
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float a,
    /* [in] */ Float b,
    /* [in] */ Float theta,
    /* [in] */ Boolean isMoreThanHalf,
    /* [in] */ Boolean isPositiveArc)
{
}

void PathDataNode::ArcToBezier(
    /* [in] */ IPath* p,
    /* [in] */ Double cx,
    /* [in] */ Double cy,
    /* [in] */ Double a,
    /* [in] */ Double b,
    /* [in] */ Double e1x,
    /* [in] */ Double e1y,
    /* [in] */ Double theta,
    /* [in] */ Double start,
    /* [in] */ Double sweep)
{
}


//===========================================================================
// PathParser
//===========================================================================

AutoPtr<IPath> PathParser::CreatePathFromPathData(
    /* [in] */ const String& pathData)
{
    return NULL;
}

AutoPtr<ArrayOf<PathDataNode*> > PathParser::CreateNodesFromPathData(
    /* [in] */ const String& pathData)
{
    return NULL;
}

AutoPtr<ArrayOf<PathDataNode*> > DeepCopyNodes(
    /* [in] */ ArrayOf<PathDataNode*>* source)
{
    return NULL;
}

Boolean PathParser::CanMorph(
    /* [in] */ ArrayOf<PathDataNode*>* nodesFrom,
    /* [in] */ ArrayOf<PathDataNode*>* nodesTo)
{
    return FALSE;
}

void PathParser::UpdateNodes(
    /* [in] */ ArrayOf<PathDataNode*>* target,
    /* [in] */ ArrayOf<PathDataNode*>* source)
{
}

Int32 PathParser::NextStart(
    /* [in] */ const String& s,
    /* [in] */ Int32 end)
{
    return 0;
}

void PathParser::AddNode(
    /* [in] */ IArrayList* list,
    /* [in] */ Char32 cmd,
    /* [in] */ ArrayOf<Float>* val)
{
}

AutoPtr<ArrayOf<Float> > PathParser::GetFloats(
    /* [in] */ const String& s)
{
    return NULL;
}

void PathParser::Extract(
    /* [in] */ const String& s,
    /* [in] */ Int32 start,
    /* [in] */ ExtractFloatResult* result)
{
}


} //Utility
} //Droid
} //Elastos
