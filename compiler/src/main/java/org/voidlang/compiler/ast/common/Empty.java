package org.voidlang.compiler.ast.common;

import org.jetbrains.annotations.NotNull;
import org.voidlang.compiler.ast.Node;
import org.voidlang.compiler.ast.NodeInfo;
import org.voidlang.compiler.ast.NodeType;
import org.voidlang.compiler.generator.Generator;
import org.voidlang.llvm.value.IRValue;

/**
 * Represents a dummy node in the Abstract Syntax Tree, that is used as a placeholder, when the parser expects
 * a node to be retrieved, but there is nothing to return.
 * <p>
 * This is used mainly to work around unexpected auto-inserted semicolons in the token stream.
 */
@NodeInfo(type = NodeType.EMPTY)
public class Empty extends Node {
    /**
     * Generate the LLVM IR code for this node, that will be put into the parent scope instruction set.
     *
     * @param generator the generation context to use for the code generation
     * @return the LLVM IR value representing the result of the node
     */
    @Override
    public @NotNull IRValue codegen(@NotNull Generator generator) {
        throw new IllegalStateException("Cannot invoke `codegen` on an empty node");
    }
}
