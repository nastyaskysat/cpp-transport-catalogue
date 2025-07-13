#pragma once 
 
#include "json.h" 
#include <optional> 
 
namespace json 
{ 
 
    class Builder; 
    class Context; 
    class DictItemContext; 
    class DictKeyContext; 
    class ArrayItemContext; 
 
    class Builder 
    { 
    public: 
        Builder(); 
 
        DictKeyContext Key(std::string key); 
        Builder &Value(Node::Value value); 
        DictItemContext StartDict(); 
        Builder &EndDict(); 
        ArrayItemContext StartArray(); 
        Builder &EndArray(); 
        Node Build(); 
        Node GetNode(Node::Value value); 
 
    private: 
        Node root_{nullptr}; 
        std::vector<Node *> nodes_stack_; 
        std::optional<std::string> key_{std::nullopt}; 
 
        friend class Context; 
        friend class DictItemContext; 
        friend class DictKeyContext; 
        friend class ArrayItemContext; 
    }; 
 
    class Context 
    { 
    public: 
        explicit Context(Builder &builder) : builder_(builder) {} 
        virtual ~Context() = default; 
 
        Builder &Value(Node::Value value); 
        DictItemContext StartDict(); 
        Builder &EndDict(); 
        DictKeyContext Key(std::string key); 
        ArrayItemContext StartArray(); 
        Builder &EndArray(); 
 
    protected: 
        Builder &builder_; 
    }; 
 
    class DictItemContext : public Context 
    { 
    public: 
        using Context::Context; 
 
        DictKeyContext Key(std::string key); 
        Builder &EndDict(); 
 
        Builder &Value(Node::Value value) = delete; 
        DictItemContext StartDict() = delete; 
        ArrayItemContext StartArray() = delete; 
        Builder &EndArray() = delete; 
    }; 
 
    class DictKeyContext : public Context 
    { 
    public: 
        using Context::Context; 
 
        DictItemContext Value(Node::Value value); 
        ArrayItemContext StartArray(); 
        DictItemContext StartDict(); 
 
        Builder &EndDict() = delete; 
        DictKeyContext Key(std::string key) = delete; 
        Builder &EndArray() = delete; 
    }; 
 
    class ArrayItemContext : public Context 
    { 
    public: 
        using Context::Context; 
 
        ArrayItemContext Value(Node::Value value); 
        DictItemContext StartDict(); 
        ArrayItemContext StartArray(); 
        Builder &EndArray(); 
        // Запрещаем методы, которые не должны быть доступны в этом контексте 
        DictKeyContext Key(std::string key) = delete; 
        Builder &EndDict() = delete; 
    }; 
 
} // namespace json 