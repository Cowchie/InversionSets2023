#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <functional>

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

// Testing an O(RANK) compile-time implementation of map
// This works specificaly for my use-case. But I don't like it...

template<int n, char gen(int)>
struct const_map{
private:
    char keys[n]  = { 0 };
private:
    inline constexpr void instantiate_keys(){
        for (int i = 0; i < n; i++)
            keys[i] = gen(i);
    }
public:
    constexpr const_map(){
        instantiate_keys();
    }
    constexpr std::optional<int> operator[](char k) const{
        for (int i = 0; i < n; i++){
            if (keys[i] == k){
                return i;
            }
        }
        return std::nullopt;
    }
    constexpr size_t size() const{
        return n;
    }
};

constexpr char f(int index){ return '1' + index; }; 

int main()
{
    std::vector<std::string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    constexpr auto map_test = const_map<7, f>();
    std::cout << map_test.size() << std::endl;

    std::cout << map_test['1'].value_or(-1) << std::endl;
    std::cout << map_test['2'].value_or(-1) << std::endl;
    std::cout << map_test['3'].value_or(-1) << std::endl;
    std::cout << map_test['4'].value_or(-1) << std::endl;
    std::cout << map_test['5'].value_or(-1) << std::endl;
    std::cout << map_test['6'].value_or(-1) << std::endl;
    std::cout << map_test['7'].value_or(-1) << std::endl;
    std::cout << map_test['8'].value_or(-1) << std::endl;

    for (const std::string& word : msg){
        std::cout << word << " ";
    }
    std::cout << std::endl;

    return 0;
}