// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Route/FlowNode_NamedRerouteUsage.h"

#include "FlowAsset.h"

UFlowNode_NamedRerouteUsage::UFlowNode_NamedRerouteUsage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Named Reroute");
	NodeStyle = EFlowNodeStyle::Custom;
#endif
	InputPins = {};
	OutputPins = {FFlowPin(TEXT("Out"))};
	AllowedSignalModes = {EFlowSignalMode::Enabled, EFlowSignalMode::Disabled, EFlowSignalMode::PassThrough};
}

#if WITH_EDITOR
void UFlowNode_NamedRerouteUsage::SetNodeName()
{
	if (Declaration)
	{
		NodeTitle = FName(*Declaration->GetNodeTitle().ToString());
	}
	else
	{
		NodeTitle = FName(TEXT("Invalid Named Reroute"));
	}
	Modify();
}

FText UFlowNode_NamedRerouteUsage::GetNodeTitle() const
{
	return FText::FromString(NodeTitle.ToString());
}

FString UFlowNode_NamedRerouteUsage::GetNodeDescription() const
{
	if (!Declaration)
	{
		return TEXT("Declaration is NOT valid!");
	}

	return FString();
}

FString UFlowNode_NamedRerouteUsage::GetStatusString() const
{
	if (ActivationState == EFlowNodeState::Active && !Declaration)
	{
		return TEXT("The Named Reroute Declaration that is linked to this usage is NOT valid!");
	}

	return FString();
}

EDataValidationResult UFlowNode_NamedRerouteUsage::ValidateNode()
{
	if (!Declaration)
	{
		ValidationLog.Error<UFlowNode>(TEXT("Named Reroute Declaration is NOT valid!"), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}

bool UFlowNode_NamedRerouteUsage::GetDynamicTitleColor(FLinearColor& OutColor) const
{
	if (Declaration)
	{
		if (NodeStyle == EFlowNodeStyle::Custom)
		{
			OutColor = Declaration->CustomNodeColor;
			return true;
		}
	}

	return false;
}
#endif
