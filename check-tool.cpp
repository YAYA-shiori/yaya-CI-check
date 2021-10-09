#include "my-gists/ukagaka/shiori_loader.hpp"
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
using namespace CODEPAGE_n;

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
	std::wstring_view str=stra;
	//TODO: Get filename & linenum from str like Taromati2
	/*
	//E:\ssp\ghost\Taromati2\ghost\master\dic\system\ERRORLOG.dic(17) : error E0041 : 'for'のループ式が異常です.
	ErrorList.SPLIT{
		_L=SPLIT(RE_REPLACEEX(_argv[0],'\((\d+|-)\) : ',',$1,'),',',3)
		//("E:\ssp\ghost\Taromati2\ghost\master\dic\system\ERRORLOG.dic","17","error E0041 : 'for'のループ式が異常です.")
		_L[2]=SPLIT(RE_REPLACEEX(_L[2],' *([WEN])(\d+|-)( *: |：)',',$1,$2,'),',',4)
		//("E:\ssp\ghost\Taromati2\ghost\master\dic\system\ERRORLOG.dic","17","error","E","0041","'for'のループ式が異常です.")
		_L
	}
	ErrorList.Gene{
		ErrorList.filename=IARRAY
		ErrorList.linenum=IARRAY
		ErrorList.type=IARRAY
		ErrorList.typecode=IARRAY
		ErrorList.code=IARRAY
		ErrorList.Info=IARRAY

		_l=GETERRORLOG
		foreach _l;_i{
			_t=ErrorList.SPLIT(_i)
			ErrorList.filename,=_t[0]
			ErrorList.linenum,=TOINT(_t[1])
			ErrorList.type,=_t[2]
			ErrorList.typecode,=_t[3]
			ErrorList.code,=TOINT(_t[4])
			ErrorList.Info,=_t[5]
		}
	}
	*/
	//then out put info in CI from
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
				fwprintf(stdout, L"::group::request call\n");
			}
			else if (!in_request_end && L"// response (Execution time : " == str.substr(0, 30)) {
				in_request_end = 1;
			}
			else if (in_request_end && str == L"\n") {
				in_request_end = 0;
				fwprintf(stdout, L"::endgroup::\n");
			}
			if (id != 0) {
				fwprintf(stdout, L"");
			}
			if (in_dic_load && id == 8) {//dic load end
				in_dic_load = 0;
				fwprintf(stdout, L"::endgroup::\n");
			}
			fwprintf(stdout,str.data());
			if (id == 3) {//dic load begin
				in_dic_load = 1;
				fwprintf(stdout, L"::group::dic load list\n");
			}
			break;
		case E_F:/* fatal */
			fwprintf(stderr,L"::error title=fatal::%ls",str.data());
			break;
		case E_E:/* error */
			fwprintf(stderr,L"::error title=error::%ls",str.data());
			break;
		case E_W:/* warning */
			fwprintf(stderr,L"::warning title=warning::%ls",str.data());
			break;
		case E_N:/* note */
			fwprintf(stderr,L"::notice title=notice::%ls",str.data());
			break;
		case E_J:/* other(j) */
			fwprintf(stdout,str.data());
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
		fwprintf(stderr,L"::error title=shiori.ALL_OK() returns false::shiori load failed\n");
	if(!shiori.can_make_CI_check()){
		fwprintf(stderr,L"::error title=checker is NULL::Unsupported shiori\n");
		return EXIT_FAILURE;
	}
	auto failed=shiori.CI_check_failed();
	if (failed){
		fwprintf(stderr,L"::error title=open your tama!::some error in your dic\n");
	}
	return failed?EXIT_FAILURE:EXIT_SUCCESS;
};
