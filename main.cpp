#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>

struct Node {
  int nodeId;
  std::vector<int> nodeDependences;
};

namespace {
typedef std::unordered_map<int, int> ColorDictionary;
typedef std::pair<int, const int> NodeColor;

enum TarjanColor { WHITE = 0, BLACK = 1, GRAY = 2 };

Node &nodeById_(std::vector<Node> &graph, int nodeId) {
  for (auto &node : graph) {
    if (node.nodeId == nodeId) {
      return node;
    }
  }
}

std::vector<Node> inverse_(const std::vector<Node> &graph) {
  std::vector<Node> inversedGraph = graph;
  for (auto &node : inversedGraph) {
    node.nodeDependences.clear();
  }

  for (auto &node : graph) {
    for (auto &dependenceId : node.nodeDependences) {
      Node &insertNode = nodeById_(inversedGraph, dependenceId);
      insertNode.nodeDependences.push_back(node.nodeId);
    }
  }

  return inversedGraph;
}

bool dependencesAreBlack_(Node &node, ColorDictionary &graphColor) {
  bool statement = true;
  for (auto &index : node.nodeDependences) {
    if (graphColor[index] != BLACK) {
      statement = false;
    }
  }

  return statement;
}

bool nodeIsEdge_(Node &node, ColorDictionary &graphColor) {
  return (node.nodeDependences.empty() ||
          dependencesAreBlack_(node, graphColor));
}

bool nodesAreBlack_(std::vector<Node> &nodes, ColorDictionary &graphColor) {
  bool statement = true;
  for (auto &node : nodes) {
    if (not graphColor[node.nodeId] == BLACK) {
      statement = false;
    }
  }
  return statement;
}

void deepSearchEdges_(ColorDictionary &graphColor, std::vector<Node> &newGraph,
                      std::vector<Node> &oldGraph, Node &currentNode) {
  if (graphColor[currentNode.nodeId] == GRAY) {
    throw "cicle found";
  }

  if (graphColor[currentNode.nodeId] == BLACK) {
    return;
  }

  if (nodeIsEdge_(currentNode, graphColor)) {
    auto state = std::find_if(
        newGraph.begin(), newGraph.end(), [&currentNode](Node node) {
          return (node.nodeId == currentNode.nodeId) ? true : false;
        });
    if (state == newGraph.end()) {
      newGraph.push_back(currentNode);
    }
    return;
  }

  graphColor[currentNode.nodeId] = GRAY;
  for (auto &dependenceIndex : currentNode.nodeDependences) {
    deepSearchEdges_(graphColor, newGraph, oldGraph,
                     nodeById_(oldGraph, dependenceIndex));
  }
  graphColor[currentNode.nodeId] = WHITE;
}

}  // namespace

void sortGraph(std::vector<Node> &graph) {
  auto inversedGraph = inverse_(graph);
  std::vector<Node> edgeNodes;
  for (int i = 0; i < inversedGraph.size(); ++i) {
    if (inversedGraph[i].nodeDependences.size() == 0) {
      edgeNodes.push_back(graph[i]);
    }
  }

  std::vector<Node> newGraph;
  ColorDictionary graphColor;

  for (auto &node : graph) {
    graphColor.insert(NodeColor(node.nodeId, WHITE));
  }

  while (not nodesAreBlack_(edgeNodes, graphColor)) {
    for (auto &node : edgeNodes) {
      deepSearchEdges_(graphColor, newGraph, graph, node);
    }
    for (auto &node : newGraph) {
      graphColor[node.nodeId] = BLACK;
    }
  }

  graph = newGraph;
}

int main() {
  Node node_0 = {0, std::vector<int>{}};
  Node node_1 = {1, std::vector<int>{0}};
  Node node_2 = {2, std::vector<int>{0, 1, 5}};
  Node node_3 = {3, std::vector<int>{1, 2}};
  Node node_5 = {5, std::vector<int>{7}};
  Node node_4 = {4, std::vector<int>{3, 2, 8}};
  Node node_6 = {6, std::vector<int>{2, 5, 3}};
  Node node_7 = {7, std::vector<int>{}};
  Node node_8 = {8, std::vector<int>{7}};

  // Node node_0 = {0, std::vector<int>{}};
  // Node node_1 = {1, std::vector<int>{0}};
  // Node node_2 = {2, std::vector<int>{1}};
  // Node node_3 = {3, std::vector<int>{2}};
  // Node node_5 = {5, std::vector<int>{1, 6}};
  // Node node_4 = {4, std::vector<int>{1}};
  // Node node_6 = {6, std::vector<int>{3, 1}};

  std::vector<Node> graph = {node_3, node_1, node_2, node_0, node_5,
                             node_4, node_6, node_7, node_8};

  sortGraph(graph);

  std::cout << "done\n";
}