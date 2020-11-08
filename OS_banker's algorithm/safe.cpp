#include <iostream>
#include <fstream>
#include<string>
#include<sstream>
using namespace std;


int allocation[10][10];
int max[10][10];
int available[10];
int need[10][10];
int request[10][10];

int work[10];
bool finish[10] = {false};

int resource_num;
int process_num;

int available_temp[10];
int allocation_temp[10];
int need_temp[10];

//allocation.txt�� ���� �� process ������ ���� �Լ�
//������ ���� 10���� �ݺ����� �ʰ� ���� ����Ƚ���� ����
void get_process_size() {
    ifstream readFile;

    readFile.open("allocation.txt");
        string line;
        while (!readFile.eof())
        {
            getline(readFile, line);
            process_num++;
        }
        readFile.close();


    }

//available array�� �а�  resource ������ ����
void read_available() {
    ifstream readFile;
    readFile.open("available.txt");
    if (readFile.is_open())
    {
            string line;
            getline(readFile, line);
            istringstream ss(line);
            string stringBuffer;
            while (getline(ss, stringBuffer, ' '))
            {
                stringstream tem(stringBuffer);
                tem >> available[resource_num];
                resource_num++;
            }
    }
        readFile.close();
}

// 2���� �迭 ������ �д� �Լ�
void read_file(string x, int arr[][10]) {
    ifstream readFile;

    readFile.open(x);
    if (readFile.is_open())
    {
        int i = 0;
        int j = 0;
        while (!readFile.eof())
        {
            string line;
            getline(readFile, line);
            istringstream ss(line);
            string stringBuffer;
            while (getline(ss, stringBuffer, ' '))
            {
                stringstream tem(stringBuffer);
                tem >> arr[i][j];
                j++;
            }
            j = 0;
            i++;
        }
        readFile.close();
    }

}


bool check_need_work(int i) {
    for (int j = 0; j < resource_num; j++) {
        if(need[i][j]> work[j]){
            return false;
        }
    }
    return true;

}

bool is_safe() {
    //step1
    for (int j=0; j < resource_num; j++) {
        work[j] = available[j];
    }
    for (int i = 0; i < process_num; i++) {
        finish[i] = { false };
    }
    
    //step2,3
    for (int i = 0; i < process_num; i++) {
        if (finish[i] == false && check_need_work(i)) {
            for (int j = 0; j < resource_num; j++) {
                work[j] = work[j] + allocation[i][j];
            }
                finish[i] = true;
                i = -1;
            }
    }

    //step 4
        for (int i = 0; i < process_num; i++)
        {
            if (finish[i]==false) {
                return false;
            }
        }
        return true;
}


bool is_granted(int i) {
    //step1
    for (int j=0; j < resource_num; j++) {
        if (request[i][j] > need[i][j])
        {
            //�߸��� ��û�̹Ƿ� request�� 0 ���� ����
            for (int k = 0; k < resource_num; k++) {
                request[i][k] = 0;
            }
            return false;
        }
    }
    //step2
    for (int j=0; j < resource_num; j++) {
        if (request[i][j] > available[j]) {
            // ��û�� ��밡���Ѱͺ��� �����Ƿ� wait
            return false;
        }

    }
    //step3
    for (int j = 0; j < resource_num; j++) {
        // ��û�� �޾Ƶ鿩���� ������ �ǵ������ϹǷ� temp�� �����ص�
        available_temp[j] = available[j];
        available[j] = available[j] - request[i][j];
        
        allocation_temp[j] = allocation[i][j];
        allocation[i][j] = allocation[i][j] + request[i][j];

        need_temp[j] = need[i][j];
        need[i][j] = need[i][j] - request[i][j];
    }
    //��ù޾Ƶ鿩��
    if (is_safe()) {
        return true;
    }
    //unsafe -> temp�� �����صа��� ����, wait
    for (int j = 0; j < resource_num; j++) {
        available[j]= available_temp[j];
        allocation[i][j]= allocation_temp[j];
        need[i][j] = need_temp[j];
    }
    return false;
    
}

int main(){
    get_process_size();
    read_available();
    read_file("allocation.txt", allocation);
    read_file("max.txt", max);

    for (int i = 0; i < process_num; i++)
    {
        for (int j = 0; j < resource_num; j++)

            need[i][j] = max[i][j] - allocation[i][j];

    }

    if (is_safe()) {
        printf("Safe state\n");
    }
    else {
        printf("Unsafe state\n");
    }

    

    int request_index;
    
    while (1) {
        printf("\nIs there any request? \nIf yes, put the index of process which sends the request. If no, just put '-1'\n");
        printf("index:");
        cin >> request_index;
        if (request_index == -1) {
            return 0;
        }
        printf("put the request. (ex:A->1 B->2 C->3 D->0  then just put '1 2 3 0 ')\n:");
        for (int j=0; j < resource_num; j++) {
            cin >> request[request_index][j];
        }
        if (is_granted(request_index)) {
            printf("Granted\n");
        }
        else {
            printf("Not Available\n");
        }
    }
}
