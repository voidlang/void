#include "Class.hpp"
#include "../../util/Strings.hpp"
#include "../../util/Lists.hpp"
#include "../parser/Instruction.hpp"
#include "../runtime/Modifier.hpp"

namespace Void {
    /**
     * Initialize the class.
     * @param name class name
     * @param superclass class superclass
     * @param modifiers class access modifiers
     * @param interfaces class interfaces
     * @param vm void virtual machine
     */
    Class::Class(String name, String superclass, int modifiers, List<String> interfaces, VirtualMachine* vm)
        : name(name), superclass(superclass), modifiers(modifiers), interfaces(interfaces), vm(vm)
    { }

    /**
     * Initialize the class.
     * @param name class name
     * @param superclass class superclass
     * @param modifiers class access modifiers
     * @param interfaces class interfaces
     * @param vm void virtual machine
     */
    Class::Class(String name, String superclass, List<String> modifiers, List<String> interfaces, VirtualMachine* vm)
        : name(name), superclass(superclass), modifiers(maskModifiers(modifiers)), interfaces(interfaces), vm(vm)
    { }

    /**
     * Build class methods, fields and inner classes from bytecode class content.
     * @param bytecode class content
     */
    void Class::build(List<String> bytecode) {
        // TODO might not need to separate variables for classes, methods and fields

        // the atributes of the currently parsed class
        String className = "<unk>";
        String classSuperclass = "Object";
        List<String> classModifiers;
        List<String> classInterfaces;

        // the attributes of the currently parsed method
        String methodName = "<unk>";
        String methodReturnType = "V";
        List<String> methodParameters;
        List<String> methodModifiers;
        
        // the attributes of the currently parsed field
        String fieldName = "<unk>";
        String fieldType = "";
        List<String> fieldModifiers;

        // the content of the currently parsed element
        List<String> content;

        // determine if the declaration of the content of the parsed class has begun
        bool contentBegun = false;

        // the offset of inner elements inside the content
        // to be incremented by every %ELEMENT%_BEGIN
        // to be decremented by every %ELEMENT%_END
        uint classOffset = 0;
        uint methodOffset = 0;
        uint fieldOffset = 0;

        // the type of the current declaration
        ElementType type = ElementType::NONE;

        // parse the raw bytecode instructions
        for (String line : bytecode) {
            // split up the line and get the instruction
            List<String> args = Strings::split(line, ' ');
            String instruction = args[0];

            // check if the element content declaration has begun
            if (contentBegun) {
                // handle class content declaration end
                if (instruction == Instructions::CLASS_END && --classOffset == 0) {
                    // mark the declaration ended
                    contentBegun = false;
                    type = ElementType::NONE;

                    // get the separator between the parent clsas and the inner class
                    // eg: static -> my.package.ClassName.StaticInnerClass
                    // non-static -> my.package.OtherClassName$NonStaticInnerClass
                    int modifiers = maskModifiers(classModifiers);
                    char separator = hasModifier(modifiers, Modifier::STATIC) ? '.' : '$';

                    // apply the prefix of the parent class to the inner class name
                    className = name + separator + className;

                    // check if the class name is in use
                    if (vm->getClass(className) != nullptr)
                        error("ClassRedefineException: Class '" << className << "' is already defined.");

                    // define the class in the virtual machine
                    Class* clazz = new Class(className, classSuperclass, modifiers, classInterfaces, vm);
                    vm->defineClass(clazz);
                    // make the class build its content
                    clazz->build(content);

                    // reset class declaration variables 
                    className = "<unk>";
                    classSuperclass = "Object";
                    classModifiers.clear();
                    classInterfaces.clear();
                    content.clear();
                }

                // handle method content declaration end
                else if (instruction == Instructions::METHOD_END && type == ElementType::METHOD && --methodOffset == 0) {
                    // make the method declaration ended
                    contentBegun = false;
                    type = ElementType::NONE;

                    // check if the method with the same signature is already in use
                    if (getMethod(methodName, methodParameters) != nullptr)
                        error(
                            "MethodRedefineException: Method " << methodName 
                            << "(" << Strings::join(methodParameters, ", ") 
                            << " is already defined for class " << name
                        );
                   
                    // define the method for the class
                    Method* method = new Method(methodName, methodReturnType, methodModifiers, methodParameters, this, vm);
                    defineMethod(method);
                    // make the method build its content
                    method->build(content);

                    // reset method declaration variables
                    methodName = "<unk>";
                    methodReturnType = "V";
                    methodModifiers.clear();
                    methodParameters.clear();
                    content.clear();
                }

                // handle field content declaration end
                else if (instruction == Instructions::FIELD_END && type == ElementType::FIELD && --fieldOffset == 0) {
                    // TODO handle field declaration
                }

                // handle inner classes, methods and fields
                else {
                    // handle inner class content begin
                    if (instruction == Instructions::CLASS_BEGIN)
                        classOffset++;
                    // handle method content declaration begin
                    else if (instruction == Instructions::METHOD_BEGIN) {
                        methodOffset++;
                        println("mbeg");
                    }
                    // handle field content declaration begn
                    else if (instruction == Instructions::FIELD_BEGIN)
                        fieldOffset++;

                    // append the line to the currently parsed element's content
                    content.push_back(line);
                }
            }

            // handle class definition start
            else if (instruction == Instructions::CLASS_DEFINE) {
                className = args[1];
                type = ElementType::CLASS;
            }

            // handle method definition start
            else if (instruction == Instructions::METHOD_DEFINE) {
                methodName = args[1];
                type = ElementType::METHOD;
            }

            // handle field definition start
            else if (instruction == Instructions::FIELD_DEFINE) {
                fieldName = args[1];
                type = ElementType::FIELD;
            }

            // handle class property setters
            else if (type == ElementType::CLASS) {
                // set class access modifiers
                if (instruction == Instructions::CLASS_MODIFIER)
                    classModifiers = Lists::subList(args, 1);

                // set class superclass
                else if (instruction == Instructions::CLASS_EXTENDS)
                    classSuperclass = args[1];

                // set class interfaces
                else if (instruction == Instructions::CLASS_IMPLEMENTS)
                    classInterfaces = Lists::subList(args, 1);

                // handle class content declaration begin
                else if (instruction == Instructions::CLASS_BEGIN && classOffset++ == 0)
                    contentBegun = true;
            }

            // handle method property setters
            else if (type == ElementType::METHOD) {
                // set the method access modifiers
                if (instruction == Instructions::METHOD_MODIFIER)
                    methodModifiers = Lists::subList(args, 1);

                // set the method return type
                else if (instruction == Instructions::METHOD_RETURN_TYPE)
                    methodReturnType = args[1];

                // set the method parameters
                else if (instruction == Instructions::METHOD_PARAMETERS)
                    methodParameters = Lists::subList(args, 1);

                // handle method content declaration begin
                else if (instruction == Instructions::METHOD_BEGIN && methodOffset++ == 0)
                    contentBegun = true;
            }

            // TODO handle field property setters
        }
    }

    /**
     * Debug parsed class and its content.
     */
    void Class::debug() {
        // parse the modifiers of the class
        List<String> modifiers = parseModifiers(this->modifiers);
        // debug the class modifiers if there is any
        if (!modifiers.empty())
            print(Strings::join(modifiers, " ") << ' ');

        // debug the class name
        print("class " << name << ' ');

        // debug the class superclass if it isn't the default superclass
        if (superclass != "Object")
            print("extends " << superclass << ' ');
            
        // debug the class interfaces
        if (!interfaces.empty())
            print("implements " << Strings::join(interfaces, ", ") << ' ');

        // debug the class body
        println('{');

        // TODO debug the class fields
    
        // debuf the class methods
        for (Method* method : methods)
            method->debug();

        // finish debugging
        println("}");
    }

    /**
     * Retrieve a class method with the given signature.
     * @param name method name
     * @param parameters method parameters
     */
    Method* Class::getMethod(String name, List<String> parameters) {
        // get the length of the erquired parameters list
        ulong checkLength = parameters.size();
        // loop through the declared class methods
        for (Method* method : methods) {
            // get the length of the method's parameter list
            ulong methodLength = method->parameters.size();
            // ignore the method if the method name or parameters length does not match
            if (method->name != name || methodLength != checkLength)
                continue;
            // check if the method parameters match
            for (ulong i = 0; i < checkLength; i++) {
                // ignore method if the parameter at the 
                // current index does not match
                if (method->parameters[i] != parameters[i])
                    goto search;
            }
            // method parameters matches, method found
            return method;
        search:;
        }
        // method not found, return a null method pointer
        return nullptr;
    }

    /**
     * Define a method in the class.
     */
    void Class::defineMethod(Method* method) {
        methods.push_back(method);
    }
}
