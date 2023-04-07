#pragma once

#include "../../Common.hpp"
#include "../token/Token.hpp"
#include "Node.hpp"

namespace Compiler {
    static TreeMap<UString, Pair<int, int>> OPERATION_INFO = {
        {U"+", {1, 0}},
        {U"-", {1, 0}},
        {U"*", {2, 0}},
        {U"/", {2, 0}},
        {U"%", {2, 0}},
        {U"^", {3, 1}}
    };

    /**
     * Represents a parser that transforms raw tokens to instructions.
     */
    class NodeParser {
    private:
        /**
         * The list of the tokens to be parsed.
         */
        List<Token> tokens;

        /**
         * The index of the currently parsed token.
         */
        uint cursor = 0;

    public:
        /**
         * Initialize the token parser.
         */
        NodeParser(List<Token> tokens);

        /**
         * Parse the next instruction node.
         * @return new instruction node
         */
        Node* next();

        /**
         * Get the current at the index.
         * @return currently parsed token
         */
        Token peek();

        /**
         * Get the current at the index.
         * Check if the retrieved token does not match the given type.
         * @param type required token type
         * @return currently parsed token
         */
        Token peek(TokenType type);

        /**
         * Get the current at the index.
         * Check if the retrieved token does not match any of the given typs.
         * @param size required token types' length
         * @return currently parsed token
         */
        Token peek(uint size, ...);

        /**
         * Get the token at the current index and move to the next position.
         * @return currently parsed token
         */
        Token get();

        /**
         * Get the token at the current index and move to the next position.
         * Check if the retrieved token does not match the given type.
         * @param type required token type
         * @return currently parsed token
         */
        Token get(TokenType type);

        /**
         * Get the token at the current index and move to the next position.
         * Check if the retrieved token does not match the given type or value.
         * @param type required token type
         * @return currently parsed token
         */
        Token get(TokenType type, UString value);

        /**
         * Get the token at the current index and move to the next position.
         * Check if the retrieved token does not match any of the given types.
         * @param size required token types' length
         * @return currently parsed token
         */
        Token get(uint size, ...);

        /**
         * Move the cursor with the give amount.
         * @param amount cursor move amount
         */
        void skip(uint amount);

        /**
         * Get the token at the given index.
         * @param index token data index
         * @return otken at the index or null if not in bounds
         */
        Token at(uint index);

        /**
         * Determine if the given index is in bounds of the data size.
         * @param index target index to check
         * @return true if hte index is in bounds
         */
        bool has(uint index);

        /**
         * Parse the next package declaration.
         * @return new declared package
         */
        Node* nextPackage();

        /**
         * Parse the next package import.
         * @return new package import
         */
        Node* nextImport();

        /**
         * Parse the next method node.
         * @return new method node
         */
        Node* nextMethod();

        /**
         * Parse the next type declaration.
         * @return new declared type
         */
        Node* nextType();

        /**
         * Parse the next type or method declaration.
         * @return new declared type or method
         */
        Node* nextTypeOrMethod();

        /**
         * Parse the next content of a type, which might be a type, method or field.
         * @return new declared type, method or field
         */
        Node* nextContent();

        /**
         * Parse the next expression instruction.
         * @return new expression
         */
        Node* nextExpression();

        /**
         * Parse the next local variable declaration.
         * @return new local declaration
         */
        Node* nextLocalDeclaration();

        /**
         * Parse the next local variable value assignation.
         * @return new local assignation
         */
        Node* nextLocalAssignation();

        /**
         * Parse the next if statement declaration.
         * @return new if statement
         */
        Node* nextIfStatement();

        /**
         * Parse the next else if statement declaration.
         * @return new else if statement
         */
        Node* nextElseIfStatement();

        /**
         * Prase the next else statement declaration.
         * @return new else statement
         */
        Node* nextElseStatement();

        /**
         * Check if the first operator has a predecende priority over the second operator.
         * @param first first operator to check
         * @param second second operator to check
         * @return true if the first operator has higher precedence than the second one
         */
        bool hasPrecedence(UString first, UString second);

        /**
         * Fix the order of the operation sequences in the parsed value node.
         * @param root root value expression node
         * @return fixed node operations
         */
        Node* fixOperationTree(Node* root);

        /**
         * Parse the generic types of a type.
         * @return generic type tokens
         */
        List<Token> parseGenerics();

        /**
         * Parse the generic names of a method or type.
         * @return generic type names
         */
        List<UString> parseGenericNames();

        /**
         * Parse the array declaration of a type.
         * @return array dimensions
         */
        int parseArray();

        /**
         * Parse the modifiers of the given node type.
         * @param type target node type
         * @return parsed modifier list
         */
        List<UString> parseModifiers(NodeType type);

        /**
         * Test if there are variadic arguments declared.
         * @return true if varargs are declared
         */
        bool testVarargs();
    };
}