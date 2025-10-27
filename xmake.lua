local project_name = "infinite"

add_rules("mode.debug", "mode.release")
set_languages("c++20")
add_requires("gtest")
add_defines(
    "NAMESPACE=" .. project_name, 
    "NAMESPACE_DECL=\"namespace " .. project_name .. "\"", 
    "USING_NAMESPACE_DECL=\"using namespace " .. project_name .. "\""
)

target("lib")
    set_basename(project_name)
    set_kind("shared")
    add_rules("lex", "yacc")
    add_files("lib/*.cc")
    add_includedirs("include", { public = true })
    add_cxxflags("-Werror")

target("qwen3")
    set_basename(project_name .. "_qwen3")
    set_kind("shared")
    add_files("arch/qwen3/*.cc")
    add_includedirs("include", { public = true })
    add_includedirs("lib")
    add_deps("lib")
    add_cxxflags("-Werror")

target("bin")
    set_basename(project_name)
    set_kind("binary")
    add_files("bin/*.cc")
    add_includedirs("include", { public = true })
    add_deps("lib")
    add_deps("qwen3")
    add_cxxflags("-Werror")

target("test")
    set_kind("binary")
    add_files("test/*.cc", "bin/*.cc")
    remove_files("bin/main.cc")
    add_includedirs("include", { public = true })
    add_includedirs("lib", "bin", "arch/qwen3")
    add_deps("lib")
    add_deps("qwen3")
    add_packages("gtest")
    add_links("gtest_main")
    add_tests("default")
    set_rundir(os.projectdir() .. "/test")
    add_cxxflags("-Werror")
