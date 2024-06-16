// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Graph/Nodes/FlowGraphNode_NamedRerouteUsage.h"
#include "FlowAsset.h"
#include "Graph/Widgets/SFlowGraphNode.h"
#include "Nodes/Route/FlowNode_NamedRerouteDeclaration.h"
#include "Nodes/Route/FlowNode_NamedRerouteUsage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowGraphNode_NamedRerouteUsage)

UFlowGraphNode_NamedRerouteUsage::UFlowGraphNode_NamedRerouteUsage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AssignedNodeClasses = {UFlowNode_NamedRerouteUsage::StaticClass()};
}

TSharedPtr<SGraphNode> UFlowGraphNode_NamedRerouteUsage::CreateVisualWidget()
{
	return Super::CreateVisualWidget();
}

void UFlowGraphNode_NamedRerouteUsage::PostCopyNode()
{
	Super::PostCopyNode();
	UFlowNode_NamedRerouteUsage* Usage = Cast<UFlowNode_NamedRerouteUsage>(GetFlowNode());

	if (const UFlowAsset* FlowAsset = GetFlowNode()->GetFlowAsset())
	{
		if (Usage->DeclarationGuid.IsValid())
		{
			if (UFlowNode_NamedRerouteDeclaration* Declaration = FlowAsset->FindNamedRerouteDeclaration(Usage->DeclarationGuid))
			{
				Usage->Declaration = Declaration;
				Usage->DeclarationGuid = Declaration->DeclarationGuid;
				Usage->SetNodeName();
			}
		}
	}

	MarkPackageDirty();
}

void UFlowGraphNode_NamedRerouteUsage::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);

	UFlowNode_NamedRerouteUsage* Usage = Cast<UFlowNode_NamedRerouteUsage>(GetFlowNode());

	if (const UFlowAsset* FlowAsset = GetFlowNode()->GetFlowAsset();
	!Usage->Declaration && FlowAsset)
	{
		if (UFlowNode_NamedRerouteDeclaration* Declaration = FlowAsset->FindNamedRerouteDeclaration(Usage->DeclarationGuid))
		{
			Usage->Declaration = Declaration;
			Usage->DeclarationGuid = Declaration->DeclarationGuid;
			Usage->SetNodeName();
		}
	}

	MarkPackageDirty();
}