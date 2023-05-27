#include <iostream>
#include <bitset>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
// Simple root vector operations
////////////////////////////////////////////////////////////////////////////////
typedef int root;

constexpr int rank = 5;

// Make sure you have enough here that the sum of maximal components in dimension vector form does not spill over to the next component.
constexpr int bits_per_comp = 3;
int component(int i, root r){
    return ( r % (1 << bits_per_comp*i) ) >> bits_per_comp*(i-1);
}

root from_components(std::vector<int> components){
    root r = 0;
    int i = 0;
    for (int component : components ){
        r += (component << bits_per_comp*i);
        i++;
    }
    return r;
}
root from_components(std::initializer_list<int> components){
    return from_components((std::vector<int>)components);
}

inline root simple(int i){
    return (1 << bits_per_comp*(i-1));
}

std::string to_string(root a){
    std::ostringstream s; 
    s << "[";
    for (size_t i = 1; i <= rank; i++){
        s << " ";
        s << component(i, a);
    }
    s << " ]";
    return s.str();
}

root meet(root a, root b){
    std::vector<int> vec;
    for (size_t i = 1; i <= rank; i++){
        vec.push_back(std::min(component(i, a), component(i, b)));
    }
    return from_components(vec);
}

root join(root a, root b){
    std::vector<int> vec;
    for (size_t i = 1; i <= rank; i++){
        vec.push_back(std::max(component(i, a), component(i, b)));
    }
    return from_components(vec);
}

root E1minusE(int j){
    std::vector<int> vec;
    for (size_t i = 1; i <= rank; i++){
        vec.push_back(i < j ? 1 : 0);
    }
    return from_components(vec);
}

root E1plusE(int j){
    std::vector<int> vec;
    for (size_t i = 1; i <= rank; i++){
        vec.push_back((i < j || i > rank-2) ? 1 : 2);
    }
    return from_components(vec);
}


////////////////////////////////////////////////////////////////////////////////
// Root system operations
////////////////////////////////////////////////////////////////////////////////

// The A system
// constexpr int num_roots = rank*(rank+1)/2;
// The D system
constexpr int num_roots = rank*(rank-1);

std::vector<root> roots;
std::vector<int> simple_inds;

int valid(root const a){
    auto iter = std::lower_bound(roots.begin(), roots.end(), a);
    if (iter >= roots.end())
        return -1;
    if (*iter != a)
        return -1;
    return iter - roots.begin();
}

bool in_subsystem(root r, std::vector<int> I_inds){
    for (size_t i = 1; i <= rank; i++){
        if (std::binary_search(I_inds.begin(), I_inds.end(), simple_inds[i]))
            continue;
        if (component(i, r) > 1)
            return false;
    }
    return true;
}

void generate_roots_A(){
    for (size_t i = 1; i <= rank; i++){                 // 0 < i <= rank
        for (size_t j = i+1; j <= rank+1; j++){         // i < j <= rank+1
            roots.push_back(E1minusE(j) - E1minusE(i)); // Ei - Ej
        }
    }
    std::sort(roots.begin(), roots.end());
    for (size_t i = 1; i <= rank; i++){
        simple_inds.push_back(valid(simple(i)));
    }
}

void generate_roots_D(){
    for (size_t i = 1; i <= rank-1; i++){               // 0 < i <= rank-1
        for (size_t j = i+1; j <= rank; j++){           // i < j <= rank
            roots.push_back(E1minusE(j) - E1minusE(i)); // Ei - Ej
            roots.push_back(E1plusE(i) - E1minusE(j));  // Ei + Ej
        }
    }
    std::sort(roots.begin(), roots.end());
    for (size_t i = 1; i <= rank; i++){
        simple_inds.push_back(valid(simple(i)));
    }
}

////////////////////////////////////////////////////////////////////////////////
// Simple inversion set operations
////////////////////////////////////////////////////////////////////////////////

typedef std::bitset<num_roots> inversion_bitset;

std::vector<inversion_bitset> inversion_sets;

std::vector<root> bitset_to_roots(inversion_bitset set){
    std::vector<root> its_roots;
    for (size_t i = 0; i < num_roots; i++){
        if (set[i])
            its_roots.push_back(roots[i]);
    }
    return its_roots;
}

inversion_bitset roots_to_bitset(std::vector<root> its_roots){
    inversion_bitset set;
    for (auto a : its_roots){
        int a_index = valid(a);
        if (a_index == -1){
            //TODO: Throw some kind of error.
            return set;
        }
        set[a_index] = true;
    }
    return set;    
}
inversion_bitset roots_to_bitset(std::initializer_list<root> its_roots){
    return roots_to_bitset((std::vector<root>)its_roots);
}

bool contains(inversion_bitset set, root a){
    int a_index = valid(a);
    return (a_index == -1) ? true : set[a_index];
}

bool is_inversion_set(inversion_bitset set){
    for (size_t i = 0; i < num_roots; i++){         // run over all roots
        root r1 = roots[i];                         // define root r1
        bool r1_in_set = set[i];                    // define bool is r1 in set
        for (size_t j = i+1; j < num_roots; j++){   // run over all roots again
            root r2 = roots[j];                     // define root r2
            if (set[j] != r1_in_set) continue;      // r2 in same set as r1?
            if (valid(r1+r2) == -1) continue;       // r1+r2 a root?
            if (set[valid(r1+r2)] != r1_in_set)     // r1+r2 in same set as r1?
                return false;
        }
    }
    return true;
}

bool is_inflation(inversion_bitset set, std::vector<int> I_inds){
    for (int s_ind : I_inds){
        root s = roots[s_ind];
        for (size_t i = 0; i < num_roots; i++){
            root r = roots[i];
            bool r_in_set = set[i];
            if (in_subsystem(r, I_inds)) continue;
            if (valid(r+s) == -1) continue;
            if (set[valid(r+s)] != r_in_set)
                return false;
        }
    }
    return true;
}

// How do we generate the inversion sets?
// Want to start with the highest root. Then assume it DOES NOT have the next highest root? This forces other roots into the inversion set. After going through that process, we can go back to assuming it has the next highest root.

// Should probably start with the simplest algorithm, just to get the ball rolling.
void generate_inversion_sets(){
    for (size_t i = 0; i < (1 << num_roots); i++){
        inversion_bitset set = i;
        if (is_inversion_set(set))
            inversion_sets.push_back(set);
    }
}


// Also do not forget to generate the pretty diagrams you like so much.

////////////////////////////////////////////////////////////////////////////////
// Printing
////////////////////////////////////////////////////////////////////////////////

std::string root_info(root a, char const * display_name){
    std::ostringstream s;
    s << "Root: " << display_name << "   ";
    s << "int rep: " << a << "    ";
    s << "dimension vec: " << to_string(a) << "     ";
    s << "order index: " << valid(a);
    return s.str();
}



int main(){

    // std::cout << (1 >> 4) << " " << (1 << 4) << std::endl;
    // std::cout << component(1,9) << " " << component(2,9) << " " << component(3,9) << " " << component(4,9) << " " << component(5,9) << " " << component(6,9) << " " << component(7,9) << std::endl;
    // std::cout << 9 << std::endl;
    // std::cout << to_string(9) << std::endl;
    // std::cout << from_components({1,1,0,0,0}) << std::endl;

    // root new_root = from_components({1,2,3,1,2});
    // std::cout << 1 << " " << 2 << " " << 3 << " " << 1 << " " << 2 << std::endl;
    // std::cout << new_root << std::endl;
    // std::cout << to_string(new_root) << std::endl;
    // std::cout << component(1,new_root) << " " << component(2,new_root) << " " << component(3,new_root) << " " << component(4,new_root) << " " << component(5,new_root) << " " << component(6,new_root) << " " << component(7,new_root) << std::endl;

    generate_roots_D();


    std::cout << "All Roots: " << std::endl;
    for (root a : roots){
        std::cout << "  " << root_info(a, "") << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Simples: " << std::endl;
    for (int s : simple_inds){
        std::cout << "  " << root_info(roots[s], "") << std::endl;
    }
    std::cout << std::endl;

    generate_inversion_sets();


    std::cout << "All inversion sets: " << std::endl;
    for (inversion_bitset set : inversion_sets){
        std::cout << set << std::endl;
    }
    std::cout << std::endl;

    // std::cout << root_info(E1minusE(rank), "E1 - En") << std::endl;
    // std::cout << root_info(E1minusE(rank-1), "E1 - E(n-1)") << std::endl;
    // std::cout << root_info(E1minusE(rank) - E1minusE(rank-1), "E(n-1) + En") << std::endl;

    // std::cout << root_info(E1plusE(rank), "E1 + En") << std::endl;
    // std::cout << root_info(E1minusE(rank-1), "E1 - E(n-1)") << std::endl;
    // std::cout << root_info(E1plusE(rank) - E1minusE(rank-1), "E(n-1) + En") << std::endl;


    // root a1 = from_components({1,0,0,0,0});
    // std::cout << root_info(a1, "a1") << std::endl;

    // inversion_bitset set = roots_to_bitset({
    //     from_components({1,0,0,0,0}), 
    //     from_components({0,1,0,0,0}),
    //     from_components({1,1,0,0,0}),
    //     from_components({0,0,1,0,0}),
    //     from_components({0,1,1,0,0}),
    //     from_components({1,1,1,0,0})
    // });
    // std::cout << std::endl;


    return 0;
}