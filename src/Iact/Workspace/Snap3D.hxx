//! class Snap3D
//! 

#ifndef  IACT_WORKSPACE_SNAP3D_HXX
#define  IACT_WORKSPACE_SNAP3D_HXX

#include "Iact/Workspace/SnapBase.hxx"
#include "Iact/MouseEventData.hxx"

class WorkspaceController;

// 3D ��׽��
class Snap3D : public SnapBase
{
public:
    Snap3D()
    {
        SetSupportedModes(static_cast<SnapModes>(
            static_cast<int>(SnapModes::Grid) |
            static_cast<int>(SnapModes::Vertex) |
            static_cast<int>(SnapModes::Edge)));
    }

    DEFINE_STANDARD_RTTIEXT(Snap3D, SnapBase)

        SnapInfo Snap(const Graphic3d_Vec2i& screenPoint, const TopoDS_Shape& shape, const Handle(AIS_InteractiveObject)& aisObject)
    {
        SnapInfo snapInfo;

        auto [mode, point] = SnapToShape(screenPoint, shape);
        if(mode != SnapModes::None)
        {
            snapInfo.mode = mode;
            snapInfo.point = point;
        }
        else
        {
            std::tie(mode, point) = SnapToAisObject(screenPoint, aisObject);
            snapInfo.mode = mode;
            snapInfo.point = point;
        }

        SetCurrentInfo(snapInfo);
        return snapInfo;
    }

    SnapInfo Snap(const Handle(MouseEventData)& controller);
    SnapInfo _SnapGrid(Graphic3d_Vec2i point);
};

DEFINE_STANDARD_HANDLE(Snap3D, SnapBase)

#endif  // IACT_WORKSPACE_SNAP3D_HXX

