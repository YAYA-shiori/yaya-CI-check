#include "my-gists/ukagaka/shiori_loader.hpp"
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <io.h>

#include <regex>

Cshiori shiori;

#define	E_I			0	/* info */
#define	E_F			1	/* fatal */
#define	E_E			2	/* error */
#define	E_W			3	/* warning */
#define	E_N			4	/* note */
#define E_J			5	/* other(j) */
#define	E_END		6	/* ログの終了 */
#define	E_SJIS		16	/* マルチバイト文字コード＝SJIS */
#define	E_UTF8		17	/* マルチバイト文字コード＝UTF-8 */
#define	E_DEFAULT	32	/* マルチバイト文字コード＝OSデフォルトのコード */
void loghandler(const wchar_t *stra, int mode, int id){
	using namespace std;

	wstring str=stra;

	//Get filename & linenum from str like Taromati2

	size_t linenum=0;
	wstring info,type,filename;
	{
		wsmatch result;
		//E:\ssp\ghost\Taromati2\ghost\master\dic\system\ERRORLOG.dic(17) : error E0041 : 'for'のループ式が異常です.
		if (regex_search(str, result,wregex(L"\\((\\d+|-)\\) : "))) {
			filename=str.substr(0,result.position());//E:\ssp\ghost\Taromati2\ghost\master\dic\system\ERRORLOG.dic
			{
				wstring linenumstr = result[0];//(17) : 
				linenumstr = linenumstr.substr(1);//17) : 
				linenumstr = linenumstr.substr(0, linenumstr.size() - 4);//17
				linenum = (size_t)stoll(linenumstr); 
			}
			info = str.substr(result.position()+result.length());//error E0041 : 'for'のループ式が異常です.
			if (regex_search(info, result, wregex(L" *([WEN])(\\d+|-)( *: |：)"))) {
				type = info.substr(0, result.position());//error
				info = info.substr(result.position()+result.length());//'for'のループ式が異常です.
			}
		}
		switch (mode) {
		case E_F:/* fatal */
			if(type.empty())
				type=L"fatal";
			goto common;
		case E_E:/* error */
			if(type.empty())
				type=L"error";
			goto common;
		case E_W:/* warning */
			if(type.empty())
				type=L"warning";
			goto common;
		case E_N:/* note */
			if(type.empty())
				type=L"notice";
			goto common;
		common:
			if (info.empty())
				info = str;
			if (regex_search(info, result, wregex(L"^// *"))) {
				info = info.substr(result.length());
			}
		}
	}
	/*
	::error file={name},line={line},endLine={endLine},title={title}::{message}
	::warning file={name},line={line},endLine={endLine},title={title}::{message}
	::notice file={name},line={line},endLine={endLine},title={title}::{message}
	*/
	static bool in_dic_load=0,in_request_end=0;
	switch(mode){
		case E_SJIS:
		case E_UTF8:
		case E_DEFAULT:
		case E_END:
			break;
		case E_I:/* info */
			if(L"// request\n"==str){
				fputws(L"::group::request call\n", stdout);
			}
			else if (!in_request_end && L"// response (Execution time : " == str.substr(0, 30)) {
				in_request_end = 1;
			}
			else if (in_request_end && str == L"\n") {
				in_request_end = 0;
				fputws(L"::endgroup::\n", stdout);
			}
			if (in_dic_load && id == 8) {//dic load end
				in_dic_load = 0;
				fputws(L"::endgroup::\n", stdout);
			}
			fputws(str.data(), stdout);
			if (id == 3) {//dic load begin
				in_dic_load = 1;
				fputws(L"::group::dic load list\n", stdout);
			}
			break;
		case E_F:/* fatal */
			fputws(str.data(), stderr);
			fputws((L"::error file="+filename+L",line="+to_wstring(linenum)+L",title="+type+L"::"+info+L"\n").data(), stderr);
			break;
		case E_E:/* error */
			if(id!=57){
				fputws(str.data(), stderr);
				if(id!=10)
					fputws((L"::error file="+filename+L",line="+to_wstring(linenum)+L",title="+type+L"::"+info+L"\n").data(), stderr);
				else
					fputws(L"::error title=Emergency mode::Goes into emergency mode\n", stderr);
			}
			else{
				fputws(str.data(), stdout);
				fputws(L"// from CI checker: E0057 is always ignored in CI check\n", stdout);
			}
			break;
		case E_W:/* warning */
			fputws(str.data(), stderr);
			fputws((L"::warning file=" + filename + L",line=" + to_wstring(linenum) + L",title=" + type + L"::" + info + L"\n").data(), stderr);
			break;
		case E_N:/* note */
			if(id!=0){
				fputws(str.data(), stderr);
				fputws((L"::notice file=" + filename + L",line=" + to_wstring(linenum) + L",title=" + type + L"::" + info + L"\n").data(), stderr);
			}
			else{
				fputws(str.data(), stdout);
				fputws(L"// from CI checker: N0000 is always ignored in CI check\n", stdout);
			}
			break;
		case E_J:/* other(j) */
			fputws(str.data(), stdout);
			break;
	};
}
int wmain(int argc,wchar_t**argv){
	void(_setmode(_fileno(stderr), _O_U8TEXT));
	void(_setmode(_fileno(stdout), _O_U8TEXT));
	void(_setmode(_fileno(stdin), _O_U8TEXT));
	//::error file={name},line={line},endLine={endLine},title={title}::{message}
	//::warning file={name},line={line},endLine={endLine},title={title}::{message}
	//::notice file={name},line={line},endLine={endLine},title={title}::{message}
	//
	shiori.Set_loghandler(loghandler);
	shiori.SetTo(argv[1]);
	if(!shiori.All_OK())
		fputws(L"::error title=shiori.ALL_OK() returns false::shiori load failed\n", stdout);
	if(!shiori.can_make_CI_check()){
		fputws(L"::error title=checker is NULL::Unsupported shiori\n", stdout);
		return EXIT_FAILURE;
	}
	auto failed=shiori.CI_check_failed();
	if (failed){
		fputws(L"::error title=open your tama!::some error in your dic\n", stdout);
	}
	return failed?EXIT_FAILURE:EXIT_SUCCESS;
};
