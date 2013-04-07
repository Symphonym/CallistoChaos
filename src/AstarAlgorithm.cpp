#include "AstarAlgorithm.h"
#include "TileMap.h"
#include <queue>
#include "Player.h"

namespace AstarAlgorithm
{
	NodeList m_openList, m_closedList;
	sf::Vector2i m_start, m_end;
	TileMap *m_tileMap;

	bool allowedTileType(int tileType)
	{
		return tileType == 5 ||
			tileType == 6;
	};

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
	};

	bool occupiedByEnemy(const sf::Vector2i &index)
	{
		return m_tileMap->getTile(index).isOccupied() && !dynamic_cast<Player*>(m_tileMap->getTile(index).getCharacter());
	}
	void controlNeighbour(AstarNode *node, const sf::Vector2i &index, NodeList &neighbours)
	{
		if((!m_tileMap->getTile(index).isSolid() && !occupiedByEnemy(index)) || (allowedTileType(m_tileMap->getTile(index).getTileType()) && !occupiedByEnemy(index)))
			neighbours.push_back(NodePtr(new AstarNode(index.x, index.y, node)));
	}
	NodeList getNeighbours(AstarNode *node)
	{
		NodeList neighbours;
		sf::Vector2i index(node->index);

		index = sf::Vector2i(node->index.x - 1, node->index.y);
		// Left side
		if(index.x >= 0)
			controlNeighbour(node, index, neighbours);

		index = sf::Vector2i(node->index.x + 1, node->index.y);
		// Right side
		if(index.x < m_tileMap->getMapSize().x)
			controlNeighbour(node, index, neighbours);

		index = sf::Vector2i(node->index.x, node->index.y - 1);
		// Top side
		if(index.y >= 0)
			controlNeighbour(node, index, neighbours);

		index = sf::Vector2i(node->index.x, node->index.y + 1);
		// Bottom side
		if(index.y < m_tileMap->getMapSize().y)
			controlNeighbour(node, index, neighbours);

		return neighbours;
	};

	// Adds legal, neighbouring tiles to openList
	// Returns true if the end node was found amongst the neighbours
	bool addToOpen(AstarNode *node)
	{
		NodeList neighbours(getNeighbours(node));
		for(int i = 0; i < neighbours.size(); i++)
		{
			if(std::find_if(m_openList.begin(), m_openList.end(), *neighbours[i]) == m_openList.end() &&  // Check if in openList
				std::find_if(m_closedList.begin(), m_closedList.end(), *neighbours[i]) == m_closedList.end()) // Check if in closedList
			{
				calculateNodeValues(*neighbours[i]);
				m_openList.push_back(neighbours[i]);

				// If end is a neighbour, return true, having the end node at the top of the stack
				if(*neighbours[i].get() == AstarNode(m_end.x, m_end.y))
					return true;

			}
		}

		return false;
	};

	NodePath tracePath(AstarNode *endNode)
	{
		NodePath path;
		AstarNode *currentNode = endNode;

		// Insert end node itself
		path.insert(path.begin(), *endNode);

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

		if(addToOpen(current.get()))
			return tracePath(m_openList.back().get());

		while(!m_openList.empty())
		{
			// Get node with lowest F value using a priority_queue with inverted sorting
			std::priority_queue<NodePtr, std::vector<NodePtr>, AstarFcompare> prioQueue;
			for(int i = 0; i < m_openList.size(); i++)
				prioQueue.push(m_openList[i]);

			current = prioQueue.top();

			m_openList.erase(std::find_if(m_openList.begin(), m_openList.end(),*current));

			m_closedList.push_back(current);

			// Check if neighbour is end
			if(addToOpen(current.get()))
				return tracePath(m_openList.back().get());
		}	
		
		return tracePath(current.get());
	};
};