#ifndef ASTARALGORITHM_H
#define ASTARALGORITHM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct AstarNode
{
	sf::Vector2i index;
	double g, h, f;
	AstarNode *parent;

	explicit AstarNode() :
		index(0,0),
		g(0.0),
		h(0.0),
		f(0.0),
		parent(nullptr)
	{};
	explicit AstarNode(int x, int y, AstarNode *parent = nullptr) : 
		index(x, y),
		g(0.0),
		h(0.0),
		f(0.0),
		parent(parent)
	{};

	inline bool operator == (const AstarNode &rhs) const
	{
		return (index.x == rhs.index.x && index.y == rhs.index.y);
	};
	inline bool operator != (const AstarNode &rhs) const
	{
		return !(*this == rhs);
	};
	inline bool operator () (const std::shared_ptr<AstarNode> &rhs) const
	{
		return *this == *rhs;
	};
	inline bool operator < (const AstarNode &rhs) const
	{
		return f < rhs.f;
	};
	inline bool operator > (const AstarNode &rhs) const
	{
		return f > rhs.f;
	};
};

class TileMap;
namespace AstarAlgorithm
{
	typedef std::shared_ptr<AstarNode> NodePtr;
	typedef std::vector<NodePtr> NodeList;
	typedef std::vector<AstarNode> NodePath;

	class AstarFcompare
	{
	public:
		inline bool operator () (const NodePtr lhs, const NodePtr rhs) const
		{
			// Inverse comparsion to get smallest value at top
			return rhs->f < lhs->f;
		};
	};

	NodePath findPath(const sf::Vector2i &start, const sf::Vector2i &end, TileMap *tileMap);
};

#endif