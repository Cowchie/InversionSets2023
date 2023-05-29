#include <map>

#define RANK 5
typedef int root_coeff;

std::map<char, int> const SIMPLE_ROOT_LABELS{{'1', 0}, {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}};

struct root{
    root_coeff coefficients[RANK] = { 0 };

    inline constexpr root_coeff operator[](int const & index) const{
        return this->coefficients[index];
    }

    inline root_coeff operator[](char const & index) const{
        return this->coefficients[SIMPLE_ROOT_LABELS.at(index)];
    }

    constexpr static root simple(int const & index){
        root r;
        r.coefficients[index] = 1;
        return r;
    }

    static root simple(char const & index){
        root r;
        r.coefficients[SIMPLE_ROOT_LABELS.at(index)] = 1;
        return r;
    } 

    inline constexpr root & operator+=(root const & rhs){
        for (int i = 0; i < RANK; i++)
            this->coefficients[i] += rhs[i];
        return *this;
    }
    friend constexpr root operator+(root lhs, root const & rhs){
        lhs += rhs;
        return lhs;
    }

    inline constexpr root & operator*=(root_coeff const & rhs){
        for (int i = 0; i < RANK; i++)
            this->coefficients[i] *= rhs;
        return *this;
    }
    friend constexpr root operator*(root lhs, root_coeff const & rhs){
        lhs *= rhs;
        return lhs;
    }
    friend constexpr root operator*(root_coeff const & lhs, root const & rhs){
        return rhs*lhs;
    }

    inline constexpr root & operator-=(root const & rhs){
        *this += (-1)*rhs;
        return *this;
    }
    friend constexpr root operator-(root lhs, root const & rhs){
        lhs -= rhs;
        return lhs;
    }
};



#include <iostream>

std::ostream & operator<<(std::ostream & stream, root const & r){
    for (int i = 0; i < RANK; i++){
        if (r[i] >= 0) 
            stream << ' ' << r[i];
        else
            stream << r[i];
    }
    return stream;
}


int main(){
    std::cout << "Let's try again..." << std::endl;

    auto r =   root::simple('1')
           - 2*root::simple('2')
           + 2*root::simple('3')
           +   root::simple('4')
           +   root::simple('5');

    std::cout << r << std::endl;

    auto s =   root::simple('1')
           + 2*root::simple('2')
           + 2*root::simple('3')
           +   root::simple('4')
           +   root::simple('5');

    std::cout << s << std::endl;

    return 0;
}