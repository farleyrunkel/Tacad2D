#include "Iact/Workspace/SnapOnCurve2D.hxx"

const std::shared_ptr<SnapInfoCurve2D> SnapInfoCurve2D::Empty =
std::make_shared<SnapInfoCurve2D>(SnapMode::None, gp_Pnt2d(0, 0), 0);

