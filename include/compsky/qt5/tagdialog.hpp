#ifndef __TAGDIALOG__
#define __TAGDIALOG__

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>

namespace compsky {
namespace qt5 {
class TagDialog : public QDialog {
 public:
    explicit TagDialog(QString title,  QString str,  QWidget* parent = 0);
    ~TagDialog();
    QLineEdit* name_edit;
 private:
    QDialogButtonBox* btn_box;
    QLabel* guide;
    QVBoxLayout* l;
};
}
}
#endif
