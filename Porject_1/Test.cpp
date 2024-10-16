#include "Utilility.h"

int main(){
    int first=RandomGenerator::getRandomValue<int>(1,3);
    int second=RandomGenerator::getRandomValue<int>(1,3);
    int third=RandomGenerator::getRandomValue<int>(1,3);
    cout<<"first "<<first<<" second "<<second<<"third "<<third<<endl;
}