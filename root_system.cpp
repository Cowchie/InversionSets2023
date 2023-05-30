#include <map>
#include <iostream>

typedef int root_coeff;
// #define RANK 5

template<int RANK>
class root_system{
public:
    std::map<char, int> SIMPLE_ROOT_LABELS{
        {'1', 0}, 
        {'2', 1}, 
        {'3', 2}, 
        {'4', 3}, 
        {'5', 4}
    };

    struct root{
        friend root_system<RANK>;
    private:
        root_coeff coefficients[RANK] = { 0 };
    public:
        // inline constexpr root_coeff operator[](int const & index) const;
        // inline           root_coeff operator[](char const & symbol) const;

        inline constexpr root& operator+=(root const & rhs);
        // friend constexpr root  operator+ (root lhs, root const & rhs);

        inline constexpr root& operator*=(root_coeff const & rhs);
        // friend constexpr root  operator* (root lhs, root_coeff const & rhs);
        // friend constexpr root  operator* (root_coeff const & lhs, root const & rhs);

        inline constexpr root& operator-=(root const & rhs);
        // friend constexpr root  operator- (root lhs, root const & rhs);

        // friend constexpr root simple(int const & index);
        // friend           root simple(char const & symbol);
    };

    inline root_coeff coeff(int const & index, root const & r) const{
        return r.coefficients[index];
    }
    inline root_coeff coeff(char const & symbol, root const & r) const{
        return r.coefficients[SIMPLE_ROOT_LABELS.at(symbol)];
    }

    constexpr root simple(int const & index){
        root r;
        r.coefficients[index] = 1;
        return r;
    }
    root simple(char const & symbol){
        root r;
        r.coefficients[SIMPLE_ROOT_LABELS.at(symbol)] = 1;
        return r;
    }

    friend constexpr root operator+(root lhs, root const & rhs){
        lhs += rhs;
        return lhs;
    }
    friend constexpr root operator*(root lhs, root_coeff const & rhs){
        lhs *= rhs;
        return lhs;
    }
    friend constexpr root operator*(root_coeff const & lhs, root const & rhs){
        return rhs*lhs;
    }
    friend constexpr root operator-(root lhs, root const & rhs){
        lhs -= rhs;
        return lhs;
}

    friend std::ostream  & operator<<(std::ostream & stream, root const & r){
        for (int i = 0; i < RANK; i++)
            stream << (r.coefficients[i] < 0 ? '\0' : ' ') << r.coefficients[i];
        return stream;
    }
};

// inline constexpr root_coeff root::operator[](int const & index) const{
//     return this->coefficients[index];
// }
// inline root_coeff root::operator[](char const & symbol) const{
//     return this->coefficients[SIMPLE_ROOT_LABELS.at(symbol)];
// }

template<int RANK>
inline constexpr auto root_system<RANK>::root::operator+=(root_system<RANK>::root const & rhs) -> root_system<RANK>::root &{
    for (int i = 0; i < RANK; i++)
        this->coefficients[i] += rhs.coefficients[i];
    return *this;
}
// template<int RANK>
// constexpr root_system<RANK>::root::operator+(root_system<RANK>::root lhs, root_system<RANK>::root const & rhs){
//     lhs += rhs;
//     return lhs;
// }

template<int RANK>
inline constexpr auto root_system<RANK>::root::operator*=(root_coeff const & rhs) -> root_system<RANK>::root &{
    for (int i = 0; i < RANK; i++)
        this->coefficients[i] *= rhs;
    return *this;
}

template<int RANK>
inline constexpr auto root_system<RANK>::root::operator-=(root_system<RANK>::root const & rhs) -> root_system<RANK>::root &{
    *this += (-1)*rhs;
    return *this;
}

// template<int RANK>
// constexpr root_system<RANK>::root operator*(root_system<RANK>::root lhs, root_coeff const & rhs){
//     lhs *= rhs;
//     return lhs;
// }
// constexpr root_system<RANK>::root operator*(root_coeff const & lhs, root_system<RANK>::root const & rhs){
//     return rhs*lhs;
// }

// template<int RANK>
// inline constexpr root_system<RANK>::root & root_system<RANK>::root::operator-=(root_system<RANK>::root const & rhs){
//     *this += (-1)*rhs;
//     return *this;
// }
// constexpr root_system<RANK>::root operator-(root_system<RANK>::root lhs, root_system<RANK>::root const & rhs){
//     lhs -= rhs;
//     return lhs;
// }

// template<int RANK>
// std::ostream & operator<<(
//                         std::ostream & stream, 
//                         typename root_system<RANK>::root const & r
// ){
//     for (int i = 0; i < RANK; i++)
//         stream << (r[i] < 0 ? '\0' : ' ') << r[i];
//     return stream;
// }

int main(){
    std::cout << "Let's try again..." << std::endl;

    auto Delta = root_system<5>();

    auto r =   Delta.simple('1')
           - 2*Delta.simple('2')
           + 2*Delta.simple('3')
           +   Delta.simple('4')
           +   Delta.simple('5');


    std::cout << r << std::endl;

    auto s =   Delta.simple('1');
           + 2*Delta.simple('2')
           + 2*Delta.simple('3')
           +   Delta.simple('4')
           +   Delta.simple('5');

    std::cout << s << std::endl;

    return 0;
}