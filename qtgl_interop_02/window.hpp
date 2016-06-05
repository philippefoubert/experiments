#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <QWidget>

namespace Ui
{
    class IWindow;
}

class IWindow : public QWidget
{
    Q_OBJECT

public:
    explicit IWindow(QWidget *i_pParent = NULL);
    ~IWindow();

protected:
    void keyPressEvent(QKeyEvent *i_pEvent);

private:
    Ui::IWindow *m_pUi;
};

#endif // WINDOW_HPP_
