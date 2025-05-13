//! class SnapBase
//! 

#ifndef IACT_WORKSPACE_SNAPBASE_HXX
#define IACT_WORKSPACE_SNAPBASE_HXX

#include <bitset>
#include <string>

#include <AIS_InteractiveObject.hxx>
#include <AIS_Point.hxx>
#include <BRep_Tool.hxx>
#include <BRepLib.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_Point.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <gp_Pnt.hxx>
#include <Standard_DefineHandle.hxx>
#include <Standard_Transient.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>

class WorkspaceController;

// ��׽ģʽö��
enum class SnapModes
{
    None = 0,
    Grid = 1 << 0,
    Vertex = 1 << 1,
    Edge = 1 << 2,
    Face = 1 << 3
};

// ��׽��Ϣ�ṹ��
struct SnapInfo
{
    SnapModes mode;
    gp_Pnt point;
    std::string targetName;

    SnapInfo(SnapModes m = SnapModes::None, const gp_Pnt& p = gp_Pnt())
        : mode(m), point(p), targetName("")
    {}

    static const SnapInfo Empty;
};

// ��׽�������ӿ�
class ISnapHandler
{
public:
    virtual ~ISnapHandler() = default;
    virtual SnapModes GetSupportedModes() const = 0;
    virtual void SetSupportedModes(SnapModes modes) = 0;
    virtual SnapInfo GetCurrentInfo() const = 0;
    virtual void SetCurrentInfo(const SnapInfo& info) = 0;
};

// ������׽��
class SnapBase : public Standard_Transient, public ISnapHandler
{
public:
    SnapBase() : supportedModes_(SnapModes::None), currentInfo_(SnapInfo::Empty) {}
    virtual ~SnapBase() = default;

    DEFINE_STANDARD_RTTIEXT(SnapBase, Standard_Transient)

    SnapModes GetSupportedModes() const override { return supportedModes_; }
    void SetSupportedModes(SnapModes modes) override { supportedModes_ = modes; }
    SnapInfo GetCurrentInfo() const override { return currentInfo_; }
    void SetCurrentInfo(const SnapInfo& info) override { currentInfo_ = info; }
	void SetWorkspaceController(const Handle(WorkspaceController)& controller) { workspaceController_ = controller; }
	Handle(WorkspaceController) GetWorkspaceController() const { return workspaceController_; }
    virtual void Cleanup() {}

protected:
    std::pair<SnapModes, gp_Pnt> SnapToShape(const Graphic3d_Vec2i& screenPoint, const TopoDS_Shape& shape)
    {
        if(shape.IsNull())
        {
            return {SnapModes::None, gp_Pnt()};
        }

        if(HasFlag(supportedModes_, SnapModes::Vertex) && shape.ShapeType() == TopAbs_VERTEX)
        {
            TopoDS_Vertex vertex = TopoDS::Vertex(shape);
            return {SnapModes::Vertex, BRep_Tool::Pnt(vertex)};
        }

        if(HasFlag(supportedModes_, SnapModes::Edge) && shape.ShapeType() == TopAbs_EDGE)
        {
            TopoDS_Edge edge = TopoDS::Edge(shape);
            Standard_Real umin, umax;
            auto curve = BRep_Tool::Curve(edge, umin, umax);
			if(curve.IsNull() && BRepLib::BuildCurve3d(edge))
			{
                curve = BRep_Tool::Curve(edge, umin, umax);
            }

            if(!curve.IsNull())
            {
                Handle(Geom_Line) ray = new Geom_Line(gp_Pnt(screenPoint.x(), screenPoint.y(), 0), gp_Dir(0, 0, 1));
                GeomAPI_ExtremaCurveCurve extrema(curve, ray);

                if(extrema.NbExtrema() >= 1)
                {
                    gp_Pnt p1, p2;
                    extrema.NearestPoints(p1, p2);
                    return {SnapModes::Edge, p1};
                }
            }
        }

        return {SnapModes::None, gp_Pnt()};
    }

    std::pair<SnapModes, gp_Pnt> SnapToAisObject(const Graphic3d_Vec2i& Graphic3d_Vec2i, const Handle(AIS_InteractiveObject)& aisObject)
    {
        if(aisObject.IsNull())
        {
            return {SnapModes::None, gp_Pnt()};
        }

        if(HasFlag(supportedModes_, SnapModes::Vertex))
        {
            Handle(AIS_Point) aisPoint = Handle(AIS_Point)::DownCast(aisObject);
            if(!aisPoint.IsNull())
            {
                return {SnapModes::Vertex, aisPoint->Component()->Pnt()};
            }
        }

        return {SnapModes::None, gp_Pnt()};
    }

    // ��������������־λ
    bool HasFlag(SnapModes modes, SnapModes flag) const
    {
        return (static_cast<int>(modes) & static_cast<int>(flag)) != 0;
    }

protected:
    SnapModes supportedModes_;
    SnapInfo currentInfo_;
	Handle(WorkspaceController) workspaceController_;
 };

// ������
DEFINE_STANDARD_HANDLE(SnapBase, Standard_Transient)

#endif  // IACT_WORKSPACE_SNAPBASE_HXX
