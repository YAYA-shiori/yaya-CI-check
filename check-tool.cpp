#include "my-gists/ukagaka/shiori_loader.hpp"
#include <cstdlib>
#include <cstdio>

Cshiori shiori;

int wmain(int argc,wchar_t**argv){
	wchar_t shiori_path[_MAX_PATH]=L"";
	_wfullpath(shiori_path, argv[1], _MAX_PATH);
	shiori.SetTo(shiori_path);
	auto failed=shiori.yaya_CI_check_failed();
	if (failed){
		fprintf(stderr,"::error");
	}
	return failed?EXIT_FAILURE:EXIT_SUCCESS;
};
