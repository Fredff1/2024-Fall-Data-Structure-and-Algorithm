#include "Matrix.h"
#include "Utilility.h"

void test_effeciency(int matrix_size,int begin_num,int end_num,int change_len){
    cout<<"-----------------------------------------------------------------\n";
    cout<<"Test scale: "<<matrix_size<<endl;
    Matrix<int> matrix_1=Matrix<int>::getRandomMatrix(matrix_size,matrix_size,begin_num,end_num);
    Matrix<int> matrix_2=Matrix<int>::getRandomMatrix(matrix_size,matrix_size,begin_num,end_num);
    
    auto start = std::chrono::high_resolution_clock::now();

    auto matrix_reuslt_1=matrix_1*matrix_2;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    cout<<"Normal Algorithm took "<<duration.count()<<" seconds"<<endl;

    start = std::chrono::high_resolution_clock::now();

    auto matrix_reuslt_2=matrix_1.Strassen_mutipliaction(matrix_2,change_len);
    end = std::chrono::high_resolution_clock::now();

    duration = end - start;
    cout<<"Merged algorithm took "<<duration.count()<<" seconds"<<endl;
    cout<<"Strassen algorithm change to normal algorithm at size "<<change_len<<endl;
    if(matrix_reuslt_1==matrix_reuslt_2){
        cout<<"The results of two algorithms are same\n";
    }
    cout<<"-----------------------------------------------------------------\n";
    start = std::chrono::high_resolution_clock::now();

    auto matrix_reuslt_3=matrix_1.Strassen_mutipliaction(matrix_2,change_len/2);
    end = std::chrono::high_resolution_clock::now();

    duration = end - start;
    cout<<"Merged algorithm took "<<duration.count()<<" seconds"<<" and ";
    cout<<"strassen algorithm change to normal algorithm at size "<<change_len/2<<endl;
    if(matrix_reuslt_1==matrix_reuslt_3){
        cout<<"The results of two algorithms are same\n";
    }

    
    cout<<"-----------------------------------------------------------------\n";
}


int main(){
    int sizes_to_exam[]={10,20,64,128,150,256,512,1025,2048,4096};
    for(auto& size:sizes_to_exam){
        test_effeciency(size,1,15,size/4);
    }
    return 0;
}