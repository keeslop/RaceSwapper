#pragma once

/*-----
* To use this in your own code:
* Copy RaceSwapperPluginAPI.h/cpp into your own project.
* in your SKSE message handler, on kPostPostLoad, request the API:
* -----

// Global API handle
RaceSwapperPluginAPI::IRaceSwapperInterface001 *g_raceSwapAPI = nullptr;

void MessageInterface(SKSE::MessagingInterface::Message* msg) {
	switch (msg->type) {
		case SKSE::MessagingInterface::kPostPostLoad:
		{
			g_raceSwapAPI = RaceSwapperPluginAPI::GetRaceSwapperInterface001();
			if(g_raceSwapAPI) // NULL if user does not have RaceSwapper installed or the version is too old
			{
				// Yay, RaceSwapper API is loaded and ready and can now be used in your code
			}
		}
		break;
	}
}
*/

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
// Interface code based on https://github.com/adamhynek/higgs

namespace RaceSwapperPluginAPI {
	constexpr const auto RaceSwapperPluginName = "RaceSwapper";
	// A message used to fetch RaceSwapper's interface
	struct RaceSwapperMessage {
		enum : uint32_t { kMessage_GetInterface = 0x7cb8a59e };  // Randomly generated
		void* (*GetApiFunction)(unsigned int revisionNumber) = nullptr;
	};

	// This object provides access to RaceSwapper's mod support API
	struct IRaceSwapperInterface001 {
		// Gets the RaceSwapper build number
		virtual unsigned int GetBuildNumber() = 0;

		/// @return Race of NPC. If the NPC wasn't swapped, return original race.
		virtual RE::TESRace* GetAppearanceRaceOfNPC(const RE::TESNPC* a_npc) = 0;

		/// @return Race of Actor. If the Actor wasn't swapped, return original race.
		virtual RE::TESRace* GetAppearanceRaceOfActor(const RE::Actor* a_actor) = 0;

		/// @return Sex of NPC. If the NPC wasn't swapped, return original sex.
		virtual RE::SEX GetAppearanceSexOfNPC(const RE::TESNPC* a_npc) = 0;

		/// @return Sex of Actor. If the Actor wasn't swapped, return original sex.
		virtual RE::SEX GetAppearanceSexOfActor(const RE::Actor* a_actor) = 0;

		/// @return Whether a NPC's appearance was swapped
		virtual bool IsNPCSwapped(const RE::TESNPC *a_npc) = 0;

		/// @return Whether an Actor's appearance was swapped
		virtual bool IsActorSwapped(const RE::Actor *a_actor) = 0;

		/// @return Skin of NPC. If the NPC wasn't swapped, return original skin.
		virtual RE::TESObjectARMO *GetAppearanceSkinOfNPC(const RE::TESNPC* a_npc) = 0;

		/// @return Skin of Actor. If the Actor wasn't swapped, return original skin.
		virtual RE::TESObjectARMO *GetAppearanceSkinOfActor(const RE::Actor* a_actor) = 0;
	};

	// Returns an IRaceSwapperInterface001 object compatible with the API above.
	// This should only be called after SKSE sends kMessage_PostLoad to your plugin
	IRaceSwapperInterface001* GetRaceSwapperInterface001();

}
