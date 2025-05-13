// class SketchConstraintPointOnSegment
//
// Implements a constraint that places a point onto a segment (line or circle)

#ifndef SketchConstraintPointOnSegment_HXX
#define SketchConstraintPointOnSegment_HXX

#include <map>
#include <memory>
#include <vector>

#include <Standard_Handle.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>

#include "Core/Shapes/Sketch/Constraints/SketchConstraintHelper.hxx"
#include "Core/Shapes/Sketch/Constraints/SketchConstraintFixed.hxx"
#include "Core/Shapes/Sketch/Constraints/SketchConstraintPointOnMidpoint.hxx"
#include "Core/Shapes/Sketch/Segments/SketchSegmentLine.hxx"
#include "Core/Shapes/Sketch/Segments/SketchSegmentCircle.hxx"
#include "Core/Shapes/Sketch/Sketch.hxx"
#include "Core/Shapes/Sketch/SketchConstraint.hxx"
#include "Core/Shapes/Sketch/SketchConstraintSolver.hxx"
#include "Core/Shapes/Sketch/SketchSegment.hxx"

class SketchConstraintPointOnSegment : public SketchConstraint
{
public:
    SketchConstraintPointOnSegment(int point, int segment,
                                   const TDF_Label& label = DocumentTool::SketchConstraintPointOnSegmentsLabel().NewChild())
        : SketchConstraint(label.FindChild(0))
        , _Label(label)
    {
        assert(!label.HasAttribute());

        TDataStd_Name::Set(label, "SketchConstraintPointOnSegment");

        SetSegments({segment});
        SetPoints({point});
    }

    SketchConstraintPointOnSegment(const TDF_Label& label)
        : SketchConstraint(label.FindChild(0))
        , _Label(label)
    {
        assert(label.HasAttribute());
    }

    TDF_Label Label() const { return _Label; }

    virtual bool MakeConstraint(const std::map<int, gp_Pnt2d>& points,
                                const std::map<int, Handle(SketchSegment)>& segments,
                                const Handle(SketchConstraintSolver)& solver) override
    {
        auto it = segments.find(Segment(0));
        if(it == segments.end())
        {
            return false;
        }

        bool valid = false;
        Handle(SketchSegment) segment = it->second;

        if(Handle(SketchSegmentLine) line = Handle(SketchSegmentLine)::DownCast(segment))
        {
            Constraint con;
            con.type = ConstraintType::PointOnLine;

            valid = solver->SetLine(con.line1, line->PointsAttribute()->Value(0),
                                    line->PointsAttribute()->Value(1), true, true);
            valid &= solver->SetPoint(con.point1, Point(0), false);

            if(valid)
            {
                solver->AddConstraint(con);
            }
        }
        else if(Handle(SketchSegmentCircle) circle = Handle(SketchSegmentCircle)::DownCast(segment))
        {
            std::vector<Constraint> constraints;

            Constraint con;
            con.type = ConstraintType::PointOnCircle;

            valid = solver->SetCircle(con.circle1, circle, constraints, points, true, false);
            valid &= solver->SetPoint(con.point1, Point(0), false);

            constraints.push_back(con);

            if(valid)
            {
                for(const auto& c : constraints)
                {
                    solver->AddConstraint(c);
                }
            }
        }

        return valid;
    }

    virtual Handle(SketchConstraint) Clone() const override
    {
        return new SketchConstraintPointOnSegment(Point(0), Segment(0));
    }

    static bool CanCreate(const Handle(Sketch)& sketch,
                          const std::vector<int>& points,
                          const std::vector<int>& segments)
    {
        return points.size() > 0
            && segments.size() == 1
            && (SketchConstraintHelper::AllSegmentsOfType<SketchSegmentLine>(sketch, segments)
                || SketchConstraintHelper::AllSegmentsOfType<SketchSegmentCircle>(sketch, segments))
            && !SketchConstraintHelper::AnySegmentAndPointConstraint<SketchConstraintPointOnSegment>(sketch, points, segments)
            && !SketchConstraintHelper::AnySegmentAndPointConstraint<SketchConstraintPointOnMidpoint>(sketch, points, segments)
            && !SketchConstraintHelper::AnyConstrainedPoint<SketchConstraintFixed>(sketch, points);
    }

    static std::vector<Handle(SketchConstraint)> Create(const Handle(Sketch)& sketch,
                                                        const std::vector<int>& points,
                                                        const std::vector<int>& segments)
    {
        std::vector<Handle(SketchConstraint)> list;

        auto it = sketch->Segments().find(segments[0]);
        if(it == sketch->Segments().end())
        {
            return list;
        }

        Handle(SketchSegment) segment = it->second;

        for(const auto& pointIndex : points)
        {
            bool contains = false;
            for(const auto& segPoint : *segment->PointsAttribute()->Array())
            {
                if(segPoint == pointIndex)
                {
                    contains = true;
                    break;
                }
            }
            if(!contains)
            {
                list.push_back(new SketchConstraintPointOnSegment(pointIndex, segments[0]));
            }
        }

        return list;
    }

private:
    TDF_Label _Label;
};

DEFINE_STANDARD_HANDLE(SketchConstraintPointOnSegment, SketchConstraint)

#endif // SketchConstraintPointOnSegment_HXX
