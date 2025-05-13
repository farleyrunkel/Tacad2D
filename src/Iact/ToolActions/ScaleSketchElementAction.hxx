#ifndef IACT_TOOLACTIONS_ScaleSketchElementAction_HXX
#define IACT_TOOLACTIONS_ScaleSketchElementAction_HXX

#include <array>
#include <optional>

#include <Bnd_Box2d.hxx>
#include <Extrema_ExtPElC2d.hxx>
#include <ElSLib.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_XY.hxx>
#include <Standard_Handle.hxx>

#include "Iact/InteractiveContext.hxx"
#include "Iact/Shapes/Sketch/SketchEditorTool.hxx"
#include "Iact/Framework/ToolAction.hxx"
#include "Iact/Visual/HintLine.hxx"
#include "Iact/Visual/Marker.hxx"
//#include "Iact/Snapping/SnapOnCurve2D.hxx"

// ���Ų�ͼԪ�صĽ���������ͨ���϶��߽�����������ű���
class ScaleSketchElementAction : public ToolAction
{
public:
    // ���캯������ʼ������
    ScaleSketchElementAction(const Handle(SketchEditorTool)& sketchEditorTool)
        : _SketchEditorTool(sketchEditorTool)
        , _MoveMode(-1)
        , _LengthOfOne(0.0)
        , _LastValue(0.0)
        //, _SnapHandler(nullptr)
    {
        for(auto& line : _Lines)
            line = nullptr;
        for(auto& handle : _Handles)
            handle = nullptr;
    }

    // ��������
    virtual ~ScaleSketchElementAction() = default;

    // ���ñ߽�򲢸����Ӿ�Ԫ��
    void SetBox(const Bnd_Box2d& box)
    {
        myBox = box;
        if(GetWorkspaceController())
            _Update();
    }

    // ��ȡ�߽��
    const Bnd_Box2d& Box() const { return myBox; }

    // ��ȡ������
    const gp_Ax2d& Axis() const { return _Axis; }

    // ��ȡ����ֵ
    double Scaled() const { return _LastValue; }

    // ��ȡ�ο�����
    double Reference() const { return _LengthOfOne; }

    // ��ȡ����¼�����
    const Handle(MouseEventData)& GetMouseEventData() const { return _LastMouseEventData; }

protected:
    // ��ʼ�������������߽��;��
    virtual bool onStart() override
    {
        if(myBox.IsVoid())
            return false;

        _Update();
        return true;
    }

    // ������Դ
    virtual void cleanup() override
    {
        // �����ź�
        Preview.disconnect_all_slots();
        Finished.disconnect_all_slots();

        // �Ƴ�����
        for(auto& line : _Lines)
        {
            if(!line.IsNull())
                Remove(line);
            line = nullptr;
        }

        // �Ƴ����
        for(auto& handle : _Handles)
        {
            if(!handle.IsNull())
                Remove(handle);
            handle = nullptr;
        }

        // �Ƴ���׽������
        //if (!_SnapHandler.IsNull())
        //{
        //    RemoveSnapHandler();
        //    _SnapHandler = nullptr;
        //}

        ToolAction::cleanup();
    }

public:
    // �źŶ���
    Signal<void(const Handle(ScaleSketchElementAction)&)> Preview;
    Signal<void(const Handle(ScaleSketchElementAction)&)> Finished;

    // ����ƶ��¼�
    virtual bool onMouseMove(const Handle(MouseEventData)& data) override
    {
        if(_MoveMode >= 0 && !m_isFinished)
        {
            _EnsureVisuals();
            ProcessMouseInput(data);

            if(_LastValue != 0.0)
            {
                _Handles[_MoveMode]->Set(ElSLib::Value(_Axis.Location().X() + _LastValue * _Axis.Direction().X(),
                                         _Axis.Location().Y() + _LastValue * _Axis.Direction().Y(),
                                         _SketchEditorTool->GetSketch()->Plane()));
                Preview(this);
            }

            return true;
        }
        return ToolAction::onMouseMove(data);
    }

    // ��갴���¼�
    virtual bool onMousePress(const Handle(MouseEventData)& data) override
    {
        _MoveMode = _DetectHandle(data);

        if(_MoveMode >= 0)
        {
            _EnsureVisuals();
            _Handles[_MoveMode]->SetSelectable(false);
            _Handles[_MoveMode]->SetSelected(true);

            //_SnapHandler = new SnapOnCurve2D();
            //_SnapHandler->SetPlane(_SketchEditorTool->GetSketch()->Plane());
            //SetSnapHandler(_SnapHandler);

            ProcessMouseInput(data);

            if(_LastValue != 0.0)
            {
                //SetCursor(CursorStyle::Move);
                Preview(this);
            }

            return true;
        }
        return ToolAction::onMousePress(data);
    }

    // ����ͷ��¼�
    virtual bool onMouseRelease(const Handle(MouseEventData)& data) override
    {
        if(_MoveMode >= 0 && !m_isFinished)
        {
            ProcessMouseInput(data);
            m_isFinished = true;
            Finished(this);

            Remove(_Handles[_MoveMode]);
            _Handles[_MoveMode] = nullptr;

            _MoveMode = -1;
            //SetCursor(CursorStyle::Default);

            _Update();
            //data->ForceReDetection = true;

            return true;
        }
        return ToolAction::onMouseRelease(data);
    }

private:
    // ����������룬ͶӰ��������
    void ProcessMouseInput(const Handle(MouseEventData)& data)
    {
        _LastMouseEventData = data;

        double u, v;
        ElSLib::Parameters(_SketchEditorTool->GetSketch()->Plane(), data->PointOnPlane(), u, v);
        gp_Pnt2d sketchPoint(u, v);

        Extrema_ExtPElC2d extrema(sketchPoint, gp_Lin2d(_Axis), 1e-5, -1e10, 1e10);
        if(extrema.IsDone() && extrema.NbExt() > 0)
        {
            _LastValue = extrema.Point(1).Parameter();
        }
        else
        {
            _LastValue = 0.0;
        }

        //gp_Pnt2d sketchPoint;
        //SnapInfo snapInfo = _SnapHandler->Snap(new Geom2d_Line(_Axis), data);
        //if (snapInfo.Mode != SnapMode::None)
        //{
        //    sketchPoint = snapInfo.Point;
        //}
        //else
        //{
        //    double u, v;
        //    ElSLib::Parameters(_SketchEditorTool->GetSketch()->Plane(), data->PointOnPlane(), u, v);
        //    sketchPoint = gp_Pnt2d(u, v);
        //}
    }

    // ����û�����ľ��
    int _DetectHandle(const Handle(MouseEventData)& data)
    {
        for(size_t i = 0; i < _Handles.size(); ++i)
        {
            //if(!_Handles[i].IsNull() && !data->DetectedAisObject().IsNull() &&
            //   data->DetectedAisObject() == _Handles[i]->AisObject())
            //{
            //    double minX, minY, maxX, maxY;
            //    myBox.Get(minX, minY, maxX, maxY);
            //    double extentX = maxX - minX;
            //    double extentY = maxY - minY;
            //    gp_Pnt2d startPoint(minX + extentX * _HandlePositions[i].X(),
            //                        minY + extentY * _HandlePositions[i].Y());
            //    gp_Vec2d vec(myBox.Center(), startPoint);
            //    _LengthOfOne = vec.Magnitude();
            //    if(_LengthOfOne < 1e-5)
            //        _LengthOfOne = 1.0; // ��ֹ����
            //    _Axis = gp_Ax2d(myBox.Center(), vec);
            //    return static_cast<int>(i);
            //}
        }
        return -1;
    }

    // ȷ���Ӿ�Ԫ�ش���
    void _EnsureVisuals()
    {
        // ȷ������
        for(size_t i = 0; i < _Lines.size(); ++i)
        {
            if(_Lines[i].IsNull())
            {
                _Lines[i] = new HintLine(HintStyle::Solid);
                Add(_Lines[i]);
            }
        }

        // ȷ�����
        for(size_t i = 0; i < _Handles.size(); ++i)
        {
            if(_MoveMode >= 0 && static_cast<int>(i) != _MoveMode)
                continue;

            if(_Handles[i].IsNull())
            {
                _Handles[i] = new Marker(MarkerStyle::Topmost, "Ball", 12);
                _Handles[i]->SetSelectable(true);
                Add(_Handles[i]);
            }
        }

        _Update();
    }

    // ���±߽��������;��
    void _Update()
    {
        double minX, minY, maxX, maxY;
        myBox.Get(minX, minY, maxX, maxY);
        double extentX = maxX - minX;
        double extentY = maxY - minY;
        gp_Trsf transform/* = _SketchEditorTool->GetSketch()->Get()*/;

        // �����������߽���ıߣ�
        for(size_t i = 0; i < _Lines.size(); ++i)
        {
            if(!_Lines[i].IsNull())
            {
                gp_Pnt start(minX + extentX * _HandlePositions[i].X(),
                             minY + extentY * _HandlePositions[i].Y(), 0.0);
                gp_Pnt end(minX + extentX * _HandlePositions[(i + 1) % 4].X(),
                           minY + extentY * _HandlePositions[(i + 1) % 4].Y(), 0.0);
                _Lines[i]->Set(start, end);
                _Lines[i]->SetLocalTransformation(transform);
            }
        }

        // ���¾�����˸��㣩
        for(size_t i = 0; i < _Handles.size(); ++i)
        {
            if(_MoveMode >= 0 && static_cast<int>(i) != _MoveMode)
                continue;

            if(!_Handles[i].IsNull())
            {
                gp_Pnt pos(minX + extentX * _HandlePositions[i].X(),
                           minY + extentY * _HandlePositions[i].Y(), 0.0);
                _Handles[i]->Set(pos);
                _Handles[i]->SetLocalTransformation(transform);
            }
        }
    }

private:
    Handle(SketchEditorTool) _SketchEditorTool; // ��ͼ�༭����
    std::array<Handle(HintLine), 4> _Lines; // �߽���ı�
    std::array<Handle(Marker), 8> _Handles; // �˸����ž��
    std::array<gp_XY, 8> _HandlePositions = { // ������λ��
        gp_XY(0.0, 0.0), gp_XY(1.0, 0.0), gp_XY(1.0, 1.0), gp_XY(0.0, 1.0), // �ǵ�
        gp_XY(0.5, 0.0), gp_XY(0.5, 1.0), gp_XY(0.0, 0.5), gp_XY(1.0, 0.5)  // ���е�
    };
    int _MoveMode; // ��ǰ�϶��ľ��������-1��ʾδѡ�У�
    Bnd_Box2d myBox; // �߽��
    double _LengthOfOne; // �ο�����
    gp_Ax2d _Axis; // ������
    double _LastValue; // �ϴ�����ֵ
    //Handle(SnapOnCurve2D) _SnapHandler; // ��׽������
    Handle(MouseEventData) _LastMouseEventData; // �������¼�����
};

DEFINE_STANDARD_HANDLE(ScaleSketchElementAction, ToolAction)

#endif // IACT_TOOLACTIONS_ScaleSketchElementAction_HXX
