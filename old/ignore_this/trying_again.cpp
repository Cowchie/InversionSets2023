#include <map>
#include <vector>

template<typename T, int N = 0>
struct const_vec{
    T arr[N]{};
    
    constexpr size_t size() const{
        return N;
    }
    constexpr const_vec<T, N+1> push_back(T t) const{
        const_vec<T, N+1> v;
        for (size_t i = 0; i < N; i++)
            v.arr[i] = this->arr[i];
        v.arr[N] = t;
        return v;
    }
    constexpr T operator[](int const & index) const{
        if (index < 0 || index >= N)
            throw std::range_error("const_vec index out of range error");
        return arr[index];
    }
};

template<typename T>
struct const_vec<T, 0>{
    constexpr size_t size() const{
        return 0;
    }
    constexpr const_vec<T, 1> push_back(T const & t) const{
        const_vec<T, 1> v;
        v.arr[0] = t;
        return v;
    }
};

#define RANK 5
typedef int root_coeff;

// std::map<char, int> constexpr SIMPLE_ROOT_LABELS{{'1', 0}, {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}};

struct root{
    root_coeff coefficients[RANK] = { 0 };

    inline constexpr root_coeff operator[](int const & index) const{
        return this->coefficients[index];
    }

    // inline constexpr root_coeff operator[](char const & index) const{
    //     return this->coefficients[SIMPLE_ROOT_LABELS.at(index)];
    // }

    constexpr static root simple(int index){
        root r;
        r.coefficients[index] = 1;
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
};



#include <iostream>

std::ostream & operator<<(std::ostream & stream, root const & r){
    for (int i = 0; i < RANK; i++)
        stream << r[i];
    return stream;
}

template<int k, typename T, int N, int i=0>
struct push_69s_back{
    static constexpr const_vec<T, N+k> result(const_vec<T, N> const & v){
        return push_69s_back<k-1, T, N+1, i+1>::result(v.push_back(i + 69));
    }
};

template<typename T, int N, int i>
struct push_69s_back<0, T, N, i>{
    static constexpr const_vec<T, N> result(const_vec<T, N> const & v){
        return v;
    }
};

int main(){

    auto v = const_vec<int>().push_back(0).push_back(1).push_back(2);

    for (size_t i = 0; i < v.size(); i++){
        std::cout << v[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << v.size() << std::endl;
    int my_array[v.size()];

    auto v2 = push_69s_back<100, int, v.size()>::result(v);

    for (size_t i = 0; i < v2.size(); i++){
        std::cout << v2[i] << std::endl;
    }
    
    int my_array2[v2.size()];

    root constexpr r =    root::simple(0) + 
                        2*root::simple(1) + 
                        2*root::simple(2) + 
                          root::simple(3) + 
                          root::simple(4);



    std::cout << r << std::endl;

    return 0;
}