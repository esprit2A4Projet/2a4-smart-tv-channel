#include "todolist.h"

#include <QLabel>
#include <QToolBar>
#include <QBoxLayout>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QGroupBox>
#include <QWidget>

CToDoList::CToDoList(QWidget *parent) : QWidget(parent)
{


    QGroupBox* groupBox = new QGroupBox("", this);
    groupBox->setStyleSheet("background-color: #ECF0F1");
    groupBox->setMinimumSize(1000, 1000); // Set a minimum size for the group box

    QVBoxLayout* groupBoxLayout = new QVBoxLayout();
    groupBoxLayout->setContentsMargins(20, 20, 20, 20); // Set margins for the layout
    groupBox->setLayout(groupBoxLayout);

    QLabel* pwTitle = new QLabel("To Do List", this);
    pwTitle->setAlignment(Qt::AlignCenter);
    pwTitle->setStyleSheet("font-size: 30pt; margin: 10%;");
    groupBoxLayout->addWidget(pwTitle);

    QHBoxLayout* pHLayoutLabels = new QHBoxLayout();
    groupBoxLayout->addLayout(pHLayoutLabels);

    QLabel* plblPending = new QLabel("En cours", this);
    plblPending->setStyleSheet("font-size: 15pt; font-weight: bold; qproperty-alignment: AlignCenter;");
    pHLayoutLabels->addWidget(plblPending);

    QLabel* plblCompleted = new QLabel("Complété", this);
    plblCompleted->setStyleSheet("font-size: 15pt; font-weight: bold; qproperty-alignment: AlignCenter;");
    pHLayoutLabels->addWidget(plblCompleted);

    QHBoxLayout* pHLayout = new QHBoxLayout();
    groupBoxLayout->addLayout(pHLayout);

    m_pwPending = new QListView(this);

    m_pwPending->setDragEnabled(true);
    m_pwPending->setAcceptDrops(true);
    m_pwPending->setDropIndicatorShown(true);
    m_pwPending->setDefaultDropAction(Qt::MoveAction);

    pHLayout->addWidget(m_pwPending);

    m_pwCompleted = new QListView(this);

    m_pwCompleted->setDragEnabled(true);
    m_pwCompleted->setAcceptDrops(true);
    m_pwCompleted->setDropIndicatorShown(true);
    m_pwCompleted->setDefaultDropAction(Qt::MoveAction);

    pHLayout->addWidget(m_pwCompleted);

    m_pwPending->setModel(new QStringListModel());
    m_pwCompleted->setModel(new QStringListModel());


    groupBox->setStyleSheet("background-image:url(:/tsawer/background.png);");  // Light background

    pwTitle->setStyleSheet("font-size: 30pt; margin: 10%; font-family: Game Of Squids;");  // Improved font

    m_pwPending->setStyleSheet("font-size: 20pt; font-weight: bold; font-family: Open Sans; background-color: #AFEEEE; padding: 5%; border: 1px solid rgb(255, 0, 0);");   // Blue for pending tasks
    m_pwCompleted->setStyleSheet("font-size: 20pt; font-weight: bold; font-family: Open Sans; background-color: #2ECC71; padding: 5%; border: 1px solid #27AE60;"); // Green for completed tasks

    loadToDoListFromFile("todo_list.txt");
}


void CToDoList::onAdd(const QString& itemText)
{
    // Ensure m_pwPending is initialized
        if (!m_pwPending) {
            return;
        }

        // Retrieve the model of the pending list view
        QStringListModel* model = qobject_cast<QStringListModel*>(m_pwPending->model());
        if (!model) {
            return;
        }

        // Add the new to-do item to the model
        QStringList itemList = model->stringList();
        itemList.append(itemText);
        model->setStringList(itemList);
}

void CToDoList::onRemove(const QString& itemText)
{
    // Ensure m_pwPending is initialized
        if (!m_pwPending) {
            return;
        }

        // Retrieve the selected indexes from the pending list view
        QModelIndexList selectedIndexes = m_pwPending->selectionModel()->selectedIndexes();

        // If no item is selected, return
        if (selectedIndexes.isEmpty()) {
            return;
        }

        // Retrieve the model of the pending list view
        QStringListModel* model = qobject_cast<QStringListModel*>(m_pwPending->model());
        if (!model) {
            return;
        }

        // Sort the selected indexes in descending order to ensure proper removal
        std::sort(selectedIndexes.begin(), selectedIndexes.end(), qGreater<QModelIndex>());

        // Remove the selected items from the model
        foreach (const QModelIndex& index, selectedIndexes) {
            model->removeRow(index.row());
        }
}

void CToDoList::saveToDoListToFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    // Save Pending items
    out << "Pending Items:\n";
    QStringListModel* pendingModel = qobject_cast<QStringListModel*>(m_pwPending->model());
    if (pendingModel) {
        QStringList pendingItems = pendingModel->stringList();
        for (const QString& item : pendingItems)
            out << item << "\n";
    }

    // Save Completed items
    out << "\nCompleted Items:\n";
    QStringListModel* completedModel = qobject_cast<QStringListModel*>(m_pwCompleted->model());
    if (completedModel) {
        QStringList completedItems = completedModel->stringList();
        for (const QString& item : completedItems)
            out << item << "\n";
    }

    file.close();
}

//dashbaord
void CToDoList::closeEvent(QCloseEvent *event)
{
    saveToDoListToFile("todo_list.txt"); // Change the filename as needed
    event->accept();
}

void CToDoList::loadToDoListFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    QStringList pendingItems;
    QStringList completedItems;
    QString line;
    enum { Pending, Completed, None } section = None;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();

        if (line == "Pending Items:") {
            section = Pending;
            continue;
        }
        else if (line == "Completed Items:") {
            section = Completed;
            continue;
        }

        switch (section) {
        case Pending:
            if (!line.isEmpty())
                pendingItems.append(line);
            break;
        case Completed:
            if (!line.isEmpty())
                completedItems.append(line);
            break;
        default:
            break;
        }
    }

    file.close();

    // Populate Pending list view
    QStringListModel* pendingModel = qobject_cast<QStringListModel*>(m_pwPending->model());
    if (pendingModel) {
        pendingModel->setStringList(pendingItems);
    }

    // Populate Completed list view
    QStringListModel* completedModel = qobject_cast<QStringListModel*>(m_pwCompleted->model());
    if (completedModel) {
        completedModel->setStringList(completedItems);
    }
}
