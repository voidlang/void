package org.voidlang.compiler.node;

/**
 * Represents an enumeration of the types of nodes that can be present in the AST.
 */
public enum NodeType {
    SCOPE,
    STATEMENT,
    EXPRESSION,

    LITERAL,

    IMMUTABLE_LOCAL_DECLARE_ASSIGN,
    MUTABLE_LOCAL_DECLARE_ASSIGN,
    LOCAL_ASSIGN,

    NAME_ACCESS,

    METHOD_CALL,

    IMMUTABLE_PARAMETER_ACCESS,

    BINARY_OPERATOR,
    PREFIX_UNARY_OPERATOR,
    POSTFIX_UNARY_OPERATOR,
    TERNARY_OPERATOR,

    RETURN,

    METHOD,
    PACKAGE,

    EMPTY,
    ERROR,
    EOF
}
