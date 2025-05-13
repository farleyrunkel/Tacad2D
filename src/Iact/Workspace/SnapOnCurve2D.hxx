#ifndef IACT_SNAPPING_SnapOnCurve2D_HXX
#define IACT_SNAPPING_SnapOnCurve2D_HXX

#include <Standard_Handle.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt2d.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_Curve.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2dAPI_InterCurveCurve.hxx>
#include <BRep_Tool.hxx>
#include <BRepLib.hxx>
#include <GeomAPI.hxx>

#include "Iact/InteractiveContext.hxx"
#include "Iact/WorkspaceController.hxx"
#include "Iact/Snapping/SnapBase.hxx"

// ��׽ģʽö��
enum class SnapModes
{
    None = 0,
    Edge = 1 << 0
};

// 2D���߲�׽��������֧�ֱ߲�׽
class SnapOnCurve2D : public SnapBase
{
public:
    // ���캯��
    SnapOnCurve2D()
        : _SupportedModes(static_cast<int>(SnapModes::Edge))
    {}

    // ��������
    virtual ~SnapOnCurve2D() = default;

    // ��ȡ�����ò�׽ƽ��
    const gp_Pln& Plane() const
    {
        return _IsPlaneSet ? _Plane : GetWorkspaceController()->workspace()->WorkingPlane();
    }

    void SetPlane(const gp_Pln& plane)
    {
        _Plane = plane;
        _IsPlaneSet = true;
    }

    // ִ�в�׽�����ز�׽��
    gp_Pnt2d Snap(const Handle(Geom2d_Curve)& curve, const Handle(MouseEventData)& mouseEvent)
    {
        return ProcessSnap(curve, {}/*mouseEvent->DetectedBrepShape()*/);
    }

protected:
    // ������Դ
    virtual void Cleanup() override
    {
        _IsPlaneSet = false;
        SnapBase::Cleanup();
    }

private:
    // ����߲�׽
    gp_Pnt2d ProcessSnap(const Handle(Geom2d_Curve)& curve, const Handle(TopoDS_Shape)& shapeToSnap)
    {
        if(!InteractiveContext::Current()->GetEditorState()->IsSnappingEnabled() ||
           !InteractiveContext::Current()->GetEditorState()->IsSnapToEdgeSelected())
        {
            return gp_Pnt2d(0.0, 0.0);
        }

        if(!shapeToSnap.IsNull() && shapeToSnap->ShapeType() == TopAbs_ShapeEnum::TopAbs_EDGE)
        {
            TopoDS_Edge edge = TopoDS::Edge(*shapeToSnap);
            double umin, umax;
            Handle(Geom_Curve) edgeCurve = BRep_Tool::Curve(edge, umin, umax);
            if(edgeCurve.IsNull() && BRepLib::BuildCurve3d(edge))
            {
                edgeCurve = BRep_Tool::Curve(edge, umin, umax);
            }

            if(!edgeCurve.IsNull())
            {
                Handle(Geom2d_Curve) projected2dCurve = GeomAPI::To2d(edgeCurve, Plane());
                Geom2dAPI_InterCurveCurve intersect(curve, projected2dCurve);
                if(intersect.NbPoints() > 0)
                {
                    return intersect.Point(1);
                }
            }
        }

        return gp_Pnt2d(0.0, 0.0);
    }

private:
    gp_Pln _Plane; // ��׽ƽ��
    bool _IsPlaneSet = false; // ���ƽ���Ƿ�����
    int _SupportedModes; // ֧�ֵĲ�׽ģʽ
};

DEFINE_STANDARD_HANDLE(SnapOnCurve2D, SnapBase)

#endif // IACT_SNAPPING_SnapOnCurve2D_HXX
