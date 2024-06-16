// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Route/FlowNode_NamedRerouteDeclaration.h"

#include "FlowAsset.h"
#include "Nodes/Route/FlowNode_NamedRerouteUsage.h"

UFlowNode_NamedRerouteDeclaration::UFlowNode_NamedRerouteDeclaration(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Named Reroute");
	NodeStyle = EFlowNodeStyle::Custom;
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

void UFlowNode_NamedRerouteDeclaration::PostInitProperties()
{
	Super::PostInitProperties();
	// Init the GUID
	UpdateDeclarationGuid(false, false);
}

void UFlowNode_NamedRerouteDeclaration::PostLoad()
{
	Super::PostLoad();
	// Init the GUID
	UpdateDeclarationGuid(false, false);
}

void UFlowNode_NamedRerouteDeclaration::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);
	UpdateDeclarationGuid(false, true);
	MakeNameUnique();
}

FText UFlowNode_NamedRerouteDeclaration::GetNodeTitle() const
{
	return FText::FromString(NodeTitle.ToString());
}

#if WITH_EDITOR
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
		MakeNameUnique();
		for (const UFlowAsset* FlowAsset = GetFlowAsset();
		     const auto Usage : FlowAsset->FindNamedRerouteUsages(DeclarationGuid))
		{
			if (Usage)
			{
				Usage->SetNodeName();
				Usage->GetGraphNode()->ReconstructNode();
				OnReconstructionRequested.ExecuteIfBound();
			}
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//Improper use of this leads to pins not knowing their current owning nodes, resulting in an "Assertion failed: OwningNode". 
	//OnReconstructionRequested.ExecuteIfBound();
}
#endif // WITH_EDITOR

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

void UFlowNode_NamedRerouteDeclaration::MakeNameUnique()
{
#if WITH_EDITORONLY_DATA
	if (const UFlowAsset* FlowAsset = GetFlowAsset())
	{
		int32 NameIndex = 1;
		bool bResultNameIndexValid = true;
		FName PotentialName;

		// Find an available unique name
		do
		{
			PotentialName = NodeTitle;
			if (NameIndex != 1)
			{
				PotentialName.SetNumber(NameIndex);
			}

			bResultNameIndexValid = true;
			for (const auto Pair : FlowAsset->GetNodes())
			{
				if (const auto* OtherDeclaration = Cast<UFlowNode_NamedRerouteDeclaration>(Pair.Value);
					OtherDeclaration && OtherDeclaration != this && OtherDeclaration->NodeTitle == PotentialName)
				{
					bResultNameIndexValid = false;
					break;
				}
			}

			NameIndex++;
		} while (!bResultNameIndexValid);

		NodeTitle = PotentialName;
	}
#endif // WITH_EDITORONLY_DATA
}