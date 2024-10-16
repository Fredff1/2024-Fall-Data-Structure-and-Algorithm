#include "Heap.h"
#include "Utilility.h"

void testCorrect(){
    auto vec_correct=RandomGenerator::getRandomVector<int>(20,0,50);
    D_Heap<int> heap_correct=D_Heap<int>(vec_correct,4);
    bool flag=heap_correct.validate();
    heap_correct.printHeap();
    if(flag){
        cout<<"Correct validate function\n";
    }else{
    }
    heap_correct.extract();
    flag=heap_correct.validate();
    if(flag){
        cout<<"Correct extract\n";
    }
    heap_correct.insert(40);
    flag=heap_correct.validate();
    if(flag){
        cout<<"Correct insert\n";
    }
    heap_correct.changeValueAt(10,200);
    flag=heap_correct.validate();
    if(flag){
        cout<<"Correct increase-key\n";
    }
    heap_correct.changeValueAt(3,-10);
    flag=heap_correct.validate();
    if(flag){
        cout<<"Correct decrease-key\n";
    }

}

void testEffeciency(int size,int d){
    auto vec_eff=RandomGenerator::getRandomVector<int>(size,0,size);
    D_Heap<int> heap_eff=D_Heap<int>(vec_eff,d);
    bool flag=heap_eff.validate();
    cout<<"---------------------------------------------------------------------------------------------------------------\n";
    cout<<"Current Size : "<<size<<endl;
    cout<<"Current d :"<<d<<endl;
    cout<<endl<<endl;
    if(flag){
        cout<<"Correct validate function\n";
    }else{
        throw std::runtime_error("Invalid validate");
    }
    auto start = std::chrono::high_resolution_clock::now();
    heap_eff.extract();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    flag=heap_eff.validate();

    if(flag){
        cout<<"Correct extract took time "<<duration.count()<<endl;
        
    }else{
        throw std::runtime_error("Invalid extract");
    }



    start = std::chrono::high_resolution_clock::now();
    heap_eff.insert(40);
    end = std::chrono::high_resolution_clock::now();
    flag=heap_eff.validate();
    duration = end - start;
    if(flag){
        cout<<"Correct insert took time "<<duration.count()<<endl;
    }else{
        throw std::runtime_error("Invalid insert");
    }



    start = std::chrono::high_resolution_clock::now();
    heap_eff.changeValueAt(size/3,heap_eff.getValueAt(size/3)+2);
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    flag=heap_eff.validate();
    if(flag){
        cout<<"Correct increase-key took time "<<duration.count()<<endl;;
    }else{
        throw std::runtime_error("Invalid increase-key");
    }

    cout<<"---------------------------------------------------------------------------------------------------------------\n";


}

int main(){
    testCorrect();
    int target_size[]={50,100,500,5000,50000,200000};
    int target_d[]= {2,5,10,20,50,500};
    for(auto&size:target_size){
        for(auto&d:target_d){
            if(d<size){
                testEffeciency(size,d);  
            }
            
        }
    }

    return 0;
}