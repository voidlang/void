package org.voidlang.compiler.ast.type;

import org.jetbrains.annotations.NotNull;
import org.voidlang.compiler.ast.type.referencing.Referencing;
import org.voidlang.compiler.util.debug.Printable;
import org.voidlang.compiler.util.node.Matcher;
import org.voidlang.llvm.module.IRContext;
import org.voidlang.llvm.type.IRType;

/**
 * Represents the base type of any elements within the Void type hierarchy.
 * <p>
 * Void differentiates four kinds of types: `scalar`, `tuple`, `lambda` and `complex`.
 * <p>
 * For example: `int value`, `(int, bool) tuple`, `int |T| predicate` and `MyType myType`.
 */
public interface Type extends Printable, Matcher<Type> {
    /**
     * Retrieve the referencing of the type, that describes how the type should be treated.
     *
     * @return the referencing of the type
     */
    @NotNull Referencing referencing();

    /**
     * Generate an LLVM type wrapper for this type.
     *
     * @param context the context in which the type is generated
     * @return the LLVM type wrapper
     */
    @NotNull IRType codegen(@NotNull IRContext context);
}
