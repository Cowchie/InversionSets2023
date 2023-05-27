#include <iostream>

template<int scale>
class rect{
    private:
        int width, height;
    public:
        constexpr rect(int w, int h) : width(0), height(0) {
            width = w;
            height = h;
        }
        int constexpr area() const {
            return scale*width*height;
        }
};

template<int number>
class my_class{
public:
    int value = number;
};

int main(){
    rect<2> constexpr R(10, 10);

    my_class<R.area()> const c;

    int my_array[c.value];

    return 1;
}