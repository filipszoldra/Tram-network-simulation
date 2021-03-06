#include "graph.h"

#include <queue>
#include <unordered_map>

std::pair<std::list<Edge *>, std::list<TrafficLight *>> Graph::findPath(Node *source, Node *target)
{
    typedef std::pair<Node *, float> nodeDistancePair;

    struct CompareNodeDistance
    {
        bool operator()(nodeDistancePair const &p1, nodeDistancePair const &p2) { return p1.second > p2.second; }
    };

    std::priority_queue<nodeDistancePair, std::vector<nodeDistancePair>, CompareNodeDistance> queue;
    std::unordered_map<Node *, Edge *> previous;
    std::unordered_map<Node *, float> distance;

    distance[source] = 0;

    for (auto e : source->m_outgoingEdges)
    {
        previous[e->m_head] = e;
        distance[e->m_head] = e->m_length;
        queue.push(std::make_pair(e->m_head, e->m_length));
    }

    nodeDistancePair pair = queue.top();
    queue.pop();

    while (pair.first != target)
    {
        for (auto e : pair.first->m_outgoingEdges)
        {
            if (previous[e->m_head] == nullptr)
            {
                previous[e->m_head] = e;
                distance[e->m_head] = pair.second + e->m_length;
                queue.push(std::make_pair(e->m_head, pair.second + e->m_length));
            }
        }

        pair = queue.top();
        queue.pop();
    }

    std::list<TrafficLight *> trafficLights;
    std::list<Edge *> path;

    auto edge = previous[target];
    path.push_front(edge);
    while (edge->m_tail != source)
    {
        edge = previous[edge->m_tail];
        path.push_front(edge);

        if (edge->m_head->isTrafficLight())
            trafficLights.push_front((TrafficLight *)edge->m_head);
    }

    return std::make_pair(path, trafficLights);
}
