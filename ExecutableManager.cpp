
#include "ExecutableManager.h"

ExecutableManager::ExecutableManager() {
	bfd_init();

	const char** names = bfd_target_list();
	for(int i = 0; ; i++) {
		if(!names[i]) {
			break;
		}
		printf("%d: %s\n", i, names[i]);
	}
}

ExecutableManager::~ExecutableManager() {
	//clean up here
	bfd_close(bfdHandle);
}

int ExecutableManager::read(string filename) {
	bfdHandle = bfd_openr(filename.c_str(), "default");

	if(!bfdHandle) {
		bfd_error_type err = bfd_get_error();
		printf("BFD Error (obj creation): %s (%d)\n", bfd_errmsg(err), err);
		return 1;
	}

	if(!bfd_check_format(bfdHandle, bfd_object)) {
		bfd_error_type err = bfd_get_error();
		printf("BFD Error (format check): %s (%d)\n", bfd_errmsg(err), err);
		return 1;
	}
	bfd_error_type err = bfd_get_error();
	printf("BFD Error: %s (%d)\n", bfd_errmsg(err), err);

	return 0;
}

int ExecutableManager::getSectionSize(int n) {
	if(n < bfd_count_sections(bfdHandle)) {
		return -1;
	}
	printf("1.1\n");

	asection* sec;	
	int i = 0;

	printf("1.2\n");
	for(sec = bfdHandle->sections; sec != NULL; sec = sec->next) {
		if(i == n) {
			break;
		}
	}
	printf("1.3\n");

	if(!sec) {
		return -1;
	}

	return (int) bfd_section_size(bfdHandle, sec);
}

int ExecutableManager::getSection(int n, void* contents, int contentsLen) {
	if(n < bfd_count_sections(bfdHandle)) {
		return 1;
	}

	asection* sec = bfdHandle->sections;	
	for(int i = 0; i < n; i++) {
		sec = sec->next;
	}

	bfd_get_section_contents(bfdHandle, sec, contents, 0, contentsLen);
	return 0;
}
