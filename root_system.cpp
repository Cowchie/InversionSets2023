#include <map>
#include <bitset>
#include <algorithm>

#include <iostream>

typedef short root_coeff_t;
// #define RANK 5

template<int RANK>
class root_system{
public:
    std::map<char, int> root_label_map{
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
        root_coeff_t coefficients[RANK] = { 0 };
    public:
        // inline constexpr root_coeff_t operator[](int const & index) const;
        // inline           root_coeff_t operator[](char const & symbol) const;

        inline constexpr root& operator+=(root const & rhs);
        // friend constexpr root  operator+ (root lhs, root const & rhs);

        inline constexpr root& operator*=(root_coeff_t const & rhs);
        // friend constexpr root  operator* (root lhs, root_coeff_t const & rhs);
        // friend constexpr root  operator* (root_coeff_t const & lhs, root const & rhs);

        inline constexpr root& operator-=(root const & rhs);
        // friend constexpr root  operator- (root lhs, root const & rhs);

        // friend constexpr root simple(int const & index);
        // friend           root simple(char const & symbol);

        // friend constexpr root_coeff_t hgt(root const & r);

        // friend constexpr bool operator<(root const & lhs, root const & rhs);

        inline constexpr bool operator!=(root const & rhs);
    };
public:
    root roots[4*RANK*RANK];
public:
    size_t const root_count = 0;
    root simple_indices[RANK];

public:
    inline root_coeff_t coeff(int const & index, root const & r) const{
        return r.coefficients[index];
    }
    inline root_coeff_t coeff(char const & symbol, root const & r) const{
        return r.coefficients[root_label_map.at(symbol)];
    }

    constexpr root simple(int const & index) const{
        root r;
        r.coefficients[index] = 1;
        return r;
    }
    root simple(char const & symbol) const{
        root r;
        r.coefficients[root_label_map.at(symbol)] = 1;
        return r;
    }
    
    constexpr root_coeff_t hgt(root const & r){
        root_coeff_t sum = 0;
        for (size_t i = 0; i < RANK; i++){
            sum += r.coefficients[i];
        }
        return sum;
    }
    constexpr bool root_compare(root const & lhs, root const & rhs){
        if(hgt(lhs) != hgt(rhs))
            return hgt(lhs) < hgt(rhs);

        for (size_t i = 0; i < RANK; i++){
            if (lhs.coefficients[i] >= rhs.coefficients[i])
                return false;
        }
        return true;
    }
private:
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

private:
    constexpr size_t generate_roots(char const & type){
        switch (type){
        case 'A':
            return generate_roots_A();
        case 'D':
            return generate_roots_D();
        default:
            return 0;
        }
    }

    inline size_t generate_roots_A(){
        size_t index = 0;
        for (size_t i = 1; i <= RANK; i++){                 // 0 < i <= rank
            for (size_t j = i+1; j <= RANK+1; j++){         // i < j <= rank+1
                roots[index] = E1minusE(j) - E1minusE(i);   // Ei - Ej
                index++;
            }
        }
        // std::sort(std::begin(roots), std::begin(roots) + index);
        return index;
    }

    inline size_t generate_roots_D(){
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
        return index;
    }
public:

    int valid(root const a){
        auto iter = std::lower_bound(
                       std::begin(roots), 
                       std::end(roots), 
                       a,
                       root_compare
                    );
        if (iter >= std::end(roots))
            return -1;
        if (*iter != a)
            return -1;
        return iter - std::begin(roots);
    }

public:
    root_system(char const & type) : 
        root_count(generate_roots(type)) 
    {
        std::sort(std::begin(roots), std::end(roots), root_compare);

        root_label_map.clear();
        for (int i = 0; i < RANK; i++)
            root_label_map['1'+i] = i;
    };

public:
    friend constexpr root operator+(root lhs, root const & rhs){
        lhs += rhs;
        return lhs;
    }
    friend constexpr root operator*(root lhs, root_coeff_t const & rhs){
        lhs *= rhs;
        return lhs;
    }
    friend constexpr root operator*(root_coeff_t const & lhs, root const & rhs){
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


template<int RANK>
inline constexpr auto root_system<RANK>::root::operator+=(root_system<RANK>::root const & rhs) -> root_system<RANK>::root &{
    for (int i = 0; i < RANK; i++)
        this->coefficients[i] += rhs.coefficients[i];
    return *this;
}

template<int RANK>
inline constexpr auto root_system<RANK>::root::operator*=(root_coeff_t const & rhs) -> root_system<RANK>::root &{
    for (int i = 0; i < RANK; i++)
        this->coefficients[i] *= rhs;
    return *this;
}

template<int RANK>
inline constexpr auto root_system<RANK>::root::operator-=(root_system<RANK>::root const & rhs) -> root_system<RANK>::root &{
    *this += (-1)*rhs;
    return *this;
}

template<int RANK>
inline constexpr auto root_system<RANK>::root::operator!=(root_system<RANK>::root const & rhs) -> bool{
    for (size_t i = 0; i < RANK; i++){
        if (this->coefficients[i] != rhs.coefficients[i])
            return false;
    }
    return true;
}

int main(){
    std::cout << "Let's try again..." << std::endl;
    std::cout << std::endl;

    root_system<7> const Delta('D');

    for (auto [k, v]: Delta.root_label_map){
        std::cout << "char: " << k << ", int: " << v << std::endl;
    }
    std::cout << std::endl;

    // auto b = std::bitset<Delta.root_count>();

    auto r =   Delta.simple('1')
           + 2*Delta.simple('2')
           + 2*Delta.simple('3')
           + 2*Delta.simple('4')
           + 2*Delta.simple('5')
           +   Delta.simple('6')
           +   Delta.simple('7');
    std::cout << r << std::endl;
    std::cout << std::endl;


    std::cout << Delta.root_count << std::endl;
    for (size_t i = 0; i < Delta.root_count; i++){
        std::cout << Delta.roots[i] << std::endl;
    }
    std::cout << std::endl;

    // std::cout << "Is r a root? " << Delta.valid(r) << std::endl;
    

    return 0;
}