#include "my-gists/ukagaka/shiori_loader.hpp"
#include <cstdlib>

Cshiori shiori;

int wmain(int argc,wchar_t**argv){
	shiori.SetTo(argv[0]);
	return shiori.yaya_CI_check_failed()?EXIT_FAILURE:EXIT_SUCCESS;
};
