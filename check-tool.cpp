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
		fprintf(stderr,"::error title={can't get fullpath of shiori}::{_wfullpath returns NULL}");
	shiori.SetTo(shiori_path);
	if(!shiori.All_OK())
		fprintf(stderr,"::error title={shiori load failed}::{shiori.ALL_OK() returns false}");
	auto failed=shiori.yaya_CI_check_failed();
	if (failed){
		fprintf(stderr,"::error title={some error in your dic}::{open your tama!}");
	}
	return failed?EXIT_FAILURE:EXIT_SUCCESS;
};
