#pragma once

#include "../../../Common.hpp"
#include "../../token/Token.hpp"
#include "../Node.hpp"

namespace Compiler {
    /**
     * Represents a node that holds a single value.
     */
    class Value : public Node {
    public:
        /**
         * The held value of the node.
         */
        Token value;

        /**
         * Initialize the single value node.
         * @param value node value
         */
        Value(Token value);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;

        /**
         * Build bytecode for this node.
         * @param bytecode result bytecode list
         */
        void build(List<UString>& bytecode) override;
    };

    /**
     * Represents an instruction that performs an operation on two expressions.
     */
    class Operation : public Node {
    public:
        /**
         * The first expression of the operation.
         */
        Node* left;

        /**
         * The target operator of the operation.
         */
        UString target;
        
        /**
         * The second expression of the operation.
         */
        Node* right;

        /**
         * Initialize the operation.
         * @param left first expression
         * @param target target operator
         * @param right second expression
         */
        Operation(Node* left, UString target, Node* right);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;

        /**
         * Build bytecode for this node.
         * @param bytecode result bytecode list
         */
        void build(List<UString>& bytecode) override;
    };

    class JoinOperation : public Node {
    public:
        /**
         * The target expression of the operation.
         */
        Node* target;

        /**
         * The child expressions of the operation.
         */
        List<Node*> children;

        /**
         * Ubutuakuze the join operation.
         * @param target first expression
         * @param children operatorands
         */
        JoinOperation(Node* left, List<Node*> right);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    class SideOperation : public Node {
    public:
        UString target;

        Node* operand;

        bool left;

        SideOperation(UString target, Node* operand, bool left);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    class Group : public Node {
    public:
        Node* value;

        Group(Node* value);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;

        /**
         * Build bytecode for this node.
         * @param bytecode result bytecode list
         */
        void build(List<UString>& bytecode) override;
    };

    class Template : public Node {
    public:
        Token value;

        Template(Token value);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    class IndexFetch : public Node {
    public:
        UString name;

        Node* index;

        IndexFetch(UString name, Node* index);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    class IndexAssign : public Node {
    public:
        UString name;

        Node* index;

        Node* value;

        IndexAssign(UString name, Node* index, Node* value);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    class Tuple : public Node {
    public:
        List<Node*> members;

        Tuple(List<Node*> members);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    enum ConstructType {
        Default, //  new Foo()
        Struct,  // new Bar { x: true, y: 2 }
        Abstract //  new Baz() { @Override void foo() { } }
    };

    class Initializator : public Node {
    public:
        TreeMap<UString, Node*> members;

        Initializator(TreeMap<UString, Node*> members);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    class NewNode : public Node {
    public:
        UString name;

        ConstructType type;

        List<Node*> arguments;

        Node* initializator;

        NewNode(UString name, ConstructType type, List<Node*> arguments, Node* initializator);

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };
}