# CMakeList.txt: проект CMake для SPO_6_labs; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SPO_6_labs")

enable_language(ASM_MASM)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (SPO_6_labs "SPO_6_labs.cpp" "SPO_6_labs.h" "labs/lab1.cpp" "labs/lab2.cpp" "labs/lab3.cpp" "labs/lab4.cpp" "labs/lab5.cpp" "labs/lab5_asm.asm" "labs/lab6.cpp")

add_library(FuzzyRelationsDll SHARED
    "dll/FuzzyRelations.cpp"
    "dll/FuzzyRelations.h"
)

target_compile_definitions(FuzzyRelationsDll PRIVATE FUZZY_DLL_EXPORTS)

target_link_libraries(SPO_6_labs PRIVATE FuzzyRelationsDll)

add_custom_command(TARGET SPO_6_labs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:FuzzyRelationsDll>
        $<TARGET_FILE_DIR:SPO_6_labs>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SPO_6_labs PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
