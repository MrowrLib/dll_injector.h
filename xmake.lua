add_rules("mode.debug")
set_toolchains("msvc")
set_languages("c++20")

add_repositories("MrowrLib https://github.com/MrowrLib/package-repo.git")

add_requires("Logging", "spdlog")

target("DLL_Injection")
    set_kind("headeronly")
    add_headerfiles("include/(**.h)")
    add_includedirs("include", {public = true})
    add_packages("Logging", {public = true})

target("Example_Injector")
    set_kind("binary")
    add_files("Example_Injector.cpp")
    add_deps("DLL_Injection")
    add_packages("spdlog")

target("Example_Injected")
    set_kind("shared")
    add_files("Example_Injected.cpp")
    add_deps("DLL_Injection")
    add_packages("spdlog")
