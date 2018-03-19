#include <stdio.h>
#include <windows.h>
#include <time.h>

float file_copy_c(char *file_source, char *file_dest)
{
	char c[4096];
	FILE *file_W, *file_R;
	clock_t start = clock();

	fopen_s(&file_R, file_source, "r");
	fopen_s(&file_W, file_dest, "w");

	while (!feof(file_R)) {
		size_t bytes = fread(c, 1, sizeof(c), file_R);
		if (bytes) {
			fwrite(c, 1, bytes, file_W);
		}
	}

	fclose(file_R);
	fclose(file_W);
	clock_t end = clock();
	return (float)(end - start) / CLK_TCK;
}

float file_copy_win(char *file_source, char *file_dest)
{
	clock_t start = clock();
	CopyFile(file_source, file_dest, 0);
	clock_t end = clock();
	return (float)(end - start) / CLK_TCK;
}

int main()
{
	char sourse16[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\input16.txt";
	char sourse64[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\input64.txt";
	char sourse256[52] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\input256.txt";
	char sourse1024[53] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\input1024.txt";
	char out16_c[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out16_c.txt";
	char out16_win[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out16_wim.txt";
	char out64_c[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out64_c.txt";
	char out64_win[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out64_win.txt";
	char out256_c[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out256_c.txt";
	char out256_win[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out256_win.txt";
	char out1024_c[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out1024_c.txt";
	char out1024_win[51] = "C:\\Users\\Þëÿ\\Documents\\GitHub\\SPL\\out1024_win.txt";
	printf("%c", 218);
	for (int i = 0; i < 4; ++i)
	{
		for (int i = 0; i < 9; ++i)
		{
			printf("%c", 196);
		}
		printf("%c", 194);
	}
	for (int i = 0; i < 9; ++i)
	{
		printf("%c", 196);
	}
	printf("%c\n", 191);
	printf("%c  Tool   %c   16    %c   64    %c   256   %c  1024   %c\n", 179, 179, 179, 179, 179, 179);
	printf("%c", 195);
	for (int i = 0; i < 4; ++i)
	{
		for (int i = 0; i < 9; ++i)
		{
			printf("%c", 196);
		}
		printf("%c", 197);
	}
	for (int i = 0; i < 9; ++i)
	{
		printf("%c", 196);
	}
	printf("%c\n", 180);
	printf("%c    C    %c%8.4f %c%8.4f %c%8.4f %c%8.4f %c\n", 179, 179, file_copy_c(sourse16, out16_c), 179, file_copy_c(sourse64, out64_c), 179, file_copy_c(sourse256, out256_c), 179, file_copy_c(sourse1024, out1024_c), 179);
	printf("%cCopyFile %c%8.4f %c%8.4f %c%8.4f %c%8.4f %c\n", 179, 179, file_copy_win(sourse16, out16_win), 179, file_copy_win(sourse64, out64_win), 179, file_copy_win(sourse256, out256_win), 179, file_copy_win(sourse1024, out1024_win), 179);
	printf("%c", 192);
	for (int i = 0; i < 4; ++i)
	{
		for (int i = 0; i < 9; ++i)
		{
			printf("%c", 196);
		}
		printf("%c", 193);
	}
	for (int i = 0; i < 9; ++i)
	{
		printf("%c", 196);
	}
	printf("%c\n", 217);
	system("pause");
}
