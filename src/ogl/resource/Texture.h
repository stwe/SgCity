#pragma once

#include <cstdint>
#include <string>

namespace sg::ogl::resource
{
    class Texture
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        uint32_t id{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Texture() = delete;
        explicit Texture(std::string t_path);
        Texture(std::string t_path, bool t_loadVerticalFlipped);

        Texture(const Texture& t_other) = delete;
        Texture(Texture&& t_other) noexcept = delete;
        Texture& operator=(const Texture& t_other) = delete;
        Texture& operator=(Texture&& t_other) noexcept = delete;

        ~Texture() noexcept;

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        void Load();

        //-------------------------------------------------
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        void Unbind() const;
        void BindForReading(uint32_t t_textureUnit);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::string m_path;
        bool m_loadVerticalFlipped{ false };
        int m_width{ 0 };
        int m_height{ 0 };
        int m_format{ 0 };
        int m_channels{ 0 };

        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        void CreateId();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void LoadFromFile();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
