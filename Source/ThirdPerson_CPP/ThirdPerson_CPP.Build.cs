// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThirdPerson_CPP : ModuleRules
{
	public ThirdPerson_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

        //bUseRTTI = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "GameplayTasks", "AIModule", "ReplicationGraph"});

        //PrivateDependencyModuleNames.AddRange(new string[] {  });

        // ��ʹ��Slate UI����ȡ��ע��
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // ��ʹ�����߹��ܣ���ȡ��ע��
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //      if (UEBuildConfiguration.bCompileSteamOSS == true)
        //      {
        //          DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //      }
        // }
    }
}
