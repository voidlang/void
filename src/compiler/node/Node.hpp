#pragma once

#include "../../Common.hpp"

namespace Compiler {
    /**
     * Represents a registry of the parseable node types.
     */
    enum class NodeType {
        Method,
        Error
    };

    /**
     * Represents an instruction node that is parsed from raw tokens.
     * A node can be an exact instruction or a holder of multiple instructions.
     * The node hierarchy is then transformed to executable bytecode.
     */
    class Node {
    public:
        NodeType type;

        Node(NodeType);
    };
    
    class ErrorNode : public Node {
    public:
        ErrorNode();
    };
}