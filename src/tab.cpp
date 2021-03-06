#include "tab.h"
#include "bfsalgorithm.h"

Tab::Tab(int type, QWidget *parent)
    : QWidget(parent),
      m_list(nullptr),
      m_storage(nullptr),
      m_settings(nullptr),
      m_little_bit(nullptr),
      m_biggest_bit(nullptr)
{
    switch(type)
    {
        case TabType::AlgorithmsTab:
        createAlgorithmsTab(this, &m_list);
        break;

        case TabType::SettingsTab:
        createSettingsTab(this, &m_settings);
        break;

        case TabType::StorageTab:
        createStorageTab(this, &m_storage);
        break;

        default:
        LOG_DEBUG("Invalid tab type!");
        break;
    }

    layout();
}

Tab::~Tab()
{

}

QRadioButton *Tab::getLittleBitRB() const
{
    return m_little_bit;
}

QRadioButton *Tab::getBiggestBitRB() const
{
    return m_biggest_bit;
}

void Tab::layout()
{
    QVBoxLayout *layout = new QVBoxLayout;

    if (m_list)
        layout->addWidget(m_list);

    if (m_storage)
        layout->addWidget(m_storage);

    if (m_settings)
        layout->addWidget(m_settings);

    this->setLayout(layout);
}

QListWidget *Tab::createAlgorithmsTab(QWidget *parent, QListWidget **list)
{
    QStringList lst;
    QListWidgetItem *item;
    QString path = SettingsWindow::pathToImages();
    SettingsWindow *sw = qobject_cast<SettingsWindow*> (this->parent());

    lst << "BFS" << "DFS" << "Dejikstra";

    *list = new QListWidget(parent);
    (*list)->setIconSize(QSize(10, 10));

    if (!sw)
    {
        LOG_DEBUG("Invalid pointer");
    }
    else
    {
        connect(*list, SIGNAL(clicked(QModelIndex)), sw,
          SLOT(setSelectedAlgorithm(QModelIndex)));
    }

    for(int i=0; i<lst.size(); i++)
    {
        item = new QListWidgetItem(lst[i], *list);
        item->setIcon(QPixmap(path + "algorithm.png"));
    }

    return *list;
}

QWidget *Tab::createStorageTab(QWidget *parent, QWidget **tab)
{
    QVBoxLayout *layout = new QVBoxLayout;

    (*tab) = new QWidget(parent);
    layout->addWidget(createPushButton("Upload", SLOT(upload())));
    layout->addWidget(createPushButton("Download", SLOT(download())));
    (*tab)->setLayout(layout);

    return *tab;
}

QPushButton *Tab::createPushButton(QString title, const char *slot)
{
    QPushButton *btn = new QPushButton(title);

    btn->setCursor(Qt::PointingHandCursor);
    connect(btn, SIGNAL(clicked(bool)), this, slot);

    return btn;
}

QWidget *Tab::createSettingsTab(QWidget *parent, QWidget **settings)
{
    QVBoxLayout *layout;

    *settings = new QWidget(parent);
    layout = new QVBoxLayout;
    m_little_bit = new QRadioButton("From little bit");
    m_biggest_bit = new QRadioButton("From biggest bit");
    m_little_bit->setChecked(true);

    layout->addWidget(m_little_bit);
    layout->addWidget(m_biggest_bit);
    (*settings)->setLayout(layout);

    return *settings;
}

void Tab::writeGraph(QVector<QVector<int> > graph, QTextStream &stream) const
{
    for(int i=0; i<graph.size(); i++)
    {
        for(int j=0; j<graph[i].size(); j++)
        {
            stream << graph[i][j];

            if (j < graph[i].size() - 1)
                stream << " ";
        }

        if (i < graph.size() - 1)
            stream << "\n";
    }
}

void Tab::writeUIConf(QString filename) const
{
    QFile file;
    QTextStream stream;
    QVector<Node*> nodes;
    size_t offset = 10; // Node's radius / 2
    GraphicsView *view = MainWindow::instance().getView();

    if (!view)
        LOG_EXIT("Invalid pointer", );

    file.setFileName(filename + ".conf");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        LOG_EXIT("Can't open file!", );

    stream.setDevice(&file);
    nodes = view->getNodes();

    for(int i=0; i<nodes.size(); i++)
    {
        stream << nodes[i]->rect().x() + offset << " "
          << nodes[i]->rect().y() + offset << "\n";
    }

    file.close();
}

void Tab::readUIConf(QString filename, QTextStream &stream) const
{
    QPointF point;
    GraphicsView *view = MainWindow::instance().getView();
    QVector<QString> tooltips = readNodeToolTips(filename);
    size_t radius = 20;
    size_t i = 0;


    if (!view || tooltips.isEmpty())
        LOG_EXIT("Invalid paramter", );

    while (!stream.atEnd())
    {
        Node *node;
        QString subline;
        QString line = stream.readLine();

        subline = line.split(" ")[0];
        point.setX(subline.toDouble());
        subline = line.split(" ")[1];
        point.setY(subline.toDouble());

        node = view->addNode(radius, QBrush(Qt::white, Qt::SolidPattern),
         point);
        node->setToolTip(tooltips[i++]);
    }
}

void Tab::readGraph(QString filename) const
{
    QTextStream stream;
    QVector<QVector<int> > graph;
    QFile file(filename);
    GraphicsView *view = MainWindow::instance().getView();

    if (!view)
        LOG_EXIT("Invalid pointer", );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        LOG_EXIT("Can't open file!", );

    stream.setDevice(&file);

    while(!stream.atEnd())
    {
        QVector<int> temp;
        QString line = stream.readLine();
        QStringList list = line.split(" ");

        for(int i=0; i<list.size(); i++)
            temp.push_back(list[i].toInt());

        graph.push_back(temp);
    }

    file.close();
    view->restoreEdges(graph);
}

void Tab::writeNodeToolTips(QString filename) const
{
    QFile file;
    QTextStream stream;
    QVector<Node*> nodes;
    GraphicsView *view = MainWindow::instance().getView();

    if (!view)
        LOG_EXIT("Invalid pointer", );

    file.setFileName(filename + "_tt.conf");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        LOG_EXIT("Can't open file!", );

    stream.setDevice(&file);
    nodes = view->getNodes();

    for(int i=0; i<nodes.size(); i++)
    {
        QString tooltip = !nodes[i]->toolTip().isEmpty() ?
          nodes[i]->toolTip() : "";

        stream << nodes[i]->text() << " "
          << tooltip << "\n";
    }

    file.close();
}

QVector<QString> Tab::readNodeToolTips(QString filename) const
{
    QFile file;
    QTextStream stream;
    QVector<QString> tooltips;

    if (filename.isEmpty())
        LOG_EXIT("Filename is empty", QVector<QString>());

    file.setFileName(filename.split(".")[0] + "_tt.conf");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        LOG_EXIT("Can't open file!: " << filename, QVector<QString>());

    stream.setDevice(&file);

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        tooltips.push_back(line.split(" ")[1]);
    }

    file.close();

    return tooltips;
}

void Tab::download()
{
    QFile file;
    QString filename;
    QTextStream stream;
    QVector<QVector<int> > graph;
    /* XXX: used only for accessing to initGraph() method. Not correct logic */
    BFSAlgorithm algorithm;

    algorithm.initGraph();
    graph = algorithm.getGraph();

    if (graph.isEmpty())
        LOG_EXIT("Canvas is empty!", );

    filename = QFileDialog::getSaveFileName(this, "Save file...", "", "*.txt");

    if (filename.isEmpty())
        LOG_EXIT("Filename is empty", );

    file.setFileName(filename + ".txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        LOG_EXIT("Can't open file!", );

    stream.setDevice(&file);
    writeGraph(graph, stream);
    file.close();
    writeUIConf(filename);
    writeNodeToolTips(filename);
}

void Tab::upload()
{
    QFile file;
    QString filename;
    QTextStream stream;

    filename = QFileDialog::getOpenFileName(this, "Open file...", "", "*.txt");

    if (filename.isEmpty())
        LOG_EXIT("Filename is empty", );

    file.setFileName(filename.split(".")[0] + ".conf");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        LOG_EXIT("Can't open file!: " << filename, );

    stream.setDevice(&file);
    readUIConf(filename, stream);
    file.close();
    readGraph(filename);
}
