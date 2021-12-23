#pragma once

#include <string>
#include <memory>
#include <vector>

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

//-------------------------------------------------
// Model
//-------------------------------------------------

namespace sg::ogl::resource
{
    class Model
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Model() = delete;

        /**
         * Constructs a new Model object.
         *
         * @param t_path The path to the model file.
         */
        explicit Model(std::string t_path);

        Model(const Model& t_other) = delete;
        Model(Model&& t_other) noexcept = delete;
        Model& operator=(const Model& t_other) = delete;
        Model& operator=(Model&& t_other) noexcept = delete;

        ~Model() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const std::vector<std::unique_ptr<ogl::buffer::Vao>>& GetVaos() const { return m_vaos; }

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        void Load();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The path to the model file.
         */
        std::string m_path;

        /**
         * A Vao for each Shape.
         */
        std::vector<std::unique_ptr<ogl::buffer::Vao>> m_vaos;

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
