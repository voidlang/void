#include "ValueNode.hpp"

#include "../../../util/Strings.hpp"
using namespace Void;

namespace Compiler {
    /**
     * Initialize the single value node.
     * @param value node value
     */
    Value::Value(Token value)
        : Node(NodeType::Value), value(value)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void Value::debug(uint& index) {
        index++;
        print(value);
        index--;
    }

    /**
     * Build bytecode for this node.
     * @param bytecode result bytecode list
     */
    void Value::build(List<String>& bytecode) {

    }

    /**
     * Initialize the operation.
     * @param left first expression
     * @param target target operator
     * @param right second expression
     */
    Operation::Operation(Node* left, UString target, Node* right)
        : Node(NodeType::Operation), left(left), target(target), right(right)
    { }

    /**
     * Build bytecode for this node.
     * @param bytecode result bytecode list
     */
    void Operation::build(List<String>& bytecode) {
    }

    SideOperation::SideOperation(UString target, Node* operand, bool left)
        : Node(NodeType::SideOperation), target(target), operand(operand), left(left)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void SideOperation::debug(uint& index) {
        index++;
        println("SideOperation {");

        println(Strings::fill(index + 1, "    ") << "target: " << target);

        println(Strings::fill(index + 1, "    ") << "side: " << (left ? "left" : "right"));
        
        print(Strings::fill(index + 1, "    ") << "operand: ");
        operand->debug(index);
        if (operand->type == NodeType::Value || operand->type == NodeType::Template)
            println("");

        println(Strings::fill(index, "    ") << "}");
        index--;
    }

    /**
     * Debug the content of the parsed node.
     */
    void Operation::debug(uint& index) {
        index++;

        println("Operation {");

        print(Strings::fill(index + 1, "    ") << "left: ");
        left->debug(index);
        if (left->type == NodeType::Value)
            println("");

        println(Strings::fill(index + 1, "    ") << "operator: '" << target << "'");

        print(Strings::fill(index + 1, "    ") << "right: ");
        right->debug(index);
        if (right->type == NodeType::Value)
            println("");

        println(Strings::fill(index, "    ") << "}");

        index--;
    }

    Group::Group(Node* value) 
        : Node(NodeType::Group), value(value)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void Group::debug(uint& index) {
        index++;
        println("Group {");
        
        print(Strings::fill(index + 1, "    "));
        value->debug(index);
        if (value->type == NodeType::Value || value->type == NodeType::Template)
            println("");
        
        println(Strings::fill(index, "    ") << "}");
        index--;
    }

    /**
     * Build bytecode for this node.
     * @param bytecode result bytecode list
     */
    void Group::build(List<String>& bytecode) {
    }

    Template::Template(Token value)
        : Node(NodeType::Template), value(value)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void Template::debug(uint& index) {
        index++;
        print("Template " << value);
        index--;
    }

    IndexFetch::IndexFetch(UString name, Node* index)
        : Node(NodeType::IndexFetch), name(name), index(index)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void IndexFetch::debug(uint& index) {
        index++;
        println("IndexFetch {");

        println(Strings::fill(index + 1, "    ") << "name: " << name);
        
        print(Strings::fill(index + 1, "    ") << "index: ");
        this->index->debug(index);
        if (this->index->type == NodeType::Value || this->index->type == NodeType::Template)
            println("");

        println(Strings::fill(index, "    ") <<  "}");
        index--;
    }

    IndexAssign::IndexAssign(UString name, Node* index, Node* value)
        : Node(NodeType::IndexAssign), name(name), index(index), value(value)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void IndexAssign::debug(uint& index) {
        index++;
        println("IndexAssign {");

        println(Strings::fill(index + 1, "    ") << "name: " << name);

        print(Strings::fill(index + 1, "    ") << "index: ");
        this->index->debug(index);
        if (this->index->type == NodeType::Value || this->index->type == NodeType::Template)
            println("");

        print(Strings::fill(index + 1, "    ") << "value: ");
        value->debug(index);
        if (value->type == NodeType::Value || value->type == NodeType::Template)
            println("");

        println(Strings::fill(index, "    ") << "}");
        index--;
    }

    Tuple::Tuple(List<Node*> members)
        : Node(NodeType::Tuple), members(members)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void Tuple::debug(uint& index) {
        index++;
        println("Tuple [");

        for (auto member : members) {
            print(Strings::fill(index + 1, "    "));
            member->debug(index);
            if (member->type == NodeType::Value || member->type == NodeType::Template)
                println("");
        }

        println(Strings::fill(index, "    ") << "]");
        index--;
    }

    Initializator::Initializator(TreeMap<UString, Node*> members) 
        : Node(NodeType::Initializator), members(members)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void Initializator::debug(uint& index) {
        index++;
        println("Initialiaztor {");

        for (auto& [key, value] : members) {
            print(Strings::fill(index + 1, "    ") << key << ": ");
            value->debug(index);
            if (value->type == NodeType::Value || value->type == NodeType::Template)
                println("");
        }

        println(Strings::fill(index, "    ") << "}");
        index--;
    }

    NewNode::NewNode(UString name, ConstructType type, List<Node*> arguments, Node* initializator)
        : Node(NodeType::New), name(name), type(type), arguments(arguments), initializator(initializator)
    { }

    /**
     * Debug the content of the parsed node.
     */
    void NewNode::debug(uint& index) {
        index++;
        println("New {");

        println(Strings::fill(index + 1, "    ") << "name: " << name);

        println(Strings::fill(index + 1, "    ") << "type: " << (type == ConstructType::Default ? "default" : "struct"));

        println(Strings::fill(index + 1, "    ") << "arguments: [");
        for (auto argument : arguments) {
            print(Strings::fill(index + 2, "    "));
            index++;
            argument->debug(index);
            index--;
            if (argument->type == NodeType::Value || argument->type == NodeType::Template)
                println("");
        }
        println(Strings::fill(index + 1, "    ") << "]");

        if (initializator != nullptr) {
            print(Strings::fill(index + 1, "    ") << "initializator: ");
            initializator->debug(index);
        }

        println(Strings::fill(index, "    ") << "}");
        index--;
    }
}