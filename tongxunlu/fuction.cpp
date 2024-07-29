﻿#include<iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include"head.h"
#include <conio.h>

#include <sstream>

#pragma warning(disable : 4996)
#define MAX_TAGS 26  // A到Z的26个字母
multimap <string, int> mp;
using namespace std;
string fileName = "contacts.txt";
vector <int> idx;

//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------Data----------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------

int readContacts(struct ContactRecord contacts[], int* num_contacts)

{
    ifstream inputFile("group.txt");

    // 检查文件是否打开成功
    if (!inputFile.is_open()) {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        char key[20];
        int value;
        if (iss >> key >> value) {
            mp.insert({ key, value });
        }
    }

    // 关闭文件
    inputFile.close();

    FILE* fp = fopen("contacts.txt", "rb");             // 以二进制只读方式打开
    if (fp == NULL) {
        perror("打开文件失败");
        return -1;
    }

    *num_contacts = 0;                                  // 初始化联系人数量为0

    struct ContactRecord tmp;

    //%20s的逻辑是 读取的时候一直读 直到遇见空格/换行/或者读满20个
    while (fscanf(fp, "%20s%20s%30s%20s%20s\n",         //这里的格式化读取有问题，要按照指定的格式来读取，才能读取到
        tmp.name, tmp.phone, tmp.address, tmp.postcode, tmp.email) == 5)
    {
        contacts[*num_contacts] = tmp;
        (*num_contacts)++;

        // 打印
        /*printf("联系人: %d\n", *num_contacts);
        printf("Name: %s\nPhone: %s\nAddress: %s\nPostcode: %s\nEmail: %s\n",
            tmp.name, tmp.phone, tmp.address, tmp.postcode, tmp.email);
        printf("-----------------------------------------------------------\n");*/
        if (*num_contacts >= MAX_CONTACTS) break;        //限制最大联系人数量
    }

    fclose(fp);  // 关闭文件
    fp = NULL;


    return 0;
}

void entry_txt(string& line) {
    ofstream fin;//输入信息
    fin.open(fileName, ios::app);
    fin << line << endl;//'\n' 
    fin.close();
    ofstream outputFile("group.txt");

    // 检查文件是否打开成功
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    // 将  写入文件
    for (const auto& pair : mp) {
        outputFile << pair.first << " " << pair.second << std::endl;
    }

    // 关闭文件
    outputFile.close();

}

int writeContacts(struct ContactRecord contacts[], int num_contacts) {
    ofstream fin;//输入信息
    fin.open(fileName);
    for (int i = 0; i < num_contacts; i++) {
        if (contacts[i].name[0] == '$') {//被逻辑删除的联系人不会被保存
            continue;
        }
        string Person;
        Person += contacts[i].name, Person += ' ';
        Person += contacts[i].phone, Person += ' ', Person += contacts[i].address, Person += ' ';
        Person += contacts[i].postcode, Person += ' ', Person += contacts[i].email;
        if (Person == "") {
            return -1;
        }
        entry_txt(Person);
    }
    return 0;
}

// 验证用户身份
bool authenticateUser(const char* filename, const char* username, const char* password)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "无法打开文件：" << filename << endl;
        return false;
    }

    string fileUsername;
    string filePassword;

    while (file >> fileUsername >> filePassword)
    {
        if (fileUsername == username && filePassword == password)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}
/*

    一个汉字在计算机中占 2~4 个字节，具体取决于使用的编码方式，文件->高级保存选项查看编码格式
    UTF - 8: 这是目前最流行的通用字符编码，它使用变长编码，汉字通常占用 3 个字节，部分生僻字可能占用 4 个字节
    本机中实际占2个字节

    省 / 自治区 / 直辖市 + 市 + 区 / 县 + 街道 / 乡镇 + 门牌号    长度约为 30 个字符
    省/自治区/直辖市 + 市 + 区/县 + 详细地址                     长度约为 20 个字符

    中国邮编一般由6位数字组成
    这六位数字的具体含义是：‌前两位数字代表省（‌直辖市、‌自治区）‌，‌第三位数字代表邮区，‌第四位数字代表县（‌市）‌，
    最后两位数字代表从这个城市哪个投递区投递的，‌即投递区的位置

    用户名部分：一般在6-30个字符之间,有些邮箱服务可能对最短长度有要求,如至少4-6个字符
    域名部分：一般在5-15个字符左右

    //计算字节
    char str[] = "12312312311\n";
    printf("字节数: %lu\n", sizeof(str) - 1);  // 减1是为了不计算结尾的'\0'

    0 < strlen(newContact.postcode) < 6+1 这样的写法在C语言中是不正确的。这个表达式会先计算 0 < strlen(newContact.postcode)，
    结果为 1（真），然后再比较 1 < 6+1，这总是为真
*/

int sortContactsByName(ContactRecord contacts[], int num_contacts)
{
    if (num_contacts == 0)  return 0;
    for (int i = 0; i < num_contacts - 1; i++) {
        for (int j = i + 1; j < num_contacts; j++) {
            if (strcmp(contacts[i].name, contacts[j].name) > 0) {
                ContactRecord temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }
    return 0;
}

void encrypt(char* pwd)					//这是一个名为encrypt的函数，用于对传入的字符串进行简单的加密处理
{
    int pwdlen = strlen(pwd);			// 获取字符串的长度，用于循环处理每个字符。
    for (int i = 0; i < pwdlen; i++)	//遍历字符串中的每个字符。
    {
        pwd[i] ^= 15;					// 对于每个字符，使用异或运算对其进行加密处理
    }
}


//-------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------Service------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
 

// 服务层：模糊搜索联系人
int fuzzySearchContacts( ContactRecord contacts[], int num_contacts,  char* keyword) {
    for (int i = 0; i < num_contacts; ++i) {
        if (contacts[i].name[0] == '$') continue;
        if (strstr(contacts[i].name, keyword) || strstr(contacts[i].phone, keyword) ||
            strstr(contacts[i].address, keyword) || strstr(contacts[i].postcode, keyword) ||
            strstr(contacts[i].email, keyword)) {
            idx.push_back(i);  // 找到联系人，返回索引
        }
    }
    if(idx.empty()) return -1; 
     // 未找到联系人
    return 0;
}

// 服务层：多条件搜索联系人
int multiConditionSearch( ContactRecord contacts[], int num_contacts,  char* name, char* phone,
     char* address,  char* postcode,  char* email) {
    for (int i = 0; i < num_contacts; ++i) {
        if ((strlen(name) == 0 || strcmp(contacts[i].name, name) == 0) &&
            (strlen(phone) == 0 || strcmp(contacts[i].phone, phone) == 0) &&
            (strlen(address) == 0 || strcmp(contacts[i].address, address) == 0) &&
            (strlen(postcode) == 0 || strcmp(contacts[i].postcode, postcode) == 0) &&
            (strlen(email) == 0 || strcmp(contacts[i].email, email) == 0)) {
            return i;  // 找到联系人，返回索引
        }
    }
    return -1;  // 未找到联系人
}


// 按地址查找联系人
 int searchContactByAddress(struct ContactRecord contacts[], int num_contacts, char* address)
{
     int mark = 0;
    for (int i = 0; i < num_contacts; i++)
    {
        if (contacts[i].name[0] == '$') continue;
        if (strcmp(contacts[i].address, address) == 0)
        {
            idx.push_back(i);
        }
    }
    if (idx.empty()) return -1;
    return 0; // 未找到联系人
}


// 删除分组
int deleteGroup(struct ContactRecord contacts[], int* num_contacts,  char* group)
{
    string tmp(group);
    return mp.erase(tmp);// 返回删除的联系人数量

    //while (i < *num_contacts)
    //{
    //    if (strcmp(contacts[i].group, group) == 0)
    //    {
    //        // 删除当前联系人，覆盖该联系人并减少联系人数量
    //        for (int j = i; j < *num_contacts - 1; j++)
    //        {
    //            contacts[j] = contacts[j + 1];
    //        }
    //        
    //    }
    //    else
    //    {
    //        i++;
    //    }
    //}

    
}

// 2.12 按分组查找联系人
int searchContactsByGroup(ContactRecord contacts[], int num_contacts, char* group) {
    for (auto it = mp.begin(); it != mp.end(); it++) {
        if (contacts[it->second].name[0] == '$') continue;
        if (strcmp(it->first.c_str(), group) == 0) {
                    idx.push_back(it->second);
                }
    }

    if (idx.empty()) {
        return -1;  // 未找到联系人
    }

    // 打印找到的联系人

    return 0;  // 返回找到的联系人数量
}

// 2.6 按邮编查找联系人
int searchContactByPostcode(ContactRecord contacts[], int num_contacts, char* postcode) {
    for (int i = 0; i < num_contacts; ++i) {
        if (contacts[i].name[0] == '$') continue;
        if (strcmp(contacts[i].postcode, postcode) == 0) {
            idx.push_back(i);
        }
    }
    if (idx.empty()) {
        return -1;
    }
    return 0;  // 未找到联系人
}
/*
searchContactByEmail
功能: 在 contacts 数组中查找指定邮箱的联系人。
参数:
   contacts[]: 结构体数组，包含联系人信息。
   num_contacts: 联系人数量。
   email: 要查找的联系人邮箱地址。
返回值:
   -1: 未找到联系人。
   >= 0: 找到联系人，返回联系人索引。
*/
// 2.7 按邮箱查找联系人
int searchContactByEmail(ContactRecord contacts[], int num_contacts, char* email) {
    for (int i = 0; i < num_contacts; ++i) {
        if (contacts[i].name[0] == '$') continue;
        if (strcmp(contacts[i].email, email) == 0) {
            return i;  // 找到联系人，返回联系人索引
        }
    }
    return -1;  // 未找到联系人
}

//2.8
int deleteContact(struct ContactRecord contacts[], int* num_contacts, char* key, int delete_type) {
    
        //0 为按姓名删除，1 为按电话号码删除，2 地址删除
    int i = -10086; int a = -10086;
        switch (delete_type) {
        case 1://
              searchContactByName(contacts, *num_contacts, key);
              for (int i : idx) {
                  cout << i;
                  printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
                  printf("\t\t\t姓名: %s\n", contacts[i].name);
                  printf("\t\t\t电话: %s\n", contacts[i].phone);
                  printf("\t\t\t地址: %s\n", contacts[i].address);
                  printf("\t\t\te-mail:%s\n", contacts[i].email);
                  printf("\t\t\t************************************************\n");

              }
              cout << "输入你想删除的联系人的序号";

              cin >> a;
              contacts[a].name[0] = '$';
              idx.clear();//由于idx是全局变量，所以使用后必须清空
              return 0;
            break;
        case 2:
             i = searchContactByPhone(contacts, *num_contacts, key);
                contacts[i].name[0] = '$';
                return 0;
            break;
        case 3:
            searchContactByAddress(contacts, *num_contacts, key);
            for (int i : idx) {
                cout << i;
                printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
                printf("\t\t\t姓名: %s\n", contacts[i].name);
                printf("\t\t\t电话: %s\n", contacts[i].phone);
                printf("\t\t\t地址: %s\n", contacts[i].address);
                printf("\t\t\te-mail:%s\n", contacts[i].email);
                printf("\t\t\t************************************************\n");

            }
            cout << "输入你想删除的联系人的序号";
            
            cin >> a;
            contacts[a].name[0] = '$';
            idx.clear();
            return 0;
            break;
        case 4:
            searchContactByPostcode(contacts, *num_contacts, key);
            for (int i : idx) {
                cout << i;
                printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
                printf("\t\t\t姓名: %s\n", contacts[i].name);
                printf("\t\t\t电话: %s\n", contacts[i].phone);
                printf("\t\t\t地址: %s\n", contacts[i].address);
                printf("\t\t\te-mail:%s\n", contacts[i].email);
                printf("\t\t\t************************************************\n");

            }
            cout << "输入你想删除的联系人的序号";
            
            cin >> a;
            contacts[a].name[0] = '$';
            idx.clear();
            return 0;
            break;
         case 5:
            searchContactByEmail(contacts, *num_contacts, key);
            contacts[a].name[0] = '$';
            return 0;
            break;
         case 6:
             fuzzySearchContacts(contacts, *num_contacts, key);
             for (int i : idx) {
                 cout << i;
                 printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
                 printf("\t\t\t姓名: %s\n", contacts[i].name);
                 printf("\t\t\t电话: %s\n", contacts[i].phone);
                 printf("\t\t\t地址: %s\n", contacts[i].address);
                 printf("\t\t\te-mail:%s\n", contacts[i].email);
                 printf("\t\t\t************************************************\n");

             }
             cout << "输入你想删除的联系人的序号";
           
             cin >> a;
             contacts[a].name[0] = '$';
             idx.clear();
             return 0;
             break;
         case 7:
             searchContactsByGroup(contacts, *num_contacts, key);
             for (int i : idx) {
                 cout << i;
                 printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
                 printf("\t\t\t姓名: %s\n", contacts[i].name);
                 printf("\t\t\t电话: %s\n", contacts[i].phone);
                 printf("\t\t\t地址: %s\n", contacts[i].address);
                 printf("\t\t\te-mail:%s\n", contacts[i].email);
                 printf("\t\t\t************************************************\n");

             }
             cout << "输入你想删除的联系人的序号";
             
             cin >> a;
             contacts[a].name[0] = '$';
             idx.clear();
             return 0;
             break;
         }
      
    
    return -1;
}


//2.16

void erase_tag(char* string) {//删除标签
    int size = sizeof(string) / sizeof(char);//获取字符串长度
    int tail = size - 1;
    while (string[tail] != '(') {
        tail--;
    }
    string[tail] = '\0';

}

//
int deleteTagFromContact(struct ContactRecord contacts[], int num_contacts, char* contactName, char* tag) {
    system("cls");
    printf("\t\t************* 请输入查找方式 ***********\n\n");
    printf("\t\t\t1.姓名查找\n");
    printf("\t\t\t2.电话查找\n");
    printf("\t\t\t3.地址查找\n");
    printf("\t\t\t4.邮编查找\n");
    printf("\t\t\t5.邮箱查找\n");
    printf("\t\t\t6.模糊查找\n");
    printf("\t\t\t7.分组查找\n");
    char s[80];
    int a;
    do {
        printf("Enter you choice(0~8):");
        scanf("%s", s);												// 获取用户输入的字符串，并存储到字符数组s中
        a = atoi(s);
    } while (a < 1 || a>8);
    system("cls");
    char key[80];
    printf("\t\t输入查找联系人信息:\n");
    scanf("%s", &key);
    int res = -1;
    switch (a) {
    case 1:
        res = searchContactByName(contacts, num_contacts, key); break;
    case 2:
        res = searchContactByPhone(contacts, num_contacts, key); break;
    case 3:
        res = searchContactByAddress(contacts, num_contacts, key); break;
    case 4:
        res = searchContactByPostcode(contacts, num_contacts, key); break;
    case 5:
        res = searchContactByEmail(contacts, num_contacts, key); break;
    case 6:
        res = fuzzySearchContacts(contacts, num_contacts, key); break;
    case 7:
        res = searchContactsByGroup(contacts, num_contacts, key); break;
    }
    if (res >= 0) {
        printf("\t\t查找成功\n");
        erase_tag(contacts[res].name);
    }
    else {
        printf("\t\t查找失败\n");
    }
    return 0;
}


int addContact(ContactRecord contacts[], int* num_contacts)
{
    
    printf("\n\t\t\t**************** 请输入用户信息 ****************\n");
    printf("\t\t\t输入姓名:");
    scanf("%s", contacts[*num_contacts].name);									// 获取用户输入的姓名，并存储到结构体数组中
    printf("\n\t\t\t输入电话号码:");
    scanf("%s", contacts[*num_contacts].phone);								// 获取用户输入的电话号码，并存储到结构体数组中
    printf("\n\t\t\t输入地址:");
    scanf("%s", contacts[*num_contacts].address);								// 获取用户输入的地址，并存储到结构体数组中
    printf("\n\t\t\t输入邮编:");
    scanf("%s", contacts[*num_contacts].postcode);								//获取用户输入的邮编，并存储到结构体数组中
    printf("\n\t\t\t输入e-mail:");
    scanf("%s", contacts[*num_contacts].email);								// 获取用户输入的电子邮件地址，并存储到结构体数组中
    (*num_contacts)++;															// 学生信息数量加1
    printf("\n\t\t\t是否继续添加?(Y/N):");// 提示用户是否继续添加
    char op;
    cin >> op;
    if (op == 'y' || op == 'Y')							// 如果用户输入的是’y’或’Y’，则递归调用adduser函数添加学生信息
        addContact(contacts, num_contacts);													// 返回0,结束函数
    return 0;
}

void listContacts(ContactRecord contacts[], int num_contacts)
{
    int i;
    
    system("cls");									// 清屏，清除控制台中之前的内容
    if (num_contacts != 0)									// 如果有学生信息，则显示所有学生信息
    {
        printf("\t\t\t*************** 以下为通讯录所有信息************\n");
        for (i = 0; i < num_contacts; i++)					// 遍历所有学生信息，并逐一显示
        {
            
            if (contacts[i].name[0] == '$'){
                
                continue;
            }
            printf("\t\t\t姓名:%s\n", contacts[i].name);
            printf("\t\t\t电话:%s\n", contacts[i].phone);
            printf("\t\t\t地址:%s\n", contacts[i].address);
            printf("\t\t\t邮编:%s\n", contacts[i].postcode);
            printf("\t\t\tEmail:%s\n", contacts[i].email);
            //if (i + 1 < num_contacts)						// 如果不是最后一个学生信息，提示用户按任意键继续
            //{
            //    system("pause");					// 将控制台暂停，等待用户按任意键继续
            //}
        }
        printf("\t\t\t************************************************\n");
    }
    else											// 如果没有学生信息，则提示用户
        printf("\t\t\t通讯录中无任何纪录\n");
    printf("\t\t\t按任意键返回主菜单:\n");
    _getch();										// 等待用户按任意键
    return;											// 返回，结束函数
}

int searchContactByName(ContactRecord contacts[], int num_contacts, char* name)
{
                											// 定义一个整型变量mark，用来标记是否查找到了符合条件的学生信息
    int i;													// 定义一个整型变量i，用来遍历所有学生信息
    printf("\t\t\t***************** 按姓名查找 *******************\n");
    										                 // 定义一个字符数组name，用来存储用户输入的姓名
    									                      // 获取用户输入的姓名
    for (i = 0; i < num_contacts; i++)								// 遍历所有学生信息，查找符合条件的学生信息
    {
        if (contacts[i].name[0] == '$') {
            continue;
        }
        if (strcmp(contacts[i].name, name) == 0)				// 如果找到了符合条件的学生信息，则输出信息并增加mark的值
        {
            idx.push_back(i);
        }      
    }
    if (idx.empty()) return -1;
    return 0;

}

int searchContactByPhone(ContactRecord contacts[], int num_contacts, char* phone)
{
   										// 定义一个整型变量mark，用来标记是否查找到了符合条件的学生信息
    int i;													// 定义一个整型变量i，用来遍历所有学生信息
    printf("\t\t\t****************** 按电话查找 ******************\n");
   
    for (i = 0; i < num_contacts; i++)								// 遍历所有学生信息，查找符合条件的学生信息
    {
        if (contacts[i].name[0] == '$') continue;
        if (strcmp(contacts[i].phone, phone) == 0)			// 如果找到了符合条件的学生信息，则输出信息并增加mark的值
        {
            return i;
        }
    }
    
   
    return - 1;
}

// 2.13 添加分组
int addGroup(struct ContactRecord contacts[], int* num_contacts, char* group) {
    char new_group[20];
    printf("请输入要添加的新分组名： ");
    scanf("%s", new_group);

    //1.先选择查找方式查找到要插入该分组的联系人
    int choice;
    printf("请选择查找方式：\n");
    printf("1.按姓名查找\n");
    printf("2.按电话查找\n");
    printf("3.按地址查找\n");
    printf("4.按邮编查找\n");
    printf("5.按邮箱查找\n");
    printf("6.模糊查找\n");
    printf("7.多条件查找\n");
    scanf("%d", &choice);
    /*getchar(); */                       //捕获换行符

    //2.再去执行相应的查找
    int index = -1;   //保存返回的索引
    int a = -10086;
    switch (choice) {
    case 1: {
        char name[20];
        printf("输入要查找的联系人姓名：");
        scanf("%s", name);
        index = searchContactByName(contacts, *num_contacts, name);
        for (int i : idx) {
            cout << i;
            printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
            printf("\t\t\t姓名: %s\n", contacts[i].name);
            printf("\t\t\t电话: %s\n", contacts[i].phone);
            printf("\t\t\t地址: %s\n", contacts[i].address);
            printf("\t\t\te-mail:%s\n", contacts[i].email);
            printf("\t\t\t************************************************\n");

        }
        if (!idx.empty()) {
            cout << "输入你想添加的联系人的序号";

            cin >> a;
            mp.insert({ new_group, a });
            idx.clear();//由于idx是全局变量，所以使用后必须清空 
        }
        break;
    }
    case 2: {
        char phone[20];
        printf("输入要查找的联系人电话：");
        scanf("%19s", phone);
        index = searchContactByPhone(contacts, *num_contacts, phone);
        if (index >= 0)
            mp.insert({ new_group, a });
        break;
    }
    case 3: {
        char address[20];
        printf("输入要查找的联系人地址：");
        scanf("%19s", address);
        index = searchContactByAddress(contacts, *num_contacts, address);
        for (int i : idx) {
            cout << i;
            printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
            printf("\t\t\t姓名: %s\n", contacts[i].name);
            printf("\t\t\t电话: %s\n", contacts[i].phone);
            printf("\t\t\t地址: %s\n", contacts[i].address);
            printf("\t\t\te-mail:%s\n", contacts[i].email);
            printf("\t\t\t************************************************\n");

        }
        if (!idx.empty()) {
            cout << "输入你想添加的联系人的序号";

            cin >> a;
            mp.insert({ new_group, a });
            idx.clear();//由于idx是全局变量，所以使用后必须清空 
        }
        break;
    }
    case 4: {
        char postcode[8];
        printf("输入要查找的联系人邮编：");
        scanf("%7s", postcode);
        index = searchContactByPostcode(contacts, *num_contacts, postcode);
        for (int i : idx) {
            cout << i;
            printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
            printf("\t\t\t姓名: %s\n", contacts[i].name);
            printf("\t\t\t电话: %s\n", contacts[i].phone);
            printf("\t\t\t地址: %s\n", contacts[i].address);
            printf("\t\t\te-mail:%s\n", contacts[i].email);
            printf("\t\t\t************************************************\n");

        }
        if (!idx.empty()) {
            cout << "输入你想添加的联系人的序号";

            cin >> a;
            mp.insert({ new_group, a });
            idx.clear();//由于idx是全局变量，所以使用后必须清空 
        }
        break;
    }
    case 5: {
        char e_mail[20];
        printf("输入要查找的联系人邮箱：");
        scanf("%19s", e_mail);
        index = searchContactByEmail(contacts, *num_contacts, e_mail);
        if (index >= 0)
            mp.insert({ new_group, a });
        break;
    }
          /*case 6: {
              char keyword[50];
              printf("输入要查找的联系人关键字：");
              scanf("%49s", keyword);
              index = fuzzySearchContacts(contacts, *num_contacts, keyword);
              break;
          }*/
    case 6: {
        char name[20];
        char phone[20];
        char address[20];
        char postcode[8];
        char e_mail[20];
        printf("输入要查找的联系人信息：");
        scanf("19%s", name);
        scanf("%19s", phone);
        scanf("%19s", address);
        scanf("%7s", postcode);
        scanf("%19s", e_mail);
        index = multiConditionSearch(contacts, *num_contacts, name, phone, address, postcode, e_mail);
        for (int i : idx) {
            cout << i;
            printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
            printf("\t\t\t姓名: %s\n", contacts[i].name);
            printf("\t\t\t电话: %s\n", contacts[i].phone);
            printf("\t\t\t地址: %s\n", contacts[i].address);
            printf("\t\t\te-mail:%s\n", contacts[i].email);
            printf("\t\t\t************************************************\n");

        }
        if (!idx.empty()) {
            cout << "输入你想添加的联系人的序号";

            cin >> a;
            mp.insert({ new_group, a });
            idx.clear();//由于idx是全局变量，所以使用后必须清空 
        }
        break;
    }
    default:
        printf("没有这个查找方式选项.\n");
        return -1;
    }

    //3.检查查找结果并更改分组
    if (index != -1) {

        printf("添加新分组成功！\n");
    }
    else {
        printf("未找到该联系人，添加分组失败\n");
    }
    return 0;				//返回0表示成功添加分组
}

//int addTagToContact(struct ContactRecord contact[], int num_contacts, char* contactName, char* tag) {
//    int findres = searchContactByName(contact, num_contacts, contactName);
//    if (findres != 0) {
//        printf("\t\t不存在该联系人!\n\t\t是否重新输入?\n\t\ty/n?\n");
//        char s;
//        char* rcontactname=NULL;
//        char* rtag=NULL;
//        scanf("%c", &s);
//        if (s == 'y' || s == 'Y') {
//            std::cin >> rcontactname >> rtag;
//            addTagToContact(contact, num_contacts, rcontactname, rtag);
//        }
//        else return -1;
//    }
//    else {
//        mp.insert(pair<char*, char*>(tag, contactName));
//        return 0;
//    }
//}



//-------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------View-----------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------



void showDeleteRecordInterface(struct ContactRecord contacts[], int *num_contacts) {

    system("cls");
    printf("\t\t************* 请输入查找方式 ***********\n\n");
    printf("\t\t\t1.姓名查找\n");
    printf("\t\t\t2.电话查找\n");
    printf("\t\t\t3.地址查找\n");
    printf("\t\t\t4.邮编查找\n");
    printf("\t\t\t5.邮箱查找\n");
    printf("\t\t\t6.模糊查找\n");
    printf("\t\t\t7.分组查找\n");
    printf("\t\t\t8.退出\n");
    char s[80];
    int a;
    do {
        printf("Enter you choice(0~8):");
        scanf("%s", s);												// 获取用户输入的字符串，并存储到字符数组s中
        a = atoi(s);
        if (a == 8) return;
    } while (a < 1 || a>8);
    system("cls");
    printf("\t\t输入删除联系人信息:\n\n");
    char key[80];
    scanf("%s", &key);
    int res = deleteContact(contacts, num_contacts, key, a);
    if (res == 0) {
        printf("\t\t删除成功\n");
        //(*num_contacts)--;   逻辑删除不能减一 否则将无法便利全部
    }
    else printf("\t\t删除失败\n");
    char op;
    printf("\t\t是否继续删除?\n");
    printf("\t\t\ty/n?\n");
    scanf(" %c", &op);
    if (op == 'y' || op == 'Y') showDeleteRecordInterface(contacts, num_contacts);
    else return;
}

void showSearchInterface(struct ContactRecord contacts[], int num_contacts) {
    system("cls");
    printf("\t\t************* 请输入查找方式 ***********\n\n");
    printf("\t\t\t1.姓名查找\n");
    printf("\t\t\t2.电话查找\n");
    printf("\t\t\t3.地址查找\n");
    printf("\t\t\t4.邮编查找\n");
    printf("\t\t\t5.邮箱查找\n");
    printf("\t\t\t6.模糊查找\n");
    printf("\t\t\t7.分组查找\n");
    printf("\t\t\t8.退出\n");
    int a;
    do {
        printf("Enter you choice(0~8):");
        cin >> a;   // 获取用户输入的字符串，并存储到字符数组s中
        if (a < 1 || a>8) {
            cout << "非法指令\n";
        }
    } while (a < 1 || a>8);

    system("cls");
    if (a == 8) return;
    char key[80];
    printf("\t\t输入查找联系人信息:\n");
    scanf("%s", key);
    
    int res = -1;
    int flag = -1;
    switch (a) {
    case 1:
        res = searchContactByName(contacts, num_contacts, key);
        flag++;
        break;
    case 2:
        res = searchContactByPhone(contacts, num_contacts, key);
        break;
    case 3:
        res = searchContactByAddress(contacts, num_contacts, key);
        flag++;
        break;
    case 4:
        res = searchContactByPostcode(contacts, num_contacts, key);
        flag++;
        break;
    case 5:
        res = searchContactByEmail(contacts, num_contacts, key);
        break;
    case 6:
        res = fuzzySearchContacts(contacts, num_contacts, key);
        flag++;
        break;
    case 7:
        res = searchContactsByGroup(contacts, num_contacts, key);
        flag++;
        break;
    case 8:
        return;
    default:
        cout << "请输入正确指令";
        return;
        break;

    }
    if (res >= 0) {
        printf("\t\t查找成功\n");
        if (flag == 0)
        {
            for (int i : idx) {
                printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
                printf("\t\t\t姓名: %s\n", contacts[i].name);
                printf("\t\t\t电话: %s\n", contacts[i].phone);
                printf("\t\t\t地址: %s\n", contacts[i].address);
                printf("\t\t\te-mail:%s\n", contacts[i].email);
                printf("\t\t\t************************************************\n");

            }
            idx.clear();
        }
        else {
            printf("\t\t\t************* 以下是您查找的用户信息 ***********\n");
            printf("\t\t\t姓名: %s\n", contacts[res].name);
            printf("\t\t\t电话: %s\n", contacts[res].phone);
            printf("\t\t\t地址: %s\n", contacts[res].address);
            printf("\t\t\te-mail:%s\n", contacts[res].email);
            printf("\t\t\t************************************************\n");
            
        }
    }
    if(res==-1) printf("\t\t查找失败\n");
    printf("\t\t是否继续进行查找?\ny/n\n");
    char op;
    scanf(" %c", &op);
    if (op == 'y' || op == 'Y') showSearchInterface(contacts, num_contacts);
    else return;
}

void showMainInterface()
{
    cout << "***************************" << endl;
    cout << "***** 1，增加通讯信息 *****" << endl;
    cout << "***** 2，查找通讯信息 *****" << endl;
    cout << "***** 3，修改通讯信息 *****" << endl;
    cout << "***** 4，删除通讯信息 *****" << endl;
    cout << "***** 5，显示所有记录 *****" << endl;
    cout << "****** 6，联系人分组 ******" << endl;
    cout << "****** 0，退出通讯录 ******" << endl;
    cout << "***************************" << endl;
}

void showInsertRecordInterface(struct ContactRecord contacts[], int* num_contacts) {
    if (*num_contacts >= MAX_CONTACTS) {
        printf("联系人列表已满，无法添加新联系人。\n");
        return;
    }

    struct ContactRecord newContact;
    printf("\n=== 插入新联系人 ===\n");

    // 获取姓名
    printf("请输入姓名 (最多19个字符): ");
    scanf(" %19[^\n]", newContact.name);

    // 获取电话号码
    printf("请输入电话号码 (最多11个字符): ");
    scanf(" %11[^\n]", newContact.phone);

    // 获取地址
    printf("请输入地址 (最多49个字符): ");
    scanf(" %49[^\n]", newContact.address);

    // 获取邮编
    printf("请输入邮编 (最多7个字符): ");
    scanf(" %7[^\n]", newContact.postcode);

    // 获取邮箱
    printf("请输入邮箱 (最多19个字符): ");
    scanf(" %19[^\n]", newContact.email);

    char confirm;
    printf("\n=== 请确认以下信息 ===\n");
    printf("姓名: %s\n", newContact.name);
    printf("电话: %s\n", newContact.phone);
    printf("地址: %s\n", newContact.address);
    printf("邮编: %s\n", newContact.postcode);
    printf("邮箱: %s\n", newContact.email);
    printf("\n确认添加这条记录吗? (Y/N): ");
    scanf(" %c", &confirm);

    if (toupper(confirm) == 'Y') {
        contacts[*num_contacts] = newContact;
        (*num_contacts)++;
        printf("新联系人已成功添加。\n");
    }
    else {
        printf("已取消添加新联系人。\n");
    }
}

void showGroupManagementInterface(ContactRecord contacts[], int *num_contacts)
{
    char* tmp = NULL;
    for (auto it = mp.begin(); it != mp.end(); it++) {
        if (contacts[it->second].name[0] == '$') {

                continue;
         }
        cout<<"组名：" << it->first << endl;
        
        printf("\t\t\t姓名:%s\n", contacts[it->second].name);
        printf("\t\t\t电话:%s\n", contacts[it->second].phone);
        printf("\t\t\t地址:%s\n", contacts[it->second].address);
        printf("\t\t\t邮编:%s\n", contacts[it->second].postcode);
        printf("\t\t\tEmail:%s\n", contacts[it->second].email);
    }
    char a[20] = "";//addGroup把输入和处理合在了一起
    cout << "1.增加分组\n2.删除分组\n3.按其他键返回主菜单\n";
    getchar();
    char op;
    op = getchar();
    if (op == '1')
        addGroup(contacts, num_contacts, a);
    else if (op == '2') {
        cout << "请输入你要删除的组名";
        cin >> a;
        int m=deleteGroup(contacts, num_contacts, a);
        if (m == 0) cout << "你想删除的组不存在\n";
        
        else {
            cout << "已移除" << a << "组内共计" << m << "人";
        }
        cout << "按任意键返回主菜单\n";
        getchar();//吞掉之前的换行
        getchar();//吞掉任意键
    }   
    else
        return;
}

//3.9 显示标签系统界面(拓展)
void showTagSystemInterface(struct ContactRecord contacts[], int num_contacts) {
    char tags[MAX_TAGS] = { 0 };  // 用于记录每个字母是否出现
    int choice;

    // 收集所有唯一的标签（姓名首字母）
    for (int i = 0; i < num_contacts; i++) {
        char firstLetter = toupper(contacts[i].name[0]);
        if (firstLetter >= 'A' && firstLetter <= 'Z') {
            tags[firstLetter - 'A'] = 1;
        }
    }

    while (1) {
        // 显示标签系统界面
        printf("\n标签系统界面 (按姓名首字母)\n");
        printf("============================\n");
        for (int i = 0; i < MAX_TAGS; i++) {
            if (tags[i]) {
                printf("%c ", 'A' + i);
            }
        }
        printf("\n============================\n");
        printf("1. 查看特定标签的联系人\n");
        printf("2. 返回主菜单\n");
        printf("请选择操作: ");

        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            char tag;
            printf("请输入要查看的标签（A-Z）: ");
            scanf(" %c", &tag);
            tag = toupper(tag);

            if (tag >= 'A' && tag <= 'Z') {
                printf("\n标签 '%c' 的联系人：\n", tag);
                int found = 0;
                for (int i = 0; i < num_contacts; i++) {
                    if (toupper(contacts[i].name[0]) == tag) {
                        found = 1;
                        printf("---------------------------\n");
                        printf("姓名: %s\n", contacts[i].name);
                        printf("电话: %s\n", contacts[i].phone);
                        printf("地址: %s\n", contacts[i].address);
                        printf("邮编: %s\n", contacts[i].postcode);
                        printf("邮箱: %s\n", contacts[i].email);
                        printf("---------------------------\n");
                    }
                }
                if (!found) {
                    printf("没有找到以 '%c' 开头的联系人。\n", tag);
                }
            }
            else {
                printf("无效的标签。\n");
            }
            break;
        }
        case 2:
            return;
        default:
            printf("无效的选择，请重新输入。\n");
        }
    }
}


// 声明一个名为num的整型变量
int menuSelect(struct ContactRecord contacts[], int* num_contacts)
{
    string select;
    while (true)
    {
        system("cls");
		showMainInterface();
		cin >> select;
		if (select.size() > 1) {
            cout << "请输入正确的指令\n";
            system("pause");
		}
		else {
			switch (select[0] - '0')
			{
			case 1: //增加
				addContact(contacts, num_contacts);
				break;
			case 2: //查找
				showSearchInterface(contacts, *num_contacts);
				break;
			case 3: //修改
				showEditRecordInterface(contacts, *num_contacts);
				break;
			case 4: //删除
				showDeleteRecordInterface(contacts, num_contacts);
				break;
			case 5: //显示
				listContacts(contacts, *num_contacts);
				break;
			case 6: //分组
				showGroupManagementInterface(contacts, num_contacts);
				break;
			case 0: //退出
				cout << "欢迎下次使用" << endl;
				system("pause");
				return 0;
			default:
				cout << "请输入正确的指令\n";
                system("pause");
				break;
			}
		}
    }
    /*showMainInterface();
    system("pause");*/
    return 0;
}

// 3.2 显示登录界面
/*
负责人:
功能: 显示登录界面，并获取用户输入的用户名和密码。
参数: void
返回值:
    0: 登录成功
    -1: 登录失败
*/
int showLoginInterface() {
    char username[20];
    char password[20];
    printf("\n\t\t\t***********登录界面***********\n");
	while (1) {
		printf("\t\t\t用户名：");
		scanf("%s", username);
		printf("\t\t\t密码：");
		scanf("%s", password);
		/*encrypt(password); */ //对输入的密码进行加密处理，方便后面与存储的密码比较
		if (authenticateUser("user_data.txt", username, password)) {
			printf("\n\t\t\t登录成功！");
			return 0;
		}
		else {
			printf("\n\t\t\t用户名或密码有误，请重新输入！");
            system("pause");
            system("cls");
		}
	}
}



// 3.4 显示编辑记录界面
/*
负责人:
功能: 显示编辑记录界面，允许用户选择要编辑的联系人，并输入新的联系信息。
参数:
contacts: 存放联系人的数组。
num_contacts: 数组中联系人的数量。
返回值: void
*/
void showEditRecordInterface(struct ContactRecord contacts[], int num_contacts) {
    int choice=0;                 //联系人的序号
    
    printf("联系人总数：%d\n", num_contacts);
    for (int i = 0; i < num_contacts; i++) {
        if (contacts[i].name[0] == '$') {
            continue;
            
        }
        else {
            printf("%d.姓名：%s，电话：%s，地址：%s，邮编：%s，邮箱：%s\n", i + 1, contacts[i].name,
            contacts[i].phone, contacts[i].address, contacts[i].postcode, contacts[i].email);
        }
       
    }

    printf("\n\t\t\t请选择需要编辑的联系人（序号）：\n");
    scanf("%d", &choice);

wohuilaile:  //goto标志 末尾choice重新输入

    int index = choice - 1;     //将序号转换为数组索引
    int flag = -1;//作为while循环推出标记 选6后加加
    if (index >= 0 && index < num_contacts) {
        while (1) {
                cout << "请输入你想修改的选项1.姓名   2.电话    3.地址    4.邮编    5.邮箱    6.退出\n";
                int a = -1;
                cin >> a;
                switch (a)
                {
                case 1:
                    printf("\t\t\t输入新姓名：");
                    scanf("%s", contacts[index].name);
                    break;
                case 2:
                    printf("\n\t\t\t输入新电话：");
                    scanf("%s", contacts[index].phone);
                    break;
                case 3:
                    printf("\n\t\t\t输入新地址：");
                    scanf("%s", contacts[index].address);
                    break;
                case 4:
                    printf("\n\t\t\t输入新邮编：");
                    scanf("%s", contacts[index].postcode);
                    break;
                case 5:
                    printf("\n\t\t\t输入新邮箱：");
                    scanf("%s", contacts[index].email);
                    break;
                case 6:
                    flag++;
                    break;
                default:
                    cout << "请输入正确指令\n";
                    break;
                }
                printf("编辑成功！\n");
                if (flag == 0) break;
        }
        
    }
    else {
        printf("没有所选的序号，请重新选择。1.返回主菜单   2.重新选择\n");
        char a = '-1';
        cin >> a;
        if (a == '1') return;
        else {
            cout << "请重新输入序号：";
            scanf("%d", &choice);
            goto wohuilaile;//回溯到上次输入之后
        }
        
    }
}