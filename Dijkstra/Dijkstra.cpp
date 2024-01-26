#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

struct Vertex
{
	Vertex(uint16_t vertexId) : id{ vertexId }{}

	uint16_t id;
	uint16_t cost{ std::numeric_limits<uint16_t>::max() };
	std::optional<uint16_t> previousId;
	std::vector <std::pair<Vertex*, uint16_t>> linkedTo;
	bool visited{ false };
};

class Graph
{
public:
	Graph() = default;
	~Graph()
	{
		std::ranges::for_each(vertices, [](Vertex* v) {delete v; });
	}

	Vertex* getVertexById(uint16_t vertexId)
	{
		const auto vertex = std::ranges::find_if(vertices, [vertexId](const Vertex* v) -> bool { return v->id == vertexId; });

		if (vertex!= vertices.end()) {
			return *vertex;
		}
		return nullptr;
	}

	void add(uint16_t vertexId)
	{
		vertices.push_back(new Vertex(vertexId));
	}

	void link(uint16_t linkFromId, uint16_t linkToId, uint16_t cost)
	{
		getVertexById(linkFromId)->linkedTo.push_back(std::make_pair(getVertexById(linkToId), cost));

	}

	bool isVertexVisited(Vertex* vertex)
	{
		return vertex->visited;
	}

	void findShortestPaths(Vertex* vertex)
	{
		if (!isVertexVisited(vertex)) {
			auto vertexLinks = vertex->linkedTo;
			for (auto& [v, c] : vertexLinks)
			{
				if (c+vertex->cost < v->cost)
				{
					v->cost=c+vertex->cost;
					v->previousId = vertex->id;
				}
				vertex->visited=true;
			}

			for(const auto& v : vertexLinks | std::views::keys)
			{
				findShortestPaths(v);
			}
		}
	}

	void print()
	{
		const auto vertex = getVertexById(1);
		vertex->cost = 0;
		findShortestPaths(vertex);
		for(const auto v : vertices)
		{
			std::cout<< v->id << ' ' << v->cost << ' ' << (v->previousId ? v->previousId.value() : -1) << std::endl;
		}
	}

private:
	std::vector<Vertex*> vertices;
};


int main()
{
	Graph g;
	g.add(1);
	g.add(2);
	g.add(3);
	g.add(4);
	g.add(5);
	g.link(1, 2, 5);
	g.link(2, 1, 10);
	g.link(1, 5, 7);
	g.link(2, 3, 4);
	g.link(2, 4, 1);
	g.link(3, 5, 10);
	g.link(3, 1, 1);
	g.link(4, 3, 1);
	g.link(2, 5, 1);
	g.print();
}
