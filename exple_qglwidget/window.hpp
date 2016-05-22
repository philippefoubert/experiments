// window.hpp

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <QWidget>

namespace Ui
{
    class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *i_pParent = NULL);
    ~Window();

protected:
    void keyPressEvent(QKeyEvent *i_pEvent);

private:
    Ui::Window *m_pUi;
};

#endif // WINDOW_HPP_
