#include "route_planner.h"
#include <algorithm>
using std::sort;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.

    //HenryT
    // :m_Model(model) initialized m_Model, Question what's the scope of this m_Model
    RouteModel::Node start_node_tmp = m_Model.FindClosestNode(start_x, start_y);
    RouteModel::Node end_node_tmp = m_Model.FindClosestNode(end_x, end_y);
    this->start_node = &start_node_tmp;
    this->end_node  = &end_node_tmp;

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    //std::cout<<"this node X: "<< node->x << " Y: " << node->y << "\n";
    //std::cout<<"End node X: "<< this->end_node->x <<" Y: " << this->end_node->y <<"\n";
    //std::cout << "Distance: " << 10000*this->end_node->distance(*node) <<"\n";
    return node->distance(*this->end_node);
    //return this->end_node->distance(*node);  // question why is this wrong
}

// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors(); // findNeighbors add nodes to node::neighbors
    for (RouteModel::Node *node : current_node->neighbors){
        node->parent = current_node;
        node->h_value = CalculateHValue(node);
        node->g_value = current_node->g_value + node->distance(*current_node); // TODO double check g value definition
        node->visited = true;
        this->open_list.push_back(node);

    }



}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.
bool RoutePlanner::Compare(RouteModel::Node *node1, RouteModel::Node *node2){
    float f1 = node1->h_value + node1->g_value;
    float f2 = node2->h_value + node2->g_value;
    return f1 > f2;
}
RouteModel::Node *RoutePlanner::NextNode() {
    sort(this->open_list.begin(), this->open_list.end(),Compare);
    RouteModel::Node *next = this->open_list.front();
    this->open_list.erase(open_list.begin());
    return next;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
    while (current_node != this->start_node){
        path_found.push_back(*current_node);
        distance += current_node->distance(*current_node->parent); // derefence
         current_node = current_node-> parent;
    }
    std::reverse(std::begin(path_found),std::end(path_found));
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    std::cout << "route_planner.cpp 114";
    RouteModel::Node *current_node = this->start_node;
    this ->AddNeighbors(current_node);
    // TODO: Implement your solution here.
    std::cout << "route_planner.cpp 117";
    while (this->open_list.size() > 0){

        
        current_node = this->NextNode();
        if (current_node = this->end_node){
            break;
        }
        this->AddNeighbors(current_node);
    }

    std::vector<RouteModel::Node> path_found = this->ConstructFinalPath(current_node);

}