#include <dlfcn.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"


#define PATH "/sdcard/Android/data/com.beatgames.beatsaber/files/logdump-"
#define EXT ".txt"

static ModInfo modInfo;


static Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

static const Logger& getLogger() {
    static const Logger logger(modInfo);
    return logger;
}

void write_info(FILE* fp, std::string str) {
    getLogger().debug("%s", str.data());
    fwrite((str + "\n").data(), str.length() + 1, 1, fp);
}


MAKE_HOOK_OFFSETLESS(FlyingSpriteSpawner_SpawnFlyingSprite, void, Il2CppObject* self, Il2CppObject* pos, Il2CppObject* rotation, Il2CppObject* inverseRotation) {
	FlyingSpriteSpawner_SpawnFlyingSprite(self, pos, rotation, inverseRotation);
}



extern "C" void setup(ModInfo& info) {
    info.id = "NoMissText";
    info.version = "1.0.0";
	modInfo = info;


    getLogger().info("Completed setup!");
    getLogger().info("Modloader name: %s", Modloader::getInfo().name.c_str());
    getConfig().Load();

}





// This function is called when the mod is loaded for the first time, immediately after il2cpp_init.
extern "C" void load() {
    getLogger().debug("Installing NoMissText!");
		
	INSTALL_HOOK_OFFSETLESS(FlyingSpriteSpawner_SpawnFlyingSprite, il2cpp_utils::FindMethodUnsafe("", "FlyingSpriteSpawner", "SpawnFlyingSprite", 3)); //burn marks
	
	getLogger().debug("Installed NoMissText!");
    getLogger().info("initialized: %s", il2cpp_functions::initialized ? "true" : "false");
}