include "../lib"

project "TestOpenGL"
    kind "WindowedApp"
    files "./**"

    -- We also need the headers
    includedirs "../lib"
    links { "Lib", "GLFW" }

    -- Now we need to add the OpenGL system libraries
    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

