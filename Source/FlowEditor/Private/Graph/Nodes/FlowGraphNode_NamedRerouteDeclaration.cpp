// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Graph/Nodes/FlowGraphNode_NamedRerouteDeclaration.h"
#include "FlowAsset.h"
#include "Graph/Widgets/SFlowGraphNode.h"
#include "Nodes/Route/FlowNode_NamedRerouteDeclaration.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowGraphNode_NamedRerouteDeclaration)

UFlowGraphNode_NamedRerouteDeclaration::UFlowGraphNode_NamedRerouteDeclaration(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AssignedNodeClasses = {UFlowNode_NamedRerouteDeclaration::StaticClass()};
}

TSharedPtr<SGraphNode> UFlowGraphNode_NamedRerouteDeclaration::CreateVisualWidget()
{
	return Super::CreateVisualWidget();
}

void UFlowGraphNode_NamedRerouteDeclaration::PostCopyNode()
{
	Super::PostCopyNode();

	UFlowNode_NamedRerouteDeclaration* Declaration = Cast<UFlowNode_NamedRerouteDeclaration>(GetFlowNode());
	if (const UFlowAsset* FlowAsset = GetFlowNode()->GetFlowAsset();
	FlowAsset && Declaration)
	{
		Declaration->UpdateDeclarationGuid(false, true);
		//For now, at the time of copying, the usages maintain the identity of their original declaration.
		//It seems that this is the right place to change that if needed. 
		Declaration->MakeNameUnique();
	}
}