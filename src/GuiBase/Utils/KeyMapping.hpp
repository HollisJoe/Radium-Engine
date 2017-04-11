#ifndef RADIUMENGINE_KEYMAPPING_HPP
#define RADIUMENGINE_KEYMAPPING_HPP

#include <Core/CoreMacros.hpp>
#include <QtCore/Qt>

namespace Ra
{
    namespace Gui
    {
        enum KeyMappingAction : uint
        {
            TRACKBALL_CAMERA_MANIPULATION = 0,
            TRACKBALL_CAMERA_ROTATE_AROUND,
            VIEWER_TOGGLE_WIREFRAME,
            VIEWER_LEFT_BUTTON_PICKING_QUERY,
            VIEWER_RIGHT_BUTTON_PICKING_QUERY,
            VIEWER_RAYCAST_QUERY,
            GIZMO_MANIPULATION,
            COLORWIDGET_PRESSBUTTON
        };

        int getKeyFromAction( const KeyMappingAction action );
        void mapKeyToAction( const int key, const KeyMappingAction action );
    }
}

#endif //RADIUMENGINE_KEYMAPPING_H
