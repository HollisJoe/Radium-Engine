#ifndef RA_QT_UTILS_HPP
#define RA_QT_UTILS_HPP

#include <QWidget>
#include <QLayout>
#include <QKeyEvent>

// ============================================================================
namespace Qt_utils {
// ============================================================================

/// Recursively hide/show every widget of a QLayout
static inline
void rec_set_visible( const QLayout &layout, bool visible )
{
    for (int i=0; i<layout.count(); ++i){
        if ( QWidget *w = layout.itemAt(i)->widget() )
            w->setVisible( visible );
        else if ( QLayout *l = layout.itemAt(i)->layout() )
            rec_set_visible( *l, visible );
    }
}

// -----------------------------------------------------------------------------

#if 0
/// convert a key event to a char. Usefull to  distinguish upper/lower case
/// key characters
/// @warning returned value is undefined for multiple key strokes
static inline
const char event_to_char(QKeyEvent* event)
{
    QString t = event->text();
    QChar c = t[0];
    return c.toAscii();
}
#endif

}// END NAMESPACE QT_UTILS =====================================================

#endif // RA_QT_UTILS_HPP
