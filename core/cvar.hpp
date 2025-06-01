#pragma once
#include <string>

enum class CvarType {
	Int,
	Float,
	Bool,
	String
};

class BaseCvar {
public:
	virtual ~BaseCvar() = default;

	virtual const std::string& getName() const = 0;
	virtual CvarType getType() const = 0;

	virtual void setFromString(const std::string& val) = 0;
	virtual std::string getAsString() const = 0;
};

template<typename T> CvarType typeOf();
template<> CvarType typeOf<int>() { return CvarType::Int; }
template<> CvarType typeOf<float>() { return CvarType::Float; }
template<> CvarType typeOf<bool>() { return CvarType::Bool; }
template<> CvarType typeOf<std::string>() { return CvarType::String; }

template<typename T> T fromString(const std::string& s);
template<typename T> T fromString(const std::string& s) {
    static_assert(sizeof(T) == 0, "fromString not implemented for this type");
}
template<> int fromString<int>(const std::string& s) { return std::stoi(s); }
template<> float fromString<float>(const std::string& s) { return std::stof(s); }
template<> bool fromString<bool>(const std::string& s) { return s == "true" || s == "1";}
template<> std::string fromString<std::string>(const std::string& s) { return s; }

template<typename T> std::string toString(const T& v);
template<typename T> std::string toString(const T& v) {
    static_assert(sizeof(T) == 0, "toString not implemented for this type");
}
template<> std::string toString<int>(const int& v) { return std::to_string(v); }
template<> std::string toString<float>(const float& v) { return std::to_string(v); }
template<> std::string toString<bool>(const bool& v) { return v ? "true" : "false"; }
template<> std::string toString<std::string>(const std::string& v) { return v; }

template <typename T>
class Cvar : public BaseCvar {
private:
    std::string name;
    void (*setter)(const T&);
    T(*getter)();

public:
    Cvar(const std::string& name, void (*setFunc)(const T&), T(*getFunc)())
        : name(name), setter(setFunc), getter(getFunc) {
    }

    void set(const T& v) { setter(v); }
    T get() const { return getter(); }

    const std::string& getName() const override { return name; }

    void setFromString(const std::string& val) override {
        setter(fromString<T>(val));
    }

    std::string getAsString() const override {
        return toString(getter());
    }

    CvarType getType() const override {
        return typeOf<T>();
    }
};