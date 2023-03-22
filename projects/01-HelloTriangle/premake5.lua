project "01-HelloTriangle"
    kind "WindowedApp"

    includedirs
    {
        "../../deps/glad/include/",
        "../../deps/glfw/include/"
    }

    files "./**"
    links{ "GLAD", "GLFW" }

    filter "system:linux"
        defines{ "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }

