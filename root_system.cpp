#include <map>
#include <iostream>

#define RANK 5
typedef int root_coeff;

std::map<char, int> const SIMPLE_ROOT_LABELS{
    {'1', 0}, 
    {'2', 1}, 
    {'3', 2}, 
    {'4', 3}, 
    {'5', 4}
};

struct root{
private:
    root_coeff coefficients[RANK] = { 0 };
public:
    inline constexpr root_coeff operator[](int const & index) const;
    inline root_coeff operator[](char const & index) const;

    inline constexpr root& operator+=(root const & rhs);
    friend constexpr root  operator+ (root lhs, root const & rhs);

    inline constexpr root& operator*=(root_coeff const & rhs);
    friend constexpr root  operator* (root lhs, root_coeff const & rhs);
    friend constexpr root  operator* (root_coeff const & lhs, root const & rhs);

    inline constexpr root& operator-=(root const & rhs);
    friend constexpr root  operator- (root lhs, root const & rhs);

    friend std::ostream  & operator<<(std::ostream & stream, root const & r);

    friend constexpr root simple(int const & index);
    friend root simple(char const & index);
};

inline constexpr root_coeff root::operator[](int const & index) const{
    return this->coefficients[index];
}
inline root_coeff root::operator[](char const & index) const{
    return this->coefficients[SIMPLE_ROOT_LABELS.at(index)];
}

inline constexpr root & root::operator+=(root const & rhs){
    for (int i = 0; i < RANK; i++)
        this->coefficients[i] += rhs[i];
    return *this;
}
constexpr root operator+(root lhs, root const & rhs){
    lhs += rhs;
    return lhs;
}

inline constexpr root & root::operator*=(root_coeff const & rhs){
    for (int i = 0; i < RANK; i++)
        this->coefficients[i] *= rhs;
    return *this;
}
constexpr root operator*(root lhs, root_coeff const & rhs){
    lhs *= rhs;
    return lhs;
}
constexpr root operator*(root_coeff const & lhs, root const & rhs){
    return rhs*lhs;
}

inline constexpr root & root::operator-=(root const & rhs){
    *this += (-1)*rhs;
    return *this;
}
constexpr root operator-(root lhs, root const & rhs){
    lhs -= rhs;
    return lhs;
}
std::ostream & operator<<(std::ostream & stream, root const & r){
    for (int i = 0; i < RANK; i++)
        stream << (r[i] < 0 ? '\0' : ' ') << r[i];
    return stream;
}

constexpr root simple(int const & index){
    root r;
    r.coefficients[index] = 1;
    return r;
}
root simple(char const & index){
    root r;
    r.coefficients[SIMPLE_ROOT_LABELS.at(index)] = 1;
    return r;
}

int main(){
    std::cout << "Let's try again..." << std::endl;

    auto r =   simple('1')
           - 2*simple('2')
           + 2*simple('3')
           +   simple('4')
           +   simple('5');

    std::cout << r << std::endl;

    auto s =   simple('1')
           + 2*simple('2')
           + 2*simple('3')
           +   simple('4')
           +   simple('5');

    std::cout << s << std::endl;

    return 0;
}