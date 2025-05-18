// Copyright [2025] TaCAD

#ifndef OCCT_OCCTHELPER_AISHELPER_H_
#define OCCT_OCCTHELPER_AISHELPER_H_

// Occt includes
#include <Graphic3d_SequenceOfHClipPlane.hxx>
#include <PrsMgr_PresentableObject.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <StdSelect_BRepOwner.hxx>
#include <TopoDS_Shape.hxx>

class AisHelper
{
public:
	static void DisableGlobalClipPlanes(const Handle(PrsMgr_PresentableObject)& object);
	static TopoDS_Shape GetShapeFromEntityOwner(const Handle(SelectMgr_EntityOwner) owner);

    // 静态函数：从选择器中获取拾取的交互对象和形状
    static int GetPickedFromSelector(const Handle(StdSelect_ViewerSelector3d)& selector,
                                     std::vector<Handle(AIS_InteractiveObject)>& interactives,
                                     std::vector<TopoDS_Shape>& shapes)
    {
        int count = 0;
        bool doInteractives = !interactives.empty();
        bool doShapes = !shapes.empty();

        // 清空输出向量
        interactives.clear();
        shapes.clear();

        // 遍历拾取结果
        for(Standard_Integer aPickIter = 1; aPickIter <= selector->NbPicked(); ++aPickIter)
        {
            Handle(SelectMgr_EntityOwner) owner = selector->Picked(aPickIter);
            if(owner.IsNull() || !owner->HasSelectable())
            {
                continue;
            }

            count++;

            // 处理交互对象
            if(doInteractives)
            {
                Handle(AIS_InteractiveObject) interactive =
                    Handle(AIS_InteractiveObject)::DownCast(owner->Selectable());
                interactives.push_back(interactive); // 空 Handle 表示 nullptr
            }

            // 处理形状
            if(doShapes)
            {
                Handle(StdSelect_BRepOwner) brepOwner =
                    Handle(StdSelect_BRepOwner)::DownCast(owner);
                if(!brepOwner.IsNull() && brepOwner->HasShape())
                {
                    shapes.push_back(brepOwner->Shape());
                }
                else
                {
                    shapes.push_back(TopoDS_Shape()); // 空形状表示 nullptr
                }
            }
        }

        return count;
    }
};

#endif  // OCCT_OCCTHELPER_AISHELPER_H_
