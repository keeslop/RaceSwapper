#include "swap/NPCAppearance.h"
#include "RaceSwapperPluginAPI.h"

using namespace RaceSwapperPluginAPI;


static unsigned int _GetBuildNumber()
{
	auto* plugin = SKSE::PluginDeclaration::GetSingleton();
	return plugin->GetVersion().pack();
}

static RE::TESRace *_GetNPCRace(const RE::TESNPC* a_npc)
{
	if(!a_npc)
		return nullptr;
	auto appearance = NPCAppearance::GetNPCAppearance(a_npc);
	return appearance && appearance->isNPCSwapped
		? appearance->alteredNPCData.race
		: a_npc->race;
}

static RE::TESRace *_GetActorRace(const RE::Actor* a_actor)
{
	if(!a_actor)
		return nullptr;
	const RE::TESNPC *base = a_actor->GetActorBase();
	return base ? _GetNPCRace(base) : a_actor->GetRace();
}

static bool _IsNPCSwapped(const RE::TESNPC* a_npc)
{
	if(!a_npc)
		return false;
	auto appearance = NPCAppearance::GetNPCAppearance(a_npc);
	return appearance && appearance->isNPCSwapped;
}

static bool _IsActorSwapped(const RE::Actor* a_actor)
{
	if(!a_actor)
		return false;
	const RE::TESNPC *base = a_actor->GetActorBase();
	return _IsNPCSwapped(base);
}

static RE::SEX _GetNPCSex(const RE::TESNPC *a_npc)
{
	if(!a_npc)
		return RE::SEX::kNone;
	auto appearance = NPCAppearance::GetNPCAppearance(a_npc);
	return appearance && appearance->isNPCSwapped
		? appearance->alteredNPCData.sex
		: a_npc->GetSex();
}

static RE::SEX _GetActorSex(const RE::Actor* a_actor)
{
	if(!a_actor)
		return RE::SEX::kNone;
	return _GetNPCSex(a_actor->GetActorBase());
}

static RE::TESObjectARMO *_GetNPCSkin(const RE::TESNPC *a_npc)
{
	if(!a_npc)
		return nullptr;
	auto appearance = NPCAppearance::GetNPCAppearance(a_npc);
	return appearance && appearance->isNPCSwapped
		? appearance->alteredNPCData.skin
		: a_npc->skin;
}


static RE::TESObjectARMO *_GetActorSkin(const RE::Actor *a_actor)
{
	if(!a_actor)
		return nullptr;
	RE::TESObjectARMO *skin = _GetNPCSkin(a_actor->GetActorBase());
	if(!skin)
		skin = a_actor->GetSkin();
	return skin;
}



struct RaceSwapperInterface001 : public IRaceSwapperInterface001
{
	unsigned int GetBuildNumber() override
	{
		return _GetBuildNumber();
	}
	RE::TESRace* GetAppearanceRaceOfNPC(const RE::TESNPC* a_npc) override
	{
		return _GetNPCRace(a_npc);
	}
	RE::TESRace* GetAppearanceRaceOfActor(const RE::Actor* a_actor) override
	{
		return _GetActorRace(a_actor);
	}
	bool IsNPCSwapped(const RE::TESNPC* a_npc) override
	{
		return _IsNPCSwapped(a_npc);
	}
	bool IsActorSwapped(const RE::Actor* a_actor) override
	{
		return _IsActorSwapped(a_actor);
	}
	RE::SEX GetAppearanceSexOfNPC(const RE::TESNPC* a_npc) override
	{
		return _GetNPCSex(a_npc);
	}
	RE::SEX GetAppearanceSexOfActor(const RE::Actor* a_actor) override
	{
		return _GetActorSex(a_actor);
	}
	RE::TESObjectARMO *GetAppearanceSkinOfNPC(const RE::TESNPC* a_npc) override
	{
		return _GetNPCSkin(a_npc);
	}
	RE::TESObjectARMO *GetAppearanceSkinOfActor(const RE::Actor* a_actor) override
	{
		return _GetActorSkin(a_actor);
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
