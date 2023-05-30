#include <iostream>
#include <vector>
#include <string>


class A{
public:
    class B{
        friend A;
    private:
        int val = 6;
    };

    int func(B b){
        return b.val;
    }
};
// int func(A::B b){
//     return b.val;
// }

int main()
{
    std::vector<std::string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const std::string& word : msg){
        std::cout << word << " ";
    }
    std::cout << std::endl;
}