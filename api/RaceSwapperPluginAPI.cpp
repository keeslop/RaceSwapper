#include "RaceSwapperPluginAPI.h"
// Interface code based on https://github.com/adamhynek/higgs

using namespace RaceSwapperPluginAPI;


static void *_GetApiVersion(unsigned apiVersion)
{
	// Dispatch a message to get the plugin interface from RaceSwapper
	RaceSwapperMessage msg;
	const auto skseMessaging = SKSE::GetMessagingInterface();
	skseMessaging->Dispatch(RaceSwapperMessage::kMessage_GetInterface, (void*)&msg, sizeof(RaceSwapperMessage*), RaceSwapperPluginName);

	return msg.GetApiFunction
		? msg.GetApiFunction(apiVersion)
		: nullptr;
}

template<typename T>
inline static T *_GetOrQuery(T *& p, unsigned apiVersion)
{
	// If the interface has already been fetched, return the same object
	return p ? p : (p = static_cast<T*>(_GetApiVersion(apiVersion)));
}

// Stores the API after it has already been fetched
static IRaceSwapperInterface001* interface001 = nullptr;


// Fetches the interface to use from MergeMapper
IRaceSwapperInterface001* RaceSwapperPluginAPI::GetRaceSwapperInterface001()
{
	return _GetOrQuery(interface001, 1);
}
