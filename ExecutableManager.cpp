
#include "ExecutableManager.h"

ExecutableManager::ExecutableManager() {
	bfd_init();
}

ExecutableManager::~ExecutableManager() {
	//clean up here
	bfd_close(bfdHandle);
}

int ExecutableManager::read(string filename) {
	bfdHandle = bfd_fopen(filename.c_str(), "default", "rb", -1);
	if(!bfdHandle) {
		bfd_perror("BFD Error");
		return 1;
	}

	return 0;
}

void ExecutableManager::getSectionNames() {

}

void ExecutableManager::getSectionTypes() {

}

void ExecutableManager::getSection(int n) {

}
