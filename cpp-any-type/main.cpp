#include <iostream>
#include <string>
#include <map>
#include <typeinfo>

namespace gl
{

class any
{
public:
    //Representation and basic construction of a generalized union type.
    any() : content(0)
    {
    }

    ~any()
    {
        delete content;
    }

    const std::type_info &type_info() const
    {
        return content ? content->type_info() : typeid(void);
    }

    //INWARD CONVERSIONS
    any(const any &other) : content(other.content ? other.content->clone() : 0)
    {
    }

    template <typename value_type>
    any(const value_type &value) : content(new holder<value_type>(value))
    {
    }

    any &swap(any &rhs)
    {
        //std::swap(content, rhs.content);
        return *this;
    }

    any &operator=(const any &rhs)
    {
        any temp(rhs);
        return swap(temp);
    }

    template <typename value_type>
    any &operator=(const value_type &rhs)
    {
        any temp(rhs);
        return swap(temp);
    }

    //OUTWARD CONVERSIONS
    operator const void *() const
    {
        return content;
    }

    template <typename value_type>
    bool copy_to(value_type &value) const
    {
        const value_type *copyable = to_ptr<value_type>();
        if (copyable)
            value = *copyable;
        return copyable;
    }

    template <typename value_type>
    const value_type *to_ptr() const
    {
        return type_info() == typeid(value_type) ? &static_cast<holder<value_type> *>(content)->held : 0;
    }

private:
    class placeholder
    {
    public:
        virtual ~placeholder()
        {
        }
        virtual const std::type_info &type_info() const = 0;
        virtual placeholder *clone() const = 0;
    };

    template <typename value_type>
    class holder : public placeholder
    {
    public:
        holder(const value_type &value) : held(value)
        {
        }

        virtual const std::type_info &type_info() const
        {
            return typeid(value_type);
        }

        virtual placeholder *clone() const
        {
            return new holder(held);
        }

        const value_type held;
    };

    placeholder *content;
};

template <typename value_type>
value_type any_cast(const any &operand)
{
    const value_type *result = operand.to_ptr<value_type>();
    return result ? *result : throw std::bad_cast();
}

} // end namespace

int main(int argc, char *argv[])
{
    gl::any i_a = 3;
    int i_b = gl::any_cast<int>(i_a);
    std::cout << i_a << "\t" << i_b << std::endl;

    gl::any s_a = std::string("demo");
    std::string s_b = gl::any_cast<std::string>(s_a);
    std::cout << s_a << "\t" << s_b << std::endl;

    std::map<std::string, gl::any> map_any;
    map_any.emplace("a", i_a);
    map_any.emplace("b", s_b);

    std::cout << i_a << "\t" << gl::any_cast<int>(map_any["a"]) << std::endl;
    std::cout << s_a << "\t" << gl::any_cast<std::string>(map_any["b"]) << std::endl;

    return 0;
}