project "14-LightCasters"
    kind "WindowedApp"

    includedirs
    {
        "../../deps/stb",
        "../../deps/glad/include",
        "../../deps/glfw/include",
        "../../deps/glm",
        "include"
    }

    files "src/**"
    links{ "STB", "GLAD", "GLFW", "GLM" }

