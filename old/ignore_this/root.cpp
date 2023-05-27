#include <bitset>
#include <vector>
#include <list>

template<int Rank>
    // requires (Rank > 0)
struct root{
public:
    int coords[Rank];
    float inner_products[Rank];
    std::bitset<Rank> support = 0;
    // std::bitset<Rank> removable_simples = std::bitset<Rank>().set();
    // std::bitset<Rank> addable_simples = std::bitset<Rank>().set();

};

#define RANK 5

std::vector<root<RANK>> simples;


template<int index>
int get_component(root<RANK> r){
    return r.coords[index];
}

    // requires (index >= 0 && index < RANK)
root<RANK> increment_component(int index, root<RANK> r){  
    r.coords[index] += 1;
    for (int i = 0; i < RANK; i++){
        r.inner_products[i] += simples[index].inner_products[i];
    }
    r.support[index] = true;
    return r; 
}




#include <iostream>
#include <string>
#include <sstream>
inline std::string print_root(root<RANK> a){
    std::ostringstream s;
    s << "Coords: ";
    for (int i = 0; i < RANK; i++){
        std::cout << a.coords[i];
    }
    std::cout << ", inner_products: ";
    for (int i = 0; i < RANK; i++){
        std::cout << a.inner_products[i] << ",";
    }
    std::cout << " support: ";
    std::cout << a.support << std::endl;
    return s.str();
}

int main(){
    std::cout << "Testing root.cpp" << std::endl;

    // Generating A type simple roots
    {
        auto r = root<RANK>();
        r.coords[0] = 1;
        r.inner_products[0] = 2; 
        r.inner_products[0+1] = -1; 
        r.support[0] = true;
        simples.push_back(r);
    }
    for (int i = 1; i < RANK-1; i++){
        auto r = root<RANK>();
        r.coords[i] = 1;
        r.inner_products[i] = 2;
        r.inner_products[i-1] = -1; 
        r.inner_products[i+1] = -1; 
        r.support[i] = true;
        simples.push_back(r);
    }{
        auto r = root<RANK>();
        r.coords[RANK-1] = 1;
        r.inner_products[RANK-1] = 2;
        r.inner_products[RANK-1-1] = -1;  
        r.support[RANK-1] = true;
        simples.push_back(r);
    }

    auto zero = root<RANK>();
    std::list<root<RANK>> delta_plus;
    std::list<root<RANK>> roots_to_check;

    for (int i = 0; i < RANK; i++){
        auto new_r = increment_component(i, zero);
        delta_plus.push_back(new_r);
        roots_to_check.push_back(new_r);
    }
    while (!roots_to_check.empty()){
        auto r = roots_to_check.back();
        roots_to_check.pop_back();
        for (int i = 0; i < RANK; i++){
            if (r.inner_products[i] >= 0){
                continue;
            }
            auto new_r = increment_component(i, r);
            delta_plus.push_back(new_r);
            roots_to_check.push_back(new_r);
        }
    }
    
    std::cout << "delta_plus" << std::endl;
    for (auto r : delta_plus){
        std::cout << "  " << print_root(r) << std::endl;
    }
    
    

    return 1;
}
