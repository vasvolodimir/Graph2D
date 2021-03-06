#include "settingswindow.h"

SettingsWindow::SettingsWindow(QDialog *parent)
    : QDialog(parent),
      m_tabs(nullptr),
      m_ok(nullptr),
      m_selected_algo(BFS),
      m_settings_tab(nullptr)
{
    QSize size = sizeHint();

    this->setFixedSize(size);
    layout();
}

SettingsWindow::~SettingsWindow()
{

}

QSize SettingsWindow::sizeHint() const
{
    return QSize(400, 250);
}

QString SettingsWindow::pathToImages()
{
    return QApplication::applicationDirPath() + "/images/";
}

AlgorithmID SettingsWindow::selectedAlgorithm() const
{
    return m_selected_algo;
}

Tab *SettingsWindow::getSettingsTab() const
{
    return m_settings_tab;
}

void SettingsWindow::layout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *controls = new QHBoxLayout;

    controls->setAlignment(Qt::AlignRight);

    if (createTabs(this, &m_tabs))
        layout->addWidget(m_tabs);

    if (createButton(this, "Ok", this, SLOT(submit()), &m_ok))
        controls->addWidget(m_ok);

    layout->addLayout(controls);
    this->setLayout(layout);
}

QTabWidget *SettingsWindow::createTabs(QWidget *parent, QTabWidget **tabs)
{
    *tabs = new QTabWidget(parent);

    (*tabs)->addTab(new Tab(AlgorithmsTab, parent), "Algorithms");
    (*tabs)->addTab((m_settings_tab = new Tab(SettingsTab, parent)), "Settings");
    (*tabs)->addTab(new Tab(StorageTab, parent), "Storage");

    return *tabs;
}

QPushButton *SettingsWindow::createButton(QWidget *parent, QString title,
 QWidget *handler, const char *slot, QPushButton **btn)
{
    *btn = new QPushButton(parent);
    (*btn)->setText(title);
    (*btn)->setCursor(Qt::PointingHandCursor);

    connect(*btn, SIGNAL(clicked(bool)), handler, slot);

    return *btn;
}

void SettingsWindow::submit()
{
    Tab *tab = qobject_cast<Tab*> (m_tabs->widget(AlgorithmsTab));

    if (!tab)
    {
        this->close();
        LOG_EXIT("Invalid pointer", );
    }

    this->close();
}

void SettingsWindow::setSelectedAlgorithm(QModelIndex index)
{
    if (!index.isValid())
        LOG_EXIT("Invalid model index", );

    m_selected_algo = (AlgorithmID) index.row();
}
