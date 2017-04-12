#include <QApplication>
#include <QTimer>

#include <Core/Containers/MakeShared.hpp>

#include <GuiBase/Viewer/Viewer.hpp>
#include <GuiBase/TimerData/FrameTimerData.hpp>

#include <Engine/Renderer/Light/SpotLight.hpp>
#include <Engine/Managers/EntityManager/EntityManager.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderConfigFactory.hpp>

#include <MattApp.hpp>

#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>
#include <Engine/Renderer/Mesh/Mesh.hpp>

#include <Engine/RadiumEngine.hpp>
#include <GuiBase/Utils/KeyMapping.hpp>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

    /// Our minimal application uses QTimer to be called at a regular frame rate.
    class MattApp : public QApplication
    {
    public:
        MattApp(int& argc, char** argv)
                : QApplication(argc, argv), _engine(Ra::Engine::RadiumEngine::createInstance()),
                  _task_queue(new Ra::Core::TaskQueue(std::thread::hardware_concurrency() - 1)), _frame_timer(nullptr),
                  _target_fps(60)
        {

            _frame_timer = new QTimer(this);
            _frame_timer->setInterval(1000 / _target_fps);
            connect(_frame_timer, &QTimer::timeout, this, &MattApp::frame);
        }

        void fitToScene()
        {
            Ra::Core::Aabb aabb;

            std::vector<std::shared_ptr<Ra::Engine::RenderObject>> ros;
            _engine->getRenderObjectManager()->getRenderObjects( ros );

            for ( auto ro : ros )
            {
                auto mesh = ro->getMesh();
                auto pos = mesh->getGeometry().m_vertices;

                for ( auto& p : pos )
                {
                    p = ro->getLocalTransform() * p;
                }

                Ra::Core::Vector3 bmin = pos.getMap().rowwise().minCoeff().head<3>();
                Ra::Core::Vector3 bmax = pos.getMap().rowwise().maxCoeff().head<3>();

                aabb.extend( bmin );
                aabb.extend( bmax );
            }

            _viewer.fitCameraToScene( aabb );
        }

        ~MattApp()
        {
            _engine->cleanup();
        }


    public slots:

        /// This function is the basic "game loop" iteration of the engine.
        /// It starts the rendering then advance all systems by one frame.
        void frame()
        {
            // We use a fixed time step, but it is also possible
            // to check the time from last frame.
            const Scalar dt = 1.f / Scalar(_target_fps);

            // Starts the renderer
            _viewer.startRendering(dt);

            // Collect and run tasks
            _engine->getTasks(_task_queue.get(), dt);
            _task_queue->startTasks();
            _task_queue->waitForTasks();
            _task_queue->flushTaskQueue();

            // Finish the frame
            _viewer.waitForRendering();
            //_viewer.update();
            _viewer.repaint();
            _engine->endFrameSync();
        }

    public:
        // Our instance of the engine
        std::unique_ptr<Ra::Engine::RadiumEngine> _engine;

        // Task queue
        std::unique_ptr<Ra::Core::TaskQueue> _task_queue;

        // Pointer to Qt/OpenGL Viewer widget.
        Ra::Gui::Viewer _viewer;

        // Timer to wake us up at every frame start.
        QTimer* _frame_timer;

        // Our framerate
        uint _target_fps;

    }; // end class



int main(int argc, char* argv[])
{

    // Create default format for Qt.
    QSurfaceFormat format;
    format.setVersion( 4, 1 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    format.setDepthBufferSize( 24 );
    format.setStencilBufferSize( 8 );
    //format.setSamples( 16 );
    format.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
    format.setSwapInterval( 0 );
    QSurfaceFormat::setDefaultFormat( format );

    // Create app
    MattApp app(argc, argv);
    app._engine->initialize();
    app._viewer.show();

    // Load Blinn-Phong shader
    Ra::Engine::ShaderConfiguration bpConfig("BlinnPhong");
    bpConfig.addShader(Ra::Engine::ShaderType_VERTEX, "Shaders/BlinnPhong.vert.glsl");
    bpConfig.addShader(Ra::Engine::ShaderType_FRAGMENT, "Shaders/BlinnPhong.frag.glsl");
    Ra::Engine::ShaderConfigurationFactory::addConfiguration(bpConfig);

    // Load Cartoon shader
    Ra::Engine::ShaderConfiguration cConfig("Cartoon");
    cConfig.addShader(Ra::Engine::ShaderType_VERTEX, "Shaders/Cartoon.vert.glsl");
    cConfig.addShader(Ra::Engine::ShaderType_FRAGMENT, "Shaders/Cartoon.frag.glsl");
    Ra::Engine::ShaderConfigurationFactory::addConfiguration(cConfig);

    // Load Lines shader (raycast, debug)
    Ra::Engine::ShaderConfiguration lConfig("Lines");
    lConfig.addShader(Ra::Engine::ShaderType_VERTEX, "Shaders/Lines.vert.glsl");
    lConfig.addShader(Ra::Engine::ShaderType_FRAGMENT, "Shaders/Lines.frag.glsl");
    Ra::Engine::ShaderConfigurationFactory::addConfiguration(lConfig);

    // Create one system
    Ra::Engine::System* sys = new MinimalSystem;
    app._engine->registerSystem("Minimal system", sys);

    // Create and initialize entity and component (BlinnPhongTorus)
    Ra::Engine::Entity * e_simpleTorus = app._engine->getEntityManager()->createEntity("BlinnPhongTorus");
    Ra::Engine::Component * c_simpleTorus = new SimpleTorus;
    e_simpleTorus->addComponent(c_simpleTorus);
    sys->registerComponent(e_simpleTorus, c_simpleTorus);
    c_simpleTorus->initialize();

    app.fitToScene();

    /* GIZMO_MANIPULATION and VIEWER_LEFT_BUTTON_PICKING_QUERY have to be mapped to the same key /!\ */
    Ra::Gui::mapKeyToAction( Qt::LeftButton, Ra::Gui::KeyMappingAction::TRACKBALL_CAMERA_MANIPULATION );
    Ra::Gui::mapKeyToAction( Qt::Key_F, Ra::Gui::KeyMappingAction::TRACKBALL_CAMERA_ROTATE_AROUND );
    Ra::Gui::mapKeyToAction( Qt::MiddleButton, Ra::Gui::KeyMappingAction::VIEWER_RIGHT_BUTTON_PICKING_QUERY );
    Ra::Gui::mapKeyToAction( Qt::RightButton, Ra::Gui::KeyMappingAction::VIEWER_LEFT_BUTTON_PICKING_QUERY );
    Ra::Gui::mapKeyToAction( Qt::Key_Space, Ra::Gui::KeyMappingAction::VIEWER_RAYCAST_QUERY );
    Ra::Gui::mapKeyToAction( Qt::Key_W, Ra::Gui::KeyMappingAction::VIEWER_TOGGLE_WIREFRAME );
    Ra::Gui::mapKeyToAction( Qt::RightButton, Ra::Gui::KeyMappingAction::GIZMO_MANIPULATION );
    Ra::Gui::mapKeyToAction( Qt::RightButton, Ra::Gui::KeyMappingAction::COLORWIDGET_PRESSBUTTON );

    Ra::Gui::saveKeymapConfiguration( "config_matthieu.txt" );

    // Start the app.
    app._frame_timer->start();
    return app.exec();
}
