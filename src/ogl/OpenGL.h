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

        /**
         * Enable alpha blending.
         */
        static void EnableAlphaBlending()
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        /**
         * Disable blending.
         */
        static void DisableBlending()
        {
            glDisable(GL_BLEND);
        }

        /**
         * Enable wireframe mode.
         */
        static void EnableWireframeMode()
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        /**
         * Disable wireframe mode.
         */
        static void DisableWireframeMode()
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    protected:

    private:

    };
}
