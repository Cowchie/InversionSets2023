#include <iostream>
#include <bitset>
#include <algorithm>
#include <vector>
#include <functional>

#include <string>
#include <sstream>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// class which represents a collection of roots in a root system.
////////////////////////////////////////////////////////////////////////////////

template<int rank, int bits_per_comp = 3>
class root_system{

public:

////////////////////////////////////////////////////////////////////////////////
// The root struct
////////////////////////////////////////////////////////////////////////////////

    struct root{
        long long as_integer;

        root(int a){
            this->as_integer = a;
        }

        root & operator=(int a){
            this->as_integer = a;
            return *this;
        }

        friend inline bool operator< (root const & lhs,    root const & rhs){
            return lhs.as_integer < rhs.as_integer;
        }
        friend inline bool operator> (root const & lhs,    root const & rhs){
            return rhs < lhs; 
        }
        friend inline bool operator<=(root const & lhs,    root const & rhs){
            return !(lhs > rhs); 
        }
        friend inline bool operator>=(root const & lhs,    root const & rhs){
            return !(lhs < rhs); 
        }

        friend inline bool operator==(root const & lhs,    root const & rhs){
            return lhs.as_integer == rhs.as_integer;
        }
        friend inline bool operator!=(root const & lhs,    root const & rhs){
            return !(lhs == rhs);
        }

        inline root &      operator+=(root const & rhs){
            this->as_integer += rhs.as_integer;
            return *this;
        }
        friend inline root operator+ (root lhs,            root const & rhs){
            lhs += rhs;
            return lhs;
        }

        inline root &      operator-=(root const & rhs){
            this->as_integer -= rhs.as_integer;
            return *this;
        }
        friend inline root operator- (root lhs,            root const & rhs){
            lhs -= rhs;
            return lhs;
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Simple root vector operations
////////////////////////////////////////////////////////////////////////////////

    inline int component(int i, root r) const{
        return ( r.as_integer % (1 << bits_per_comp*i) ) >> bits_per_comp*(i-1);
    }

    root from_components(std::vector<int> components) const{
        root r = 0;
        int i = 0;
        for (int component : components ){
            r.as_integer += (component << bits_per_comp*i);
            i++;
        }
        return r;
    }
    root from_components(std::initializer_list<int> components) const{
        return from_components((std::vector<int>)components);
    }

    inline root simple(int i) const{
        return (1 << bits_per_comp*(i-1));
    }

////////////////////////////////////////////////////////////////////////////////
// The meet and join of two roots.
////////////////////////////////////////////////////////////////////////////////

    root meet(root a, root b) const{
        std::vector<int> vec;
        for (size_t i = 1; i <= rank; i++){
            vec.push_back(std::min(component(i, a), component(i, b)));
        }
        return from_components(vec);
    }

    /// @brief This may not actually be correct
    /// @param a 
    /// @param b 
    /// @return 
    root join(root a, root b) const{
        std::vector<int> vec;
        for (size_t i = 1; i <= rank; i++){
            vec.push_back(std::max(component(i, a), component(i, b)));
        }
        return from_components(vec);
    }

////////////////////////////////////////////////////////////////////////////////
// To string
////////////////////////////////////////////////////////////////////////////////

    std::string to_string(root a) const{
        std::ostringstream s; 
        s << "[";
        for (size_t i = 1; i <= rank; i++){
            s << " ";
            s << component(i, a);
        }
        s << " ]";
        return s.str();
    }

////////////////////////////////////////////////////////////////////////////////
// The roots in $$ \varepsilon_i \pm \varepsilon_j $$ form
////////////////////////////////////////////////////////////////////////////////
private:

    root E1minusE(int j) const{
        std::vector<int> vec;
        for (size_t i = 1; i <= rank; i++){
            vec.push_back(i < j ? 1 : 0);
        }
        return from_components(vec);
    }

    root E1plusE(int j) const{
        std::vector<int> vec;
        for (size_t i = 1; i <= rank; i++){
            vec.push_back((i < j || i > rank-2) ? 1 : 2);
        }
        return from_components(vec);
    }

////////////////////////////////////////////////////////////////////////////////
// Operations which generate root systems
////////////////////////////////////////////////////////////////////////////////
public:
    int num_roots = 0;

    std::vector<root> roots;
    int simple_inds[rank];

private:
    void generate_roots_A(){
        for (size_t i = 1; i <= rank; i++){                 // 0 < i <= rank
            for (size_t j = i+1; j <= rank+1; j++){         // i < j <= rank+1
                roots.push_back(E1minusE(j) - E1minusE(i)); // Ei - Ej
            }
        }
        std::sort(roots.begin(), roots.end());
        num_roots = roots.size();
        for (size_t i = 1; i <= rank; i++){
            simple_inds[i] = valid(simple(i));
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
        num_roots = roots.size();
        for (size_t i = 1; i <= rank; i++){
            simple_inds[i] = valid(simple(i));
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

public:
    root_system(char const type){
        if (type == 'A')
            generate_roots_A();
        else if (type == 'D')
            generate_roots_D();
        else{
            std::ostringstream s;
            s << "Not Implemented root system of type ";
            s << type;
            throw std::logic_error(s.str());
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Check properties of roots in root systems.
////////////////////////////////////////////////////////////////////////////////

    int valid(root const & a) const{
        auto iter = std::lower_bound(roots.begin(), roots.end(), a);
        if (iter >= roots.end())
            return -1;
        if (*iter != a)
            return -1;
        return iter - roots.begin();
    }

    bool in_subsystem(root r, std::vector<int> I_inds) const{
        for (size_t i = 1; i <= rank; i++){
            if (std::binary_search(I_inds.begin(), I_inds.end(), simple_inds[i]))
                continue;
            if (component(i, r) > 0)
                return false;
        }
        return true;
    }
};


////////////////////////////////////////////////////////////////////////////////
// class which represents a collection of root sets.
////////////////////////////////////////////////////////////////////////////////

template<typename root_system, int num_roots>
class root_set_collection{
    
    typedef root_system::root root

    struct root_set{
        std::bitset<num_roots> bitset_form;

        bool operator[](int i) const{
            return bitset_form[i];
        }
    };

    std::vector<root> set_to_vector(std::vector<root> roots, root_set set){
        std::vector<root> its_roots;
        for (size_t i = 0; i < num_roots; i++){
            if (set[i])
                its_roots.push_back(roots[i]);
        }
        return its_roots;
    }

private:
    root_system const * delta;

public:
    root_set roots_to_set(std::vector<root> its_roots){
        root_set set;
        for (auto a : its_roots){
            int a_index = delta->valid(a);
            if (a_index == -1){
                //TODO: Throw some kind of error.
                return set;
            }
            set.bitset_form[a_index] = true;
        }
        return set;    
    }
    root_set roots_to_bitset(std::initializer_list<root> its_roots){
        return roots_to_set((std::vector<root>)its_roots);
    }

    bool contains(root_set set, root a){
        int a_index = delta->valid(a);
        return (a_index == -1) ? true : set[a_index];
    }

    bool is_inversion_set(std::vector<root> roots, root_set set){
        for (size_t i = 0; i < num_roots; i++){         // run over all roots
            root r1 = roots[i];                         // define root r1
            bool r1_in_set = set[i];                    // define bool is r1 in set
            for (size_t j = i+1; j < num_roots; j++){   // run over all roots again
                root r2 = roots[j];                     // define root r2
                if (set[j] != r1_in_set) continue;      // r2 in same set as r1?
                if (delta->valid(r1+r2) == -1) continue;       // r1+r2 a root?
                if (set[delta->valid(r1+r2)] != r1_in_set)     // r1+r2 in same set as r1?
                    return false;
            }
        }
        return true;
    }

    // bool is_inflation(std::vector<root> roots, root_set set, std::vector<int> I_inds){
    //     for (int s_ind : I_inds){
    //         root s = roots[s_ind];
    //         for (size_t i = 0; i < num_roots; i++){
    //             root r = roots[i];
    //             bool r_in_set = set[i];
    //             if (in_subsystem(r, I_inds)) continue;
    //             if (valid(r+s) == -1) continue;
    //             if (set[valid(r+s)] != r_in_set)
    //                 return false;
    //         }
    //     }
    //     return true;
    // }

    std::vector<root_set> vector_form;

private:
    // Should probably start with the simplest algorithm, just to get the ball rolling.
    void generate_inversion_sets(){
        for (size_t i = 0; i < (1 << num_roots); i++){
            auto set = root_set{i};
            if (is_inversion_set(set))
                vector_form.push_back(set);
        }
    }

public:
    root_set_collection(root_system const & system){
        delta = &system;
        generate_inversion_sets();
    }
    // root_set_collection(){
    //     valid = [](root r){return true;};
    //     generate_inversion_sets();
    // }

};













////////////////////////////////////////////////////////////////////////////////
// Testing
////////////////////////////////////////////////////////////////////////////////

int main(){
    std::cout << "Testing root_system.cpp" << std::endl;

    int const N = 3;

    root_system<N> delta_plus('A');

    for (auto r : delta_plus.roots){
        std::cout << delta_plus.to_string(r) << std::endl;
    }

    auto inversion_sets = root_set_collection<root_system<N>, 2*N*N>(delta_plus);

    for (auto s : inversion_sets.vector_form){
        std::cout << s << std::endl;
    }

    return 1;
}