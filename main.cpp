#include <iostream>
#include <string>
#include <unordered_map>
#include <typeinfo>

class ComponentBase {
public:
    ComponentBase() = default;
    virtual ~ComponentBase() = default;

private:
    ComponentBase(const ComponentBase&) = delete;
    ComponentBase(ComponentBase&&) = delete;
    ComponentBase& operator=(const ComponentBase&) = delete;
    ComponentBase& operator=(ComponentBase&&) = delete;
};

template<class T>
class Component : public ComponentBase {
public:
    Component() = default;
    virtual ~Component() = default;

    void create_component() {
        _data_pointer = new(std::nothrow) T();
    }

    T* get_component() {
        return _data_pointer;
    }

private:
    Component(const Component&) = delete;
    Component(Component&&) = delete;
    Component& operator=(const Component&) = delete;
    Component& operator=(Component&&) = delete;

    T* _data_pointer {nullptr};
};

class Singleton {
public:
    ~Singleton() = default;

    static Singleton* get_instance() {
        static Singleton instance;
        return &instance;
    }

    template<class T>
    void registe_component(const std::string& name) {
        auto pointer = new(std::nothrow) Component<T>();
        pointer->create_component();
        _component_map[name] = pointer;
    }

    template<class T>
    T* get_component(const std::string& name) {
        auto iter = _component_map.find(name);
        if (iter == _component_map.end()) {
            return nullptr;
        }
        auto pointer = dynamic_cast<Component<T>*>(iter->second);
        if (pointer == nullptr) {
            return nullptr;
        }

        return pointer->get_component();
    }

private:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    std::unordered_map<std::string, ComponentBase*> _component_map;
};

int main(int argc, char** argv) {
    Singleton* singleton_instance = Singleton::get_instance();

    // 注册
    singleton_instance->registe_component<int>("int_class");
    singleton_instance->registe_component<std::string>("string_class");

    // 获取int_class
    auto* int_pointer = singleton_instance->get_component<int>("int_class");
    if (int_pointer == nullptr) {
        std::cout << "int_pointer is nullptr" << std::endl;
    } else {
        std::cout << "int_pointer is not nullptr, type name:" << typeid(*int_pointer).name() << std::endl;
    }

    // 获取string_class
    auto* string_pointer = singleton_instance->get_component<std::string>("string_class");
    if (string_pointer == nullptr) {
        std::cout << "string_pointer is nullptr" << std::endl;
    } else {
        std::cout << "string_pointer is not nullptr, type name:" << typeid(*string_pointer).name() << std::endl;
    }

    // name不存在
    auto* nullptr_pointer = singleton_instance->get_component<int>("empty_class");
    if (nullptr_pointer == nullptr) {
        std::cout << "nullptr_pointer is nullptr" << std::endl;
    } else {
        std::cout << "nullptr_pointer is not nullptr, type name:" << typeid(*nullptr_pointer).name() << std::endl;
    }

    // name与类型不对应
    auto* error_pointer = singleton_instance->get_component<int>("string_class");
    if (error_pointer == nullptr) {
        std::cout << "error_pointer is nullptr" << std::endl;
    } else {
        std::cout << "error_pointer is not nullptr, type name:" << typeid(*error_pointer).name() << std::endl;
    }
    return 0;
}
