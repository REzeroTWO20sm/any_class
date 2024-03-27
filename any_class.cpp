#include <iostream>
#include <typeinfo>

using namespace std;

// g++ any_class.cpp -o any_class --std=c++14 -I/usr/local/include

class any
{
public:
    template<typename T>
    any(const T& t) : held_(new holder<T>(t)), initialized_(true) {}
    
    any() : held_(new holder<int>(0)), initialized_(false) {}
    
    ~any() { delete held_; }
    
    template<typename U>
    U cast() const
    {
        if (typeid(U) != held_->type_info()){
            throw std::runtime_error("Bad any cast");
        }
        if (!is_initialized()) {
            throw std::runtime_error("Any is not initialized");
        }
        
        return static_cast<holder<U>*>(held_)->t_;
    }
    
    bool is_initialized() const
    {
        return initialized_;
    }
    
private:
    struct base_holder
    {
        virtual ~base_holder(){}
        virtual const std::type_info& type_info() const = 0;
    };
  
    template<typename T>
    struct holder : base_holder
    {
        holder(const T& t) : t_(t) {}
        
        const std::type_info& type_info() const
        {
            return typeid(t_);
        }
        
        T t_;
    };
  
private:
    bool initialized_;
    base_holder* held_;
};

int main()
{
    any a;
    any b(2);
    any c(string("Hello"));
    any d(2.5);
    
    if (a.is_initialized()) { // проверка работает ли функция проверки инициализации, я этот коммент сам пишу !
        cout << "Object 'a' is initialized" << endl;
    }
    else {
        cout << "Object 'a' is not initialized" << endl;
    }

    try {
        cout << b.cast<int>() << endl;
        cout << d.cast<double>() << endl;
        //cout << c.cast<double>() << endl; // вызывет исключение Bad any cast
        cout << a.cast<int>() << endl;  // вызовет исключение any is not initialized
    }
    catch (const std::runtime_error& e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
