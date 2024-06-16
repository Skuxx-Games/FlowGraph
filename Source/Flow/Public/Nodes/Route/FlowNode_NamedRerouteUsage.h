// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "CoreMinimal.h"

#include "FlowNode_NamedRerouteDeclaration.h"
#include "FlowNode_NamedRerouteUsage.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class FLOW_API UFlowNode_NamedRerouteUsage : public UFlowNode
{
	GENERATED_UCLASS_BODY()
	friend class UFlowNode_NamedRerouteDeclaration;
public:
	
	// The declaration this node is linked to
	UPROPERTY()
	TObjectPtr<UFlowNode_NamedRerouteDeclaration> Declaration;
	
	// The declaration GUID
	UPROPERTY()
	FGuid DeclarationGuid;
	
#if WITH_EDITOR
	void SetNodeName();
#endif
	
protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FName NodeTitle;
#endif
	
#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;
	virtual FString GetNodeDescription() const override;
	virtual FString GetStatusString() const override;
	virtual EDataValidationResult ValidateNode() override;
	virtual bool GetDynamicTitleColor(FLinearColor& OutColor) const override;
#endif
};