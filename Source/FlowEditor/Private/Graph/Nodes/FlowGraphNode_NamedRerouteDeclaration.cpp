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

void UFlowGraphNode_NamedRerouteDeclaration::PostCopyNode()
{
	Super::PostCopyNode();
	UFlowNode_NamedRerouteDeclaration* Declaration = Cast<UFlowNode_NamedRerouteDeclaration>(GetFlowNode());
	if (const UFlowAsset* FlowAsset = GetFlowNode()->GetFlowAsset();
		FlowAsset && Declaration)
	{
		Declaration->UpdateDeclarationGuid(false, true);
		Declaration->EnsureUniqueNodeTitle();
		
		for (UFlowNode_NamedRerouteUsage* Usage : FlowAsset->FindNamedRerouteUsages(Declaration->DeclarationGuid))
		{
			if (Usage && Usage->Declaration == Declaration)
			{
				Usage->DeclarationGuid = Declaration->DeclarationGuid;
				Usage->SetNodeName();
			}
		}
	}
}

void UFlowGraphNode_NamedRerouteDeclaration::PostPlacedNewNode()
{
	UFlowNode_NamedRerouteDeclaration* Declaration = Cast<UFlowNode_NamedRerouteDeclaration>(GetFlowNode());
	if (const UFlowAsset* FlowAsset = GetFlowNode()->GetFlowAsset();
		FlowAsset && Declaration)
	{
		Declaration->EnsureUniqueNodeTitle();
	}
	Super::PostPlacedNewNode();
}