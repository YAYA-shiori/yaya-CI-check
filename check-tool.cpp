#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/codepage.hpp"
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
void loghandler(const wchar_t *str, int mode){
	switch(mode){
		case E_SJIS:
		case E_UTF8:
		case E_DEFAULT:
		case E_END:
			break;
		case E_I:/* info */
			fwprintf(stdout,str);
			break;
		case E_F:/* fatal */
			fwprintf(stderr,L"::error title=fatal::%ls",str);
			break;
		case E_E:/* error */
			fwprintf(stderr,L"::error title=error::%ls",str);
			break;
		case E_W:/* warning */
			fwprintf(stderr,L"::warning title=warning::%ls",str);
			break;
		case E_N:/* note */
			fwprintf(stderr,L"::notice title=notice::%ls",str);
			break;
		case E_J:/* other(j) */
			fwprintf(stdout,str);
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
