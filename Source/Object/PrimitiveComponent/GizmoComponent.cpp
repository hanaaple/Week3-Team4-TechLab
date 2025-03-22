#include "GizmoComponent.h"


UGizmoComponent::UGizmoComponent()
{
	//SetMaterial(TEXT("AlwaysVisibleMaterial"));
	SetStaticMesh(TEXT("GizmoArrow"));
	bCanBeRendered = true;
}

void UGizmoComponent::OnChangedGizmoType(EGizmoType Gizmo)
{
	switch (Gizmo)
	{
	case EGizmoType::Translate:
		SetStaticMesh(TEXT("GizmoArrow"));
		break;
	case EGizmoType::Rotate:
		SetStaticMesh(TEXT("GizmoRotation"));
		break;
	case EGizmoType::Scale:
		SetStaticMesh(TEXT("GizmoScale"));
		break;
	case EGizmoType::Max:
		break;
	default:
		break;
	}
}
