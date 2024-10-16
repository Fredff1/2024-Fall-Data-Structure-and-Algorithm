#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>  
#include <functional>

using std::vector;
using std::cin,std::cout,std::endl;

template<typename T>
class Heap{
private:
    vector<T> data;
    int size;
    std::function<int(T,T)> compareTo;
    void heapIfUp(int index);
    void heapIfDown(int index);
    void printHeapHelper(int index, int indent) const;
    int getParentNodeIndex(int index) const{
        if(index>=size){
            throw std::out_of_range("Node Index out of range");

        }
        return (index-1)/2;
    }

    int getChildLeftNodeIndex(int index) const{
        if(index>=size){
            //cout<<"Invalid index(The node does not exist)\n";
            return -1;
        }
        if(2*index+1>=size){
            //cout<<"invalid index(The node has no childs)\n";
            return -1;
        }
        return 2*index+1;
    }

    int getChildRightNodeIndex(int index)const{
        if(index>=size){
            //cout<<"Invalid index(The node does not exist)\n";
            return -1;
        }
        if(2*index+2>=size){
            //cout<<"invalid index(The node has no childs)\n";
            return -1;
        }
        return 2*index+2;
    }

    void buildHeap(const vector<T>& target){
        data=target;
        size=target.size();
        for(int index=size/2-1;index>=0;index--){
            heapIfDown(index);
        }
    }
public:
    Heap(const vector<T>& vec) : Heap(vec, [](T a, T b) {
        if (a > b) return 1;
        if (a == b) return 0;
        return -1;
    }) {}

    Heap(const vector<T>& vec, std::function<int(T, T)> comp) : compareTo(comp) {
        buildHeap(vec);
    }

    

    void swap(int index_1,int index_2){
        if(index_1<size&&index_2<size&&index_1>=0&&index_2>=0){
            //cout<<"swap "<<index_1<<" and "<<index_2<<endl;
            std::swap(data[index_1],data[index_2]);
        }else{
            cout<<"Invalid index input "<<index_1<<" and "<<index_2<<endl;
            throw std::invalid_argument("Invalid Argument index_1 and index_2");
        }
    }

    bool isEmpty(){
        return data.empty();
    }

    

    void insert(T value){
        data.push_back(value);
        size++;
        heapIfUp(size-1);
    }

    T extract(){
        if(size<1){
            cout<<"Heap is empty\n";
            return T();
        }
        T rootValue=data[0];
        data[0]=data.back();
        data.pop_back();
        size--;
        heapIfDown(0);
        return rootValue;
    }



    void printHeap(){
        for(const auto&elem:data){
            cout<<elem<<' '<<endl;
        }
    }
    
};

/*Use for a sepcific node to keep its branch valid*/
template<typename T>
void Heap<T>::heapIfUp(int index){
    while(index>0){
        int parentIndex=getParentNodeIndex(index);
        if(compareTo(data[parentIndex],data[index])>=0){
            break;
        }
        // if(data[index]<=data[parentIndex]){
        //     break;
        // }
        this->swap(index,parentIndex);
        index=parentIndex;
    }
}

/*Use for a parent node to keep all its children valid*/
template<typename T>
void Heap<T>::heapIfDown(int index){
    while(index<size){
        int leftChildIndex=getChildLeftNodeIndex(index);
        int rightChildIndex=getChildRightNodeIndex(index);
        int largestIndex=index;
        int compareResultLeft=compareTo(data[leftChildIndex],data[largestIndex]);
        
        if(leftChildIndex>=0&&compareResultLeft>0){
            largestIndex=leftChildIndex;
        }
        int compareResultRIght=compareTo(data[rightChildIndex],data[largestIndex]);
        if(rightChildIndex>=0&&compareResultRIght>0){
            largestIndex=rightChildIndex;
        }
        if(largestIndex==index){
            break;
        }
        this->swap(index,largestIndex);
        index=largestIndex;
    }
}

template<typename T>
void Heap<T>::printHeapHelper(int index, int indent) const {
    if (index < size) {
        /* print right nodes recursively*/
        if (getChildRightNodeIndex(index) != -1) {
            printHeapHelper(getChildRightNodeIndex(index), indent + 4);
        }
        /* indent for current root node*/
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        /* print '/'*/
        if (getChildRightNodeIndex(index) != -1) {
            std::cout << " /\n" << std::setw(indent) << ' ';
        }
        std::cout << data[index] << "\n ";
        /*print '\' and left nodes recursively*/
        if (getChildLeftNodeIndex(index) != -1) {
            std::cout << std::setw(indent) << ' ' << " \\\n";
            printHeapHelper(getChildLeftNodeIndex(index), indent + 4);
        }
    }
}

template<typename T>
class D_Heap{
  private:
    vector<T> data;
    int d;
    int size;
    std::function<int(T,T)> compareTo;
    void buildHeap(const vector<T>& target, const int d){
        data=target;
        size=target.size();
        this->d=d;
        for (int index = (size - 1) / d; index >= 0; index--) {
            heapIfDown(index);
        }
    }
    void heapIfUp(int index);
    void heapIfDown(int index);
  public:
    D_Heap(const vector<T>& vec, const int d) : D_Heap(vec,d , [](T a, T b) {
        if (a > b) return 1;
        if (a == b) return 0;
        return -1;
    }) {}

    D_Heap(const vector<T>& vec, const int d, std::function<int(T, T)> comp) : compareTo(comp) {
        buildHeap(vec,d);
    }

    void swap(int index_1,int index_2){
        if(index_1<size&&index_2<size&&index_1>=0&&index_2>=0){
            //cout<<"swap "<<index_1<<" and "<<index_2<<endl;
            std::swap(data[index_1],data[index_2]);
        }else{
            cout<<"Invalid index input "<<index_1<<" and "<<index_2<<endl;
            throw std::invalid_argument("Invalid Argument index_1 and index_2");
        }
    }

    int getParentNodeIndex(int index) const{
        if(index>=size){
            throw std::out_of_range("Node Index out of range");

        }
        return (index-1)/d;
    }

    int getChildNodeIndex(int indexOfFather,int serialOfChild)const {
        if(indexOfFather>=size){
            throw std::out_of_range("Node Index out of range");
        } else if(serialOfChild>d || serialOfChild<1){
            throw std::invalid_argument("Child Serial out of range.Expect 1<serial<d, get "+std::to_string(serialOfChild));
        }
        return d*indexOfFather+serialOfChild;
    }

    T extract(){
        if(size<1){
            cout<<"Heap is empty\n";
            return T();
        }
        T rootValue=data[0];
        data[0]=data.back();
        data.pop_back();
        size--;
        heapIfDown(0);
        return rootValue;
    }

    void insert(T value){
        data.push_back(value);
        size++;
        heapIfUp(size-1);
    }

    bool validate() const{
        for (int index = 0; index < size; index++) {
            for (int i = 1; i <= d; i++) {
                auto childIndex = getChildNodeIndex(index, i);
                if (childIndex < size) {
                    if (compareTo(data[index], data[childIndex]) < 0) {
                        return false; 
                    }
                }
            }
        }
        return true;
    }

    void changeValueAt(int index, T new_value){
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range");
        }
        if (compareTo(data[index],new_value) >= 0) {
            data[index]=new_value;
            heapIfDown(index);
        }else{
            data[index]=new_value;
            heapIfUp(index);
        }
    }

    T getValueAt(int index){
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range");
        }
        return data[index];
    }

    void printHeap() const{
        for(const auto&value: data){
            cout<<value<<' ';
        }
        cout<<endl;
    }
};

/*Use for a sepcific node to keep its branch valid*/
template<typename T>
void D_Heap<T>::heapIfUp(int index){
    while(index>0){
        int parentIndex=getParentNodeIndex(index);
        if(compareTo(data[parentIndex],data[index])>=0){
            break;
        }
        this->swap(index,parentIndex);
        index=parentIndex;
    }
}

/*Use for a parent node to keep all its children valid*/
template<typename T>
void D_Heap<T>::heapIfDown(int index){
    while(index<size){
        int largestIndex=index;
        for(int i=1;i<=d;i++){
            int child_index=getChildNodeIndex(index,i);
            if(child_index>=size){
                break;
            }
            if (compareTo(data[child_index], data[largestIndex]) > 0) {
                largestIndex = child_index;
            }
        }
        if(largestIndex==index){
            break;
        }
        this->swap(index,largestIndex);
        index=largestIndex;
    }
}



#endif 
