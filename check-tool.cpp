#include "my-gists/ukagaka/shiori_loader.hpp"
#include <cstdlib>
#include <cstdio>

Cshiori shiori;

int wmain(int argc,wchar_t**argv){
	//::error file={name},line={line},endLine={endLine},title={title}::{message}
	//::warning file={name},line={line},endLine={endLine},title={title}::{message}
	//::notice file={name},line={line},endLine={endLine},title={title}::{message}
	//
	wchar_t shiori_path[_MAX_PATH]=L"";
	if (!_wfullpath(shiori_path, argv[1], _MAX_PATH))
		fprintf(stderr,"::error title=_wfullpath returns NULL::can't get fullpath of shiori");
	shiori.SetTo(shiori_path);
	if(!shiori.All_OK())
		fprintf(stderr,"::error title=shiori.ALL_OK() returns false::shiori load failed");
	if(!shiori.can_make_CI_check())
		fprintf(stderr,"::error title=checker is NULL::Unsupported shiori");
	auto failed=shiori.CI_check_failed();
	if (failed){
		fprintf(stderr,"::error title=open your tama!::some error in your dic");
	}
	return failed?EXIT_FAILURE:EXIT_SUCCESS;
};
