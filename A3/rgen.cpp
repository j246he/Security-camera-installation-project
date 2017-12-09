// adapted from https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;
int s;
int recorder=0;
/// main() must be declared with arguments
/// otherwise command line arguments are ignored
int mult(int x1,int y1,int x2,int y2,int x3,int y3){
    int mult_num = (x1-x3)*(y2-y3)-(x2-x3)*(y1-y3);
    return mult_num;
}

int main (int argc, char **argv) {
    if(recorder>25){
        cerr<<"Error:over 25 times!"<<endl;
        return 0;
    }
    std::string cvalue;
    int cint_value;
    int c;
    unsigned int num_s = 1;
    unsigned int num_n = 0;
    unsigned int num_l = 0;
    int num_c1;
    int num_c2;
    unsigned int positive=0;
    int num_n_set[50]={0};
    int num_c_set[50][50]={200};
    int cint_values=10;
    int cint_valuen=5;
    int cint_valuel=5;
    int cint_valuec=20;
    char street_name[50][10]={0};

    while(1){
        loop2:
        std::ifstream urandom("/dev/urandom");
        if (urandom.fail()) {
            std::cerr << "Error: cannot open /dev/urandom\n";
            return 1;
        }

        opterr=0;
        num_s=1;
        num_n=0;
        num_l=0;
        num_c1=200;
        num_c2=200;
        positive=0;
        for(int i=0;i<50;++i){
            num_n_set[i]=0;
        }

        for(int i=0;i<50;++i) {
            for (int j = 0; j < 50; ++j) {
                num_c_set[i][j] =200;
            }
        }

        // expected options are '-a', '-b', and '-c value'
        while ((c = getopt(argc, argv, "s:n:l:c:")) != -1) {
            switch (c) {
                case 's':
                    cvalue = optarg;
                    cint_value = atoi(cvalue.c_str());
                    if(cint_value<2){
                        cerr<<"Error:invalid command line."<<endl;
                        return 0;
                    }
                    else{
                        while (num_s < 2 || num_s > cint_value) {
                            urandom.read((char *) &num_s, 1);
                        }
                        cint_values=cint_value;
                        break;
                    }



                case 'n':
                    if(num_s==1){
                        num_s=10;
                    }

                    cvalue = optarg;
                    cint_value = atoi(cvalue.c_str());


                    if(cint_value<1){
                        cerr<<"Error:invalid command line."<<endl;
                        return 0;
                    }
                    else{
                        for(unsigned int i=0;i<num_s;++i) {

                            while (num_n < 2 || num_n > (cint_value+1)) {
                                urandom.read((char *) &num_n, 1);
                            }
                            num_n_set[i]=num_n;
                            num_n=0;
                        }
                        cint_valuen=cint_value;
                        break;
                    }

                case 'l':
                    cvalue = optarg;
                    cint_value = atoi(cvalue.c_str());

                    if(cint_value<5){
                        cerr<<"Error:invalid command line."<<endl;
                        return 0;
                    }
                    else{
                        while (num_l < 5 || num_l > cint_value) {
                            urandom.read((char *) &num_l, 1);
                        }
                        cint_valuel=cint_value;
                        break;
                    }

                case 'c':
                    if(num_s==1){
                        while (num_s < 2 || num_s > 10) {
                            urandom.read((char *) &num_s, 1);
                        }
                    }
                    for(unsigned int i=0;i<num_s;++i) {
                        if (num_n_set[i] == 0) {
                            while (num_n < 2 || num_n > 6) {
                                urandom.read((char *) &num_n, 1);
                            }
                            num_n_set[i]=num_n;
                            num_n=0;
                        }
                    }
                    cvalue = optarg;
                    cint_value = atoi(cvalue.c_str());
                    if(cint_value<1){
                        cerr<<"Error:invalid command line."<<endl;
                        return 0;
                    }
                    else{
                        for(unsigned int i=0;i<num_s;++i){
                            for(int j=0;j<2*num_n_set[i];j=j+2){

                                if (num_c_set[i][j]==200) {
                                    loop1:
                                    while (num_c1 < 0 || num_c1 > cint_value) {
                                        urandom.read((char *) &num_c1, 1);
                                    }
                                    while (positive < 1 || positive > 2) {
                                        urandom.read((char *) &positive, 1);
                                    }
                                    if (positive == 1) {
                                        num_c1 = num_c1 * 1;
                                    } else if (positive == 2) {
                                        num_c1 = num_c1 * (-1);
                                    }
                                    positive = 0;
                                    num_c_set[i][j] = num_c1;
                                    num_c1 = 100;

                                    while (num_c2 < 0 || num_c2 > cint_value) {
                                        urandom.read((char *) &num_c2, 1);
                                    }
                                    while (positive < 1 || positive > 2) {
                                        urandom.read((char *) &positive, 1);
                                    }
                                    if (positive == 1) {
                                        num_c2 = num_c2 * 1;
                                    } else if (positive == 2) {
                                        num_c2 = num_c2 * (-1);
                                    }
                                    positive = 0;
                                    num_c_set[i][j+1] = num_c2;
                                    num_c2 = 100;

                                }


                                for(int k=0;k<j;k=k+2){
                                    if(num_c_set[i][j]==num_c_set[i][k]){
                                        if(num_c_set[i][j+1]==num_c_set[i][k+1]){
                                            recorder++;
                                            goto loop1;
                                        }

                                    }
                                }

                            }
                        }

                        cint_valuec=cint_value;
                        break;
                    }


                default:
                    cerr<<"Error: invalid command line."<<endl;
                    return 0;
            }
        }



        if(num_s==1){
            while (num_s < 2 || num_s > cint_values) {
                urandom.read((char *) &num_s, 1);
            }
        }

        for(unsigned int i=0;i<num_s;++i) {
            if (num_n_set[i] == 0) {
                while (num_n < 2 || num_n > cint_valuen+1) {
                    urandom.read((char *) &num_n, 1);
                }
                num_n_set[i]=num_n;
                num_n=0;
            }
        }
        if(num_l==0){
            num_l=cint_valuel;
        }

        for(unsigned int i=0;i<num_s;++i){
            for(int j=0;j<2*num_n_set[i];j=j+2){

                if (num_c_set[i][j]==200) {
                    loop:
                    while (num_c1 < 0 || num_c1 > cint_valuec) {
                        urandom.read((char *) &num_c1, 1);
                    }
                    while (positive < 1 || positive > 2) {
                        urandom.read((char *) &positive, 1);
                    }
                    if (positive == 1) {
                        num_c1 = num_c1 * 1;
                    }
                    else if (positive == 2) {
                        num_c1 = num_c1 * (-1);
                    }
                    positive = 0;
                    num_c_set[i][j] = num_c1;
                    num_c1 = 100;

                    while (num_c2 < 0 || num_c2 > cint_valuec) {
                        urandom.read((char *) &num_c2, 1);
                    }
                    while (positive < 1 || positive > 2) {
                        urandom.read((char *) &positive, 1);
                    }
                    if (positive == 1) {
                        num_c2 = num_c2 * 1;
                    }
                    else if (positive == 2) {
                        num_c2 = num_c2 * (-1);
                    }
                    positive = 0;
                    num_c_set[i][j+1] = num_c2;
                    num_c2 = 100;

                }

                for(int k=0;k<j;k=k+2){

                    if(num_c_set[i][j]==num_c_set[i][k]){
                        if(num_c_set[i][j+1]==num_c_set[i][k+1]){
                            recorder++;
                            goto loop;
                        }

                    }
                }

            }
        }

        int flag;
        for(unsigned int i=0;i<num_s;++i) {
            if(2*num_n_set[i]>6){
                for (int j = 0; j < 2 * num_n_set[i]; j = j + 2) {
                    for(int k=4;(j+k+3)<2*num_n_set[i];k=k+2){
                        int x1,y1,x2,y2,x3,y3,x4,y4;
                        x1 = num_c_set[i][j];
                        y1 = num_c_set[i][j+1];
                        x2 = num_c_set[i][j+2];
                        y2 = num_c_set[i][j+3];

                        x3 = num_c_set[i][j+k];
                        y3 = num_c_set[i][j+k+1];
                        x4 = num_c_set[i][j+k+2];
                        y4 = num_c_set[i][j+k+3];

                        if (max(x1,x2) < min(x3,x4)){
                            flag=0;
                        }
                        else if (max(y1,y2) < min(y3,y4)){
                            flag=0;
                        }
                        else if (max(x3,x4)<min(x1,x2)){
                            flag=0;
                        }
                        else if (max(y3,y4)<min(y1,y2)){
                            flag=0;
                        }
                        else if ((mult(x3,y3,x2,y2,x1,y1)*mult(x2,y2,x4,y4,x1,y1)<0)){
                            flag=0;
                        }
                        else if ((mult(x1,y1,x4,y4,x3,y3)*mult(x4,y4,x2,y2,x3,y3)<0)){
                            flag=0;
                        }
                        else{
                            flag=1;
                        }

                        if (flag==1){
                            recorder++;
                            goto loop2;
                        }
                    }
                }
            }
        }


        int flag_position=0;
        for(unsigned int i=0;i<num_s;++i) {
            for(int k=i+1;k<num_s;++k){
                if(num_n_set[i]==num_n_set[k]){
                    for (int j = 0; j < 2*num_n_set[i]; ++j) {
                        if (num_c_set[i][j]!=num_c_set[k][j]){
                            flag_position=0;
                            break;
                        }
                        else{
                            flag_position=1;
                        }
                    }
                }
            }
        }

        if (flag_position==1){
            recorder++;
            goto loop2;
        }

        char street_name1=0;
        char street_name2=0;
        char street_name3=0;
        char street_name4=0;
        char street_name5=0;

        if(street_name[0][0]!=0){
            for(int i=0;i<s;++i){
                cout<<"r"<<" "<<'"'<<street_name[i][0]<<street_name[i][1]<<street_name[i][2]<<street_name[i][3]<<street_name[i][4]<<'"'<<endl;
            }
        }


        for(unsigned int i=0;i<num_s;++i) {
            cout<<"a"<<' ';
            while (street_name1 < 'A' || (street_name1 > 'Z'&&street_name1 < 'a') || street_name1 > 'z') {
                urandom.read(&street_name1, 1);
            }
            while (street_name2 < 'a' || street_name2 > 'z') {
                urandom.read(&street_name2, 1);
            }
            while (street_name3 < 'a' || street_name3 > 'z') {
                urandom.read(&street_name3, 1);
            }
            while (street_name4 < 'a' || street_name4 > 'z') {
                urandom.read(&street_name4, 1);
            }
            while (street_name5 < 'a' || street_name5 > 'z') {
                urandom.read(&street_name5, 1);
            }

            cout<<'"'<<street_name1<<street_name2<<street_name3<<street_name4<<street_name5<<'"'<<' ';
            for (int j = 0; j < 2*num_n_set[i]; j=j+2) {
                std::cout << '('<<num_c_set[i][j] <<','<<num_c_set[i][j+1]<<')';
            }
            street_name[i][0]=street_name1;
            street_name[i][1]=street_name2;
            street_name[i][2]=street_name3;
            street_name[i][3]=street_name4;
            street_name[i][4]=street_name5;
            street_name1=0;
            street_name2=0;
            street_name3=0;
            street_name4=0;
            street_name5=0;
        }
        urandom.close();
        cout<<'g'<<endl;
        fflush(stdout);

        s=num_s;
        sleep(num_l);
    }

    return 0;
}
