#include "AstarAlgorithm.h"
#include "TileMap.h"
#include <queue>
#include <iostream>
namespace AstarAlgorithm
{
	NodeList m_openList, m_closedList;
	sf::Vector2i m_start, m_end;
	TileMap *m_tileMap;

	int calculateValueG(const AstarNode &node)
	{
		return 10;
	};
	int calculateValueH(const AstarNode &node)
	{
		return std::abs(m_end.x - node.index.x) + std::abs(m_end.y - node.index.y);
	};
	void calculateNodeValues(AstarNode &node)
	{
		node.g = node.parent->g + calculateValueG(node);
		node.h = calculateValueH(node);
		node.f = node.g + node.h;
		std::cout << "X(" << node.index.x << ") Y(" << node.index.y << ") F(" << node.f << ")" << std::endl;
	};

	NodeList getNeighbours(AstarNode &node)
	{
		NodeList neighbours;

		// Left side
		if(node.index.x - 1 >= 0)
			if(!m_tileMap->getTile(node.index.x - 1, node.index.y).isSolid())
				neighbours.push_back(NodePtr(new AstarNode(node.index.x - 1, node.index.y, &node)));

		// Right side
		if(node.index.x + 1 < m_tileMap->getMapSize().x)
			if(!m_tileMap->getTile(node.index.x + 1, node.index.y).isSolid())
				neighbours.push_back(NodePtr(new AstarNode(node.index.x + 1, node.index.y, &node)));

		// Top side
		if(node.index.y - 1 >= 0)
			if(!m_tileMap->getTile(node.index.x, node.index.y - 1).isSolid())
				neighbours.push_back(NodePtr(new AstarNode(node.index.x, node.index.y - 1, &node)));

		// Bottom side
		if(node.index.y + 1 < m_tileMap->getMapSize().y)
			if(!m_tileMap->getTile(node.index.x, node.index.y + 1).isSolid())
				neighbours.push_back(NodePtr(new AstarNode(node.index.x, node.index.y + 1, &node)));

		return neighbours;
	};

	// Adds legal, neighbouring tiles to openList
	// Returns true if the end node was found amongst the neighbours
	bool addToOpen(AstarNode &node)
	{
		NodeList neighbours(getNeighbours(node));
		for(int i = 0; i < neighbours.size(); i++)
			if(std::find_if(m_openList.begin(), m_openList.end(), *neighbours[i]) == m_openList.end() &&  // Check if in openList
				std::find_if(m_closedList.begin(), m_closedList.end(), *neighbours[i]) == m_closedList.end()) // Check if in closedList
			{
				// If end is a neighbour
				if(*neighbours[i].get() == AstarNode(m_end.x, m_end.y))
					return true;

				calculateNodeValues(*neighbours[i]);
				m_openList.push_back(neighbours[i]);
			}

		return false;
	};

	NodePath tracePath(AstarNode &endNode)
	{
		NodePath path;
		AstarNode *currentNode = &endNode;
		while(currentNode->parent != nullptr)
		{
			path.insert(path.begin(), *currentNode);
			currentNode = currentNode->parent;
		}

		return path;
	};

	NodePath findPath(const sf::Vector2i &start, const sf::Vector2i &end, TileMap *tileMap)
	{
		m_start = start;
		m_end = end;

		m_openList.clear();
		m_closedList.clear();
		m_tileMap = tileMap;

		// Push start node to closedList
		NodePtr current = NodePtr(new AstarNode(start.x, start.y));
		m_closedList.push_back(current);

		if(addToOpen(*current.get()))
			return tracePath(*m_openList.back());

		while(!m_openList.empty())
		{
			// Get node with lowest F value
			current = *std::min_element(m_openList.begin(), m_openList.end());

			m_openList.erase(std::find_if(m_openList.begin(), m_openList.end(),*current));

			m_closedList.push_back(current);

			// Check if neighbour is end
			if(addToOpen(*current.get()))
				return tracePath(*m_openList.back());
		}	
		
		return tracePath(*current.get());
	};
};