#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    time_t vrijeme;
    time(&vrijeme);
    std::cout<<ctime(&vrijeme)<<std::endl;
    
}