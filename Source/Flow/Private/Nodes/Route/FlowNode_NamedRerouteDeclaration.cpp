// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Route/FlowNode_NamedRerouteDeclaration.h"

#include "FlowAsset.h"
#include "Nodes/Route/FlowNode_NamedRerouteUsage.h"

UFlowNode_NamedRerouteDeclaration::UFlowNode_NamedRerouteDeclaration(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Route");
	NodeStyle = EFlowNodeStyle::Custom;
	CustomNodeColor = FLinearColor::MakeRandomColor();
#endif
	NodeTitle = TEXT("Named Reroute");
	AllowedSignalModes = {EFlowSignalMode::Enabled, EFlowSignalMode::Disabled, EFlowSignalMode::PassThrough};
}

void UFlowNode_NamedRerouteDeclaration::ExecuteInput(const FName& PinName)
{
	TriggerFirstOutput(true);
	for (const UFlowAsset* FlowAsset = GetFlowAsset();
	const auto Usage : FlowAsset->FindNamedRerouteUsages(DeclarationGuid))
	{
		if (Usage)
		{
			Usage->TriggerFirstOutput(true);
		}
	}
}

#if WITH_EDITOR
FText UFlowNode_NamedRerouteDeclaration::GetNodeTitle() const
{
	return FText::FromString(NodeTitle.ToString());
}

bool UFlowNode_NamedRerouteDeclaration::GetDynamicTitleColor(FLinearColor& OutColor) const
{
	if (NodeStyle == EFlowNodeStyle::Custom)
	{
		OutColor = CustomNodeColor;
		return true;
	}

	return false;
}

void UFlowNode_NamedRerouteDeclaration::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, NodeTitle) ||
		PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, CustomNodeColor))
	{
		for (const UFlowAsset* FlowAsset = GetFlowAsset();
		     const auto Usage : FlowAsset->FindNamedRerouteUsages(DeclarationGuid))
		{
			if (Usage)
			{
				Usage->SetNodeName();
			}
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UFlowNode_NamedRerouteDeclaration::UpdateDeclarationGuid(const bool bForceGeneration, const bool bAllowMarkingPackageDirty)
{
	// If we are in the editor, and we don't have a valid GUID yet, generate one.
	if (GIsEditor && !FApp::IsGame())
	{
		if (bForceGeneration || !DeclarationGuid.IsValid())
		{
			DeclarationGuid = FGuid::NewGuid();

			if (bAllowMarkingPackageDirty)
			{
				MarkPackageDirty();
			}
		}
	}
}

void UFlowNode_NamedRerouteDeclaration::EnsureUniqueNodeTitle()
{
	if (const UFlowAsset* FlowAsset = GetFlowAsset())
	{
		int32 NameIndex = 1;
		FString BaseTitle = NodeTitle.ToString();
		FName UniqueTitle = NodeTitle;

		while (true)
		{
			bool bIsUnique = true;
			for (const auto& Pair : FlowAsset->GetNodes())
			{
				if (const UFlowNode_NamedRerouteDeclaration* OtherDeclaration = Cast<UFlowNode_NamedRerouteDeclaration>(Pair.Value))
				{
					if (OtherDeclaration && OtherDeclaration != this && OtherDeclaration->NodeTitle == UniqueTitle)
					{
						bIsUnique = false;
						break;
					}
				}
			}

			if (bIsUnique)
			{
				break;
			}

			UniqueTitle = FName(*FString::Printf(TEXT("%s_%d"), *BaseTitle, NameIndex));
			++NameIndex;
		}

		NodeTitle = UniqueTitle;
	}
}
#endif // WITH_EDITOR