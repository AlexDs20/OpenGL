project "03-Textures"
    kind "WindowedApp"

    includedirs {
            "../../deps/stb/",
            "../../deps/glad/include/",
            "../../deps/glfw/include/",
            "./include"}

    files "./src/**"
    links{ "STB", "GLAD", "GLFW" }

    filter "system:linux"
        defines{ "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }
