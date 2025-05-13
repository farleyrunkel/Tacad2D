// Copyright [2025] TaCAD

// Own include
#include "Occt/OcctHelper/AisHelper.hxx"

// Occt includes
#include <PrsMgr_PresentableObject.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <StdSelect_BRepOwner.hxx>
#include <TopoDS_Shape.hxx>

void AisHelper::DisableGlobalClipPlanes(const Handle(PrsMgr_PresentableObject)& object)
{
	auto seq = new Graphic3d_SequenceOfHClipPlane();
	seq->SetOverrideGlobal(true);
	object->SetClipPlanes(seq);
}

TopoDS_Shape AisHelper::GetShapeFromEntityOwner(const Handle(SelectMgr_EntityOwner) owner)
{
	if (owner.IsNull())
		return {};

	auto brepOwner = Handle(StdSelect_BRepOwner)::DownCast(owner);
	if (brepOwner.IsNull() || !brepOwner->HasShape())
		return {};

	return TopoDS_Shape(brepOwner->Shape());
}
