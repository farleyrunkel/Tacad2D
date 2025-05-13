//#ifndef SKETCHTOOLSCALE_HXX
//#define SKETCHTOOLSCALE_HXX
//
//#include <map>
//#include <set>
//#include <vector>
//
//#include <Bnd_Box2d.hxx>
//#include <BndLib_Add2dCurve.hxx>
//#include <gp_Ax2d.hxx>
//#include <gp_Pnt2d.hxx>
//#include <gp_XY.hxx>
//#include <Standard_Handle.hxx>
//
//#include "Core/Geom/MathUtils.hxx"
//#include "Core/Shapes/Sketch/Elements/SketchElementPoint.hxx"
//#include "Core/Shapes/Sketch/Elements/SketchElementSegment.hxx"
//#include "Core/Shapes/Sketch/Segments/SketchSegment.hxx"
//#include "Core/Shapes/Sketch/Sketch.hxx"
//#include "Iact/HudElements/LabelHudElement.hxx"
//#include "Iact/InteractiveContext.hxx"
//#include "Iact/Shapes/Sketch/SketchTool.hxx"
//#include "Iact/ToolActions/ScaleSketchElementAction.hxx"
//
//// ���Ų�ͼԪ�صĹ��ߣ������û�ͨ���϶��������ѡ���ĵ���߶�
//class SketchToolScale : public SketchTool
//{
//public:
//    // ���캯������ʼ����Ա����
//    SketchToolScale()
//        : _ToolAction(nullptr)
//        , _HudElement(nullptr)
//        , _PreviewIsValid(false)
//    {}
//
//    // ��������
//    virtual ~SketchToolScale() = default;
//
//    // ��ʼ�����ߣ��������Ų���
//    virtual bool OnStart() override
//    {
//        // ���ز�ͼԼ���Լ����Ӿ�����
//        SketchEditorTool->Elements()->SetConstraintsVisible(false);
//
//        // �ռ�ѡ�е��߶κ͵�
//        _OriginalSegments = SketchEditorTool->SelectedSegments();
//        std::set<int> pointSet;
//        for(const auto& seg : _OriginalSegments)
//        {
//            auto points = seg->PointsAttribute()->Array();
//            pointSet.insert(points.begin(), points.end());
//        }
//        for(int point : SketchEditorTool->SelectedPoints())
//            pointSet.insert(point);
//        _SelectedPoints.assign(pointSet.begin(), pointSet.end());
//
//        if(_SelectedPoints.empty() && _OriginalSegments.empty())
//            return false;
//
//        // ��¡�߶Σ�������ʱ������������Ԥ��
//        _TempSegments.clear();
//        for(size_t i = 0; i < _OriginalSegments.size(); ++i)
//        {
//            _TempSegments[static_cast<int>(i)] = _OriginalSegments[i]->Clone();
//        }
//
//        // ��ʼ�����Ŷ���
//        _ToolAction = new ScaleSketchElementAction(SketchEditorTool);
//        if(!_UpdateBoundingBox())
//            return false;
//
//        // ����Ԥ��������ź�
//        _ToolAction->Preview.connect([this](const auto& sender, const auto& args) { _PointAction_Preview(sender, args); });
//        _ToolAction->Finished.connect([this](const auto& sender, const auto& args) { _PointAction_Finished(sender, args); });
//
//        if(!StartAction(_ToolAction))
//            return false;
//
//        // ���ò�ͼԪ��ѡ��
//        SketchEditorTool->Elements()->Activate(false, false, false);
//
//        // �����û���ʾ
//        SetHintMessage("Scale selected elements by dragging handles, press Ctrl to round to 5%, press Shift to scale relative to center.");
//
//        return true;
//    }
//
//    // �����ͼ�仯�����±߽��
//    virtual void OnSketchChanged(const Handle(Sketch)& sketch, Sketch::ElementType types) override
//    {
//        _UpdateBoundingBox();
//    }
//
//    // ֹͣ����
//    virtual void OnStop() override
//    {
//        SketchTool::OnStop();
//        Stoped();
//    }
//
//    // ������Դ
//    virtual void cleanup() override
//    {
//        // �Ƴ�Ԥ����Ԫ��
//        for(const auto& [key, value] : _PreviewPointElements)
//        {
//            if(!value.IsNull())
//                value->ForgetAllAttributes();
//        }
//        _PreviewPointElements.clear();
//
//        // �Ƴ�Ԥ���߶�Ԫ��
//        for(const auto& [key, value] : _PreviewSegmentElements)
//        {
//            if(!value.IsNull())
//                value->ForgetAllAttributes();
//        }
//        _PreviewSegmentElements.clear();
//
//        // �Ƴ�HUDԪ��
//        if(_HudElement)
//        {
//            Remove(_HudElement);
//            _HudElement = nullptr;
//        }
//
//        // ������Ŷ���
//        if(_ToolAction)
//        {
//            _ToolAction->Reset();
//            _ToolAction = nullptr;
//        }
//
//        // �����ʱ����
//        _TempPoints.clear();
//        _TempSegments.clear();
//        _SelectedPoints.clear();
//        _OriginalSegments.clear();
//
//        SketchTool::cleanup();
//    }
//
//private:
//    // ���±߽�򣬻���ѡ�еĵ���߶�
//    bool _UpdateBoundingBox()
//    {
//        Bnd_Box2d box;
//        const auto& points = _TempPoints.empty() ? SketchEditorTool->GetSketch()->Points() : _TempPoints;
//        for(int i : _SelectedPoints)
//        {
//            box.Add(points.at(i));
//        }
//        for(const auto& seg : _OriginalSegments)
//        {
//            if(!seg->CachedCurve().IsNull())
//            {
//                BndLib_Add2dCurve::Add(seg->CachedCurve(), 0.01, box);
//            }
//        }
//        if(box.IsVoid())
//            return false;
//
//        _ToolAction->SetBox(box);
//        return true;
//    }
//
//    // ����ԭʼ�߶μ��������Ŀɼ���
//    void _UpdateSegmentElementsVisibility(bool show)
//    {
//        if(_OriginalSegments.empty())
//            return;
//
//        // ��ʾ/�����߶�Ԫ��
//        auto segmentElements = SketchEditorTool->Elements()->SegmentElements();
//        for(const auto& seg : _OriginalSegments)
//        {
//            for(auto& element : segmentElements)
//            {
//                if(element->Segment() == seg)
//                    element->SetVisible(show);
//            }
//        }
//
//        // ��ʾ/���ؽ���ԭʼ�߶�ʹ�õĵ�
//        std::set<int> allPoints;
//        for(const auto& seg : _OriginalSegments)
//        {
//            auto points = seg->PointsAttribute()->Array();
//            allPoints.insert(points.begin(), points.end());
//        }
//
//        for(int pointIndex : allPoints)
//        {
//            std::vector<Handle(SketchSegment)> segmentsWithPoint;
//            for(const auto& [idx, seg] : SketchEditorTool->GetSketch()->Segments())
//            {
//                auto segPoints = seg->PointsAttribute()->Array();
//                if(std::find(segPoints.begin(), segPoints.end(), pointIndex) != segPoints.end())
//                    segmentsWithPoint.push_back(seg);
//            }
//
//            bool allContained = true;
//            for(const auto& seg : segmentsWithPoint)
//            {
//                if(std::find(_OriginalSegments.begin(), _OriginalSegments.end(), seg) == _OriginalSegments.end())
//                {
//                    allContained = false;
//                    break;
//                }
//            }
//
//            if(allContained)
//            {
//                for(auto& element : SketchEditorTool->Elements()->PointElements())
//                {
//                    if(element->PointIndex() == pointIndex)
//                        element->SetVisible(show);
//                }
//            }
//        }
//
//        if(show)
//        {
//            SketchEditorTool->Elements()->OnPointsChanged(SketchEditorTool->GetSketch()->Points(),
//                                                          SketchEditorTool->GetSketch()->Segments());
//        }
//    }
//
//    // �������Ŷ�����Ԥ������
//    void _PointAction_Preview(const Handle(ScaleSketchElementAction)& sender, const ScaleSketchElementAction::EventArgs& args)
//    {
//        double scale = _UpdatePoints(args);
//        _UpdateElements();
//
//        if(!_HudElement)
//        {
//            _HudElement = new LabelHudElement();
//            Add(_HudElement);
//        }
//        _HudElement->SetText("Scale: " + std::to_string(static_cast<int>(scale * 100)) + "%");
//    }
//
//    // �������Ŷ������
//    void _PointAction_Finished(const Handle(ScaleSketchElementAction)& sender, const ScaleSketchElementAction::EventArgs& args)
//    {
//        if(_HudElement)
//        {
//            Remove(_HudElement);
//            _HudElement = nullptr;
//        }
//
//        _UpdatePoints(args);
//
//        // ɾ��ԭʼ�߶�
//        for(const auto& seg : _OriginalSegments)
//            SketchEditorTool->GetSketch()->DeleteSegment(seg);
//
//        // ���ѡ�еĵ�
//        SketchEditorTool->SelectedPoints().clear();
//
//        // ������ź�ĵ���߶�
//        SketchEditorTool->GetSketch()->AddElements(_TempPoints, {}, _TempSegments, {});
//
//        // �����ʱ����
//        _OriginalSegments.clear();
//        _TempPoints.clear();
//        _TempSegments.clear();
//
//        // �ύ������ʷ
//        InteractiveContext::Current()->UndoHandler()->Commit();
//
//        Stop();
//    }
//
//    // ���µ�λ�ã��������ű任
//    double _UpdatePoints(const ScaleSketchElementAction::EventArgs& args)
//    {
//        if(_TempPoints.empty())
//        {
//            _TempPoints = SketchEditorTool->GetSketch()->Points();
//        }
//
//        gp_XY scaleCenter = args.Axis.Location().Coord();
//        double scaleFactor = args.Scaled / args.Reference;
//
//        if(!InteractiveContext::IsModifierKeyPressed(ModifierKey::Shift))
//        {
//            scaleCenter.Add(args.Axis.Direction().Coord().Multiplied(-args.Reference));
//            scaleFactor = (args.Scaled + args.Reference) / (args.Reference * 2.0);
//        }
//
//        if(InteractiveContext::IsModifierKeyPressed(ModifierKey::Control))
//        {
//            scaleFactor = MathUtils::RoundToNearest(scaleFactor, 0.05);
//        }
//
//        scaleFactor = MathUtils::Clamp(scaleFactor, 0.01, 10000.0);
//        gp_XY scale(args.Axis.Direction().X() == 0 ? 1.0 : scaleFactor,
//                    args.Axis.Direction().Y() == 0 ? 1.0 : scaleFactor);
//
//        const auto& sketchPoints = SketchEditorTool->GetSketch()->Points();
//        for(int i : _SelectedPoints)
//        {
//            gp_XY coord = sketchPoints.at(i).Coord();
//            coord.Subtract(scaleCenter);
//            coord.Multiply(scale);
//            coord.Add(scaleCenter);
//            _TempPoints[i] = gp_Pnt2d(coord);
//        }
//
//        return scaleFactor;
//    }
//
//    // ����Ԥ��Ԫ�أ���ʾ����Ч��
//    void _UpdateElements()
//    {
//        if(!_PreviewIsValid)
//        {
//            // ����ԭʼԪ��
//            _UpdateSegmentElementsVisibility(false);
//
//            // ������Ԥ��Ԫ��
//            const auto& sketchPoints = SketchEditorTool->GetSketch()->Points();
//            for(const auto& point : _SelectedPoints)
//            {
//                _PreviewPointElements[point] = new SketchElementPoint(point, gp::Origin2d());
//            }
//
//            // �����߶�Ԥ��Ԫ��
//            for(const auto& [key, seg] : _TempSegments)
//            {
//                _PreviewSegmentElements[key] = new SketchElementSegment(seg);
//            }
//
//            _PreviewIsValid = true;
//        }
//
//        // ���µ�Ԥ��
//        for(const auto& point : _SelectedPoints)
//        {
//            _PreviewPointElements[point]->OnPointsChanged(_TempPoints, {});
//        }
//
//        // �����߶�Ԥ��
//        for(const auto& [key, seg] : _TempSegments)
//        {
//            _PreviewSegmentElements[key]->OnPointsChanged(_TempPoints, {});
//        }
//    }
//
//private:
//    std::vector<int> _SelectedPoints; // ѡ�еĵ�����
//    std::vector<Handle(SketchSegment)> _OriginalSegments; // ԭʼ�߶�
//    std::map<int, gp_Pnt2d> _TempPoints; // ���ź����ʱ��
//    std::map<int, Handle(SketchSegment)> _TempSegments; // ���ź����ʱ�߶�
//    std::map<int, Handle(SketchElementPoint)> _PreviewPointElements; // Ԥ����Ԫ��
//    std::map<int, Handle(SketchElementSegment)> _PreviewSegmentElements; // Ԥ���߶�Ԫ��
//    Handle(ScaleSketchElementAction) _ToolAction; // ���Ŷ���
//    Handle(LabelHudElement) _HudElement; // HUDԪ�أ���ʾ���ű���
//    bool _PreviewIsValid; // Ԥ����Ч��־
//
//public:
//    static Signal<void()> Stoped;
//};
//
//#endif // SKETCHTOOLSCALE_HXX
