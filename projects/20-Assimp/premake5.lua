project "20-Assimp"
    kind "WindowedApp"

    includedirs
    {
        "include",
        "../../deps/glad/include",
        "../../deps/glfw/include",
        "../../deps/glm",
        "../../deps/stb/",
    }

    files "src/**"
    links{ "assimp", "STB", "GLAD", "GLFW", "GLM" }
    -- Uses system assimp
