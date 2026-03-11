#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
struct Person {
	char pid[20];
	char name[20];
	char sex[4];
	char type[20];
	char from[20];
	float salary;
};
typedef struct PersonNode {
	Person data;
	struct PersonNode* next;
}PersonNode;

PersonNode* person_head = NULL;
int backup_number = 1;

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
		printf("教职工数据文件不存在，将为您新建一个\n");
		return;
	}
	Person temp = { 0 };
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
	printf("已读取教职工数据\n");
}

void insertPersonData() {//用户写入教职工信息
	FILE* fp = fopen("data.txt", "a");
	char pid[20];
	char name[20];
	char sex[4];
	char type[20];
	char from[20];
	float salary;
	if (!fp) {
		printf("数据文件打开失败，无法写入！\n");
		return;
	}
	printf("请输入：\n工号\t姓名\t性别\t职称\t部门\t薪资(空格隔开)\n");
	scanf("%s %s %s %s %s %f", pid, name, sex, type, from, &salary);
	if (isPersonInList(pid)) {
		printf("已有该教职工的信息!\n");
		fclose(fp);
		return;
	}
	fprintf(fp, "\n%s %s %s %s %s %.1f", pid, name, sex, type, from, salary);
	fclose(fp);
	
	readPersonData();
}
void showAllPersonData() {//查看所有教职工的各项信息
	PersonNode* temp = person_head;
	printf("========================================================\n");
	printf("工号\t姓名\t性别\t职称\t部门\t\t薪资\n");
	while (temp) {
		printf("%s\t%s\t%s\t%s\t%s\t%.1f\n", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
		temp = temp->next;
	}
	printf("========================================================\n");
}
void showPersonDataByPID() {
	PersonNode* temp = person_head;
	printf("请输入工号:\n");
	char pid[20];
	scanf("%s",pid);
	printf("工号\t姓名\t性别\t职称\t部门\t\t薪资\n");
	while (temp) {
		if (strcmp(temp->data.pid, pid) == 0) {
			printf("%s\t%s\t%s\t%s\t%s\t%.1f\n", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
		}
		temp = temp->next;
	}
}
void showPersonDataByFrom() {
	PersonNode* temp = person_head;
	printf("请输入部门名称:\n");
	char from[20];
	scanf("%s", from);
	printf("工号\t姓名\t性别\t职称\t部门\t\t薪资\n");
	while (temp) {
		if (strcmp(temp->data.from, from) == 0) {
			printf("%s\t%s\t%s\t%s\t%s\t%.1f\n", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
		}
		temp = temp->next;
	}
}
void showPersonDataByType() {
	PersonNode* temp = person_head;
	printf("请输入职称名称:\n");
	char type[20];
	scanf("%s", type);
	printf("工号\t姓名\t性别\t职称\t部门\t\t薪资\n");
	while (temp) {
		if (strcmp(temp->data.type, type) == 0) {
			printf("%s\t%s\t%s\t%s\t%s\t%.1f\n", temp->data.pid, temp->data.name, temp->data.sex, temp->data.type, temp->data.from, temp->data.salary);
		}
		temp = temp->next;
	}
}
void savePersonData() {
	FILE* fp = fopen("data.txt", "w");
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
	PersonNode* pre = person_head;
	PersonNode* p = pre->next;
	printf("请输入工号或名称:\n");
	char pid[20];
	scanf("%s", pid);
	if (!isPersonInList(pid)) {
		printf("未找到%s的信息！\n", pid);
		return;
	}
	while (p) {
		if (strcmp(pre->data.pid, pid) == 0 || strcmp(pre->data.name, pid) == 0) {
			person_head->next = p;
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
	printf("已删除%s的信息\n",pid);
}
void changePersonData() {
	PersonNode* pre = person_head;
	printf("请输入工号或名称:\n");
	char pid[20];
	scanf("%s", pid);
	if (!isPersonInList(pid)) {
		printf("未找到%s的信息！\n", pid);
		return;
	}
	while (pre) {
		if (strcmp(pre->data.pid, pid) == 0 || strcmp(pre->data.name, pid) == 0) {
			break;
		}
		pre = pre->next;
	}
	printf("该教职工的现有信息为:\n工号\t姓名\t性别\t职称\t部门\t\t薪资\n");
	printf("%s\t%s\t%s\t%s\t%s\t%.1f\n", pre->data.pid, pre->data.name, pre->data.sex, pre->data.type, pre->data.from, pre->data.salary);
	char value[20];
	
	while (1) {
		printf("请输入需要更改的信息:（exit为退出）\n");
		scanf("%s", value);
		char keys[20];

		if (strcmp(value, "工号") == 0) {
			printf("请输入修改后的信息:\n");
			scanf("%s", keys);
			strcpy(pre->data.pid, keys);
			printf("已修改完成!\n");
			break;
		}
		else if (strcmp(value, "姓名") == 0) {
			printf("请输入修改后的信息:\n");
			scanf("%s", keys);
			strcpy(pre->data.name, keys);
			printf("已修改完成!\n");
			break;
		}
		else if (strcmp(value, "性别") == 0) {
			printf("请输入修改后的信息:\n");
			scanf("%s", keys);
			strcpy(pre->data.sex, keys);
			printf("已修改完成!\n");
			break;
		}
		else if (strcmp(value, "职称") == 0) {
			printf("请输入修改后的信息:\n");
			scanf("%s", keys);
			strcpy(pre->data.type, keys);
			printf("已修改完成!\n");
			break;
		}
		else if (strcmp(value, "部门") == 0) {
			printf("请输入修改后的信息:\n");
			scanf("%s", keys);
			strcpy(pre->data.from, keys);
			printf("已修改完成!\n");
			break;
		}
		else if (strcmp(value, "薪资") == 0) {
			printf("请输入修改后的信息:\n");
			float salary;
			scanf("%f", &salary);
			pre->data.salary = salary;
			printf("已修改完成!\n");
			break;
		}
		else if (strcmp(value, "exit") == 0) {
			break;
		}
		else {
			printf("输入有误，请重新输入\n");
			continue;
		}
	
	}
	

	savePersonData();
	
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
				/*is_new_line = 0;
				continue;*/
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
		printf("打开失败");
		return;
	}
	char backup_data[50];
	while (fgets(backup_data, sizeof(backup_data), fp)!=NULL) {
		if (backup_data[0] == 'C') {
			continue;
		}
		else
		{
			printf("%s", backup_data);
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
	printf("备份完毕！备份编号为%d\n", backup_number-1);
	fclose(fp1);
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
		while (1) {
			if (fgets(line, sizeof(line), fp) == NULL)break; 
			if (line[0] != 'C') {
				break;
			}
			printf("%s", line);
			sscanf(line, "%s %s %s %s %s %f", temp.pid, temp.name, temp.sex, temp.type, temp.from, &temp.salary);
			if (!isPersonInList(temp.pid)) {
				PersonNode* newnode = (PersonNode*)malloc(sizeof(PersonNode));
				newnode->data = temp;
				newnode->next = person_head;
				person_head = newnode;
			
			}
		
		}
		
	}
	else {
		printf("未找到编号为 %d 的备份\n", target_id);
	}

	fclose(fp);
}
void menu() {
	int key = -1;
	do {
		system("cls");
		printf("========================================================\n");
		printf("教职工信息管理系统\n");
		printf("1.教职工信息录入\n");
		printf("2.管理教职工信息（删/改）\n");
		printf("3.查询信息\n");
		printf("4.数据备份与恢复\n");
		scanf("%d", &key);
		int key0 = -1;
		int key1 = -1;
		int key2 = -1;
		int key3 = -1;
		int key4 = -1;
		int key_backup_select = -1;
		int temp;
		switch (key) {
		case 1:
			readPersonData();
			insertPersonData();
			break;
		case 2:
			do {
				system("cls");
				printf("教职工信息管理系统->管理教职工信息\n");
				printf("1.删除某个教职工的信息\n");
				printf("2.更改某个教职工的信息\n");
				scanf("%d", &key2);
				switch (key2) {
				case 1:readPersonData(); deletePersonDataByPID(); break;
				case 2:readPersonData();changePersonData(); break;
				}
				Sleep(500);
			
			} while (key2 != 0);
			
			
			break;
		case 3: {

			do {
				printf("教职工信息管理系统->查询信息:\n");
				printf("1.查看所有员工信息\n");
				printf("2.按工号查询对应信息\n");
				printf("3.按部门查询对应信息\n");
				printf("4.按职称查询对应信息\n");
				scanf("%d", &key3);
				switch (key3) {
				case 1:
					printf("请输入排序指标:\n");
					printf("1.工号\n");
					printf("2.薪资\n");
					scanf("%d", &key0);
					switch (key0) {
					case 1:readPersonData(); sortPersonByPID(); showAllPersonData(); break;
					case 2:readPersonData(); sortPersonBySalary(); showAllPersonData(); break;
					}
					break;
				case 2:readPersonData(); showPersonDataByPID(); break;
				case 3:readPersonData(); showPersonDataByFrom(); break;
				case 4:readPersonData(); showPersonDataByType(); break;
				}

			} while (key3 != 0);
			break;
		}
		case 4: {
			printf("教职工信息管理系统->数据备份与恢复\n");
			printf("1.备份当前教工数据\n");
			printf("2.查看所有备份数据时间\n");
			scanf("%d", &key4);
			switch (key4) {
			case 1:readPersonData(); getBackupNumber(); backupPersonData(); break;
			case 2:; readPersonData(); 
				readBackupIDandTime();
				printf("查看某个编号对应的备份信息\n");
				do {
					printf("请输入编号：(0为退出)\n");
					scanf("%d", &key_backup_select);
					if (key_backup_select != 0) {
						showBackupById(key_backup_select);
						temp = key_backup_select;
					}
				} while (key_backup_select != 0);
				printf("是否恢复为%d号备份？（1 or 0）",temp);
				scanf("%d", &key_backup_select);
				if (key_backup_select) {
					savePersonData();
					printf("恢复成功！\n");
				}
				break;
			}
			break;
		}
		}
		printf("\n");
	} while (key != 0);
	
}
void main() {
	menu();
}