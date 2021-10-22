#include <stdio.h>
#include <ctype.h>

#define __RELEASE__
#define PUTS_AND_RETURN(arg) {puts(arg); return -1;}
#define BYTE unsigned char
#define LL long long

int main(const int argc, const char* argv[])
{
#ifdef __RELEASE__

	if (argc == 1)
		goto help;
	else if (argc == 2)
	{
		if (argv[1][0] == '-' || argv[1][0] == '/')
		{
			if (argv[1][1] == '?')
			{
			help:
				//没有参数	显示帮助。这与键入 /? 是一样的。
				///?		显示帮助。这与不键入任何选项是一样的。
				puts("用法: HexReader [Path]\n"\
					"\n\t没有参数\t显示帮助。这与键入 /? 是一样的"\
					"\n\t/?\t\t显示帮助。这与不键入任何选项是一样的。"\
					"\n\t[Path]\t\t提供文件路径。这会读取文件并以十六进制格式显示文件内容");
				return 0;
			}
			else
				goto failed;
		}
	}
	else
		failed:
	PUTS_AND_RETURN("参数错误！");

	const char* FilePath = argv[1];
#else
	const char* FilePath = "Debug.bin";
#endif

	FILE* ReadFile = fopen(FilePath, "rb");

	if (!ReadFile)
		PUTS_AND_RETURN("文件打开失败！");

	//变量声明
	LL FilePointerAddress = 0;
	BYTE Save[16] = { 0 };
	BYTE* p = NULL, * p2 = NULL;

	//循环直到文件结尾
	while (!feof(ReadFile))
	{
		for (p = Save; !feof(ReadFile) && (p < Save + 16); p += 1)//读取16个字符
			*p = fgetc(ReadFile);

		printf("%08llx  ", FilePointerAddress);//输出第一个字符的地址
		FilePointerAddress += 16;

		if (p > Save + 8)
		{
			for (p2 = Save; p2 < Save + 8; p2 += 1)//十六进制格式输出
				printf("%02X ", *p2);
			putchar(' ');
			for (p2 = Save + 8; p2 < p; p2 += 1)//十六进制格式输出
				printf("%02X ", *p2);
		}
		else
		{
			for (p2 = Save; p2 < p; p2 += 1)//十六进制格式输出
				printf("%02X ", *p2);
			putchar(' ');
		}

		for (unsigned m = 0; m < (Save + 16 - p) / sizeof(BYTE); m += 1)//对齐
			printf("   ");
		putchar(' ');

		for (p2 = Save; p2 < p; p2 += 1)//字符格式输出
		{
			if (isprint(*p2))
				printf("%c", *p2);
			else
				putchar('.');
		}

		putchar('\n');
	}


	fclose(ReadFile);
	return 0;
}
