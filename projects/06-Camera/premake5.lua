project "06-Camera"
    kind "WindowedApp"

    includedirs
    {
        "../../deps/stb",
        "../../deps/glad/include",
        "../../deps/glfw/include",
        "../../deps/glm",
        "include",
    }

    files "src/**"
    links{ "STB", "GLAD", "GLFW", "GLM" }

    filter "system:linux"
        defines{ "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }

    filter "system:MAC"
        defines { "_MAC" }

