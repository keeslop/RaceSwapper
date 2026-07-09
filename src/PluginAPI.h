#pragma once

// internal use only

#include "RaceSwapperPluginAPI.h"

void* GetRaceSwapperPluginApi(unsigned int revisionNumber);

void PluginApiMessageHandler(SKSE::MessagingInterface::Message *msg);
