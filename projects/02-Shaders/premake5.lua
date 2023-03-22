project "02-Shaders"
    kind "WindowedApp"

    includedirs
    {
        "../../deps/glad/include/",
        "../../deps/glfw/include/",
        "./include"
    }

    files "./src/**"
    links{ "GLAD", "GLFW" }

    filter "system:linux"
        defines{ "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }

