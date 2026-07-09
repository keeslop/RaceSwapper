#include "swap/NPCAppearance.h"
#include "RaceSwapperPluginAPI.h"

using namespace RaceSwapperPluginAPI;


static unsigned int _GetBuildNumber()
{
	auto* plugin = SKSE::PluginDeclaration::GetSingleton();
	return plugin->GetVersion().pack();
}

static RE::TESRace *_GetNPCRace(RE::TESNPC* a_npc)
{
	if(!a_npc)
		return nullptr;
	auto appearance = NPCAppearance::GetNPCAppearance(a_npc);
	return appearance && appearance->isNPCSwapped
		? appearance->alteredNPCData.race
		: a_npc->race;
}

static RE::TESRace *_GetActorRace(RE::Actor* a_actor)
{
	if(!a_actor)
		return nullptr;
	RE::TESNPC *base = a_actor->GetActorBase();
	return base ? _GetNPCRace(base) : a_actor->GetRace();
}

static bool _IsNPCSwapped(RE::TESNPC* a_npc)
{
	if(!a_npc)
		return false;
	auto appearance = NPCAppearance::GetNPCAppearance(a_npc);
	return appearance && appearance->isNPCSwapped;
}

static bool _IsActorSwapped(RE::Actor* a_actor)
{
	if(!a_actor)
		return false;
	RE::TESNPC *base = a_actor->GetActorBase();
	return _IsNPCSwapped(base);
}

static RE::SEX _GetNPCSex(RE::TESNPC *a_npc)
{
	if(!a_npc)
		return RE::SEX::kNone;
	auto appearance = NPCAppearance::GetNPCAppearance(a_npc);
	return appearance && appearance->isNPCSwapped
		? appearance->alteredNPCData.sex
		: a_npc->GetSex();
}

static RE::SEX _GetActorSex(RE::Actor* a_actor)
{
	if(!a_actor)
		return RE::SEX::kNone;
	return _GetNPCSex(a_actor->GetActorBase());
}


struct RaceSwapperInterface001 : public IRaceSwapperInterface001
{
	unsigned int GetBuildNumber() override
	{
		return _GetBuildNumber();
	}
	RE::TESRace* GetAppearanceRaceOfNPC(RE::TESNPC* a_npc) override
	{
		return _GetNPCRace(a_npc);
	}
	RE::TESRace* GetAppearanceRaceOfActor(RE::Actor* a_actor) override
	{
		return _GetActorRace(a_actor);
	}
	bool IsNPCSwapped(RE::TESNPC* a_npc) override
	{
		return _IsNPCSwapped(a_npc);
	}
	bool IsActorSwapped(RE::Actor* a_actor) override
	{
		return _IsActorSwapped(a_actor);
	}
	RE::SEX GetAppearanceSexOfNPC(RE::TESNPC* a_npc) override
	{
		return _GetNPCSex(a_npc);
	}
	RE::SEX GetAppearanceSexOfActor(RE::Actor* a_actor) override
	{
		return _GetActorSex(a_actor);
	}
};

// Interface classes are stored statically
static RaceSwapperInterface001 impl001;


// Constructs and returns an API of the revision number requested
void* GetRaceSwapperPluginApi(unsigned int revisionNumber)
{
	void *api = nullptr;
	switch (revisionNumber)
	{
		case 1: api = &impl001; break;
	}
	logger::info("Interface revision {} requested -> 0x{:X}", revisionNumber, api);
	return api;
}

void PluginApiMessageHandler(SKSE::MessagingInterface::Message* msg)
{
    if (msg->type == RaceSwapperMessage::kMessage_GetInterface)
	{
        RaceSwapperMessage* rsmsg = (RaceSwapperMessage*)msg->data;
        rsmsg->GetApiFunction = GetRaceSwapperPluginApi;
        logger::info("Provided RaceSwapper plugin interface to {}", msg->sender);
    }
}
