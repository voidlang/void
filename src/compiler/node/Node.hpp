#pragma once

#include "../../Common.hpp"

namespace Compiler {
    /**
     * Represents a registry of the parseable node types.
     */
    enum class NodeType {
        ModifierList,
        ModifierBlock,
        Method,
        Field,
        MultiField,
        Package,
        Import,
        Class,
        Struct,
        TupleStruct,
        Enum,
        Interface,
        Annotation,
        LocalDeclare,
        MultiLocalDeclare,
        LocalDeclareAssign,
        LocalDeclareDestructure,
        LocalAssign,
        Value,
        New,
        Initializator,
        Operation,
        JoinOperation,
        SideOperation,
        MethodCall,
        Group,
        Template,
        Lambda,
        IndexFetch,
        IndexAssign,
        Return,
        Defer,
        Tuple,
        If,
        ElseIf,
        Else,
        While,
        DoWhile,
        For,
        ForEach,
        Error,
        Finish
    };

    OutputStream& operator<<(OutputStream& stream, NodeType& type);

    /**
     * Represents an instruction node that is parsed from raw tokens.
     * A node can be an exact instruction or a holder of multiple instructions.
     * The node hierarchy is then transformed to executable bytecode.
     */
    class Node {
    public:
        /**
         * The type of the node.
         */
        NodeType type;

        /**
         * Initialize the node.
         * @param type node type
         */
        Node(NodeType type);

        /**
         * Debug the content of the parsed node.
         */
        virtual void debug(uint& index);

        /**
         * Build bytecode for this node.
         * @param bytecode result bytecode list
         */
        virtual void build(List<String>& bytecode);
    
        /**
         * Determine if this node has the given type.
         * @param type target type to check
         * @return true if the type matches this node
         */
        bool is(NodeType type);
    };
    
    /**
     * Represewnts a node that holds the information of a compiling error 
     * that occurred whilst parsing tokens to nodes.
     */
    class ErrorNode : public Node {
    public:
        /**
         * Initialize the error node.
         */
        ErrorNode();

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    /**
     * Represents a node that indicates that the parsing of the file has been ended.
     */
    class FinishNode : public Node {
    public:
        /**
         * Initialize the finish node.
         */
        FinishNode();

        /**
         * Debug the content of the parsed node.
         */
        void debug(uint& index) override;
    };

    class Modifiable : public Node {
    public:
        List<UString> modifiers; // to be initialized later

        Modifiable(NodeType type);
    };
}