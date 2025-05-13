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
};

#endif  // OCCT_OCCTHELPER_AISHELPER_H_
