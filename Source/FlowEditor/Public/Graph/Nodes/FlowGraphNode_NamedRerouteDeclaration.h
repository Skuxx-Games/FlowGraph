// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Graph/Nodes/FlowGraphNode.h"
#include "FlowGraphNode_NamedRerouteDeclaration.generated.h"

/**
 * 
 */
UCLASS()
class FLOWEDITOR_API UFlowGraphNode_NamedRerouteDeclaration : public UFlowGraphNode
{
	GENERATED_UCLASS_BODY()
	
	// UEdGraphNode
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual void PostCopyNode() override;
	
	// --
};