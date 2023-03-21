#pragma once

#include "../Instruction.hpp"

#ifdef VOID_INSTRUCTION
namespace Void {
#pragma region INTEGER_PUSH
    /**
     * Represents an instruction that pushes an integer to the stack.
     */
    class IntegerPush : public Instruction {
    private:
        /**
         * The value to push to the stack.
         */
        int value;

    public:
        /**
         * Initialize the integer push instruction.
         */
        IntegerPush();

        /**
         * Parse raw bytecode instruction.
         * @param raw bytecode data
         * @parma args split array of the data
         * @param line bytecode line index
         * @aram executable bytecode executor
         */
        void parse(String data, List<String> args, uint line, Executable* executable) override;

        /**
         * Execute the instruction in the executable context.
         * @param context bytecode execution context
         */
        void execute(Context* context) override;
    
        /**
         * Get the string representation of the instruction.
         * @return instruction bytecode data
         */
        String debug() override;
    };
#pragma endregion
}
#endif