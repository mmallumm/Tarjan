#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

struct Node {
  int nodeId;
  std::vector<int> nodeDependences;
};

namespace {
typedef std::map<int, int> ColorDictionary;
typedef std::pair<int, const int> NodeColor;

enum TarjanColor { WHITE = 0, BLACK = 1, GRAY = 2 };

Node& nodeById_(std::vector<Node>& graph, int nodeId) {
  for (auto& node : graph) {
    if (node.nodeId == nodeId) {
      return node;
    }
  }
}

std::vector<Node> inverse_(const std::vector<Node>& graph) {
  std::vector<Node> inversedGraph = graph;
  for (auto& node : inversedGraph) {
    node.nodeDependences.clear();
  }

  for (auto& node : graph) {
    for (auto& dependenceId : node.nodeDependences) {
      Node& insertNode = nodeById_(inversedGraph, dependenceId);
      insertNode.nodeDependences.push_back(node.nodeId);
    }
  }

  return inversedGraph;
}

bool dependencesAreBlack_(Node& node, ColorDictionary& graphColor) {
  bool statement = true;
  for (auto& index : node.nodeDependences) {
    statement = (graphColor[index] != BLACK) ? false : true;
  }

  return statement;
}

void deepSearchSorting_(ColorDictionary& graphColor, std::vector<Node>& newGraph,
                        std::vector<Node>& oldGraph, Node& currentNode) {
  if (graphColor[currentNode.nodeId] == GRAY) {
    throw "cicle found";
  }

  if (graphColor[currentNode.nodeId] == BLACK) {
    return;
  }

  if (currentNode.nodeDependences.empty() ||
      dependencesAreBlack_(currentNode, graphColor)) {
    graphColor.insert_or_assign(currentNode.nodeId, BLACK);
    newGraph.push_back(currentNode);
    return;
  }

  graphColor.insert_or_assign(currentNode.nodeId, GRAY);
  for (auto& dependenceIndex : currentNode.nodeDependences) {
    Node& dependenceNode = nodeById_(oldGraph, dependenceIndex);
    deepSearchSorting_(graphColor, newGraph, oldGraph, dependenceNode);
  }

  graphColor.insert_or_assign(currentNode.nodeId, BLACK);
  newGraph.push_back(currentNode);
}

}  // namespace

void sortGraph(std::vector<Node>& graph) {
  auto inversedGraph = inverse_(graph);

  std::vector<int> leafNodesId;
  for (auto& node : inversedGraph) {
    if (node.nodeDependences.size() == 0) {
      leafNodesId.push_back(node.nodeId);
    }
  }

  std::vector<Node> newGraph;
  std::map<int, int> graphColor;
  newGraph.reserve(graph.size());

  for (auto& node : graph) {
    graphColor.insert(NodeColor(node.nodeId, WHITE));
  }

  for (auto& leaf : leafNodesId) {
    deepSearchSorting_(graphColor, newGraph, graph, nodeById_(graph, leaf));
  }

  graph = newGraph;
}

int main() {
  Node node_0 = {0, std::vector<int>{}};
  Node node_1 = {1, std::vector<int>{0}};
  Node node_2 = {2, std::vector<int>{0, 1, 5}};
  Node node_3 = {3, std::vector<int>{1, 2}};
  Node node_5 = {5, std::vector<int>{}};
  Node node_4 = {4, std::vector<int>{3, 2}};
  Node node_6 = {6, std::vector<int>{2, 5, 3}};

  std::vector<Node> graph = {node_2, node_1, node_3, node_0,
                             node_5, node_4, node_6};

  sortGraph(graph);

  std::cout << "done\n";
}