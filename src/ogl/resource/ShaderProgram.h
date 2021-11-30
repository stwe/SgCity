#pragma once

#include <cstdint>
#include <string>

namespace sg::ogl::resource
{
    class ShaderProgram
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        uint32_t id{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderProgram() = delete;
        explicit ShaderProgram(std::string t_path);

        ShaderProgram(const ShaderProgram& t_other) = delete;
        ShaderProgram(ShaderProgram&& t_other) noexcept = delete;
        ShaderProgram& operator=(const ShaderProgram& t_other) = delete;
        ShaderProgram& operator=(ShaderProgram&& t_other) noexcept = delete;

        ~ShaderProgram() noexcept;

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        void Load();

        //-------------------------------------------------
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        void Unbind() const;

    protected:

    private:
        //-------------------------------------------------
        // Type
        //-------------------------------------------------

        struct Uniform
        {
            std::string type;
            std::string name;
        };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::string m_path;

        uint32_t m_vertexShaderId{ 0 };
        uint32_t m_fragmentShaderId{ 0 };

        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        void CreateId();

        //-------------------------------------------------
        // Add shader types
        //-------------------------------------------------

        void AddVertexShader(const std::string& t_shaderCode);
        void AddFragmentShader(const std::string& t_shaderCode);

        //-------------------------------------------------
        // Shader
        //-------------------------------------------------

        static uint32_t CreateShaderObject(int32_t t_shaderType);
        static void CompileShader(uint32_t t_shaderId, const std::string& t_shaderCode);
        static void CheckCompileStatus(uint32_t t_shaderId);
        void FindUniforms(const std::string& t_shaderCode);

        uint32_t AddShader(const std::string& t_shaderCode, int32_t t_shaderType);

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
