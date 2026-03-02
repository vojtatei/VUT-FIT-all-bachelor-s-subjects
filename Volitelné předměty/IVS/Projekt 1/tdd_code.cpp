//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"


Graph::Graph(){}

Graph::~Graph(){}

std::vector<Node*> Graph::nodes() {
    std::vector<Node*> nodes;
    for (Node* node : m_nodes)
    {
        nodes.push_back(node);
    }
    return nodes;
}

std::vector<Edge> Graph::edges() const{
    std::vector<Edge> edges;
    for (auto edge : m_edges)
    {
        edges.push_back(edge);
    }

    return edges;
}

Node* Graph::addNode(size_t nodeId) {
    for (auto node : m_nodes){
        if(node->id == nodeId)
            return nullptr;
    }
    Node* newNode = new Node();
    newNode->id = nodeId;
    m_nodes.push_back(newNode);
    return newNode;

}

bool Graph::addEdge(const Edge& edge){
    if (edge.a == edge.b)
        return false;
    for (const auto& existingedge : edges())
    {
        if (existingedge == edge)
        return false;
    }

    addNode(edge.a);
    addNode(edge.b);
    m_edges.push_back(edge);
        return true;
    
    return true;
}


void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
    for (const auto& edge :edges)
    {
        addNode(edge.a);
        addNode(edge.b);
        addEdge(edge);
    }

}

Node* Graph::getNode(size_t nodeId){
    for (auto node : m_nodes)
    {
        if (node->id == nodeId)
        return node;
    }
    
    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const{
    for (const auto& e : m_edges)
    {
        if((e.a == edge.a && e.b == edge.b) || (e.a == edge.b && e.b == edge.a))
        return true; 
    }
       return false;
}

void Graph::removeNode(size_t nodeId){
    Node* nodetoremove = nullptr;
    for (auto node : m_nodes)
    {
        if (node->id == nodeId)
        {
            nodetoremove = node;
            break;
        }
    }
    if (!nodetoremove)
     throw std::out_of_range("Node with ID " + std::to_string(nodeId) + " does not exist");

    for (auto it = m_edges.begin(); it != m_edges.end(); ) {
        if (it->a == nodeId || it->b == nodeId) {
            it = m_edges.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = m_nodes.begin(); it != m_nodes.end();)
    {
        if(*it == nodetoremove)
        {
            delete nodetoremove;
            it = m_nodes.erase(it);
        }
        else
        ++it;
    } 
}

void Graph::removeEdge(const Edge& edge){
    bool edgefound = false;
    for(auto it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        if(*it == edge)
        {
            m_edges.erase(it);
            edgefound = true;
            break;
        }
    }

    if (m_edges.empty())
        throw std::out_of_range("Graph is empty, cannot remove edge");
    else if(!edgefound)
    throw std::out_of_range("Edge does not exist in the graph");
}

size_t Graph::nodeCount() const{
    return m_nodes.size();
}

size_t Graph::edgeCount() const{
    return m_edges.size();
}

size_t Graph::nodeDegree(size_t nodeId) const{
    bool nodeexist = false;
    for (auto node : m_nodes)
    {
        if(node->id == nodeId)
        {
            nodeexist = true;
            break;
        }
    }
    if (!nodeexist)
     throw std::out_of_range("Node with ID " + std::to_string(nodeId) + " does not exist");
    size_t degree = 0;
    for (const auto& edge : edges())
    {
        if(edge.a == nodeId || edge.b == nodeId)
        {
            degree++;
        }
    }
    return degree;
    
}

size_t Graph::graphDegree() const{
    size_t degree = 0;
    for (const auto& node : m_nodes) {
        size_t nodeDegree = 0;
        for (const auto& edge : m_edges) {
            if (edge.a == node->id || edge.b == node->id) {
                nodeDegree++;
            }
        }
        degree += nodeDegree;
    }
    return degree;
}

void Graph::coloring()
{
    
}
void Graph::clear() {
    for(Node *node : m_nodes)
    {
        delete node;
    }
    m_nodes.clear();
    m_edges.clear();
}

/*** Konec souboru tdd_code.cpp ***/