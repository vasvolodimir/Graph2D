#include "bfsalgorithm.h"

BFSAlgorithm::BFSAlgorithm(QObject *parent)
    : AbstractAlgorithm(parent)
{

}

BFSAlgorithm::~BFSAlgorithm()
{

}

void BFSAlgorithm::algorithm(Node *start, Node *finish, GraphicsView *view)
{
    int id;
    bool debug, reset, found = false;
    bool *visited = new bool[10]();

    if (!visited)
    {
        LOG_DEBUG("Invalid pointer");
        goto Exit;
    }

    id = start->text().toInt() - 1;
    visited[id] = reset = true;
    m_list.push_back(id);

    /* XXX: For graph debugging */
    debug = true;

    while(!m_list.isEmpty())
    {
        Node *node;
        int current = m_list.front();

        if (debug)
            m_debug.push_back(current + 1);

        m_list.pop_front();
        m_way.push_back(new Vertex(current, false));

        if (!(node = view->findNodeByIndex(current)))
        {
            LOG_DEBUG("Node doesn't exist");
            goto Exit;
        }

        /* Mark opened nodes until not found connected one */
        if (!found)
            node->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));

        for(int i=0; i<m_graph[current].size(); i++)
        {
            if (m_graph[current][i]) /* if connectced with i-node */
            {
                if (!(node = view->findNodeByIndex(i)))
                {
                    LOG_DEBUG("Node doesn't exist");
                    goto Exit;
                }

            /* XXX: Workaround: Found only one way
             * After found way, algorithm not stoping.
             * We continue opening all nodes. This is important in case
             * if want mark more than one way from start to finish point later. */
                if ((node == finish) && !found)
                {
                    markWay(view, finish, reset);
                    found = true;
                    reset = false;
                }

                if (!visited[i]) /* if not visited yet */
                {
                    visited[i] = true;
                    m_list.push_back(i);
                }
            }
        }
    }

    if (debug)
        qDebug() << m_debug;

  Exit:
    delete[] visited;
}