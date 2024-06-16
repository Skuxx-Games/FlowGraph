// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"

#include "FlowNode_NamedRerouteDeclaration.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class FLOW_API UFlowNode_NamedRerouteDeclaration : public UFlowNode
{
	GENERATED_UCLASS_BODY()
	friend class UFlowNode_NamedRerouteUsage;
	
protected:
	
	virtual void ExecuteInput(const FName& PinName) override;
	//~ Begin UObject Interface
#if WITH_EDITOR
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	virtual bool GetDynamicTitleColor(FLinearColor& OutColor) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual FText GetNodeTitle() const override;

public:
	UPROPERTY(EditAnywhere, Category = "Named Reroute")
	FName NodeTitle;
	/** The color of the graph node. The same color will apply to all linked usages of this Declaration node */
	UPROPERTY(EditAnywhere, Category = "Named Reroute", DisplayName = "Node Color")
	FLinearColor CustomNodeColor;
	// The declaration GUID
	UPROPERTY()
	FGuid DeclarationGuid;
	/** 
	* Generates a GUID for the declaration if one doesn't already exist
	* @param bForceGeneration	Whether we should generate a GUID even if it is already valid.
	* @param bAllowMarkingPackageDirty
	*/
	void UpdateDeclarationGuid(bool bForceGeneration, bool bAllowMarkingPackageDirty);
	void MakeNameUnique();	
};