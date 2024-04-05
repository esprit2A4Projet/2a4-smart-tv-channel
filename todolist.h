#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>

class CToDoList : public QWidget
{
    Q_OBJECT
public:
    CToDoList(QWidget *parent = nullptr);
    void onAdd(const QString& itemText);
    void onRemove(const QString& itemText);
    void loadToDoListFromFile(const QString& filename);
    void saveToDoListToFile(const QString& filename);
    QListView* m_pwPending = nullptr;
    QListView* m_pwCompleted = nullptr;
protected slots:

    void closeEvent(QCloseEvent *event);

private:


    QAction* m_pActAdd = nullptr;
    QAction* m_pActRemove = nullptr;
};


#endif // TODOLIST_H
