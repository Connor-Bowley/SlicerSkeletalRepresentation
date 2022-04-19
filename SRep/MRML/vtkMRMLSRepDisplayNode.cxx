#include "vtkMRMLSRepDisplayNode.h"
#include "vtkMRMLSRepNode.h"

//--------------------------------------------------------------------------
// vtkMRMLSRepDisplayNode::DisplayHelper
//--------------------------------------------------------------------------
vtkMRMLSRepDisplayNode::DisplayHelper::DisplayHelper()
    : visible(true)
    , color(vtkColor3ub{0,0,0})
{}

vtkMRMLSRepDisplayNode::DisplayHelper::DisplayHelper(bool visible_, const vtkColor3ub& color_)
    : visible(visible_)
    , color(color_)
{}

//--------------------------------------------------------------------------
// vtkMRMLSRepDisplayNode
//--------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLSRepDisplayNode);

vtkMRMLSRepDisplayNode::vtkMRMLSRepDisplayNode()
    : vtkMRMLDisplayNode()
    , OverallVisibility(true)
    , UpSpoke(true, vtkColor3ub{255, 99, 71}) // Tomato
    , DownSpoke(true, vtkColor3ub{189, 252, 201}) // Mint
    , CrestSpoke(true, vtkColor3ub{255, 215, 0}) // Gold
    , CrestCurve(true, vtkColor3ub{255, 215, 0}) // Gold
    , SkeletalSheet(true, vtkColor3ub{255, 248, 220}) // Cornsilk
    , SkeletonToCrestConnection(true, vtkColor3ub{0, 0, 0}) // Black
    , Spine(true, vtkColor3ub{188, 143, 143}) // RosyBrown
    , RelativeThickness(0.001)
    , AbsoluteThickness(0.25)
    , UseAbsoluteThickness(false)
{}

vtkMRMLSRepDisplayNode::~vtkMRMLSRepDisplayNode() = default;

vtkMRMLSRepNode* vtkMRMLSRepDisplayNode::GetSRepNode() {
    return vtkMRMLSRepNode::SafeDownCast(this->GetDisplayableNode());
}

int vtkMRMLSRepDisplayNode::GetVisibility3D() {
    return static_cast<int>(this->OverallVisibility);
}

void vtkMRMLSRepDisplayNode::SetVisibility3D(const int visible) {
    if (visible != this->OverallVisibility) {
        this->OverallVisibility = visible;
        this->Modified();
    }
}

int vtkMRMLSRepDisplayNode::GetVisibility2D() {
    return false;
}

void vtkMRMLSRepDisplayNode::SetVisibility2D(int /*visible*/) {}

void vtkMRMLSRepDisplayNode::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os,indent);

  os << "SRep";
}

#define SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(name) \
    void vtkMRMLSRepDisplayNode::Set##name##Visibility(bool visible) { \
        if (this->name.visible != visible) { \
            this->name.visible = visible; \
            this->Modified(); \
        } \
    } \
    bool vtkMRMLSRepDisplayNode::Get##name##Visibility() const { \
        return this->name.visible; \
    } \
    void vtkMRMLSRepDisplayNode::Set##name##Color(const vtkColor3ub& color) { \
        if (this->name.color != color) { \
            this->name.color = color; \
            this->Modified(); \
        } \
    } \
    const vtkColor3ub& vtkMRMLSRepDisplayNode::Get##name##Color() const { \
        return this->name.color; \
    }

SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(UpSpoke)
SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(DownSpoke)
SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(CrestSpoke)
SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(CrestCurve)
SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(SkeletalSheet)
SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(SkeletonToCrestConnection)
SREP_DISPLAY_NODE_DISPLAY_HELPER_FUNCTIONS(Spine)

void vtkMRMLSRepDisplayNode::SetRelativeThickness(double relativeThickness) {
    if (relativeThickness < 0 || 1 < relativeThickness) {
        vtkErrorMacro("Relative thickness must be between 0 and 1. Found " << relativeThickness);
        return;
    }
    if (this->RelativeThickness != relativeThickness) {
        this->UseAbsoluteThickness = false;
        this->RelativeThickness = relativeThickness;
        this->Modified();
    }
}
double vtkMRMLSRepDisplayNode::GetRelativeThickness() const {
    return this->RelativeThickness;
}

void vtkMRMLSRepDisplayNode::SetAbsoluteThickness(double absoluteThickness) {
    if (this->AbsoluteThickness != absoluteThickness) {
        this->UseAbsoluteThickness = true;
        this->AbsoluteThickness = absoluteThickness;
        this->Modified();
    }
}
double vtkMRMLSRepDisplayNode::GetAbsoluteThickness() const {
    return this->AbsoluteThickness;
}

bool vtkMRMLSRepDisplayNode::GetUseAbsoluteThickness() const {
    return this->UseAbsoluteThickness;
}
void vtkMRMLSRepDisplayNode::SetUseAbsoluteThickness(bool use) {
    if (this->UseAbsoluteThickness != use) {
        this->UseAbsoluteThickness = use;
        this->Modified();
    }
}
void vtkMRMLSRepDisplayNode::UseAbsoluteThicknessOn() {
    this->SetUseAbsoluteThickness(true);
}
void vtkMRMLSRepDisplayNode::UseAbsoluteThicknessOff() {
    this->SetUseAbsoluteThickness(false);
}
