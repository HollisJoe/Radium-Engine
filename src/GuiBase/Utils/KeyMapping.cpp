#include "KeyMapping.hpp"

#include <map>
#include <fstream>
#include <Core/Log/Log.hpp>

namespace Ra
{
    namespace Gui
    {
        std::map<KeyMappingAction, int> g_actions_mapped;

        int getKeyFromAction( const KeyMappingAction action )
        {
            return g_actions_mapped[action];
        }

        void mapKeyToAction( const int key, const KeyMappingAction action )
        {
            g_actions_mapped[action] = key;
        }

        void loadDefaultKeymapConfiguration()
        {
            /* GIZMO_MANIPULATION and VIEWER_LEFT_BUTTON_PICKING_QUERY have to be mapped to the same key /!\ */
            Ra::Gui::mapKeyToAction( Qt::MiddleButton, Ra::Gui::KeyMappingAction::TRACKBALL_CAMERA_MANIPULATION );
            Ra::Gui::mapKeyToAction( Qt::Key_F, Ra::Gui::KeyMappingAction::TRACKBALL_CAMERA_ROTATE_AROUND );
            Ra::Gui::mapKeyToAction( Qt::RightButton, Ra::Gui::KeyMappingAction::VIEWER_RIGHT_BUTTON_PICKING_QUERY );
            Ra::Gui::mapKeyToAction( Qt::LeftButton, Ra::Gui::KeyMappingAction::VIEWER_LEFT_BUTTON_PICKING_QUERY );
            Ra::Gui::mapKeyToAction( Qt::Key_Space, Ra::Gui::KeyMappingAction::VIEWER_RAYCAST_QUERY );
            Ra::Gui::mapKeyToAction( Qt::Key_Z, Ra::Gui::KeyMappingAction::VIEWER_TOGGLE_WIREFRAME );
            Ra::Gui::mapKeyToAction( Qt::LeftButton, Ra::Gui::KeyMappingAction::GIZMO_MANIPULATION );
            Ra::Gui::mapKeyToAction( Qt::LeftButton, Ra::Gui::KeyMappingAction::COLORWIDGET_PRESSBUTTON );
        }

        void saveKeymapConfiguration( const std::string filename )
        {
            std::ofstream configFile( filename );

            if( configFile.is_open() )
            {
                for( std::map<KeyMappingAction, int>::iterator it = g_actions_mapped.begin() ;
                     it != g_actions_mapped.end() ;
                     ++it )
                {
                    configFile << it->first << std::endl;
                    configFile << it->second << std::endl;
                }

                configFile.close();
            }
            else
            {
                LOG( logERROR ) << "Unable to create keymap configuration file : \"" << filename << "\"";
            }
        }

        void loadKeymapConfiguration( const std::string filename )
        {
            bool problems = false;

            std::string stringAction, stringKey;
            std::ifstream configFile ( filename );

            if( configFile.is_open() )
            {
                while( std::getline( configFile, stringAction ) )
                {
                    if( !std::getline( configFile, stringKey ) )
                    {
                        problems = true;
                        LOG( logERROR ) << "Unable to read configuration file, it has an incorrect format."
                                           "Default keymap configuration loaded.";
                        loadDefaultKeymapConfiguration();
                        break;
                    }

                    KeyMappingAction action = static_cast<KeyMappingAction>( std::stoi( stringAction.c_str() ) );
                    int key = std::stoi( stringKey.c_str() );

                    mapKeyToAction( key, action );
                }

                configFile.close();
            }
            else
            {
                problems = true;
                LOG( logERROR ) << "Unable to load keymap configuration file : \"" << filename << "\".";
            }

            if( problems )
            {
                LOG( logERROR ) << "Keymap configuration loading finished with errors.";
            }
        }
    }
}