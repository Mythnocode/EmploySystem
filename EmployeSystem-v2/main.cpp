#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>


struct Person {
	char pid[20];
	char name[50];
	char sex[8];
	char type[50];
	char from[50];
	float salary;
};
typedef struct PersonNode {
	Person data;
	struct PersonNode* next;
}PersonNode;
typedef enum {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_RED = 4,
	COLOR_YELLOW = 6,
	COLOR_WHITE = 7,
	COLOR_BRIGHT = 8,
	COLOR_ORANGE = 14,
	COLOR_CYAN = 3,
	COLOR_PURPLE = 5,
	COLOR_DARKGRAY = 8,        // 深灰色
	COLOR_LIGHTYELLOW = 14,    // 浅金色
	COLOR_DARKBLUE = 1 | 8,    // 深蓝色
	COLOR_LIGHTBLUE = 1,       // 浅蓝色
	COLOR_DARKGREEN = 2 | 8,   // 深绿色
	COLOR_LIGHTGREEN = 2,      // 浅橄榄绿
	COLOR_LIGHTCYAN = 3 | 8,   // 淡青色
	COLOR_DARKPURPLE = 5 | 8,  // 深紫色
	COLOR_LIGHTPURPLE = 5,     // 淡紫色
	COLOR_DARKBROWN = 6 | 8,   // 深棕色
	COLOR_BEIGE = 7,           // 米色
	COLOR_LIGHTBEIGE = 7 | 8,  // 浅米色
	COLOR_DARKRED = 4 | 8   ,  // 深酒红
	COLOR_LIGHT_PINK = 5 | 8,  // 浅粉色
	COLOR_PINK = 13,
}ConsoleColor;


PersonNode* person_head = NULL;
int backup_number = 1;
void center(const char* arr);
void center_color(const char* output, int text_color, int bg_color);
void outputspace(int n) {
	for (int i = 0; i < n; i++) {
		printf(" ");
	}
}
int getwidth(const char* str) {
	int width = 0;
	while (*str != '\0') {
		if ((unsigned char)*str >= 0xE0) {
			width += 2;
			str += 3;
		}
		else if ((unsigned char)*str >= 0x80) {
			width += 2;
			str += 2;
		}
		else {
			width += 1;
			str += 1;
		}
	}
	return width;
}
int deletelast(char* str) {
	int len = strlen(str);
	if (len == 0)return 0;
	int i = len - 1;
	if ((unsigned char)str[i] >= 0x80 && (unsigned char)str[i] <= 0xBF) {
		i--;
		if ((unsigned char)str[i] >= 0x80 && (unsigned char)str[i] <= 0xBF) {
			i--;
		}
		else {
			i--;
		}
	}
	str[i >= 0 ? i : 0] = '\0';
	return 1;
}
void centerinputchinese(char input[]) {
	char ch;
	char temp_buf[4] = { 0 };
	int temp_idx = 0;
	int i = 0;
	printf("\r");
	outputspace(60);
	fflush(stdout);
	while (1) {
		ch = _getch();
		if (ch == '\r') {
			input[i] = '\0';
			printf("\n");
			break;
		}
		if (ch == '\b') {
			if (i > 0) {
				deletelast(input);
				i = strlen(input);
				printf("\r");
				int dw = getwidth(input);
				outputspace((120 - dw) / 2);
				printf("%s", input);
				printf("\033[K");
				fflush(stdout);
			}
			continue;
		}
		if ((unsigned char)ch >= 0x80) {
			temp_buf[temp_idx++] = ch;
			if ((temp_idx == 3 && (unsigned char)temp_buf[0] >= 0xE0) || (temp_idx == 2 && (unsigned char)temp_buf[0] >= 0x80 && (unsigned char)temp_buf[0] < 0xE0)) {
				strcat(input, temp_buf);
				i += temp_idx;
				temp_idx = 0;
				memset(temp_buf, 0, sizeof(temp_buf));
			}
		}
		else {
			if (isprint(ch)) {
				input[i++] = ch;
				input[i] = '\0';
			}
		}
		if (temp_idx == 0) {
			printf("\r");
			int dw = getwidth(input);
			outputspace((120 - dw) / 2);
			printf("%s", input);
			printf("\033[K");
			fflush(stdout);
		}
	}
}
void centerinput(char password[]) {
	char ch;
	int i = 0;
	int n = 0;
	memset(password,0,sizeof(password));
	while ((ch = _getch()) != '\r') {
		printf("\r");
		outputspace((120 - i) / 2);
		printf("%s", password);
		fflush(stdout);
		if (ch == '\b' && i > 0) {
			printf("\r");
			password[--i] = '\0';
			outputspace((120 - i) / 2);
			printf("%s", password);
			printf("\033[K");//清除光标到行尾的所有内容
			fflush(stdout);
			continue;
		}
		else if (ch >= 32 && ch <= 126) {
			password[i++] = ch;
			printf("%c", ch);
			fflush(stdout);
		}
	}
	password[i] = '\0';
	printf("\n");
}
void setcolor(int text_color, int bg_color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attrs=(WORD)((text_color & 0x07) | ((bg_color & 0x07) << 4) |
		((text_color & COLOR_BRIGHT) ? FOREGROUND_INTENSITY : 0) |
		((bg_color & COLOR_BRIGHT) ? BACKGROUND_INTENSITY : 0));
	SetConsoleTextAttribute(hConsole, attrs);
}
void reset_color() {
	setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_BLACK);
}
void Hash(char *inputkeys,char*hashresult) {//单向加密
	unsigned int hash = 0x12345678;
	int len = strlen(inputkeys);
	for (int i=0; i < len; i++) {
		hash ^= (unsigned int)inputkeys[i];
		hash = (hash << 5) | (hash >> (32 - 5));
		hash += (unsigned int)inputkeys[i] * (i + 1);
		hash %= 0xFFFF;
	}
	sprintf(hashresult, "%04X", hash);
}
int isPersonInList(char* pid) {
	PersonNode* p = person_head;
	while (p != NULL) {
		if (strcmp(p->data.pid, pid) == 0||strcmp(p->data.name,pid)==0) {
			return 1;
		}
		p = p->next;
	}
	return 0;
}
void readPersonData() {
	FILE* fp = fopen("data.txt", "r");
	if (!fp) {
		center("教职工数据文件不存在，将为您新建一个\n");
		return;
	}
	Person temp = { 0 };
	fseek(fp, 0, SEEK_END);
	if (ftell(fp) == 0) { 
		center("没有信息！\n");
		fclose(fp);
		return;
	}
	rewind(fp);
	while (fscanf(fp, "%s %s %s %s %s %f",
		temp.pid, temp.name, temp.sex, temp.type, temp.from, &temp.salary
	) == 6) {

		if (!isPersonInList(temp.pid)) {
			PersonNode* newnode = (PersonNode*)malloc(sizeof(PersonNode));
			newnode->data = temp;
			newnode->next = person_head;
			person_head = newnode;
		}
	}
	fclose(fp);
	//printf("已读取教职工数据\n");
}
void insertAPersonData() {
	char temp_str[200];  
	FILE* fp = fopen("data.txt", "a");
	if (fp == NULL) {  
		center("文件打开失败！\n");
		system("pause");
		return;
	}
	int isin = 1; 
	int iswrong = 0;
	while (1) {
		int idx = 1;
		while (1) {
			setcolor(COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			printf("请输入：工号        姓名        性别        职称        部门               薪资");
			reset_color();
			printf("\n");
			if (fgets(temp_str, sizeof(temp_str), stdin) == NULL) {
				printf("输入读取失败！\n");
				continue;
			}
			char* newline = strchr(temp_str, '\n');
			if (newline != NULL) {
				*newline = '\0';
			}
			char* token = strtok(temp_str, " ");
			if (token == NULL) {
				continue;
			}
			if (strcmp(token, "exit") == 0 || strcmp(token, "0") == 0) {
				fclose(fp);
				return;
			}
			iswrong = 0;
			idx = 1;
			//判断是否合法
			while (token != NULL && !iswrong) {
				iswrong = 0;
				switch (idx) {
				case 1:
					if (isPersonInList(token)) {
						printf("已有该教职工的信息！\n");
						iswrong = 1;
					}
					else if (strlen(token) != 4 || token[0] != 'C' ||
						!isdigit(token[1]) || !isdigit(token[2]) || !isdigit(token[3])) {
						printf("工号输入错误！格式为\"CXXX\"（X为数字）\n");
						iswrong = 1;
					}
					if (!iswrong&&isin==1) {
						fprintf(fp, "%s", token);
						isin++;
					}
					break;

				case 2:
					for (int i = 0; token[i] != '\0'; i++) {
						if (isdigit((unsigned char)token[i])) {
							printf("姓名中不能包含数字！\n");
							iswrong = 1;
							break;
						}
					}
					if (!iswrong&&isin==2) {
						fprintf(fp, " %s", token);
						isin++;
					}
					break;

				case 3:
					if (strcmp(token, "男") != 0 && strcmp(token, "女") != 0) {
						printf("性别只能是男或女！\n");
						iswrong = 1;
					}
					if (!iswrong&&isin==3) {
						fprintf(fp, " %s", token);
					}
					break;

				case 4:
					fprintf(fp, " %s", token);
					break;

				case 5:
					fprintf(fp, " %s", token);
					break;

				case 6:
					fprintf(fp, " %s", token);
					break;

				default:
					center("输入字段过多！\n");
					iswrong = 1;
					break;
				}
				if (!iswrong) {
					idx++;
					token = strtok(NULL, " ");
				}
			}
			if (!iswrong && idx >= 7) {
				fprintf(fp, "\n");
				center_color("信息录入成功", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
				break;
			}
			else if (!iswrong) {
				printf("输入字段不完整！需包含：工号、姓名、性别、职称、部门、薪资\n");
			}
		}
	}
	fclose(fp);
	system("pause");
}
void insertAllPersonData() {
	char line[200];
	int index = 0;
	center_color("请输入需要的数据(exit或ok为结束)", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
	FILE* fp = fopen("data.txt", "a");
	if (fp == NULL) {
		center("文件打开失败\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	while (1) {
		memset(line, 0, sizeof(line));
		fgets(line, sizeof(line), stdin);
		if (strcmp(line, "exit\n") == 0||strcmp(line,"ok\n")==0) {
			break;
		}
		fprintf(fp, "%s", line);
		index++;
	}
	fclose(fp);
	center_color("写入成功！", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
	center("按任意键继续...\n");
	getchar();
}
void insertPersonData() {//用户写入教职工信息
	FILE* fp = fopen("data.txt", "a");
	char pid[20] = {0};
	char name[50] = {0};
	char sex[8] = {0};
	char type[50] = {0};
	char from[50] = {0};
	float salary=0.0;
	char select_mode[10] = { 0 };
	if (!fp) {
		center("数据文件打开失败，无法写入！\n");
		return;
	}
	//do {
		do {
			system("cls");
			center_color("************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			center_color("*教职工信息管理系统->教职工信息录入*", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			center_color("*----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			center_color("*          请选择录入模式          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("*           1.单独录入             *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("*           2.整体录入             *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("*           3.批量录入             *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			centerinput(select_mode);
			switch (select_mode[0]) {
			case '1':
					center_color("请输入工号：（exit为退出）", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
					centerinputchinese(pid);
					if (strcmp(pid, "exit")==0) {
						return;
					}
					else if (strcmp(pid,"0")==0) {
						return;
					}
					else if(isPersonInList(pid)) {
						center("已有该教职工的信息!\n");
						system("pause");
						continue;
					}
					else if (strcmp(pid, "C000") <= 0 || strcmp(pid, "C999") > 0) {
						center_color("输入错误请重新输入,格式为\"CXXX\"", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
						system("pause");
						continue;
					}
					else {
						do {
							center_color("请输入姓名：",COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
							centerinputchinese(name);
							if (strcmp(pid, "exit") == 0) {
								return;
							}
							int hasdigit = 0;
							for (int i = 0; name[i] != '\0'; i++) {
								if (isdigit((unsigned char)name[i])) {
									center("\n姓名中不能包含数字\n");
									hasdigit = 1;
									break;
								}
							}
							if (!hasdigit) {
								break;
							}
						} while (1);
						do {
							center_color("请输入性别：", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
							centerinputchinese(sex);
							if (strcmp(sex, "exit") == 0) {
								return;
							}
						} while (strcmp(sex, "男") != 0 && strcmp(sex, "女") != 0);
						center_color("请输入职称：", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
						centerinputchinese(type);
						if (strcmp(type, "exit") == 0) {
							return;
						}
						center_color("请输入部门：", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
						centerinputchinese(from);
						if (strcmp(type, "exit") == 0) {
							return;
						}
						center_color("请输入薪资：", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
						char csalary[30] = { 0 };
						centerinput(csalary);
						salary = atof(csalary);
						fprintf(fp, "\n%s %s %s %s %s %.1f", pid, name, sex, type, from, salary);
						center_color("录入完毕！", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
						fclose(fp);
						readPersonData();
						system("pause");
						break;
					}
						break;
			case '2':
				insertAPersonData();
				break;
			case '3':
				insertAllPersonData();
				break;
			default:return;
			}
		} while (1);
	system("pause");
}
void showAllPersonData() {//查看所有教职工的各项信息
	if (person_head == NULL) {
		center("暂无数据!\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	PersonNode* temp = person_head;
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	while (temp) {
		printf("                      ");
		setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
		printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
		temp = temp->next;
		reset_color();
		printf("\n");
	}
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
}
void showPersonDataByPID() {
	if (person_head == NULL) {
		center("暂无数据!\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	PersonNode* temp = person_head;
	center_color("请输入工号:", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	char pid[20];
	centerinputchinese(pid);
	//scanf("%s",pid);
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	while (temp) {
		if (strcmp(temp->data.pid, pid) == 0) {
			printf("                      ");
			setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
			reset_color();
			printf("\n");
		}
		temp = temp->next;

	}
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
}
void showPersonDataByFrom() {
	if (person_head == NULL) {
		center("暂无数据!\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	PersonNode* temp = person_head;
	center_color("请输入部门名称:", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	char from[20] = {0};
	centerinputchinese(from);
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	while (temp) {
		if (strcmp(temp->data.from, from) == 0) {
			printf("                      ");
			setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
			reset_color();
			printf("\n");
		}
		temp = temp->next;
	}
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
}
void showPersonDataByType() {
	if (person_head == NULL) {
		center("暂无数据!\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	PersonNode* temp = person_head;
	center_color("请输入职称名称:", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	char type[20] = {0};
	centerinputchinese(type);
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	while (temp) {
		if (strcmp(temp->data.type, type) == 0) {
			printf("                      ");
			setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
			reset_color();
			printf("\n");
		}
		temp = temp->next;
	}
	center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
}
void savePersonData() {
	FILE* fp = fopen("data.txt", "w");
	if (!fp) {
		center("文件打开失败，无法保存！\n");
	}
	PersonNode* temp = person_head;
	while (temp) {
		fprintf(fp, "%s %s %s %s %s %.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
		if (temp->next != NULL) {
			fprintf(fp,"\n");
		}
		temp = temp->next;
	}
	fclose(fp);
}
void deletePersonDataByPID() {
	if (person_head == NULL) {
		center("暂无数据!\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	PersonNode* pre = person_head;
	PersonNode* p = pre->next;
	center_color("请输入工号或名称:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
	char pid[20];
	char lines[50];
	int ishead = 0;
	centerinputchinese(pid);
	if (!isPersonInList(pid)) {
		strcpy(lines, "未找到");
		strcat(lines, pid);
		strcat(lines, "的信息！\n");
		center(lines);
		system("pause");
		return;
	}
	while (p!=NULL) {
		if (strcmp(pre->data.pid, pid) == 0 || strcmp(pre->data.name, pid) == 0) {
			person_head= p;
			free(pre);
			break;

		}
		else if (strcmp(p->data.pid, pid) == 0 || strcmp(p->data.name, pid)==0) {
			pre->next = p->next;
			free(p);
			break;
		}
		pre = pre->next;
		p = p->next;
	}
	savePersonData();
	strcpy(lines, "已删除");
	strcat(lines, pid);
	strcat(lines, "的信息");
	center_color(lines, COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
	system("pause");
}
void changePersonData() {
	if (person_head == NULL) {
		center("暂无数据!\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	PersonNode* pre = person_head;
	center_color("请输入工号或名称:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
	char pid[20];
	centerinputchinese(pid);
	//scanf("%s", pid);
	if (!isPersonInList(pid)) {
		printf("未找到%s的信息！\n", pid);
		center("按任意键继续");
		getchar();
		return;
	}
	while (pre) {
		if (strcmp(pre->data.pid, pid) == 0 || strcmp(pre->data.name, pid) == 0) {
			break;
		}
		pre = pre->next;
	}
	center_color("====================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("工号       姓名       性别       职称       部门            薪资", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	printf("                            ");
	setcolor(COLOR_WHITE | COLOR_BRIGHT,COLOR_DARKBLUE);
	printf("%-11s%-12s%-10s%-10s%-16s%-8.1f", pre->data.pid, pre->data.name, pre->data.sex, pre->data.type, pre->data.from, pre->data.salary);
	reset_color();
	printf("\n");
	center_color("====================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	char value[20] = {-1};
	memset(value,0,sizeof(value));
	while (1) {
		center_color("请输入需要更改的信息:（exit为退出）", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
		centerinputchinese(value);
		//scanf("%s", value);
		char keys[20] = {0};
		memset(keys, 0, sizeof(keys));
		if (strcmp(value, "工号") == 0) {
			center_color("请输入修改后的信息:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			centerinputchinese(keys);
			//scanf("%s", keys);
			strcpy(pre->data.pid, keys);
			center_color("已修改完成!", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			break;
		}
		else if (strcmp(value, "姓名") == 0) {
			center_color("请输入修改后的信息:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			centerinputchinese(keys);
			//scanf("%s", keys);
			strcpy(pre->data.name, keys);
			center_color("已修改完成!", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			break;
		}
		else if (strcmp(value, "性别") == 0) {
			center_color("请输入修改后的信息:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			centerinputchinese(keys);
			//scanf("%s", keys);
			strcpy(pre->data.sex, keys);
			center_color("已修改完成!", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			break;
		}
		else if (strcmp(value, "职称") == 0) {
			center_color("请输入修改后的信息:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			centerinputchinese(keys);
			//scanf("%s", keys);
			strcpy(pre->data.type, keys);
			center_color("已修改完成!", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			break;
		}
		else if (strcmp(value, "部门") == 0) {
			center_color("请输入修改后的信息:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			centerinputchinese(keys);
			scanf("%s", keys);
			center_color("已修改完成!", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			break;
		}
		else if (strcmp(value, "薪资") == 0) {
			center_color("请输入修改后的信息:", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			float salary;
			char csalary[20] = { 0 };
			centerinputchinese(csalary);
			salary = atof(csalary);
			pre->data.salary = salary;
			center_color("已修改完成!", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			break;
		}
		else if (strcmp(value, "exit") == 0) {
			break;
		}
		else {
			center("输入有误，请重新输入\n");
			continue;
		}
	
	}
	savePersonData();
	center("按任意键继续...");
	getchar();
}
void sortPersonByPID() {
	if (person_head== NULL || person_head->next == NULL)return;
	int swapped;
	PersonNode* p;
	PersonNode* tail = NULL;
	do {
		swapped = 0;
		p = person_head;
		while (p->next != tail) {
			if (strcmp(p->data.pid, p->next->data.pid) > 0) {
				Person temp = p->data;
				p->data = p->next->data;
				p->next->data = temp;
				swapped = 1;
			}
			p = p->next;
		}
		tail = p;
	} while (swapped);
	savePersonData();
}
void sortPersonBySalary() {
	if (person_head == NULL || person_head->next == NULL)return;
	int swapped;
	PersonNode* p;
	PersonNode* tail = NULL;
	do {
		swapped = 0;
		p = person_head;
		while (p->next != tail) {
			if (p->data.salary> p->next->data.salary) {
				Person temp = p->data;
				p->data = p->next->data;
				p->next->data = temp;
				swapped = 1;
			}
			p = p->next;
		}
		tail = p;
	} while (swapped);
	savePersonData();
}
void getBackupNumber() {
	FILE* fp = fopen("backup.txt", "r");
	if (fp == NULL) {
		printf("打开失败");
		return;
	}
	int ch;
	int is_new_line = 1;
	char num_buf[10] = { 0 };
	int num_idx = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (is_new_line) {
			if (ch == 'C') {
				while ((ch = fgetc(fp)) != EOF && ch != '\n');
			}
			else if (isdigit(ch)) {
				num_idx = 0;
				num_buf[num_idx++] = ch;
				while ((ch = fgetc(fp)) != EOF && ch != '\n' && isdigit(ch)) {
				
					num_buf[num_idx++] = ch;
				}
				num_buf[num_idx] = '\0';
			
				backup_number = atoi(num_buf) + 1;
				while (ch != EOF && ch != '\n') {
					ch = fgetc(fp);
				}
			}
			else {
				while ((ch = fgetc(fp)) != EOF && ch != '\n');
			}
			is_new_line = 0;
		}
		
		if (ch == '\n') {
			is_new_line = 1;
		}
	}
	fclose(fp);
}
void readBackupIDandTime() {
	FILE* fp = fopen("backup.txt","r");
	if (fp == NULL) {
		center("打开失败");
		return;
	}
	char backup_data[70];
	while (fgets(backup_data, sizeof(backup_data), fp)!=NULL) {
		if (backup_data[0] == 'C') {
			continue;
		}
		else
		{
			backup_data[strlen(backup_data)-1] = '\0';
			center_color(backup_data, COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		}
	}
	fclose(fp);
}
void backupPersonData() {
	FILE* fp = fopen("backup.txt", "a");
	time_t now_time = time(NULL);
	struct tm* local_time = localtime(&now_time);
	char time_str[100];
	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);
	fprintf(fp, "%d %s\n", backup_number,time_str);
	backup_number++;
	fclose(fp);
	FILE* fp1 = fopen("backup.txt", "a");
	PersonNode* temp = person_head;
	while (temp) {
		fprintf(fp1, "%s %s %s %s %s %.1f\n", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
		temp = temp->next;
	}
	char str[20] = {0};
	sprintf(str, "%d", backup_number-1);
	char lines[30];
	strcpy(lines, "备份完毕！备份编号为");
	strcat(lines, str);
	strcat(lines, "\n");
	center(lines);
	fclose(fp1);
	system("pause");
}
void showBackupById(int target_id) {
	FILE* fp = fopen("backup.txt", "r");
	if (fp == NULL) {
		printf("打开backup.txt失败\n");
		return;
	}
	PersonNode* temp_node = person_head;
	while (temp_node != NULL) {
		PersonNode* next_node = temp_node->next;
		free(temp_node);
		temp_node = next_node;

	}
	person_head = NULL;
	char line[100];
	int current_id = 0;
	int is_target_found = 0; 
	while (fgets(line, sizeof(line), fp) != NULL) {
		if (isdigit(line[0])) {
			sscanf(line, "%d", &current_id);
			if (current_id == target_id) {
				is_target_found = 1;
				break;
			}
		}
	}
	if (is_target_found) {
		Person temp = { 0 };
		center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		while (1) {
			if (fgets(line, sizeof(line), fp) == NULL)break; 
			if (line[0] != 'C') {
				break;
			}
			sscanf(line, "%s %s %s %s %s %f", temp.pid, temp.name, temp.sex, temp.type, temp.from, &temp.salary);
			printf("                      ");
			setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp.pid, temp.name, temp.sex, temp.type, temp.from, temp.salary);
			reset_color();
			printf("\n");
			if (!isPersonInList(temp.pid)) {
				PersonNode* newnode = (PersonNode*)malloc(sizeof(PersonNode));
				newnode->data = temp;
				newnode->next = person_head;
				person_head = newnode;
			}
		}
		center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	}
	else if(target_id){
		char str[30] = {0};
		memset(line,0,sizeof(line));
		sprintf(str, "%d", target_id);
		strcpy(line, "未找到编号为");
		strcat(line, str);
		strcat(line, "的备份\n");
		center(line);
	}
	fclose(fp);
}
void showPersonDataByOdd() {
	if (person_head == NULL) {
		center("暂无数据!\n");
		center("按任意键继续...\n");
		getchar();
		return;
	}
	char keys[20];
	char sex[50];
	float start=0.0;
	float end=20000.0;
	int count_salary = 0;
	char str[20] = {0};
	char line[50];
	char csalarys[20];
	char csalarye[20];
	center_color("===========================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("=            请选择查询的方法：           =", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
	center_color("=            1.按 薪 资 区 间             =", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
	center_color("=            2.统          计             =", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
	center_color("===========================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	centerinputchinese(keys);
	switch (keys[0]) {
	case '1': {
		do {
			count_salary = 0;
	center_color("===========================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("=             请输入开始区间:             =", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
	centerinput(csalarys);
	start = atof(csalarys);
	center_color("=             请输入结束区间:             =", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
	centerinput(csalarye);
	end = atof(csalarye);
			if (start > end) {
				center("输入区间错误\n");
				system("pause");
				continue;
			}
			else {
				PersonNode* temp = person_head;
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				sortPersonByPID();
				sortPersonBySalary();
				while (temp!=NULL) {
					if (temp->data.salary >= start && temp->data.salary <= end) {
						printf("                      ");
						setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
						printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
						reset_color();
						printf("\n");
						count_salary++;
					}
					temp = temp->next;
				}
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				sprintf(str, "%.1f", start);
				strcpy(line, "薪资在区间[");
				strcat(line, str);
				strcat(line, ",");
				memset(str, 0, sizeof(str));
				sprintf(str, "%.1f", end);
				strcat(line, str);
				strcat(line, "]的共有");
				memset(str, 0, sizeof(str));
				sprintf(str, "%d", count_salary);
				strcat(line, str);
				strcat(line, "个人");
				center_color(line, COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				system("pause");
				system("cls");
				break;
			}

		} while (1);
		break;
		}
	case '2': {
		center_color("=======================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("==请输入统计指标:(指标名称或快捷数字)==", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
		center_color("==              1.性 别              ==", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
		center_color("==              2.职 称              ==", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
		center_color("==              3.部 门              ==", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
		center_color("=======================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		memset(keys, 0, sizeof(keys));
		centerinput(keys);
		if (strcmp(keys, "1") == 0) {
			center_color("请输入性别：", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			do
			{
				count_salary = 0;
				centerinputchinese(sex);
				if (strcmp(sex, "男") != 0 && strcmp(sex, "女") != 0) {
					center("没有该性别！\n");
					continue;
				}
				sortPersonByPID();
				sortPersonBySalary();
				PersonNode* temp = person_head;
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				while (temp != NULL) {
					if (strcmp(temp->data.sex, sex) == 0) {
						printf("                      ");
						setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
						printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
						count_salary++;
						reset_color();
						printf("\n");
					}
					temp = temp->next;
				}
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				memset(str, 0, sizeof(str));
				memset(line, 0, sizeof(line));
				sprintf(str, "%d", count_salary);
				strcpy(line, "性别为");
				strcat(line, sex);
				strcat(line, "的人共有");
				strcat(line, str);
				strcat(line, "人");
				center_color(line, COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				system("pause");
				break;
			} while (1);
		}
		else if (strcmp(keys, "2") == 0) {
			center_color("请输入职称：", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
			do
			{
				count_salary = 0;
				memset(sex, 0, sizeof(sex));
				centerinputchinese(sex);
				sortPersonByPID();
				sortPersonBySalary();
				PersonNode* temp = person_head;
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				while (temp != NULL) {
					if (strcmp(temp->data.type, sex) == 0) {
						printf("                      ");
						setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
						printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
						count_salary++;
						reset_color();
						printf("\n");
					}
					temp = temp->next;
				}
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				memset(str, 0, sizeof(str));
				memset(line, 0, sizeof(line));
				sprintf(str, "%d", count_salary);
				strcpy(line, "职称为");
				strcat(line, sex);
				strcat(line, "的人共有");
				strcat(line, str);
				strcat(line, "人");
				center_color(line, COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				system("pause");
				break;
			} while (1);
		}
		else if (strcmp(keys, "3") == 0) {
			center_color("请输入部门：", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			do
			{
				count_salary = 0;
				memset(sex, 0, sizeof(sex));
				centerinputchinese(sex);
				sortPersonByPID();
				sortPersonBySalary();
				PersonNode* temp = person_head;
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				center_color("工号       姓名       性别       职称           部门                薪资    ", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				while (temp != NULL) {
					if (strcmp(temp->data.from, sex) == 0) {
						printf("                      ");
						setcolor(COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
						printf("%-11s%-12s%-10s%-10s%-25s%-8.1f", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
						count_salary++;
						reset_color();
						printf("\n");
					}
					temp = temp->next;
				}
				center_color("============================================================================", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				memset(str, 0, sizeof(str));
				memset(line, 0, sizeof(line));
				sprintf(str, "%d", count_salary);
				strcpy(line, "职称为");
				strcat(line, sex);
				strcat(line, "的人共有");
				strcat(line, str);
				strcat(line, "人");
				center_color(line, COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				system("pause");
				break;
			} while (1);
		}
		else {
			break;
		}
		break;
	}
	}
}
void center(const char* output) {
	int total = 120 - strlen(output) + 1;
	int number = total / 2;
	for (int i = 0; i < number; i++) {
		printf(" ");
	}
	printf("%s", output);
}
void center_color(const char* output,int text_color,int bg_color) {
	reset_color();
	int total = 120 - strlen(output) + 1;
	int number = total / 2;
	for (int i = 0; i < number; i++) {
		printf(" ");
	}
	setcolor(text_color, bg_color);
	printf("%s", output);
	reset_color();
	for (int i = 0; i < number-1; i++) {
		printf(" ");
	}
	printf("\n");
}
void menu1() {
	char key[8] = {-1};
	do {
		system("cls");
		center_color("**********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("*       教职工信息管理系统       *", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
		center_color("*--------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("*       您的身份是:管理员        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
		center_color("*--------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("*        1.教职工信息录入        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("*        2.管理教职工信息        *",COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("*        3.查  询  信  息        *",COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("*        4.数据备份与恢复        *",COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("*        0.退  出  系  统        *",COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("**********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		centerinput(key);
		char key1[8] = {-1};
		char lines[50] = { 0 };
		if (strcmp(key, "1") == 0) {
			system("cls");
			readPersonData();
			insertPersonData();
		}
		else if (strcmp(key, "2") == 0) {
			do {
				system("cls");
				center_color("************************************", COLOR_WHITE | COLOR_BRIGHT,COLOR_DARKGRAY);
				center_color("*教职工信息管理系统->管理教职工信息*", COLOR_DARKGRAY | COLOR_BRIGHT, COLOR_DARKBROWN);
				center_color("*----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				center_color("*      1.删除某个教职工的信息      *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*      2.更改某个教职工的信息      *",COLOR_WHITE|COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*      0.退    出    系    统      *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				char key2[8] = {-1};
				centerinput(key2);
				if (strcmp(key2, "1")==0) {
					readPersonData(); deletePersonDataByPID();
				}
				else if (strcmp(key2, "2")==0) {
					readPersonData(); changePersonData();
				}
				else {
					break;
				}
			} while (1);
		}
		else if (strcmp(key, "3")==0) {
			system("cls");
			do {
				center_color("******************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_BEIGE);
				center_color("*教职工信息管理系统->查询信息:（0为退出）*", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
				center_color("*----------------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_BEIGE);
				center_color("*          1.查看所有员工的信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          2.按工号查询对应信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          3.按部门查询对应信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          4.按职称查询对应信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          5.通过条件查询或统计          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("******************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_BEIGE);
				char key3[8] = { -1 };
				centerinput(key3);
				if (strcmp(key3, "1") == 0) {
					center_color("********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					center_color("*       请输入排序指标:        *", COLOR_DARKGRAY | COLOR_BRIGHT, COLOR_DARKBROWN);
					center_color("*------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					center_color("*            1.工号            *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
					center_color("*            2.薪资            *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
					center_color("*            0.退出            *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
					center_color("********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					char key0[8] = { -1 };
					centerinput(key0);
					if (strcmp(key0, "1") == 0) {
						readPersonData(); sortPersonByPID(); showAllPersonData();
					}
					else if (strcmp(key0, "2") == 0) {
						readPersonData(); sortPersonBySalary(); showAllPersonData();
					}
					else {
						break;
					}
				}
				else if (strcmp(key3, "2") == 0) {
					readPersonData(); showPersonDataByPID();
				}
				else if (strcmp(key3, "3") == 0) {
					readPersonData(); showPersonDataByFrom();
				}
				else if (strcmp(key3, "4") == 0) {
					readPersonData(); showPersonDataByType();
				}
				else if (strcmp(key3, "5") == 0) {
					readPersonData(); showPersonDataByOdd();
				}
				else {
					break;
				}
			} while (1);
		}
		else if (strcmp(key, "4")==0) {
			system("cls");
			center_color("************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			center_color("*教职工信息管理系统->数据备份与恢复*", COLOR_DARKGRAY | COLOR_BRIGHT, COLOR_DARKBROWN);
			center_color("*----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			center_color("*        1.备份当前教工数据        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("*        2.查看所有备份数据        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("*        0.退            出        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			char key4[8] = {-1};
			centerinput(key4);
			if (strcmp(key4, "1") == 0) {
				readPersonData(); getBackupNumber(); backupPersonData();
			}
			else if (strcmp(key4, "2") == 0) {
				readPersonData();
				readBackupIDandTime();
				center_color("*----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
				center_color("*    查看某个编号对应的备份信息    *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				char temp[8] = { -1 };
				char key_backup_select[20] = {-1};
				do {
					center_color("*----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					center_color("*      请输入编号：(0为退出)       *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
					center_color("*----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					centerinput(key_backup_select);
					int num = atoi(key_backup_select);
					if (num!=0) {
						showBackupById(num);
						sprintf(temp, "%d", num);
					}
				} while (strcmp(key_backup_select,"0")!=0);
				strcpy(lines, "是否恢复为");
				strcat(lines, temp);
				strcat(lines, "号备份？（y or n）");
				center_color(lines, COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				memset(temp, 0, sizeof(temp));
				centerinput(temp);
				if (strcmp(temp,"y")==0) {
					savePersonData();
					center_color("恢复成功！", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				}
				system("pause");
			}
		}
		else {
			center_color("=============谢谢使用=============", COLOR_WHITE | COLOR_BRIGHT, COLOR_PURPLE);
			break;
		}
	}while (1);
}
void menu0() {
	char key[8] = { -1 };
	do {
		system("cls");
		center_color("**********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("*       教职工信息管理系统       *", COLOR_DARKGRAY, COLOR_LIGHTYELLOW);
		center_color("*--------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("*       您的身份是:游  客        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
		center_color("*--------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		center_color("*        1.教职工信息录入        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("*        2.查  询  信  息        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("*        3.数据备份与恢复        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("*        0.退  出  系  统        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
		center_color("**********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
		centerinput(key);
		char key1[8] = { -1 };
		char lines[50] = { 0 };
		if (strcmp(key, "1") == 0) {
			system("cls");
			readPersonData();
			insertPersonData();
		}
		else if (strcmp(key, "2") == 0) {
			system("cls");
			do {
				center_color("******************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_BEIGE);
				center_color("*教职工信息管理系统->查询信息:（0为退出）*", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
				center_color("*----------------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_BEIGE);
				center_color("*          1.查看所有员工的信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          2.按工号查询对应信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          3.按部门查询对应信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          4.按职称查询对应信息          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("*          5.通过条件查询或统计          *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
				center_color("******************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_BEIGE);
				char key3[8] = { -1 };
				centerinput(key3);
				if (strcmp(key3, "1") == 0) {
					center_color("********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					center_color("*       请输入排序指标:        *", COLOR_DARKGRAY | COLOR_BRIGHT, COLOR_DARKBROWN);
					center_color("*------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					center_color("*            1.工号            *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
					center_color("*            2.薪资            *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
					center_color("*            0.退出            *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
					center_color("********************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
					char key0[8] = { -1 };
					centerinput(key0);
					if (strcmp(key0, "1") == 0) {
						readPersonData(); sortPersonByPID(); showAllPersonData();
					}
					else if (strcmp(key0, "2") == 0) {
						readPersonData(); sortPersonBySalary(); showAllPersonData();
					}
					else {
						break;
					}
				}
				else if (strcmp(key3, "2") == 0) {
					readPersonData(); showPersonDataByPID();
				}
				else if (strcmp(key3, "3") == 0) {
					readPersonData(); showPersonDataByFrom();
				}
				else if (strcmp(key3, "4") == 0) {
					readPersonData(); showPersonDataByType();
				}
				else if (strcmp(key3, "5") == 0) {
					readPersonData(); showPersonDataByOdd();
				}
				else {
					break;
				}
			} while (1);
		}
		else if (strcmp(key, "3") == 0) {
			system("cls");
			center_color("************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			center_color("*教职工信息管理系统->数据备份与恢复*", COLOR_DARKGRAY | COLOR_BRIGHT, COLOR_DARKBROWN);
			center_color("*----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			center_color("*        1.备份当前教工数据        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("*        0.退            出        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
			center_color("************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
			char key4[8] = { -1 };
			centerinput(key4);
			if (strcmp(key4, "1") == 0) {
				readPersonData(); getBackupNumber(); backupPersonData();
			}
		}
		else {
			center_color("=============谢谢使用=============", COLOR_WHITE | COLOR_BRIGHT, COLOR_PURPLE);
			break;
		}
	} while (1);
}
void main() {
	char numkeys[30] = {0};
	char temp1[30];
	char temp2[30];
	center_color("*************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("*************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("*                                   *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
	center_color("*       这 是 一 个 登 录 页        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
	center_color("*                                   *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
	center_color("*-----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("*-----------------------------------*", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("*                                   *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
	center_color("*          请 输 入 密 码           *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
	center_color("*                                   *", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKBLUE);
	center_color("*************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	center_color("*************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_DARKGRAY);
	centerinput(numkeys);
	FILE* fp0 = fopen("keys.txt", "r");
	fscanf(fp0, "%s", temp1);
	Hash(numkeys,temp2);
	if (strcmp(temp2, temp1) == 0) {
	center_color("*************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
	center_color("*        欢迎进入管理员系统!        *", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
	center_color("*************************************", COLOR_WHITE | COLOR_BRIGHT, COLOR_LIGHT_PINK);
	center("按任意键继续...");
	getchar();
		system("cls");
		menu1();
		return;
	}
	menu0();
}