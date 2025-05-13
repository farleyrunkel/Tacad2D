#include "Iact/Workspace/SnapBase.hxx"

const SnapInfo SnapInfo::Empty = SnapInfo(SnapModes::None, gp_Pnt());

IMPLEMENT_STANDARD_RTTIEXT(SnapBase, Standard_Transient)
