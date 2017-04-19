#include "KeyMappingManager.hpp"

#include <Core/Log/Log.hpp>

namespace Ra
{
    namespace Gui
    {
        KeyMappingManager::KeyMappingManager() :
            m_domDocument("Keymapping QDomDocument"),
            m_metaEnumAction(QMetaEnum::fromType<KeyMappingAction>()),
            m_metaEnumKey(QMetaEnum::fromType<Qt::Key>()),
            m_file(new QFile("Configs/default.xml"))
        {
            if( !m_file->open( QIODevice::ReadOnly ) )
            {
                LOG(logERROR) << "Failed to open default keymapping configuration file !";
                delete m_file;
            }
        }

        void KeyMappingManager::bindKeyToAction( int keyCode, KeyMappingAction action )
        {
            m_mapping[action] = keyCode;
        }

        int KeyMappingManager::getKeyFromAction( KeyMappingAction action )
        {
            return m_mapping[action];
        }

        bool KeyMappingManager::actionTriggered( QMouseEvent * event, KeyMappingAction action )
        {
            return event->button() == getKeyFromAction( action );
        }

        bool KeyMappingManager::actionTriggered( QKeyEvent * event, KeyMappingAction action )
        {
            return ( event->key() | event->modifiers() ) == getKeyFromAction( action );
        }

        void KeyMappingManager::loadConfiguration( const char * filename )
        {
            if( !filename )
            {
                filename = "Configs/default.xml";
            }

            if( m_file )
            {
                if( m_file->isOpen() )
                {
                    m_file->close();
                    delete m_file;
                }
            }

            m_file = new QFile( filename );

            if( !m_file->open( QIODevice::ReadOnly ) )
            {
                LOG(logERROR) << "Failed to open default keymapping configuration file !";
                delete m_file;
            }

            if( !m_domDocument.setContent( m_file ) )
            {
                LOG( logERROR ) << "Can't associate XML file to QDomDocument !";
                m_file->close();
            }

            loadConfigurationInternal();
        }

        void KeyMappingManager::loadConfigurationInternal()
        {
            QDomElement domElement = m_domDocument.documentElement();
            QDomNode node = domElement.firstChild();

            if( domElement.tagName() != "keymaps" )
            {
                LOG(logERROR) << "Error while loading keymapping configuration file : no <keymaps> global bounding tag !";
            }

            while( !node.isNull() )
            {
                QDomElement nodeElement = node.toElement();

                loadConfigurationTagsInternal( nodeElement );

                node = node.nextSibling();
            }
        }

        void KeyMappingManager::loadConfigurationTagsInternal( QDomElement& node )
        {
            if(node.tagName() == "keymap") {
                QDomNode nodeChild = node.firstChild();

                std::string keyString = nodeChild.toElement().attribute("value").toStdString();
                std::string typeString = nodeChild.toElement().attribute("type").toStdString();
                std::string modifierString = nodeChild.toElement().attribute("modifier").toStdString();
                nodeChild = nodeChild.nextSibling();
                std::string actionString = nodeChild.toElement().attribute("value").toStdString();

                loadConfigurationMappingInternal( typeString, modifierString, keyString, actionString );
            }
            else
            {
                LOG(logERROR) << "Unrecognized XML keymapping configuration file tag \"" << qPrintable(node.tagName()) << "\" !";
                return;
            }
        }

        void KeyMappingManager::loadConfigurationMappingInternal( const std::string& typeString, const std::string& modifierString,
                                                                  const std::string& keyString, const std::string& actionString )
        {
            KeyMappingAction actionValue = static_cast<KeyMappingAction>( m_metaEnumAction.keyToValue( actionString.c_str() ) );
            Qt::KeyboardModifier modifierValue = getQtModifierValue( modifierString );

            if( typeString == "key" )
            {
                int keyValue = m_metaEnumKey.keyToValue( keyString.c_str() );
                bindKeyToAction( keyValue | modifierValue, actionValue );
            }
            else if( typeString == "mouse" )
            {
                int buttonValue = getQtMouseButtonValue( keyString );
                bindKeyToAction( buttonValue, actionValue );
            }
        }

        Qt::KeyboardModifier KeyMappingManager::getQtModifierValue( const std::string& modifierString )
        {
            Qt::KeyboardModifier modifier = Qt::NoModifier;

            if( modifierString == "ShiftModifier" )
            {
                modifier = Qt::ShiftModifier;
            }
            else if( modifierString == "ControlModifier" )
            {
                modifier = Qt::ControlModifier;
            }
            else if( modifierString == "AltModifier" )
            {
                modifier = Qt::AltModifier;
            }
            else if( modifierString == "MetaModifier" )
            {
                modifier = Qt::MetaModifier;
            }
            else if( modifierString == "KeypadModifier" )
            {
                modifier = Qt::KeypadModifier;
            }
            else if( modifierString == "GroupSwitchModifier" )
            {
                modifier = Qt::GroupSwitchModifier;
            }

            return modifier;
        }

        Qt::MouseButton KeyMappingManager::getQtMouseButtonValue( const std::string& keyString )
        {
            Qt::MouseButton key = Qt::NoButton;

            if( keyString == "LeftButton" )
            {
                key = Qt::LeftButton;
            }
            else if( keyString == "RightButton" )
            {
                key = Qt::RightButton;
            }
            else if( keyString == "MidButton" )
            {
                key = Qt::MidButton;
            }
            else if( keyString == "MiddleButton" )
            {
                key = Qt::MiddleButton;
            }
            else if( keyString == "XButton1" )
            {
                key = Qt::XButton1;
            }
            else if( keyString == "XButton2" )
            {
                key = Qt::XButton2;
            }

            return key;
        }

        void KeyMappingManager::reloadConfiguration()
        {
            if( !m_file->isOpen() )
            {
                return;
            }

            QString filename = m_file->fileName();
            m_file->close();
            loadConfiguration( filename.toStdString().c_str() );
        }

        KeyMappingManager::~KeyMappingManager()
        {
            if( m_file->isOpen() )
            {
                m_file->close();
            }
        }

        RA_SINGLETON_IMPLEMENTATION(KeyMappingManager);
    }
}
