#include "Settings.h"

static Settings singleton;

Settings* Settings::GetSingleton()
{
	return std::addressof(singleton);
}

void Settings::Load()
{
	constexpr auto path = L"Data/SKSE/Plugins/RaceSwapper.ini";

	CSimpleIniA ini;
	ini.SetUnicode();

	ini.LoadFile(path);

	const char* section = "Features";

    get_value(ini, Features::kPlaythroughRandomization, false, section, "bRandomizePerPlaythrough", "; In each playthough, RaceSwapper will alter NPCs differently, giving a different appearance");

	get_value(ini, Features::kStrictHeadPartMatching, false, section, "bStrictHeadParts", "; RaceSwapper will be stricter when choosing valid headparts for NPCs to reduces chances of using the wrong headpart (eg Khajiit head when swapping to a nord race). This can reduce variety of headpart distribution as a result!");

	get_value(ini, Features::kDebugLogging, false, section, "bDebugLogging", "; Enable debug logging. WARNING: Debug logs are quite spammy when loading lots of NPCs");

	get_value(ini, Features::kUseActorBase, false, section, "bUseActorBase", "; Use the ActorBase FormID instead of the Actor FormID for randomization.\n; This can help to keep the appearance of cloned NPCs stable.\n; Enable this if you use mods that clone NPCs, eg. YieldToMe, PAHE, Diary of Mine, etc.\n; Enabling this reduces randomization variety somewhat.");

	#ifdef _DEBUG 
		Settings::GetSingleton()->features.set(Settings::Features::kDebugLogging);
	#endif

	ini.SaveFile(path);
}
