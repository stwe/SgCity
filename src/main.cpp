#include "Log.h"
#include "Application.h"
#include "SgException.h"

int main()
{
    sg::Log::Init();

    sg::Log::SG_LOG_DEBUG("[main()] Starting main.");
    sg::Log::SG_LOG_DEBUG("[main()] Logger was initialized.");

    try
    {
        sg::Application application;
        application.Run();

        return EXIT_SUCCESS;
    }
    catch (const sg::SgException& e)
    {
        sg::Log::SG_LOG_ERROR("SgException {}", e.what());
    }
    catch (const std::exception& e)
    {
        sg::Log::SG_LOG_ERROR("Standard Exception: {}", e.what());
    }
    catch ( ... )
    {
        sg::Log::SG_LOG_ERROR("Unknown Exception. No details available.");
    }

    return EXIT_FAILURE;
}
