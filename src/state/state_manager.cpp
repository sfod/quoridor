#include "state_manager.hpp"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

#include "logger.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

StateManager::StateManager()
    : win_(), states_(), last_time_pulse_(0.001 * SDL_GetTicks()),
    is_running_(true)
{
    init_sdl_();
    CEGUI::OpenGLRenderer::bootstrapSystem();
    init_resource_provider_();

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().
	    setDefaultImage("TaharezLook/MouseArrow");
}

StateManager::~StateManager()
{
}

void StateManager::change_state(std::shared_ptr<IState> state)
{
    if (!states_.empty()) {
        states_.pop_back();
    }
    states_.push_back(state);

    CEGUI::System::getSingleton().getDefaultGUIContext().
            setRootWindow(state->window().get());
}

void StateManager::handle_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_MOUSEMOTION:
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectMousePosition(static_cast<float>(e.motion.x),
                    static_cast<float>(e.motion.y));
            break;
        case SDL_MOUSEBUTTONDOWN:
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
            break;
        case SDL_MOUSEBUTTONUP:
            CEGUI::System::getSingleton().getDefaultGUIContext().
                injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
            break;
        case SDL_QUIT:
            stop();
            break;
        }
    }

    double current_time_pulse = 0.001 * SDL_GetTicks();
    float diff = static_cast<float>(current_time_pulse - last_time_pulse_);
    CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(diff);
    CEGUI::System::getSingleton().injectTimePulse(diff);
    last_time_pulse_ = current_time_pulse;
}

void StateManager::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    CEGUI::System::getSingleton().renderAllGUIContexts();
    SDL_GL_SwapWindow(win_);
}

void StateManager::init_sdl_()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::exception();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    win_ = SDL_CreateWindow("OpenGL",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED,
                800, 600, SDL_WINDOW_RESIZABLE);

    SDL_CreateRenderer(win_, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GLContext ctx = SDL_GL_CreateContext(win_);
    if (ctx == NULL) {
        throw std::exception();
    }

    SDL_ShowCursor(SDL_DISABLE);
}

void StateManager::init_resource_provider_()
{
    CEGUI::DefaultResourceProvider *rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemes", "resources/schemes/");
    rp->setResourceGroupDirectory("imagesets", "resources/imagesets/");
    rp->setResourceGroupDirectory("fonts", "resources/fonts/");
    rp->setResourceGroupDirectory("layouts", "resources/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "resources/looknfeel/");

    rp->setResourceGroupDirectory("schemas", "resources/xml_schemas/");

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");

    CEGUI::XMLParser *parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup")) {
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");
    }
}

void StateManager::inject_time_pulse_()
{
    double current_time_pulse = 0.001 * SDL_GetTicks();
    float diff = static_cast<float>(current_time_pulse - last_time_pulse_);
    CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(diff);
    CEGUI::System::getSingleton().injectTimePulse(diff);
    last_time_pulse_ = current_time_pulse;
}

}  /* namespace Quoridor */
