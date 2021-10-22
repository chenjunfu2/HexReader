#include <stdio.h>
#include <ctype.h>

#define __RELEASE__

#define PUTS_AND_RETURN(arg) {puts(arg); return -1;}
#define BYTE unsigned char
#define LLADRESS long long
#define READSIZE int

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

#ifndef __RELEASE__
	while (!feof(ReadFile))
		printf("%d ", fread(Save, 1, 16, ReadFile));
	putchar('\n');
#endif


	//变量声明
	LLADRESS FilePointerAddress = 0;
	READSIZE r = 0;
	BYTE Save[16] = { 0 };
	BYTE* p = NULL, * SaveAddp = NULL;


	//循环直到文件结尾
	while (!feof(ReadFile))
	{
		if (!(r = fread(Save, 1, 16, ReadFile)))//读取16个字符
			break;
		SaveAddp = Save + r;

		printf("%08llx  ", FilePointerAddress);//输出第一个字符的地址
		FilePointerAddress += 16;


		if (r > 8)
		{
			for (p = Save; p < Save + 8; p += 1)//十六进制格式输出
				printf("%02X ", *p);
			putchar(' ');//分隔符
			for (p = Save + 8; p < SaveAddp; p += 1)//十六进制格式输出
				printf("%02X ", *p);
		}
		else
		{
			for (p = Save; p < SaveAddp; p += 1)//十六进制格式输出
				printf("%02X ", *p);
			putchar(' ');//分隔符
		}

		for (int m = 0; m < 16 - r; m += 1)//对齐输出
			printf("   ");
		putchar(' ');//分隔符

		for (p = Save; p < SaveAddp; p += 1)//字符格式输出
			printf("%c", (isprint(*p)) ? *p : '.');

		putchar('\n');
	}


	fclose(ReadFile);
	return 0;
}
