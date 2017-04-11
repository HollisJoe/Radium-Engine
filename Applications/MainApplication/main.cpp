#include <MainApplication.hpp>

#include <QCommandLineParser>

#include <cstdio>
#include <GuiBase/Utils/KeyMapping.hpp>

int main( int argc, char** argv )
{
    Ra::MainApplication app( argc, argv );

    const uint& fpsMax = app.m_targetFPS;
    const Scalar deltaTime( fpsMax == 0 ? 0.f : 1.f / Scalar( fpsMax ) );

    Ra::Core::Timer::TimePoint t0, t1;

    /* GIZMO_MANIPULATION and VIEWER_LEFT_BUTTON_PICKING_QUERY have to be mapped to the same key /!\ */
    Ra::Gui::mapKeyToAction( Qt::LeftButton, Ra::Gui::KeyMappingAction::TRACKBALL_CAMERA_MANIPULATION );
    Ra::Gui::mapKeyToAction( Qt::Key_F, Ra::Gui::KeyMappingAction::TRACKBALL_CAMERA_ROTATE_AROUND );
    Ra::Gui::mapKeyToAction( Qt::MiddleButton, Ra::Gui::KeyMappingAction::VIEWER_RIGHT_BUTTON_PICKING_QUERY );
    Ra::Gui::mapKeyToAction( Qt::RightButton, Ra::Gui::KeyMappingAction::VIEWER_LEFT_BUTTON_PICKING_QUERY );
    Ra::Gui::mapKeyToAction( Qt::Key_Space, Ra::Gui::KeyMappingAction::VIEWER_RAYCAST_QUERY );
    Ra::Gui::mapKeyToAction( Qt::Key_W, Ra::Gui::KeyMappingAction::VIEWER_TOGGLE_WIREFRAME );
    Ra::Gui::mapKeyToAction( Qt::RightButton, Ra::Gui::KeyMappingAction::GIZMO_MANIPULATION );
    Ra::Gui::mapKeyToAction( Qt::RightButton, Ra::Gui::KeyMappingAction::COLORWIDGET_PRESSBUTTON );

    while ( app.isRunning() )
    {
        t0 = Ra::Core::Timer::Clock::now();
        // Main loop
        app.radiumFrame();

        // Wait for VSync
        Scalar remaining = deltaTime;
        while (remaining > 0.0)
        {
            t1 = Ra::Core::Timer::Clock::now();
            remaining -= Ra::Core::Timer::getIntervalSeconds(t0, t1);
            t0 = t1;
        }
    }

    app.exit();
}
