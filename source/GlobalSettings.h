#include <string>

struct GlobalSettings
{
	std::string gameVersion = "0.0.1";

	int windowWidth = 1920;
	int windowHeight = 1080;

	std::string windowTitle = "Procrop";
};

static GlobalSettings globalSettings;
