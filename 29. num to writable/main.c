#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <locale.h>

int try_strcat(char *str, char *buf, size_t bufsize, int *bufp, char *space_status){
	if (*space_status){
		*space_status = 0;
	}
	else{
		*space_status = 1;
		try_strcat(" ", buf, bufsize, bufp, space_status);
	}
	if (strlen(str) < bufsize - *bufp){
		strcat(buf, str);
		*bufp += strlen(str);
		return 1;
	}
	return 0;
}

int number_to_text(unsigned long long num, char *buf, size_t bufsize)
{
	char *name1f[] = { "одна", "две", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять", "десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятьнадцать" };
	char *name1m[] = { "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять", "десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятьнадцать" };
	char *name10[] = { "десять", "двадцать", "тридцать", "сорок", "пятьдесят", "шестьдесят", "семьдесят", "восемьдесят", "девяносто" };
	char *name100[] = { "сто", "двести", "триста", "четыреста", "пятьсот", "шестьсот", "семьсот", "восемьсот", "девятьсот" };
	char *bigname1[] = { "тысяча", "миллион", "миллиард", "триллион", "квадриллион", "квинтиллион" };
	char *bigname2[] = { "тысячи", "миллиона", "миллиарда", "триллиона", "квадриллиона", "квинтиллиона" };
	char *bigname3[] = { "тысяч", "миллионов", "миллиардов", "триллионов", "квадриллионов", "квинтиллионов" };
	char *zero = "ноль";
	int bufp = 0;

	char firstuse = 1;
	if (num == 0){
		if (!try_strcat(zero, buf, bufsize, &bufp, &firstuse))
			return 1;
		return 0;
	}


	while (num != 0){
		unsigned long long t = num;
		int pw = 0;
		while (t != t % 1000){
			t /= 1000;
			pw++;
		}
		num = num - t*((unsigned long long) pow(10, pw * 3));
		char **name1 = (pw == 1) ? name1f : name1m;
		int a = (t / 100) % 10;
		int b = (t / 10) % 10;
		int c = t % 10;
		if (a != 0){
			if (!try_strcat(name100[a - 1], buf, bufsize, &bufp, &firstuse))
				return 1;
		}
		if ((b != 0) && (b != 1)){
			if (!try_strcat(name10[b - 1], buf, bufsize, &bufp, &firstuse))
				return 1;
		}
		if (b == 1){
			c += 10;
		}
		if (c != 0){
			if (!try_strcat(name1[c - 1], buf, bufsize, &bufp, &firstuse))
				return 1;
		}

		if (pw != 0){
			char **bigname;
			switch (c){
				case 1:
					bigname = bigname1;
					break;
				case 2:
				case 3:
				case 4:
					bigname = bigname2;
					break;
				default:
					bigname = bigname3;
					break;
			}
			if (!try_strcat(bigname[pw - 1], buf, bufsize, &bufp, &firstuse))
				return 1;
		}
	}
	return 0;
}

void test1(int n) {
	char *buf = (char *)calloc(100500, sizeof(char));
	memset(buf, 0, 100500);
	assert(number_to_text(n, buf, 100500) == 0);
	printf("%d = %s\n", n, buf);
	free(buf);
}

void test(){
	char *buf = (char *)calloc(100500, sizeof(char));
	assert(number_to_text(0, buf, 100500) == 0);
	assert(strcmp(buf, "ноль") == 0);

	memset(buf, 0, 100500);
	assert(number_to_text(934, buf, 100500) == 0);
	assert(strcmp(buf, "девятьсот тридцать четыре") == 0);

	memset(buf, 0, 100500);
	assert(number_to_text(1009, buf, 100500) == 0);
	assert(strcmp(buf, "одна тысяча девять") == 0);

	memset(buf, 0, 100500);
	assert(number_to_text(17301, buf, 100500) == 0);
	assert(strcmp(buf, "семнадцать тысяч триста один") == 0);

	memset(buf, 0, 100500);
	assert(number_to_text(18446744073709551615UL, buf, 100500) == 0);
	assert(strcmp(buf, "восемнадцать квинтиллионов четыреста сорок шесть квадриллионов семьсот сорок четыре триллиона семьдесят три миллиарда семьсот девять миллионов пятьсот пятьдесят одна тысяча шестьсот пятнадцать") == 0);

	memset(buf, 0, 100500);
	assert(number_to_text(18446744073709551615UL, buf, 100) == 1);
	//printf("%s\n", buf);

	free(buf);
}

int main(){
	setlocale(LC_ALL, "Russian");
	test();

	test1(1);
	test1(10);
	test1(11);
	test1(19);
	test1(20);
	test1(21);
	test1(100);
	test1(111);
	test1(199);
	test1(999);
	test1(1011);

	test1(210000001);
	test1(22000);
	test1(25000);

	return 0;
}