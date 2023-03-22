project "00-HelloWindow"
    kind "WindowedApp"

    includedirs
    {
        "../../deps/glad/include/",
        "../../deps/glfw/include/"
    }

    files "./**"
    links{ "GLAD", "GLFW" }

    filter "system:linux"
        links{ "dl", "pthread"}

        defines{ "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }
