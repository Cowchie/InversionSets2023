#include <map>
#include <algorithm>

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

private:
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
public:
    root roots[4*RANK*RANK];
public:
    int const root_count = 0;

public:
    inline root_coeff coeff(int const & index, root const & r) const{
        return r.coefficients[index];
    }
    inline root_coeff coeff(char const & symbol, root const & r) const{
        return r.coefficients[SIMPLE_ROOT_LABELS.at(symbol)];
    }

    constexpr root simple(int const & index) const{
        root r;
        r.coefficients[index] = 1;
        return r;
    }
    root simple(char const & symbol) const{
        root r;
        r.coefficients[SIMPLE_ROOT_LABELS.at(symbol)] = 1;
        return r;
    }
public:
    inline root E1minusE(int const & j) const{
        root r;
        for (size_t i = 1; i < j; i++){
            r += simple(int(i-1));
        }
        return r;
        return j<2 ? root() : simple(j) + E1minusE(j-1);
    }

    inline root E1plusE(int const & j) const{
        return E1minusE(RANK) + E1minusE(RANK-1) + simple(RANK-1) - E1minusE(j);
    }

    int generate_roots(char const & type){
        switch (type){
        case 'A':
            return generate_roots_A();
        case 'D':
            return generate_roots_D();
        default:
            return 0;
        }
    }

    int generate_roots_A(){
        size_t index = 0;
        for (size_t i = 1; i <= RANK; i++){                 // 0 < i <= rank
            for (size_t j = i+1; j <= RANK+1; j++){         // i < j <= rank+1
                roots[index] = E1minusE(j) - E1minusE(i);   // Ei - Ej
                index++;
            }
        }
        // std::sort(std::begin(roots), std::begin(roots) + index);
        // for (size_t i = 1; i <= rank; i++){
        //     simple_inds.push_back(valid(simple(i)));
        // }
        return index;
    }

    int generate_roots_D(){
        size_t index = 0;
        for (size_t i = 1; i <= RANK-1; i++){               // 0 < i <= rank
            for (size_t j = i+1; j <= RANK; j++){           // i < j <= rank+1
                roots[index] = E1minusE(j) - E1minusE(i);   // Ei - Ej
                index++;
                roots[index] =  E1plusE(i) - E1minusE(j);   // Ei + Ej
                index++;
            }
        }
        // std::sort(std::begin(roots), std::begin(roots) + index);
        // for (size_t i = 1; i <= rank; i++){
        //     simple_inds.push_back(valid(simple(i)));
        // }
        return index;
    }

public:
    root_system(char const & type) : 
        root_count(generate_roots(type)) 
    {};

public:
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

    auto Delta = root_system<5>('D');

    auto r =   Delta.simple('1')
           + 2*Delta.simple('2')
           + 2*Delta.simple('3')
           +   Delta.simple('4')
           +   Delta.simple('5');
    std::cout << r << std::endl;
    std::cout << std::endl;

    std::cout << Delta.E1plusE(2) << std::endl;
    std::cout << Delta.E1plusE(3) << std::endl;
    std::cout << Delta.E1plusE(4) << std::endl;
    std::cout << Delta.E1plusE(5) << std::endl;
    std::cout << std::endl;


    std::cout << Delta.root_count << std::endl;
    for (size_t i = 0; i < Delta.root_count; i++){
        std::cout << Delta.roots[i] << std::endl;
    }
    

    return 0;
}