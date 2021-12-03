#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace sg::ogl
{
    /**
     * Enable or disable OpenGL capabilities.
     */
    class OpenGL
    {
    public:
        //-------------------------------------------------
        // OpenGL states
        //-------------------------------------------------

        /**
         * Specify clear values for the color buffers.
         *
         * @param r red - range [0, 1].
         * @param g green - range [0, 1].
         * @param b blue - range [0, 1].
         * @param a alpha - range [0, 1].
         */
        static void SetClearColor(float t_red, float t_green, float t_blue, float t_alpha)
        {
            glClearColor(t_red, t_green, t_blue, t_alpha);
        }

        /**
         * Clear buffers to preset values.
         */
        static void Clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }

    protected:

    private:

    };
}
