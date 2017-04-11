#include "KeyMapping.hpp"

#include <map>

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
    }
}